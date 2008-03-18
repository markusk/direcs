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
	
	if (direction == "LEFT")
	{
		if (whichEye==LEFTEYE)
		{
			servos->moveServo(SERVO4, servos->getServoPosition(SERVO4, SVSTART));
		}

		if (whichEye==RIGHTEYE)
		{
			servos->moveServo(SERVO1, servos->getServoPosition(SERVO1, SVSTART));
		}
		return;
	}
	
	if (direction == "RIGHT")
	{
		if (whichEye==LEFTEYE)
		{
			servos->moveServo(SERVO4, servos->getServoPosition(SERVO4, SVEND));
		}

		if (whichEye==RIGHTEYE)
		{
			servos->moveServo(SERVO1, servos->getServoPosition(SERVO1, SVEND));
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


void Head::resetMovementCounter(short int motor)
{
	switch (motor)
	{
		case MOTOR1:
		{
			// Initialize the motor step counters
			//steps1 = 0;
		 
			// Initialize the motor revolutions counters
			revolutions1 = 0;
		 
			// Initialize the distance to next objects in front of the robot
			drivenDistance1 = 0;
			return;
			break;
		}
		case MOTOR2:
		{
			// Initialize the motor step counters
			//steps2 = 0;
		 
			// Initialize the motor revolutions counters
			revolutions2 = 0;
		 
			// Initialize the distance to next objects in front of the robot
			drivenDistance2 = 0;
			return;
			break;
		}
	}
}


void Head::setMotorSpeed(int motor, int speed)
{
	if (speed < 0)
	{
		speed = 0;
	}
	
	if (speed > 255)
	{
		speed = 255;
	}
	
	switch(motor)
	{
		case 1:
			// store the speed
			motor1Speed = speed;
			// send the command to the microcontroller (MOTOR 1)
			break;
			
		case 2:
			// store the speed
			motor2Speed = speed;
			// send the command to the microcontroller (MOTOR 2)
			break;
	
		case 3:
			// store the speed
			motor3Speed = speed;
			// send the command to the microcontroller (MOTOR 3)
			break;
			
		case 4:
			// store the speed
			motor4Speed = speed;
			// send the command to the microcontroller (MOTOR 4)
			break;
	} // switch
}


int Head::getMotorSpeed(int motor)
{
	switch (motor)
	{
		case 1:
			return motor1Speed;
			break;
		case 2:
			return motor2Speed;
			break;
	}
	
	return -1;
}
