/*************************************************************************
 *   Copyright (C) 2011 by Markus Knapp                                  *
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

#include "rgbLed.h"

RgbLed::RgbLed(InterfaceAvr *i, QMutex *m)
{
	// get the name of this class (this is for debugging messages)
	className = this->staticMetaObject.className();

	// copy the pointer from the original object
	interface1 = i;
	mutex = m;

	// init arrays
	for (int rgbLed=0; rgbLed<NUMBEROFRGBLEDS; rgbLed++)
	{
		minBrightness[rgbLed] = 0;
		defaultBrightness[rgbLed] = 64;
		maxBrightness[rgbLed] = 255;
		brightness[rgbLed] = defaultBrightness[rgbLed];
	}

	robotState = ON; // Wer're thinking positive. The robot is ON until we know nothing other. :-)
}


RgbLed::~RgbLed()
{
}


bool RgbLed::setBrightness(unsigned char rgbLed, unsigned char bness)
{
	QString answer = "error";


	if (robotState == ON)
	{
		// rgbLed number okay?
		if (rgbLed > NUMBEROFRGBLEDS-1)
		{
			emit message(QString("<font color=\"#FF0000\">RgbLed%1 is not an allowed rgbLed numer (1-%2)! (setRgbLed)</font>").arg(rgbLed+1).arg(NUMBEROFRGBLEDS-1) );
			return false;
		}


		// wanted rgbLed position okay?
		if ( (bness < minBrightness[rgbLed]) || (bness > maxBrightness[rgbLed]) )
		{
			emit message(QString("<font color=\"#FF0000\">RGBLED%1 brightness %2 out of allowed range (%3-%4)! (setRgbLed)</font>").arg(rgbLed+1).arg(bness).arg(minBrightness[rgbLed]).arg(maxBrightness[rgbLed]));
			return false;
		}


		// store the new rgbLed position
		brightness[rgbLed] = bness;

		// Lock the mutex.
		mutex->lock();


		// move rgbLed
		// send command to microcontroller
		if (interface1->sendString(QString("*sv%1%2#").arg(rgbLed + 1).arg(bness), className) == true)
		{
			// check if the robot answers with "ok"
			if ( interface1->receiveString(answer, className) == true)
			{
				if (answer == QString("*sv%1#").arg(rgbLed + 1))
				{
					// Unlock the mutex
					mutex->unlock();
					return true;
				}
			}
		}


		// Unlock the mutex.
		mutex->unlock();

		emit message(QString("<font color=\"#FF0000\">Error moving rgbLed%1 (moveRgbLed)</font>").arg(rgbLed+1));
		return false;

	} // robot is ON

	// robot is OFF
	return false;
}


void RgbLed::setRgbLedBrightness(int rgbLed, unsigned char type, unsigned char bness)
{
	switch (type)
	{
		case RGBLEDMIN:
			minBrightness[rgbLed] = bness;
			return;
			break;
		case RGBLEDMAX:
			maxBrightness[rgbLed] = bness;
			return;
			break;
		case RGBLEDDEFAULT:
			defaultBrightness[rgbLed] = bness;
			return;
			break;
		case RGBLEDACTUAL:
			brightness[rgbLed] = bness;
			return;
			break;
	}
}


void RgbLed::init(void)
{
	if (robotState == ON)
	{
		for (int rgbLed=0; rgbLed<NUMBEROFRGBLEDS; rgbLed++)
		{
			setBrightness(rgbLed, defaultBrightness[rgbLed]);
			//emit message(QString("Init rgbLed%1 to def-pos: %2").arg(rgbLed+1).arg(rgbLedDefaultPosition[rgbLed]));
		}
	}
}


unsigned char RgbLed::getRgbLedBrightness(int rgbLed, unsigned char type)
{
	if ( (rgbLed < RGBLED1) || (rgbLed > (NUMBEROFRGBLEDS-1)) )
	{
		emit message(QString("<font color=\"#FF0000\">RgbLed%1 out of allowed range! (getRgbLedPosition)</font>").arg(rgbLed+1));
		return 0;
	}


	switch (type)
	{
		case RGBLEDMIN:
			return minBrightness[rgbLed];
			break;
		case RGBLEDMAX:
			return maxBrightness[rgbLed];
			break;
		case RGBLEDDEFAULT:
			return defaultBrightness[rgbLed];
			break;
		case RGBLEDACTUAL:
			return brightness[rgbLed];
			break;
	}

	// this line is never reached
	return 0;
}


void RgbLed::setRobotState(bool state)
{
	// store the state within this class
	robotState = state;

	emit message(QString("Robot state set to %1 in %2").arg(state).arg(className));
}
