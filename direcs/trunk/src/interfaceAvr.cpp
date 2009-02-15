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
	#ifdef _TTY_WIN_
	serialPort = new QextSerialPort();
	#else
	serialPort = new DirecsSerial();
	#endif
	
	robotState = ON; // Wer're thinking positive. The robot is ON untill whe know nothing other. :-)
}


InterfaceAvr::~InterfaceAvr()
{
	delete serialPort;
}


bool InterfaceAvr::openComPort(QString comPort)
{
#ifdef _TTY_WIN_ // Windows code
	if (serialPort->open(QIODevice::ReadWrite) == false)
	{
		qDebug("Error opening serial port! [InterfaceAvr::openComPort]");
		return false;
	}

	// open a serial port ("COM1" for example on Windows) using qextserialport
	serialPort->setBaudRate(BAUD9600);
	
	// Setting of flow control HAS to be AFTER opening an port!
	// If not, a memory access error occurs!
	serialPort->setFlowControl(FLOW_OFF);
	serialPort->setParity(PAR_NONE);
	serialPort->setDataBits(DATA_8);
	serialPort->setStopBits(STOP_1);
	
	// Flushes all pending I/O to the serial port.
	//serialPort->flush();

	return true;
	
#else // Linux AND ARM code using direcsSerial
	
	// for QString to char* conversion
	QByteArray ba = comPort.toLatin1();


	// check if file (serial port) exists
	if (QFile::exists(comPort) == false)
	{
		qDebug("Serial port file not found!");
		return false;
	}
	

	// serial port config also done in openAtmelPort!
	if (serialPort->openAtmelPort( ba.data() ) != -1)
		return true;
	else
		return false;
#endif
}


void InterfaceAvr::closeComPort()
{
#ifdef _TTY_WIN_
	serialPort->close();
#else
	// using direcsSerial
	serialPort->closeAtmelPort();
#endif
}


bool InterfaceAvr::sendChar(unsigned char character)
{
	static int receiveErrorCounter = 0;


#ifdef _TTY_WIN_
	// TODO: which line one was Original?!?? None of it! Original was writeData ?!??
	if (serialPort->putChar(character) == false)
//	if (serialPort->write(&character, 1) == -1)
#else
	// send one byte to the serial port with direcsSerial
	if (serialPort->writeAtmelPort(&character, 1) <= 0)
#endif
	{
		receiveErrorCounter++;
		//qDebug("%d. ERROR sendChar to serial port (sendChar, InterfaceAvr)", receiveErrorCounter);

		// MASSIVE COMMUNICATION ERROR!
		if (receiveErrorCounter >= 4)
		{
			receiveErrorCounter = 0;
			emit tooMuchErrors();
		}
		return false;
	}

	return true;
}


bool InterfaceAvr::receiveChar(unsigned char *character)
{
#ifdef _TTY_WIN_
	// QextSerialPort code, when using Windows
	return serialPort->getChar(character);
#else
	// reading one char with direcsSerial
	return serialPort->readAtmelPort(character, 1); // TODO: check, how many chars are availabe ( numChars() )
#endif
}


bool InterfaceAvr::receiveInt(int *value)
{
	static int receiveErrorCounter = 0;
	unsigned char character = 0;
	int intValue = 0;
	int helpValue = 0;


	//-----------------
	// receive MS-Byte
	//-----------------
	if (receiveChar(&character) == false)
	{
		receiveErrorCounter++;
		//qDebug("%d. ERROR receiving from serial port (receiveInt, InterfaceAvr)", receiveErrorCounter);

		//
		// MASSIVE COMMUNICATION ERROR!
		//
		if (receiveErrorCounter >= 4)
		{
			receiveErrorCounter = 0;
			emit tooMuchErrors();
		}

		value = 0;
		return false;
	}

	// reset error counter
	receiveErrorCounter = 0;


	// bit shifting
	intValue = (character << 8);


	//-----------------
	// receive LS-Byte
	//-----------------
	if (receiveChar(&character) == false)
	{
		//qDebug("ERROR receiving from serial port (receiveInt, InterfaceAvr)");
		value = 0;
		return false;
	}

	
	// build the int value
	// (add the LS-Byte to the MS-Byte)
	*value = (intValue + character);

	return true;
}


void InterfaceAvr::setRobotState(bool state)
{
	robotState = state;
}
