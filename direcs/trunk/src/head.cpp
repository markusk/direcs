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
	
	if (direction == "FORWARD")
	{
		eye(LEFTEYE, direction);
		eye(RIGHTEYE, direction);
		return;
	}
	
	if (direction == "NORMAL")
	{
		eyebrow(LEFTBROW, direction);
		eyebrow(RIGHTBROW, direction);
		return;
	}
	
	if (direction == "CURIOUS")
	{
		eyebrow(LEFTBROW, "UP");
		eyebrow(RIGHTBROW, "UP");
		return;
	}
	
	if (direction == "ANGRY")
	{
		eyebrow(LEFTBROW, "DOWN");
		eyebrow(RIGHTBROW, "DOWN");
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
			// default
			servos->moveServo( SERVO1, servos->getServoPosition(SERVO1, SVDEFAULT) );
			return;
		}

		if (whichEye==RIGHTEYE)
		{
			// up
			servos->moveServo( SERVO5, servos->getServoPosition(SERVO5, SVSTART) );
			// default
			servos->moveServo( SERVO4, servos->getServoPosition(SERVO4, SVDEFAULT) );
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
			// default
			servos->moveServo( SERVO1, servos->getServoPosition(SERVO1, SVDEFAULT) );
			return;
		}

		if (whichEye==RIGHTEYE)
		{
			// down
			servos->moveServo( SERVO5, servos->getServoPosition(SERVO5, SVEND) );
			// default
			servos->moveServo( SERVO4, servos->getServoPosition(SERVO4, SVDEFAULT) );
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
			// default
			servos->moveServo( SERVO2, servos->getServoPosition(SERVO2, SVDEFAULT) );
			return;
		}

		if (whichEye==RIGHTEYE)
		{
			// left
			servos->moveServo( SERVO1, servos->getServoPosition(SERVO1, SVSTART) );
			// default
			servos->moveServo( SERVO5, servos->getServoPosition(SERVO5, SVDEFAULT) );
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
		//qDebug("right");
		if (whichEye==LEFTEYE)
		{
			// right
			servos->moveServo( SERVO4, servos->getServoPosition(SERVO4, SVEND) );
			// default
			servos->moveServo( SERVO2, servos->getServoPosition(SERVO2, SVDEFAULT) );
			return;
		}

		if (whichEye==RIGHTEYE)
		{
			// right
			servos->moveServo( SERVO1, servos->getServoPosition(SERVO1, SVEND) );
			// default
			servos->moveServo( SERVO5, servos->getServoPosition(SERVO5, SVDEFAULT) );
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
	
	if (direction == "FORWARD")
	{
		servos->moveServo( SERVO1, servos->getServoPosition(SERVO1, SVDEFAULT) );
		servos->moveServo( SERVO2, servos->getServoPosition(SERVO2, SVDEFAULT) );
		servos->moveServo( SERVO4, servos->getServoPosition(SERVO4, SVDEFAULT) );
		servos->moveServo( SERVO5, servos->getServoPosition(SERVO5, SVDEFAULT) );
		return;
	}
}


void Head::eyebrow(unsigned char whichBrow, QString direction)
{
	if (direction == "NORMAL")
	{
		if (whichBrow==LEFTBROW)
		{
			// default pos
			servos->moveServo( SERVO3, servos->getServoPosition(SERVO3, SVDEFAULT) );
			return;
		}

		if (whichBrow==RIGHTBROW)
		{
			// default pos
			servos->moveServo( SERVO6, servos->getServoPosition(SERVO6, SVDEFAULT) );
			return;
		}
		return;
	}

	if (direction == "UP")
	{
		if (whichBrow==LEFTBROW)
		{
			// inside up
			servos->moveServo( SERVO3, servos->getServoPosition(SERVO3, SVEND) );
			return;
		}

		if (whichBrow==RIGHTBROW)
		{
			// inside up
			servos->moveServo( SERVO6, servos->getServoPosition(SERVO6, SVSTART) );
			return;
		}
		return;
	}
	
	if (direction == "DOWN")
	{
		if (whichBrow==LEFTBROW)
		{
			// inside down
			servos->moveServo( SERVO3, servos->getServoPosition(SERVO3, SVSTART) );
			return;
		}

		if (whichBrow==RIGHTBROW)
		{
			// inside down
			servos->moveServo( SERVO6, servos->getServoPosition(SERVO6, SVEND) );
			return;
		}
		return;
	}
}
