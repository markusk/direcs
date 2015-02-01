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

#include "direcsSerialQext.h"


DirecsSerialQext::DirecsSerialQext()
{
	// get the name of this class (this is for debugging messages)
	className = this->staticMetaObject.className();

	mDev_fd = -1; // the file descriptor !!

	// settings for the serial port
	settings.BaudRate = BAUD9600;
	settings.DataBits = DATA_8;
	settings.Parity = PAR_NONE;
	settings.StopBits = STOP_1;
	settings.FlowControl = FLOW_OFF;
	settings.Timeout_Millisec = 10;
}


DirecsSerialQext::~DirecsSerialQext()
{
}


bool DirecsSerialQext::openSerialPort()
{
	// initialise the serial port
	// continue only on success (true)
	if (openSerialPort() == true)
	{
		emit message("Serial device openend.");

/*
		// display message in GUI
		ui->textEdit->insertHtml("<b>Sending data to Arduino in some seconds (arduinoInit)...</b><br><br>");

		// Special timer, needed for Arduino!
		//
		// Reason:
		// When the serial (USB) port is opened, the Arduino is not ready for serial communication immediately.
		// Therefore we start a timer. After 3000 ms (3 seconds), it will call the function arduinoInit().
		// This can then be used for a first command to the Arduino, like "Hey Arduino, Qt-Software now startet!".
 @todo: where do we put this?!? already a timerthread in direcs.cpp!!		QTimer::singleShot(3000, this, SLOT(timerSlot()));
 */
	}

	// error
	emit message(QString("<font color=\"#FF0000\">ERROR opening serial port %1 (%2).</font>").arg(serialPortName).arg(className));
	return false;
}


int DirecsSerialQext::purgeRx()
{
		/**
		Clears the read buffer.
		TCIFLUSH = Flush data received but not read.
		 */
		return tcflush(mDev_fd, TCIFLUSH);
}


int DirecsSerialQext::writeData(unsigned char *c, QString callingClassName)
{
	int n = write(mDev_fd, c, 1);

	if (n < 0)
	{
		emit message(QString("<font color=\"#FF0000\">ERROR '%1=%2' when writing to serial device at DirecsSerialQext::writeData called from %3.</font>").arg(errno).arg(strerror(errno)).arg(callingClassName));
//		qDebug("Error %d writing to serial device: %s\n", errno, strerror(errno));
		return errno;
	}
	else
	{
		//qDebug("%1 byte(s) written.", n);
	}

	return n;
}


int DirecsSerialQext::readData(unsigned char *buf, int nChars, QString callingClassName)
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
			emit message(QString("<font color=\"#FF0000\">ERROR '%1=%2' <br>when selecting serial device at DirecsSerialQext::readData called from %3.</font>").arg(errno).arg(strerror(errno)).arg(callingClassName));
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
				emit message(QString("<font color=\"#FF0000\">ERROR No data available within %1 µs when using select() on serial device at DirecsSerialQext::readData called from %2.</font>").arg(READ_TIMEOUT_ATMEL).arg(callingClassName));
				emit message(QString("<font color=\"#FF0000\">ERROR %1: %2.</font>").arg(errno).arg(strerror(errno)));

				return errno;
			}
		}

		// read from the serial device
		amountRead = read(mDev_fd, buf, nChars);

		if (amountRead < 0 && errno != EWOULDBLOCK)
		{
			emit message(QString("<font color=\"#FF0000\">ERROR '%1=%2' when using read() on serial device at DirecsSerialQext::readData called from %3.</font>").arg(errno).arg(strerror(errno)).arg(callingClassName));
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


int DirecsSerialQext::closePort()
{
  return close(mDev_fd);
}
