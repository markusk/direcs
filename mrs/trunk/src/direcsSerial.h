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

/**
Library to read data from and write data to the serial line.
*/


#ifndef DIRECS_SERIAL_H
#define DIRECS_SERIAL_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/time.h>

#ifdef CYGWIN
#include <sys/socket.h>
#else
#include <linux/serial.h>
#endif

#define READ_TIMEOUT          250000      /* less than 1e6 */
#define _POSIX

#include <QtGlobal>
#include <QString>
#include <QDebug>


class DirecsSerial : public QObject
{
    Q_OBJECT
	
	public:
		DirecsSerial();
		~DirecsSerial();
	
		/**
		Open a connection to the serial line 
		
		@param *dev_fd returns the file descriptor associated to the serial line
		@param dev_name the name of the serial device, e.g. /dev/ttyS0
		@return 0 if everything is fine, -1 in case of an error.
		**/
		int openPort(int *dev_fd, char *dev_name); // serial_connect
		
		
		/**
		Sets the parameters for (a connected) serial line.
		
		@param dev_fd The file descriptor associated to the serial line.
		@param baudrate The baud rate to use (e.g. 9600, 19200, etc.).
		@param parity The parity to use (e.g. "N" for none).
		**/
		void configurePort(int dev_fd, int baudrate, char *parity);
		
		/**
		Returns the number of availabe bytes
		
		@param *dev_fd returns the file descriptor associated to the serial line
		@return number of available bytes or -1 in case of an error.
		**/
		long numChars(int dev_fd);
		
		/**
		Clears the buffer of the serial line.
		
		@param *dev_fd returns the file descriptor associated to the serial line
		@return number of bytes removed from the serial line.
		**/
		int clearInputBuffer(int dev_fd);
		
		/**
		Writes data to the serial line
		
		@param dev_fd The file descriptor associated to the serial line.
		@param *buf Pointer to unsiged char buffer to the data to be send over the serial line
		@param nChars Number of bytes in buf
		@return The number of bytes sent to the serial line.
		**/
		int writePort(int dev_fd, unsigned char *buf, int nChars);
		
		/**
		Reads data from the serial line
		
		@param dev_fd The file descriptor associated to the serial line.
		@param *buf Pointer to unsiged char buffer for the data to be read
		@param nChars Number of bytes to be read (<= size of the buffer array).
		@return The number of bytes read.
		**/
		int readPort(int dev_fd, unsigned char *buf, int nChars);
		
		/**
		Closes the file descriptior.
		
		@param dev_fd The file descriptor associated to the serial line.
		@return The results reported by close.
		**/
		int closePort(int dev_fd);
		
		/**
		Avtivates the low latency mode for the serial line.
		This works with real serial devices, USB-to-RS232 often does not work.
		In this case, the system continues in the standard operational mode.
		Note: Low latency does not run with cygwin.
		
		@param fd The file descriptor associated to the serial line.
		@return 0=successful switched to low latency mode. 0=continue in normal mode.
		**/
		int setLowLatency(int fd);
		
	private:
		/**
		Set RTS
		*/
		#if (defined(TIOCM_RTS) && defined(TIOCMODG)) || defined(_COHERENT)
		void setRTS(int fd);
		#else
		void setRTS(int fd  __attribute__ ((unused)));
		#endif

		/**
		*/
		void setParms(int fd, char *baudr, char *par, char *bits, int hwf, int swf);
};

#endif
