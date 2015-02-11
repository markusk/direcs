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
	settings.BaudRate = BAUD9600; // dummy, can be changed with openPort(baudrate)
	settings.DataBits = DATA_8;
	settings.Parity = PAR_NONE;
	settings.StopBits = STOP_1;
	settings.FlowControl = FLOW_OFF;
	settings.Timeout_Millisec = 10;

	portOpened = false;

	// create the serial port object.
	// we get the serial data on the port "event driven".
	port = new QextSerialPort(serialPortName, settings, QextSerialPort::EventDriven);
}


DirecsSerialQext::~DirecsSerialQext()
{
	delete port;
}


bool DirecsSerialQext::openPort(QString portName, BaudRateType baudrate)
{
	//set port name
	serialPortName = portName;

	// set baudrate
	settings.BaudRate = baudrate;

	port->setPortName(serialPortName);

	// open the serial port
	port->open(QIODevice::ReadWrite | QIODevice::Unbuffered);

	// error opening port
	if (port->isOpen() == false)
	{
		// show error message
		emit message(QString("<font color=\"#FF0000\">ERROR opening serial port %1 (%2).</font>").arg(serialPortName).arg(className));

		return false;
	}

	portOpened = true;

	// success
	return true;
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

		// show sent data / debug...
		// emit message(QString("<font color=\"#0000FF\">%1 byte(s) written. Written value: %2 (DEC) / %3 (HEX) / %4 (ASCII)</font>").arg(bw).arg(value).arg(value, 0, 16).arg(QChar(value)));

		// flush serial port
		port->flush();

		if (bw == -1)
		{
			emit message(QString("<font color=\"#FF0000\">ERROR '%1' when writing to serial port (%2 called from %3.</font>").arg(port->errorString()).arg(className).arg(callingClassName));
		}
	}

	return bw;
}


int DirecsSerialQext::readData(char *buf, int charsToRead, QString callingClassName)
{
	QTime startTime; // For measuring elapsed time while waiting for an answer on the serial port
	qint64 ba = 0; // bytes available on the serial port
	qint64 bytesRead = 0;
	//char buf[1024]; // stores all recieved data from the serial port

	// the following varaibles are not needed an only fro displaying different formats in the GUI
	QChar ch = 0; // the char of the received data
	int dec = 0;  // the int of the received data
	QString str;  // a string to show the received data


	// just to make sure...
	if (port->isOpen() == false)
	{
		emit message("ERROR: serial port not opened!");

		return -1;
	}

	// check if the Arduino sends all data within an wanted time...
	startTime.start();

	do
	{
		// how many bytes are available?
		ba = port->bytesAvailable();

		// if data available (should _always_ be the case, since this method is called automatically by an event)
		if (ba > 0)
		{
			// show message
			emit message(QString("<em>%1 byte(s) available.</em>").arg(ba));

			//--------------------------------------------------------------------
			// read a maximum of 'ba' available bytes into the buffer as char *
			//--------------------------------------------------------------------
//			bytesRead = port->read(buf, ba);
			bytesRead = port->read(buf, charsToRead);

			// ERROR
			if (bytesRead == -1)
			{
				// show error code and message
				emit message(QString("ERROR %1 at readData: %2").arg(bytesRead).arg(port->errorString()));

				return -1;
			}

			// show message
			emit message(QString("<em>%1 byte(s) received.</em><br>").arg(bytesRead));

			// position in the string (index)
			n = 0;

			// show each byte
			while (n < bytesRead)
			{
				// convert char to int
				dec = (int) buf[n];

				// convert chcar to QChar
				ch = buf[n];

				// build a QString for convenience
				str.append(ch);

				// show in GUI
				emit message(QString("Byte No.%1: %2 (ASCII) / %3 (DEC) / %4 (HEX)<br>").arg(n+1).arg(ch).arg(dec).arg(dec, 0, 16));

				// counter +1
				n++;
			}

			// add a new line
			emit message("<br>");


			// FINISHED READING :-)
			if (bytesRead == charsToRead)
			{
				return bytesRead;
			}

		} // bytes available

	} while (startTime.elapsed() < serialReadTimout);

	// show whole string in GUI
	// emit message(QString("Complete String: %1").arg(str));
	// ERROR
	emit message(QString("ERROR: Timeout at readData, called from %1").arg(callingClassName));

	return -1;
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

		// show in GUI / debug...
		// emit message(QString("<font color=\"#0000FF\">%1 byte(s) received. ASCII: %2<font>").arg(ba).arg(str));
		emit message(QString("<font color=\"#0000FF\">%1 byte/s: %2<font>").arg(ba).arg(str));

		// show each byte
		while (n < receivedData.length())
		{
			// show DEC of each char
			//
			// convert one byte to QChar
			ch = receivedData.at(n);

			// convert to int
			dec = (int) ch.toAscii();

			// show in GUI / debug...
			// emit message(QString("<font color=\"#0000FF\">Byte No.%1: %2 (DEC)</font>").arg(n).arg(dec));

			// counter +1
			n++;
		}

		// all data received
		emit dataReceived(str);
	}
}


void DirecsSerialQext::closePort()
{
	if (portOpened)
	{
		port->close();
	}
}
