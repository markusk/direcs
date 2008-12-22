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
	dev_fd = 0; // the file descriptor !!
}


DirecsSerial::~DirecsSerial()
{
}


int DirecsSerial::openAtmelPort(char *dev_name)
{
	// This method is only used for the atmel serial port!
	// *Not* for the laser scanners!
	if ((dev_fd = open(dev_name, O_RDWR | O_NOCTTY, 0)) < 0)
	{
		return (-1);
	}
	
	/*
	Original code in sick_serial_conect:
	#ifdef CARMEN_LASER_LOW_LATENCY
		serialPort->setLowLatency(laser->dev.fd);
	#endif
	*/
	

	// Contents of sick_set_serial_params(laser) placed here:
	struct termios  ctio;
		
	tcgetattr(dev_fd, &ctio); /* save current port settings */
	ctio.c_iflag = IXON | IGNPAR; // TODO: IXON oder IXOFF ?!?? < < < <
	ctio.c_oflag = 0;
	//                   |  SW flow control  |  Parity 0  |  8 Databits  |  1 StopBit
	ctio.c_cflag = CREAD |  CLOCAL           |  0         |  CS8         |  1;
	ctio.c_lflag = 0;
	ctio.c_cc[VTIME] = 0;     /* inter-character timer unused */
	ctio.c_cc[VMIN] = 0;      /* blocking read until 0 chars received */
	cfsetispeed(&ctio, (speed_t) B9600);
	cfsetospeed(&ctio, (speed_t) B9600);
	tcflush(dev_fd, TCIFLUSH);
	tcsetattr(dev_fd, TCSANOW, &ctio);
	
	return (dev_fd);
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
	/*  huh? - MvS */
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


//int DirecsSerial::writeAtmelPort(int dev_fd, unsigned char *c, int nChars)
int DirecsSerial::writeAtmelPort(unsigned char *c, int nChars)
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
		qDebug("write() of n bytes failed!");
	}
	else
	{
		//qDebug("%1 byte(s) written.", n);
	}
	
	return n;
}


int DirecsSerial::readAtmelPort(unsigned char *buf, int nChars)
{
	//
	// Original code from method readPort
	// Only using the local member dev_fd, instead of serial ports from laser scanner struct
	//
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



int DirecsSerial::closeAtmelPort()
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
		qDebug("Cannot get the serial attributes for low latency serial mode. Switching to normal mode");
		return result;
	}
	else
	{
		serial.flags |= ASYNC_LOW_LATENCY;
		serial.xmit_fifo_size = 1;
		ioctl(fd, TIOCSSERIAL, &serial); 
		if (result)
		{
			qDebug("Cannot activeate low latency mode. Switching to normal mode");
			return result;
		}
	}
	return result;
	#endif
}
