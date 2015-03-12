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
	arduinoStateSet = false;

	// unless otherwise told by the circuit class
	robotState = true;
}


TimerThread::~TimerThread()
{
}


void TimerThread::stop()
{
	stopped = true;
}


void TimerThread::setRobotState(bool state)
{
	// store the state within this class
	robotState = state;
}


void TimerThread::run()
{
	// get the current date and time
	startTime = QTime::currentTime();
//	emit message(QString("Timer thread start at %1-%2-%3 %4:%5:%6.%7.").arg(startTime.toString("yyyy")).arg(startTime.toString("MM")).arg(startTime.toString("dd")).arg(startTime.toString("hh")).arg(startTime.toString("mm")).arg(startTime.toString("ss")).arg(startTime.toString("zzz")));
	emit message("Timer thread started now (run).");

	//
	//  start "threading"...
	//
	while (!stopped)
	{
		// let the thread sleep some time
		// for having more time for the other threads
		msleep(THREADSLEEPTIME);

		// get current time
		now = QTime::currentTime();
		// qDebug("%d seconds from pgm start.", startTime.secsTo( QDateTime::currentDateTime() ));

		//------------------
		// master or slave?
		//------------------
		if (networkStateSet==false)
		{
			// time elapsed for receiving a master signal.
			if (startTime.secsTo(now) >= timeToNetworkCheck)
			{
				networkStateSet = true;
				emit checkNetworkState();
			}
		}

		//------------------
		// Arduino called?
		//
		// only do this, when robot is not already identified as 'off'. i.e. open com port did not work
		//------------------
		if ((arduinoStateSet == false) && (robotState == true))
		{
			// time elapsed for receiving a master signal.
			if (startTime.secsTo(now) >= timeToArduinoCheck)
			{
				arduinoStateSet = true;
				emit message("Calling checkArduinoState...");
				emit checkArduinoState();
			}
		}


		//----------------------
		// send 'master' signal
		// every second
		//----------------------
		emit networkMessage();
	}
	stopped = false;
}


bool TimerThread::init()
{
	return true;
}
