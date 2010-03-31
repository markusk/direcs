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

#include <QtGlobal> // for Q_OS_* Makro!

#ifndef Q_OS_WIN32 // currently supported under linux and MAC OS (no Windoze at the moment)
#include <termios.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#endif
#ifdef Q_OS_LINUX // currently supported only under linux (no MAC OS, Windoze at the moment)
#include <linux/serial.h>
#endif

#define DEFAULT_LASER_RATE		115200
#define DEFAULT_LASER_PORT		"/dev/tty.USA19Hfa141P1.1" // Keyspan
//#define DEFAULT_LASER_PORT		"/dev/tty.PL2303-000013FD" // Prolific
#define DEFAULT_LASER_SAMPLES	381
#define LASER_MAX_BUFFER_SIZE	1024
#define DEFAULT_LASER_MODE		"request"

#define LASER_CONTINUOUS_MODE	1
#define LASER_REQUEST_MODE		2

// #define HAVE_HI_SPEED_SERIAL	1 // TODO: disabled! Looks like this is needed only for serial lines with 500k baud.

#define READ_TIMEOUT          250000      // less than 1e6 (from direcsSerialPort)

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
		Constructor
		*/
		SickS300();
		
		/**
		Destructor
		*/
		~SickS300();

		/**
		Set up the device.  Return 0 if things go well, and -1 otherwise.
		*/
		int setup();
		
		/**
		Shutdown the device
		*/
		int shutdown();


	private:
		/** @brief Data: scan
		The basic laser data packet.
		*/
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
		
		int readBytes(unsigned char *buf, int nChars);

		int readContinuousTelegram(float *ranges);
		
		int readRequestTelegram(float *ranges);

		player_laser_data_t data; /// the scan data, including the measuread lengths!
		int port_rate;
		const char *device_name;
		int laser_fd;
		int read_mode;
#ifndef Q_OS_WIN32 // currently supported under linux and MAC OS (no Windoze at the moment)
		struct termios oldtio;

#ifdef HAVE_HI_SPEED_SERIAL
		struct serial_struct old_serial;
		#endif
#endif
};

#endif
