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

Circuit::Circuit(InterfaceAvr *i, QMutex *m) : QObject()
{
	// copy the pointer from the original object
	interface1 = i;
	mutex = m;

	circuitState = false; //  has to be TRUE at startup. Could be set to false, if InterfaceAvr is not finding a serial port. Will be later set within the run method.
	firstInitDone = false;
	compassCircuitState = false;

	atmelCommand.clear();
	expectedAtmelAnswer.clear();
	answerReceived = false;
	answerTimeout = false;

	// get the strings emmited from the interfaceAcrt class
	connect(interface1, SIGNAL(commandCompleted(bool, QString)), this, SLOT(takeAnswer(bool, QString)));
}


Circuit::~Circuit()
{
}


void Circuit::initCircuit()
{
	// maybe robot is already recognized as OFF by the interface class (e.g. path to serial port not found)!
	if (circuitState)
	{
		atmelCommand = "re";
		expectedAtmelAnswer = "*ok#";

		// Lock the mutex. If another thread has locked the mutex then this call will block until that thread has unlocked it.
		mutex->lock();

		//-------------------------------------------------------
		// Basic init for all the bits on the robot circuit
		//-------------------------------------------------------
		// sending RESET (INIT) command
		emit message("Sending *re#...");
		if (interface1->sendString(atmelCommand) == true)
		{
			// start own time measuring. This will be used, if we get an answer from the Atmel
			duration.start();

			// start additional seperate timer. If we NEVER get an answer, this slot will be called
			QTimer::singleShot(ATMELTIMEOUT, this, SLOT(timeout()) );

			emit message("Sent.");
			emit message("Waiting for an answer...");

			// Unlock the mutex.
			mutex->unlock();

			return;
		}

		emit message("Error sending string.");
	}

	// Unlock the mutex.
	mutex->unlock();

	qDebug("INFO from initCircuit: Robot is OFF.");
	emit message("Robot is OFF.");
	expectedAtmelAnswer.clear();
	firstInitDone = true;
	circuitState = false;
	emit robotState(false);
}


void Circuit::takeCircuitAnswer(bool state, QString atmelAnswer)
{
	answerReceived = state;
	emit message( QString("takeAnswer: %1").arg(atmelAnswer) );

	// how long did it take?
	if (duration.elapsed() > ATMELTIMEOUT)
	{
		emit message(QString("Timeout (%1 > %2ms)").arg(duration.elapsed()).arg(ATMELTIMEOUT));

		qDebug("INFO from initCircuit: Robot is OFF.");
		firstInitDone = true;
		circuitState = false;
		atmelCommand.clear();
		expectedAtmelAnswer.clear();
		emit robotState(false);

		return;
	}

	// everthing's fine :-)
	emit message("Answer received.");

	if (atmelAnswer == expectedAtmelAnswer)
	{
		emit message(QString("Answer %1 was correct.").arg(atmelAnswer));

		// ciruit init okay
		firstInitDone = true;
		circuitState = true;
		atmelCommand.clear();
		expectedAtmelAnswer.clear();
		emit robotState(true);

		return;
	}
	else
	{
		emit message(QString("ERROR: Answer was %1 intead of %2.").arg(atmelAnswer).arg(expectedAtmelAnswer));

		qDebug("INFO from initCircuit: Robot is OFF.");
		firstInitDone = true;
		circuitState = false;
		atmelCommand.clear();
		expectedAtmelAnswer.clear();
		emit robotState(false);

		return;
	}
}


void Circuit::timeout()
{
	// check if we have already a valid answer
	if (circuitState == true)
	{
		// we are happy
		return;
	}


	emit message(QString("Timeout (> %2ms)").arg(ATMELTIMEOUT));

	qDebug("INFO from initCircuit: Robot is OFF.");
	firstInitDone = true;
	circuitState = false;
	atmelCommand.clear();
	expectedAtmelAnswer.clear();
	emit robotState(false);
}


bool Circuit::initCompass()
{
/*
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
*/
	compassCircuitState = false;
	emit compassState(false);

	return false;
}


bool Circuit::isConnected()
{
	// if not tried to init hardware, do this!
	if (firstInitDone == false)
	{
		initCircuit(); /// @todo implement this initCircuit and isConnected for event mode!
		firstInitDone = true;
	}

	return circuitState;
}


bool Circuit::compassConnected()
{
	// if not tried to init the robots (and compass) hardware, do this!
	if (firstInitDone == false)
	{
		initCircuit(); /// @todo implement this initCircuit and compassConnected for event mode!
		firstInitDone = true;
	}

	return compassCircuitState;
}


void Circuit::setRobotState(bool state)
{
	// store the state within this class
	circuitState = state;
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
