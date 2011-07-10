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

Circuit::Circuit() : QObject()
{
	className = this->staticMetaObject.className();

	circuitState = true; //  has to be TRUE at startup for the first init! Could be set to false, later if we fail to initialise the circuit.
	firstInitDone = false;
	compassCircuitState = false;

	// the Atmel commands
	commandInitCircuit = "re";
	commandInitCompass	= "cc";
	commandSleep		= "sl";
}


Circuit::~Circuit()
{
}


void Circuit::initCircuit()
{
	QString command = commandInitCircuit;

	/*
	// maybe robot is already recognized as OFF by the interface class (e.g. path to serial port not found)!
	// if the serial port could be opened before calling this method, circuitState will be already TRUE.
	if (circuitState)
	{
		atmelCommand = commandInitCircuit;
		expectedAtmelAnswer = "*re#";

		// Lock the mutex. If another thread has locked the mutex then this call will block until that thread has unlocked it.
		mutex->lock();

		//-------------------------------------------------------
		// Basic init for all the bits on the robot circuit
		//-------------------------------------------------------
		// sending RESET (INIT) command
//		emit message(QString("Sending *%1#...").arg(atmelCommand));
		if (interface1->sendString(atmelCommand) == true)
		{
			// start own time measuring. This will be used, if we get an answer from the Atmel
			duration.start();

			// start additional seperate timer. If we NEVER get an answer, this slot will be called
			QTimer::singleShot(ATMELTIMEOUT, this, SLOT(timeout()) );

//			emit message("Sent.");
//			emit message("Waiting for an answer...");

			// Unlock the mutex.
			mutex->unlock();

			return;
		}

		emit message("Error sending string.");

		// Unlock the mutex.
		mutex->unlock();
	}

	// error
	qDebug("INFO from initCircuit: Robot is OFF.");
	emit message("Robot is OFF.");
	atmelCommand = "none"; // reset current command
	expectedAtmelAnswer.clear();
	firstInitDone = true;
	circuitState = false;

	// emit result (instead of old-school returning 'bool')
	emit robotState(false);
	*/

	emit message(QString("%1 sends *%2#...").arg(className).arg(command));

	// send command and caller class name to (Atmel) command handler
	emit sendCommand(command, className);
}


void Circuit::initCompass()
{
/*
	// maybe robot is already recognized as OFF by the interface class (e.g. path to serial port not found)!
	if (circuitState)
	{
		atmelCommand = commandInitCompass;
		expectedAtmelAnswer = "*cc#";

		// Lock the mutex. If another thread has locked the mutex then this call will block until that thread has unlocked it.
		mutex->lock();

		//-------------------------------------------------------
		// Basic init for all the bits on the robot circuit
		//-------------------------------------------------------
		// sending command
//		emit message(QString("Sending *%1#...").arg(atmelCommand));
		if (interface1->sendString(atmelCommand) == true)
		{
			// start own time measuring. This will be used, if we get an answer from the Atmel
			duration.start();

			// start additional seperate timer. If we NEVER get an answer, this slot will be called
			QTimer::singleShot(ATMELTIMEOUT, this, SLOT(timeout()) );

//			emit message("Sent.");
//			emit message("Waiting for an answer...");

			// Unlock the mutex.
			mutex->unlock();

			return;
		}

		emit message("Error sending string.");

		// Unlock the mutex.
		mutex->unlock();
	}

	// error
	atmelCommand = "none"; // reset current command
	expectedAtmelAnswer.clear();
	compassCircuitState = false;
	emit message("Compass is OFF.");

	emit compassState(false);
	*/


	/// TEST TEST TEST with commandHandler

	compassCircuitState = true;
	emit message("Compass is ON (fake).");

	emit compassState(true);
}


void Circuit::sleep()
{
/*
	// maybe robot is already recognized as OFF by the interface class (e.g. path to serial port not found)!
	if (circuitState)
	{
		atmelCommand = commandSleep;
		expectedAtmelAnswer = "*sl#";

		// Lock the mutex. If another thread has locked the mutex then this call will block until that thread has unlocked it.
		mutex->lock();

		//---------------------------------------------------------------------------
		// Sleep command for the robot circuit (disables the watchdog on the Atmel)
		//---------------------------------------------------------------------------
		// sending command
//		emit message(QString("Sending *%1#...").arg(atmelCommand));
		if (interface1->sendString(atmelCommand) == true)
		{
			// start own time measuring. This will be used, if we get an answer from the Atmel
			duration.start();

			// start additional seperate timer. If we NEVER get an answer, this slot will be called
			QTimer::singleShot(ATMELTIMEOUT, this, SLOT(timeout()) );

//			emit message("Sent.");
//			emit message("Waiting for an answer...");

			// Unlock the mutex.
			mutex->unlock();

			return;
		}

		emit message("Error sending string.");

		// Unlock the mutex.
		mutex->unlock();
	}

	// error
	atmelCommand = "none"; // reset current command
	expectedAtmelAnswer.clear();
	circuitState = false;
	expectedAtmelAnswer.clear();

	emit robotState(false); /// @todo check if we should use the 'massive error handling' here or if this is relevant, since we only call this when we shutdown direcs
*/
}


void Circuit::takeCommandAnswer(QString atmelAnswer, QString caller)
{
/*
//	emit message( QString("Circuit takes answer for %1: %2").arg(atmelCommand).arg(atmelAnswer) );
//	emit message( QString("Circuit checks: %1 = %2 ?").arg(regardingCommand).arg(atmelCommand) );
/// @todo simplifiy takeCommandAnswer like in circuit.cpp!
	if (regardingCommand != atmelCommand)
	{
//		emit message(QString("Answer %1 is not for me (Circuit).").arg(atmelAnswer));
		return;
	}

	//----------
	// timeout?
	//----------
	if (duration.elapsed() > ATMELTIMEOUT)
	{
		emit message(QString("Timeout (%1ms > %2ms)").arg(duration.elapsed()).arg(ATMELTIMEOUT));

		// check the last command
		if (atmelCommand == commandInitCircuit)
		{
			// timeout
			qDebug("INFO from initCircuit: Robot is OFF.");
			atmelCommand = "none"; // reset current command
			expectedAtmelAnswer.clear();
			firstInitDone = true;
			circuitState = false;
			emit robotState(false);
			return;
		} // initCircuit

		// check the last command
		if (atmelCommand == commandInitCompass)
		{
			// timeout
			atmelCommand = "none"; // reset current command
			expectedAtmelAnswer.clear();
			compassCircuitState = false;
			emit compassState(false);
			return;
		} // InitCompass

		// check the last command
		if (atmelCommand == commandSleep)
		{
			// timeout
			atmelCommand = "none"; // reset current command
			expectedAtmelAnswer.clear();
			circuitState = false;
			emit robotState(false); /// @todo check if we should use the 'massive error handling' here or if this is relevant, since we only call this when we shutdown direcs
			return;
		} // sleep
	}

	//------------------
	// everthing's fine
	//------------------
	if (atmelAnswer == expectedAtmelAnswer)
	{
//		emit message(QString("Answer %1 was correct (Circuit).").arg(atmelAnswer));

		// check the last command
		if (atmelCommand == commandInitCircuit)
		{
			// ciruit init okay
			atmelCommand = "none"; // reset current command
			firstInitDone = true;
			circuitState = true;
			emit robotState(true);
			return;
		} // initCircuit

		// check the last command
		if (atmelCommand == commandInitCompass)
		{
			// compass init okay
			atmelCommand = "none"; // reset current command
			compassCircuitState = true;
			emit compassState(true);
			return;
		} // InitCompass

		// check the last command
		if (atmelCommand == commandSleep)
		{
			// command okay
			atmelCommand = "none"; // reset current command
			return;
		} // sleep
	}
	else
	{
		//--------------
		// wrong answer
		//--------------
		emit message(QString("ERROR: Answer was %1 intead of %2.").arg(atmelAnswer).arg(expectedAtmelAnswer));

		// check the last command
		if (atmelCommand == commandInitCircuit)
		{
			qDebug("INFO from initCircuit: Robot is OFF.");
			atmelCommand = "none"; // reset current command
			expectedAtmelAnswer.clear();
			firstInitDone = true;
			circuitState = false;
			emit robotState(false);
			return;
		} // initCircuit

		// check the last command
		if (atmelCommand == commandInitCompass)
		{
			atmelCommand = "none"; // reset current command
			expectedAtmelAnswer.clear();
			compassCircuitState = false;
			emit compassState(false);
			return;
		} // InitCompass

		// check the last command
		if (atmelCommand == commandSleep)
		{
			atmelCommand = "none"; // reset current command
			expectedAtmelAnswer.clear();
			circuitState = false;
			emit robotState(false); /// @todo check if we should use the 'massive error handling' here or if this is relevant, since we only call this when we shutdown direcs
			return;
		} // sleep
	}
*/

	// was it this class which asked for the answer?
	if (caller != className) // not 'stopped=true' check here in contrast to sensorThread
	{
		emit message(QString("Answer %1 is not for %2.").arg(atmelAnswer).arg(className));
		return;
	}

	// debug msg
	// emit message(QString("Answer %1 received in %2. Command=%3.").arg(atmelAnswer).arg(className).arg(commandInitCircuit));

	//------
	// okay
	//------

	// remove starter and terminator from string (different to sensorThread)
	atmelAnswer.remove(starter);
	atmelAnswer.remove(terminator);

	// check answer
	if (atmelAnswer == commandInitCircuit) // this is different to sensorThread where the atmelAnswer contains the regarding command AND a sensor value (i.e. *s7=42#)
	{
		firstInitDone = true;
		circuitState = true;
		emit robotState(true);
		return;
	}


	//-------------------
	// unexpected answer
	//-------------------
	qDebug("INFO from initCircuit: Robot is OFF.");
	emit message(QString("ERROR: Answer %1 not expected in %2.").arg(atmelAnswer).arg(className));

	// let this class know, that we had an error
	firstInitDone = true;
	circuitState = false;

	// let other classes know, that we had an error
	emit robotState(false);

//	varMutex.lock();
///	atmelCommand = "none"; /// reset current command      < < < < <   check this < < < < also senorThread ! !
//	varMutex.unlock();
}


void Circuit::timeout()
{
/*
	/// @todo simplifiy takeCommandAnswer like in circuit.cpp!
	// check the last command
	if (atmelCommand == commandInitCircuit)
	{
		// first check if we had already an answer from the Atmel
		if (firstInitDone == true)
		{
			// we are happy
			return;
		}

		// timeout
		emit message(QString("Timeout (> %2ms)").arg(ATMELTIMEOUT));
		qDebug("INFO from initCircuit: Robot is OFF.");
		firstInitDone = true;
		circuitState = false;
		emit robotState(false);

		return;
	} // initCircuit


	// check the last command
	if (atmelCommand == commandInitCompass)
	{
		// check if we have already a valid answer
		if (compassCircuitState == true)
		{
			// we are happy
			return;
		}

		// timeout
		emit message(QString("Timeout (> %2ms)").arg(ATMELTIMEOUT));
		compassCircuitState = false;
		emit message("Compass is OFF.");
		emit compassState(false);

		return;
	} // InitCompass


	// check the last command
	if (atmelCommand == commandSleep)
	{
		// check if we have already a valid answer
		if (circuitState == true)
		{
			// we are happy
			return;
		}

		// timeout
		emit message(QString("Timeout (> %2ms)").arg(ATMELTIMEOUT));
		emit message("Robot is OFF.");
		/// @todo do we need this information in other classes? normaly only called once at direcs shutdown to stop the Atnel watchdog
		// emit robotState(false);

		return;
	} // sleep
*/
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
		initCircuit(); /// @todo implement this initCircuit and compassConnected for event mode!
		firstInitDone = true;
	}

	return compassCircuitState;
}
