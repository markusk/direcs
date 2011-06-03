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

#ifndef INTERFACEAVR_H
#define INTERFACEAVR_H

#include <QtGlobal> // for Q_OS_* Makro!

#include <qextserialport.h> /// http://code.google.com/qextserialport/

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
		Sends a string to the serial port/device. When sending it prepends a start char and appends a terminator char wich the corresponting receiving Atmel expects.
		Example: *s8# sends the command 's8' to the receiver.
		@param string is the information to send - without starter and terminator!
		@return true on access or false if an error occured.
		*/
		bool sendString(QString string);

		/**
		Receives a string from the serial port/device. It expects a starter at the beginning and a termintator at the end of the string which is currently defined as a const.
		Example: With *42# the 42 is the 'string' here.
		@param &string is the complete result - including the starter and the terminator!
		@return true on access or false if an error occured.
		*/
		bool receiveString(QString &string);

		/**
		Receives an integer value from the serial port/device (consisting of two char values, which are receivced one after another and combined to an int back).
		@param *value
		@return true on access or false if an error occured.
		*/
		bool receiveInt(int *value);

		/**
		Returns an integer value which comes from a former recevied string.
		Example: With *42# the 42 is the 'string' here.
		@param string
		@param &value is the int value when the conversion was successfull.
		@return true on access or false if an error occured.
		*/
		bool convertStringToInt(QString string, int &value);

		/**
		Checks if chars are avialable to be received.

		@return true if chars are available
		**/
		bool charsAvailable();

		/**
		Clears the serial input and output buffer. There may be some unwanted chars in the queue from a former run or so. So this methods just performs a 'flush'.
		*/
		void flush();



	signals:
		/**
		This signal emits if a complete command string was received. @sa SimulationThread::commandReceived()
		@param string contains the received string
		*/
		void commandCompleted(QString string);

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


	private slots:
	   /**
		*/
		void onReadyRead();

		/**
		*/
		void onDsrChanged(bool status);


	private:
		QextSerialPort *serialPort;

		static const bool ON  = true;   /// For robot is "ON"
		static const bool OFF = false;  /// For robot is "OFF"

		static const char starter    = 42; /// This starts the serial string for the Atmel controller.     42  =  *
		static const char terminator = 35; /// This terminates the serial string for the Atmel controller. 35  =  #
};

#endif
