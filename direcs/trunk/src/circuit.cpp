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
 *   mrs is distributed in the hope that it will be useful,              *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of      *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the        *
 *   GNU General Public License for more details.                        *
 *                                                                       *
 *   You should have received a copy of the GNU General Public License   *
 *   along with mrs. If not, see <http://www.gnu.org/licenses/>.         *
 *                                                                       *
 *************************************************************************/

#include "circuit.h"

Circuit::Circuit(InterfaceAvr *i, QMutex *m)
{
	// copy the pointer from the original object
	interface1 = i;
	mutex = m;

	robotIsOn = false;
	firstInitDone = false;
}


Circuit::~Circuit()
{
}


void Circuit::initCircuit()
{
	// Lock the mutex. If another thread has locked the mutex then this call will block until that thread has unlocked it.
	mutex->lock();

	//-------------------------------------------------------
	// Basic init for all the bits on the robot circuit
	//-------------------------------------------------------
	if (interface1->sendChar(INIT) == true)
	{
		// check if the robot answers with "@"
		unsigned char answer = 0;
		interface1->receiveChar(&answer);

		// everthing's fine :-)
		if (answer == INITANSWER)
		{
			// Unlock the mutex
			mutex->unlock();
			firstInitDone = true;
			robotIsOn = true;
			emit robotState(true);
			return;
		}
	}

	// Unlock the mutex.
	mutex->unlock();

	qDebug("INFO: Robot is OFF.");
	firstInitDone = true;
	robotIsOn = false;
	emit robotState(false);
}


bool Circuit::isConnected()
{
	// if not tried to init hardware, do this!
	if (firstInitDone == false)
	{
		initCircuit();
		firstInitDone = true;
	}

	return robotIsOn;
}
