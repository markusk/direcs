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
	stopped = false;

	// copy the pointer from the original object
	interface1 = i;
	mutex = m;

	// init arrays
	for (int servo=0; servo<NUMBEROFSERVOS; servo++)
	{
		servoStartPosition[servo] = 1;
		servoMinPosition[servo] = 10;
		servoDefaultPosition[servo] = 19;
		servoEndPosition[servo] = 30;
		servoMaxPosition[servo] = 255;
		servoPosition[servo] = servoDefaultPosition[servo];
	}

	robotState = ON; // Wer're thinking positive. The robot is ON untill whe know nothing other. :-)
}


Servo::~Servo()
{
}


void Servo::stop()
{
	stopped = true;
}

void Servo::run()
{
	//
	//  start "threading"...
	//
	while (!stopped)
	{
		// let the thread sleep some time
		// for having more time for the other threads
		msleep(THREADSLEEPTIME);

		// do something
		// ...
	}
	stopped = false;
}


void Servo::moveServo(unsigned char servo, unsigned char position)
{
	if (robotState == ON)
	{
		//qDebug("moveServo%d to position %d. Start=%d / Default=%d / End=%d", servo, position, servoStartPosition[servo], servoDefaultPosition[servo], servoEndPosition[servo]);

		// *don't* move servo to a position out of the allowed range!!
		if ( (position < servoMinPosition[servo]) || (position > servoMaxPosition[servo]) )
		{
			//emit message(QString("<b><font color=\"#FF0000\">Servo%1 position %2 out of allowed range (%3-%4)! (moveServo)</font>").arg(servo+1).arg(position).arg(servoStartPosition[servo]).arg(servoEndPosition[servo]));
			return;
		}

		// store the newservo position
		servoPosition[servo] = position;

		static bool toggle = false;

		if (!toggle)
		{
			toggle = !toggle;
			emit message("<font color=\"#FF0000\">Servo functions deactivated in the program [servo.cpp]!</font>");
		}

		/// \todo temporarily deactivated (no servos mounted on the current robot)
		/*
		switch (servo)
		{
			case SERVO1:
				// Lock the mutex. If another thread has locked the mutex then this call will block until that thread has unlocked it.
				mutex->lock();
				// which servo
				if (interface1->sendChar(SET_SERVO1) == false)
				{
					// Unlock the mutex.
					mutex->unlock();
					qDebug("ERROR sending to serial port (Servo)");
					return;
				}
				// the position
				if (interface1->sendChar(position) == false)
				{
					// Unlock the mutex.
					mutex->unlock();
					qDebug("ERROR sending to serial port (Servo)");
					return;
				}
				// Unlock the mutex.
				mutex->unlock();
				break;
			case SERVO2:
				// Lock the mutex. If another thread has locked the mutex then this call will block until that thread has unlocked it.
				mutex->lock();
				// which servo
				if (interface1->sendChar(SET_SERVO2) == false)
				{
					// Unlock the mutex.
					mutex->unlock();
					qDebug("ERROR sending to serial port (Servo)");
					return;
				}
				// the position
				if (interface1->sendChar(position) == false)
				{
					// Unlock the mutex.
					mutex->unlock();
					qDebug("ERROR sending to serial port (Servo)");
					return;
				}
				// Unlock the mutex.
				mutex->unlock();
				break;
			case SERVO3:
				// Lock the mutex. If another thread has locked the mutex then this call will block until that thread has unlocked it.
				mutex->lock();
				// which servo
				if (interface1->sendChar(SET_SERVO3) == false)
				{
					// Unlock the mutex.
					mutex->unlock();
					qDebug("ERROR sending to serial port (Servo)");
					return;
				}
				// the position
				if (interface1->sendChar(position) == false)
				{
					// Unlock the mutex.
					mutex->unlock();
					qDebug("ERROR sending to serial port (Servo)");
					return;
				}
				// Unlock the mutex.
				mutex->unlock();
				break;
			case SERVO4:
				// Lock the mutex. If another thread has locked the mutex then this call will block until that thread has unlocked it.
				mutex->lock();
				// which servo
				if (interface1->sendChar(SET_SERVO4) == false)
				{
					// Unlock the mutex.
					mutex->unlock();
					qDebug("ERROR sending to serial port (Servo)");
					return;
				}
				// the position
				if (interface1->sendChar(position) == false)
				{
					// Unlock the mutex.
					mutex->unlock();
					qDebug("ERROR sending to serial port (Servo)");
					return;
				}
				// Unlock the mutex.
				mutex->unlock();
				break;
			case SERVO5:
				// Lock the mutex. If another thread has locked the mutex then this call will block until that thread has unlocked it.
				mutex->lock();
				// which servo
				if (interface1->sendChar(SET_SERVO5) == false)
				{
					// Unlock the mutex.
					mutex->unlock();
					qDebug("ERROR sending to serial port (Servo)");
					return;
				}
				// the position
				if (interface1->sendChar(position) == false)
				{
					// Unlock the mutex.
					mutex->unlock();
					qDebug("ERROR sending to serial port (Servo)");
					return;
				}
				// Unlock the mutex.
				mutex->unlock();
				break;
			case SERVO6:
				// Lock the mutex. If another thread has locked the mutex then this call will block until that thread has unlocked it.
				mutex->lock();
				// which servo
				if (interface1->sendChar(SET_SERVO6) == false)
				{
					// Unlock the mutex.
					mutex->unlock();
					qDebug("ERROR sending to serial port (Servo)");
					return;
				}
				// the position
				if (interface1->sendChar(position) == false)
				{
					// Unlock the mutex.
					mutex->unlock();
					qDebug("ERROR sending to serial port (Servo)");
					return;
				}
				// Unlock the mutex.
				mutex->unlock();
				break;
			default:
				emit message(QString("<b><font color=\"#FF0000\">ERROR: Servo number %1 not supportet (moveServo)</font>").arg(servo));
				break;
		}
	*/
	} // robot is ON
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
	/// \todo temporarily deactivated (no servos mounted on the current robot)
	/*
	if (robotState == ON)
	{
		for (int servo=0; servo<NUMBEROFSERVOS; servo++)
		{
			moveServo(servo, servoDefaultPosition[servo]);
			//emit message(QString("Init servo%1 to def-pos: %2").arg(servo+1).arg(servoDefaultPosition[servo]));
		}
	} // robot is ON
	*/
}


unsigned char Servo::getServoPosition(int servo, unsigned char type)
{
	if ( (servo < SERVO1) || (servo > (NUMBEROFSERVOS-1)) )
	{
		emit message(QString("<b><font color=\"#FF0000\">Servo%1 out of allowed range! (getServoPosition)</font>").arg(servo+1));
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
}
