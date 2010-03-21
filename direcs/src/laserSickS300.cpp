/*************************************************************************
*   Copyright (C) 2009 by Markus Knapp                                  *
*   www.direcs.de                                                       *
*                                                                       *
*   This file is part of direcs.                                        *
*                                                                       *
*   direcs is free software: you can redistribute it and/or modify it   *
*   under the terms of the GNU General Public License as published      *
*   by the Free Software Foundation, version 3 of the License.          *
*                                                                       *
*   direcs is distributed in the hope that it will be useful,           *
*   but WITHOUT ANY WARRANTY; without even the implied warranty of      *
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the        *
*   GNU General Public License for more details.                        *
*                                                                       *
*   You should have received a copy of the GNU General Public License   *
*   along with direcs. If not, see <http://www.gnu.org/licenses/>.      *
*                                                                       *
*************************************************************************/

#include "laserSickS300.h"

/* mk not needed?
Driver* SickS3000_Init(ConfigFile* cf, int section)
{
	// Create and return a new instance of this driver
	return((Driver*)(new SickS3000(cf, section)));
}

void SickS3000_Register(DriverTable* table)
{
	table->AddDriver("sicks3000", SickS3000_Init);
}
*/


// org1: SickS3000::SickS3000(ConfigFile* cf, int section) : Driver(cf, section, true, PLAYER_MSGQUEUE_DEFAULT_MAXLEN, PLAYER_LASER_CODE)
// org2: SickS3000::SickS3000(ConfigFile* cf, int section)
SickS300::SickS300()
{
	port_rate = DEFAULT_LASER_RATE;		// TODO: put these settings to the ini-file
	device_name = DEFAULT_LASER_PORT;	// TODO: put these settings to the ini-file
	read_mode = LASER_CONTINUOUS_MODE;	// TODO: put these settings to the ini-file
//	read_mode = LASER_REQUEST_MODE;		// TODO: put these settings to the ini-file
	
	
	if (read_mode == LASER_CONTINUOUS_MODE)
	{
		qDebug("7, continuous mode output");
	}
	else
	{
		if (read_mode == LASER_REQUEST_MODE)
		{
			qDebug("7, request mode output");
		}
		else
		{
			qDebug("unknown read mode");
		}
	}

	qDebug() << "Laserscanner SICK S300 started";
	qDebug() << "Reading mode:" << read_mode << "(" << LASER_CONTINUOUS_MODE << "continuous mode;" << LASER_REQUEST_MODE << "request mode)";
	qDebug() << "Device:" << device_name;
	qDebug() << "Baudrate:" << port_rate;
}


SickS300::~SickS300()
{
	// Shutting laserscanner down
	shutdown();
}


int SickS300::setup()
{
	qDebug("Initialising Sick S300...");

	// Here you do whatever is necessary to setup the device, like open and
	// configure a serial port.
	if (openTerm() < 0)
	{
		qDebug("Error opening serial port for S300");
		return -1;
	}

	// change term speed
	if (changeTermSpeed(port_rate) != 0)
		return -1;

	qDebug("Sick S300 ready");

	// Initialise the laser scanner communication
	return init();
	
	// Start the device thread; spawns a new thread and executes
	// SickS300::Main(), which contains the main loop for the driver. Renamed to 'init()'
	// FIXME: StartThread();
}


int SickS300::shutdown()
{
	qDebug("Shutting laserscanner SICK S300 down");
	
	// Stop and join the driver thread
	// FIXME: StopThread();
	
	sleep(1);
	
	closeTerm();
	
	qDebug("SICK S300 has been shutdown");
	
	return 0;
}


int SickS300::closeTerm()
{
/* REMOVE
#ifdef HAVE_HI_SPEED_SERIAL
	if (ioctl(laser_fd, TIOCSSERIAL, &old_serial) < 0)
	{
		//RETURN_ERROR(1, "error trying to reset serial to old state");
		qDebug("ioctl() failed while trying to reset serial to old state");
	}
#endif
*/
	
	close(laser_fd);
	return 0;
}


int SickS300::changeTermSpeed(int speed)
{
#ifndef Q_OS_WIN32 // currently supported under linux and MAC OS (no Windoze at the moment)
	struct termios term;
#else
	Q_UNUSED(speed);
#endif

#ifndef Q_OS_WIN32 // currently supported under linux and MAC OS (no Windoze at the moment)
#ifdef HAVE_HI_SPEED_SERIAL
	struct serial_struct serial;

	// we should check and reset the AYSNC_SPD_CUST flag
	// since if it's set and we request 38400, we're likely
	// to get another baud rate instead (based on custom_divisor)
	// this way even if the previous call doesn't reset the
	// port correctly, we'll end up with the right speed we want
	if (ioctl(laser_fd, TIOCGSERIAL, &serial) < 0) 
	{
		//RETURN_ERROR(1, "error on TIOCGSERIAL in beginning");
		qDebug("ioctl() failed while trying to get serial port info");
		return -1;
	}
	else
	{
		serial.flags &= ~ASYNC_SPD_CUST;
		serial.custom_divisor = 0;
		
		if (ioctl(laser_fd, TIOCSSERIAL, &serial) < 0) 
		{
			//RETURN_ERROR(1, "error on TIOCSSERIAL in beginning");
			qDebug("ioctl() failed while trying to set serial port info");
			return -1;
		}
	}
#endif

	qDebug("LASER: change TERM speed: %d", speed);

	switch(speed)
	{
		case 9600:
			//PLAYER_MSG0(2, "terminal speed to 9600");
			if( tcgetattr( laser_fd, &term ) < 0 )
			{
				qDebug("unable to get device attributes");
				return -1;
			}
	
			cfmakeraw( &term );
			cfsetispeed( &term, B9600 );
			cfsetospeed( &term, B9600 );
	
			if( tcsetattr( laser_fd, TCSAFLUSH, &term ) < 0 )
			{
				qDebug("unable to set device attributes");
				return -1;
			}
			break;

		case 19200:
			//PLAYER_MSG0(2, "terminal speed to 9600");
			if( tcgetattr( laser_fd, &term ) < 0 )
			{
				qDebug("unable to get device attributes");
				return -1;
			}
	
			cfmakeraw( &term );
			cfsetispeed( &term, B19200 );
			cfsetospeed( &term, B19200 );
	
			if( tcsetattr( laser_fd, TCSAFLUSH, &term ) < 0 )
			{
				qDebug("unable to set device attributes");
				return -1;
			}
			break;

		case 38400:
			//PLAYER_MSG0(2, "terminal speed to 38400");
			if( tcgetattr( laser_fd, &term ) < 0 )
			{
				qDebug("unable to get device attributes");
				return -1;
			}

			cfmakeraw( &term );
			cfsetispeed( &term, B38400 );
			cfsetospeed( &term, B38400 );

			if( tcsetattr( laser_fd, TCSAFLUSH, &term ) < 0 )
			{
				qDebug("unable to set device attributes");
				return -1;
			}
			break;

		case 115200: // only Sick S300 !
			//PLAYER_MSG0(2, "terminal speed to 115200");
			if( tcgetattr( laser_fd, &term ) < 0 )
			{
				qDebug("unable to get device attributes");
				return -1;
			}

			cfmakeraw( &term );
			cfsetispeed( &term, B115200 );
			cfsetospeed( &term, B115200 );

			if( tcsetattr( laser_fd, TCSAFLUSH, &term ) < 0 )
			{
				qDebug("unable to set device attributes");
				return -1;
			}
			break;

		case 230400: // only Sick S300 !
			//PLAYER_MSG0(2, "terminal speed to 230400");
			if( tcgetattr( laser_fd, &term ) < 0 )
			{
				qDebug("unable to get device attributes");
				return -1;
			}

			cfmakeraw( &term );
			cfsetispeed( &term, B230400 );
			cfsetospeed( &term, B230400 );

			if( tcsetattr( laser_fd, TCSAFLUSH, &term ) < 0 )
			{
				qDebug("unable to set device attributes");
				return -1;
			}
			break;

		case 500000:
			//PLAYER_MSG0(2, "terminal speed to 500000");
#endif

#ifndef Q_OS_WIN32 // currently supported under linux and MAC OS (no Windoze at the moment)
#ifdef HAVE_HI_SPEED_SERIAL
			if (ioctl(laser_fd, TIOCGSERIAL, &old_serial) < 0)
			{
				qDebug("error on TIOCGSERIAL ioctl");
				return -1;
			}

			serial = old_serial;

			serial.flags |= ASYNC_SPD_CUST;
			serial.custom_divisor = 48; // for FTDI USB/serial converter divisor is 240/5

			if (ioctl(laser_fd, TIOCSSERIAL, &serial) < 0)
			{
				qDebug("error on TIOCSSERIAL ioctl");
				return -1;
			}

#else
			qDebug() << stderr << "sicklms200: Trying to change to 500kbps, but no support compiled in, defaulting to 38.4kbps.";
#endif

			// even if we are doing 500kbps, we have to set the speed to 38400...
			// the driver will know we want 500000 instead.

			if( tcgetattr( laser_fd, &term ) < 0 )
			{
				qDebug("unable to get device attributes");
				return -1;
			}

			cfmakeraw( &term );
			cfsetispeed( &term, B38400 );
			cfsetospeed( &term, B38400 );

			if( tcsetattr( laser_fd, TCSAFLUSH, &term ) < 0 )
			{
				qDebug("unable to set device attributes");
				return -1;
			}

			break;
		default:
			qDebug("unknown speed %d", speed);
			return -1;
	}
	return 0;
#else
	return -1;
#endif
}


/* mk: FIXME: später...
// org: int SickS3000::processMessage(MessageQueue* resp_queue, player_msghdr * hdr, void * data)
int SickS300::processMessage(player_msghdr * hdr, void * data)
{
	// Process messages here.  Send a response if necessary, using Publish().
	// If you handle the message successfully, return 0.  Otherwise,
	// return -1, and a NACK will be sent for you, if a response is required.
	qDebug("7, ProcessMessage: some message arrived");

	assert(hdr);
	assert(data);

	if(Message::MatchMessage(hdr, PLAYER_MSGTYPE_REQ, PLAYER_LASER_REQ_SET_CONFIG, device_addr))
	{
		qDebug("7, ProcessMessage: message type:  PLAYER_LASER_REQ_SET_CONFIG");
		return 0;
	}

	if(Message::MatchMessage(hdr, PLAYER_MSGTYPE_REQ, PLAYER_LASER_REQ_GET_CONFIG, device_addr))
	{
		qDebug("7, ProcessMessage: message type: PLAYER_LASER_REQ_GET_CONFIG");
		return 0;
	}

	if(Message::MatchMessage(hdr, PLAYER_MSGTYPE_REQ, PLAYER_LASER_REQ_GET_GEOM, device_addr))
	{
		qDebug("7, ProcessMessage: message type:  PLAYER_LASER_REQ_GET_GEOM ");
		player_laser_geom_t geom;
		geom.pose.px = 0.0;
		geom.pose.py = 0.0;
		geom.pose.pa = 
		//355*M_PI/180;
		0.0;
		geom.size.sl = 0.25;
		geom.size.sw = 0.25;
	
		// mk: to ged rid of the MessageQueue. MessageQueue is a class defined in player-3.0.0/libplayercore/message.h
		// FIXME: publish to where now? emmit?
		this->Publish(device_addr, resp_queue, PLAYER_MSGTYPE_RESP_ACK, PLAYER_LASER_REQ_GET_GEOM, (void*)&geom, sizeof(geom), NULL);
		
		return 0;
	}

	return -1;
}
*/


int SickS300::init()
{
	const unsigned char get_token_buf[]={0x00,0x00,0x41,0x44,0x19,0x00,0x00,0x05,0xFF,0x07,0x19,0x00,0x00,0x05,0xFF,0x07,0x07,0x0F,0x9F,0xD0};
	const unsigned char read_data_buf[]={0x00,0x00,0x45,0x44,0x0c,0x00,0x02,0xfe,0xff,0x07};
	
	unsigned char buffer[LASER_MAX_BUFFER_SIZE];
	int n_count;
	
	/* mk org:
	data.min_angle=-5*M_PI/180;
	data.max_angle=180*M_PI/180; // 190 degrees: angle from -5� to 185�
	data.resolution=0.5*M_PI/180;
	// mk end */
	
	// mk new:
	data.min_angle=-45*M_PI/270;
	data.max_angle=225*M_PI/270; // 270 degrees: angle from -45 deg to 225 deg
	data.resolution=0.5*M_PI/270;
	
	data.max_range=5.0;
	data.ranges_count=DEFAULT_LASER_SAMPLES;
	
	data.intensity_count=0; /* do not know this */
	data.id=0;

	if (read_mode==LASER_REQUEST_MODE)
	{
		/* request token */
		qDebug("Requesting token because we're in the request mode...");
		n_count=write(laser_fd,get_token_buf,20);
		
		if (n_count!=20)
		{
			qDebug("error sending request token");
			return -1;
		}
		
		qDebug("7, token requested");
		n_count = readBytes(laser_fd,buffer,4);
		
		if (n_count!=4 || (buffer[0]!=0x00 && buffer[1]!=0x00 && buffer[2]!=0x00 && buffer[3]!=0x00))
		{
			qDebug("error getting request token");
			return -1;
		}
		else
		{
			qDebug("7, got token");
		}
	}
	
	
	/// FIXME: this is a first test
	// try to read some data

	
/* FIXME: uncomment this
	// The main loop; interact with the device here
	for(;;)
	{
		// test if we are supposed to cancel
		pthread_testcancel();
		
		// Process incoming messages.  SickS300::ProcessMessage() is
		// called on each message.
		// mk FIXME: später ProcessMessages();
*/
		// Interact with the device, and push out the resulting data, using
		// Driver::Publish()
		if (read_mode==LASER_CONTINUOUS_MODE)
		{
			if (readContinuousTelegram(data.ranges)<0)
			{
				qDebug("error reading continuous telegram");
			}
			else
			{
				// FIXME: Make data available
				// this->Publish(device_addr, NULL, PLAYER_MSGTYPE_DATA, PLAYER_LASER_DATA_SCAN, (void*)&data, sizeof(data), NULL);
				qDebug("### continuous telegram read successfully! :-) ###");
			}
		}
		else // working on request mode
		{
			// request scan data (block 12)
			n_count=write(laser_fd,read_data_buf,10);
			
			if (n_count!=10)
			{
				qDebug("error requesting scan data");
			}
			
			if (readRequestTelegram(data.ranges)<0)
			{
				qDebug("error reading request telegram");
			}
			else
			{
				// Make data available
				// mk FIXME: später	    this->Publish(device_addr, NULL, PLAYER_MSGTYPE_DATA, PLAYER_LASER_DATA_SCAN, (void*)&data, sizeof(data), NULL);
				qDebug("### request telegram read successfully! :-) ###");
			}
		}
/* mk	
		data.id++;
	
		// Sleep (you might, for example, block on a read() instead)
		if (read_mode==LASER_CONTINUOUS_MODE)
		{
			usleep(10000);
		}
	} // for(;;)
mk */
		return 0; // TODO: check if this is correct!
}


ssize_t SickS300::readBytes(int fd, unsigned char *buf, size_t count)
{
	size_t i;
	int res,b;


	res=0;
	
	for(i=0;i<count;i++)
	{
		b=read(fd,buf+i,1);
		
		if(b!=1)
		{
			return b;
		}
		
		res++;
	}

	return res;
}


int SickS300::openTerm()
{
#ifndef Q_OS_WIN32 // currently supported under linux and MAC OS (no Windoze at the moment)
	qDebug("Opening serial port %s...", (char*) device_name);
	laser_fd = open(device_name, O_RDWR | O_SYNC , S_IRUSR | S_IWUSR );
	
	if (laser_fd < 0)
	{
		qDebug("unable to open serial port [%s]; [%s]", (char*) device_name, strerror(errno));
		return -1;
	}

	// set the serial port speed to 9600 to match the laser
	// later we can ramp the speed up to the SICK's 38K
	//
	struct termios term;
	if( tcgetattr( laser_fd, &term ) < 0 )
	{
		qDebug("Unable to get serial port attributes");
		return -1;
	}

	cfmakeraw( &term );
	cfsetispeed( &term, B9600 );
	cfsetospeed( &term, B9600 );

	if( tcsetattr( laser_fd, TCSAFLUSH, &term ) < 0 )
	{
		qDebug("1, Unable to set serial port attributes");
		return -1;
	}

	// Make sure queue is empty
	//
	tcflush(laser_fd, TCIOFLUSH);
#endif

	return 0;
}


int SickS300::readContinuousTelegram(float *ranges)
{
#ifndef Q_OS_WIN32 // currently supported under linux and MAC OS (no Windoze at the moment)
	unsigned char buffer[LASER_MAX_BUFFER_SIZE];
	int n_count;
	int i;
	int count_answer=0;
	bool parsed=false;
	bool parsing=false;
	int telegram_size;
	
	/* read header */
	i=0;
	count_answer=0;
	parsed=false;
	parsing=false;
	
	memset(buffer,0xFF,LASER_MAX_BUFFER_SIZE);
	
	qDebug("9, waiting for header ... ");
	
	/* parse first 6 bytes 
	* 0x00 0x00 0x00 0x00 0x00 0x00
	* 4 byte reply header
	* 2 byte block numbbre (0x00 0x00 for data output)
	*/
	n_count = readBytes(laser_fd,buffer,1);
	if (n_count<1)
	{
		qDebug("Could not read header");
		return -1;
	}
	
	if (buffer[0]==0x00)
	{
		i++;
		parsing=true;
	}
	
	count_answer++;
	
	while (n_count!=-1 && n_count!=0 && !parsed)
	{
		n_count = readBytes(laser_fd,buffer,1);
		count_answer++;
		if (buffer[0]==0x00)
		{
			i++;
			parsing=true;
		}
		else
		{
			i=0;
			parsing=false;
		}
		
		if(i==6)
		{
			parsed=true;
		}
	}
	
	/* header first 6 bytes received */
	qDebug("7,telegram start received");

	/* read size of telegram */
	telegram_size=0;
	n_count = readBytes(laser_fd,buffer,2);
	if (n_count==2)
	{
		telegram_size=(buffer[0] << 8) | buffer[1];
	}
	else
	{
		qDebug("error parsing: telegram size expected");
		return -1;
	}

	/*
	* still to read: (telegram_size*2) - 4 bytes
	*
	*/
	telegram_size=(telegram_size*2) - 4 ;

	/* remaining header */
	/* coordination flag */
	n_count = readBytes(laser_fd,buffer,1);
	if (n_count==1 && buffer[0]==0xFF)
	{
		telegram_size--;
	}
	else
	{
		qDebug("error parsing: coordination flag expected");
		return -1;
	}
	
	/* device code */
	n_count = readBytes(laser_fd,buffer,1);
	if (n_count==1 && buffer[0]==0x07)
	{
		telegram_size--;
	}
	else
	{
		qDebug("error parsing: device code expected");
		return -1;
	}
	
	/* protocol version */
	n_count = readBytes(laser_fd,buffer,2);
	if (n_count==2 && buffer[0]==0x02 && buffer[1]==0x01)
	{
		telegram_size-=2;
	}
	else
	{
		qDebug("error parsing: protocol version expected");
		return -1;
	}
	
	/* status */
	n_count = readBytes(laser_fd,buffer,2);
	if (n_count==2 && buffer[0]==0x00 && (buffer[1]==0x00 || buffer[1]==0x01))
	{
		telegram_size-=2;
	}
	else
	{
		qDebug("error parsing: status expected");
		return -1;
	}
	
	/* timestamp */
	n_count = readBytes(laser_fd,buffer,4);
	if (n_count==4)
	{
		telegram_size-=4;
	}
	else
	{
		return -1;
	}
	
	/* telegram number */
	n_count = readBytes(laser_fd,buffer,2);
	if (n_count==2)
	{
		telegram_size-=2;
	}
	else
	{
		return -1;
	}

	/* first 4 bytes of data unused 
	* bb bb flag
	* 11 11 id for measurement data from angular range 1
	*/
	n_count = readBytes(laser_fd,buffer,4);
	if (n_count!=4)
	{
		return -1;
	}
	
	telegram_size-=4;

	/* read data */
	// cout << "reading data: " << telegram_size-2 << " bytes, " << (telegram_size-2)/2 << " samples" << endl;
	n_count = readBytes(laser_fd,buffer,telegram_size-2);
	if (n_count!=telegram_size-2)
	{
		return -1;
	}

	/* fill data */
	for (i=0;i<DEFAULT_LASER_SAMPLES;i++)
	{
		ranges[i]=(float)(((buffer[2*i+1] & 0x1f)<<8) | buffer[2*i]);
		ranges[i] /= 100.0; /* original measure in cm , player works in meters */
	}

	//  telegram_size=2;
	telegram_size-=(telegram_size-2);

	/* read crc */
	n_count = readBytes(laser_fd,buffer,2);
	if (n_count==2)
	{
		telegram_size-=2;
	}
	else
	{
		qDebug("error parsing");
		return -1;
	}

	qDebug("9,continuous telegram succesfully received");

	return 0;
#else
	Q_UNUSED(ranges);

	return -1;
#endif
}


int SickS300::readRequestTelegram(float *ranges)
{
#ifndef Q_OS_WIN32 // currently supported under linux and MAC OS (no Windoze at the moment)
	const char header_start_buff[]={0x00,0x00,0x00,0x00};
	unsigned char buffer[1522];
	int n_count;
	int i;

	/* read answer header , 4 bytes of 0x00*/
	qDebug("READING answer header");
	n_count = readBytes(laser_fd, buffer, 4);

	// check if answer is right (4x 0x00)
	if (n_count!=4 || (strncmp((const char *)buffer,header_start_buff,4)!=0))
	{
		qDebug("header start expected (4x 0x00)");
		return -1;
	}
	qDebug("answer header was okay");
	
	/* read repeated header, 6 bytes */
	qDebug("READING repeated header");
	n_count = readBytes(laser_fd, buffer, 6);// FIXME: ths leads to an endless loop!!
	if (n_count!=6)
	{
		qDebug("repeated header expected");
		return -1;
	}

	/* block 12 monitoring data */
	qDebug("READING monitoring data");
	n_count = read(laser_fd,buffer,2);
	if (n_count!=2)
	{
		qDebug("monitoring data expected");
		return -1;
	}

	/* read data */
	n_count = readBytes(laser_fd,buffer,1522);
	if (n_count!=1522)
	{
		qDebug("insuficient number of bytes retrieved");
		return -1;
	}

	for (i=0;i<DEFAULT_LASER_SAMPLES;i++)
	{
		ranges[i]=(float)(((buffer[2*i+1] & 0x1f)<<8) | buffer[2*i]);
		ranges[i] /= 100.0; // original measure in cm , player works in meters -> FIXME convert back to cm!
	}

	/* read crc */
	n_count = readBytes(laser_fd,buffer,2);
	if (n_count!=2)
	{
		qDebug("crc code expected");
		return -1;
	}

	return 0;
#else
	Q_UNUSED(ranges);

	return -1;
#endif
}

/* mk not nedded?
extern "C"
{
	int player_driver_init(DriverTable* table)
	{
		qDebug("SickS300 driver initializing");
		SickS300_Register(table);
		qDebug("SickS300 driver done");
		
		return 0;
	}
}
*/
