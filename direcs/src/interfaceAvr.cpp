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

#include "interfaceAvr.h"

InterfaceAvr::InterfaceAvr()
{
	// creating the serial port object
	#ifdef Q_OS_LINUX
		serialPort = new DirecsSerial();
	#else
		serialPort = new QextSerialPort();
	#endif
}


InterfaceAvr::~InterfaceAvr()
{
	delete serialPort;
}


bool InterfaceAvr::openComPort(QString comPort)
{
	qDebug() << "searching fo the serial port named " << comPort;
#ifdef Q_OS_LINUX // Linux uses direcsSerial:

	// for QString to char* conversion
	QByteArray ba = comPort.toLatin1();

	// check if file (serial port) exists
	if (QFile::exists(comPort) == false)
	{
		// this tells other classes that the robot is OFF!
		emit robotState(false);

		return false;
	}


	// serial port config and flush also done in openAtmelPort!
	if (serialPort->openAtmelPort( ba.data() ) != -1)
	{
		return true;
	}
	else
	{
		// this tells other classes that the robot is OFF!
		emit robotState(false);

		return false;
	}
#else // other OSes use qextserialport
	if (serialPort->open(QIODevice::ReadWrite) == false) // FIXME: which comport?
	{
		// this tells other classes that the robot is OFF!
		emit robotState(false);

		return false;
	}

	// open a serial port ("COM1" for example on Windows, or /dev/ttyUSB0 on UNIX) using qextserialport
	serialPort->setBaudRate(BAUD9600);

	// Setting of flow control HAS to be AFTER opening an port!
	// If not, a memory access error occurs!
	serialPort->setFlowControl(FLOW_OFF);
	serialPort->setParity(PAR_NONE);
	serialPort->setDataBits(DATA_8);
	serialPort->setStopBits(STOP_1);

	// Flushes all pending I/O to the serial port
	serialPort->flush();

	return true;

#endif
}


void InterfaceAvr::closeComPort()
{
#ifdef Q_OS_LINUX
	// using direcsSerial
	serialPort->closeAtmelPort();
#else
	serialPort->close();
#endif
}


bool InterfaceAvr::sendChar(unsigned char character)
{
// 	static int receiveErrorCounter = 0;

#ifdef Q_OS_LINUX
	// send one byte to the serial port with direcsSerial
	if (serialPort->writeAtmelPort(&character) <= 0)
#else
	// TODO: which line one was Original?!?? None of it! Original was writeData ?!??
	if (serialPort->putChar(character) == false)
		//	if (serialPort->write(&character, 1) == -1)
#endif
	{
// 		receiveErrorCounter++;
 		emit emitMessage("<font color=\"#FF0000\">ERROR writing sserial port (sendChar, InterfaceAvr)!<font>");

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
#ifdef Q_OS_LINUX
	// reading one char with direcsSerial
	// Must return 1 (1 character succussfull read)!
	if (serialPort->readAtmelPort(character, 1) != 1)
	{
		// ERROR
		emit emitMessage("<font color=\"#FF0000\">ERROR reading serial port (receiveChar, InterfaceAvr)!<font>");
		return false;
	}

	return true;
#else
	// QextSerialPort code, when using Mac or Windows
	return serialPort->getChar( (char*) character);
#endif
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

	return true;
}
