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

#include "partyThread.h"

PartyThread::PartyThread()
{
	stopped = false;
	partyMode = false;

	// start random number generator
	srand(time(NULL));
}


PartyThread::~PartyThread()
{
}


void PartyThread::stop()
{
	stopped = true;
}


void PartyThread::run()
{
	int number1 = 1;
	int number2 = 1;
	int number3 = 1;
	//	static bool toggle = false;

	//
	//  start "threading"...
	//
	while (!stopped)
	{
//		toggle = !toggle;

		if (0)
		{
			emit setRGBLEDBrightness(RGBLED1, rand() % 99 +1);
			emit setRGBLEDBrightness(RGBLED2, rand() % 99 +1);
			emit setRGBLEDBrightness(RGBLED3, rand() % 99 +1);
			emit setRGBLEDBrightness(RGBLED4, rand() % 99 +1);
			emit setRGBLEDBrightness(RGBLED5, rand() % 99 +1);
			emit setRGBLEDBrightness(RGBLED6, rand() % 99 +1);
		}
		else
		{
			number1 = rand() % 99 +1;
			number2 = rand() % 99 +1;
			number3 = rand() % 99 +1;

			emit setRGBLEDBrightness(RGBLED1, number1);
			emit setRGBLEDBrightness(RGBLED2, number2);
			emit setRGBLEDBrightness(RGBLED3, number3);

			emit setRGBLEDBrightness(RGBLED4, number1);
			emit setRGBLEDBrightness(RGBLED5, number2);
			emit setRGBLEDBrightness(RGBLED6, number3);
		}

		// let the thread sleep some time - kind of timer...
		msleep(THREADSLEEPTIME);
	}
	stopped = false;
}


bool PartyThread::init()
{
	return true;
}
