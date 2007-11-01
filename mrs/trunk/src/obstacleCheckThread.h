#ifndef OBSTACLECHECKTHREAD_H
#define OBSTACLECHECKTHREAD_H

//-------------------------------------------------------------------
#include "sensorThread.h"
#include "laserThread.h"

#include <QTimer>
#include <QThread>
//-------------------------------------------------------------------


// forward declarations because of circular includes!
class SensorThread;


class ObstacleCheckThread : public QThread
{
    Q_OBJECT

	public:
		ObstacleCheckThread(SensorThread *s, LaserThread *l);
		~ObstacleCheckThread();
		void setMinObstacleDistance(int value);
		void setMinObstacleDistanceLaser(int value);
		void stop();
		virtual void run();
		
	
	public slots:
		/*!
		This slot enables or disables the simulation mode.
		*/
		void setSimulationMode(bool status);
		
		/*!
		This slots set the angle, where the robots has to fit thru
		*/
		void setRobotSlot(int angle);


	signals:
		void obstacleDetected(int sensors);


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
		
		/*! defines the minimum slot in degrees, where the robot has to fit thru */
		int robotSlot;
		
		// the tags for the laser lines
		static const int FREEWAY = 0;
		static const int OBSTACLE = 1;
		static const int LARGESTFREEWAY = 2;
		static const int CENTEROFLARGESTFREEWAY = 3;
		
		// Every thread sleeps some time, for having a bit more time fo the other threads!
		// Time in milliseconds
		static const unsigned long THREADSLEEPTIME = 100; // 100 ! original !
		
		// Give the sensors some names
		//
		// DONT CHANGE THIS NUMBERS!
		// THEY ARE ALSO USED TO ADRESS THE ARRAY "iRSensorValue[]" !!
		//
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
		// Value if no sensor has a value to react
		static const short int NONE = 0;
};

#endif
