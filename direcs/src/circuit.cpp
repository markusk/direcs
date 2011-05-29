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

#include "circuit.h"

Circuit::Circuit(InterfaceAvr *i, QMutex *m) : QThread()
{
	// copy the pointer from the original object
	interface1 = i;
	mutex = m;

	stopped = false;

	circuitState = false;
	firstInitDone = false;
	compassCircuitState = false;

	atmelCommand.clear();
	atmelAnswer = "error";
	answerReceived = false;

	// get the strings emmited from the interfaceAcrt class
	connect(interface1, SIGNAL(commandCompleted(bool, QString)), this, SLOT(getString(bool, QString)));
}


Circuit::~Circuit()
{
	stopped = true;
}


void Circuit::stop()
{
	stopped = true;
}


void Circuit::run()
{
	//  start "threading"...
	while (!stopped)
	{

		// do some heavy threading here...


		// let the thread sleep some time
		msleep(THREADSLEEPTIME);

	} // while !stopped


	stopped = false;
}


bool Circuit::initCircuit()
{
	bool myTimeout = false;


	if (circuitState) // maybe robot is already recognized as OFF by the interface class (e.g. path to serial port not found)!
	{
		// Lock the mutex. If another thread has locked the mutex then this call will block until that thread has unlocked it.
		mutex->lock();

		//-------------------------------------------------------
		// Basic init for all the bits on the robot circuit
		//-------------------------------------------------------

		// sending RESET (INIT) command
		emit message("Sending *re#...");
		if (interface1->sendString("re") == true)
		{
			emit message("Sent.");
			emit message("Waiting for an answer...");

			// start own time measuring
			duration.start();

			// let us wait for an answer from the Atmel
			do
			{
				if (duration.elapsed() > ATMELTIMEOUT)
					myTimeout = true;

			} while ((answerReceived == false) && (myTimeout == false) );


			if (myTimeout)
			{
				emit message(QString("Timeout (%1 > %2ms)").arg(duration.elapsed()).arg(ATMELTIMEOUT));
				return false;
			}


			if (!answerReceived)
			{
				emit message(QString("TEST!! atmelString=%1.").arg(atmelAnswer));
				emit message("No complete answer received.");
				return false;
			}

			// reset indicator
			answerReceived = false;

			// everthing's fine :-)
			emit message("Answer received.");

			// (atmelString is set via Slot getString() )
			if (atmelAnswer == "*ok#")
			{
				emit message("Answer was correct.");
				// Unlock the mutex
				mutex->unlock();

				// ciruit init okay
				firstInitDone = true;
				circuitState = true;
				emit robotState(true);

				return true;
			}

		}
		else
		{
			emit message("Error sending string.");
		}

		// Unlock the mutex.
		mutex->unlock();

	} // robot alread marked as OFF

	qDebug("INFO from initCircuit: Robot is OFF.");
	firstInitDone = true;
	circuitState = false;
	emit robotState(false);

	return false;
}


bool Circuit::initCompass()
{
	QString answer = "error";


	if (circuitState) // maybe robot is already recognized as OFF by the interface class (e.g. path to serial port not found)!
	{
		// Lock the mutex. If another thread has locked the mutex then this call will block until that thread has unlocked it.
		mutex->lock();

		// check if the 3D compass sensor is connected to the Atmel board
		if (interface1->sendString("cc") == true)
		{
			// check if the robot answers with "ok"
			if ( interface1->receiveString(answer) == true)
			{
				if (answer == "*ok#")
				{
					// Unlock the mutex
					mutex->unlock();

					compassCircuitState = true;
					emit compassState(true);

					return true;
				}
			}
		}

		// Unlock the mutex.
		mutex->unlock();

	}

	compassCircuitState = false;
	emit compassState(false);

	return false;
}


bool Circuit::isConnected()
{
	// if not tried to init hardware, do this!
	if (firstInitDone == false)
	{
		initCircuit();
		firstInitDone = true;
	}

	return circuitState;
}


bool Circuit::compassConnected()
{
	// if not tried to init the robots (and compass) hardware, do this!
	if (firstInitDone == false)
	{
		initCircuit();
		firstInitDone = true;
	}

	return compassCircuitState;
}


void Circuit::setRobotState(bool state)
{
	// store the state within this class
	circuitState = state;
	qDebug("Circuit::setRobotState: state=%d", circuitState);
}


bool Circuit::sleep()
{
	QString answer = "error";


	if (circuitState) // maybe robot is already recognized as OFF by the interface class (e.g. path to serial port not found)!
	{
		// Lock the mutex. If another thread has locked the mutex then this call will block until that thread has unlocked it.
		mutex->lock();

		// sending SLEEP command
		if (interface1->sendString("sl") == true)
		{
			// check if the robot answers with "sl"
			if ( interface1->receiveString(answer) == true)
			{
				// everthing's fine
				if (answer == "*sl#")
				{
					// Unlock the mutex
					mutex->unlock();

					return true;
				}
			}
		}

		// Unlock the mutex.
		mutex->unlock();

	}

	return false;
}


void Circuit::getString(bool state, QString string)
{
	answerReceived = state;
	atmelAnswer = string;
	emit message( QString("Slot getstring received: %1").arg(string) );
}
