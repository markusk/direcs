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

Circuit::Circuit(InterfaceAvr *i, QMutex *m)
{
	// copy the pointer from the original object
	interface1 = i;
	mutex = m;

	circuitState = false;
	firstInitDone = false;
	compassCircuitState = false;

	expectedAtmelAnswer = "error";

	// theAtmelcommands
	commandInitCircuit	= "re";
	commandInitCompass	= "cc";
	commandSleep		= "sl";
}


Circuit::~Circuit()
{
}


bool Circuit::initCircuit()
{
	// maybe robot is already recognized as OFF by the interface class (e.g. path to serial port not found)!
	// if the serial port could be opened before calling this method, circuitState will be already TRUE.
	if (circuitState)
	{
		atmelCommand = commandInitCircuit;

		// Lock the mutex. If another thread has locked the mutex then this call will block until that thread has unlocked it.
		mutex->lock();

		//-------------------------------------------------------
		// Basic init for all the bits on the robot circuit
		//-------------------------------------------------------

		// sending RESET (INIT) command
		emit message(QString("Sending %1%2%3...").arg(starter).arg(atmelCommand).arg(terminator));
		if (interface1->sendString(atmelCommand) == true)
		{
			emit message("Sent.");
			emit message("Waiting for an answer...");
			// check if the robot answers with "re"
			if ( interface1->receiveString(atmelAnswer) == true)
			{
				emit message(QString("Answer '%1' received.").arg(atmelAnswer));

				// remove starter and terminator from answer string
				atmelAnswer.remove(starter);
				atmelAnswer.remove(terminator);

				// everthing's fine :-)
				if (atmelAnswer == commandInitCircuit)
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
				emit message("No answer received.");
			}
		}
		else
		{
			emit message("Error sending string.");
		}

		// Unlock the mutex.
		mutex->unlock();

	}

	qDebug("INFO from initCircuit: Robot is OFF.");
	firstInitDone = true;
	circuitState = false;
	emit robotState(false);

	return false;
}


bool Circuit::initCompass()
{
	// maybe robot is already recognized as OFF by the interface class (e.g. path to serial port not found)!
	if (circuitState)
	{
		atmelCommand = commandInitCompass;

		// Lock the mutex. If another thread has locked the mutex then this call will block until that thread has unlocked it.
		mutex->lock();

		// check if the 3D compass sensor is connected to the Atmel board
		if (interface1->sendString("cc") == true)
		{
			// check if the robot answers with "ok"
			if ( interface1->receiveString(atmelAnswer) == true)
			{
				// remove starter and terminator from answer string
				atmelAnswer.remove(starter);
				atmelAnswer.remove(terminator);

				if (atmelAnswer == commandInitCompass)
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
