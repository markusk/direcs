/*************************************************************************
 *   Copyright (C) 2011 by Markus Knapp                                  *
 *   www.direcs.de                                                       *
 *                                                                       *
 *   This file is part of direcs.                                        *
 *                                                                       *
 *   direcs is free software: you can redistribute it and/or modify it   *
 *   under the terms of the GNU General Public License as published      *
 *   by the Free Software Foundation, version 3 of the License.          *
 *                                                                       *
 *   direcs is distributed in the hope that it will be useful,           *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of      *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the        *
 *   GNU General Public License for more details.                        *
 *                                                                       *
 *   You should have received a copy of the GNU General Public License   *
 *   along with direcs. If not, see <http://www.gnu.org/licenses/>.      *
 *                                                                       *
 *************************************************************************/

#include "motor.h"

Motor::Motor(InterfaceAvr *i, QMutex *m)
{
	// copy the pointer from the original object
	interface1 = i;
	mutex = m;

	// Initialize the motor step counters
	//steps1 = 0;
	//steps2 = 0;

	// Initialize the motor revolutions counters
	revolutions1 = 0;
	revolutions2 = 0;
	revolutions3 = 0;
	revolutions4 = 0;

	// Initialize the distance to next objects in front of the robot
	drivenDistance1 = 0;
	drivenDistance2 = 0;
	drivenDistance3 = 0;
	drivenDistance4 = 0;

	// Initialize the motor speed (not to fast, before reading the real values from a file!)
	motor1Speed = 20;
	motor2Speed = 20;
	motor3Speed = 20;
	motor4Speed = 20;
	motorSpeedAllMotors = 20;

	robotState = ON; // Wer're thinking positive. The robot is ON untill whe know nothing other. :-)


	circuitState = true; //  has to be TRUE at startup for the first init! Could be set to false, later if we fail to initialise the circuit.
	firstInitDone = false;
	compassCircuitState = false;

	commandExecutedSuccessfull = false;

	atmelCommand.clear();
	expectedAtmelAnswer.clear();
	answerTimeout = false;

	// the Atmel commands
	commandFlashlightOn		= "f0on";
	commandFlashlightOff	= "f0of";

	connect(interface1, SIGNAL(commandCompleted(QString)), this, SLOT(takeCommandAnswer(QString)));
}


Motor::~Motor()
{
	/*
	parkStepper(STEPPER1);
	parkStepper(STEPPER2);
	*/
}


/*
unsigned int Motor::getSteps1()
{
	return steps1;
}


unsigned int Motor::getSteps2()
{
	return steps2;
}
*/


unsigned int Motor::getRevolutions(unsigned char motor)
{
	switch (motor)
	{
		case MOTOR1:
			return revolutions1;
			break;
		case MOTOR2:
			return revolutions2;
			break;
		case MOTOR3:
			return revolutions3;
			break;
		case MOTOR4:
			return revolutions4;
			break;
	}

	return 0;
}


double Motor::getDrivenDistance(unsigned char motor)
{
	switch (motor)
	{
		case MOTOR1:
			return drivenDistance1;
			break;
		case MOTOR2:
			return drivenDistance2;
			break;
		case MOTOR3:
			return drivenDistance3;
			break;
		case MOTOR4:
			return drivenDistance4;
			break;
	}

	return 0;
}


bool Motor::motorControl(int motor, bool power, int direction)
{
	QString command = "bst"; // stop robot
	QString answer = "error";


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
				if (interface1->sendString(command) == true)
				{
					// check if the robot answers with the sent command
					if (interface1->receiveString(answer) == true)
					{
						if (answer == QString("*%1#").arg(command))
						{
							// Unlock the mutex
							mutex->unlock();
							return true;
						}
					}
				}
				//qDebug("ERROR sending to serial port (MotorControl)");
				// Unlocks the mutex
				mutex->unlock();
				return false;
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
								return true;
							}
						}
					}
					//qDebug("ERROR sending to serial port (MotorControl)");
					// Unlocks the mutex
					mutex->unlock();
					return false;
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
								return true;
							}
						}
					}
					//qDebug("ERROR sending to serial port (MotorControl)");
					// Unlocks the mutex
					mutex->unlock();
					return false;
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
								return true;
							}
						}
					}
					//qDebug("ERROR sending to serial port (MotorControl)");
					// Unlocks the mutex
					mutex->unlock();
					return false;
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
								return true;
							}
						}
					}
					//qDebug("ERROR sending to serial port (MotorControl)");
					// Unlocks the mutex
					mutex->unlock();
					return false;
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
								return true;
							}
						}
					}
					//qDebug("ERROR sending to serial port (MotorControl)");
					// Unlocks the mutex
					mutex->unlock();
					return false;
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
								return true;
							}
						}
					}
					//qDebug("ERROR sending to serial port (MotorControl)");
					// Unlocks the mutex
					mutex->unlock();
					return false;
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
								return true;
							}
						}
					}
					//qDebug("ERROR sending to serial port (MotorControl)");
					// Unlocks the mutex
					mutex->unlock();
					return false;
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
								return true;
							}
						}
					}
					//qDebug("ERROR sending to serial port (MotorControl)");
					// Unlocks the mutex
					mutex->unlock();
					return false;
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
								return true;
							}
						}
					}
					//qDebug("ERROR sending to serial port (MotorControl)");
					// Unlocks the mutex
					mutex->unlock();
					return false;
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
								return true;
							}
						}
					}
					//qDebug("ERROR sending to serial port (MotorControl)");
					// Unlocks the mutex
					mutex->unlock();
					return false;
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
								return true;
							}
						}
					}
					//qDebug("ERROR sending to serial port (MotorControl)");
					// Unlocks the mutex
					mutex->unlock();
					return false;
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
								return true;
							}
						}
					}
					//qDebug("ERROR sending to serial port (MotorControl)");
					// Unlocks the mutex
					mutex->unlock();
					return false;
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
								return true;
							}
						}
					}
					//qDebug("ERROR sending to serial port (MotorControl)");
					// Unlocks the mutex
					mutex->unlock();
					return false;
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
								return true;
							}
						}
					}
					//qDebug("ERROR sending to serial port (MotorControl)");
					// Unlocks the mutex
					mutex->unlock();
					return false;
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
								return true;
							}
						}
					}
					//qDebug("ERROR sending to serial port (MotorControl)");
					// Unlocks the mutex
					mutex->unlock();
					return false;
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
								return true;
							}
						}
					}
					//qDebug("ERROR sending to serial port (MotorControl)");
					// Unlocks the mutex
					mutex->unlock();
					return false;
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
								return true;
							}
						}
					}
					//qDebug("ERROR sending to serial port (MotorControl)");
					// Unlocks the mutex
					mutex->unlock();
					return false;
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
								return true;
							}
						}
					}
					//qDebug("ERROR sending to serial port (MotorControl)");
					// Unlocks the mutex
					mutex->unlock();
					return false;
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
								return true;
							}
						}
					}
					//qDebug("ERROR sending to serial port (MotorControl)");
					// Unlocks the mutex
					mutex->unlock();
					return false;
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
								return true;
							}
						}
					}
					//qDebug("ERROR sending to serial port (MotorControl)");
					// Unlocks the mutex
					mutex->unlock();
					return false;
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
								return true;
							}
						}
					}
					//qDebug("ERROR sending to serial port (MotorControl)");
					// Unlocks the mutex
					mutex->unlock();
					return false;
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


	return false;
}


void Motor::flashlight(bool light)
{
	// maybe robot is already recognized as OFF by the interface class (e.g. path to serial port not found)!
	if (robotState == ON)
	{
		if (light == ON)
		{
			atmelCommand = commandFlashlightOn;
			expectedAtmelAnswer = commandFlashlightOn;
		}
		else
		{
			atmelCommand = commandFlashlightOff;
			expectedAtmelAnswer = commandFlashlightOff;
		}

		// Lock the mutex. If another thread has locked the mutex then this call will block until that thread has unlocked it.
		mutex->lock();

		//------------------
		// sending command
		//------------------
		emit message(QString("Sending *%1#...").arg(atmelCommand));
		if (interface1->sendString(atmelCommand) == true)
		{
			// start own time measuring. This will be used, if we get an answer from the Atmel
			duration.start();

			// start additional seperate timer. If we NEVER get an answer, this slot will be called
			QTimer::singleShot(ATMELTIMEOUT, this, SLOT(timeout()) );

			emit message("Sent.");
			emit message("Waiting for an answer...");

			// Unlock the mutex.
			mutex->unlock();

			return;
		}

		emit message("Error sending string.");

		// Unlock the mutex.
		mutex->unlock();
	}

	expectedAtmelAnswer.clear();

	// mark the robot as OFF within this class
	robotState = OFF;

	emit message("Error switching flashlight.");
///  @todo emit a Signal here?  No. Nobody needs to know that we had a problem setting the flashlight.
}


void Motor::takeCommandAnswer(QString atmelAnswer)
{
	emit message( QString("takeAnswer for %1: %2").arg(atmelCommand).arg(atmelAnswer) );

	//----------
	// timeout?
	//----------
	if (duration.elapsed() > ATMELTIMEOUT)
	{
		emit message(QString("Timeout (%1 > %2ms)").arg(duration.elapsed()).arg(ATMELTIMEOUT));

		// check the last command
		if (atmelCommand == commandFlashlightOn)
		{
			// timeout
			commandExecutedSuccessfull = false;
			atmelCommand.clear();
			expectedAtmelAnswer.clear();
			return;
		} // flashlight on

	}

	//------------------
	// everthing's fine
	//------------------
	if (atmelAnswer == expectedAtmelAnswer)
	{
		emit message(QString("Answer %1 was correct.").arg(atmelAnswer));

		// check the last command
		if (atmelCommand == commandFlashlightOn)
		{
			commandExecutedSuccessfull = true;
			atmelCommand.clear();
			expectedAtmelAnswer.clear();
			return;
		} // flashlight on
	}
	else
	{
		//--------------
		// wrong answer
		//--------------
		emit message(QString("ERROR: Answer was %1 intead of %2.").arg(atmelAnswer).arg(expectedAtmelAnswer));

		// check the last command
		if (atmelCommand == commandFlashlightOn)
		{
			// let this class know, that we had an error
			robotState = false;
			atmelCommand.clear();
			expectedAtmelAnswer.clear();
			return;
		} // flashlight on
	}
}


void Motor::timeout()
{
	// check the last command
	if (atmelCommand == commandFlashlightOn)
	{
		// first check if we had already an answer from the Atmel
		if (commandExecutedSuccessfull == true)
		{
			// reset state
			commandExecutedSuccessfull = false;
			// we are happy
			return;
		}

		emit message(QString("Timeout (> %2ms)").arg(ATMELTIMEOUT));
		expectedAtmelAnswer.clear();
		atmelCommand.clear();

		// let this class know, that we had an error
		robotState = false;

		return;
	} // flashlight on
}


/*
void Motor::makeSteps(int steps)
{
	if (robotState == ON)
	{
		for (int i=steps; i>=steps; i--)
		{
			qDebug("step %d", i);

			// Lock the mutex. If another thread has locked the mutex then this call will block until that thread has unlocked it.
			mutex->lock();

			if (interface1->sendChar(STEPPER_CLOCK) == false)        // FIXME: use sendString with a new nice string!
			{
				//qDebug("ERROR sending to serial port (Motor)");
			}

			// Unlocks the mutex. Attempting to unlock a mutex in a different thread to the one that locked it results in an error.
			mutex->unlock();

		}
	}
}


void Motor::parkStepper(unsigned char motor)
{
	if (robotState == ON)
	{
		/// \todo park stepper correctly!! (temporarily not in use)
		motorControl(STEPPER1, OFF, SAME);
		motorControl(STEPPER2, OFF, SAME);
	}
}
*/

void Motor::calculateMovement()
{
/* test

	/// \todo (not implemented in robot hardware and in avr code!)

	//--------------------------------------------------------------------------
	// if motor 1 is ON, step counter + 1
	//--------------------------------------------------------------------------

	// string for converting number to string
	QString str;

	if ((usbPort->port0 & MOTOR1POWER) == MOTOR1POWER)
	{
		// Steps + 1
		str = QString("%L1").arg(++steps1);
		// set the label text
		ui.labelSteps1->setText(str);


		// Measure driven distance and display it
		//
		//  One motor-step = 1,8 -> 360 = 200 steps = one revolution
		//  Diameter wheel = 66 mm
		//  Wheel circumference = PI*d = PI*66 mm = 207,345 mm
		//  Distance per step: 207,345 mm / 200 steps = 1,036725 mm = 0,1036725 cm
		//
		/// \todo check the conversion value and make it a const!
		drivenDistance1 += 0.1036725;

		//
		// cut all digits after the comma / point
		// for nice displaying the string in a label.
		//
		str = QString("%L1").arg(drivenDistance1);
		// search for the comma in the string and
		// truncate the string at the position of the comma + 2
		str.truncate(str.indexOf(commaSeparator) + 3);
		// append the measure unit
		str.append(" cm");
		// set the label text
		ui.labelDistance1->setText(str);


		//
		// Check if already enough steps are made for one revolution
		// (divide the made steps through 200)
		//
		div_t value = div(steps1, 200);

		// if remainder = 0 then 200 steps are made (one revolution)
		if (value.rem == 0)
		{
			// Revolutions + 1
			str = QString("%L1").arg(++revolutions1);
			ui.labelRevolutions1->setText(str);
		}
	} // Motor 1 on


	//--------------------------------------------------------------------------
	// if motor 2 is ON, step counter + 1
	//--------------------------------------------------------------------------
	if ((usbPort->port0 & MOTOR2POWER) == MOTOR2POWER)
	{
		// Steps + 1
		str = QString("%L1").arg(++steps2);
		// set the label text
		ui.labelSteps2->setText(str);


		// Measure driven distance and display it
		//
		//  One motor-step = 1,8 -> 360 = 200 steps = one revolution
		//  Diameter wheel = 66 mm
		//  Wheel circumference = PI*d = PI*66 mm = 207,345 mm
		//  Distance per step: 207,345 mm / 200 steps = 1,036725 mm = 0,1036725 cm
		//
		/// \todo check the conversion value and make it a const!
		drivenDistance2 += 0.1036725;

		//
		// cut all digits after the comma / point
		// for nice displaying the string in a label.
		//
		str = QString("%L1").arg(drivenDistance2);
		// search for the comma in the string and
		// truncate the string at the position of the comma + 2
		str.truncate(str.indexOf(commaSeparator) + 3);
		// append the measure unit
		str.append(" cm");
		// set the label text
		ui.labelDistance2->setText(str);


		//
		// Check if already enough steps are made for one revolution
		// (divide the made steps through 200)
		//
		div_t value = div(steps2, 200);

		// if remainder = 0 then 200 steps are made (one revolution)
		if (value.rem == 0)
		{
			// Revolutions + 1
			str = QString("%1").arg(++revolutions2);
			ui.labelRevolutions2->setText(str);

		}
	} // Motor 2 on
test */
}


void Motor::resetMovementCounter(short int motor)
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
		case MOTOR3:
		{
			// Initialize the motor step counters
			//steps3 = 0;

			// Initialize the motor revolutions counters
			revolutions3 = 0;

			// Initialize the distance to next objects in front of the robot
			drivenDistance3 = 0;
			return;
			break;
		}
		case MOTOR4:
		{
			// Initialize the motor step counters
			//steps4 = 0;

			// Initialize the motor revolutions counters
			revolutions4 = 0;

			// Initialize the distance to next objects in front of the robot
			drivenDistance4 = 0;
			return;
			break;
		}
	}
}


bool Motor::setMotorSpeed(int motor, int speed)
{
	QString answer = "error";


	if (robotState == ON)
	{
		if (speed < 0)
		{
			speed = 0;
		}

		if (speed > 255) /// \todo check if this max value is okay, since we have an int here now. sa: direcsAcr for max values!
		{
			speed = 255;
		}


		// Lock the mutex. If another thread has locked the mutex then this call will block until that thread has unlocked it.
		mutex->lock();
		switch (motor)
		{
			case ALLMOTORS: // "motor 0"
				// store the speed
				motorSpeedAllMotors = speed;
				motor1Speed = speed;
				motor2Speed = speed;
				motor3Speed = speed;
				motor4Speed = speed;
				// send command to microcontroller
				if (interface1->sendString(QString("*mv0%1#").arg(speed)) == true)
				{
					// check if the robot answers with "ok"
					if ( interface1->receiveString(answer) == true)
					{
						if (answer == "*mv0#")
						{
							// Unlock the mutex
							mutex->unlock();
							return true;
						}
					}
				}
				//qDebug("ERROR sending to serial port (MotorControl)");
				// Unlocks the mutex
				mutex->unlock();
				return false;
				break;

			case MOTOR1:
				// store the speed
				motor1Speed = speed;
				// send command to microcontroller
				if (interface1->sendString(QString("*mv1%1#").arg(speed)) == true)
				{
					// check if the robot answers with "ok"
					if ( interface1->receiveString(answer) == true)
					{
						if (answer == "*mv1#")
						{
							// Unlock the mutex
							mutex->unlock();
							return true;
						}
					}
				}
				//qDebug("ERROR sending to serial port (MotorControl)");
				// Unlocks the mutex
				mutex->unlock();
				return false;
				break;

			case MOTOR2:
				// store the speed
				motor2Speed = speed;
				// send command to microcontroller
				if (interface1->sendString(QString("*mv2%1#").arg(speed)) == true)
				{
					// check if the robot answers with "ok"
					if ( interface1->receiveString(answer) == true)
					{
						if (answer == "*mv2#")
						{
							// Unlock the mutex
							mutex->unlock();
							return true;
						}
					}
				}
				//qDebug("ERROR sending to serial port (MotorControl)");
				// Unlocks the mutex
				mutex->unlock();
				return false;
				break;

			case MOTOR3:
				// store the speed
				motor3Speed = speed;
				// send command to microcontroller
				if (interface1->sendString(QString("*mv3%1#").arg(speed)) == true)
				{
					// check if the robot answers with "ok"
					if ( interface1->receiveString(answer) == true)
					{
						if (answer == "*mv3#")
						{
							// Unlock the mutex
							mutex->unlock();
							return true;
						}
					}
				}
				//qDebug("ERROR sending to serial port (MotorControl)");
				// Unlocks the mutex
				mutex->unlock();
				return false;
				break;

			case MOTOR4:
				// store the speed
				motor4Speed = speed;
				// send command to microcontroller
				if (interface1->sendString(QString("*mv4%1#").arg(speed)) == true)
				{
					// check if the robot answers with "ok"
					if ( interface1->receiveString(answer) == true)
					{
						if (answer == "*mv4#")
						{
							// Unlock the mutex
							mutex->unlock();
							return true;
						}
					}
				}
				//qDebug("ERROR sending to serial port (MotorControl)");
				// Unlocks the mutex
				mutex->unlock();
				return false;
				break;
		} // switch

		// Unlock the mutex.
		mutex->unlock();
	} // robot is ON


	// robot is off
	return false;
}


void Motor::setMaximumSpeed(int speed)
{
	// FIXME: set the maximum speed for the robot / in this class!
	Q_UNUSED(speed);
}


int Motor::getMotorSpeed(int motor)
{
	switch (motor)
	{
		case MOTOR1:
			return motor1Speed;
			break;
		case MOTOR2:
			return motor2Speed;
			break;
		case MOTOR3:
			return motor3Speed;
			break;
		case MOTOR4:
			return motor4Speed;
			break;
	}

	return -1;
}


void Motor::setRobotState(bool state)
{
	// store the state within this class
	robotState = state;
}
