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

#define READ_TIMEOUT          250000      /// the timout for serial reading in micro seconds! s.a. 'select' command in @sa readAtmelPort
#define READ_TIMEOUT_ATMEL    250000      /// the timout for serial reading in micro seconds! s.a. 'select' command in @sa readAtmelPort
#define _POSIX

#include <QtGlobal>
#include <QString>
#include <QDebug>


/**
\brief Class to read data from and write data to the serial port.

This class is used to communicate with the laser scanners and with the robot (with the Atmel controller).
*/
class DirecsSerial : public QObject
{
	Q_OBJECT

	public:
		DirecsSerial();
		~DirecsSerial();

		/**
		Open a connection to the serial line. This method is only used for the Atmel serial port *and* for the SICK laser S300!
		The serial port settings (8,N,1) for the serial port are set in this method, too!!

		@param dev_name the name of the serial device, e.g. /dev/ttyUSB0 or /dev/ttyS0
		@param baudr The baud rate can be B0, B300, B600, B1200, B2400, B4800, B9600, B19200, B38400, B57600, B115200 or B500000.
		@return the file descriptor if everything is fine, -1 in case of an error.
		**/
		int openAtmelPort(char *dev_name, int baudrate);

		/**
		Sets the parameters for (a connected) serial line.

		@param dev_fd The file descriptor associated to the serial line.
		@param baudrate The baud rate to use (e.g. 9600, 19200, etc.).
		@param parity The parity bit can be 0, E, M or S (parityOdd, parityEven, parityM or parityS)
		**/
		void configurePort(int dev_fd, int baudrate, char parity);

		/**
		Returns the number of availabe bytes

		@param dev_fd returns the file descriptor associated to the serial line
		@return number of available bytes or -1 in case of an error.
		**/
		long numChars(int dev_fd);

		/**
		Returns the number of availabe bytes.
		This method is provided for convenience and uses the internal member for the file descriptor. It needs no parameters!

		@return number of available bytes or -1 in case of an error.
		**/
		long numChars();

		/**
		Clears the buffer of the serial line.

		@param *dev_fd returns the file descriptor associated to the serial line
		@return number of bytes removed from the serial line.
		**/
		int clearInputBuffer(int dev_fd);

		/**
		Clears the read buffer.
		@return Upon successful completion, this function returns zero.
		*/
		int purgeRx();

		/**
		Writes data to the serial line

		@param dev_fd The file descriptor associated to the serial line.
		@param *buf Pointer to unsiged char buffer to the data to be send over the serial line
		@param nChars Number of bytes in buf
		@return The number of bytes sent to the serial line.
		**/
		int writePort(int dev_fd, unsigned char *buf, int nChars);

		/**
		Writes data to the serial line.
		This method is only used for the atmel serial port! *Not* for the laser scanners!

		@param *c Pointer to unsiged char buffer to the data to be send over the serial line
		@return The number of bytes sent to the serial line.
		 */
		int writeAtmelPort(unsigned char *c);

		/**
		Reads data from the serial line

		@param dev_fd The file descriptor associated to the serial line.
		@param *buf Pointer to unsiged char buffer for the data to be read
		@param nChars Number of bytes to be read (<= size of the buffer array).
		@return The number of bytes written.
		**/
		int readPort(int dev_fd, unsigned char *buf, int nChars);


		/**
		Reads data from the serial line
		This method is only used for the atmel serial port! *Not* for the laser scanners!

		@param *buf Pointer to unsigned char buffer for the data to be read.
		@param nChars Number of bytes to be written (<= size of the buffer array).
		@return The number of bytes read.
		 */
		int readAtmelPort(unsigned char *buf, int nChars);


		/**
		Closes the serial port.
		This method is only used for the atmel serial port! *Not* for the laser scanners!

		@return the results reported by close.
		**/
		int closeAtmelPort();


		/**
		Sets the serial port parameters
		@param fd The file descriptor associated to the serial port.
		@param baudr The baud rate can be B0, B300, B600, B1200, B2400, B4800, B9600, B19200, B38400, B57600, B115200 or B500000.
		@param par The parity bit can be 0, E, M or S (parityOdd, parityEven, parityM or parityS)
		@param bits The data bits can be 5, 6, 7, 8, bitsM or bitS.
		@param hwf The flow control
		@param swf The flow control
		@param stopb The stop bits (default=0). This parameter is optional!
		*/
		void setParms(int fd, int baudr, char par, char bits, int hwf, int swf, int stopb = 0);

		/**
		Avtivates the low latency mode for the serial line.
		This works with real serial devices, USB-to-RS232 often does not work.
		In this case, the system continues in the standard operational mode.
		Note: Low latency does not run with cygwin.

		@param fd The file descriptor associated to the serial line.
		@return 0=successful switched to low latency mode. 0=continue in normal mode.
		**/
		int setLowLatency(int fd);


	signals:
		/**
		Emits a info or error message to a slot.
		This slot can be used to display a text on a splash screen, log file, to print it to a console...
		*/
		void message(QString text);


	private:
		/**
		Set RTS
		*/
		#if (defined(TIOCM_RTS) && defined(TIOCMODG)) || defined(_COHERENT)
		void setRTS(int fd);
		#else
		void setRTS(int fd  __attribute__ ((unused)));
		#endif

		int mDev_fd; //! the file descriptor of the serial port


		static const char parityOdd = 1;  /// @sa setParms() @sa configurePort()
		static const char parityEven = 2; /// @sa setParms() @sa configurePort()
		static const char parityM = 3;    /// @sa setParms() @sa configurePort()
		static const char parityS = 4;    /// @sa setParms() @sa configurePort()

		static const char bitsM = 1  ;    /// @sa setParms() @sa configurePort()
		static const char bitsS = 2;      /// @sa setParms() @sa configurePort()
};

#endif
