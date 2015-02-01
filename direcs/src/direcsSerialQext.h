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

#include <errno.h>
#include <fcntl.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/time.h>

#include <qextserialport.h> /// This is for serial port communication
#include <qextserialenumerator.h> /// This is for getting a list of serial ports

#include <QtGlobal> // for Q_OS_* Makro!

#ifdef CYGWIN
#include <sys/socket.h>
#else

#ifdef Q_OS_LINUX
#include <linux/serial.h>
#endif

#endif

#define READ_TIMEOUT          250000      /// the timout for serial reading in microseconds! s.a. 'select' command in @sa readAtmelPort
#define READ_TIMEOUT_ATMEL    250000      /// the timout for serial reading in microseconds! s.a. 'select' command in @sa readAtmelPort
#define _POSIX

#include <QtGlobal>
#include <QString>
#include <QDebug>


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
		 * @return true on success
		 */
		bool openSerialPort();

		/**
		Clears the read buffer.

		@return Upon successful completion, this function returns zero.
		*/
		int purgeRx();

		/**
		Writes data to the serial line.

		@param *c Pointer to unsiged char buffer to the data to be send over the serial line
		@param callingClassName may contain the name of the calling class. This is for debug messages only.
		@return The number of bytes sent to the serial line.
		 */
		int writeData(unsigned char *c, QString callingClassName = "none");

		/**
		Reads data from the serial line

		@param *buf Pointer to unsigned char buffer for the data to be read.
		@param nChars Number of bytes to be written (<= size of the buffer array).
		@param callingClassName may contain the name of the calling class. This is for debug messages only.
		@return The number of bytes read.
		 */
		int readData(unsigned char *buf, int nChars, QString callingClassName = "none");

		/**
		Closes the serial port.

		@return the results reported by close.
		**/
		int closePort();


	signals:
		/**
		Emits a info or error message to a slot.
		This slot can be used to display a text on a splash screen, log file, to print it to a console...
		*/
		void message(QString text);


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
};

#endif
