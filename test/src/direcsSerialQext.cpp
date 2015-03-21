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
	settings.Timeout_Millisec = 10; // dummy, will be changed by member 'serialReadTimout'

	portOpened = false;

	// create the serial port object.
	// we get the serial data on the port asynchronously!
	port = new QextSerialPort(serialPortName, settings);

	// sets the read and write timeouts for the port to the given milliseconds.
	port->setTimeout(serialReadTimout);
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

		if (bw < 1)
		{
			emit message(QString("<font color=\"#FF0000\">ERROR '%1' when writing to serial port (%2 called from %3.</font>").arg(port->errorString()).arg(className).arg(callingClassName));
		}
	}

	return bw;
}


int DirecsSerialQext::readData(QString &string, QString callingClassName)
{
	QTime startTime; // For measuring elapsed time while waiting for an answer on the serial port
	qint64 ba = 0; // bytes available on the serial port
	qint64 bytesRead = 0;
	char buf[1024]; // stores all recieved data from the serial port

	// the following varaibles are not needed an only fro displaying different formats in the GUI
	QChar ch = 0; // the char of the received data
	int dec = 0;  // the int of the received data


	//
	string.clear();

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

		// if data available
		if (ba > 0)
		{
			// show in GUI / log to file (debugging)
			emit message(QString("<em>%1 byte(s) available.</em>").arg(ba));

			//--------------------------------------------------------------------
			// read a maximum of 'charsToRead' of all available bytes
			//--------------------------------------------------------------------
			bytesRead = port->read(buf, ba);

			// ERROR
			if (bytesRead == -1)
			{
				// show error code and message
				emit message(QString("ERROR %1 at readData: %2").arg(bytesRead).arg(port->errorString()));

				return -1;
			}

			// show in GUI / log to file (debugging)
			emit message(QString("<em>%1 byte(s) received.</em>").arg(bytesRead));

			//
			// store/concat to QString!
			//
			// position in the string (index)
			n = 0;
			while (n < bytesRead)
			{
				// build a QString for convenience
				string.append(buf[n]);

				// show in GUI
				emit message(QString("Byte No.%1: %2 (ASCII) %3 (INT)").arg(n+1).arg(buf[n]).arg((int)buf[n]));

				// counter +1
				n++;
			}

		} // bytes available

	} while (startTime.elapsed() < serialReadTimout);


	// SUCCESS
	if (string.length() > 0)
	{
		return string.length();
	}


	// TIMEOUT ERROR
	emit message(QString("ERROR: Timeout at readData, called from %1").arg(callingClassName));
	return -1;
}


void DirecsSerialQext::closePort()
{
	if (portOpened)
	{
		port->close();
	}
}
