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

#ifndef LASERTHREAD_H
#define LASERTHREAD_H

#include "laserSickS300.h"
#include "inifile.h" // for reading the sim values
#include <QThread>


/**
\author Markus Knapp
\brief Handles all Laser scanners.

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
		@param laserType can be S300
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
		void setSimulationMode(bool state);

		/**
		Saves the current laser line simulation values to disk (.sim file).
		Resulting filename will look like direcs-YYYY_MM_DD__HH_MM_SS.sim
		@return true on success
		*/
		bool saveLaserData();

		/**
		  This slot sets the measure mode. If activated one can walk around the laser, and the lessest value will only be stored.
		  With this one can set a new alarm distance by walking around the robot.
		  */
		void setMeasureMode(bool state);


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
		Emits a info or error message to a slot.
		This slot can be used to display a text on a splash screen, log file, to print it to a console...
		@param text is the message to be emitted
		*/
		void message(QString text);

		/**
		Sends a string over the network.
		*/
		void sendNetworkString(QString text);

		/**
		Emits a emergency signal for letting other modules know that we have a massive sensor error. So in that case an emergency stop or so could be initiated.
		@param errorlevel needs to be defined. Temporariliy we use -1 in case of error.
		*/
		void systemerror(int errorlevel);


	private:
		void getAndStoreLaserValuesFront(); /// \todo use one common getAndStoreLaserValue method!
		void getAndStoreLaserValuesRear();  /// \todo use one common getAndStoreLaserValue method!
		bool readSimulationValues(); /// Reads the laser line simulation values and sets these as currrent 'real' laser values.
		//mutable QMutex mutex; // make this class thread-safe
		volatile bool stopped;
		unsigned char laserscannerTypeFront;
		unsigned char laserscannerTypeRear;
		bool laserScannerFrontIsConnected;
		bool laserScannerRearIsConnected;
		int numReadingsFront;
		int numReadingsRear;
		bool simulationMode;
		bool measureMode;
		QString mountingLaserscannerFront; /// cane be "normal" or "flipped"
		QString mountingLaserscannerRear; /// cane be "normal" or "flipped"
		int laserscannerAngleFront; /// this values holds the range of the laserscanner viewing angle. e.g. 180 or 270 degrees
		int laserscannerAngleRear; /// this values holds the range of the laserscanner viewing angle. e.g. 180 or 270 degrees
		float laserscannerResolutionFront; /// this values holds the resolution of the laserscanner. e.g. 0.5 degrees
		float laserscannerResolutionRear;  /// this values holds the resolution of the laserscanner. e.g. 0.5 degrees

		SickS300 *laserS300; // the object for the S300 laserscanner
		Inifile *inifile1; //  for reading simulation values
		inifile *inifileLaserdata; // for writing the current laser values to a file

		// Every thread sleeps some time, for having a bit more time fo the other threads!
		// Time in milliseconds
		static const unsigned long THREADSLEEPTIME = 500; // Default: 250 ms

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

		/**
		Nuber of tries reading from laser scanner.
		In case of error, we send the Signal systemerror, which lets the robot stop. In case of good reading, we decrease the errorcounter by one to zero.
		*/
		static const short int MAXERRORS = 2;
};

#endif
