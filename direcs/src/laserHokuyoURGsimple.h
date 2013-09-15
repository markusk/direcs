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

#ifndef HOKUYOURGSIMPLE_H
#define HOKUYOURGSIMPLE_H

#include <QtGlobal> // for Q_OS_* Makro!
#include <QObject>
#include <QDebug>
#include <QFile>
#include <QTime>


#include "direcsSerial.h"


/**
\author Markus Knapp
@brief Gives access to the simple Hokuyo URG (URG-04LX-UG01 / UURG005) laserscanner / laser range finder.
*/
class HokuyoURGsimple : public QObject
{
	Q_OBJECT

	public:
		/**
		Constructor
		*/
		HokuyoURGsimple();

		/**
		Destructor
		*/
		~HokuyoURGsimple();

		/**
		Set up the device.
		@return 0 if things go well, and -1 otherwise.
		*/
		int setup();

		/**
		Reads a telegram from the laser scanner in the request mode.
		@return 0 if things go well, and -1 otherwise.
		*/
		int readRequestTelegram();

		/**
		Reads a special unknown telegram from the laser scanner in the request mode. It reads block 0B, but I don't know whats stored there...
		@return 0 if things go well, and -1 otherwise.
		*/
		int readUnknownTelegram();

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

		/**
		Returns the measured distance of a specific laser angle index.
		@param angleIndex is the number of the angle. Since we have a half degree resolution, an angeleIndex of 0 means 0.0 deg, 1 means 0.5, 2 means 1.0 degrees, and so on.
		@returns the distance
		*/
		float getDistance(int angleIndex);


	signals:
		/**
		Emits a info or error message to a slot.
		This slot can be used to display a text on a splash screen, log file, to print it to a console...
		@param text is the message to be emitted
		*/
		void message(QString text);


	private:
		DirecsSerial *serialPort;
		int baudRate;
		QString laserSerialPort; /// the path to the serial device. e.g. /dev/tty.usbmodemfa1411

		static const unsigned int LASERSAMPLES = 1082;
		unsigned char scanData[LASERSAMPLES];

		static const float LASERMAXLENGTH = 2000; /// if a measured laser distance is greater than this value in cm, it will be set to 0 cm! (This is due to a bug when reading angle 0)

		float distances[540]; // 540 is the maximum of this laserscanner, because we have 270 degrees at a resolution of 0.5

		static const bool ON  = true;   /// For laser is "ON"
		static const bool OFF = false;  /// For laser is "OFF"
};

#endif
