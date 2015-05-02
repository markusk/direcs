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


#ifndef DIRECS_SERIAL_H
#define DIRECS_SERIAL_H

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

#include <QtGlobal> // for Q_OS_* Makro!

#ifdef CYGWIN
#include <sys/socket.h>
#else

#ifdef Q_OS_LINUX
#include <linux/serial.h>
#endif

#endif

#define READ_TIMEOUT          250000      ///       (the timout for serial reading in microseconds! s.a. 'select' command in @sa readAtmelPort)
#define READ_TIMEOUT_ATMEL    500000      /// 0,25s (the timout for serial reading in microseconds! s.a. 'select' command in @sa readAtmelPort)
#define _POSIX

#include <QtGlobal>
#include <QString>
#include <QDebug>


/**
\author Markus Knapp
\brief Class to read data from and write data to the serial port.

This class is used to communicate with laser scanners and with the robot (with the microcontroller).
*/
class DirecsSerial : public QObject
{
	Q_OBJECT

	public:
		DirecsSerial();
		~DirecsSerial();

		/**
		Open a connection to the serial line.

		The serial port settings (8,N,1) for the serial port are set in this method, too!

		@param dev_name the name of the serial device, e.g. /dev/ttyUSB0 or /dev/ttyS0
		@param baudr The baud rate can be B0, B300, B600, B1200, B2400, B4800, B9600, B19200, B38400, B57600, B115200 or B500000.
		@return the file descriptor if everything is fine, -1 in case of an error.
		**/
		int openPort(char *dev_name, int baudrate);

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
};

#endif
