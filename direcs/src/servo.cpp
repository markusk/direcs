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

#include "servo.h"

Servo::Servo(InterfaceAvr *i, QMutex *m)
{
	// get the name of this class (this is for debugging messages)
	className = this->staticMetaObject.className();

	// copy the pointer from the original object
	interface1 = i;
	mutex = m;

	// init arrays
	for (int servo=0; servo<NUMBEROFSERVOS; servo++)
	{
		servoStartPosition[servo] = 0;
		servoMinPosition[servo] = 0;
		servoDefaultPosition[servo] = 64;
		servoEndPosition[servo] = 255;
		servoMaxPosition[servo] = 255;
		servoPosition[servo] = servoDefaultPosition[servo];
	}

	robotState = ON; // Wer're thinking positive. The robot is ON until we know nothing other. :-)
}


Servo::~Servo()
{
}


bool Servo::moveServo(unsigned char servo, unsigned char position)
{
	QString answer = "error";


	if (robotState == ON)
	{
		// servo number okay?
		if (servo > NUMBEROFSERVOS-1)
		{
			emit message(QString("<font color=\"#FF0000\">Servo%1 is not an allowed servo numer (1-%2)! (moveServo)</font>").arg(servo+1).arg(NUMBEROFSERVOS-1) );
			return false;
		}


		// wanted servo position okay?
		if ( (position < servoMinPosition[servo]) || (position > servoMaxPosition[servo]) )
		{
			emit message(QString("<font color=\"#FF0000\">Servo%1 position %2 out of allowed range (%3-%4)! (moveServo)</font>").arg(servo+1).arg(position).arg(servoStartPosition[servo]).arg(servoEndPosition[servo]));
			return false;
		}


		// store the new servo position
		servoPosition[servo] = position;

		// Lock the mutex.
		mutex->lock();


		// move servo
		// send command to microcontroller
		if (interface1->sendString(QString("*sv%1%2#").arg(servo + 1).arg(position), className) == true)
		{
			// check if the robot answers with "ok"
			if ( interface1->receiveString(answer, className) == true)
			{
				if (answer == QString("*sv%1#").arg(servo + 1))
				{
					// Unlock the mutex
					mutex->unlock();
					return true;
				}
			}
		}


		// Unlock the mutex.
		mutex->unlock();

		emit message(QString("<font color=\"#FF0000\">Error moving servo%1 (moveServo)</font>").arg(servo+1));
		return false;

	} // robot is ON

	// robot is OFF
	return false;
}


void Servo::setServoPosition(int servo, unsigned char type, unsigned char position)
{
	switch (type)
	{
		case SVSTART:
			servoStartPosition[servo] = position;
			return;
			break;
		case SVEND:
			servoEndPosition[servo] = position;
			return;
			break;
		case SVMIN:
			servoMinPosition[servo] = position;
			return;
			break;
		case SVMAX:
			servoMaxPosition[servo] = position;
			return;
			break;
		case SVDEFAULT:
			servoDefaultPosition[servo] = position;
			return;
			break;
		case SVCURRENT:
			// converting SERVO5 value, because of reverse fixing on the robot!
			if (servo==SERVO5)
			{
			}

			// only within the allowd ranges!
			if (position < servoMinPosition[servo])
			{
				position = servoMinPosition[servo];
			}
			if (position > servoMaxPosition[servo])
			{
				position = servoMaxPosition[servo];
			}
			servoPosition[servo] = position;
			return;
			break;
	}
}


void Servo::init(void)
{
	if (robotState == ON)
	{
		for (int servo=0; servo<NUMBEROFSERVOS; servo++)
		{
			moveServo(servo, servoDefaultPosition[servo]);
			//emit message(QString("Init servo%1 to def-pos: %2").arg(servo+1).arg(servoDefaultPosition[servo]));
		}
	} // robot is ON
}


unsigned char Servo::getServoPosition(int servo, unsigned char type)
{
	if ( (servo < SERVO1) || (servo > (NUMBEROFSERVOS-1)) )
	{
		emit message(QString("<font color=\"#FF0000\">Servo%1 out of allowed range! (getServoPosition)</font>").arg(servo+1));
		return 0;
	}


	switch (type)
	{
		case SVSTART:
			return servoStartPosition[servo];
			break;
		case SVEND:
			return servoEndPosition[servo];
			break;
		case SVMIN:
			return servoMinPosition[servo];
			break;
		case SVMAX:
			return servoMaxPosition[servo];
			break;
		case SVDEFAULT:
			return servoDefaultPosition[servo];
			break;
		case SVCURRENT:
			return servoPosition[servo];
			break;
	}

	// this line is never reached
	return 0;
}


void Servo::setRobotState(bool state)
{
	// store the state within this class
	robotState = state;

	emit message(QString("Robot state set to %1 in %2").arg(state).arg(className));
}
