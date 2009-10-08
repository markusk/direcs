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
SickS3000::SickS3000()
{
  const char *c_read_mode;

  // Read an option from the configuration file
  this->port_rate = cf->ReadInt(section, "rate", DEFAULT_LASER_RATE);
  this->device_name = cf->ReadString(section,"port",DEFAULT_LASER_PORT);
  c_read_mode = cf->ReadString(section,"read_mode",DEFAULT_LASER_MODE);

  if(strcasecmp(c_read_mode,"continuous")==0)
    {
      PLAYER_MSG0(7,"continuous mode output");
      read_mode=LASER_CONTINUOUS_MODE;
    }
  else
    if(strcasecmp(c_read_mode,"request")==0)
      {
	PLAYER_MSG0(7,"request mode output");
	read_mode=LASER_REQUEST_MODE;
      }
    else
      {
	PLAYER_ERROR("unknown read mode");
      }

  printf("Player SICK S300 started\n");
  printf("Reading mode: %d (%d continuous mode ; %d request mode)\n",read_mode,LASER_CONTINUOUS_MODE,LASER_REQUEST_MODE);
  printf("Device: %s\n",this->device_name); 
  printf("Baudrate: %d\n",this->port_rate);

  return;
}


int SickS3000::Setup()
{
  puts("Sick S3000 driver initialising");

  // Here you do whatever is necessary to setup the device, like open and
  // configure a serial port.
  if(this->OpenTerm()<0)
    {
      PLAYER_ERROR("Error opening serial port");
      return -1;
    }

  ChangeTermSpeed(this->port_rate);

  puts("Sick S3000 driver ready");

  // Start the device thread; spawns a new thread and executes
  // SickS3000::Main(), which contains the main loop for the driver.
  StartThread();

  return(0);
}


int SickS3000::Shutdown()
{
  puts("Shutting SickS3000 driver down");

  // Stop and join the driver thread
  StopThread();

  sleep(1);

  CloseTerm();

  puts("SickS3000 driver has been shutdown");

  return(0);
}


int SickS3000::CloseTerm()
{
  /* REMOVE
#ifdef HAVE_HI_SPEED_SERIAL
  if (ioctl(this->laser_fd, TIOCSSERIAL, &this->old_serial) < 0) {
    //RETURN_ERROR(1, "error trying to reset serial to old state");
    PLAYER_WARN("ioctl() failed while trying to reset serial to old state");
  }
#endif
  */

  ::close(this->laser_fd);
  return 0;
}


int SickS3000::ChangeTermSpeed(int speed)
{
  struct termios term;

#ifdef HAVE_HI_SPEED_SERIAL
  struct serial_struct serial;

  // we should check and reset the AYSNC_SPD_CUST flag
  // since if it's set and we request 38400, we're likely
  // to get another baud rate instead (based on custom_divisor)
  // this way even if the previous player doesn't reset the
  // port correctly, we'll end up with the right speed we want
  if (ioctl(this->laser_fd, TIOCGSERIAL, &serial) < 0) 
  {
    //RETURN_ERROR(1, "error on TIOCGSERIAL in beginning");
    PLAYER_WARN("ioctl() failed while trying to get serial port info");
  }
  else
  {
    serial.flags &= ~ASYNC_SPD_CUST;
    serial.custom_divisor = 0;
    if (ioctl(this->laser_fd, TIOCSSERIAL, &serial) < 0) 
    {
      //RETURN_ERROR(1, "error on TIOCSSERIAL in beginning");
      PLAYER_WARN("ioctl() failed while trying to set serial port info");
    }
  }
#endif

  //printf("LASER: change TERM speed: %d\n", speed);

  switch(speed)
  {
    case 9600:
      //PLAYER_MSG0(2, "terminal speed to 9600");
      if( tcgetattr( this->laser_fd, &term ) < 0 )
        RETURN_ERROR(1, "unable to get device attributes");
        
      cfmakeraw( &term );
      cfsetispeed( &term, B9600 );
      cfsetospeed( &term, B9600 );
        
      if( tcsetattr( this->laser_fd, TCSAFLUSH, &term ) < 0 )
        RETURN_ERROR(1, "unable to set device attributes");
      break;

    case 38400:
      //PLAYER_MSG0(2, "terminal speed to 38400");
      if( tcgetattr( this->laser_fd, &term ) < 0 )
        RETURN_ERROR(1, "unable to get device attributes");
        
      cfmakeraw( &term );
      cfsetispeed( &term, B38400 );
      cfsetospeed( &term, B38400 );
        
      if( tcsetattr( this->laser_fd, TCSAFLUSH, &term ) < 0 )
        RETURN_ERROR(1, "unable to set device attributes");
      break;

    case 500000:
      //PLAYER_MSG0(2, "terminal speed to 500000");

#ifdef HAVE_HI_SPEED_SERIAL    
      if (ioctl(this->laser_fd, TIOCGSERIAL, &this->old_serial) < 0) {
        RETURN_ERROR(1, "error on TIOCGSERIAL ioctl");
      }
    
      serial = this->old_serial;
    
      serial.flags |= ASYNC_SPD_CUST;
      serial.custom_divisor = 48; // for FTDI USB/serial converter divisor is 240/5
    
      if (ioctl(this->laser_fd, TIOCSSERIAL, &serial) < 0) {
        RETURN_ERROR(1, "error on TIOCSSERIAL ioctl");
      }
    
#else
      fprintf(stderr, "sicklms200: Trying to change to 500kbps, but no support compiled in, defaulting to 38.4kbps.\n");
#endif

      // even if we are doing 500kbps, we have to set the speed to 38400...
      // the driver will know we want 500000 instead.

      if( tcgetattr( this->laser_fd, &term ) < 0 )
        RETURN_ERROR(1, "unable to get device attributes");    

      cfmakeraw( &term );
      cfsetispeed( &term, B38400 );
      cfsetospeed( &term, B38400 );
    
      if( tcsetattr( this->laser_fd, TCSAFLUSH, &term ) < 0 )
        RETURN_ERROR(1, "unable to set device attributes");
    
      break;
    default:
      PLAYER_ERROR1("unknown speed %d", speed);
  }
  return 0;
}


/* mk: FIXME: später...
// org: int SickS3000::ProcessMessage(MessageQueue* resp_queue, player_msghdr * hdr, void * data)
int SickS3000::ProcessMessage(player_msghdr * hdr, void * data)
{
  // Process messages here.  Send a response if necessary, using Publish().
  // If you handle the message successfully, return 0.  Otherwise,
  // return -1, and a NACK will be sent for you, if a response is required.
  PLAYER_MSG0(7,"ProcessMessage: some message arrived");

  assert(hdr);
  assert(data);
  
  if(Message::MatchMessage(hdr, PLAYER_MSGTYPE_REQ, 
                           PLAYER_LASER_REQ_SET_CONFIG, 
                           this->device_addr))
  {
    PLAYER_MSG0(7,"ProcessMessage: message type:  PLAYER_LASER_REQ_SET_CONFIG");
    return 0;
  }

  if(Message::MatchMessage(hdr, PLAYER_MSGTYPE_REQ, 
                           PLAYER_LASER_REQ_GET_CONFIG, 
                           this->device_addr))
  {
    PLAYER_MSG0(7,"ProcessMessage: message type: PLAYER_LASER_REQ_GET_CONFIG");
    return 0;
  }

  if(Message::MatchMessage(hdr, PLAYER_MSGTYPE_REQ, 
                           PLAYER_LASER_REQ_GET_GEOM, 
                           this->device_addr))
  {
    PLAYER_MSG0(7,"ProcessMessage: message type:  PLAYER_LASER_REQ_GET_GEOM ");
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
	this->Publish(this->device_addr, resp_queue, PLAYER_MSGTYPE_RESP_ACK, PLAYER_LASER_REQ_GET_GEOM, (void*)&geom, sizeof(geom), NULL);
	
	return 0;
  }
  return -1;

}
*/

void SickS3000::Main()
{
  const unsigned char get_token_buf[]={0x00,0x00,0x41,0x44,0x19,0x00,0x00,0x05,0xFF,0x07,0x19,0x00,0x00,0x05,0xFF,0x07,0x07,0x0F,0x9F,0xD0};
  const unsigned char read_data_buf[]={0x00,0x00,0x45,0x44,0x0c,0x00,0x02,0xfe,0xff,0x07};

  player_laser_data_t data;
  unsigned char buffer[LASER_MAX_BUFFER_SIZE];
  int n_count;

  data.min_angle=-5*M_PI/180;
  data.max_angle=180*M_PI/180; /* 190 degrees: angle from -5� to 185� */
  data.resolution=0.5*M_PI/180;
  data.max_range=5.0;
  data.ranges_count=DEFAULT_LASER_SAMPLES;

  data.intensity_count=0; /* do not know this */
  data.id=0;

  if(read_mode==LASER_REQUEST_MODE)
    {
      /* request token */
      n_count=write(laser_fd,get_token_buf,20);
      if(n_count!=20)
	{
	  PLAYER_ERROR("error sending request token");
	}
      PLAYER_MSG0(7,"token requested");
      n_count=ReadBytes(laser_fd,buffer,4);
      if(n_count!=4 || (buffer[0]!=0x00 && buffer[1]!=0x00 && buffer[2]!=0x00 && buffer[3]!=0x00))
	PLAYER_ERROR("error getting request token");
      else
	PLAYER_MSG0(7,"got token");
    }

  // The main loop; interact with the device here
  for(;;)
  {
    // test if we are supposed to cancel
    pthread_testcancel();

    // Process incoming messages.  SickS3000::ProcessMessage() is
    // called on each message.
    ProcessMessages();

    // Interact with the device, and push out the resulting data, using
    // Driver::Publish()
    if(read_mode==LASER_CONTINUOUS_MODE)
      {
	if(ReadContinuousTelegram(data.ranges)<0)
	  PLAYER_ERROR("error reading continuous telegram");
	else
		; // mk
	  // FIXME: Make data available
		/*
	  this->Publish(this->device_addr, NULL, 
			PLAYER_MSGTYPE_DATA, PLAYER_LASER_DATA_SCAN,
			(void*)&data, sizeof(data), NULL);
		*/
      }

    else /* working on request mode */
      {
	/* request scan data (block 12) */
	n_count=write(laser_fd,read_data_buf,10);
	if(n_count!=10)
	  PLAYER_ERROR("error requesting scan data");
	if(ReadRequestTelegram(data.ranges)<0)
	  PLAYER_ERROR("error reading request telegram");
	else
	  {
	  // Make data available
	    this->Publish(this->device_addr, NULL, 
			  PLAYER_MSGTYPE_DATA, PLAYER_LASER_DATA_SCAN,
			  (void*)&data, sizeof(data), NULL);
	  }
      }

    data.id++;

    // Sleep (you might, for example, block on a read() instead)
    if(read_mode==LASER_CONTINUOUS_MODE)
      usleep(10000);
  }
}


ssize_t SickS3000::ReadBytes(int fd, unsigned char *buf, size_t count)
{
  size_t i;
  int res,b;

  res=0;
  for(i=0;i<count;i++)
    {
      b=read(fd,buf+i,1);
      if(b!=1)
	return b;
      res++;
    }

  return res;
}


int SickS3000::OpenTerm()
{
  this->laser_fd = ::open(this->device_name, O_RDWR | O_SYNC , S_IRUSR | S_IWUSR );
  if (this->laser_fd < 0)
  {
    PLAYER_ERROR2("unable to open serial port [%s]; [%s]",
                  (char*) this->device_name, strerror(errno));
    return 1;
  }

  // set the serial port speed to 9600 to match the laser
  // later we can ramp the speed up to the SICK's 38K
  //
  struct termios term;
  if( tcgetattr( this->laser_fd, &term ) < 0 )
    RETURN_ERROR(1, "Unable to get serial port attributes");
  
  cfmakeraw( &term );
  cfsetispeed( &term, B9600 );
  cfsetospeed( &term, B9600 );
  
  if( tcsetattr( this->laser_fd, TCSAFLUSH, &term ) < 0 )
    RETURN_ERROR(1, "Unable to set serial port attributes");

  // Make sure queue is empty
  //
  tcflush(this->laser_fd, TCIOFLUSH);
    
  return 0;
}


int SickS3000::ReadContinuousTelegram(float *ranges)
{
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

  PLAYER_MSG0(9,"waiting for header ... ");
  /* parse first 6 bytes 
   * 0x00 0x00 0x00 0x00 0x00 0x00
   * 4 byte reply header
   * 2 byte block numbbre (0x00 0x00 for data output)
   */
  n_count=ReadBytes(this->laser_fd,buffer,1);
  if(n_count<1)
    {
      PLAYER_ERROR("Could not read header");
      return -1;
    }
  if(buffer[0]==0x00)
    {
      i++;
      parsing=true;
    }
  count_answer++;
  while(n_count!=-1 && n_count!=0 && !parsed)
    {
      n_count=ReadBytes(this->laser_fd,buffer,1);
      count_answer++;
      if(buffer[0]==0x00)
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
	parsed=true;
    }
  /* header first 6 bytes received */
  PLAYER_MSG0(7,"telegram start received");

/* read size of telegram */
  telegram_size=0;
  n_count=ReadBytes(this->laser_fd,buffer,2);
  if(n_count==2)
    {
      telegram_size=(buffer[0] << 8) | buffer[1];
    }
  else
    {
      PLAYER_ERROR("error parsing: telegram size expected");
      return -1;
    }

  /*
   * still to read: (telegram_size*2) - 4 bytes
   *
   */
  telegram_size=(telegram_size*2) - 4 ;

  /* remaining header */
  /* coordination flag */
  n_count=ReadBytes(this->laser_fd,buffer,1);
  if(n_count==1 && buffer[0]==0xFF)
    {
      telegram_size--;
    }
  else
    {
      PLAYER_ERROR("error parsing: coordination flag expected");
      return -1;
    }
  /* device code */
  n_count=ReadBytes(this->laser_fd,buffer,1);
  if(n_count==1 && buffer[0]==0x07)
    {
      telegram_size--;
    }
  else
    {
      PLAYER_ERROR("error parsing: device code expected");
      return -1;
    }
  /* protocol version */
  n_count=ReadBytes(this->laser_fd,buffer,2);
  if(n_count==2 && buffer[0]==0x02 && buffer[1]==0x01)
    {
      telegram_size-=2;
    }
  else
    {
      PLAYER_ERROR("error parsing: protocol version expected");
      return -1;
    }
  /* status */
  n_count=ReadBytes(this->laser_fd,buffer,2);
  if(n_count==2 && buffer[0]==0x00 && (buffer[1]==0x00 || buffer[1]==0x01))
    {
      telegram_size-=2;
    }
  else
    {
      PLAYER_ERROR("error parsing: status expected");
      return -1;
    }
  /* timestamp */
  n_count=ReadBytes(this->laser_fd,buffer,4);
  if(n_count==4)
    {
      telegram_size-=4;
    }
  else
    {
      return -1;
    }
  /* telegram number */
  n_count=ReadBytes(this->laser_fd,buffer,2);
  if(n_count==2)
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
  n_count=ReadBytes(this->laser_fd,buffer,4);
  if(n_count!=4)
    {
      return -1;
    }
  telegram_size-=4;

  /* read data */
  // cout << "reading data: " << telegram_size-2 << " bytes, " << (telegram_size-2)/2 << " samples" << endl;
  n_count=ReadBytes(this->laser_fd,buffer,telegram_size-2);
  if(n_count!=telegram_size-2)
    {
      return -1;
    }

  /* fill data */
  for(i=0;i<DEFAULT_LASER_SAMPLES;i++)
    {
      ranges[i]=(float)(((buffer[2*i+1] & 0x1f)<<8) | buffer[2*i]);
      ranges[i] /= 100.0; /* original measure in cm , player works in meters */
    }
  //  telegram_size=2;
  telegram_size-=(telegram_size-2);

  /* read crc */
  n_count=ReadBytes(this->laser_fd,buffer,2);
  if(n_count==2)
    {
      telegram_size-=2;
    }
  else
    {
      PLAYER_ERROR("error parsing");
      return -1;
    }

  PLAYER_MSG0(9,"continuous telegram succesfully received");

  return 0;
}


int SickS3000::ReadRequestTelegram(float *ranges)
{
  const char header_start_buff[]={0x00,0x00,0x00,0x00};
  unsigned char buffer[1522];
  int n_count;
  int i;

  /* read answer header , 4 bytes */
  n_count=ReadBytes(laser_fd,buffer,4);
  if(n_count!=4 || (strncmp((const char *)buffer,header_start_buff,4)!=0))
    {
      PLAYER_ERROR("header start expected");
      return -1;
    }

  /* read repeated header, 6 bytes */
  n_count=ReadBytes(laser_fd,buffer,6);
  if(n_count!=6)
    {
      PLAYER_ERROR("repeated header expected");
      return -1;
    }

  /* block 12 monitoring data */
  n_count=read(laser_fd,buffer,2);
  if(n_count!=2)
    {
      PLAYER_ERROR("monitoring data expected");
      return -1;
    }

  /* read data */
  n_count=ReadBytes(laser_fd,buffer,1522);
  if(n_count!=1522)
    {
      PLAYER_ERROR("insuficient number of bytes retrieved");
      return -1;
    }

  for(i=0;i<DEFAULT_LASER_SAMPLES;i++)
    {
      ranges[i]=(float)(((buffer[2*i+1] & 0x1f)<<8) | buffer[2*i]);
      ranges[i] /= 100.0; /* original measure in cm , player works in meters */
    }

  /* read crc */
  n_count=ReadBytes(laser_fd,buffer,2);
  if(n_count!=2)
    {
      PLAYER_ERROR("crc code expected");
      return -1;
    }

  return 0;
}

/* mk not nedded?
extern "C" {
  int player_driver_init(DriverTable* table)
  {
    puts("SickS3000 driver initializing");
    SickS3000_Register(table);
    puts("SickS300 driver done");
    return(0);
  }
}
*/
