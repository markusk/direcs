/*************************************************************************
 *   Copyright (C) Markus Knapp                                          *
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

#include "direcsSerial.h"


DirecsSerial::DirecsSerial()
{
	// get the name of this class (this is for debugging messages)
	className = this->staticMetaObject.className();

	mDev_fd = -1; // the file descriptor !!
}


DirecsSerial::~DirecsSerial()
{
}


int DirecsSerial::openPort(char *dev_name, int baudrate)
{
	struct termios  options;
	int spd = -1;
	int newbaud = 0;


	 // mDev_fd = open(dev_name, O_RDWR | O_NOCTTY, 0);	// 2010-05-23: not needed for Atmel and SICK laser S300!
	mDev_fd = open(dev_name, O_RDWR | O_NOCTTY | O_NONBLOCK);

	// Note that open() follows POSIX semantics: multiple open() calls to
	// the same file will succeed unless the TIOCEXCL ioctl is issued.
	// This will prevent additional opens except by root-owned processes.
	// See tty(4) ("man 4 tty") and ioctl(2) ("man 2 ioctl") for details.
	if (ioctl(mDev_fd, TIOCEXCL) == -1)
	{
		emit message(QString("<font color=\"#FF0000\">ERROR %1 setting TIOCEXCL on serial device:<br>%2 in %3.</font>").arg(errno).arg(strerror(errno)).arg(className));
		emit message(QString("<font color=\"#FF0000\">Serial port already opened?</font>"));
		return -1;
	}

	// Now that the device is open, clear the O_NONBLOCK flag so subsequent I/O will block.
	// See fcntl(2) ("man 2 fcntl") for details.
	if (fcntl(mDev_fd, F_SETFL, 0) == -1)
	{
		emit message(QString("<font color=\"#FF0000\">ERROR %1 clearing O_NONBLOCK on serial device:<br>%2 in %3.</font>").arg(errno).arg(strerror(errno)).arg(className));
		return -1;
	}

	if (mDev_fd < 0)
	{
		emit message(QString("<font color=\"#FF0000\">ERROR %1 opening serial device:<br>%2 in %3.</font>").arg(errno).arg(strerror(errno)).arg(className));
		return errno;
	}


	// Get current port settings
	tcgetattr(mDev_fd, &options);

	// this setting is needed for Mac OS! But works under Linux, too!
	options.c_cflag |= CLOCAL;

	// 8N1
	options.c_cflag &= ~PARENB; // no parity bit
	options.c_cflag &= ~CSTOPB; // 1 stop bit
	options.c_cflag &= ~CSIZE;  //
	options.c_cflag |= CS8;     // 8 data bit

	// Disable hardware flow control:
	options.c_cflag &= ~CRTSCTS;

	//	options.c_lflag = 0; // DISABLED! Using cfmakeraw instead below! Now we are fine under Linux with the laser scanner S300 which uses this method, too! (2.4.2011)

	options.c_cc[VTIME] = 0;     // inter-character timer unused
	options.c_cc[VMIN] = 0;      // blocking read until 0 chars received

	// this part is originally from setparms:
	newbaud = (baudrate/100);

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

	// set speed (input and output)
	if(spd != -1)
	{
		if (cfsetispeed(&options, (speed_t) spd) != 0)
		{
			emit message(QString("<font color=\"#FF0000\">ERROR setting serial port input speed at DirecsSerial::openPort!</font>"));
			return -1;
		}

		if (cfsetospeed(&options, (speed_t) spd) != 0)
		{
			emit message(QString("<font color=\"#FF0000\">ERROR setting serial port output speed at DirecsSerial::openPort!</font>"));
			return -1;
		}
	}
	else
	{
		emit message(QString("<font color=\"#FF0000\">ERROR: Wrong value for speed parameter at DirecsSerial::openPort!</font>"));
		return -1;
	}

	// Flushes all pending I/O to the serial port. This clears only the read buffer!
	if (tcflush(mDev_fd, TCIFLUSH) != 0)
	{
		emit message(QString("<font color=\"#FF0000\">ERROR fluhsing serial input buffer at DirecsSerial::openPort!</font>"));
		return -1;
	}

	/// Set *TERMIOS_P to indicate raw mode.
	// Added on 2.4.2011 for the usage of the laser scanner S300 under Linux. Resolved the "resource unavailable" error and works under Mac OS X 10.6, too.
	cfmakeraw(&options);

	// Cause the new options to take effect immediately.
	if (tcsetattr(mDev_fd, TCSANOW, &options) != 0)
	{
		emit message(QString("<font color=\"#FF0000\">ERROR setting serial port attributes at DirecsSerial::openPort!</font>"));
		return -1;
	}

	emit message("Serial device openend.");
	return (mDev_fd);
}


int DirecsSerial::purgeRx()
{
		/**
		Clears the read buffer.
		TCIFLUSH = Flush data received but not read.
		 */
		return tcflush(mDev_fd, TCIFLUSH);
}


int DirecsSerial::writeData(unsigned char *c, QString callingClassName)
{
	int n = write(mDev_fd, c, 1);

	if (n < 0)
	{
		emit message(QString("<font color=\"#FF0000\">ERROR '%1=%2' when writing to serial device at DirecsSerial::writeData called from %3.</font>").arg(errno).arg(strerror(errno)).arg(callingClassName));
//		qDebug("Error %d writing to serial device: %s\n", errno, strerror(errno));
		return errno;
	}
	else
	{
		//qDebug("%1 byte(s) written.", n);
	}

	return n;
}


int DirecsSerial::readData(unsigned char *buf, int nChars, QString callingClassName)
{
	//
	// Original code from method readPort
	// Only using the local member dev_fd, instead of serial ports from laser scanner struct
	//
	int amountRead = 0, bytes_read = 0;
	struct timeval t;
	fd_set set;
	int returnValue;


	while (nChars > 0)
	{
		// wait up to 0,25 seconds (250000 microseconds)
		// Timeout is not changed by select(), and may be reused on subsequent calls, however it is good style to re-initialize it before each invocation of select().
		t.tv_sec  = 0;
		t.tv_usec = READ_TIMEOUT_ATMEL; // 0,25 seconds

		// watch serial port to see when it has input
		FD_ZERO(&set);
		FD_SET(mDev_fd, &set);

		// is the serial port ready for reading?
		returnValue = select(mDev_fd + 1, &set, NULL, NULL, &t);

		// check if timeout or an error occured
		if (returnValue == -1)
		{
			emit message(QString("<font color=\"#FF0000\">ERROR '%1=%2' <br>when selecting serial device at DirecsSerial::readData called from %3.</font>").arg(errno).arg(strerror(errno)).arg(callingClassName));
			return errno;
		}
		else
		{
			if (returnValue)
			{
				// data available now
			}
			else
			{
				// timeout
				emit message(QString("<font color=\"#FF0000\">ERROR No data available within %1 µs when using select() on serial device at DirecsSerial::readData called from %2.</font>").arg(READ_TIMEOUT_ATMEL).arg(callingClassName));
				emit message(QString("<font color=\"#FF0000\">ERROR %1: %2.</font>").arg(errno).arg(strerror(errno)));

				return errno;
			}
		}

		// read from the serial device
		amountRead = read(mDev_fd, buf, nChars);

		if (amountRead < 0 && errno != EWOULDBLOCK)
		{
			emit message(QString("<font color=\"#FF0000\">ERROR '%1=%2' when using read() on serial device at DirecsSerial::readData called from %3.</font>").arg(errno).arg(strerror(errno)).arg(callingClassName));
// FIXME: was, wenn return 0 ?!?!?
			return errno;
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


int DirecsSerial::closePort()
{
  return close(mDev_fd);
}
