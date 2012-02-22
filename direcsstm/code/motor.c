#include "main.h"


int motorControl(int motor, int power, int direction)
{
	char command[64] = "bst"; // stop robot


	switch (motor)
	{
		//----------------------------------------------------------------------------------
		// ALLMOTORS is for sending only -one- command for all motors over the serial line!
		//----------------------------------------------------------------------------------
		case ALLMOTORS:
			// convert the given direction into the corresponding serial command for the avr code
			switch (direction)
			{
				case FORWARD:
					// bot drive forward
					break;
				case BACKWARD:
					// bot drive backward
					break;
				case LEFT:
					// bot drive left
					break;
				case RIGHT:
					// bot drive right
					break;
				case TURNLEFT:
					// bot turn left
					break;
				case TURNRIGHT:
					// bot turn right
					break;
				case START:
					// bot "go"
					break;
				case STOP:
					// bot stop
					// turn all drive motor bits off (except PWM bits)
					GPIO_ResetBits(MOTORPORT, MOTOR1BITA | MOTOR1BITB | MOTOR2BITA | MOTOR2BITB | MOTOR3BITA | MOTOR3BITB | MOTOR4BITA | MOTOR4BITB);
					return 0;
					break;
				case WAIT:
					// bot wait
					// turn all drive motor bits off (except PWM bits)
					GPIO_ResetBits(MOTORPORT, MOTOR1BITA | MOTOR1BITB | MOTOR2BITA | MOTOR2BITB | MOTOR3BITA | MOTOR3BITB | MOTOR4BITA | MOTOR4BITB);
					return 0;
					break;
			}

			// send command to bot
// @todo 
			return 0;
			break;

		//-------------------------
		// Motor 1
		//-------------------------
		case MOTOR1:
			if (power == ON)
			{
				// Motors with gearings don't have to be turned on!
				// (This part is obsolete because of no longer using stepping motors)
			}
			else
			{
				// turn off the MOTOR -> break!
				return 0;
			}

			if (direction == FORWARD)
			{
				return 0;
			}

			if (direction == BACKWARD)
			{
				return 0;
			}

			if (direction == SAME)
			{
				// don't change the direction (motor was only turned on or off)!
			}

			return 0;
			break;


		//-------------------------
		// Motor 2
		//-------------------------
		case MOTOR2:
			if (power == ON)
			{
				// Motors with gearings don't have to be turned on!
				// (This part is obsolete because of no longer using stepping motors)
			}
			else
			{
				// turn off the MOTOR -> break!
				return 0;
			}

			if (direction == FORWARD)
			{
				return 0;
			}

			if (direction == BACKWARD)
			{
				return 0;
			}

			if (direction == SAME)
			{
				// don't change the direction (motor was only turned on or off)!
			}

			return 0;
			break;


		//-------------------------
		// Motor 3
		//------------------------
		case MOTOR3:
			if (power == ON)
			{
				// Motors with gearings don't have to be turned on!
				// (This part is obsolete because of no longer using stepping motors)
			}
			else
			{
				// turn off the MOTOR -> break!
				return 0;
			}

			if (direction == FORWARD)
			{
				return 0;
			}

			if (direction == BACKWARD)
			{
				return 0;
			}

			if (direction == SAME)
			{
				// don't change the direction (motor was only turned on or off)!
			}

			return 0;
			break;


		//-------------------------
		// Motor 4
		//------------------------
		case MOTOR4:
			if (power == ON)
			{
				// Motors with gearings don't have to be turned on!
				// (This part is obsolete because of no longer using stepping motors)
			}
			else
			{
				// turn off the MOTOR -> break!
				return 0;
			}

			if (direction == FORWARD)
			{
				return 0;
			}

			if (direction == BACKWARD)
			{
				return 0;
			}

			if (direction == SAME)
			{
				// don't change the direction (motor was only turned on or off)!
			}

			return 0;
			break;
	}

	return -1;
}
