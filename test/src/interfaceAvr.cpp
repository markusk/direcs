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
	serialPort = new LightweightSerial("/dev/tty.USA19Hfa141P1.1", 9600); /// @todo change path to var, add baudrate var

	// let the error messages from the direcsSerial object be transferred to the GUI
	// (connect the signal from the interface class to the signal from this class)
///	connect(serialPort, SIGNAL(message(QString)), this, SIGNAL(message(QString)));
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
		emit message(QString("<font color=\"#FF0000\">ERROR: %1 not found!</font>").arg(comPort));
		// this tells other classes that the robot is OFF!
		emit robotState(false);

		return false;
	}

/*
	// serial port config and flush also done in openAtmelPort!
	if (serialPort->openAtmelPort( ba.data(), 9600 ) == -1)
	{
		// this tells other classes that the robot is OFF!
		emit robotState(false);
		return false;
	}
*/
	/// @todo put open stuff seperately
	return true;
}


void InterfaceAvr::closeComPort()
{
	// using direcsSerial
/// @todo done in destructor	serialPort-> closeAtmelPort();
}


bool InterfaceAvr::sendChar(unsigned char character)
{
	int result = 0;
// 	static int receiveErrorCounter = 0;


	// send one byte to the serial port with direcsSerial
	//emit emitMessage( QString("Sending '%1'.").arg(character) ); // this makes the program to slow and than to crash!!
	result = serialPort->write(character);

	if (result == false)
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
	int result = 0;


	// reading one char with direcsSerial
	// Must return 1 (1 character succussfull read)!
// direcsserial:	result = serialPort->readAtmelPort(character, 1);
	result = serialPort->read(character);

	if (result == false)
	{
		// ERROR
		// emit message( QString("<font color=\"#FF0000\">ERROR '%1' (InterfaceAvr::receiveChar)!<font>").arg(strerror(result)) );  < error message already emitted from readAtmelPort!
		return false;
	}

	// emit emitMessage( QString("Received '%1'.").arg(result) ); // this makes the program to slow and than to crash!!
	return true;
}


bool InterfaceAvr::sendString(QString string)
{
//	QString debugstring;


	// send starter
	if (sendChar(starter) == true)
	{
		// send 'content' of string
//		debugstring = "*";
		for (int i=0; i<string.length(); i++)
		{
			// char by char
			if (sendChar( string.at(i).toAscii() ) == false)
			{
				return false;
			}
//			debugstring.append(string.at(i));
		}

		// send terminator
		if (sendChar(terminator) == true)
		{
			// success
//			debugstring.append("#");
//			emit message(debugstring);
			return true;
		}
	}

	return false;
}


bool InterfaceAvr::receiveString(QString &string)
{
	int result = 0;
	unsigned char character;
	QByteArray ba;


	do
	{
		// reading one char. Must return 1 (one character succussfull read).
		result = serialPort->read(&character);

		if (result == true)
		{
			// append received char to byte array
			ba.append(character);
		}

	} while ( (result == true) && (character != '#') );

	if (result == false)
	{
		// ERROR (error message already emitted from readAtmelPort!)
		qDebug() << "error at receiveString";
		return false;
	}

	// copy chars to QString to pointer to return the QString
	string = QString::fromUtf8(ba.data(), ba.length());

	// check result!
	if ((string.startsWith(starter)) && (string.endsWith(terminator)))
	{
		return true;
	}


	return false;
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
