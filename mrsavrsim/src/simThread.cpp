/*************************************************************************
 *   Copyright (C) 2008 by Markus Knapp                                  *
 *   mrs @ direcs.de                                                     *
 *                                                                       *
 *   This file is part of mrs.                                           *
 *                                                                       *
 *   mrs is free software: you can redistribute it and/or modify it      *
 *   under the terms of the GNU General Public License as published      *
 *   by the Free Software Foundation, version 3 of the License.          *
 *                                                                       *
 *   mrs is distributed in the hope that it will be useful,              *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of      *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the        *
 *   GNU General Public License for more details.                        *
 *                                                                       *
 *   You should have received a copy of the GNU General Public License   *
 *   along with mrs. If not, see <http://www.gnu.org/licenses/>.         *
 *                                                                       *
 *************************************************************************/

#include "simThread.h"

SimThread::SimThread(InterfaceAvr *i, QMutex *m)
{
	stopped = false;
	simulationMode = false;
	
	// copy the pointer from the original object
	interface1 = i;
	mutex = m;
}


SimThread::~SimThread()
{
}


void SimThread::stop()
{
	stopped = true;
}


void SimThread::run()
{
	
	
	//
	//  start "threading"...
	//
	while (!stopped)
	{
		// let the thread sleep some time
		// for having more time for the other threads
		msleep(THREADSLEEPTIME);
		
		
		if (simulationMode == false)
		{

			// Lock the mutex. If another thread has locked the mutex then this call will block until that thread has unlocked it.
			mutex->lock();
			
/*
infrared Sensors temporarily removed from robot!!
			
			// TODO: open, init and close serial port
			
			//------------------------------------------------------
			// read value from sensor 1
			//------------------------------------------------------
			if (interface1->sendChar(READ_SENSOR_1) == false)
			{
				// Unlock the mutex.
				mutex->unlock();
				qDebug("ERROR s1 sending to serial port (SimThread)");
				return;
			}
			
			// receive the 16 Bit answer from the MC
			if (interface1->receiveInt(&value) == false)
			{
				// Unlock the mutex.
				mutex->unlock();
			}
*/
	
			
			// Unlock the mutex.
			mutex->unlock();

		} // simulation = false
		
		//====================================================================
		//  e m i t  Signal
		//====================================================================
		emit sensorDataComplete();
	}
	stopped = false;
}
