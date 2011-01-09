/*************************************************************************
 *   Copyright (C) 2011 by Markus Knapp                                  *
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
	mDev_fd = -1; // the file descriptor !!
}


DirecsSerial::~DirecsSerial()
{
}


int DirecsSerial::openAtmelPort(char *dev_name, int baudrate)
{
	/*
	struct termios  options;
	int spd = -1;
	int newbaud = 0;
*/


	// + + + + +  sick old laser code start
	// + + + + +  sick old laser code start
	// + + + + +  sick old laser code start

	Q_UNUSED(baudrate);

	// for QString to char* conversion
	QByteArray ba = dev_name;


	if ( (mDev_fd = open(ba.data(), O_RDWR | O_NOCTTY | O_NONBLOCK)) < 0) // TODO: check if this still works with Linux!
	{
		return -1;
	}


	// Note that open() follows POSIX semantics: multiple open() calls to
	// the same file will succeed unless the TIOCEXCL ioctl is issued.
	// This will prevent additional opens except by root-owned processes.
	// See tty(4) ("man 4 tty") and ioctl(2) ("man 2 ioctl") for details.
	if (ioctl(mDev_fd, TIOCEXCL) == -1)
	{
		emit message(QString("Error setting TIOCEXCL on /dev/tty... - %1(%2).").arg(strerror(errno)).arg(errno));
		return -1;
	}
	emit message("TIOCEXCL set succesfully.");

	// Now that the device is open, clear the O_NONBLOCK flag so
	// subsequent I/O will block.
	// See fcntl(2) ("man 2 fcntl") for details.
	if (fcntl(mDev_fd, F_SETFL, 0) == -1)
	{
		emit message(QString("Error clearing O_NONBLOCK - %1(%2).").arg(strerror(errno)).arg(errno));
		return -1;
	}
	emit message("O_NONBLOCK cleared successfully.");


	//-----------------
	// Orignal laser.h
	//-----------------
	#define DIRECS_LASER_USE_SELECT 1
	#define DIRECS_LASER_LOW_LATENCY 1


	#ifdef DIRECS_LASER_LOW_LATENCY
	setLowLatency();
	#endif

	//-----------------
	// Original laser.cpp code now here: sick_set_serial_params(laser) inserted here:
	//-----------------
	struct termios  ctio;


	// Get current port settings
	tcgetattr(mDev_fd, &ctio);

	// set the baudrate
	cfsetispeed(&ctio, (speed_t) B38400 );
	cfsetospeed(&ctio, (speed_t) B38400 );

	// org laser.cpp: ctio.c_iflag = iSoftControl(laser->dev.swf) | iParity(laser->dev.parity);
	// swf = 0 = IXOFF ?      [iSoftControl]
	// parity = N = PARENB ?  [cParity]
	ctio.c_iflag = IXOFF | IGNPAR;

	ctio.c_oflag = 0;

	// org laser.cpp: ctio.c_cflag =         CREAD | CLOCAL                       | cParity(laser->dev.parity) | cDataSize(laser->dev.databits) | cStopSize(laser->dev.stopbits);
	// 8 Data byte = CS8  [cDataSize]
	// ^ stop byte = 0 ?  [cStopSize]

	ctio.c_cflag  = CREAD | CLOCAL | PARENB | CS8 | 0;
	ctio.c_cflag &= ~CRTSCTS; // disable HW flow control!! // TODO: check if that also works under linux!
	ctio.c_lflag = 0;
	ctio.c_cc[VTIME] = 0;     /* inter-character timer unused */
	ctio.c_cc[VMIN] = 0;      /* blocking read until 0 chars received */


	// Cause the new options to take effect immediately.
	tcsetattr(mDev_fd, TCSANOW, &ctio);

	//-----------------



	emit message("Serial device openend.");
	return(mDev_fd);


	// + + + + +  sick old laser code end
	// + + + + +  sick old laser code end
	// + + + + +  sick old laser code end



/* - - - - original:


	// mDev_fd = open(dev_name, O_RDWR | O_NOCTTY, 0);	// 2010-05-23: not needed for Atmel and SICK laser S300!
	mDev_fd = open(dev_name, O_RDWR | O_NOCTTY | O_NONBLOCK);

	// Note that open() follows POSIX semantics: multiple open() calls to
	// the same file will succeed unless the TIOCEXCL ioctl is issued.
	// This will prevent additional opens except by root-owned processes.
	// See tty(4) ("man 4 tty") and ioctl(2) ("man 2 ioctl") for details.
	if (ioctl(mDev_fd, TIOCEXCL) == -1)
	{
		emit message(QString("<font color=\"#FF0000\">ERROR %1 setting TIOCEXCL on serial device:<br>%2.</font>").arg(errno).arg(strerror(errno)));
		emit message(QString("<font color=\"#FF0000\">Serial port already opened?</font>"));
		return -1;
	}

	// Now that the device is open, clear the O_NONBLOCK flag so subsequent I/O will block.
	// See fcntl(2) ("man 2 fcntl") for details.
	if (fcntl(mDev_fd, F_SETFL, 0) == -1)
	{
		emit message(QString("<font color=\"#FF0000\">ERROR %1 clearing O_NONBLOCK on serial device:<br>%2.</font>").arg(errno).arg(strerror(errno)));
		return -1;
	}

	if (mDev_fd < 0)
	{
		emit message(QString("<font color=\"#FF0000\">ERROR %1 opening serial device:<br>%2.</font>").arg(errno).arg(strerror(errno)));
		return errno;
	}


	// Get current port settings
	if (tcgetattr(mDev_fd, &options))
	{
		emit message(QString("<font color=\"#FF0000\">ERROR getting serial port attributes at DirecsSerial::openAtmelPort!</font>"));
		return -1;
	}

	// this setting is needed for Mac OS! But works under Linux, too!
	options.c_cflag |= CLOCAL;

	// 8N1
	options.c_cflag &= ~PARENB;
	options.c_cflag &= ~CSTOPB;
	options.c_cflag &= ~CSIZE;
	options.c_cflag |= CS8;

	// Disable hardware flow control:
	options.c_cflag &= ~CRTSCTS;

	options.c_lflag = 0;
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
			emit message(QString("<font color=\"#FF0000\">ERROR setting serial port input speed at DirecsSerial::openAtmelPort!</font>"));
			return -1;
		}

		if (cfsetospeed(&options, (speed_t) spd) != 0)
		{
			emit message(QString("<font color=\"#FF0000\">ERROR setting serial port output speed at DirecsSerial::openAtmelPort!</font>"));
			return -1;
		}
	}
	else
	{
		emit message(QString("<font color=\"#FF0000\">ERROR: Wrong value for speed parameter at DirecsSerial::openAtmelPort!</font>"));
		return -1;
	}

	// Flushes all pending I/O to the serial port. This clears only the read buffer!
	if (tcflush(mDev_fd, TCIFLUSH) != 0)
	{
		emit message(QString("<font color=\"#FF0000\">ERROR fluhsing serial input buffer at DirecsSerial::openAtmelPort!</font>"));
		return -1;
	}

	// Cause the new options to take effect immediately.
	if (tcsetattr(mDev_fd, TCSANOW, &options) != 0)
	{
		emit message(QString("<font color=\"#FF0000\">ERROR setting serial port attributes at DirecsSerial::openAtmelPort!</font>"));
		return -1;
	}


	emit message("Serial device openend.");
	return (mDev_fd);
 - - - - original end */
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


void DirecsSerial::setParms(int fd, int baudr, char par, char bits, int hwf, int swf, int stopb)
{
	int spd = -1;
	int newbaud = 0;


	#ifdef _POSIX
	struct termios tty;
	tcgetattr(fd, &tty);
	#else
	struct sgttyb tty;
	ioctl(fd, TIOCGETP, &tty);
	#endif


	/* We generate mark and space parity ourself. */
	if(bits == 7 && (par == parityM || par == parityS))
		bits = 8;


	/* Check if 'baudr' is really a number */
// 	if((newbaud = (atol(baudr) / 100)) == 0 && baudr[0] != '0')
// 		newbaud = -1;
	newbaud = (baudr/100);

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

	if(par == parityEven) // even
		tty.sg_flags |= EVENP;
	else if(par == parityOdd) // odd
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
	if(par == parityEven)
		tty.sg_flags |= EVENP;
	else if(par == parityOdd)
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
	switch (bits)
	{
		case 5:
			tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS5;
			break;
		case 6:
			tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS6;
			break;
		case 7:
			tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS7;
			break;
		case 8:
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

	if(par == parityEven)
		tty.c_cflag |= PARENB; // Parity=EVEN
	else if(par == parityOdd)
		tty.c_cflag |= PARODD; // Parity=ODD



	/* The stop bit */
	switch (stopb)
	{
		case 0:
			tty.c_cflag |= CSTOPB;
			break;
		case 1:
			tty.c_cflag &= ~CSTOPB; // deleting the bis is setting it to 1 STOP BIT!
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


void DirecsSerial::configurePort(int dev_fd, int baudrate, char parity)
{
	if (baudrate == 55555)
		baudrate = 57600;

	setParms(dev_fd, baudrate, parity, 8, 0, 0);
}

long DirecsSerial::numChars(int dev_fd)
{
	int available = 0;


	int err = ioctl(dev_fd, FIONREAD, &available);
	if (err == 0)
	{
		emit message(QString("Bytes available at readAtmelPort: %1").arg(available));
		return available;
	}
	else
	{
		emit message(QString("<font color=\"#FF0000\">ERROR '%1=%2' when using ioctl() on serial device at DirecsSerial::numChars().</font>").arg(errno).arg(strerror(errno)));
		return errno;
	}
}


long DirecsSerial::numChars()
{
	int available = 0;
	double timeout=0.1;
	fd_set read_set;
	struct timeval timer;
	timer.tv_sec=(long)(floor(timeout));
	timer.tv_usec=(long)((timeout-floor(timeout))*1000000);
	FD_ZERO(&read_set);
	FD_SET(mDev_fd, &read_set);
	select(mDev_fd + 1, &read_set, NULL, NULL, &timer);


	int err = ioctl(mDev_fd, FIONREAD, &available);
	if (err == 0)
	{
		emit message(QString("Bytes available at readAtmelPort: %1").arg(available));
		return available;
	}
	else
	{
		emit message(QString("<font color=\"#FF0000\">ERROR '%1=%2' when using ioctl() on serial device at DirecsSerial::numChars().</font>").arg(errno).arg(strerror(errno)));
		return errno;
	}
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


int DirecsSerial::purgeRx()
{
		/**
		Clears the read buffer.
		TCIFLUSH = Flush data received but not read.
		 */
		return tcflush(mDev_fd, TCIFLUSH);
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


int DirecsSerial::writeAtmelPort(unsigned char *c)
{
	int n = write(mDev_fd, c, 1);

	if (n < 0)
	{
		emit message(QString("<font color=\"#FF0000\">ERROR '%1=%2' when writing to serial device at DirecsSerial::writeAtmelPort.</font>").arg(errno).arg(strerror(errno)));
//		qDebug("Error %d writing to serial device: %s\n", errno, strerror(errno));
		return errno;
	}
	else
	{
		//qDebug("%1 byte(s) written.", n);
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


int DirecsSerial::readAtmelPort(unsigned char *buf, int nChars)
{
	// + + + + +  sick old laser code start (from readPort of this class taken)
	// + + + + +  sick old laser code start (from readPort of this class taken)
	// + + + + +  sick old laser code start (from readPort of this class taken)

	// ---------------------------------------------------------
	// taken from sick_handle_laser():
	// ---------------------------------------------------------

	int amountRead = 0, bytes_read = 0;
	struct timeval t;
	fd_set set;
	int err;

	while (nChars > 0)
	{
		t.tv_sec = 0;
		t.tv_usec = READ_TIMEOUT;
		FD_ZERO(&set);
		FD_SET(mDev_fd, &set);

		err = select(mDev_fd + 1, &set, NULL, NULL, &t);
		if (err == 0)
		{
			emit message(QString("<font color=\"#FF0000\">ERROR '%1=%2' when using select() on serial device at DirecsSerial::readAtmelPort.</font>").arg(errno).arg(strerror(errno)));
			return errno;
		}

		amountRead = read(mDev_fd, buf, nChars);
		emit message(QString("Read byte=0x%2").arg(*buf, 2, 16, QLatin1Char('0')));

		if (amountRead < 0 && errno != EWOULDBLOCK)
		{
			emit message(QString("<font color=\"#FF0000\">ERROR '%1=%2' when using read() on serial device at DirecsSerial::readAtmelPort.</font>").arg(errno).arg(strerror(errno)));
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


	// + + + + +  sick old laser code end
	// + + + + +  sick old laser code end
	// + + + + +  sick old laser code end


/*
	//
	// Original code from method readPort
	// Only using the local member dev_fd, instead of serial ports from laser scanner struct
	//
	int amountRead = 0, bytes_read = 0;
	struct timeval t;
	fd_set set;
	int err;

	while (nChars > 0)
	{
		// Timeout is not changed by select(), and may be reused on subsequent calls, however it is good style to re-initialize it before each invocation of select().
		t.tv_sec = 0;
		t.tv_usec = READ_TIMEOUT_ATMEL;
		FD_ZERO(&set);
		FD_SET(mDev_fd, &set);

		err = select(mDev_fd + 1, &set, NULL, NULL, &t);

		// check if time limit expired (select=0)
		if (err == 0)
		{
			emit message(QString("<font color=\"#FF0000\">ERROR '%1=%2' when using select() on serial device at DirecsSerial::readAtmelPort.</font>").arg(errno).arg(strerror(errno)));
			return errno;
		}

		// read from the serial device
		amountRead = read(mDev_fd, buf, nChars);


		if(amountRead < 0 && errno != EWOULDBLOCK)
		{
			emit message(QString("<font color=\"#FF0000\">ERROR '%1=%2' when using read() on serial device at DirecsSerial::readAtmelPort.</font>").arg(errno).arg(strerror(errno)));
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
*/
}


int DirecsSerial::closeAtmelPort()
{
  return close(mDev_fd);
}


int DirecsSerial::setLowLatency(int fd)
{
#ifdef Q_OS_LINUX // currently supported only under linux (no MAC OS, Windoze at the moment)
	#ifdef CYGWIN
	fd = fd;
	return -1;
	#else

	struct serial_struct serial;
	int result;
	result = ioctl(fd, TIOCGSERIAL, &serial);

	if (result)
	{
		emit message("<font color=\"#FF0000\">ERROR: Cannot get the serial attributes for low latency serial mode. Switching to normal mode</font>");
		return result;
	}
	else
	{
		serial.flags |= ASYNC_LOW_LATENCY;
		serial.xmit_fifo_size = 1;

		result = ioctl(fd, TIOCSSERIAL, &serial);

		if (result)
		{
			emit message("<font color=\"#FF0000\">ERROR: Cannot activeate low latency mode. Switching to normal mode</font>");
			return result;
		}
	}
	return result;
	#endif
#else
	Q_UNUSED(fd);
	return -1;
#endif
}


int DirecsSerial::setLowLatency()
{
#ifdef Q_OS_LINUX // currently supported only under linux (no MAC OS, Windoze at the moment)
	#ifdef CYGWIN
	return -1;
	#else

	struct serial_struct serial;
	int result;
	result = ioctl(mDev_fd, TIOCGSERIAL, &serial);

	if (result)
	{
		emit message("<font color=\"#FF0000\">ERROR: Cannot get the serial attributes for low latency serial mode. Switching to normal mode</font>");
		return result;
	}
	else
	{
		serial.flags |= ASYNC_LOW_LATENCY;
		serial.xmit_fifo_size = 1;

		result = ioctl(mDev_fd, TIOCSSERIAL, &serial);

		if (result)
		{
			emit message("<font color=\"#FF0000\">ERROR: Cannot activeate low latency mode. Switching to normal mode</font>");
			return result;
		}
	}
	return result;
	#endif
#else
	return -1;
#endif
}
