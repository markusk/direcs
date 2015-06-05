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

#include "demoThread.h"

DemoThread::DemoThread()
{
	stopped = false;
	demoMode = false;

	// start random number generator
	qsrand(time(NULL));
}


DemoThread::~DemoThread()
{
}


void DemoThread::stop()
{
	stopped = true;
}


void DemoThread::run()
{
	int brightness1 = 1;
	int brightness2 = 1;
	int brightness3 = 1;
	//	static bool toggle = false;

	//
	//  start "threading"...
	//
	while (!stopped)
	{
//		toggle = !toggle;

		if (0)
		{
			emit setRGBLEDBrightness(RGBLED1, qrand() % 99 +1);
			emit setRGBLEDBrightness(RGBLED2, qrand() % 99 +1);
			emit setRGBLEDBrightness(RGBLED3, qrand() % 99 +1);
			emit setRGBLEDBrightness(RGBLED4, qrand() % 99 +1);
			emit setRGBLEDBrightness(RGBLED5, qrand() % 99 +1);
			emit setRGBLEDBrightness(RGBLED6, qrand() % 99 +1);
		}
		else
		{
			brightness1 = qrand() % 99 +1;
			brightness2 = qrand() % 99 +1;
			brightness3 = qrand() % 99 +1;

			emit setRGBLEDBrightness(RGBLED1, brightness1);
			emit setRGBLEDBrightness(RGBLED2, brightness2);
			emit setRGBLEDBrightness(RGBLED3, brightness3);

			emit setRGBLEDBrightness(RGBLED4, brightness1);
			emit setRGBLEDBrightness(RGBLED5, brightness2);
			emit setRGBLEDBrightness(RGBLED6, brightness3);
		}

		// let the thread sleep some time - kind of timer...
		msleep(THREADSLEEPTIME);
	}
	stopped = false;
}


bool DemoThread::init()
{
	return true;
}
