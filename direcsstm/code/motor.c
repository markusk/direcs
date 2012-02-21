int motorControl(int motor, int power, int direction)
{
	char command[64] = "bst"; // stop robot


	if (robotState == ON)
	{
		// Lock the mutex. If another thread has locked the mutex then this call will block until that thread has unlocked it.
		mutex->lock();

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
								// Unlock the mutex
								mutex->unlock();
								return 0;
							}
						}
					}
					//qDebug("ERROR sending to serial port (MotorControl)");
					// Unlocks the mutex
					mutex->unlock();
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
								// Unlock the mutex
								mutex->unlock();
								return 0;
							}
						}
					}
					//qDebug("ERROR sending to serial port (MotorControl)");
					// Unlocks the mutex
					mutex->unlock();
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
								// Unlock the mutex
								mutex->unlock();
								return 0;
							}
						}
					}
					//qDebug("ERROR sending to serial port (MotorControl)");
					// Unlocks the mutex
					mutex->unlock();
					return -1;
				}

				if (direction == SAME)
				{
					// don't change the direction (motor was only turned on or off)!
				}

				// Unlocks the mutex
				mutex->unlock();
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
								// Unlock the mutex
								mutex->unlock();
								return 0;
							}
						}
					}
					//qDebug("ERROR sending to serial port (MotorControl)");
					// Unlocks the mutex
					mutex->unlock();
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
								// Unlock the mutex
								mutex->unlock();
								return 0;
							}
						}
					}
					//qDebug("ERROR sending to serial port (MotorControl)");
					// Unlocks the mutex
					mutex->unlock();
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
								// Unlock the mutex
								mutex->unlock();
								return 0;
							}
						}
					}
					//qDebug("ERROR sending to serial port (MotorControl)");
					// Unlocks the mutex
					mutex->unlock();
					return -1;
				}

				if (direction == SAME)
				{
					// don't change the direction (motor was only turned on or off)!
				}

				// Unlocks the mutex
				mutex->unlock();
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
								// Unlock the mutex
								mutex->unlock();
								return 0;
							}
						}
					}
					//qDebug("ERROR sending to serial port (MotorControl)");
					// Unlocks the mutex
					mutex->unlock();
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
								// Unlock the mutex
								mutex->unlock();
								return 0;
							}
						}
					}
					//qDebug("ERROR sending to serial port (MotorControl)");
					// Unlocks the mutex
					mutex->unlock();
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
								// Unlock the mutex
								mutex->unlock();
								return 0;
							}
						}
					}
					//qDebug("ERROR sending to serial port (MotorControl)");
					// Unlocks the mutex
					mutex->unlock();
					return -1;
				}

				if (direction == SAME)
				{
					// don't change the direction (motor was only turned on or off)!
				}

				// Unlocks the mutex
				mutex->unlock();
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
								// Unlock the mutex
								mutex->unlock();
								return 0;
							}
						}
					}
					//qDebug("ERROR sending to serial port (MotorControl)");
					// Unlocks the mutex
					mutex->unlock();
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
								// Unlock the mutex
								mutex->unlock();
								return 0;
							}
						}
					}
					//qDebug("ERROR sending to serial port (MotorControl)");
					// Unlocks the mutex
					mutex->unlock();
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
								// Unlock the mutex
								mutex->unlock();
								return 0;
							}
						}
					}
					//qDebug("ERROR sending to serial port (MotorControl)");
					// Unlocks the mutex
					mutex->unlock();
					return -1;
				}

				if (direction == SAME)
				{
					// don't change the direction (motor was only turned on or off)!
				}

				// Unlocks the mutex
				mutex->unlock();
				break;


	/*
			//-------------------------
			// stepper 1
			//------------------------
			case STEPPER1:
				if (power == ON)
				{
					// turn on stepper 1
					if (interface1->sendString(STEPPER1_ON) == true)        // FIXME: use sendString with a new nice string!
					{
						// check if the robot answers with the sent command
						if ( interface1->receiveString(answer) == true)
						{
							if (answer == " fix this ")
							{
								// Unlock the mutex
								mutex->unlock();
								return 0;
							}
						}
					}
					//qDebug("ERROR sending to serial port (MotorControl)");
					// Unlocks the mutex
					mutex->unlock();
					return -1;
				}
				else
				{
					// turn off stepper 1
					if (interface1->sendString(STEPPER1_OFF) == true)        // FIXME: use sendString with a new nice string!
					{
						// check if the robot answers with the sent command
						if ( interface1->receiveString(answer) == true)
						{
							if (answer == " fix this ")
							{
								// Unlock the mutex
								mutex->unlock();
								return 0;
							}
						}
					}
					//qDebug("ERROR sending to serial port (MotorControl)");
					// Unlocks the mutex
					mutex->unlock();
					return -1;
				}

				if (direction == CLOCKWISE)
				{
					// set the direction
					if (interface1->sendString(STEPPER1_CLOCKWISE) == true)        // FIXME: use sendString with a new nice string!
					{
						// check if the robot answers with the sent command
						if ( interface1->receiveString(answer) == true)
						{
							if (answer == " fix this ")
							{
								// Unlock the mutex
								mutex->unlock();
								return 0;
							}
						}
					}
					//qDebug("ERROR sending to serial port (MotorControl)");
					// Unlocks the mutex
					mutex->unlock();
					return -1;
				}

				if (direction == COUNTERCLOCKWISE)
				{
					// set the direction
					if (interface1->sendString(STEPPER1_COUNTERCLOCKWISE) == true)        // FIXME: use sendString with a new nice string!
					{
						// check if the robot answers with the sent command
						if ( interface1->receiveString(answer) == true)
						{
							if (answer == " fix this ")
							{
								// Unlock the mutex
								mutex->unlock();
								return 0;
							}
						}
					}
					//qDebug("ERROR sending to serial port (MotorControl)");
					// Unlocks the mutex
					mutex->unlock();
					return -1;
				}

				if (direction == SAME)
				{
					// don't change the direction (motor was only turned on or off)!
				}

				// Unlocks the mutex. Attempting to unlock a mutex in a different thread to the one that locked it results in an error.
				mutex->unlock();

				break;


			//-------------------------
			// stepper 2
			//------------------------
			case STEPPER2:
				if (power == ON)
				{
					// turn on stepper 2
					if (interface1->sendString(STEPPER2_ON) == true)        // FIXME: use sendString with a new nice string!
					{
						// check if the robot answers with the sent command
						if ( interface1->receiveString(answer) == true)
						{
							if (answer == " fix this ")
							{
								// Unlock the mutex
								mutex->unlock();
								return 0;
							}
						}
					}
					//qDebug("ERROR sending to serial port (MotorControl)");
					// Unlocks the mutex
					mutex->unlock();
					return -1;
				}
				else
				{
					// turn off stepper 2
					if (interface1->sendString(STEPPER2_OFF) == true)        // FIXME: use sendString with a new nice string!
					{
						// check if the robot answers with the sent command
						if ( interface1->receiveString(answer) == true)
						{
							if (answer == " fix this ")
							{
								// Unlock the mutex
								mutex->unlock();
								return 0;
							}
						}
					}
					//qDebug("ERROR sending to serial port (MotorControl)");
					// Unlocks the mutex
					mutex->unlock();
					return -1;
				}

				if (power == CLOCK)
				{
					// make a stepper with every enabled motor
					if (interface1->sendString(STEPPER_CLOCK) == true)        // FIXME: use sendString with a new nice string!
					{
						// check if the robot answers with the sent command
						if ( interface1->receiveString(answer) == true)
						{
							if (answer == " fix this ")
							{
								// Unlock the mutex
								mutex->unlock();
								return 0;
							}
						}
					}
					//qDebug("ERROR sending to serial port (MotorControl)");
					// Unlocks the mutex
					mutex->unlock();
					return -1;
				}

				if (direction == CLOCKWISE)
				{
					// set the direction
					if (interface1->sendString(STEPPER2_CLOCKWISE) == true)        // FIXME: use sendString with a new nice string!
					{
						// check if the robot answers with the sent command
						if ( interface1->receiveString(answer) == true)
						{
							if (answer == " fix this ")
							{
								// Unlock the mutex
								mutex->unlock();
								return 0;
							}
						}
					}
					//qDebug("ERROR sending to serial port (MotorControl)");
					// Unlocks the mutex
					mutex->unlock();
					return -1;
				}

				if (direction == COUNTERCLOCKWISE)
				{
					// set the direction
					if (interface1->sendString(STEPPER2_COUNTERCLOCKWISE) == true)        // FIXME: use sendString with a new nice string!
					{
						// check if the robot answers with the sent command
						if ( interface1->receiveString(answer) == true)
						{
							if (answer == " fix this ")
							{
								// Unlock the mutex
								mutex->unlock();
								return 0;
							}
						}
					}
					//qDebug("ERROR sending to serial port (MotorControl)");
					// Unlocks the mutex
					mutex->unlock();
					return -1;
				}

				if (direction == SAME)
				{
					// don't change the direction (motor was only turned on or off)!
				}

				// Unlocks the mutex. Attempting to unlock a mutex in a different thread to the one that locked it results in an error.
				mutex->unlock();

				break;
	*/
		}

		// Unlocks the mutex, if no case statement fits in the switch command
		mutex->unlock();
	} // robot is ON


	return -1;
}
