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

#ifndef LASERSICKS300_H
#define LASERSICKS300_H

#include <unistd.h>
#include <string.h>

#include <termios.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/serial.h>

// mk #include <libplayercore/playercore.h>


// mk start new (instead of including libplayercore/playercore.h)
// This is from player-3.0.0/libplayercommon/error.h
/// @internal Message types (internal use only; code should use the macros)
#define PLAYER_ERR_ERR 0
#define PLAYER_ERR_WARN 1
#define PLAYER_ERR_MSG 2
#define PLAYER_ERR_DBG 2

/// Error message macros
#define PLAYER_ERROR(msg)         ErrorPrint(PLAYER_ERR_ERR, 0, __FILE__, __LINE__, "error   : " msg "\n")
#define PLAYER_ERROR1(msg, a)     ErrorPrint(PLAYER_ERR_ERR, 0, __FILE__, __LINE__, "error   : " msg "\n", a)
#define PLAYER_ERROR2(msg, a, b)  ErrorPrint(PLAYER_ERR_ERR, 0, __FILE__, __LINE__, "error   : " msg "\n", a, b)

/// Warning message macros
#define PLAYER_WARN(msg)        ErrorPrint(PLAYER_ERR_WARN, 0, __FILE__, __LINE__, "warning : " msg "\n")

/// General messages.  Use level to indicate the message importance
///  - 0 : important
///  - 1 : informative
///  - 2+ : diagnostic
/// All messages are recorded in the log file, but only the more important
/// messages are printed on the console.  Use the command line option to
/// dictate which messages will be printed.
#define PLAYER_MSG0(level, msg) ErrorPrint(PLAYER_ERR_MSG, level, __FILE__, __LINE__, "" msg "\n") 

// mk end new


#define DEFAULT_LASER_RATE 38400
#define DEFAULT_LASER_PORT "/dev/ttyS0"
#define DEFAULT_LASER_SAMPLES 381
#define LASER_MAX_BUFFER_SIZE 1024
#define DEFAULT_LASER_MODE "continuous" /* by default, try continuous mode */

#define LASER_CONTINUOUS_MODE 1
#define LASER_REQUEST_MODE 2

#define HAVE_HI_SPEED_SERIAL 1

// Error macros
#define RETURN_ERROR(erc, m) {PLAYER_ERROR(m); return erc;}

#include <QtGlobal>
#include <QString>
#include <QDebug>


/**
to be described...
*/
class SickS3000 : public QObject
{
	Q_OBJECT
	
	public:
		/**
		Constructor.  Retrieve options from the configuration file and do any pre-Setup() setup.
		*/
		// org: SickS3000(ConfigFile* cf, int section);
		SickS3000();

		/**
		Set up the device.  Return 0 if things go well, and -1 otherwise.
		*/
		int Setup(); // mk: why virtual?
		
		/**
		Shutdown the device
		*/
		int Shutdown();
		
		/**
		Process messages here.  Send a response if necessary, using Publish().
		This method will be invoked on each incoming message.
		If you handle the message successfully, return 0.  Otherwise,
		return -1, and a NACK will be sent for you, if a response is required.
		*/
		// org: int ProcessMessage(MessageQueue* resp_queue, player_msghdr * hdr, void * data);
// FIXME: später		int ProcessMessage(player_msghdr * hdr, void * data);
		// mk: MessageQueue is a class defined in player-3.0.0/libplayercore/message.h

	private:
		/**
		Main function for device thread
		*/
		void Main();
		
		/**
		Open the terminal
		Returns 0 on success
		*/
		int OpenTerm();
		
		/**
		Close the terminal
		Returns 0 on success
		*/
		int CloseTerm();
		
		int ChangeTermSpeed(int speed);
		
		ssize_t ReadBytes(int fd, unsigned char *buf, size_t count);
		
		int ReadContinuousTelegram(float *ranges);
		
		int ReadRequestTelegram(float *ranges);

//-- mk ab hier neu
		/// maybe not needed? // Create and return a new instance of this driver
// 		Driver* SickS3000_Init(ConfigFile* cf, int section); 

		/// maybe not needed? 
// 		void SickS3000_Register(DriverTable* table);

		/*  mk: not needed?
		Extra stuff for building a shared object.
		extern "C" {
			int player_driver_init(DriverTable* table)
		*/
		//-- mk ende hier

		int port_rate;
		const char *device_name;
		int laser_fd;
		struct termios oldtio;
		int read_mode;

		#ifdef HAVE_HI_SPEED_SERIAL
		struct serial_struct old_serial;
		#endif
};

#endif
