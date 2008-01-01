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
		float getLaserScannerValue(int angle);
		float getLaserScannerFlag(int angle);
		void setLaserScannerFlag(int angle, int flag);
		int getNumReadings();
		
		/*!
		This method sets the "nolaserScannerFlag".
		@param flag is true, when a Laser Scanner <b>is</b> connected and the CARMEN modules <b>are</b> started.
		*/
		void setLaserScannerFlag(bool flag);
		
		/*!
		Returns the state of a connected laser scanner. Also sets the serial port.
		@param serialPortLaserscannerFront is the port of the front Laser scanner
		@param serialPortLaserscannerRear is the port of the rear Laser scanner
		@return true, if connected
		*/
		bool isConnected(QString serialPortLaserscannerFront, QString serialPortLaserscannerRear);
	
	
	public slots:
		/*!
		This slot enables or disables the simulation mode.
		*/
		void setSimulationMode(bool status);


	signals:
		/*! Emits a pointer to the array with the 180 laser values (distances) */
		void laserDataComplete(float *laserScannerValues, int *laserScannerFlags);


	private:
		//mutable QMutex mutex; // make this class thread-safe
		volatile bool stopped;
		bool laserScannerIsConnected;
		int numReadings;
		bool simulationMode;
		
		// Every thread sleeps some time, for having a bit more time fo the other threads!
		// Time in milliseconds
		static const unsigned long THREADSLEEPTIME = 150; // Default: 150 ms
		
		/*!
		Defines the size of the laserScannerValue[] array
		This es equal to the number of degrees.
		*/
		static const unsigned char LASERSCANNERARRAYSIZE = 180;
		
		/*!
		The first column of this array is the measured distance to an object (the length of the real laser line in meter (m)).
		The second column is an flag if an obstacle is detected (the distance is to small from the robot to an object)
		*/
		QList <float> laserScannerValues;  /** The measured distances from the laser scanner. */
		QList <int> laserScannerFlags;  /** Some flags for each laser line (like "free way", "obstacle" etc. */
		
		/*!
		The names for addressing the laser scanner array
		*/
		static const unsigned char DISTANCE = 0;
		static const unsigned char FLAG = 1;
		
		/*!
		For the integrated CARMEN laser module
		*/
		static const short int LASER1 = 1;
		static const short int LASER2 = 2;
		static const short int LASER3 = 4;
		static const short int LASER4 = 8;
		static const short int LASER5 = 16;
};

#endif
