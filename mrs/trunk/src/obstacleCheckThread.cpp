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

	centerOfFreeWay = -1;
	
	robotSlot = 1;
	robotSlotTolerance = 0;
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
			if ( ((int) (laserThread->getLaserScannerValue(angle)*100)) < minObstacleDistanceLaserScanner)
			{
				//-----------------------------
				// set the "obstacle flag"
				//-----------------------------
				laserThread->setLaserScannerFlag(angle, OBSTACLE);
			}
			else
			{
				//-------------------------------------------------------------
				// delete the "obstacle flag" -> free way at the actual angle
				//-------------------------------------------------------------
				laserThread->setLaserScannerFlag(angle, FREEWAY);
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

		centerOfFreeWay = -1;
	
		//--------------------------------------------------------------------------------
		// First find the largest free area (the one, with the widest "free sight" angle)
		//--------------------------------------------------------------------------------
		for (int angle=0; angle<180; angle++)
		{
			// check only lines with no obstacles!
			if (laserThread->getLaserScannerFlag(angle) == FREEWAY)
			{
				// If this is the FIRST angle AND the next angle is "free"
				// OR
				// If current angle has "free" sight (no obstacles) AND next angle is free AND the angle before current is NOT free,
				// THEN set the current angle to "free area start"
				//
				// Automaticaly angles at the size of 1° will be NOT be ignored (not set to "free area start"!)
				//
				if (
					((laserThread->getLaserScannerFlag(angle+1) == FREEWAY) && (angle == 0)) ||
					((laserThread->getLaserScannerFlag(angle+1) == FREEWAY) && (laserThread->getLaserScannerFlag(angle-1) == OBSTACLE))
				   )
				{
					// store current free area beginning
					actualFreeAreaStart = angle;
					//qDebug("angle=%d / actualFreeAreaStart=%d", angle, actualFreeAreaStart);
				}
				
				// If current angle has "free" sight (no obstacles)
				// AND next angle is NOT free, set this one as "end"
				// AND the angle before current is free
				// AND angle robot slot is wide enough
				//
				// THEN set the current angle to "free area end"
				//
				// Automaticaly angles at the size of 1° will be NOT be ignored (not set to "free area end"!)
				//
				if ( (laserThread->getLaserScannerFlag(angle+1) == OBSTACLE) && (laserThread->getLaserScannerFlag(angle-1) == FREEWAY) && ((angle - actualFreeAreaStart) >= robotSlot - 1) )
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
		// The old flags were set to new values above in this code!
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
		
		// free area found  :-)
		if (largestFreeAreaEnd != -1)
		{
			// find the center of the largest free area
			centerOfFreeWay = largestFreeAreaEnd - qRound( (largestFreeAreaEnd - largestFreeAreaStart) / 2);
			
			// set flag to "light green"
			laserThread->setLaserScannerFlag(centerOfFreeWay, CENTEROFLARGESTFREEWAY);
		}
		else
		{
		}


		//----------------------------------------------------------------------------
		// LASER SCANNER DATA ANALYSIS - STEP IV
		//----------------------------------------------------------------------------
		// Emit the result
		//----------------------------------------------------------------------------
		emit newDrivingAngleSet(largestFreeAreaStart, largestFreeAreaEnd, centerOfFreeWay);

		if (centerOfFreeWay == -1)
		{
			// obstacles EVERYWHERE IN FRONT
			emit obstacleDetected(OBSTACLESEVERYWHEREINFRONT);
		}
		else
		{
			// value within the tolerance range?
			if (
				( (centerOfFreeWay < 90) && (centerOfFreeWay >= (90 - robotSlotTolerance - 1)) ) ||
				( (centerOfFreeWay > 90) && (centerOfFreeWay <= (90 + robotSlotTolerance - 1)) )
			   )
			{
				// NO obstacle
				emit obstacleDetected(NONE);
			}
			else
			{
				if ( (centerOfFreeWay < 90) && (centerOfFreeWay > -1) )
				{
					// obstacle LEFT
					emit obstacleDetected(OBSTACLEFRONTLEFT);
				}
				else
				{
					if (centerOfFreeWay > 90)
					{
						// obstacle RIGHT
						emit obstacleDetected(OBSTACLEFRONTRIGHT);
					}
				}
			}
		}
	}
	
	stopped = false;
}


void ObstacleCheckThread::setMinObstacleDistance(int distance)
{
	minObstacleDistance = distance;
}


void ObstacleCheckThread::setMinObstacleDistanceLaser(int distance)
{
	minObstacleDistanceLaserScanner = distance;
}


void ObstacleCheckThread::setRobotSlot(int angle)
{
	robotSlot = angle;
}


void ObstacleCheckThread::setRobotSlotTolerance(int tolerance)
{
	robotSlotTolerance = tolerance;
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
