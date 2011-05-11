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

#include "interfaceAvr.h"

InterfaceAvr::InterfaceAvr()
{
	// creating the serial port object
	serialPort = new QextSerialPort(QextSerialPort::Polling);

	// serial port settings
	serialPort->setBaudRate(BAUD9600);
	serialPort->setDataBits(DATA_8);
	serialPort->setParity(PAR_NONE);
	serialPort->setStopBits(STOP_1);
//	serialPort->setDtr(false); /// @todo check if this is needed. By default set to true!
//	serialPort->setRts(false);
	serialPort->setFlowControl(FLOW_OFF);

	//set timeouts to 500 ms
	serialPort->setTimeout(500);

	// let the error messages from the direcsSerial object be transferred to the GUI
	// (connect the signal from the interface class to the signal from this class)
/// @todo	connect(serialPort, SIGNAL(message(QString)), this, SIGNAL(message(QString)));
}


InterfaceAvr::~InterfaceAvr()
{
	if (serialPort->isOpen())
	{
		// flush input and output port
		serialPort->flush();

		serialPort->close();
	}

	delete serialPort;
}


bool InterfaceAvr::openComPort(QString comPort)
{
	//------------------------------------
	// check if file (serial port) exists
	//------------------------------------
	if (QFile::exists(comPort) == false)
	{
		emit message(QString("<font color=\"#FF0000\">ERROR: %1 not found!</font>").arg(comPort));
		// this tells other classes that the robot is OFF!
		emit robotState(false);
		qDebug("openComPort: serial path not found!");
		return false;
	}

	//------------------------------------
	// set port name / path to device
	//------------------------------------
	serialPort->setPortName(comPort);

	//------------------------------------
	// open serial port for read and write
	//------------------------------------
	// 'unbufferd' used because of qextserial example 'QESPTA'.
	if (serialPort->open(QIODevice::ReadWrite | QIODevice::Unbuffered) == false)
	{
		// this tells other classes that the robot is OFF!
		emit robotState(false);
		qDebug("openComPort: error opening serial port!");
		return false;
	}

	//------------------------------------
	// flush serial port
	//------------------------------------
	serialPort->flush();

	qDebug("openComPort: serial port opened.");
	emit robotState(true); /// let the circuit class know, that we opened it

	return true;
}


void InterfaceAvr::closeComPort()
{
	if (serialPort->isOpen())
	{
		// flush input and output port
		serialPort->flush();

		serialPort->close();
	}
}


bool InterfaceAvr::sendChar(unsigned char character)
{
	int result = 0;
// 	static int receiveErrorCounter = 0;

	// convert to QByteArray since write() needs that like this
	QByteArray data;
	data.resize(1);
	data[0] = character;

	// send *one* byte to the serial port with direcsSerial
	// Return code shall be *one*
	if (serialPort->write(data, 1) != 1)
	{
// 		receiveErrorCounter++;
		emit message( QString("<font color=\"#FF0000\">ERROR '%1' (InterfaceAvr::sendChar)!<font>").arg(strerror(result)) );

// 		// MASSIVE COMMUNICATION ERROR!
// 		if (receiveErrorCounter >= 4)
// 		{
// 			receiveErrorCounter = 0;
			emit tooMuchErrors();
// 		}
		return false;
	}

	return true;
}


bool InterfaceAvr::receiveChar(unsigned char *character)
{
	// data available?
	if (serialPort->bytesAvailable() < 1)
	{
		// ERROR
		return false;
	}

	// read data
	// and 'convert' from unsigned char* to char*. See also http://bytes.com/topic/c/answers/720858-convert-unsigned-char-char
	if (serialPort->read((char *) character, 1) != 1)
	{
		// ERROR
		return false;
	}

	return true;
}


bool InterfaceAvr::sendString(QString string)
{
	// add starter
	string.prepend("*");

	// add terminator
	string.append("#");

	qDebug() << "sending string:" << string;

	// sending the string returns the number of chars sent
	if ( (serialPort->write(string.toAscii(), string.length()) != string.length()) )
	{
		return false;
	}

	return true;
}


bool InterfaceAvr::receiveString(QString &string)
{
	char buff[1024];
	int numBytes = 0;

	// data available?
	numBytes = serialPort->bytesAvailable();

	// are data available?
	// if there are >1024 bytes available, something goes wrong here...
	if ((numBytes < 1) || (numBytes > 1024))
	{
		return false;
	}

	if (serialPort->read(buff, numBytes) == -1)
	{
		return false;
	}

	// 'return' data read
	string = buff;

	return true;
}


bool InterfaceAvr::receiveInt(int *value)
{
// 	static int receiveErrorCounter = 0;
	unsigned char character = 0;
	int intValue = 0;


	//-----------------
	// receive MS-Byte
	//-----------------
	if (receiveChar(&character) == false)
	{
// 		receiveErrorCounter++;
		// emit error message already in calling receiveChar!

		//
		// MASSIVE COMMUNICATION ERROR!
		//
// 		if (receiveErrorCounter >= 4)
// 		{
// 			receiveErrorCounter = 0;
//			emit tooMuchErrors();
// 		}

		value = 0;
		return false;
	}

	// reset error counter
// 	receiveErrorCounter = 0;


	// bit shifting
	intValue = (character << 8);


	//-----------------
	// receive LS-Byte
	//-----------------
	if (receiveChar(&character) == false)
	{
		// emit error message already in calling receiveChar!
		value = 0;
		return false;
	}


	// build the int value
	// (add the LS-Byte to the MS-Byte)
	*value = (intValue + character);

	// emit emitMessage( QString("Received int '%1'.").arg(*value) ); // this makes the program to slow and than to crash!!
	return true;
}


bool InterfaceAvr::convertStringToInt(QString string, int &value)
{
	bool conversion = false;


	// remove starter
	string = string.remove(starter);
	// remove terminator
	string = string.remove(terminator);

	// convert to int
	value = string.toInt(&conversion);

	if (conversion)
	{
		return true;
	}

	value = 0;
	return false;
}


bool InterfaceAvr::charsAvailable()
{
	if (serialPort->bytesAvailable() > 0)
	{
		return true;
	}

	return false;
}


void InterfaceAvr::flush()
{
	serialPort->flush();
}
