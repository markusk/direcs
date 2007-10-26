//
// C++ Interface: laserThread
//
// Description: 
//
//
// Author: Markus Knapp <webmaster@direcs.de>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef LASERTHREAD_H
#define LASERTHREAD_H

//-------------------------------------------------------------------
#include "laser.h"
#include <QThread>
//-------------------------------------------------------------------


class LaserThread : public QThread
{
    Q_OBJECT

	public:
		LaserThread();
		~LaserThread();
		void stop();
		virtual void run();
		float getLaserScannerValue(int angle);
		float getLaserScannerFlag(int angle);
		void setLaserScannerFlag(int angle, int flag);
		int getNumReadings();
		
	
	public slots:
		/*!
		This slot enables or disables the simulation mode.
		*/
		void setSimulationMode(bool status);


	signals:
		/*! emits a pointer to the array with the 180 laser values (distances) */
		void laserDataComplete();


	private:
		int numReadings;
		//mutable QMutex mutex; // make this class thread-safe
		volatile bool stopped;
		bool simulationMode;
		
		// Every thread sleeps some time, for having a bit more time fo the other threads!
		// Time in milliseconds
		static const unsigned long THREADSLEEPTIME = 500; // Default: 500 ms
		
		/*!
		Defines the size of the laserScannerValue[] array
		This es equal to the number of degrees. */
		static const unsigned char LASERSCANNERARRAYSIZE = 181;
		
		/*!
		The first column of this array is the measured distance to an object (the length of the real laser line in meter (m)).
		The second column is an flag if an obstacle is detected (the distance is to small from the robot to an object)
		*/
		// float laserScannerValue[LASERSCANNERARRAYSIZE][2];
		QList <float> laserScannerValues;  /** The measured distances from the laser scanner. */
		QList <int> laserScannerFlags;  /** Some flags for each laser line (like "free way", "obstacle" etc. */
		
		/*!
		The names for addressing the laser scanner array
		*/
		static const unsigned char DISTANCE = 0;
		static const unsigned char FLAG = 1;
};

#endif
