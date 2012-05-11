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

#include "obstacleCheckThread.h"

ObstacleCheckThread::ObstacleCheckThread(SensorThread *s, LaserThread *l)
{
	// get the name of this class (this is for debugging messages)
	className = this->staticMetaObject.className();

	initCompleted = false;
	stopped = false;

	// copy the pointer from the original SensorThread object
	sensThread = s;
	laserThread = l;
}


ObstacleCheckThread::~ObstacleCheckThread()
{
}


void ObstacleCheckThread::init()
{
	minObstacleDistance = 0;
	minObstacleDistanceLaserFront = 0;
	minObstacleDistanceLaserRear = 0;
	sensorValue = NONE;

	actualFreeAreaStart = -1;
	actualFreeAreaEnd = -1;

	largestFreeAreaStart = -1;
	largestFreeAreaEnd = -1;

	centerOfFreeWayFront = -1;
	centerOfFreeWayRear = -1;

	laserscannerFrontIgnoreArea1Start = 0.0;
	laserscannerFrontIgnoreArea1End = 0.0;
	laserscannerFrontIgnoreArea2Start = 0.0;
	laserscannerFrontIgnoreArea2End = 0.0;

	laserscannerRearIgnoreArea1Start = 0.0;
	laserscannerRearIgnoreArea1End = 0.0;
	laserscannerRearIgnoreArea2Start = 0.0;
	laserscannerRearIgnoreArea2End = 0.0;

	robotSlot = 1;      // 1 degree
	robotSlotWidth = 1; // 1 cm
	straightForwardDeviation = 0;

	laserResolutionFront = 0.0;
	laserResolutionRear = 0.0;
	laserAngleFront = 0;
	laserAngleRear = 0;


	// Note: init of the laser scanner flags is now done in the laserThread


	// get resolution and angle from the laser thread for all laser scanners
	if (laserThread->isRunning())
	{
		laserResolutionFront = laserThread->getResolution(LASER1);
		laserAngleFront = laserThread->getAngle(LASER1);

		laserResolutionRear = laserThread->getResolution(LASER2);
		laserAngleRear = laserThread->getAngle(LASER2);
	}
	else
	{
		emit message("ERROR getting laser angle and resolution from laserThread for front laser. LaserThread is not running! ObstacleCheckThread::run()");
		laserResolutionFront = 0.0;
		laserAngleFront = 0;

		emit message("ERROR getting laser angle and resolution from laserThread for rear laser. LaserThread is not running! ObstacleCheckThread::run()");
		laserResolutionRear = 0.0;
		laserAngleRear = 0;
	}


	// init completed
	initCompleted = true;
}


void ObstacleCheckThread::stop()
{
	stopped = true;
}


void ObstacleCheckThread::run()
{
	float middleOfLaser = 0;


	if (initCompleted == false)
	{
		emit message(QString("<font color=\"#FF0000\">%1::init not called! Stopping thread!</font>").arg(className));
		stopped = false;

		return;
	}


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

		// reset "drive to angle" to "middle of the number of laser lines" -> FORWARD
		centerOfFreeWayFront = ( (laserAngleFront / laserResolutionFront) / 2);


/*			infrared Sensors temporarily removed from robot!!
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
		emit obstacleDetected(sensorValue, QDateTime::currentDateTime());
		//=======================================================================================================
*/


		//---------------------------------------------------------
		// LASER SCANNER 1 DATA ANALYSIS - STEP I
		//---------------------------------------------------------
		// If obstacle in front of the laser scanner,
		// set a flag at the corresponding angles
		//---------------------------------------------------------
		for (int angleIndex=0; angleIndex < ( laserAngleFront / laserResolutionFront ); angleIndex++)
		{
			// first set if we ignore this area and than mark this as such
			if (
					((angleIndex >= (laserscannerFrontIgnoreArea1Start*(1/laserResolutionFront))) && (angleIndex <= (laserscannerFrontIgnoreArea1End*(1/laserResolutionFront)))) ||
					((angleIndex >= (laserscannerFrontIgnoreArea2Start*(1/laserResolutionFront))) && (angleIndex <= (laserscannerFrontIgnoreArea2End*(1/laserResolutionFront)) ))
			   )
			{
				//------------------------------
				// first set the "ignore flag"
				//------------------------------
				laserThread->setFlag(LASER1, angleIndex, IGNORETHIS);
			}
			else
			{
				// if obstacle detected
				if ( ((int) (laserThread->getValue(LASER1, angleIndex) * 100)) < minObstacleDistanceLaserFront)
				{
					//-----------------------------
					// set the "obstacle flag"
					//-----------------------------
					laserThread->setFlag(LASER1, angleIndex, OBSTACLE);
				}
				else
				{
					//---------------------------------------------------------------------------
					// set the "no obstacle flag" -> free way at the current angle
					//---------------------------------------------------------------------------
					laserThread->setFlag(LASER1, angleIndex, FREEWAY);
				}
			}
		}


		//------------------------------------------------------------------
		// LASER SCANNER 1 DATA ANALYSIS - STEP II
		//------------------------------------------------------------------
		// Find all free areas and store start and end angle.
		// Since we analyse the previos, the current and the next angle
		// and we have to take caere of the first, "middle" and last angle
		// we have to check all 24x3 posibilities and react on these.
		// In the end I ended up in five different "actions".
		//------------------------------------------------------------------
		largestFreeAreaStart = -1;
		largestFreeAreaEnd   = -1;
		centerOfFreeWayFront = -1;


		//--------------------------------------------------------------------------------
		// Check all angles
		//--------------------------------------------------------------------------------
		int firstAngle = 0;
		int lastAngle  = (laserAngleFront / laserResolutionFront) - 1;

		for (int angleIndex=0; angleIndex < (laserAngleFront / laserResolutionFront); angleIndex++)
		{
			// check only lines with which are *not* in an area to be ognored
			if (laserThread->getFlag(LASER1, angleIndex) != IGNORETHIS )
			{
				// green
				if(
						((angleIndex == firstAngle) &&
						(laserThread->getFlag(LASER1, angleIndex) == FREEWAY) &&
						(laserThread->getFlag(LASER1, angleIndex+1) == FREEWAY))
						||
						((angleIndex != firstAngle) && (angleIndex != lastAngle) && // any in between
						(laserThread->getFlag(LASER1, angleIndex-1) == OBSTACLE) &&
						(laserThread->getFlag(LASER1, angleIndex) == FREEWAY) &&
						(laserThread->getFlag(LASER1, angleIndex+1) == FREEWAY))
				  )
				{
					// store this angle index as free
					freeStartAreas.append(angleIndex);
				}

				// blue
				if(
						((angleIndex == lastAngle) &&
						(laserThread->getFlag(LASER1, angleIndex-1) == FREEWAY) &&
						(laserThread->getFlag(LASER1, angleIndex) == FREEWAY))
						||
						((angleIndex != firstAngle) && (angleIndex != lastAngle) && // any in between
						(laserThread->getFlag(LASER1, angleIndex-1) == FREEWAY) &&
						(laserThread->getFlag(LASER1, angleIndex) == FREEWAY) &&
						(laserThread->getFlag(LASER1, angleIndex+1) == OBSTACLE))
				  )
				{
					// store this angle index as free
					freeEndAreas.append(angleIndex);
				}

				// white
				if(
						((angleIndex == firstAngle) &&
						(laserThread->getFlag(LASER1, angleIndex) == OBSTACLE) &&
						(laserThread->getFlag(LASER1, angleIndex+1) == FREEWAY))
						||
						((angleIndex != firstAngle) && (angleIndex != lastAngle) && // any in between
						(laserThread->getFlag(LASER1, angleIndex-1) == OBSTACLE) &&
						(laserThread->getFlag(LASER1, angleIndex) == OBSTACLE) &&
						(laserThread->getFlag(LASER1, angleIndex+1) == FREEWAY))
				  )
				{
					// store next angle index as free
					freeStartAreas.append(angleIndex + 1);
				}

				// yellow
				if(
						((angleIndex == lastAngle) &&
						(laserThread->getFlag(LASER1, angleIndex-1) == OBSTACLE) &&
						(laserThread->getFlag(LASER1, angleIndex) == FREEWAY))
						||
						((angleIndex == firstAngle) &&
						(laserThread->getFlag(LASER1, angleIndex) == FREEWAY) &&
						(laserThread->getFlag(LASER1, angleIndex+1) == OBSTACLE))
						||
						((angleIndex != firstAngle) && (angleIndex != lastAngle) && // any in between
						(laserThread->getFlag(LASER1, angleIndex-1) == OBSTACLE) &&
						(laserThread->getFlag(LASER1, angleIndex) == FREEWAY) &&
						(laserThread->getFlag(LASER1, angleIndex+1) == OBSTACLE))
				  )
				{
					// store this angle index as free
					freeStartAreas.append(angleIndex);
					freeEndAreas.append(angleIndex);
				}
			} // flag != IGNORETHIS
		}



		//------------------------------------------------------------
		// Then tag the *largest* free area, if multiple were found
		// (to show it in the GUI and to know, where to drive)
		// The old flags were set to new values above in this code!
		//------------------------------------------------------------
		if (largestFreeAreaEnd != -1)
		{
			for (int angleIndex=largestFreeAreaStart; angleIndex<=largestFreeAreaEnd; angleIndex++)
			{
				laserThread->setFlag(LASER1, angleIndex, LARGESTFREEWAY);
			}
		}
		else
		{
			// only *one* large free area found (actual area), so tag this one
			if (actualFreeAreaEnd != -1)
			{
				for (int angle=actualFreeAreaStart; angle<=actualFreeAreaEnd; angle++)
				{
					laserThread->setFlag(LASER1, angle, LARGESTFREEWAY);
				}
			}
		}


		//----------------------------------------------------------------------------
		// LASER SCANNER 1 DATA ANALYSIS - STEP III
		//----------------------------------------------------------------------------
		// Find the center of the largest free area for the robot
		//----------------------------------------------------------------------------

		// free area found  :-)
			// set flag to "light green"
//			laserThread->setFlag(LASER1, centerOfFreeWayFront, CENTEROFLARGESTFREEWAY);


		//----------------------------------------------------------------------------
		// LASER SCANNER 1 DATA ANALYSIS - STEP III a
		//--------------------------------------------------------------------------------------
		// Calculate the width of the estimated drive-tru direction/area with the 'Kosinussatz'
		// (a² = b² + c² - 2bc * cos alpha)  where 'a' is the width
		//--------------------------------------------------------------------------------------
		if ( (largestFreeAreaStart > 0)  &&  ( largestFreeAreaEnd < (laserAngleFront / laserResolutionFront) ))
		{
			// b and c are the sides of the triangle
			b = laserThread->getValue(LASER1, largestFreeAreaStart) * 100; // converted in cm, here!
			c = laserThread->getValue(LASER1, largestFreeAreaEnd) * 100; // converted in cm, here!
			alpha = (largestFreeAreaEnd - largestFreeAreaStart);

			// to get the 'true drive-tru width':
			// make the triangle to be one with same-length-sides of b and c
			if (c < b)
			{
				b = c;
			}
			else
			{
				if (b < c)
				{
					c = b;
				}
			}

			// calculate
			// WARNING: "cos" functions use radians!! so we convert the degrees to radions here!
			width = sqrt( pow(b, 2.0) + pow(c, 2.0) - 2.0*b*c * cos(alpha*M_PI / (double) laserAngleFront) );
		}
		else
		{
			width = -1;
		}


		//----------------------------------------------------------------------------
		// LASER SCANNER 1 DATA ANALYSIS - STEP IV
		//----------------------------------------------------------------------------
		// Emit the result for the GUI
		//----------------------------------------------------------------------------

		// since this signal is only used to display the values in the gui,
		// the values are multiplied by the resolution to have the correct value in degrees!
		emit newDrivingAngleSet((largestFreeAreaStart * laserResolutionFront), (largestFreeAreaEnd * laserResolutionFront), (centerOfFreeWayFront * laserResolutionFront), width);

		// get the middle of the laser (when we have 180 deg, the middle is as 90 deg)
		middleOfLaser = (laserAngleFront / laserResolutionFront) / 2;

		if (centerOfFreeWayFront == -1)
		{
			// obstacles EVERYWHERE IN FRONT
			emit obstacleDetected(OBSTACLESEVERYWHEREINFRONT, QDateTime::currentDateTime());
		}
		else
		{
			// value within the tolerance range (deviation to 90 deg.)?
			if (
				// FIXME: why -1 ?!? But works so far!
				( (centerOfFreeWayFront < middleOfLaser) && (centerOfFreeWayFront >= (middleOfLaser - (straightForwardDeviation / laserResolutionFront) - 1)) ) ||
				( (centerOfFreeWayFront > middleOfLaser) && (centerOfFreeWayFront <= (middleOfLaser + (straightForwardDeviation / laserResolutionFront) - 1)) ) ||
				(  centerOfFreeWayFront == middleOfLaser )
			   )
			{
				// NO obstacle
				emit obstacleDetected(NONE, QDateTime::currentDateTime());
			}
			else
			{
				if ( (centerOfFreeWayFront < middleOfLaser) && (centerOfFreeWayFront > -1) )
				{
					// free way left  ->  obstacle RIGHT
					emit obstacleDetected(OBSTACLEFRONTRIGHT, QDateTime::currentDateTime());
				}
				else
				{
					if (centerOfFreeWayFront > middleOfLaser)
					{
						// free way right  ->  obstacle LEFT
						emit obstacleDetected(OBSTACLEFRONTLEFT, QDateTime::currentDateTime());
					}
				}
			}
		}



		//================================= START LASER 2 =========================================
		//================================= START LASER 2 =========================================
		//================================= START LASER 2 =========================================



		//================================= END LASER 2 =========================================
		//================================= END LASER 2 =========================================
		//================================= END LASER 2 =========================================

	} // while

	stopped = false;
}


void ObstacleCheckThread::setMinObstacleDistance(int distance)
{
	minObstacleDistance = distance;
}


void ObstacleCheckThread::setMinObstacleDistanceLaserFront(int distance)
{
	minObstacleDistanceLaserFront = distance;
}


void ObstacleCheckThread::setMinObstacleDistanceLaserRear(int distance)
{
	minObstacleDistanceLaserRear = distance;
}


void ObstacleCheckThread::setIgnoreArea(short int laser, int area, int start, int end)
{
	if (laser==LASER1)
	{
		if (area==AREA1)
		{
			laserscannerFrontIgnoreArea1Start = start;
			laserscannerFrontIgnoreArea1End = end;
			return;
		}

		if (area==AREA2)
		{
			laserscannerFrontIgnoreArea2Start = start;
			laserscannerFrontIgnoreArea2End = end;
			return;
		}
	}


	if (laser==LASER2)
	{
		if (area==AREA1)
		{
			laserscannerRearIgnoreArea1Start = start;
			laserscannerRearIgnoreArea1End = end;
			return;
		}

		if (area==AREA2)
		{
			laserscannerRearIgnoreArea2Start = start;
			laserscannerRearIgnoreArea2End = end;
			return;
		}
	}

	emit message(QString("ERROR in %1::setIgnoreArea: laser number %2 or area %3 not implemented!").arg(className).arg(laser).arg(area));
}


void ObstacleCheckThread::setRobotSlot(int angle)
{
	robotSlot = angle;
}


void ObstacleCheckThread::setRobotSlotWidth(int width)
{
	robotSlotWidth = width;
}


void ObstacleCheckThread::setStraightForwardDeviation(int deviation)
{
	straightForwardDeviation = deviation;
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


void ObstacleCheckThread::systemerrorcatcher(int errorlevel)
{
	if (errorlevel == -1)
	{
// wtf?		// obstacles EVERYWHERE IN FRONT
// wtf?		emit obstacleDetected(OBSTACLESEVERYWHEREINFRONT, QDateTime::currentDateTime());

		emit message("<font color=\"#FF0000\">Error received from other module. Stopping obstacle check thread.</font>");

		// stop this thread
		stop();
	}
}
