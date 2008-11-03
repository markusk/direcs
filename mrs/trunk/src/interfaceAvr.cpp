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
#ifdef _TTY_POSIX_
	serialPort = new DirecsSerial();
	dev_fd = NULL;
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
#ifdef _TTY_POSIX_
	// for QString to char* conversion
	QByteArray ba = comPort.toLatin1();


	// check if file (serial port) exists
	if (QFile::exists(comPort) == false)
	{
		qDebug("Serial port file not found!");
		return false;
	}
	

	/*
	if (serialPort->openPort(&dev_fd, ba.data()) == -1)
	{
		qDebug("Error opening serial port! [InterfaceAvr::openComPort]");
		return false;
	}

	// Configuring the serial port with 9600, 8N1, no flow control (0, 0)
	serialPort->setParms(dev_fd, "9600", "N", "8", 0, 0, 1);
	*/

	// FIXME: test!!!!!!
	return serialPort->openAtmelPort(&dev_fd, ba.data());
	
	
#else
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
#endif
}


void InterfaceAvr::closeComPort()
{
#ifdef _TTY_POSIX_
	// TODO: no pointer for device parameter?
	serialPort->closePort(dev_fd);
#else
	serialPort->close();
#endif
}


bool InterfaceAvr::sendChar(char character)
{
	static int receiveErrorCounter = 0;


	// send one byte to serial port
#ifdef _TTY_POSIX_
	//--------------------------------------------------
	// convert from signed char to unsigned int
	//--------------------------------------------------
	int helpValue = 0;
	int intValue = 0;
	unsigned char c = intValue;
	
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
	//--------------------------------------------------
	
/*	
	if (serialPort->writePort(dev_fd, &c, 1) == -1)
*/
	
#else
	// FIXME: which one was Original?!?? None of it! Original was writeData ?!??
	if (serialPort->putChar(character) == false)
//	if (serialPort->write(&character, 1) == -1)
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


bool InterfaceAvr::receiveChar(char *character)
{
#ifdef _TTY_POSIX_
	//--------------------------------------------------
	// convert from signed char to unsigned int
	//--------------------------------------------------
	int helpValue = 0;
	int intValue = 0;
	unsigned char c = intValue;
	
	if (character < 0)
	{
		helpValue = ( 127 & *character ) + 128;
	}
	else
	{
		helpValue = *character;
	}
	// this is the 16 Bit result
	intValue += helpValue;
	//--------------------------------------------------
	
	
    // int             readPort(int dev_fd, unsigned char *buf, int nChars);
    return serialPort->readPort(dev_fd, &c, 1);
#else
    return serialPort->getChar(character);
#endif
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

	//--------------------------------------------------
	// convert from signed char to unsigned int
	//--------------------------------------------------
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
	//--------------------------------------------------

	// "return" the value
	*value = intValue;

	return true;
}
