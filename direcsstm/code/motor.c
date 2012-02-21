#define FORWARD 	10 /// Motor direction (formerly "clockwise")
#define BACKWARD 	20 /// Motor direction (formerly "counterclockwise"
#define LEFT		30
#define RIGHT		40
#define TURNLEFT	50
#define TURNRIGHT	60
#define START		70
#define STOP		80
#define WAIT		90
// #define DIAGONAL_FORWARD_LEFT
// #define DIAGONAL_FORWARD_RIGHT
// #define DIAGONAL_BACKWARD_LEFT
// #define DIAGONAL_BACKWARD_RIGHT

#define SAME 		200 /// Motor direction/power "same like before"
#define MOTOR1    	210 /// Motor 1 front left
#define MOTOR2    	220 /// Motor 2 front right
#define MOTOR3    	230 /// Motor 3 back left
#define MOTOR4    	240 /// Motor 4 back right
#define ALLMOTORS 	250 /// used for letting the robot getting only one command for all motors (like 'forward all').

#define ON			1
#define OFF			0


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
					command = "bdf";
					break;
				case BACKWARD:
					// bot drive backward
					command = "bdb";
					break;
				case LEFT:
					// bot drive left
					command = "bdl";
					break;
				case RIGHT:
					// bot drive right
					command = "bdr";
					break;
				case TURNLEFT:
					// bot turn left
					command = "btl";
					break;
				case TURNRIGHT:
					// bot turn right
					command = "btr";
					break;
				case START:
					// bot "go"
					command = "bgo";
					break;
				case STOP:
					// bot stop
					command = "bst";
					break;
				case WAIT:
					// bot wait
					command = "bwa";
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
