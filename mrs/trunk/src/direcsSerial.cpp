/*************************************************************************
 *   Copyright (C) 2008 by Markus Knapp                                  *
 *   mrs @ direcs.de                                                     *
 *                                                                       *
 *   This file is part of mrs.                                           *
 *                                                                       *
 *   mrs is free software: you can redistribute it and/or modify it      *
 *   under the terms of the GNU General Public License as published      *
 *   by the Free Software Foundation, version 3 of the License.          *
 *                                                                       *
 *   mrs is distributed in the hope that it will be useful,              *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of      *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the        *
 *   GNU General Public License for more details.                        *
 *                                                                       *
 *   You should have received a copy of the GNU General Public License   *
 *   along with mrs. If not, see <http://www.gnu.org/licenses/>.         *
 *                                                                       *
 *************************************************************************/

#include "direcsSerial.h"


DirecsSerial::DirecsSerial()
{
}


DirecsSerial::~DirecsSerial()
{
}


int DirecsSerial::openPort(int *dev_fd, char *dev_name)
{
	int BAUDRATE = B9600;
	struct termios newtio;
	
	*dev_fd = open(dev_name, O_RDWR | O_SYNC | O_NOCTTY, 0);
	if(*dev_fd < 0) {
		fprintf(stderr,"Serial I/O Error:  Could not open port %s\n", dev_name);
		return -1;
	}
	
	//--------------------------------------------------
	// configuring the serial port.
	// these settings can be changed with setParms() !
	//--------------------------------------------------
	
	// get the current options for the port
	tcgetattr(*dev_fd, &newtio);
	
	// setting the baud rate
	cfsetispeed(&newtio, BAUDRATE);
	cfsetospeed(&newtio, BAUDRATE);
	
	newtio.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
	newtio.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON | IXOFF);
	newtio.c_cflag &= ~(CSIZE | PARENB | PARODD);
	
	newtio.c_cflag |= (CS8); // 8 Bit
	newtio.c_oflag &= ~(OPOST);
	newtio.c_cc[VTIME] = 1;
	newtio.c_cc[VMIN] = 0;
	// flush
	tcflush(*dev_fd, TCIFLUSH);
	// set the options now
	tcsetattr(*dev_fd, TCSANOW, &newtio);
	return 0;
}


bool DirecsSerial::openAtmelPort(int *dev_fd, char *dev_name)
{
	//--------------------------------------------------
	// This is now original code from test/serial.c !
	//--------------------------------------------------
	// open_port
	//--------------------------------------------------
	*dev_fd = open(dev_name, O_RDWR | O_NOCTTY | O_NDELAY);
	
	if (*dev_fd == -1)
	{
		/*
		* Could not open the port.
		*/
		qDebug("open_port: Unable to open %1", dev_name);
		return false;
	}
	else
	{
		fcntl(*dev_fd, F_SETFL, 0);
	}
	
	
	//--------------------------------------------------
	// configure_port
	//--------------------------------------------------
	struct termios options;	
	
	/*
	* GET the current options for the port...
	*/
	tcgetattr(*dev_fd, &options);
	
	/*
	* Setting the baud rate.
	*/
	cfsetispeed(&options, B9600);
	cfsetospeed(&options, B9600);
	
	/*
	* Enable the receiver and set local mode...
	*/
	options.c_cflag |= (CLOCAL | CREAD);

	/*
	* Setting the Character Size
	*/
	options.c_cflag &= ~CSIZE; /* Mask the character size bits */
	options.c_cflag |= CS8;    /* Select 8 data bits */

	/*
	* Setting Parity Checking
	*/
	// No parity (8N1)
	options.c_cflag &= ~PARENB;
	options.c_cflag &= ~CSTOPB;
	options.c_cflag &= ~CSIZE;
	options.c_cflag |= CS8;

	/*
	* Disabling Hardware Flow Control (Also called CRTSCTS)
	*/
	options.c_cflag &= ~CRTSCTS;
	
	/*
	* SET the new options for the port...
	*/
	tcsetattr(*dev_fd, TCSANOW, &options);

	// FLUSH
	fcntl(*dev_fd, F_SETFL, 0);
	
	return true;
}


#if (defined(TIOCM_RTS) && defined(TIOCMODG)) || defined(_COHERENT)
void DirecsSerial::setRTS(int fd)
#else
void DirecsSerial::setRTS(int fd  __attribute__ ((unused)))
#endif
{
	#if defined(TIOCM_RTS) && defined(TIOCMODG)
	int mcs = 0;
	
	ioctl(fd, TIOCMODG, &mcs);
	mcs |= TIOCM_RTS;
	ioctl(fd, TIOCMODS, &mcs);
	#endif
	#ifdef _COHERENT
	ioctl(fd, TIOCSRTS, 0);
	#endif
}


void DirecsSerial::setParms(int fd, char *baudr, char *par, char *bits, int hwf, int swf, int stopb)
{
	int spd = -1;
	int newbaud = 0;
	int bit = bits[0];
	#ifdef _POSIX
	struct termios tty;
	tcgetattr(fd, &tty);
	#else
	struct sgttyb tty;
	ioctl(fd, TIOCGETP, &tty);
	#endif
	
	
	/* We generate mark and space parity ourself. */
	if(bit == '7' && (par[0] == 'M' || par[0] == 'S'))
		bit = '8';


	/* Check if 'baudr' is really a number */
	if((newbaud = (atol(baudr) / 100)) == 0 && baudr[0] != '0')
		newbaud = -1;
	
	switch(newbaud)
	{
		case 0:
		#ifdef B0
			spd = B0;	   break;
		#else
			spd = 0;  	   break;
		#endif
		case 3:	
			spd = B300;	   break;
		case 6:
			spd = B600;	   break;
		case 12:
			spd = B1200;   break;
		case 24:
			spd = B2400;   break;
		case 48:
			spd = B4800;   break;
		case 96:
			spd = B9600;   break;
		#ifdef B19200
		case 192:	
			spd = B19200;  break;
		#else
		#ifdef EXTA
		case 192:
			spd = EXTA;    break;
		#else
		case 192:
			spd = B9600;   break;
		#endif	
		#endif	
		#ifdef B38400
		case 384:
			spd = B38400;  break;
		#else
		#ifdef EXTB
		case 384:
			spd = EXTB;    break;
		#else
		case 384:	
			spd = B9600;   break;
		#endif
		#endif	
		#ifdef B57600
		case 576:	
			spd = B57600;  break;
		#endif
		#ifdef B115200
		case 1152:
			spd = B115200; break;
		#endif
		#ifdef B500000
		case 5000:
			spd = B500000; break;
		#endif
	}
	
	#if defined (_BSD43) && !defined(_POSIX)
	if(spd != -1)
		tty.sg_ispeed = tty.sg_ospeed = spd;
	
	/* Number of bits is ignored */
	tty.sg_flags = RAW | TANDEM;
	
	if(par[0] == 'E') // even
		tty.sg_flags |= EVENP;
	else if(par[0] == 'O') // odd
		tty.sg_flags |= ODDP;
	else
		tty.sg_flags |= PASS8 | ANYP; //
	
	ioctl(fd, TIOCSETP, &tty);
	#ifdef TIOCSDTR
	/* FIXME: huh? - MvS */
	ioctl(fd, TIOCSDTR, 0);
	#endif
	#endif
	
	#if defined (_V7) && !defined(_POSIX)
	if(spd != -1) tty.sg_ispeed = tty.sg_ospeed = spd;
	tty.sg_flags = RAW;
	if(par[0] == 'E')
		tty.sg_flags |= EVENP;
	else if(par[0] == 'O')
		tty.sg_flags |= ODDP;
	ioctl(fd, TIOCSETP, &tty);
	#endif
	
	#ifdef _POSIX
	if(spd != -1)
	{
		cfsetospeed(&tty, (speed_t)spd);
		cfsetispeed(&tty, (speed_t)spd);
	}


	/* The data bits */
	switch (bit)
	{
		case '5':
			tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS5;
			break;
		case '6':
			tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS6;
			break;
		case '7':
			tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS7;
			break;
		case '8':
		default:
			tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;
			break;
	}
	
	/* Set into raw, no echo mode */
	#if !defined(_DGUX_SOURCE)
	tty.c_iflag &= ~(IGNBRK | IGNCR | INLCR | ICRNL | /* IUCLC | */ IXANY | IXON | IXOFF | INPCK | ISTRIP);
	tty.c_iflag |= (BRKINT | IGNPAR);
	tty.c_oflag &= ~OPOST;
	tty.c_lflag = ~(ICANON | ISIG | ECHO | ECHONL | ECHOE | ECHOK | IEXTEN);
	tty.c_cflag |= CREAD | CRTSCTS;
	#else /* Okay, this is better. XXX - Fix the above. */
	tty.c_iflag =  IGNBRK;
	tty.c_lflag = 0;
	tty.c_oflag = 0;
	tty.c_cflag |= CLOCAL | CREAD;
	#endif
	tty.c_cc[VMIN] = 1;
	tty.c_cc[VTIME] = 5;
	
	/* Flow control. */
	if(hwf)
	{
		// enable Hardware flow control
		tty.c_cflag |= CRTSCTS;
		tty.c_cflag &= ~CLOCAL;
	}
	else
	{
		// disable Hardware flow control
		tty.c_cflag &= ~CRTSCTS;
		tty.c_cflag |= CLOCAL;
	}
	
	if(swf)
	{
		// Enable software flow control (outgoing and incoming)
		tty.c_iflag |= (IXON | IXOFF);
	}
	else
	{
		// Disable software flow control (outgoing and incoming)
		tty.c_iflag &= ~(IXON | IXOFF);
	}
	
	// set the PARITY
	tty.c_cflag &= ~(PARENB | PARODD); // delete parity bit and odd bit, so we have PARITY=N here
	
	if(par[0] == 'E')
		tty.c_cflag |= PARENB; // Parity=EVEN
	else if(par[0] == 'O')
		tty.c_cflag |= PARODD; // Parity=ODD
	


	/* The stop bit */
	switch (stopb)
	{
		case 0:
			tty.c_cflag |= CSTOPB;
			break;
		case 1:
			tty.c_cflag &= ~CSTOPB; // deleting the bis is setitng it to 1 STOP BIT ?!??
			break;
	}
	
	
	/* Set the new settings for the serial port NOW! */
	tcsetattr(fd, TCSANOW, &tty);
	
	setRTS(fd);
	#ifdef _DGUX_SOURCE
	serial_sethwf(fd, hwf);
	#endif
	#endif
}


void DirecsSerial::configurePort(int dev_fd, int baudrate, char *parity)
{
	switch(baudrate)
	{
		case 9600: 
			setParms(dev_fd, "9600", parity, "8", 0, 0);
			break;
		case 19200: 
			setParms(dev_fd, "19200", parity, "8", 0, 0);
			break;
		case 38400: 
			setParms(dev_fd, "38400", parity, "8", 0, 0);
			break;
		case 57600:
		case 55555:
			setParms(dev_fd, "57600", parity, "8", 0, 0);
			break;
		case 115200:
			setParms(dev_fd, "115200", parity, "8", 0, 0);
		case 500000:
			setParms(dev_fd, "500000", parity, "8", 0, 0);
			break;
	}
}


long DirecsSerial::numChars(int dev_fd)
{
	long available = 0;
	
	if(ioctl(dev_fd, FIONREAD, &available) == 0)
		return available;
	else
		return -1;
}


int DirecsSerial::clearInputBuffer(int dev_fd)
{
	int max_serial_buffer_size = 16384; 
	unsigned char buffer[max_serial_buffer_size]; 
	int val = 0;
	int val_total = 0;
	
	val_total = numChars(dev_fd);
	val = val_total;
	
	while (val > max_serial_buffer_size)
	{
		read(dev_fd, &buffer, max_serial_buffer_size);
		val -= max_serial_buffer_size;
	}
	
	if(val > 0) 
		read(dev_fd, &buffer, val);
	
	return(val_total);
}


int DirecsSerial::writePort(int dev_fd, unsigned char *buf, int nChars)
{
	int amountWritten = 0;
	
	while(nChars > 0)
	{
		amountWritten = write(dev_fd, buf, nChars);
		if(amountWritten < 0) {
		if(errno == EWOULDBLOCK) {
			fprintf(stderr, "\nWARNING: writeN: EWOULDBLOCK: trying again!\n");
		} 
		else {
			return -1;
		}
		}
		else {
		nChars -= amountWritten;
		buf += amountWritten;
		}
	}
	return 0;
}


int DirecsSerial::writeAtmelPort(int dev_fd, unsigned char *c, int nChars)
{
	//--------------------------------------------------
	// This is now original code from test/serial.c !
	//--------------------------------------------------
	/*
	Writing Data to the Port
	Writing data to the port is easy - just use the write(2) system call to send data it.
	The write function returns the number of bytes sent or -1 if an error occurred.
	Usually the only error you'll run into is EIO when a MODEM or data link drops the Data Carrier Detect (DCD) line.
	This condition will persist until you close the port.int write_port(void)
	*/
	int n = write(dev_fd, c, 1);
	
	if (n < 0)
	{
		// error
		fputs("write() of n bytes failed!\n", stderr);
	}
	else
	{
		printf("%d byte(s) written.\n", n);
	}
	
	return n;
}


int DirecsSerial::readPort(int dev_fd, unsigned char *buf, int nChars)
{
	int amountRead = 0, bytes_read = 0;
	struct timeval t;
	fd_set set;
	int err;
	
	while(nChars > 0)
	{
		t.tv_sec = 0;
		t.tv_usec = READ_TIMEOUT;
		FD_ZERO(&set);
		FD_SET(dev_fd, &set);
		err = select(dev_fd + 1, &set, NULL, NULL, &t);
		if(err == 0)
		return -2;
	
		amountRead = read(dev_fd, buf, nChars);
		if(amountRead < 0 && errno != EWOULDBLOCK)
		return -1;
		else if(amountRead > 0) {
		bytes_read += amountRead;
		nChars -= amountRead;
		buf += amountRead;
		}
	}
	return bytes_read;
}


int DirecsSerial::readAtmelPort(int dev_fd, unsigned char *buf, int nChars)
{
	//--------------------------------------------------
	// This is now original code from test/serial.c !
	//--------------------------------------------------
	int amountRead = 0, bytes_read = 0;
	
	
	/*
	Reading Data from the Port
	Reading data from a port is a little trickier.
	When you operate the port in raw data mode, each read system call will return the number of characters that are actually available in the serial input buffers.
	If no characters are available, the call will block (wait) until characters come in, an interval timer expires, or an error occurs.
	The read function can be made to return immediately by doing the following.
	*/
	fcntl(dev_fd, F_SETFL, FNDELAY);

	/*
	The FNDELAY option causes the read function to return 0 if no characters are available on the port.
	To restore normal (blocking) behavior, call fcntl() without the FNDELAY option.
	This is also used after opening a serial port with the O_NDELAY option.
	*/
	fcntl(dev_fd, F_SETFL, 0);
	
	// read!
	amountRead = read(dev_fd, buf, nChars);
}


int DirecsSerial::closePort(int dev_fd)
{
  return close(dev_fd);
}


int DirecsSerial::setLowLatency(int fd)
{
	#ifdef CYGWIN
	fd = fd;
	return -1;
	#else
	
	struct serial_struct serial; 
	int result;
	result=ioctl(fd, TIOCGSERIAL, &serial);
	
	if (result)
	{
		qDebug("Cannot get the serial attributes for low latency serial mode.\n Switching to normal mode");
		return result;
	}
	else
	{
		serial.flags |= ASYNC_LOW_LATENCY;
		serial.xmit_fifo_size = 1;
		ioctl(fd, TIOCSSERIAL, &serial); 
		if (result)
		{
			qDebug("Cannot activeate low latency mode\nSwitching to normal mode");
			return result;
		}
	}
	return result;
	#endif
}
