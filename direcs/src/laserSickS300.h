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

#include <libplayercore/playercore.h>

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
 
/**
to be described...
*/
class SickS3000 : public QObject
{
	Q_OBJECT
	
	public:
		SickS3000(ConfigFile* cf, int section);

		virtual int Setup();
		virtual int Shutdown();
		virtual int ProcessMessage(MessageQueue* resp_queue, player_msghdr * hdr, void * data); // This method will be invoked on each incoming message

	private:
		// Main function for device thread.
		virtual void Main();
		int OpenTerm();
		int CloseTerm();
		int ChangeTermSpeed(int speed);
		ssize_t ReadBytes(int fd, unsigned char *buf, size_t count);
		int ReadContinuousTelegram(float *ranges);
		int ReadRequestTelegram(float *ranges);

//-- mk ab hier neu
		/// maybe not needed? // Create and return a new instance of this driver
		Driver* SickS3000_Init(ConfigFile* cf, int section); 

		/// maybe not needed? 
		void SickS3000_Register(DriverTable* table);

		/**
		Constructor.  Retrieve options from the configuration file and do any pre-Setup() setup.
		*/
		SickS3000(ConfigFile* cf, int section) : Driver(cf, section, true, PLAYER_MSGQUEUE_DEFAULT_MAXLEN, PLAYER_LASER_CODE);

		/**
		Set up the device.  Return 0 if things go well, and -1 otherwise.
		*/
		int Setup();

		/**
		Shutdown the device
		*/
		int Shutdown();

		/**
		Close the terminal
		Returns 0 on success
		*/
		int CloseTerm();

		/**
		Process messages here.  Send a response if necessary, using Publish().
		If you handle the message successfully, return 0.  Otherwise,
		return -1, and a NACK will be sent for you, if a response is required.
		*/
		int ProcessMessage(MessageQueue* resp_queue, player_msghdr * hdr, void * data);
		
		/**
		Main function for device thread
		*/
		void Main();

		ssize_t ReadBytes(int fd, unsigned char *buf, size_t count);

		/*
		Open the terminal
		Returns 0 on success
		*/
		int OpenTerm();

		int ReadContinuousTelegram(float *ranges);

		int ReadRequestTelegram(float *ranges);

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
