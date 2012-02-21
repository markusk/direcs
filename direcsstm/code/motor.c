int motorControl(int motor, int power, int direction)
{
	char command[64] = "bst"; // stop robot


	if (robotState == ON)
	{

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
				return;
				break;

			//-------------------------
			// Motor 1
			//-------------------------
			case MOTOR1:
				if (power == ON)
				{
					// Motors with gearings don't have to be turned on!
					//
					// (This function is obsolete because of no longer using stepping motors)
				}
				else
				{
					// turn off the MOTOR -> break!
					if (interface1->sendString("mp1of") == true)
					{
						// check if the robot answers with the sent command
						if ( interface1->receiveString(answer) == true)
						{
							if (answer == "*mp1of#")
							{
								return 0;
							}
						}
					}
					//qDebug("ERROR sending to serial port (MotorControl)");
					return -1;
				}

				if (direction == FORWARD)
				{
					// set the direction
					if (interface1->sendString("md1cw") == true)
					{
						// check if the robot answers with the sent command
						if ( interface1->receiveString(answer) == true)
						{
							if (answer == "*md1cw#")
							{
								return 0;
							}
						}
					}
					//qDebug("ERROR sending to serial port (MotorControl)");
					return -1;
				}

				if (direction == BACKWARD)
				{
					// set the direction
					if (interface1->sendString("md1cc") == true)
					{
						// check if the robot answers with the sent command
						if ( interface1->receiveString(answer) == true)
						{
							if (answer == "*md1cc#")
							{
								return 0;
							}
						}
					}
					//qDebug("ERROR sending to serial port (MotorControl)");
					return -1;
				}

				if (direction == SAME)
				{
					// don't change the direction (motor was only turned on or off)!
				}

				break;


			//-------------------------
			// Motor 2
			//-------------------------
			case MOTOR2:
				if (power == ON)
				{
					// Motors with gearings don't have to be turned on!
					//
					// (This function is obsolete because of no longer using stepping motors)
				}
				else
				{
					// turn off the MOTOR -> break!
					if (interface1->sendString("mp2of") == true)
					{
						// check if the robot answers with the sent command
						if ( interface1->receiveString(answer) == true)
						{
							if (answer == "*mp2of#")
							{
								return 0;
							}
						}
					}
					//qDebug("ERROR sending to serial port (MotorControl)");
					return -1;
				}

				if (direction == FORWARD)
				{
					// set the direction
					if (interface1->sendString("md2cw") == true)
					{
						// check if the robot answers with the sent command
						if ( interface1->receiveString(answer) == true)
						{
							if (answer == "*md2cw#")
							{
								return 0;
							}
						}
					}
					//qDebug("ERROR sending to serial port (MotorControl)");
					return -1;
				}

				if (direction == BACKWARD)
				{
					// set the direction
					if (interface1->sendString("md2cc") == true)
					{
						// check if the robot answers with the sent command
						if ( interface1->receiveString(answer) == true)
						{
							if (answer == "*md2cc#")
							{
								return 0;
							}
						}
					}
					//qDebug("ERROR sending to serial port (MotorControl)");
					return -1;
				}

				if (direction == SAME)
				{
					// don't change the direction (motor was only turned on or off)!
				}

				break;


			//-------------------------
			// Motor 3
			//------------------------
			case MOTOR3:
				if (power == ON)
				{
					// Motors with gearings don't have to be turned on!
					//
					// (This function is obsolete because of no longer using stepping motors)
				}
				else
				{
					// turn off the MOTOR -> break!
					if (interface1->sendString("mp3of") == true)
					{
						// check if the robot answers with the sent command
						if ( interface1->receiveString(answer) == true)
						{
							if (answer == "*mp3of#")
							{
								return 0;
							}
						}
					}
					//qDebug("ERROR sending to serial port (MotorControl)");
					return -1;
				}

				if (direction == FORWARD)
				{
					// set the direction
					if (interface1->sendString("md3cw") == true)
					{
						// check if the robot answers with the sent command
						if ( interface1->receiveString(answer) == true)
						{
							if (answer == "*md3cw#")
							{
								return 0;
							}
						}
					}
					//qDebug("ERROR sending to serial port (MotorControl)");
					return -1;
				}

				if (direction == BACKWARD)
				{
					// set the direction
					if (interface1->sendString("md3cc") == true)
					{
						// check if the robot answers with the sent command
						if ( interface1->receiveString(answer) == true)
						{
							if (answer == "*md3cc#")
							{
								return 0;
							}
						}
					}
					//qDebug("ERROR sending to serial port (MotorControl)");
					return -1;
				}

				if (direction == SAME)
				{
					// don't change the direction (motor was only turned on or off)!
				}

				break;


			//-------------------------
			// Motor 4
			//------------------------
			case MOTOR4:
				if (power == ON)
				{
					// Motors with gearings don't have to be turned on!
					//
					// (This function is obsolete because of no longer using stepping motors)
				}
				else
				{
					// turn off the MOTOR -> break!
					if (interface1->sendString("mp4of") == true)
					{
						// check if the robot answers with the sent command
						if ( interface1->receiveString(answer) == true)
						{
							if (answer == "*mp4of#")
							{
								return 0;
							}
						}
					}
					//qDebug("ERROR sending to serial port (MotorControl)");
					return -1;
				}

				if (direction == FORWARD)
				{
					// set the direction
					if (interface1->sendString("md4cw") == true)
					{
						// check if the robot answers with the sent command
						if ( interface1->receiveString(answer) == true)
						{
							if (answer == "*md4cw#")
							{
								return 0;
							}
						}
					}
					//qDebug("ERROR sending to serial port (MotorControl)");
					return -1;
				}

				if (direction == BACKWARD)
				{
					// set the direction
					if (interface1->sendString("md4cc") == true)
					{
						// check if the robot answers with the sent command
						if ( interface1->receiveString(answer) == true)
						{
							if (answer == "*md4cc#")
							{
								return 0;
							}
						}
					}
					//qDebug("ERROR sending to serial port (MotorControl)");
					return -1;
				}

				if (direction == SAME)
				{
					// don't change the direction (motor was only turned on or off)!
				}

				break;
		}

	} // robot is ON

	return -1;
}
