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


void Head::look(QString direction)
{
	if (direction == "UP")
	{
		
		eye(LEFTEYE, direction);
		eye(RIGHTEYE, direction);
		return;
	}
	
	if (direction == "UPLEFT")
	{
		eye(LEFTEYE, direction);
		eye(RIGHTEYE, direction);
		return;
	}
	
	if (direction == "UPRIGHT")
	{
		eye(LEFTEYE, direction);
		eye(RIGHTEYE, direction);
		return;
	}
		
	if (direction == "DOWN")
	{
		eye(LEFTEYE, direction);
		eye(RIGHTEYE, direction);
		return;
	}
	
	if (direction == "DOWNLEFT")
	{
		eye(LEFTEYE, direction);
		eye(RIGHTEYE, direction);
		return;
	}
	
	if (direction == "DOWNRIGHT")
	{
		eye(LEFTEYE, direction);
		eye(RIGHTEYE, direction);
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
		eye(LEFTEYE, direction);
		eye(RIGHTEYE, direction);
		return;
	}
}


void Head::eye(unsigned char whichEye, QString direction)
{
	// move servo to the start or end position (*not* to the min or max posisiotns!)
	if (direction == "UP")
	{
		if (whichEye==LEFTEYE)
		{
			// up
			servos->moveServo( SERVO2, servos->getServoPosition(SERVO2, SVEND) );
			return;
		}

		if (whichEye==RIGHTEYE)
		{
			// up
			servos->moveServo( SERVO5, servos->getServoPosition(SERVO5, SVSTART) );
			return;
		}
		return;
	}

	if (direction == "UPLEFT")
	{
		if (whichEye==LEFTEYE)
		{
			// up
			servos->moveServo( SERVO2, servos->getServoPosition(SERVO2, SVEND) );
			//left
			servos->moveServo( SERVO4, servos->getServoPosition(SERVO4, SVSTART) );
			return;
		}

		if (whichEye==RIGHTEYE)
		{
			// up
			servos->moveServo( SERVO5, servos->getServoPosition(SERVO5, SVSTART) );
			// left
			servos->moveServo( SERVO1, servos->getServoPosition(SERVO1, SVSTART) );
			return;
		}
		return;
	}
	
	if (direction == "UPRIGHT")
	{
		if (whichEye==LEFTEYE)
		{
			// right
			servos->moveServo( SERVO4, servos->getServoPosition(SERVO4, SVEND) );
			// up
			servos->moveServo( SERVO2, servos->getServoPosition(SERVO2, SVEND) );
			return;
		}

		if (whichEye==RIGHTEYE)
		{
			// up
			servos->moveServo( SERVO5, servos->getServoPosition(SERVO5, SVSTART) );
			// right
			servos->moveServo( SERVO1, servos->getServoPosition(SERVO1, SVEND) );
			return;
		}
		return;
	}
		
	// move servo to the start or end position (*not* to the min or max posisiotns!)
	if (direction == "DOWN")
	{
		if (whichEye==LEFTEYE)
		{
			// down
			servos->moveServo( SERVO2, servos->getServoPosition(SERVO2, SVSTART) );
			return;
		}

		if (whichEye==RIGHTEYE)
		{
			// down
			servos->moveServo( SERVO5, servos->getServoPosition(SERVO5, SVEND) );
			return;
		}
		return;
	}
	
	if (direction == "DOWNLEFT")
	{
		if (whichEye==LEFTEYE)
		{
			// down
			servos->moveServo( SERVO2, servos->getServoPosition(SERVO2, SVSTART) );
			// left
			servos->moveServo( SERVO4, servos->getServoPosition(SERVO4, SVSTART) );
			return;
		}

		if (whichEye==RIGHTEYE)
		{
			// down
			servos->moveServo( SERVO5, servos->getServoPosition(SERVO5, SVEND) );
			// left
			servos->moveServo( SERVO1, servos->getServoPosition(SERVO1, SVSTART) );
			return;
		}
		return;
	}
	
	if (direction == "DOWNRIGHT")
	{
		if (whichEye==LEFTEYE)
		{
			// down
			servos->moveServo( SERVO2, servos->getServoPosition(SERVO2, SVSTART) );
			// right
			servos->moveServo( SERVO4, servos->getServoPosition(SERVO4, SVEND) );
			return;
		}

		if (whichEye==RIGHTEYE)
		{
			// right
			servos->moveServo( SERVO1, servos->getServoPosition(SERVO1, SVEND) );
			// down
			servos->moveServo( SERVO5, servos->getServoPosition(SERVO5, SVEND) );
			return;
		}
		return;
	}
	
	// move servo to the start or end position (*not* to the min or max posisiotns!)
	if (direction == "LEFT")
	{
		if (whichEye==LEFTEYE)
		{
			// left
			servos->moveServo( SERVO4, servos->getServoPosition(SERVO4, SVSTART) );
			return;
		}

		if (whichEye==RIGHTEYE)
		{
			// left
			servos->moveServo( SERVO1, servos->getServoPosition(SERVO1, SVSTART) );
			return;
		}
		return;
	}
	
	// move servo to its maximum end position
	if (direction == "LEFTMAX")
	{
		if (whichEye==LEFTEYE)
		{
			// 
			servos->moveServo( SERVO4, servos->getServoPosition(SERVO4, SVMIN) );
			return;
		}

		if (whichEye==RIGHTEYE)
		{
			// 
			servos->moveServo( SERVO1, servos->getServoPosition(SERVO1, SVMIN) );
			return;
		}
		return;
	}
	
	// move servo to the start or end position (*not* to the min or max posisiotns!)
	if (direction == "RIGHT")
	{
		qDebug("right");
		if (whichEye==LEFTEYE)
		{
			// right
			servos->moveServo( SERVO4, servos->getServoPosition(SERVO4, SVEND) );
			qDebug("left eye right");
			return;
		}

		if (whichEye==RIGHTEYE)
		{
			// right
			servos->moveServo( SERVO1, servos->getServoPosition(SERVO1, SVEND) );
			qDebug("right eye right");
			return;
		}
		return;
	}
	
	// move servo to its maximum end position
	if (direction == "RIGHTMAX")
	{
		if (whichEye==LEFTEYE)
		{
			servos->moveServo( SERVO4, servos->getServoPosition(SERVO4, SVMAX) );
			return;
		}

		if (whichEye==RIGHTEYE)
		{
			servos->moveServo( SERVO1, servos->getServoPosition(SERVO1, SVMAX) );
			return;
		}
		return;
	}
	
	if (direction == "NONE")
	{
		servos->init();
		return;
	}
}
