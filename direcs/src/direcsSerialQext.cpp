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
		emit message("Serial port openend.");

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


void DirecsSerialQext::purgeRx()
{
	port->flush();
}


int DirecsSerialQext::writeData(int value, QString callingClassName)
{
	QByteArray byte; // byte to sent to the port
	qint64 bw = -1;   // bytes really written


	byte.clear(); // clear buffer to be sent
	byte.append(value); // fill buffer with value to be sent

	if (port != NULL)
	{
		// write byte to serial port
		bw = port->write(byte);

		// show sent data
		// ui->textEdit->insertHtml(QString("%1 byte(s) written. Written value: %2 (DEC) / %3 (HEX) / %4 (ASCII)<br>").arg(bw).arg(value).arg(value, 0, 16).arg(QChar(value)));

		// flush serial port
		port->flush();

		if (bw == -1)
		{
//			emit message(QString("<font color=\"#FF0000\">ERROR '%1' when writing to serial port (%2 called from %3.</font>").arg(port->errorString()).arg(className).arg(callingClassName));
		}
	}

	return bw;
}


int DirecsSerialQext::readData(unsigned char *buf, int nChars, QString callingClassName)
{
	//
	// Original code from method readPort
	// Only using the local member dev_fd, instead of serial ports from laser scanner struct
	//
	int amountRead = 0, bytes_read = 0;
/*
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
*/
	return bytes_read;
}


void DirecsSerialQext::onReadyRead()
{
	QByteArray receivedData; // the data received from the serial port
	qint64 ba = 0; // bytes available
	QString str; // a string to show it
	QChar ch = 0; // the char of the received data
	int dec = 0; // the int of the received data


	// how many bytes are available?
	ba = port->bytesAvailable();

	// position in the string (index!)
	n = 0;

	// if data available (should _always_ be the case, since this method is called automatically by an event)
	if (ba > 0)
	{
		// read data and convert them to a QString
		receivedData = port->readAll();

		// convert from QByteArray to QString
		str = QString::fromUtf8(receivedData.constData());

		// show received data as QString
//		ui->textEdit->insertHtml(QString("%1 byte(s) received. ASCII: %2<br>").arg(ba).arg(str));

		// show each byte
		while (n < receivedData.length())
		{
			// show DEC of each char
			//
			// convert one byte to QChar
			ch = receivedData.at(n);

			// convert to int
			dec = (int) ch.toAscii();

			// show in GUI
//			ui->textEdit->insertHtml(QString("Byte No.%1: %2 (DEC)<br>").arg(n).arg(dec));

			// counter +1
			n++;
		}
	}
}


void DirecsSerialQext::closePort()
{
  port->close();
}
