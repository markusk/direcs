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

#ifndef INTERFACEAVR_H
#define INTERFACEAVR_H

//-------------------------------------------------------------------
#include "qextserialport.h"
#include <QObject>
//-------------------------------------------------------------------

// forward declarations because of circular includes!
class QextSerialPort;


/**
\brief Provides the low level communication between the robots microcontroller and the laptop.

This class is used for the low level communication between the robots microcontroller and the laptop. It is based on qextserialport for the serial communication.
*/
class InterfaceAvr : public QObject
{
    Q_OBJECT
	
	public:
		InterfaceAvr();
		~InterfaceAvr();
		
		/**
		Opens a serial port/device. On POSIX systems this looks like "/dev/tty4" oder "/dev/ttyUSB0". On Windows systems it's like "COM1".
		@param comPort is the serial port
		@return true on access or false if an error occured, while opening the device.
		*/
		bool openComPort(QString comPort);
		
		/**
		Closes the serial port
		*/
		void closeComPort();
		
		/**
		Sends a character to the serial port/device.
		@param character
		@return true on access or false if an error occured.
		*/
		bool sendChar(char character);
		
		/**
		Receives a character from the serial port/device.
		@param character
		@return true on access or false if an error occured.
		*/
		bool receiveChar(char *character);
		
		/**
		Receives an integer value from the serial port/device (consisting of two char values, which are receivced one after another and combined to an int back).
		@param *value
		@return true on access or false if an error occured.
		*/
		bool receiveInt(int *value);
	
	signals:
		
		/**
		This signal is emmited, when too much errors occur while receiving or sending to the serial port.
		@sa Mrs::showExitDialog()
		*/
		void tooMuchErrors();

	private:
		QextSerialPort *serialPort;
};

#endif
