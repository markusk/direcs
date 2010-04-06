/*************************************************************************
*   Copyright (C) 2010 by Markus Knapp                                  *
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

#include <QtGlobal> // for Q_OS_* Makro!
#include <QObject>
#include <QDebug>
#include <QFile>


#include "direcsSerial.h"


/**
@brief Gives access to a SICK laser scanner S300.
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
		Set up the device.
		@return 0 if things go well, and -1 otherwise.
		*/
		int setup();

		/**
		  */
		int readRequestTelegram(float *laserScannerValues);

		/**
		  */
		int readContinuousTelegram(float *ranges);

		/**
		  Sets the serial port. Has to be set before using @sa openComPort. On POSIX systems this looks like "/dev/tty4" or "/dev/ttyUSB0".
		  */
		void setDevicePort(QString serialPort);

		/**
		Opens a serial port/device. On POSIX systems this looks like "/dev/tty4" or "/dev/ttyUSB0".
		@return true on access or false if an error occured.
		*/
		bool openComPort();

		/**
		Closes the serial port
		@return 0 if things go well, and -1 otherwise.
		*/
		int closeComPort();

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


	signals:
		/**
		Sends a string to the GUI log.
		@param text is the message to be emitted
		*/
		void emitMessage(QString text);


	private:
		DirecsSerial *serialPort;
		int baudRate;
		QString laserSerialPort; /// the path to the serial device. e.g. /dev/ttyUSB0
		/** @brief Data: scan
		The basic laser data packet.
		*/
		typedef struct laserData
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
			unsigned int ranges_count;
			/** Range readings [cm]. */
			float *ranges;
			/** Number of intensity readings */
			unsigned int intensity_count;
			/** Intensity readings. */
			unsigned int *intensity;
			/** A unique, increasing, ID for the scan */
			unsigned int id;
		} scanData;

		scanData data; /// the scan data, including the measuread lengths!

		static const unsigned int LASERSAMPLES = 1080;
		static const bool ON  = true;   /// For laser is "ON"
		static const bool OFF = false;  /// For laser is "OFF"
};

#endif
