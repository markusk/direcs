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

#ifndef LASERTHREAD_H
#define LASERTHREAD_H

//-------------------------------------------------------------------
#include "laser_main.h" // modified CARMEN module
#include <QThread>
//-------------------------------------------------------------------

/*!
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
		float getLaserScannerValue(short int laserScanner, int angle);
		float getLaserScannerFlag(short int laserScanner, int angle);
		
		/**
		This method sets the a flag for each laser line (angle) which represents a free way, an obstackle etc.
		@param laserScanner can be LASER1 or LASER2
		@param angle is the angle in degrees
		@param flag can be FREEWAY, OBSTACLE, LARGESTFREEWAY or CENTEROFLARGESTFREEWAY.
		@sa ObstacleCheckThread
		*/
		void setLaserScannerFlag(short int laserScanner, int angle, int flag);
		
		/**
		Sets the serial port for a laser scanner (for modified CARMEN module).
		@param laserScanner can be LASER1 or LASER2
		@param serialPort is the port of the laser scanner
		@sa setDevicePort, read_parameters, laser_main.h
		*/
		void setSerialPort(short int laserScanner, QString serialPort);
		
		/**
		Returns the state of a connected laser scanner.
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
		/** Emits a pointer to the array with the 180 front laser values (distances) */
		void laserDataCompleteFront(float *laserScannerValuesFront, int *laserScannerFlagsFront);
		/** Emits a pointer to the array with the 180 rear laser values (distances) */
		void laserDataCompleteRear(float *laserScannerValuesRear, int *laserScannerFlagsRear);


	private:
		void getAndStoreLaserValuesFront();
		void getAndStoreLaserValuesRear();
		//mutable QMutex mutex; // make this class thread-safe
		volatile bool stopped;
		bool laserScannerFrontIsConnected;
		bool laserScannerRearIsConnected;
		int numReadingsFront;
		int numReadingsRear;
		bool simulationMode;
		
		// Every thread sleeps some time, for having a bit more time fo the other threads!
		// Time in milliseconds
		static const unsigned long THREADSLEEPTIME = 150; // Default: 150 ms
		
		/**
		Defines the size of the laserScannerValue[] array
		This es equal to the number of degrees.
		*/
		static const unsigned char LASERSCANNERARRAYSIZE = 180;
		
		QList <float> laserScannerValuesFront;  /** The measured distances from the front laser scanner. */
		QList <float> laserScannerValuesRear;  /** The measured distances from the rear laser scanner. */
		QList <int> laserScannerFlagsFront;  /** Some flags for each front laser line (like "free way", "obstacle" etc. */
		QList <int> laserScannerFlagsRear;  /** Some flags for each rear laser line (like "free way", "obstacle" etc. */
		
		/**
		The names for addressing the laser scanner array
		*/
		static const unsigned char DISTANCE = 0;
		static const unsigned char FLAG = 1;
		
		/**
		For the integrated CARMEN laser module
		*/
		static const short int LASER1 = 1;
		static const short int LASER2 = 2;
		static const short int LASER3 = 4;
		static const short int LASER4 = 8;
		static const short int LASER5 = 16;
};

#endif
