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

#include "interfaceAvr.h"

InterfaceAvr::InterfaceAvr()
{
	// creating the serial port object
	//serialPort = new QextSerialPort("/dev/ttyUSB0"); // < < < < < <
	serialPort = new QextSerialPort();
}


InterfaceAvr::~InterfaceAvr()
{
	delete serialPort;
}


bool InterfaceAvr::openComPort(QString comPort)
{
	// check if file (serial port) exists
	if (QFile::exists(comPort) == false)
	{
		qDebug("Serial port file not found!");
		return false;
	}


	//-------------------------------------------------------
	// open a serial port ("COM1" for example on Windows)
	// using qextserialport
	//-------------------------------------------------------
//	serialPort->setPortName(comPort);
	serialPort->setBaudRate(BAUD9600);
	// Due to a bug in posix_qextserialport.h setting of flow control HAS to be AFTER opening an port!?!?!??
	// If not, a memory access error occurs!
	serialPort->setFlowControl(FLOW_OFF);
	serialPort->setParity(PAR_NONE);
	serialPort->setDataBits(DATA_8);
	serialPort->setStopBits(STOP_1);
	//serialPort->setTimeout(0, 100); // setting time out to 0 seconds and 100 millliseconds


//	return serialPort->open(QIODevice::ReadWrite);

	if (serialPort->open(QIODevice::ReadWrite) == false)
	{
		qDebug("What the...");
		return false;
	}

	// Flushes all pending I/O to the serial port.
	//serialPort->flush();

	return true;
}


void InterfaceAvr::closeComPort()
{
	serialPort->close();
}


bool InterfaceAvr::sendChar(char character)
{
	static int receiveErrorCounter = 0;

	//------------------------------------------
	// send one byte to serial port
	//------------------------------------------
	if (serialPort->putChar(character) == false)
//	if (serialPort->write(&character, 1) == -1)
	{
		receiveErrorCounter++;
		//qDebug("%d. ERROR sendChar to serial port (sendChar, InterfaceAvr)", receiveErrorCounter);

		//
		// MASSIVE COMMUNICATION ERROR!
		//
		if (receiveErrorCounter >= 4)
		{
			receiveErrorCounter = 0;
			emit tooMuchErrors();
		}

		return false;
	}


	return true;
}


bool InterfaceAvr::receiveChar(char *character)
{
	return serialPort->getChar(character);
}


bool InterfaceAvr::receiveInt(int *value)
{
	static int receiveErrorCounter = 0;
	char character = 0;
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

	// convert from signed char to unsigned int
	if (character < 0)
	{
		helpValue = ( 127 & character ) + 128;
	}
	else
	{
		helpValue = character;
	}


	// this is the 16 Bit result
	intValue += helpValue;

	// "return" the value
	*value = intValue;

	return true;
}
