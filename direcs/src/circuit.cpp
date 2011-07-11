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


	// maybe robot is already recognized as OFF by the interface class (e.g. path to serial port not found)!
	// if the serial port could be opened before calling this method, circuitState will be already TRUE.
	if (circuitState)
	{
		//-------------------------------------------------------
		// Basic init for all the bits on the robot circuit
		//-------------------------------------------------------

		// sending command
		emit message(QString("%1 sends *%2#...").arg(className).arg(command));

		// send command and caller class name to (Atmel) command handler
		emit sendCommand(command, className);
	}
}


void Circuit::initCompass()
{
	QString command = commandInitCompass;


	// maybe robot is already recognized as OFF by the interface class (e.g. path to serial port not found)!
	// if the serial port could be opened before calling this method, circuitState will be already TRUE.
	if (circuitState)
	{
		//-------------------------------------------------------
		// Basic init for the robots compass
		//-------------------------------------------------------

		// sending command
		emit message(QString("%1 sends *%2#...").arg(className).arg(command));

		// send command and caller class name to (Atmel) command handler
		emit sendCommand(command, className);
	}
}


void Circuit::sleep()
{
	QString command = commandSleep;


	// maybe robot is already recognized as OFF by the interface class (e.g. path to serial port not found)!
	// if the serial port could be opened before calling this method, circuitState will be already TRUE.
	if (circuitState)
	{
		//-------------------------------------------------------
		// Basic init for the robots compass
		//-------------------------------------------------------

		// sending command
		emit message(QString("%1 sends *%2#...").arg(className).arg(command));

		// send command and caller class name to (Atmel) command handler
		emit sendCommand(command, className);
	}
}


void Circuit::takeCommandAnswer(QString atmelAnswer, QString caller)
{
	// was it this class which asked for the answer?
	if (caller != className) // not 'stopped=true' check here in contrast to sensorThread
	{
		// debug msg
		//emit message(QString("Answer %1 is not for %2.").arg(atmelAnswer).arg(className));
		return;
	}

	// debug msg
	// emit message(QString("Answer %1 received in %2.").arg(atmelAnswer).arg(className));

	//------
	// okay
	//------

	// remove starter and terminator from string (different to sensorThread)
	atmelAnswer.remove(starter);
	atmelAnswer.remove(terminator);

	//--------------
	// check answer
	//--------------
	if (atmelAnswer == commandInitCircuit) // this is different to sensorThread where the atmelAnswer contains the regarding command AND a sensor value (i.e. *s7=42#)
	{
		firstInitDone = true;
		circuitState = true;
		emit robotState(true);
		return;
	}

	if (atmelAnswer == commandInitCompass) // this is different to sensorThread where the atmelAnswer contains the regarding command AND a sensor value (i.e. *s7=42#)
	{
		// compass init okay
		compassCircuitState = true;
		emit compassState(true);
		return;
	}

	if (atmelAnswer == commandSleep) // this is different to sensorThread where the atmelAnswer contains the regarding command AND a sensor value (i.e. *s7=42#)
	{
		// sleep okay
		emit message("Robot felt asleep.");
		return;
	}


	//-------------------
	// unexpected answer
	//-------------------

	// we do not need to check worng answers for 'sleep' or 'initCompass'. All we need to do now is makr the robot as OFF and let all other moduled know that.
	emit message(QString("ERROR: Answer %1 not expected in %2.").arg(atmelAnswer).arg(className));
	qDebug("INFO from initCircuit: Robot is OFF.");

	// let this class know, that we had an error
	firstInitDone = true;
	circuitState = false;
	compassCircuitState = false;

	// let other classes know, that we had an error
	emit robotState(false);
	emit compassState(false);
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
