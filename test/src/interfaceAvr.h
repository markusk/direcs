/*************************************************************************
 *   Copyright (C) Markus Knapp                                          *
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

#include <QtGlobal> // for Q_OS_* Makro!

#include "direcsSerial.h"

#include <QFile>
#include <QObject>
#include <QDebug>


/**
\author Markus Knapp
\brief Provides the low level communication between the robots microcontroller and the laptop.

This class is used for the low level communication between the robots microcontroller and the laptop via a serial port, e.g. COM1 or /dev/ttyUSB0.
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
		@param callingClassName may contain the name of the calling class. This is for debug messages only.
		@return true on access or false if an error occured.
		*/
		bool sendChar(unsigned char character, QString callingClassName = "none");

		/**
		Receives a character from the serial port/device.
		@param character
		@param callingClassName may contain the name of the calling class. This is for debug messages only.
		@return true on access or false if an error occured.
		*/
		bool receiveChar(char *character, QString callingClassName = "none");

		/**
		Sends a string to the serial port/device. When sending it prepends a start char and appends a terminator char wich the corresponting receiving Atmel expects.
		Example: *s8# sends the command 's8' to the receiver.
		@param string is the information to send - without starter and terminator!
		@param callingClassName may contain the name of the calling class. This is for debug messages only.
		@return true on access or false if an error occured.
		*/
		bool sendString(QString string, QString callingClassName = "none");

		/**
		Receives a string from the serial port/device. It expects a starter at the beginning and a termintator at the end of the string which is currently defined as a const.
		Example: With *42# the 42 is the 'string' here.
		@param &string is the complete result - including the starter and the terminator!
		@param callingClassName may contain the name of the calling class. This is for debug messages only.
		@return true on access or false if an error occured.
		*/
		bool receiveString(QString &string, QString callingClassName = "none");

		/**
		Receives an integer value from the serial port/device (consisting of two char values, which are receivced one after another and combined to an int back).
		@param *value
		@param callingClassName may contain the name of the calling class. This is for debug messages only.
		@return true on access or false if an error occured.
		*/
		bool receiveInt(int *value, QString callingClassName = "none");

		/**
		Returns an integer value which comes from a former recevied string.
		Example: With *42# the 42 is the 'string' here.
		@param string
		@param &value is the int value when the conversion was successfull.
		@return true on access or false if an error occured.
		*/
		bool convertStringToInt(QString string, int &value);


	signals:
		/**
		This signal emits the robots state to all connected slots, to tell them if the robot is ON or OFF
		This signal is emitted, when the openComPort() method fails!
		@param state can be ON or OFF
		*/
		void robotState(bool state);

		/**
		This signal is emmited, when too much errors occur while receiving or sending to the serial port.
		@sa Direcs::showExitDialog()
		*/
		void tooMuchErrors();

		/**
		Emits a info or error message to a slot.
		This slot can be used to display a text on a splash screen, log file, to print it to a console...
		@param text is the message to be emitted
		*/
		void message(QString text, bool CR=true, bool sayIt=false, bool addTimestamp=true);


	private:
		QString className;	/// this will contain the name of _this_ class at runtime for debug messages
		DirecsSerial *serialPort;
		QString answer; /// this will contain the event driven answer when data are received @sa receiveData Slot.
		const int MAXCOMMANDLENGTH = 32; /// Max. length of a command from the Arduino.
		bool commandStarted;
		bool commandCompleted;


		static const bool ON  = true;   /// For robot is "ON"
		static const bool OFF = false;  /// For robot is "OFF"

		const int BAUDRATE = 9600;	/// This is the baudrate with which the serial port will be opened. The other (not parameterized) settings are "8N1".

		static const char starter    = 42; /// This starts the serial string for the Atmel controller.     42  =  *
		static const char terminator = 35; /// This terminates the serial string for the Atmel controller. 35  =  #
};

#endif
