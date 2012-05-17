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

	// basic init
	// has to be here, not in init() since we would reset these values when calling init again!
	minObstacleDistance = 0;
	minObstacleDistanceLaserFront = 0;
	minObstacleDistanceLaserRear = 0;
	sensorValue = NONE;

	actualFreeAreaStart = -1;
	actualFreeAreaEnd = -1;

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
}


ObstacleCheckThread::~ObstacleCheckThread()
{
}


void ObstacleCheckThread::init()
{
	if (initCompleted == false)
	{
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


		// Delete the previous found areas for the next analysis.
		freeStartAreas.clear();
		freeEndAreas.clear();

		// init completed
		initCompleted = true;
	}
}


void ObstacleCheckThread::stop()
{
	stopped = true;
}


void ObstacleCheckThread::run()
{
	int first = 0;					/// holds the first angle index
	int last = 0;					/// holds the last last angle index
	double currentWidth = 0.0;		///
	double largestWidth = 0.0;
	float farestDistance = 0.0;		///
	float currentDistance = 0.0;	///
	float b = 0.0;
	float c = 0.0;
	int largestFreeAreaStart = 0;
	int largestFreeAreaEnd = 0;
	float centerOfFreeWayFront = 0;
	float middleOfLaser = 0.0;		/// this is for detecting the driving direction ("drive left" means "drive left from the middle of the laser")



	if (initCompleted == false)
	{
		emit message(QString("<font color=\"#FF0000\">%1::init not called! Stopping thread!</font>").arg(className));
		stopped = false;

		return;
	}


	//  start "threading"...
	while (!stopped)
	{
		// This value contains the sum of all SENSORx values!
		sensorValue = NONE;

		// reset "drive to angle" to "middle of the number of laser lines" -> FORWARD
		centerOfFreeWayFront = ( (laserAngleFront / laserResolutionFront) / 2);  /// @todo  correct to reset the centerOfFreeway here?!?


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
		// set the indexes for the upcoming loops
		//---------------------------------------------------------
		first = 0;
		last  = (laserAngleFront / laserResolutionFront) -1;
		// e.g. 180 degrees with resolution   1 degree  result in an index from 0 to 179
		// e.g. 270 degrees with resolution 0,5 degrees result in an index from 0 to 539

		//---------------------------------------------------------
		// LASER SCANNER 1 DATA ANALYSIS - STEP I
		//---------------------------------------------------------
		// If obstacle in front of the laser scanner,
		// set a flag at the corresponding angles
		//---------------------------------------------------------
		for (int angleIndex=first; angleIndex <= last; angleIndex++)
		{
			// first set if we ignore this area and than mark this as such
			if (
					((angleIndex >= (laserscannerFrontIgnoreArea1Start*(1/laserResolutionFront))) && (angleIndex <= (laserscannerFrontIgnoreArea1End*(1/laserResolutionFront)))) ||
					((angleIndex >= (laserscannerFrontIgnoreArea2Start*(1/laserResolutionFront))) && (angleIndex <= (laserscannerFrontIgnoreArea2End*(1/laserResolutionFront))))
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
		// In the end I ended up in three different "actions".
		//------------------------------------------------------------------

		//--------------------------------------------------------------------------------
		// Check all angles
		//--------------------------------------------------------------------------------

		//--------------------------------------------------------------------------------
		// Set the 'new' first and 'last' angles
		// due to the 'ignore areas' at the beging and at the and of all lines
		//
		// Be aware of setting the ignore areas than only at start and end lines!
		//--------------------------------------------------------------------------------
		first = laserscannerFrontIgnoreArea1End + 1;
		last  = laserscannerFrontIgnoreArea2Start - 1;

		for (int angleIndex = first; angleIndex <= last; angleIndex++)
		{
			//qDebug("%d -----------", angleIndex);

			// check only lines with which are *not* in an area to be ognored
			if (laserThread->getFlag(LASER1, angleIndex) != IGNORETHIS )
			{
				// green
				if(
						((angleIndex == first) &&
						(laserThread->getFlag(LASER1, angleIndex) == FREEWAY) &&
						(laserThread->getFlag(LASER1, angleIndex+1) == FREEWAY))
						||
						((angleIndex != first) && (angleIndex != last) && // any in between
						(laserThread->getFlag(LASER1, angleIndex-1) == OBSTACLE) &&
						(laserThread->getFlag(LASER1, angleIndex) == FREEWAY) &&
						(laserThread->getFlag(LASER1, angleIndex+1) == FREEWAY))
				  )
				{
					// store current angle index as free
					freeStartAreas.append(angleIndex);
					//qDebug("%d = green", angleIndex);
				}
				else
				// blue
				if(
						((angleIndex == last) &&
						(laserThread->getFlag(LASER1, angleIndex-1) == FREEWAY) &&
						(laserThread->getFlag(LASER1, angleIndex) == FREEWAY))
						||
						((angleIndex != first) && (angleIndex != last) && // any in between
						(laserThread->getFlag(LASER1, angleIndex-1) == FREEWAY) &&
						(laserThread->getFlag(LASER1, angleIndex) == FREEWAY) &&
						(laserThread->getFlag(LASER1, angleIndex+1) == OBSTACLE))
				  )
				{
					// store current angle index as free
					freeEndAreas.append(angleIndex);
					//qDebug("%d = blue", angleIndex);
				}
				else
				// yellow
				if(
						((angleIndex == last) &&
						(laserThread->getFlag(LASER1, angleIndex-1) == OBSTACLE) &&
						(laserThread->getFlag(LASER1, angleIndex) == FREEWAY))
						||
						((angleIndex == first) &&
						(laserThread->getFlag(LASER1, angleIndex) == FREEWAY) &&
						(laserThread->getFlag(LASER1, angleIndex+1) == OBSTACLE))
						||
						((angleIndex != first) && (angleIndex != last) && // any in between
						(laserThread->getFlag(LASER1, angleIndex-1) == OBSTACLE) &&
						(laserThread->getFlag(LASER1, angleIndex) == FREEWAY) &&
						(laserThread->getFlag(LASER1, angleIndex+1) == OBSTACLE))
				  )
				{
					// store current angle index as free
					freeStartAreas.append(angleIndex);
					freeEndAreas.append(angleIndex);
					//qDebug("%d = yellow", angleIndex);
				}
			} // flag != IGNORETHIS
		}


		//------------------------------------------------------------------
		// LASER SCANNER 1 DATA ANALYSIS - STEP III
		//------------------------------------------------------------------
		// do a logic check of the found free areas:
		// do have the same amount of 'start' and 'end' angles?
		//------------------------------------------------------------------
		if (freeStartAreas.count() != freeEndAreas.count())
		{
			emit message(QString("ERROR in logical check of free laser areas in %1!").arg(className));
			emit message("Reaction to be implemented!!!");
			// qDebug() << "ERROR in logical check of free laser areas in:" << className;
		}


		//  t e s t
		//  t e s t
		//  t e s t
//		qDebug() << "freeStartAreas:" << freeStartAreas;
//		qDebug() << "freeEndAreas:"   << freeEndAreas;


		//----------------------------------------------------------------------------
		// LASER SCANNER 1 DATA ANALYSIS - STEP IV
		//----------------------------------------------------------------------------
		// Calculate the width of the drive-trough direction/area
		// and chose the area whoch is the widest free one.
		//----------------------------------------------------------------------------

		// Look in all free found areas
		for (int i=0; i<freeStartAreas.count() ; i++ )
		{
			// set the triangle sides
			b = (laserThread->getValue(LASER1, freeStartAreas.at(i)) * 100); // * 100 since we se cm here
			c = (laserThread->getValue(LASER1, freeEndAreas.at(i))   * 100);

			// use the shorter angle to be sure we fit really through.
			// make the triangle to be one with same-length-sides of b and c!
			if (c < b)
			{
				b = c;
				currentDistance = b;
			}
			else
			{
				c = b;
				currentDistance = c;
			}


			// get the width of one side of the triangle using alpha, b and c.
			// where b and c have to be in cm here!
			currentWidth = calculateDriveThroughWidth(LASER1, (freeEndAreas.at(i) - freeStartAreas.at(i)), currentDistance, currentDistance);

//			qDebug("currentWidth: %.1f",currentWidth);
//			qDebug("largestWidth: %.1f",largestWidth);

			// is the current width the widest so far?
			if (currentWidth > largestWidth)
			{
				// use this width as the most far
				largestWidth = currentWidth;

				//
				// Enable *this* code to chose the widest area, which is also the area with the largest free distance!!
				//

				// check if this width is the width which is the farest away
//				if (currentDistance > farestDistance)
//				{
//					// use this!
//					farestDistance = currentDistance;
//
//					// store the corresponing angles, too!
//					largestFreeAreaStart = freeStartAreas.at(i);
//					largestFreeAreaEnd   = freeEndAreas.at(i);
//				}


				//
				// alternatively use *the following* code for just using the widest area as direction of choice
				//
				// store the corresponing angles
				largestFreeAreaStart = freeStartAreas.at(i);
				largestFreeAreaEnd   = freeEndAreas.at(i);

			}

//			qDebug("Using largestWidth: %.1f",largestWidth);
			// Nothing *else* here! we use the last width and distance as the best choice  ? ? ? ? ? ?

			//laserThread->setFlag(LASER1, angleIndex, LARGESTFREEWAY);
		}

//		qDebug("Final largestWidth: %.1f",largestWidth);
//		qDebug() << "\n";

//		qDebug() << "largestFreeAreaStart:" << largestFreeAreaStart;
//		qDebug() << "largestFreeAreaEnd:"   << largestFreeAreaEnd;


		//----------------------------------------------------------------------------
		// LASER SCANNER 1 DATA ANALYSIS - STEP V
		//----------------------------------------------------------------------------
		// Then tag the *largest* free area
		// (to show it in the GUI and to know, where to drive)
		//----------------------------------------------------------------------------

		// free area found?
		if (largestFreeAreaEnd != 0)
		{
			for (int angleIndex=largestFreeAreaStart; angleIndex<=largestFreeAreaEnd; angleIndex++)
			{
				laserThread->setFlag(LASER1, angleIndex, LARGESTFREEWAY);
			}

			// get the middle of the free area (this is for displaying reasons in the GUI only)
			centerOfFreeWayFront = largestFreeAreaStart + ((largestFreeAreaEnd - largestFreeAreaStart) / 2);

			// set driving direction flag to "light green"
			laserThread->setFlag(LASER1, centerOfFreeWayFront, CENTEROFLARGESTFREEWAY);

			// this signal is only used to display the values and the "driection arrow" in the GUI
			// // the values are multiplied by the resolution to have the correct value in degrees!
			// emit newDrivingAngleSet((largestFreeAreaStart * laserResolutionFront), (largestFreeAreaEnd * laserResolutionFront), (centerOfFreeWayFront * laserResolutionFront), largestWidth);
			emit newDrivingAngleSet(largestFreeAreaStart, largestFreeAreaEnd, centerOfFreeWayFront, largestWidth);


			//----------------------------------------------------------------------------
			// LASER SCANNER 1 DATA ANALYSIS - STEP VI
			//----------------------------------------------------------------------------
			// Emit driving directiomn to the GUI
			//----------------------------------------------------------------------------

			// get the middle of the laser (when we have 180 deg, the middle is as 90 deg)
			// this is needed later for detecting the driving direction
			middleOfLaser = (laserAngleFront / laserResolutionFront) / 2;

			// value within the tolerance range (deviation to 90 deg.)?
			if (
				// when we are within the allowed deviation from the center, we still drive forward
				( (centerOfFreeWayFront < middleOfLaser) && (centerOfFreeWayFront >= (middleOfLaser - (straightForwardDeviation / laserResolutionFront))) ) ||
				( (centerOfFreeWayFront > middleOfLaser) && (centerOfFreeWayFront <= (middleOfLaser + (straightForwardDeviation / laserResolutionFront))) ) ||
				(  centerOfFreeWayFront == middleOfLaser ) // straight forward
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
		else
		{
			// this signal is only used to display the values and the "driection arrow" in the GUI
			emit newDrivingAngleSet(0, 0, 0, 0);

			// obstacles EVERYWHERE IN FRONT (no free area found)
			emit obstacleDetected(OBSTACLESEVERYWHEREINFRONT, QDateTime::currentDateTime());
		}



		//----------------------------------------------------------------------------
		// LASER SCANNER 1 DATA ANALYSIS - STEP VII
		//----------------------------------------------------------------------------
		// Reset the previous found areas, angles and width for the next analysis.
		//----------------------------------------------------------------------------
		freeStartAreas.clear();
		freeEndAreas.clear();
		currentWidth = 0.0;
		largestWidth = 0.0;
		farestDistance = 0.0;
		currentDistance = 0.0;
		b = 0.0;
		c = 0.0;
		largestFreeAreaStart = 0;
		largestFreeAreaEnd = 0;
		centerOfFreeWayFront = 0.0;




		//================================= START LASER 2 =========================================
		//================================= START LASER 2 =========================================
		//================================= START LASER 2 =========================================



		//================================= END LASER 2 =========================================
		//================================= END LASER 2 =========================================
		//================================= END LASER 2 =========================================

		//-------------------------------------------------
		// Let the thread sleep some time,
		// for having a bit more time fo the other threads!
		//-------------------------------------------------
		msleep(THREADSLEEPTIME);
	} // while

	stopped = false;
}


double ObstacleCheckThread::calculateDriveThroughWidth(short int laserscanner, int alpha, float b, float c)
{
	// calculate
	// WARNING: "cos" functions use radians!! so we convert the degrees to radions here!

	if (laserscanner == LASER1)
		return sqrt( pow(b, 2.0) + pow(c, 2.0) - 2.0*b*c * cos(alpha*M_PI / (double) laserAngleFront) );

	if (laserscanner == LASER2)
		return sqrt( pow(b, 2.0) + pow(c, 2.0) - 2.0*b*c * cos(alpha*M_PI / (double) laserAngleRear) );

	emit message(QString("ERROR: laser no. %1 not supported (calculateDriveThroughWidth::%2)").arg(laserscanner).arg(className));
	return 0.0;
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

	emit message(QString("<font color=\"#FF0000\">ERROR in %1::setIgnoreArea: laser number %2 or area %3 not implemented!<font>").arg(className).arg(laser).arg(area));
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
