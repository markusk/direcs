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

#ifndef OBSTACLECHECKTHREAD_H
#define OBSTACLECHECKTHREAD_H

//-------------------------------------------------------------------
#include "sensorThread.h"
#include "laserThread.h"

#include <QTimer>
#include <QDateTime> // for the signal emit with a timestamp
#include <QThread>
//-------------------------------------------------------------------


// forward declarations because of circular includes!
class SensorThread;


/*!
\brief Checks if there is an obstacle in front of any sensor.

 Emits the number of the sensor(s). This value contains the sum of all SENSORx values!
*/
class ObstacleCheckThread : public QThread
{
    Q_OBJECT

	public:
		ObstacleCheckThread(SensorThread *s, LaserThread *l);
		~ObstacleCheckThread();
		void stop();
		virtual void run();
		
	
	public slots:
		/**
		This slot enables or disables the simulation mode.
		*/
		void setSimulationMode(bool status);
		
		/**
		This slot sets the angle, where the robots has to fit through.
		*/
		void setRobotSlot(int angle);

		/**
		This slot sets the width in centimeters (cm), where the robots has to fit through.
		*/
		void setRobotSlotWidth(int width);

		/**
		This slot sets the deviation to 90 degrees when the robot drives forward.
		When the laser angle is 180 degrees, the middle is 90 degrees, at 270 degrees it's 135.
		@param deviation is the angle in degrees.
		*/
		void setStraightForwardDeviation(int deviation);
		
		/**
		This slot set the minimum distance, which the robot needs. Used by the infrared and ultra sonic sensors.
		@param distance in centimeters
		*/
		void setMinObstacleDistance(int distance);
		
		/**
		This slot set the minimum distance, which the robot needs. Used by the Laser scanner.
		@param distance in centimeters
		 */
		void setMinObstacleDistanceLaser(int distance);


	signals:
		/**
		Emits a signal to the logical unit of @sa Direcs
		@param sensors consists of the sum of all infrared and ultrasonic sensor numbers, which had an alarm.
		@param timestamp is the timestmap when the signal was emitted.
		*/
		void obstacleDetected(int sensors, QDateTime timestamp);
		
		/**
		Emits a signal to set the angle where the robot can drive through.
		@param largestFreeAreaStart is the start angle of the largest free area.
		@param largestFreeAreaEnd is the end angle of the largest free area.
		@param centerOfFreeWay is the center of the largest free area.
		@param width of the estimated drive-tru area
		*/
		void newDrivingAngleSet(int largestFreeAreaStart, int largestFreeAreaEnd, int centerOfFreeWay, float width);


	private:
		SensorThread *sensThread;
		LaserThread *laserThread;
		volatile bool stopped;
		int minObstacleDistance;
		int minObstacleDistanceLaserScanner;
		unsigned char sensorValue;
		bool simulationMode;
		
		int actualFreeAreaStart;
		int actualFreeAreaEnd;
		
		int largestFreeAreaStart;
		int largestFreeAreaEnd;
		
		int centerOfFreeWay;
		
		double width;
		double b;
		double c;
		double alpha;
		
		int robotSlot; /// defines the minimum slot in degrees, where the robot has to fit through
		int robotSlotWidth; /// defines the minimum slot in centimeters (cm), where the robot has to fit through

		int straightForwardDeviation; /// and this is the deviation to 90 degrees, when driving forward
	
		// the tags for the laser lines
		static const int FREEWAY = 0;
		static const int OBSTACLE = 1;
		static const int LARGESTFREEWAY = 2;
		static const int CENTEROFLARGESTFREEWAY = 3;
		
		// Every thread sleeps some time, for having a bit more time fo the other threads!
		// Time in milliseconds
		static const unsigned long THREADSLEEPTIME = 250; // 100 ! original !
		
		// Give the sensors some names
		//
		// DONT CHANGE THIS NUMBERS!
		// THEY ARE ALSO USED TO ADRESS THE ARRAY "iRSensorValue[]" !!
		//
		// TODO: make these values nicer
		static const short int SENSOR1 = 1;
		static const short int SENSOR2 = 2;
		static const short int SENSOR3 = 4;
		static const short int SENSOR4 = 8;
		static const short int SENSOR5 = 16;
		static const short int SENSOR6 = 32;
		static const short int SENSOR7 = 64;
		static const short int SENSOR8 = 128;
		
		// ultrasonic sensor
		static const short int SENSOR16 = 256;
		
		// TODO: make these values nicer
		static const short int OBSTACLEFRONTLEFT = 512;
		static const short int OBSTACLEFRONTRIGHT = 1024;
		static const short int OBSTACLESEVERYWHEREINFRONT = 2048;
		
		// Value if no sensor has a value to react
		static const short int NONE = 0;
		
		// the laser scanners
		static const short int LASER1 = 1;
		static const short int LASER2 = 2;
		static const short int LASER3 = 4;
		static const short int LASER4 = 8;
		static const short int LASER5 = 16;
};

#endif
