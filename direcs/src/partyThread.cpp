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
	static bool toggle = false;

	//
	//  start "threading"...
	//
	while (!stopped)
	{
		toggle = !toggle;

		if (toggle)
		{
			emit setRGBLEDBrightness(RGBLED1, 50);
		}
		else
		{
			emit setRGBLEDBrightness(RGBLED1, 1);
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
