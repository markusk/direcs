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
	lastAmountSMS = 0;
	firstSMSCount = true;

	// for storing a virtual heartbeat value (high=5V)
	//heartbeatValue[0] = 0;

	robotState = ON; // Wer're thinking positive. The robot is ON untill whe know nothing other. :-)

	GSMState   = OFF; // will be set to ON, once the init function ran successful

	networkState = GSM_Not_Registered; // we are not registered on the GSM network
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
	QString smsText = "error";


	//  start "threading"...
	while (!stopped)
	{
		// let the thread sleep some time for having more time for the other threads
		msleep(THREADSLEEPTIME);

		if ( (robotState == ON) && (GSMState == ON) && (simulationMode == false) )
		{
			// Lock the mutex. If another thread has locked the mutex then this call will block until that thread has unlocked it.
			mutex->lock();

			//-----------------
			// get GSM status
			//-----------------
			if (getStatus() == false)
			{
				emit message(QString("<font color=\"#FF0000\">ERROR getting GSM state. Stopping %1!</font>").arg(className));

				// Unlock the mutex.
				mutex->unlock();

				// stop this thread
				stop();

				// inform other modules
				emit systemerror(-3);

				return;
			}
			else
			{
				// We have GSM
				if ((networkState == GSM_Registered_Home) || (networkState == GSM_Registered_Roaming))
				{
					emit GSMStatus(GREEN);

					//-----------------
					// count SMS
					//-----------------
					if (countSMS() == -1)
					{
						//
						// disabled: we continue, since this is not critical
						//
					}
					else
					{
						// Do we have a *new* SMS?
						if (availableSMS > lastAmountSMS)
						{
							// store the new amount
							lastAmountSMS = availableSMS;

							// read SMS from GSM module
//							if (readLastSMS(smsText) == true)
							{
								// emit the no. and text of SMS
								emit SMSavailable(availableSMS, smsText);

								// send value over the network
								// *0s42# means 42 SMS available from GSM module 0 (yes, i know, we have only one...)
								emit sendNetworkString( QString("*0s%1#").arg(availableSMS) );
							}
						}
					}
				}
			}


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
	}
	stopped = false;
}


int GSMThread::getSMSavailable()
{
	return availableSMS;
}


int GSMThread::getGSMStatus()
{
	return networkState;
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


bool GSMThread::init()
{
	QString answer = "error";


	// Initialise the GSM module
	// "gsmi"
	if (interface1->sendString("gsmi", className) == true)
	{
		// check if the robot answers with answer. e.g. "*42#"
		if (interface1->receiveString(answer, className) == true)
		{
			if (answer == "*gsmi#")
			{
				GSMState = ON;

				// unlock PIN
				if (unlockPIN() == true)
				{
					emit message("Waiting for GSM...");

					// update GUI (show LED status from call before)
					QCoreApplication::processEvents();

					do
					{
						//-----------------
						// get GSM status
						//-----------------
						if (getStatus() == false)
						{
						}
					} while (((networkState != GSM_Registered_Home) && (networkState != GSM_Registered_Roaming))); // wait for reception...   < < < < < < <  forever!!  Add timeout measurement!!!

					// we have GSM!  :-)
					emit GSMStatus(GREEN);

					// Check very first SMS amount available
					// could be the case, that there are already old SMS on the SIM
					if (firstSMSCount)
					{
						firstSMSCount = false;

						// count SMS from GSM module
						if (countSMS() != -1)
						{
							// store the new amount
							lastAmountSMS = availableSMS;

							// emit message(QString("%1 SMS already on SIM.").arg(availableSMS));

							return true;
						}
					}
				}
			}
		}
	}

	// error
	emit message("ERROR initialising GSM module.");
	emit GSMStatus(RED);
	GSMState = OFF;

	return false;
}


bool GSMThread::unlockPIN()
{
	QString answer = "error";


	// Unlocks the SIM PIN
	// "gsmp"
	if (interface1->sendString("gsmp", className) == true)
	{
		// check if the robot answers with answer. e.g. "*42#"
		if (interface1->receiveString(answer, className) == true)
		{
			if (answer == "*gsmp#")
			{
				GSMState = ON;
				emit GSMStatus(YELLOW);
				emit message("GSM SIM unlocked.");

				return true;
			}
		}
	}

	// error
	emit message("ERROR unlocking GSM SIM PIN.");
	GSMState = OFF;

	return false;
}


bool GSMThread::getStatus()
{
	int value = 0;
	QString answer = "error";


	// get GSM network status
	// "gsms"
	if (interface1->sendString("gsms", className) == true)
	{
		// check if the robot answers with answer. e.g. "*42#"
		if (interface1->receiveString(answer, className) == true)
		{
			// convert to int
			if (interface1->convertStringToInt(answer, value))
			{
				// store measured value
				networkState = value;
				// emit message(QString("GSM network status: %1.").arg(networkState));

				return true;
			}
		}
	}

	// error
	emit message("ERROR getting GSM network status.");
	networkState = 0;

	// disable thread
	GSMState = OFF;

	return false;
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

				// emit message(QString("%1 SMS available.").arg(availableSMS));

				return availableSMS;
			}
		}
	}

	// error
	emit message("GSM: Error reading available SMS");
	availableSMS = -1;

	// we do not disable the whole thread, when we cannot read SMS!
	//	GSMState = OFF;

	return -1;
}


bool GSMThread::readLastSMS(QString &text)
{
	QString answer = "error";


	emit message(QString("TEST1: %1").arg(text));

	mutex->lock();

	// "smsl"
	if (interface1->sendString("smsl", className) == true)
	{
		// check if the robot answers with answer. e.g. "*hello#"
		if (interface1->receiveString(answer, className) == true)
		{
			emit message(QString("TEST2: %1").arg(answer));

			if (answer != "*err#")
			{
				text = answer;
				emit message(QString("SMS: %1").arg(text));

				mutex->unlock();

				return true;
			}
		}
	}

	// error
	emit message("GSM: Error reading last SMS (text).");

	mutex->unlock();

	return false;
}
