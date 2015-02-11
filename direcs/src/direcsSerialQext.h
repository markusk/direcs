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


#ifndef DIRECS_SERIAL_QEXT_H
#define DIRECS_SERIAL_QEXT_H

#include <qextserialport.h> /// This is for serial port communication
#include <qextserialenumerator.h> /// This is for getting a list of serial ports

#include <QString>
#include <QTime> /// For measuring elapsed time while waiting for an answer on the serial port


/**
\author Markus Knapp
\brief Class to read data from and write data to the serial port.

This class is used to communicate with laser scanners and with the robot (with the microcontroller).
*/
class DirecsSerialQext : public QObject
{
	Q_OBJECT

	public:
		DirecsSerialQext();
		~DirecsSerialQext();

		/**
		 * Open a connection to the serial line.
		 * @param portName i.e. /dev/tty.usbmodemfd1411
		 * @param baudrate i.e. BAUD9600
		 * @return true on success
		 */
		bool openPort(QString portName, BaudRateType baudrate);

		/**
		 * Flushes the serial buffer.
		*/
		void purgeRx(); /// @todo rename to flush

		/**
		Writes data to the serial line.

		@param *c Pointer to unsiged char buffer to the data to be send over the serial line
		@param callingClassName may contain the name of the calling class. This is for debug messages only.
		@return The number of bytes sent to the serial line.
		 */
		int writeData(int value, QString callingClassName);

		/**
		Reads data from the serial line

		@param *buf Pointer to char buffer for the data to be read.
		@param nChars Number of bytes to be reaad (<= size of the buffer array).
		@param callingClassName may contain the name of the calling class. This is for debug messages only.
		@return The number of bytes read.
		 */
		int readData(char *buf, int charsToRead, QString callingClassName = "none");

		/**
		 * Closes the serial port.
		**/
		void closePort();


	signals:
		/**
		Emits a info or error message to a slot.
		This slot can be used to display a text on a splash screen, log file, to print it to a console...
		*/
		void message(QString text);

		/**
		  * This signal is sent when the received data are 'complete'
		  * @param data contains the data as a QString
		**/
		void dataReceived(QString data);


	private slots:
		/**
		 * @brief onReadyRead is automatically called, when data on the serial port are available.
		 */
		void onReadyRead();


	private:
		QString className;	/// this will contain the name of this class at runtime for debug messages

		int mDev_fd; //! the file descriptor of the serial port

		QextSerialPort *port; /// The serial port
		/// the settings for the serial port (Baudrate, Parity etc.)
		/// Be aware, that the Arduino has to use the same speed (9600 Baud)
		PortSettings settings;

		QextSerialEnumerator *enumerator; /// This is for getting a list of serial ports (filenames like /dev/ttyUSB0)
		QString serialPortName; /// for the (file)name of the serial port, like /dev/ttyUSB0 or COM1
		int n;
		bool portOpened; /// will be set in port open and checked in close port to avoid crash when trying to close an unopend port

		static const int serialReadTimout = 500; /// time in ms for waiting for an answer for all bytes. @sa readData
};

#endif
