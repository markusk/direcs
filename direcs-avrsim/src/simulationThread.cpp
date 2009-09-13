/*************************************************************************
 *   Copyright (C) 2009 by Markus Knapp                                  *
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

#include "simulationThread.h"

SimulationThread::SimulationThread(InterfaceAvr *i, QMutex *m)
{
	stopped = false;
	simulationMode = false;

	// copy the pointer from the original object
	interface1 = i;
	mutex = m;

	robotState = ON; // Wer're thinking positive. The robot is ON untill whe know nothing other. :-)
}


SimulationThread::~SimulationThread()
{
}


void SimulationThread::stop()
{
	stopped = true;
}


void SimulationThread::run()
{
// 	int value = 0;
	bool heartbeatToggle = false;


	//
	//  start "threading"...
	//
	while (!stopped)
	{
		// let the thread sleep some time
		// for having more time for the other threads
		msleep(THREADSLEEPTIME);


		if ( (robotState == ON) && (simulationMode == false) )
		{

			// Lock the mutex. If another thread has locked the mutex then this call will block until that thread has unlocked it.
			mutex->lock(); // TODO: do a lock direct before a interface call!!

/*
			//---------------------------------------------------------
			// read value from voltage sensor 1 (formerly IR sensor 8)
			//---------------------------------------------------------
			if (interface1->sendChar(READ_SENSOR_8) == false)
			{
				// Unlock the mutex.
				mutex->unlock();
				qDebug("ERROR reading voltage sensor 1 [SimulationThread]");
				return;
			}

			// receive the 16 Bit answer from the MC
			interface1->receiveInt(&value);
			
			//====================================================================
*/

			//====================================================================
			// Unlock the mutex.
			//====================================================================
			mutex->unlock();

		} // simulation = false
		
		if (simulationMode)
		{
			// FIXME: here or where, when simulating?????  send an optical heartbeat signal to the GUI
			// TODO: now we're sleeping 500ms because ww're only simulating. Is this okay????
			msleep(500);
			heartbeatToggle = !heartbeatToggle;
			
			emit heartbeat(heartbeatToggle);
		}

	}
	stopped = false;
}


void SimulationThread::setSimulationMode(bool state)
{
	simulationMode = state;

	// fill array with some nice values
	if (simulationMode == true)
	{
		// TODO: fill with content
	}
	else
	{
		// TODO: fill with content
	}
}


void SimulationThread::setRobotState(bool state)
{
	// store the state within this class
	robotState = state;
}
