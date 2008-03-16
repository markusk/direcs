#include "servo.h"

Servo::Servo(InterfaceAvr *i)
{
	//------------------------------------------------------------------
	// copy the pointer from the original usb object
	//------------------------------------------------------------------
	interface1 = i;

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
			// which servo
			if (interface1->sendChar(SET_SERVO1) == false)
			{
				qDebug("ERROR sending to serial port (Servo)");
				return;
			}
			// the position
			if (interface1->sendChar(position) == false)
			{
				qDebug("ERROR sending to serial port (Servo)");
				return;
			}
			break;
		case SERVO2:
			// which servo
			if (interface1->sendChar(SET_SERVO2) == false)
			{
				qDebug("ERROR sending to serial port (Servo)");
				return;
			}
			// the position
			if (interface1->sendChar(position) == false)
			{
				qDebug("ERROR sending to serial port (Servo)");
				return;
			}
			break;
		case SERVO3:
			// which servo
			if (interface1->sendChar(SET_SERVO3) == false)
			{
				qDebug("ERROR sending to serial port (Servo)");
				return;
			}
			// the position
			if (interface1->sendChar(position) == false)
			{
				qDebug("ERROR sending to serial port (Servo)");
				return;
			}
			break;
		case SERVO4:
			// which servo
			if (interface1->sendChar(SET_SERVO4) == false)
			{
				qDebug("ERROR sending to serial port (Servo)");
				return;
			}
			// the position
			if (interface1->sendChar(position) == false)
			{
				qDebug("ERROR sending to serial port (Servo)");
				return;
			}
			break;
		case SERVO5:
			// which servo
			if (interface1->sendChar(SET_SERVO5) == false)
			{
				qDebug("ERROR sending to serial port (Servo)");
				return;
			}
			// the position
			if (interface1->sendChar(position) == false)
			{
				qDebug("ERROR sending to serial port (Servo)");
				return;
			}
			break;
		case SERVO6:
			// which servo
			if (interface1->sendChar(SET_SERVO6) == false)
			{
				qDebug("ERROR sending to serial port (Servo)");
				return;
			}
			// the position
			if (interface1->sendChar(position) == false)
			{
				qDebug("ERROR sending to serial port (Servo)");
				return;
			}
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
	}
}


unsigned char Servo::getServoPosition(int servo)
{
	if ( (servo < SERVO1) || (servo > (NUMBEROFSERVOS-1)) )
	{
		emit message(QString("<b><font color=\"#FF0000\">Servo%1 out of allowed range! (getServoPosition)</font>").arg(servo+1));
		return 0;
	}
	
	return servoPosition[servo];
}
