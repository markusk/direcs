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

#ifndef LASERTHREAD_H
#define LASERTHREAD_H

#include "laser.h"
#include "laserSickS300.h"
#include <QThread>


/**
\brief Handles all the Laser scanners.

It checks, if they are connected, accesses them and emits all read data via Signal. It also provides a simulation mode.
*/
class LaserThread : public QThread
{
    Q_OBJECT

	public:
		LaserThread();
		~LaserThread();
		void stop();
		virtual void run();
		
		/**
		@return the (last) measuread value (distance) in meters(!)
		*/
		float getValue(short int laserScanner, int angle);
		
		/**
		@param laserScanner can be LASER1 or LASER2
		@param angle is the wanted angle in degrees
		@returns the laser scanner line flag (the flag shows if there is an obstacle at this line or not)
		*/
		int getFlag(short int laserScanner, int angle);
		
		/**
		Sets the type of the laser
		@param laserScanner can be LASER1 or LASER2
		@param laserType can be PLS or S300 (others possible, but not implemented completely. @sa Laser::direcs_laser_laser_type_t )
		*/
		void setType(short int laserScanner, QString laserType);

		/**
		This method sets the a flag for each laser line (angle) which represents a free way, an obstackle etc.
		@param laserScanner can be LASER1 or LASER2
		@param angle is the angle in degrees
		@param flag can be FREEWAY, OBSTACLE, LARGESTFREEWAY or CENTEROFLARGESTFREEWAY.
		@sa ObstacleCheckThread
		*/
		void setFlag(short int laserScanner, int angle, int flag);
		
		/**
		Sets the serial port for a laser scanner.
		@param laserScanner can be LASER1 or LASER2
		@param serialPort is the port of the laser scanner
		@sa setDevicePort, read_parameters, laser.h
		*/
		void setSerialPort(short int laserScanner, QString serialPort);
		
		/**
		Sets the mounting for a laser scanner to flip the data for example from 0-179 to 179-0, when the laser scanner is mounted flipped.
		@param laserScanner can be LASER1 or LASER2
		@param mounting can be "normal" or "flipped"
		@sa setDevicePort, read_parameters, laser.h
		*/
		void setMounting(short int laserScanner, QString mounting);

		/**
		Defines the angle of the laserscanner e.g. 180 or 270 degrees.
		@param laserScanner can be LASER1 or LASER2
		@param angle is the angle
		*/
		void setAngle(short int laserScanner, int angle);

		/**
		Defines the resolution of the laserscanner e.g. 0.5 or 1.0 degrees.
		@param laserScanner can be LASER1 or LASER2
		@param resolution can be 0.5 or 1.00 degree
		*/
		void setResolution(short int laserScanner, float resolution);

		/**
		Returns the physical angle of the laserscanner in degrees, e.g. 180 or 270.
		@param laserScanner can be LASER1 or LASER2
		@return the angle in degrees
		*/
		int getAngle(short int laserScanner);

		/**
		Returns the physical resolution of the laserscanner in degrees, e.g. 1.0 or 0.5.
		@param laserScanner can be LASER1 or LASER2
		@return the resolution in degrees
		*/
		float getResolution(short int laserScanner);

		/**
		Returns the state of a connected laser scanner. This method also does some init stuff for all the important mebers like the QList (laser values etc.).
		@param laserScanner can be LASER1 or LASER2
		@return true, if connected
		*/
		bool isConnected(short int laserScanner);
	
	
	public slots:
		/**
		This slot enables or disables the simulation mode.
		*/
		void setSimulationMode(bool status);


	signals:
		/**
		Emits a list containing all front laser values (distances) and their flags.
		*/
		void laserDataCompleteFront(QList <float> laserScannerValuesFront, QList <int> laserScannerFlagsFront);

		/**
		Emits a list containing all rear laser values (distances) and their flags.
		*/
		void laserDataCompleteRear(QList <float> laserScannerValuesRear, QList <int> laserScannerFlagsRear);

		/**
		Emits a info messge to a slot.
		This slot can be used to display a text on a splash screen, log file, to print it to a console...
		*/
		void message(QString text);
		
		/**
		Sends a string over the network.
		*/
		void sendNetworkString(QString text);


	private:
		void getAndStoreLaserValuesFront(); // TODO: use one common getAndStoreLaserValue method!
		void getAndStoreLaserValuesRear();  // TODO: use one common getAndStoreLaserValue method!
		void setSimulationValues(); // Sets the laser line simulatiov values in the QList
		//mutable QMutex mutex; // make this class thread-safe
		volatile bool stopped;
		unsigned char laserscannerTypeFront;
		unsigned char laserscannerTypeRear;
		bool laserScannerFrontIsConnected;
		bool laserScannerRearIsConnected;
		int numReadingsFront;
		int numReadingsRear;
		bool simulationMode;
		QString mountingLaserscannerFront; /// cane be "normal" or "flipped"
		QString mountingLaserscannerRear; /// cane be "normal" or "flipped"
		int laserscannerAngleFront; /// this values holds the range of the laserscanner viewing angle. e.g. 180 or 270 degrees
		int laserscannerAngleRear; /// this values holds the range of the laserscanner viewing angle. e.g. 180 or 270 degrees
		float laserscannerResolutionFront; /// this values holds the resolution of the laserscanner. e.g. 0.5 degrees
		float laserscannerResolutionRear;  /// this values holds the resolution of the laserscanner. e.g. 0.5 degrees

		Laser *laser; // the object for the PLS or LMS laserscanner
		SickS300 *laserS300; // the object for the S300 laserscanner
		
		// Every thread sleeps some time, for having a bit more time fo the other threads!
		// Time in milliseconds
		static const unsigned long THREADSLEEPTIME = 1000; // Default: 150 ms
		
		// FIXME: put this to the ini-file or so. Fix also: read_parameters() in laser.cpp !!
		static const unsigned char LMS = 0; // this is temporary
		static const unsigned char PLS = 1; // this is temporary
		static const unsigned char HOKUYO_URG = 2; // this is not in use, but according to laser.h, direcs_laser_laser_type_t
		static const unsigned char SIMULATED_LASER = 3; // this is not in use, but according to laser.h, direcs_laser_laser_type_t
		static const unsigned char S300 = 4; // this is temporary
		static const unsigned char NONE = 255;

		QList <float> laserScannerValuesFront; /// The measured distances from the front laser scanner. This method also flippes the values when the laser is mounted flipped!
		QList <float> laserScannerValuesRear;  /// The measured distances from the rear laser scanner. This method also flippes the values when the laser is mounted flipped!

		QList <int> laserScannerFlagsFront;  /// Some flags for each front laser line (like "free way", "obstacle" etc.)
		QList <int> laserScannerFlagsRear;   /// Some flags for each rear laser line (like "free way", "obstacle" etc.)

		QList <float> simulationValuesFront;   /// The simulated distances for the front laser scanner.
		QList <float> simulationValuesRear;    /// The simulated distances for the rear laser scanner.

		/**
		The names for addressing the laser scanner array
		*/
//		static const unsigned char DISTANCE = 0;
//		static const unsigned char FLAG = 1;

		/**
		For the integrated DIRECS laser module
		*/
		static const short int LASER1 = 1;
		static const short int LASER2 = 2;
		static const short int LASER3 = 4;
		static const short int LASER4 = 8;
		static const short int LASER5 = 16;

		// the tags for the laser lines
		static const int FREEWAY = 0;
		static const int OBSTACLE = 1;
};

#endif
