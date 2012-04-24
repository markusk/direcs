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


/**
\author Markus Knapp
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


	public:
		/**
		Init all important class members, get settings/parameters from laser or sensor thread.
		*/
		void init();


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
		This slot set the minimum distance, which the robot needs. Used by the laser scanner.
		Has to be a seperate method because of the signal from the GUI slider for each laser!
		@param distance in centimeters
		*/
		void setMinObstacleDistanceLaserFront(int distance);

		/**
		This slot set the minimum distance, which the robot needs. Used by the laser scanner.
  Has to be a seperate method because of the signal from the GUI slider for each laser!
		@param distance in centimeters
		*/
		void setMinObstacleDistanceLaserRear(int distance);

		/**
		Set the laser area which will be ignored when checking for an obstacle.
		@param laser is the number of the laser
		@param area is the number of the area
		@param start is the start angle in degrees
		@param end is the end angle in degrees
		*/
		void setIgnoreArea(short int laser, int area, int start, int end);

		/**
		This slot catches all signals from the signal @sa systemerror
		*/
		void systemerrorcatcher(int errorlevel);


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

		/**
		Emits a info or error message to a slot.
		This slot can be used to display a text on a splash screen, log file, to print it to a console...
		@param text is the message to be emitted
		*/
		void message(QString text);


	private:
		QString className;	/// this will contain the name of this class at runtime for debug messages
		SensorThread *sensThread;
		LaserThread *laserThread;
		volatile bool stopped;
		bool initCompleted; /// see if the init method of this class was already called
		int minObstacleDistance; /// if the measured distance from a sensor is *smaller* than this value, than we have an obstacle! This is for the old infrared sensors.
		int minObstacleDistanceLaserFront; /// if the measured distance from a sensor is *smaller* than this value, than we have an obstacle! This is for laser scanner 1.
		int minObstacleDistanceLaserRear; /// if the measured distance from a sensor is *smaller* than this value, than we have an obstacle! This is for laser scanner 2.
		unsigned char sensorValue;
		bool simulationMode;

		int actualFreeAreaStart;
		int actualFreeAreaEnd;

		int largestFreeAreaStart;
		int largestFreeAreaEnd;

		int centerOfFreeWayFront; /// this is user for laser1
		int centerOfFreeWayRear; /// this is user for laser2

		int laserscannerFrontIgnoreArea1Start;/// this area will be ignored when there is an obstacle
		int laserscannerFrontIgnoreArea1End;/// this area will be ignored when there is an obstacle
		int laserscannerFrontIgnoreArea2Start;/// this area will be ignored when there is an obstacle
		int laserscannerFrontIgnoreArea2End;/// this area will be ignored when there is an obstacle

		int laserscannerRearIgnoreArea1Start;/// this area will be ignored when there is an obstacle
		int laserscannerRearIgnoreArea1End;/// this area will be ignored when there is an obstacle
		int laserscannerRearIgnoreArea2Start;/// this area will be ignored when there is an obstacle
		int laserscannerRearIgnoreArea2End;/// this area will be ignored when there is an obstacle

		float laserResolutionFront; /// stores the laser resolution when getting the value from the laser thread
		float laserResolutionRear; /// stores the laser resolution when getting the value from the laser thread
		float laserAngleFront; /// stores the laser angle when getting the value from the laser thread
		float laserAngleRear; /// stores the laser angle when getting the value from the laser thread

		double width;
		double b;
		double c;
		double alpha;

		int robotSlot; /// defines the minimum slot in degrees, where the robot has to fit through
		int robotSlotWidth; /// defines the minimum slot in centimeters (cm), where the robot has to fit through

		int straightForwardDeviation; /// and this is the deviation to 90 degrees, when driving forward

		/// the tags for the laser lines @sa Gui
		static const int FREEWAY = 0;
		static const int OBSTACLE = 1;
		static const int LARGESTFREEWAY = 2;
		static const int CENTEROFLARGESTFREEWAY = 3;
		static const int IGNORETHIS = 4;

		/**
		This are areas to be ignored when checking the laser distances.
		*/
		static const short int AREA1 = 1;
		static const short int AREA2 = 2;

		// Every thread sleeps some time, for having a bit more time fo the other threads!
		// Time in milliseconds
		static const unsigned long THREADSLEEPTIME = 250; // 250 ! original !

		// Give the sensors some names
		//
		// DONT CHANGE THIS NUMBERS!
		// THEY ARE ALSO USED TO ADRESS THE ARRAY "iRSensorValue[]" !!
		//
		/// \todo make these values nicer
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

		/// \todo make these values nicer
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
