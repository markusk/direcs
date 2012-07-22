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

#include "timerThread.h"

TimerThread::TimerThread()
{
	stopped = false;
	networkStateSet = false;
}


TimerThread::~TimerThread()
{
}


void TimerThread::stop()
{
	stopped = true;
}


void TimerThread::run()
{
	// get the current date and time
	startTime = QDateTime::currentDateTime();
	emit message(QString("Timer thread run at %1-%2-%3 %4:%5:%6.%7.").arg(startTime.toString("yyyy")).arg(startTime.toString("MM")).arg(startTime.toString("dd")).arg(startTime.toString("hh")).arg(startTime.toString("mm")).arg(startTime.toString("ss")).arg(startTime.toString("zzz")));


	//
	//  start "threading"...
	//
	while (!stopped)
	{
		// let the thread sleep some time
		// for having more time for the other threads
		msleep(THREADSLEEPTIME);

		// get current time
		now = QDateTime::currentDateTime();
		// qDebug("%d seconds from pgm start.", startTime.secsTo( QDateTime::currentDateTime() ));

		if (networkStateSet==false)
		{
			if (startTime.secsTo(now) >= timeToNetworkCheck)
			{
				networkStateSet = true;
				emit checkNetworkState();
			}
		}

	}
	stopped = false;
}


bool TimerThread::init()
{
	return true;
}
