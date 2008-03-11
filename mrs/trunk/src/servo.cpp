#include "servo.h"

Servo::Servo(InterfaceAvr *i)
{
	//------------------------------------------------------------------
	// copy the pointer from the original usb object
	//------------------------------------------------------------------
	interface1 = i;

	// set initial servo position !!
	
}


Servo::~Servo()
{
}


void Servo::moveServo(unsigned char servo, unsigned char position)
{
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
		default:
			qDebug("ERROR: servo number not supportet (setServoPosition(%d) )", servo);
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
	}
}
