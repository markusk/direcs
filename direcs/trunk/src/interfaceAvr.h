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

#ifndef INTERFACEAVR_H
#define INTERFACEAVR_H

//-------------------------------------------------------------------
#ifdef _TTY_WIN_ // On Windoze systems use qextserialport (I had problems with this under linux in the past!)
	#include "qextserialport.h"
	class QextSerialPort; // forward declarations because of circular includes!
#else
	#include "direcsSerial.h" // On LINUX and ARM systems use my own serial port class
#endif

#include <QFile>
#include <QObject>
//-------------------------------------------------------------------


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
		bool sendChar(unsigned char character);
		
		/**
		Receives a character from the serial port/device.
		@param character
		@return true on access or false if an error occured.
		*/
		bool receiveChar(unsigned char *character);
		
		/**
		Receives an integer value from the serial port/device (consisting of two char values, which are receivced one after another and combined to an int back).
		@param *value
		@return true on access or false if an error occured.
		*/
		bool receiveInt(int *value);
		
		
	public slots:
		/**
		This slots takes the robot (circuit) state, to know if the robot is ON or OFF.
		When the class knows this, unnecessary communication with the interface can be avoided.
		
		@param state can be ON or OFF
		 */
		void setRobotState(bool state);
	
		
	signals:
		
		/**
		This signal is emmited, when too much errors occur while receiving or sending to the serial port.
		@sa Direcs::showExitDialog()
		*/
		void tooMuchErrors();

	private:
#ifdef _TTY_WIN_
		QextSerialPort *serialPort;
#else
		DirecsSerial *serialPort;
#endif
		bool robotState;
};

#endif
