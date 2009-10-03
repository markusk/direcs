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


void Motor::motorControl(unsigned char motor, bool power, unsigned char direction)
{
	unsigned char command = BOTSTOP;
	
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
						command = BOTFORWARD;
						break;
					case BACKWARD:
						command = BOTBACKWARD;
						break;
					case LEFT:
						command = BOTLEFT;
						break;
					case RIGHT:
						command = BOTRIGHT;
						break;
					case TURNLEFT:
						command = BOTTURNLEFT;
						break;
					case TURNRIGHT:
						command = BOTTURNRIGHT;
						break;
					case START:
						command = BOTSTART;
						break;
					case STOP:
						command = BOTSTOP;
						break;
					case WAIT:
						command = BOTWAIT;
						break;
				}
					
				// send command to bot
				if (interface1->sendChar(command) == false)
				{
					// Unlocks the mutex. Attempting to unlock a mutex in a different thread to the one that locked it results in an error.
					mutex->unlock();
					//qDebug("ERROR sending to serial port (Motor)");
					return;
				}
				// Unlocks the mutex
				mutex->unlock();
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
					if (interface1->sendChar(MOTOR1_OFF) == false)
					{
						// Unlocks the mutex. Attempting to unlock a mutex in a different thread to the one that locked it results in an error.
						mutex->unlock();
						//qDebug("ERROR sending to serial port (Motor)");
						return;
					}
				}
		
				if (direction == CLOCKWISE)
				{
					// set the direction
					if (interface1->sendChar(MOTOR1_CLOCKWISE) == false)
					{
						// Unlocks the mutex. Attempting to unlock a mutex in a different thread to the one that locked it results in an error.
						mutex->unlock();
						//qDebug("ERROR sending to serial port (Motor)");
						return;
					}
				}
			
				if (direction == COUNTERCLOCKWISE)
				{
					// set the direction
					if (interface1->sendChar(MOTOR1_COUNTERCLOCKWISE) == false)
					{
						// Unlocks the mutex. Attempting to unlock a mutex in a different thread to the one that locked it results in an error.
						mutex->unlock();
						//qDebug("ERROR sending to serial port (Motor)");
						return;
					}
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
					// set the direction
					if (interface1->sendChar(MOTOR2_OFF) == false)
					{
						// Unlocks the mutex. Attempting to unlock a mutex in a different thread to the one that locked it results in an error.
						mutex->unlock();
						//qDebug("ERROR sending to serial port (Motor)");
						return;
					}
				}
		
				//
				//   A T T E N T I O N :
				//
				//   Because of the right-left reversed mounting of one of the motors
				//   the control-logic for CLOCKWISE and COUNTERCLOCKWISE could be used 
				//   reversed in this method. And only in this method!
				//   Because of this reversed logic the commands could swapped here!
				//   (counterclockwise = clockwise and vice versa)
				//   THIS HAS ONLY TO BE DONE, WHEN THE MOTORS ARE WIRED EXACTLY THE SAME!
				//   black = minus	red = plus	->	on both motors identically!
				//
				if (direction == CLOCKWISE)
				{
					// set the direction
					if (interface1->sendChar(MOTOR2_COUNTERCLOCKWISE) == false)
					{
						// Unlocks the mutex. Attempting to unlock a mutex in a different thread to the one that locked it results in an error.
						mutex->unlock();
						//qDebug("ERROR sending to serial port (Motor)");
						return;
					}
				}
				
				if (direction == COUNTERCLOCKWISE)
				{
					// set the direction
					// set the direction
					if (interface1->sendChar(MOTOR2_CLOCKWISE) == false)
					{
						// Unlocks the mutex. Attempting to unlock a mutex in a different thread to the one that locked it results in an error.
						mutex->unlock();
						//qDebug("ERROR sending to serial port (Motor)");
						return;
					}
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
					if (interface1->sendChar(MOTOR3_OFF) == false)
					{
						// Unlocks the mutex. Attempting to unlock a mutex in a different thread to the one that locked it results in an error.
						mutex->unlock();
						//qDebug("ERROR sending to serial port (Motor)");
						return;
					}
				}
			
				if (direction == CLOCKWISE)
				{
					// set the direction
					if (interface1->sendChar(MOTOR3_CLOCKWISE) == false)
					{
						// Unlocks the mutex. Attempting to unlock a mutex in a different thread to the one that locked it results in an error.
						mutex->unlock();
						//qDebug("ERROR sending to serial port (Motor)");
						return;
					}
				}
				
				if (direction == COUNTERCLOCKWISE)
				{
					// set the direction
					if (interface1->sendChar(MOTOR3_COUNTERCLOCKWISE) == false)
					{
						// Unlocks the mutex. Attempting to unlock a mutex in a different thread to the one that locked it results in an error.
						mutex->unlock();
						//qDebug("ERROR sending to serial port (Motor)");
						return;
					}
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
					if (interface1->sendChar(MOTOR4_OFF) == false)
					{
						// Unlocks the mutex. Attempting to unlock a mutex in a different thread to the one that locked it results in an error.
						mutex->unlock();
						//qDebug("ERROR sending to serial port (Motor)");
						return;
					}
				}
			
				//
				//   A T T E N T I O N :
				//
				//   Because of the right-left reversed mounting of one of the motors
				//   the control-logic for CLOCKWISE and COUNTERCLOCKWISE could be used 
				//   reversed in this method. And only in this method!
				//   Because of this reversed logic the commands could swapped here!
				//   (counterclockwise = clockwise and vice versa)
				//   THIS HAS ONLY TO BE DONE, WHEN THE MOTORS ARE WIRED EXACTLY THE SAME!
				//   black = minus	red = plus	->	on both motors identically!
				//
				if (direction == CLOCKWISE)
				{
					// set the direction
					if (interface1->sendChar(MOTOR4_COUNTERCLOCKWISE) == false)
					{
						// Unlocks the mutex. Attempting to unlock a mutex in a different thread to the one that locked it results in an error.
						mutex->unlock();
						//qDebug("ERROR sending to serial port (Motor)");
						return;
					}
				}
				
				if (direction == COUNTERCLOCKWISE)
				{
					// set the direction
					if (interface1->sendChar(MOTOR4_CLOCKWISE) == false)
					{
						// Unlocks the mutex. Attempting to unlock a mutex in a different thread to the one that locked it results in an error.
						mutex->unlock();
						//qDebug("ERROR sending to serial port (Motor)");
						return;
					}
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
					if (interface1->sendChar(STEPPER1_ON) == false)
					{
						//qDebug("ERROR sending to serial port (Motor)");
						//return;
					}
				}
				else
				{
					// turn off stepper 1
					if (interface1->sendChar(STEPPER1_OFF) == false)
					{
						//qDebug("ERROR sending to serial port (Motor)");
						//return;
					}
				}
		
				if (direction == CLOCKWISE)
				{
					// set the direction
					if (interface1->sendChar(STEPPER1_CLOCKWISE) == false)
					{
						//qDebug("ERROR sending to serial port (Motor)");
						//return;
					}
				}
				
				if (direction == COUNTERCLOCKWISE)
				{
					// set the direction
					if (interface1->sendChar(STEPPER1_COUNTERCLOCKWISE) == false)
					{
						//qDebug("ERROR sending to serial port (Motor)");
						//return;
					}
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
					if (interface1->sendChar(STEPPER2_ON) == false)
					{
						//qDebug("ERROR sending to serial port (Motor)");
						//return;
					}
				}
				else
				{
					// turn off stepper 2
					if (interface1->sendChar(STEPPER2_OFF) == false)
					{
						//qDebug("ERROR sending to serial port (Motor)");
						//return;
					}
				}
		
				if (power == CLOCK)
				{
					// make a stepper with every enabled motor
					if (interface1->sendChar(STEPPER_CLOCK) == false)
					{
						//qDebug("ERROR sending to serial port (Motor)");
						//return;
					}
				}
				
				if (direction == CLOCKWISE)
				{
					// set the direction
					if (interface1->sendChar(STEPPER2_CLOCKWISE) == false)
					{
						//qDebug("ERROR sending to serial port (Motor)");
						//return;
					}
				}
				
				if (direction == COUNTERCLOCKWISE)
				{
					// set the direction
					if (interface1->sendChar(STEPPER2_COUNTERCLOCKWISE) == false)
					{
						//qDebug("ERROR sending to serial port (Motor)");
						//return;
					}
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
			
			if (interface1->sendChar(STEPPER_CLOCK) == false)
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
		// TODO: park stepper correctly!! (temporarily not in use)
		motorControl(STEPPER1, OFF, SAME);
		motorControl(STEPPER2, OFF, SAME);
	}
}
*/

void Motor::calculateMovement()
{
/* test
	
	// TODO: (not implemented in robot hardware and in avr code!)
	
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
		// TODO: check the conversion value and make it a const!
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
		// TODO: check the conversion value and make it a const!
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


void Motor::setMotorSpeed(int motor, int speed)
{
	// FIXME: add a return code on serial error (false or so)

	if (robotState == ON)
	{
		if (speed < 0)
		{
			speed = 0;
		}
		
		if (speed > 255) // TODO: check if this max value is okay, since we have an int here now. sa: direcsAcr for max values!
		{
			speed = 255;
		}
		
		
		// Lock the mutex. If another thread has locked the mutex then this call will block until that thread has unlocked it.
		mutex->lock();
		switch (motor)
		{
			case ALLMOTORS:
				// store the speed
				motorSpeedAllMotors = speed;
				motor1Speed = speed;
				motor2Speed = speed;
				motor3Speed = speed;
				motor4Speed = speed;
				// send the command to the microcontroller (MOTOR 1)
				if (interface1->sendChar(SPEED_SET_ALLMOTORS) == true)
				{
					// send the value to the microcontroller
					if (interface1->sendChar(speed) == false)
					{
						// Unlock the mutex.
						mutex->unlock();
						//qDebug("ERROR sending to serial port (Motor)");
						return;
					}
				}
				else	
				{
					// Unlock the mutex.
					mutex->unlock();
					//qDebug("ERROR sending to serial port (Motor)");
					return;
				}
				break;

			case MOTOR1:
				// store the speed
				motor1Speed = speed;
				// send the command to the microcontroller (MOTOR 1)
				if (interface1->sendChar(MOTOR1_SPEED_SET) == true)
				{
					// send the value to the microcontroller
					if (interface1->sendChar(speed) == false)
					{
						// Unlock the mutex.
						mutex->unlock();
						//qDebug("ERROR sending to serial port (Motor)");
						return;
					}
				}
				else	
				{
					// Unlock the mutex.
					mutex->unlock();
					//qDebug("ERROR sending to serial port (Motor)");
					return;
				}
				break;
				
			case MOTOR2:
				// store the speed
				motor2Speed = speed;
				// send the command to the microcontroller (MOTOR 2)
				if (interface1->sendChar(MOTOR2_SPEED_SET) == true)
				{
					// send the value to the microcontroller
					if (interface1->sendChar(speed) == false)
					{
						// Unlock the mutex.
						mutex->unlock();
						//qDebug("ERROR sending to serial port (Motor)");
						return;
					}
				}
				else	
				{
					// Unlock the mutex.
					mutex->unlock();
					//qDebug("ERROR sending to serial port (Motor)");
					return;
				}
				break;
		
			case MOTOR3:
				// store the speed
				motor3Speed = speed;
				// send the command to the microcontroller (MOTOR 3)
				if (interface1->sendChar(MOTOR3_SPEED_SET) == true)
				{
					// send the value to the microcontroller
					if (interface1->sendChar(speed) == false)
					{
						// Unlock the mutex.
						mutex->unlock();
						//qDebug("ERROR sending to serial port (Motor)");
						return;
					}
				}
				else	
				{
					// Unlock the mutex.
					mutex->unlock();
					//qDebug("ERROR sending to serial port (Motor)");
					return;
				}
				break;
				
			case MOTOR4:
				// store the speed
				motor4Speed = speed;
				// send the command to the microcontroller (MOTOR 4)
				if (interface1->sendChar(MOTOR4_SPEED_SET) == true)
				{
					// send the value to the microcontroller
					if (interface1->sendChar(speed) == false)
					{
						// Unlock the mutex.
						mutex->unlock();
						//qDebug("ERROR sending to serial port (Motor)");
						return;
					}
				}
				else	
				{
					// Unlock the mutex.
					mutex->unlock();
					//qDebug("ERROR sending to serial port (Motor)");
					return;
				}
				break;
		} // switch
		
		// Unlock the mutex.
		mutex->unlock();
	} // robot is ON
}


void Motor::setMaximumSpeed(int speed)
{
	// FIXME: set the maximum speed for the robot / in this class!
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


void Motor::flashlight(bool state)
{
	if (robotState == ON)
	{
		// Lock the mutex. If another thread has locked the mutex then this call will block until that thread has unlocked it.
		mutex->lock();
		
		if (state == ON)
		{
			
			if (interface1->sendChar(FLASHLIGHT_ON) == false)
			{
				// Unlocks the mutex. Attempting to unlock a mutex in a different thread to the one that locked it results in an error.
				mutex->unlock();
				//qDebug("ERROR sending to serial port (Motor)");
				return;
			}
		}
		else
		{
			if (interface1->sendChar(FLASHLIGHT_OFF) == false)
			{
				// Unlocks the mutex. Attempting to unlock a mutex in a different thread to the one that locked it results in an error.
				mutex->unlock();
				//qDebug("ERROR sending to serial port (Motor)");
				return;
			}
		}
		
		// Unlocks the mutex. Attempting to unlock a mutex in a different thread to the one that locked it results in an error.
		mutex->unlock();
	} // robot is ON
}


void Motor::setRobotState(bool state)
{
	// store the state within this class
	robotState = state;
}
