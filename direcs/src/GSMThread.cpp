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

#include "GSMThread.h"

GSMThread::GSMThread(InterfaceAvr *i, QMutex *m)
{
	// get the name of this class (this is for debugging messages)
	className = this->staticMetaObject.className();

	stopped = false;
	simulationMode = false;

	// copy the pointer from the original object
	interface1 = i;
	mutex = m;

	// reset no of SMS available
	availableSMS = 0;

	// for storing a virtual heartbeat value (high=5V)
	//heartbeatValue[0] = 0;

	robotState = ON; // Wer're thinking positive. The robot is ON untill whe know nothing other. :-)
	GSMState = true; // Same applies here
}


GSMThread::~GSMThread()
{
	// send "off" or "shutdown" heartbeat signal over network to remote app
	//emit heartbeat(RED);

	// send heartbeat over the network
	// *0h2# means 'heartbeat no. 0 is DEAD'
	//emit sendNetworkString("*0h2#");
}


void GSMThread::stop()
{
	stopped = true;

	// send "off" or "shutdown" heartbeat signal over network to remote app
	//emit heartbeat(RED);

	// send heartbeat over the network
	// *0h2# means 'heartbeat no. 0 is DEAD'
	//emit sendNetworkString("*0h2#");
}


void GSMThread::run()
{
	//bool heartbeatToggle = false;


	//  start "threading"...
	while (!stopped)
	{
		// let the thread sleep some time for having more time for the other threads
		msleep(THREADSLEEPTIME);

		if ( (GSMState == ON) && (simulationMode == false) )
		{
			// Lock the mutex. If another thread has locked the mutex then this call will block until that thread has unlocked it.
			mutex->lock();

			//-----------------
			// count SMS
			//-----------------
			if (countSMS() == false)
			{
				emit message(QString("<font color=\"#FF0000\">ERROR counting SMS. Stopping %1!</font>").arg(className));
				// Unlock the mutex.
				 mutex->unlock();
				 // stop this thread
				 stop();
				 // inform other modules
				 emit systemerror(-3);
				 return;
			}
			// send value over the network
			// *0s42# means 42 SMS available from GSM module 0 (yes, i know, we have only one...)
			emit sendNetworkString( QString("*0s%1#").arg(availableSMS) );



/*			//====================================================================
			// send an optical heartbeat signal to the GUI
			if (!heartbeatToggle)
			{
				// set plot value to high = 5 Volt
				heartbeatValue[0] = MAXIMUMPLOTHEARTBEAT;
				emit heartbeat(GREEN);

				// send heartbeat over the network
				// *0h1# means 'heartbeat no. 0 is HIGH'
				emit sendNetworkString("*0h1#");
			}
			else
			{
				// set plot value to low = 0 Volt
				heartbeatValue[0] = 0;
				emit heartbeat(LEDOFF);

				// send heartbeat over the network
				// *0h1# means 'heartbeat no. 0 is LOW'
				emit sendNetworkString("*0h0#");
			}
			heartbeatToggle = !heartbeatToggle;
			//====================================================================
*/

			// Unlock the mutex.
			mutex->unlock();

		} // simulation = false

		if (simulationMode)
		{
			// now we're sleeping additional 500ms because we're only simulating.
			msleep(500);

/*
			// send an (simulated) optical heartbeat signal to the GUI
			if (!heartbeatToggle)
			{
				emit heartbeat(GREEN);
			}
			else
			{
				emit heartbeat(LEDOFF);
			}
			heartbeatToggle = !heartbeatToggle;
*/
		}

		//  e m i t  Signal
		emit GSMDataComplete();
	}
	stopped = false;
}


int GSMThread::getSMSavailable()
{
	return availableSMS;
}




void GSMThread::setSimulationMode(bool state)
{
	simulationMode = state;

	// fill array with some nice values
	if (simulationMode == true)
	{
		availableSMS = 42;
	}
	else
	{
//		if (robotState==OFF)
//			emit heartbeat(RED);

		availableSMS = 0;
	}
}


void GSMThread::setRobotState(bool state)
{
	// store the state within this class
	robotState = state;
}


void GSMThread::setGSMState(bool state)
{
	// store the state within this class
	GSMState = state;
}


int GSMThread::getHeartbeatValue()
{
//	return heartbeatValue[0];
	return 0;
}


int GSMThread::countSMS()
{
	int value = 0;
	QString answer = "error";


	// check for/count no of available SMS
	// "smsc"
	if (interface1->sendString("smsc", className) == true)
	{
		// check if the robot answers with answer. e.g. "*42#"
		if (interface1->receiveString(answer, className) == true)
		{
			// convert to int
			if (interface1->convertStringToInt(answer, value))
			{
				// store measured value
				availableSMS = value;
				return availableSMS;
			}
		}
	}

	// error
	availableSMS = -1;
	return -1;
}
