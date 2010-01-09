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

#include <math.h>  // for M_PI
#include <errno.h> // for errno
#include <unistd.h>
#include <string.h>

#ifdef Q_OS_LINUX // currently supported only under linux (no MAC OS, Windoze at the moment)
#include <termios.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/serial.h>
#endif

#define DEFAULT_LASER_RATE		38400
#define DEFAULT_LASER_PORT		"/dev/ttyS0"
#define DEFAULT_LASER_SAMPLES	381
#define LASER_MAX_BUFFER_SIZE	1024
#define DEFAULT_LASER_MODE		"continuous"	// by default, try continuous mode

#define LASER_CONTINUOUS_MODE	1
#define LASER_REQUEST_MODE		2

#define HAVE_HI_SPEED_SERIAL	1

#include <QtGlobal>
#include <QString>
#include <QDebug>


/**
TODO: to be described...
*/
class SickS300 : public QObject
{
	Q_OBJECT
	
	public:
		/**
		Constructor.  Retrieve options from the configuration file and do any pre-Setup() setup.
		*/
		// org: SickS300(ConfigFile* cf, int section);
		SickS300();
		
		/**
		Destructor
		*/
		~SickS300();

		/**
		Set up the device.  Return 0 if things go well, and -1 otherwise.
		*/
		int setup(); // mk: why virtual?
		
		/**
		Shutdown the device
		*/
		int shutdown();
		
		/**
		Process messages here.  Send a response if necessary, using Publish().
		This method will be invoked on each incoming message.
		If you handle the message successfully, return 0.  Otherwise,
		return -1, and a NACK will be sent for you, if a response is required.
		*/
		// org: int ProcessMessage(MessageQueue* resp_queue, player_msghdr * hdr, void * data);
		// mk FIXME: später		int processMessage(player_msghdr * hdr, void * data);
		// mk: MessageQueue is a class defined in player-3.0.0/libplayercore/message.h

	private:
		// mk 2 start new
		// This is from player-3.0.0/libplayerinterface/player_interfaces.h
		/** @brief Data: scan (@ref PLAYER_LASER_DATA_SCAN)
		
		The basic laser data packet.  */
		typedef struct player_laser_data
		{
			/** Start and end angles for the laser scan [rad].  */
			float min_angle;
			/** Start and end angles for the laser scan [rad].  */
			float max_angle;
			/** Angular resolution [rad].  */
			float resolution;
			/** Maximum range [m]. */
			float max_range;
			/** Number of range readings.  */
			// org uint32_t ranges_count;
			unsigned int ranges_count;
			/** Range readings [m]. */
			float *ranges;
			/** Number of intensity readings */
			// org uint32_t intensity_count;
			unsigned int intensity_count;
			/** Intensity readings. */
			// org uint8_t *intensity;
			unsigned int *intensity;
			/** A unique, increasing, ID for the scan */
			// org uint32_t id;
			unsigned int id;
		} player_laser_data_t;
		// mk 2 end new
		
		/**
		Initialises the laser scanner communication.
		@return 0 on access and any other value on error
		*/
		int init();
		
		/**
		Open the terminal
		Returns 0 on success
		*/
		int openTerm();
		
		/**
		Close the terminal
		Returns 0 on success
		*/
		int closeTerm();
		
		int changeTermSpeed(int speed);
		
		ssize_t readBytes(int fd, unsigned char *buf, size_t count);
		
		int readContinuousTelegram(float *ranges);
		
		int readRequestTelegram(float *ranges);

		//-- mk ab hier neu
		// TODO: maybe not needed? // Create and return a new instance of this driver
		// Driver* SickS300_Init(ConfigFile* cf, int section); 

		// TODO: maybe not needed? 
		// void SickS300_Register(DriverTable* table);

		/*  mk: TODO: not needed?
		Extra stuff for building a shared object.
		extern "C"
		{
			int player_driver_init(DriverTable* table)
		*/
		//-- mk ende hier
	
		player_laser_data_t data; /// the scan data, including the measuread lengths!
		int port_rate;
		const char *device_name;
		int laser_fd;
		struct termios oldtio;
		int read_mode;

#ifdef Q_OS_LINUX // currently supported only under linux (no MAC OS, Windoze at the moment)
		#ifdef HAVE_HI_SPEED_SERIAL
		struct serial_struct old_serial;
		#endif
#endif
};

#endif
