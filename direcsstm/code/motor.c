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
					// bot drive forward = all motors clockwise
					GPIO_ResetBits(MOTORPORT, MOTOR1BITA);
					GPIO_SetBits(MOTORPORT, MOTOR1BITB);
					GPIO_ResetBits(MOTORPORT, MOTOR2BITA);
					GPIO_SetBits(MOTORPORT, MOTOR2BITB);
					GPIO_ResetBits(MOTORPORT, MOTOR3BITA);
					GPIO_SetBits(MOTORPORT, MOTOR3BITB);
					GPIO_ResetBits(MOTORPORT, MOTOR4BITA);
					GPIO_SetBits(MOTORPORT, MOTOR4BITB);
					return;
					break;
				case BACKWARD:
					// bot drive backward = all motors counterclockwise
					GPIO_SetBits(MOTORPORT, MOTOR1BITA);
					GPIO_ResetBits(MOTORPORT, MOTOR1BITB);
					GPIO_SetBits(MOTORPORT, MOTOR2BITA);
					GPIO_ResetBits(MOTORPORT, MOTOR2BITB);
					GPIO_SetBits(MOTORPORT, MOTOR3BITA);
					GPIO_ResetBits(MOTORPORT, MOTOR3BITB);
					GPIO_SetBits(MOTORPORT, MOTOR4BITA);
					GPIO_ResetBits(MOTORPORT, MOTOR4BITB);
					return;
					break;
				case LEFT:
					// bot drive left
					// MOTOR 1 COUNTERCLOCKWISE = backward
					GPIO_SetBits(MOTORPORT, MOTOR1BITA);
					GPIO_ResetBits(MOTORPORT, MOTOR1BITB);
					// MOTOR 2 CLOCKWISE = forward
					GPIO_ResetBits(MOTORPORT, MOTOR2BITA);
					GPIO_SetBits(MOTORPORT, MOTOR2BITB);
					// MOTOR 3 CLOCKWISE = forward
					GPIO_ResetBits(MOTORPORT, MOTOR3BITA);
					GPIO_SetBits(MOTORPORT, MOTOR3BITB);
					// MOTOR 4 COUNTERCLOCKWISE = backward
					GPIO_SetBits(MOTORPORT, MOTOR4BITA);
					GPIO_ResetBits(MOTORPORT, MOTOR4BITB);
					return;
					break;
				case RIGHT:
					// bot drive right
					// MOTOR 1 CLOCKWISE = forward
					GPIO_ResetBits(MOTORPORT, MOTOR1BITA);
					GPIO_SetBits(MOTORPORT, MOTOR1BITB);
					// MOTOR 2 COUNTERCLOCKWISE = backward
					GPIO_SetBits(MOTORPORT, MOTOR2BITA);
					GPIO_ResetBits(MOTORPORT, MOTOR2BITB);
					// MOTOR 3 COUNTERCLOCKWISE = backward
					GPIO_SetBits(MOTORPORT, MOTOR3BITA);
					GPIO_ResetBits(MOTORPORT, MOTOR3BITB);
					// MOTOR 4 CLOCKWISE = forward
					GPIO_ResetBits(MOTORPORT, MOTOR4BITA);
					GPIO_SetBits(MOTORPORT, MOTOR4BITB);
					return;
					break;
				case TURNLEFT:
					// bot turn left
					// MOTOR 1 CLOCKWISE = forward
					GPIO_ResetBits(MOTORPORT, MOTOR1BITA);
					GPIO_SetBits(MOTORPORT, MOTOR1BITB);
					// MOTOR 2 COUNTERCLOCKWISE = backward
					GPIO_SetBits(MOTORPORT, MOTOR2BITA);
					GPIO_ResetBits(MOTORPORT, MOTOR2BITB);
					// MOTOR 3 CLOCKWISE = forward
					GPIO_ResetBits(MOTORPORT, MOTOR3BITA);
					GPIO_SetBits(MOTORPORT, MOTOR3BITB);
					// MOTOR 4 COUNTERCLOCKWISE = backward
					GPIO_SetBits(MOTORPORT, MOTOR4BITA);
					GPIO_ResetBits(MOTORPORT, MOTOR4BITB);
					return;
					break;
				case TURNRIGHT:
					// bot turn right
					// MOTOR 1 COUNTERCLOCKWISE = backward
					GPIO_SetBits(MOTORPORT, MOTOR1BITA);
					GPIO_ResetBits(MOTORPORT, MOTOR1BITB);
					// MOTOR 2 CLOCKWISE = forward
					GPIO_ResetBits(MOTORPORT, MOTOR2BITA);
					GPIO_SetBits(MOTORPORT, MOTOR2BITB);
					// MOTOR 3 COUNTERCLOCKWISE = backward
					GPIO_SetBits(MOTORPORT, MOTOR3BITA);
					GPIO_ResetBits(MOTORPORT, MOTOR3BITB);
					// MOTOR 4 CLOCKWISE = forward
					GPIO_ResetBits(MOTORPORT, MOTOR4BITA);
					GPIO_SetBits(MOTORPORT, MOTOR4BITB);
					return;
					break;
				case START:
					// bot "go"
					// bot drive forward = all motors clockwise
					GPIO_ResetBits(MOTORPORT, MOTOR1BITA);
					GPIO_SetBits(MOTORPORT, MOTOR1BITB);
					GPIO_ResetBits(MOTORPORT, MOTOR2BITA);
					GPIO_SetBits(MOTORPORT, MOTOR2BITB);
					GPIO_ResetBits(MOTORPORT, MOTOR3BITA);
					GPIO_SetBits(MOTORPORT, MOTOR3BITB);
					GPIO_ResetBits(MOTORPORT, MOTOR4BITA);
					GPIO_SetBits(MOTORPORT, MOTOR4BITB);
					return;
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
