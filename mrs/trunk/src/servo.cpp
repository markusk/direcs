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
		servoDefaultPosition[servo] = 11;
		servoEndPosition[servo] = 255;
		servoPosition[servo] = servoDefaultPosition[servo];
	}
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
	//qDebug("moveServo%d to position %d. Start=%d / Default=%d / End=%d", servo, position, servoStartPosition[servo], servoDefaultPosition[servo], servoEndPosition[servo]);

	// *don't* move servo to a position out of the allowed range!!
	if ( (position < servoStartPosition[servo]) || (position > servoEndPosition[servo]) )
	{
		emit message(QString("<b><font color=\"#FF0000\">Servo%1 position %2 out of allowed range (%3-%4)! (moveServo)</font>").arg(servo+1).arg(position).arg(servoStartPosition[servo]).arg(servoEndPosition[servo]));
		return;
	}

	// store the newservo position
	servoPosition[servo] = position;
	
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
			servoPosition[servo] = position;
			return;
			break;
	}
}


void Servo::init(void)
{
	for (int servo=0; servo<NUMBEROFSERVOS; servo++)
	{
		moveServo(servo, servoDefaultPosition[servo]);
		//emit message(QString("Init servo%1 to def-pos: %2").arg(servo+1).arg(servoDefaultPosition[servo]));
	}
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
