#include "obstacleCheckThread.h"

ObstacleCheckThread::ObstacleCheckThread(SensorThread *s, LaserThread *l)
{
	stopped = false;
	
	// copy the pointer from the original SensorThread object
	sensThread = s;
	laserThread = l;
	
	minObstacleDistance = 0;
	minObstacleDistanceLaserScanner = 0;
	sensorValue = NONE;
	
	actualFreeAreaStart = -1;
	actualFreeAreaEnd = -1;

	largestFreeAreaStart = -1;
	largestFreeAreaEnd = -1;

	centerOfFreeWay = 90;
	
	robotSlot = 1;
}


ObstacleCheckThread::~ObstacleCheckThread()
{
}


void ObstacleCheckThread::stop()
{
	stopped = true;
}


void ObstacleCheckThread::run()
{
	//  start "threading"...
	while (!stopped)
	{
		//-------------------------------------------------
		// Let the thread sleep some time,
		// for having a bit more time fo the other threads!
		//-------------------------------------------------
		msleep(THREADSLEEPTIME);
		
		// This value contains the sum of all SENSORx values!
		sensorValue = NONE;
		
		// reset "drive to angle" to 90° -> FORWARD
		centerOfFreeWay = 90;
	
		
		//-------------------------------------------------------------
		// if obstacle in front of sensor 1
		// (measured distance is smaller than the minObStacleDistance)
		//-------------------------------------------------------------
		if (sensThread->getDistance(SENSOR1) <= minObstacleDistance)
		{
			// Add the sensor value to the return value
			sensorValue += SENSOR1;
		}
	
	
		//-----------------------------------
		// if obstacle in front of sensor 2
		//-----------------------------------
		if (sensThread->getDistance(SENSOR2) <= minObstacleDistance)
		{
			// Add the sensor value to the return value
			sensorValue += SENSOR2;
		}
	
	
		//-----------------------------------
		// if obstacle in front of sensor 3
		//-----------------------------------
		if (sensThread->getDistance(SENSOR3) <= minObstacleDistance)
		{
			// Add the sensor value to the return value
			sensorValue += SENSOR3;
		}
		
		
		//-----------------------------------
		// if obstacle in front of sensor 4
		//-----------------------------------
		if (sensThread->getDistance(SENSOR4) <= minObstacleDistance)
		{
			// Add the sensor value to the return value
			sensorValue += SENSOR4;
		}
		
		
		//-----------------------------------
		// if obstacle in front of sensor 5
		//-----------------------------------
		if (sensThread->getDistance(SENSOR5) <= minObstacleDistance)
		{
			// Add the sensor value to the return value
			sensorValue += SENSOR5;
		}
		
		
		//-----------------------------------
		// if obstacle in front of sensor 6
		//-----------------------------------
		if (sensThread->getDistance(SENSOR6) <= minObstacleDistance)
		{
			// Add the sensor value to the return value
			sensorValue += SENSOR6;
		}
		
		
		//-----------------------------------
		// if obstacle in front of sensor 7
		//-----------------------------------
		if (sensThread->getDistance(SENSOR7) <= minObstacleDistance)
		{
			// Add the sensor value to the return value
			sensorValue += SENSOR7;
		}
		
		
		//-----------------------------------
		// if obstacle in front of sensor 8
		//-----------------------------------
		if (sensThread->getDistance(SENSOR8) <= minObstacleDistance)
		{
			// Add the sensor value to the return value
			sensorValue += SENSOR8;
		}
	
		//=======================================================================================================
	
		//---------------------------------------------------------
		// if obstacle in front of sensor 16 (ultra sonic sensor!)
		//---------------------------------------------------------
		if (sensThread->getUsSensorValue(SENSOR16) <= minObstacleDistance)
		{
			// Add the sensor value to the return value
			sensorValue += SENSOR16;
		}
	
	
		//=======================================================================================================
		//
		//   First  e m i t  of the sensor data
		//
		//qDebug("obstacleCheckThread:  emit sensorValue: %d", sensorValue);
//		emit obstacleDetected(sensorValue);
		//=======================================================================================================
	
		
		//---------------------------------------------------------
		// LASER SCANNER DATA ANALYSIS - STEP I
		//---------------------------------------------------------
		// If obstacle in front of the laser scanner,
		// set a flag at the corresponding angles
		//---------------------------------------------------------
		// TODO: change this 180 to a const or something else
		for (int angle=0; angle<180; angle++)
		{
			// if obstacle detected
			if ((int) (laserThread->getLaserScannerValue(angle)*100) <= minObstacleDistanceLaserScanner)
			{
				//-----------------------------
				// set the "obstacle flag"
				//-----------------------------
				laserThread->setLaserScannerFlag(angle, 1);
				//qDebug("laser line %d black", laserThread->getLaserScannerValue(angle));
			}
			else
			{
				//-------------------------------------------------------------
				// delete the "obstacle flag" -> free way at the actual angle
				//-------------------------------------------------------------
				laserThread->setLaserScannerFlag(angle, 0);
			}
		}
		
		
		//---------------------------------------------------------
		// LASER SCANNER DATA ANALYSIS - STEP II
		//---------------------------------------------------------
		// Find the widest free range of angles within the data.
		// Set a flag at the corresponding angles.
		//---------------------------------------------------------
		actualFreeAreaStart = -1;
		actualFreeAreaEnd   = -1;
		
		largestFreeAreaStart = -1;
		largestFreeAreaEnd   = -1;
		
		
		//--------------------------------------------------------------------------------
		// First find the largest free area (the one, with the widest "free sight" angle)
		//--------------------------------------------------------------------------------
		for (int angle=0; angle<180; angle++)
		{
			// check only lines with no obstacles!
			if (laserThread->getLaserScannerFlag(angle) == FREEWAY)
			{
				// Actual angle has "free" sight (no obstacles)!
				// If next angle is free AND the angle before this actual is NOT free, set this one as "start"
				//
				// Additionaly angles at the size of 1° will be ignored with this logic!
				//
				if (
					((laserThread->getLaserScannerFlag(angle+1) == FREEWAY) && (laserThread->getLaserScannerFlag(angle-1) == OBSTACLE)) ||
					((laserThread->getLaserScannerFlag(angle+1) == FREEWAY) && (angle == 0))
					)
				{
					// store current free area beginning
					actualFreeAreaStart = angle;
				}
				
				// Actual angle has "free" sight (no obstacles)!
				// If next angle is NOT free, set this one as "end"
				// and angle slot is wide enough for the robot
				if ( (laserThread->getLaserScannerFlag(angle+1) == OBSTACLE) && ((angle - actualFreeAreaStart) >=robotSlot) )
				{
					// store current free area end
					actualFreeAreaEnd = angle;
					
					// if current free area is larger than the last, store it
					// AND the robot fits through this angle!
					if ( (largestFreeAreaEnd - largestFreeAreaStart) < (actualFreeAreaEnd - actualFreeAreaStart) )
					{
						// store the current free area as largest
						largestFreeAreaStart = actualFreeAreaStart;
						largestFreeAreaEnd = actualFreeAreaEnd;
					}
				}
			}
		}
		
		
		//------------------------------------------------------------
		// Then tag the *largest* free area, if multiple were found
		// (to show it in the GUI and to know, where to drive)
		//------------------------------------------------------------
		if (largestFreeAreaEnd != -1)
		{
			for (int angle=largestFreeAreaStart; angle<=largestFreeAreaEnd; angle++)
			{
				laserThread->setLaserScannerFlag(angle, LARGESTFREEWAY);
			}
		}
		else
		{
			// only *one* large free area found (actual area), so tag this one
			if (actualFreeAreaEnd != -1)
			{
				for (int angle=actualFreeAreaStart; angle<=actualFreeAreaEnd; angle++)
				{
					laserThread->setLaserScannerFlag(angle, LARGESTFREEWAY);
				}
			}
		}
	
		
		//----------------------------------------------------------------------------
		// LASER SCANNER DATA ANALYSIS - STEP III
		//----------------------------------------------------------------------------
		// Find the center of the largest free area for the robot
		//----------------------------------------------------------------------------
		centerOfFreeWay = largestFreeAreaEnd - qRound( (largestFreeAreaEnd - largestFreeAreaStart) / 2);
		
		// FIXME: what, if = -1 ?!?
		
		// set flag to "light green"
		laserThread->setLaserScannerFlag(centerOfFreeWay, CENTEROFLARGESTFREEWAY);


		//----------------------------------------------------------------------------
		// LASER SCANNER DATA ANALYSIS - STEP IV
		//----------------------------------------------------------------------------
		// Emit the result
		//----------------------------------------------------------------------------
		// FIXME: correct some lines above!!
		if (centerOfFreeWay == -1)
			centerOfFreeWay == 90;

		emit newDrivingAngleSet(largestFreeAreaStart, largestFreeAreaEnd, centerOfFreeWay);

		if (centerOfFreeWay < 90)
		{
			// obstacle LEFT, drive RIGHT
			emit obstacleDetected(OBSTACLEFRONTLEFT);
		}
		
		if (centerOfFreeWay > 90)
		{
			// obstacle RIGHT, drive LEFT
			emit obstacleDetected(OBSTACLEFRONTRIGHT);
		}
		
		if (centerOfFreeWay == 90)
		{
			// obstacle FRONTMIDDLE
			emit obstacleDetected(NONE);
		}
	}
	
	stopped = false;
}


void ObstacleCheckThread::setMinObstacleDistance(int value)
{
	minObstacleDistance = value;
}


void ObstacleCheckThread::setMinObstacleDistanceLaser(int value)
{
	minObstacleDistanceLaserScanner = value;
}


void ObstacleCheckThread::setRobotSlot(int angle)
{
	robotSlot = angle;
}


void ObstacleCheckThread::setSimulationMode(bool status)
{
	simulationMode = status;
	/*
	
	if (simulationMode == true)
	{
		// some nice simulation values
		
	}
	else
	{
		// initialisation
		
		
		// for refreshing the gui
		obstacleDetected(NONE);
	}
	*/
}
