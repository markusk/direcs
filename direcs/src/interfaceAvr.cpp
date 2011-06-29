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
	// using 'EventDriven' instead of 'Polling', since last one seems to make some problems ().
	serialPort = new QextSerialPort(QextSerialPort::EventDriven);

	// let the error messages from the direcsSerial object be transferred to the GUI
	// (connect the signal from the interface class to the signal from this class)
/// @todo	connect(serialPort, SIGNAL(message(QString)), this, SIGNAL(message(QString)));

	// commandString is the last command which was received from any other class to be sent to the Atmel via @sa sendString()
	commandString.clear();

	// this is for readyRead
	bytes.clear();
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
//		qDebug("openComPort: serial path not found!");
		return false;
	}

	//------------------------------------
	// set port name / path to device
	//------------------------------------
	serialPort->setPortName(comPort);

	//------------------------------------
	// open serial port for read and write
	//------------------------------------
	// 'Unbuffered' mode removed, since we use the 'EventDriven' mode in the constructor
	if (serialPort->open(QIODevice::ReadWrite | QIODevice::Unbuffered ) == false)
	{
		// this tells other classes that the robot is OFF!
		emit robotState(false);
//		qDebug("openComPort: error opening serial port!");
		return false;
	}

	//------------------------------------
	// flush serial port
	//------------------------------------
//	serialPort->flush();

//	qDebug("openComPort: serial port opened.");
//	emit robotState(true); /// let the circuit class know, that we opened it   << disabled since we only set this to true, when the robot ANSWERS correctly!!

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

	connect(serialPort, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
	connect(serialPort, SIGNAL(dsrChanged(bool)), this, SLOT(onDsrChanged(bool)));

	if (!(serialPort->lineStatus() & LS_DSR))
		qDebug() << "Warning: Serial device is not turned on. This message may not be correct, when working with USB-to-Serial-Converters!";

	qDebug() << "listening for data on" << serialPort->portName();

	emit message("Waiting for Atmel command string start...");

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
	// store the "current command"
	// this will be sent back, when emmiting the commandCompleted Signal!
	lastCommand = string;

	// add starter
	string.prepend("*");

	// add terminator
	string.append("#");

//	qDebug() << "sending string:" << string;

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


	// string i.e. *s7=42#
	if (string.contains(divider))
	{
		// remove everything from '*' to '='
		string.remove(string.indexOf(starter), (string.lastIndexOf(divider) - string.indexOf(starter) + 1));
		// remove terminator
		string = string.remove(terminator);
	}
	else
	{
		// string i.e. *re#
		//
		// remove starter
		string = string.remove(starter);
		// remove terminator
		string = string.remove(terminator);
	}

	// convert remaing string to int
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
	if (serialPort->isOpen())
	{
		emit message("Flusing I/O buffer from serial port.");
		// flush input and output port
		serialPort->flush();
	}
}


void InterfaceAvr::onReadyRead()
{
	//	static bool redLEDtoggle = false;
	const int maxStringLength = 32; /// @sa direcs-avr/usart.h: uart_buffer_size


	// check how many bytes are available.
	// Since this Slot is called automatically, there *have* to be some available
	int a = serialPort->bytesAvailable();
	//emit message(QString("%1 bytes available").arg(a));

	// this should never happen
	if (a==0)
	{
		emit message("strange...");
		return;
	}

	// reserve space
	bytes.resize(a);

	//--------------------------
	// read all available bytes
	//--------------------------
	serialPort->read(bytes.data(), bytes.size());

	//--------------------------
	// append to receiveString
	//--------------------------
	commandString.append(QString(bytes));
	//emit message(QString("commandString=%1").arg(commandString));

	// toggling the red LED on and off with every received serial stuff
	//	redLEDtoggle = !redLEDtoggle;
	//	emit redLED(redLEDtoggle);

	//-------------------------------------------
	// serial buffer 'overflow?'
	/// @sa direcs-avr/usart.h: uart_buffer_size
	//-------------------------------------------
	if (commandString.length() > maxStringLength)
	{
		commandString.clear();
		bytes.clear();

		// emit greenLED(OFF);
		emit message("+++ String size exceeded. +++");
		emit message("+++ Discarding chars. +++");

		return;
	}


	//------------------------
	// command string start ?
	// but not yet completed !
	//------------------------
	if ((commandString.startsWith(starter)) && !(commandString.endsWith(terminator)))
	{
		// emit greenLED(ON);

		// return and wait for next bytes..
		return;
	}


	//----------------------------------------
	// correct terminator, but wrong starter
	//----------------------------------------
	if ((commandString.endsWith(terminator)) &&  !(commandString.startsWith(starter)))
	{
		emit message(QString("ERROR: String '%1' did not start with '%2'!").arg(commandString).arg(starter));

		commandString.clear();
		bytes.clear();

		return;
	}


	//----------------------------
	// command string completed ?
	//----------------------------
	if ((commandString.startsWith(starter)) && (commandString.endsWith(terminator)))
	{
		//  emit greenLED(OFF);
		emit message(QString("Atmel says: %1").arg(commandString));

		//  emit completed Atmel command
		emit commandCompleted(commandString, lastCommand);

		commandString.clear();
		bytes.clear();

		return;
	}


	emit message("ERROR: This line should never be reached (interfaceAvr::onReadyRead).");
}


void InterfaceAvr::onDsrChanged(bool status)
{
	if (status)
		qDebug() << "device was turned on";
	else
		qDebug() << "device was turned off";
}
