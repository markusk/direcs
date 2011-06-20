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
//	serialPort = new LightweightSerial("/dev/tty.USA19Hfa141P1.1", 9600); /// @todo change path to var, add baudrate var
//	serialPort = new LightweightSerial("/dev/tty.PL2303-003014FA", 9600); /// @todo change path to var, add baudrate var
	serialPort = new LightweightSerial("/dev/ttyAtmelBoard", 9600); /// @todo change path to var, add baudrate var

	// let the error messages from the direcsSerial object be transferred to the GUI
	// (connect the signal from the interface class to the signal from this class)
///	connect(serialPort, SIGNAL(message(QString)), this, SIGNAL(message(QString)));
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
	// for QString to char* conversion
	QByteArray ba = comPort.toLatin1();


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

/*
	// serial port config and flush also done in openAtmelPort!
	if (serialPort->openAtmelPort( ba.data(), 9600 ) == -1)
	//------------------------------------
	// set port name / path to device
	//------------------------------------
	serialPort->setPortName(comPort);

	{
		// this tells other classes that the robot is OFF!
		emit robotState(false);
		return false;
	}
*/
	/// @todo put open stuff seperately

	//------------------------------------
	// flush serial port
	//------------------------------------
//	serialPort->flush();

	qDebug("openComPort: serial port opened.");
	emit robotState(true); /// let the circuit class know, that we opened it

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


	qDebug() << "listening for data on" << serialPort->portName();

	emit message("Waiting for Atmel command string start...");

	return true;
}


void InterfaceAvr::closeComPort()
{
	// using direcsSerial
/// @todo done in destructor	serialPort-> closeAtmelPort();
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
	if (serialPort->isOpen())
	{
		emit message("Flusing I/O buffer from serial port.");
		// flush input and output port
		serialPort->flush();
	}
}


void InterfaceAvr::onReadyRead()
{
	QByteArray bytes;
	QByteArray newBytes;
	// - - -
	const int maxStringLength = 32; /// @sa direcs-avr/usart.h: uart_buffer_size
	QString receiveString;
	QString commandString;
	QChar qchar; // this is for conversion from unsigned char to QString
	bool stringStarted = false;
	bool commandComplete = false;
//	static bool redLEDtoggle = false;
	// - - -


	int a = serialPort->bytesAvailable();
	qDebug() << "bytes available:" << a;

	newBytes.resize(a);
	serialPort->read(newBytes.data(), newBytes.size());
//    newBytes.resize(port->readLine(newBytes.data(), 1024));
	qDebug() << "bytes read:" << newBytes.size();
	qDebug() << "bytes:" << newBytes;

//    QByteArray newBytes = port->readAll();
//    qDebug() << newBytes;

	// merge.
	bytes.append(newBytes);

	// copy to receiveString
	receiveString = QString(bytes);

	qDebug() << "total:" << bytes;


	// - - - - - - - - - - - - - - - - - - -
	// - - - - - - - - - - - - - - - - - - -
	// - - - - - - - - - - - - - - - - - - -



	// toggling the red LED on and off with every received serial commmand
//		redLEDtoggle = !redLEDtoggle;
//		emit redLED(redLEDtoggle);


	if (commandComplete == false)
	{
		//---------------------------------------------
		// serial buffer overflow?
		//---------------------------------------------
		if (receiveString.length() > maxStringLength)
		{
				commandComplete = false;
				stringStarted = false;

//			emit greenLED(OFF);

				emit message("<br>", false, false, false);
				emit message("+++ String size exceeded. +++");
				emit message("+++ Discarding chars. +++");
				emit message("Waiting for Atmel command string start...");

				return;
		}


		//---------------------------------------------
		// command string from Atmel started?
		//---------------------------------------------
		if (receiveString.startsWith(starter))
		{
				stringStarted = true;
				commandComplete = false;

				emit message("<br>", false, false, false);
				// send char to GUI (with no CR, but timestamp)
				emit message(QString("%1").arg( qchar ), false, false, true);

//				emit greenLED(ON);
		}
		else
		{
			// send recevied string to GUI)
			emit message(receiveString);

			//-------------------------------------------------
			// wrong starter -> reset received string
			//-------------------------------------------------
			receiveString.clear();
			stringStarted = false;
			commandComplete = false;
		}


		//---------------------------------------------
		// command string from Atmel completed?
		//---------------------------------------------
		if (receiveString.endsWith(terminator))
		{

			// send char to GUI (with CR, but no timestamp)
			emit message(QString("%1").arg( receiveString ), true, false, false);

			commandComplete = true;
			stringStarted = false;

//			emit greenLED(OFF);


			// copy string for command check
			commandString = receiveString;

			emit message(QString("Atmel command string: %1.").arg(commandString));

			//-------------------------------------------------
			// reset received string for next upcoming command
			//-------------------------------------------------
			receiveString.clear();
			stringStarted = false;
			commandComplete = false;

			// emit completed Atmel command
			emit commandCompleted( QString(commandString) );
		}

	} // commandComplete = false

}


void InterfaceAvr::onDsrChanged(bool status)
{
	if (status)
		qDebug() << "device was turned on";
	else
		qDebug() << "device was turned off";
}
