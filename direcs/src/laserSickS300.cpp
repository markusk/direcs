/*************************************************************************
*   Copyright (C) 2010 by Markus Knapp                                  *
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


SickS300::SickS300()
{
	port_rate = DEFAULT_LASER_RATE;		// TODO: put these settings to the ini-file
	device_name = DEFAULT_LASER_PORT;	// TODO: put these settings to the ini-file
//	read_mode = LASER_CONTINUOUS_MODE;	// TODO: put these settings to the ini-file
	read_mode = LASER_REQUEST_MODE;		// TODO: put these settings to the ini-file
	
	
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

	qDebug("Serial port configured.");

	// change term speed
	if (changeTermSpeed(port_rate) != 0)
	{
		qDebug("Error chaning serial port speed to %d baud.", port_rate);
		return -1;
	}

	qDebug("Serial port to SICK S300 opened and port speed changed.");
	qDebug("Now initializing the laser (get token etc.)...");

	// Initialise the laser scanner communication (Get token, wait for answer)
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


int SickS300::init()
{
	const unsigned char get_token_buf[]={0x00,0x00,0x41,0x44,0x19,0x00,0x00,0x05,0xFF,0x07,0x19,0x00,0x00,0x05,0xFF,0x07,0x07,0x0F,0x9F,0xD0}; // siehe Seite 14 "Get Token"
	const unsigned char read_data_buf[]={0x00,0x00,0x45,0x44,0x0c,0x00,0x02,0xfe,0xff,0x07}; // siehe Seite 14 "Read Scandata (block 12)"
	
	unsigned char buffer[LASER_MAX_BUFFER_SIZE];
	int n_count = 0;
	
	/* org:
	data.min_angle=-5*M_PI/180;
	data.max_angle=180*M_PI/180; // 190 degrees: angle from -5 to 185
	data.resolution=0.5*M_PI/180;
	*/
	
	// new:
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
		qDebug("Requesting token from laser because we're in the request mode...");
		n_count = write(laser_fd, get_token_buf, 20);
		
		if (n_count != 20)
		{
			qDebug("error sending request token");
			return -1;
		}
		
		qDebug("Sending token request OKAY.");
		qDebug("Now waiting for answer from laser (should be 00 00 00 00)...");

		n_count = readBytes(buffer, 4);
		
		if (n_count != 4)
		{
			qDebug("Error getting request token. Number of received bytes was %d instead of 4.", n_count);
			return -1;
		}

		if (buffer[0]!=0x00 && buffer[1]!=0x00 && buffer[2]!=0x00 && buffer[3]!=0x00)
		{
			qDebug("Error getting request token. Received 4 bytes were not 00 00 00 00.");
			return -1;
		}

		qDebug("Got correct answer / the token. :-)");
	}

	
	// ********************************************************************
	// ********************************************************************
	// ********************************************************************
	/// FIXME: this is a first test
	// try to read some data
	// ********************************************************************
	// ********************************************************************
	// ********************************************************************

	
/* FIXME: make a nice thread / loop of that
	// The main loop; interact with the device here
	for(;;)
	{
*/
		if (read_mode==LASER_CONTINUOUS_MODE)
		{
			if (readContinuousTelegram(data.ranges)<0)
			{
				qDebug("error reading continuous telegram");
			}
			else
			{
				qDebug("Continuous telegram read successfully! :-)");
			}
		}
		else // working in request mode
		{
			// Laser command:
			// Read scan data (Block 12)
			n_count = write(laser_fd, read_data_buf, 10);
			
			if (n_count!=10)
			{
				qDebug("Error requesting scan data from laser");
			}
			
			if (readRequestTelegram(data.ranges) < 0)
			{
				qDebug("error reading request telegram");
			}
			else
			{
				// Make data available
				// TODO: emit the data!   // this->Publish(device_addr, NULL, PLAYER_MSGTYPE_DATA, PLAYER_LASER_DATA_SCAN, (void*)&data, sizeof(data), NULL);
				qDebug("Request telegram read successfully! :-)");
			}
		}
/*
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


// org: ssize_t SickS300::readBytes(int fd, unsigned char *buf, size_t nChars)
int SickS300::readBytes(unsigned char *buf, int nChars)
{
	/* org:
	size_t i = 0;
	int res = 0;
	int b = 0;


	for (i=0; i < nChars; i++)
	{
		b = read(fd, buf+i, 1); // FIXME: hier hängt es auf dem Mac !
		
		if (b != 1)
		{
			return b;
		}
		
		res++;
	}

	return res;
	*/

	// - - - - - - code from readAtmelPort from here:  - - - - - - - - -
	int amountRead = 0, bytes_read = 0;
	struct timeval t;
	fd_set set;
	int err = -1;

	while (nChars > 0)
	{
		t.tv_sec = 0;
		t.tv_usec = READ_TIMEOUT;
		FD_ZERO(&set);
		FD_SET(laser_fd, &set);

		err = select(laser_fd + 1, &set, NULL, NULL, &t);
		if (err == 0)
		{
			return -2;
		}

		// read from the serial device
		amountRead = read(laser_fd, buf, nChars);

		if(amountRead < 0 && errno != EWOULDBLOCK)
		{
			return -1;
		}
		else
		{
			if(amountRead > 0)
			{
				bytes_read += amountRead;
				nChars -= amountRead;
				buf += amountRead;
			}
		}
	}

	return bytes_read;
}


int SickS300::openTerm()
{
#ifndef Q_OS_WIN32 // currently supported under linux and MAC OS (no Windoze at the moment)
	struct termios options;


	qDebug("Opening serial port %s...", (char*) device_name);
	laser_fd = open(device_name, O_RDWR | O_NOCTTY | O_NONBLOCK); // TODO: check if this works under Linux!!
	
	if (laser_fd < 0)
	{
		qDebug("unable to open serial port [%s]; [%s]", (char*) device_name, strerror(errno));
		return -1;
	}

	// Note that open() follows POSIX semantics: multiple open() calls to
	// the same file will succeed unless the TIOCEXCL ioctl is issued.
	// This will prevent additional opens except by root-owned processes.
	// See tty(4) ("man 4 tty") and ioctl(2) ("man 2 ioctl") for details.
	if (ioctl(laser_fd, TIOCEXCL) == -1)
	{
		qDebug("Error setting TIOCEXCL on /dev/tty. ... - %s(%d).\n", strerror(errno), errno);
		return -1;
	}

	// Now that the device is open, clear the O_NONBLOCK flag so
	// subsequent I/O will block.
	// See fcntl(2) ("man 2 fcntl") for details.
	if (fcntl(laser_fd, F_SETFL, 0) == -1)
	{
		qDebug("Error clearing O_NONBLOCK - %s(%d).\n", strerror(errno), errno);
		return -1;
	}

	// Get current port settings
	if (tcgetattr(laser_fd, &options) < 0)
	{
		qDebug("Unable to get serial port attributes");
		return -1;
	}

	// set the serial port speed to 9600 to match the laser
	// later we can ramp the speed up to the SICK's 38K
	// cfmakeraw( &options ); // TODO: check if that is needed
	cfsetispeed(&options, B9600);
	cfsetospeed(&options, B9600);

	// The CLOCAL setting is needed for MAC OS X 10.6 !!
	options.c_cflag |= CLOCAL;		 	// Local line - do not change "owner" of port. @sa: http://www.easysw.com/~mike/serial/serial.html#3_1_1

	// 8N1
	options.c_cflag &= ~PARENB;
	options.c_cflag &= ~CSTOPB;
	options.c_cflag &= ~CSIZE;
	options.c_cflag |= CS8;

	// Disable hardware flow control:
	options.c_cflag &= ~CRTSCTS;

	// Make sure queue is empty
	// tcflush(laser_fd, TCIOFLUSH);

	// Cause the new options to take effect immediately.
	if (tcsetattr(laser_fd, TCSAFLUSH, &options ) < 0)
	{
		qDebug("1, Unable to set serial port attributes");
		return -1;
	}
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
	n_count = readBytes(buffer, 1); // FIXME: we have an error here, when using continous mode on the Mac. *****************************************************
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
		n_count = readBytes(buffer, 1);
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
	n_count = readBytes(buffer, 2);
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
	n_count = readBytes(buffer, 1);
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
	n_count = readBytes(buffer, 1);
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
	n_count = readBytes(buffer, 2);
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
	n_count = readBytes(buffer, 2);
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
	n_count = readBytes(buffer, 4);
	if (n_count==4)
	{
		telegram_size-=4;
	}
	else
	{
		return -1;
	}
	
	/* telegram number */
	n_count = readBytes(buffer, 2);
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
	n_count = readBytes(buffer, 4);
	if (n_count!=4)
	{
		return -1;
	}
	
	telegram_size-=4;

	/* read data */
	// cout << "reading data: " << telegram_size-2 << " bytes, " << (telegram_size-2)/2 << " samples" << endl;
	n_count = readBytes(buffer, telegram_size-2);
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
	n_count = readBytes(buffer, 2);
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


	qDebug("READING answer header");

	// read answer header from laser, 4 bytes of 0x00
	// (this is the anssaer from the last command before calling this method!)
	n_count = readBytes(buffer, 4);

	// check if answer is right (4x 0x00)
	if (n_count!=4 || (strncmp((const char *)buffer,header_start_buff,4)!=0))
	{
		qDebug("ERROR: Laser did not answer 0x00 0x00 0x00 0x00.)");
		return -1;
	}

	qDebug("Laser answer was 00 00 00. So it was OKAY. :-)");
	

	qDebug("Now reading repeated header...");

	// read repeated header from laser, 6 bytes (0C 00 02 FE FF 07)
	n_count = readBytes(buffer, 6);// FIXME: we have an error here, when using request mode on the Mac. *****************************************************
	if (n_count!=6)
	{
		qDebug("Error reading repeated header.");
		return -1;
	}

	qDebug("Now reading monitoring data...");

	/* block 12 monitoring data */
	qDebug("READING monitoring data");
	n_count = readBytes(buffer, 2);
	if (n_count!=2)
	{
		qDebug("monitoring data expected");
		return -1;
	}


	/* read data */
	n_count = readBytes(buffer, 1522);
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
	n_count = readBytes(buffer, 2);
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
