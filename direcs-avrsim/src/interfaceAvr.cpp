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
	serialPort = new DirecsSerial();
}


InterfaceAvr::~InterfaceAvr()
{
	delete serialPort;
}


bool InterfaceAvr::openComPort(QString comPort)
{
	// for QString to char* conversion
	QByteArray ba = comPort.toLatin1();


	// check if file (serial port) exists
	if (QFile::exists(comPort) == false)
	{
		//qDebug() << "Error opening" << comPort <<"!  [InterfaceAvr::openComPort]";
		
		// this tells other classes that the robot is OFF!
		emit robotState(false);
		
		return false;
	}


	// serial port config also done in openAtmelPort!
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
}


void InterfaceAvr::closeComPort()
{
	// using direcsSerial
	serialPort->closeAtmelPort();
}


bool InterfaceAvr::sendChar(unsigned char character)
{
// 	static int receiveErrorCounter = 0;


	// send one byte to the serial port with direcsSerial
	if (serialPort->writeAtmelPort(&character) <= 0)
	{
// 		receiveErrorCounter++;
		qDebug("*** ERROR writing to serial port (sendChar, InterfaceAvr) ***");

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
	// reading one char with direcsSerial
	// Must return 1 (1 character succussfull read)!
	if (serialPort->readAtmelPort(character, 1) != 1)
	{
		// ERROR
		qDebug("*** ERROR reading from serial port (receiveChar, InterfaceAvr) ***");
		return false;
	}
	
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
		qDebug("*** ERROR reading from serial port (receiveInt, InterfaceAvr) ***");

		//
		// MASSIVE COMMUNICATION ERROR!
		//
// 		if (receiveErrorCounter >= 4)
// 		{
// 			receiveErrorCounter = 0;
			emit tooMuchErrors();
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
		qDebug("*** ERROR reading from serial port (receiveChar, InterfaceAvr) ***");
		value = 0;
		return false;
	}

	
	// build the int value
	// (add the LS-Byte to the MS-Byte)
	*value = (intValue + character);

	return true;
}
