#include "head.h"

Head::Head(Servo *s)
{
	//------------------------------------------------------------------
	// copy the pointer from the original usb object
	//------------------------------------------------------------------
	servos = s;

	// Initialize something
}


Head::~Head()
{
	/*
	park();
	*/
}


void Head::eye(unsigned char whichEye, QString direction)
{
	if (direction == "UP")
	{
		return;
	}

	if (direction == "UPLEFT")
	{
		return;
	}
	
	if (direction == "UPRIGHT")
	{
		return;
	}
		
	if (direction == "DOWN")
	{
		return;
	}
	
	if (direction == "DOWNLEFT")
	{
		return;
	}
	
	if (direction == "DOWNRIGHT")
	{
		return;
	}
	
	// move servo in the middle of "middle" - "start/end position"
	if (direction == "LEFT")
	{
		if (whichEye==LEFTEYE)
		{
			int value = (((servos->getServoPosition(SERVO4, SVDEFAULT) - servos->getServoPosition(SERVO4, SVSTART)) / 2) + servos->getServoPosition(SERVO4, SVDEFAULT)); 
			//qDebug("Left eye, servo4 left=%d", value);
			servos->moveServo( SERVO4, value );
			return;
		}

		if (whichEye==RIGHTEYE)
		{
			int value = (((servos->getServoPosition(SERVO1, SVDEFAULT) - servos->getServoPosition(SERVO1, SVSTART)) / 2) + servos->getServoPosition(SERVO1, SVDEFAULT));
			//qDebug("Right eye, servo4 left=%d", value);
			servos->moveServo( SERVO1, value );
			return;
		}
	}
	
	// move servo to its maximum end position
	if (direction == "LEFTMAX")
	{
		if (whichEye==LEFTEYE)
		{
			servos->moveServo( SERVO4, servos->getServoPosition(SERVO4, SVSTART) );
		}

		if (whichEye==RIGHTEYE)
		{
			servos->moveServo( SERVO1, servos->getServoPosition(SERVO1, SVSTART) );
		}
		return;
	}
	
	// move servo in the middle of "middle" - "start/end position"
	if (direction == "RIGHT")
	{
		if (whichEye==LEFTEYE)
		{
			int value = (servos->getServoPosition(SERVO4, SVEND) - ((servos->getServoPosition(SERVO4, SVEND) - servos->getServoPosition(SERVO4, SVDEFAULT)) / 2) );
			//qDebug("Left eye, servo4 right=%d", value);
			servos->moveServo( SERVO4, value);
		}

		if (whichEye==RIGHTEYE)
		{
			int value = (servos->getServoPosition(SERVO1, SVEND) - ((servos->getServoPosition(SERVO1, SVEND) - servos->getServoPosition(SERVO1, SVDEFAULT)) / 2) ); 
			//qDebug("Right eye, servo4 right=%d", value);
			servos->moveServo( SERVO1, value);
		}
		return;
	}
	
	// move servo to its maximum end position
	if (direction == "RIGHTMAX")
	{
		if (whichEye==LEFTEYE)
		{
			servos->moveServo( SERVO4, servos->getServoPosition(SERVO4, SVEND) );
		}

		if (whichEye==RIGHTEYE)
		{
			servos->moveServo( SERVO1, servos->getServoPosition(SERVO1, SVEND) );
		}
		return;
	}
	
	if (direction == "NONE")
	{
		return;
	}
}


void Head::look(QString direction)
{
	if (direction == "UP")
	{
		
		return;
	}
	
	if (direction == "UPLEFT")
	{
		return;
	}
	
	if (direction == "UPRIGHT")
	{
		return;
	}
		
	if (direction == "DOWN")
	{
		return;
	}
	
	if (direction == "DOWNLEFT")
	{
		return;
	}
	
	if (direction == "DOWNRIGHT")
	{
		return;
	}
	
	if (direction == "LEFT")
	{
		eye(LEFTEYE, direction);
		eye(RIGHTEYE, direction);
		return;
	}
	
	if (direction == "RIGHT")
	{
		eye(LEFTEYE, direction);
		eye(RIGHTEYE, direction);
		return;
	}
	
	if (direction == "NONE")
	{
		return;
	}
}
