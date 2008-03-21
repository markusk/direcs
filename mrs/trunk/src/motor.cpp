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
 
	// Initialize the distance to next objects in front of the robot
	drivenDistance1 = 0;
	drivenDistance2 = 0;

	// Initialize the motor speed (not to fast, before reading the real values from a file!)
	motor1Speed = 20;
	motor2Speed = 20;
	motor3Speed = 10;
	motor4Speed = 10;
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


unsigned int Motor::getRevolutions1()
{
	return revolutions1;
}


unsigned int Motor::getRevolutions2()
{
	return revolutions2;
}


double Motor::getDrivenDistance1()
{
	return drivenDistance1;
}


double Motor::getDrivenDistance2()
{
	return drivenDistance2;
}


void Motor::motorControl(unsigned char motor, bool power, unsigned char direction)
{
	//---------------------------------------------------------------------------
	//
	// This method turn on/off the motors and switches them to clockwise or ccw.
	//
	//---------------------------------------------------------------------------

	// TODO: change to 'case' instrcution
	//-------------------------
	// if motor 1
	//------------------------
	if (motor == MOTOR1)
	{
		if (power == ON)
		{
			// Motors with gearings don't have to be turned on!
			// 
			// (This function is obsolete because of no longer using stepping motors)
		}
	
		if (power == OFF)
		{
			// turn off the MOTOR -> break!
			if (interface1->sendChar(MOTOR1_OFF) == false)
			{
				//qDebug("ERROR sending to serial port (Motor)");
				return;
			}
		}
	
		if (direction == CLOCKWISE)
		{
			// set the direction
			if (interface1->sendChar(MOTOR1_CLOCKWISE) == false)
			{
				//qDebug("ERROR sending to serial port (Motor)");
				return;
			}
		}
		
		if (direction == COUNTERCLOCKWISE)
		{
			// set the direction
			if (interface1->sendChar(MOTOR1_COUNTERCLOCKWISE) == false)
			{
				//qDebug("ERROR sending to serial port (Motor)");
				return;
			}
		}
			
		if (direction == SAME)
		{
			// don't change the direction (motor was only turned on or off)!
		}

		return;
		
	} // MOTOR1
 
 
	//-------------------------
	// If motor 2
	//-------------------------
	if (motor == MOTOR2)
	{
		if (power == ON)
		{
			// Motors with gearings don't have to be turned on!
			// 
			// (This function is obsolete because of no longer using stepping motors)
		}
	
		if (power == OFF)
		{
			// set the direction
			if (interface1->sendChar(MOTOR2_OFF) == false)
			{
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
			if (interface1->sendChar(MOTOR2_CLOCKWISE) == false)
			{
				//qDebug("ERROR sending to serial port (Motor)");
				return;
			}
		}
		
		if (direction == COUNTERCLOCKWISE)
		{
			// set the direction
			// set the direction
			if (interface1->sendChar(MOTOR2_COUNTERCLOCKWISE) == false)
			{
				//qDebug("ERROR sending to serial port (Motor)");
				return;
			}
		}
		
		if (direction == SAME)
		{
			// don't change the direction (motor was only turned on or off)!
		}
		
		return;
	} // MOTOR2

	
	//-------------------------
	// if motor 3
	//------------------------
	if (motor == MOTOR3)
	{
		if (power == ON)
		{
			// Motors with gearings don't have to be turned on!
			// 
			// (This function is obsolete because of no longer using stepping motors)
		}
	
		if (power == OFF)
		{
			// turn off the MOTOR -> break!
			if (interface1->sendChar(MOTOR3_OFF) == false)
			{
				//qDebug("ERROR sending to serial port (Motor)");
				return;
			}
		}
	
		if (direction == CLOCKWISE)
		{
			// set the direction
			if (interface1->sendChar(MOTOR3_CLOCKWISE) == false)
			{
				//qDebug("ERROR sending to serial port (Motor)");
				return;
			}
		}
		
		if (direction == COUNTERCLOCKWISE)
		{
			// set the direction
			if (interface1->sendChar(MOTOR3_COUNTERCLOCKWISE) == false)
			{
				//qDebug("ERROR sending to serial port (Motor)");
				return;
			}
		}
			
		if (direction == SAME)
		{
			// don't change the direction (motor was only turned on or off)!
		}

		return;
		
	} // MOTOR3

	
	//-------------------------
	// if motor 4
	//------------------------
	if (motor == MOTOR4)
	{
		if (power == ON)
		{
			// Motors with gearings don't have to be turned on!
			// 
			// (This function is obsolete because of no longer using stepping motors)
		}
	
		if (power == OFF)
		{
			// turn off the MOTOR -> break!
			if (interface1->sendChar(MOTOR4_OFF) == false)
			{
				//qDebug("ERROR sending to serial port (Motor)");
				return;
			}
		}
	
		if (direction == CLOCKWISE)
		{
			// set the direction
			if (interface1->sendChar(MOTOR4_CLOCKWISE) == false)
			{
				//qDebug("ERROR sending to serial port (Motor)");
				return;
			}
		}
		
		if (direction == COUNTERCLOCKWISE)
		{
			// set the direction
			if (interface1->sendChar(MOTOR4_COUNTERCLOCKWISE) == false)
			{
				//qDebug("ERROR sending to serial port (Motor)");
				return;
			}
		}
			
		if (direction == SAME)
		{
			// don't change the direction (motor was only turned on or off)!
		}

		return;
		
	} // MOTOR4
 
 
	
	/*
	//-------------------------
	// if stepper motor 1
	//------------------------
	if (motor == STEPPER1)
	{
		if (power == ON)
		{
			// turn on stepper 1
			if (interface1->sendChar(STEPPER1_ON) == false)
			{
				//qDebug("ERROR sending to serial port (Motor)");
				//return;
			}
		}
		
		if (power == OFF)
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

		return;
		
	} // STEPPER1
	
	//-------------------------
	// if stepper motor 2
	//------------------------
	if (motor == STEPPER2)
	{
		if (power == ON)
		{
			// turn on stepper 2
			if (interface1->sendChar(STEPPER2_ON) == false)
			{
				//qDebug("ERROR sending to serial port (Motor)");
				//return;
			}
		}
		
		if (power == OFF)
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

		return;
		
	} // STEPPER2
	*/
}


/*
void Motor::makeSteps(int steps)
{
	for (int i=steps; i>=steps; i--)
	{
		qDebug("step %d", i);

		if (interface1->sendChar(STEPPER_CLOCK) == false)
		{
			//qDebug("ERROR sending to serial port (Motor)");
		}
	}
}


void Motor::parkStepper(unsigned char motor)
{
	// FIXME: park stepper correctly!! (temporarily not in use)
	motorControl(STEPPER1, OFF, SAME);
	motorControl(STEPPER2, OFF, SAME);
}
*/

void Motor::calculateMovement()
{
/* test
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


/*

! ersetzt durch setMotorSpeed in motorThread !

void Motor::setMotorPWM(bool state)
{
	unsigned char port0 = 0; // USB port 0
	
	//-----------------
	// Read Port 0
	//-----------------
	usbPort->readPort(0, &port0);
	
	
	if (state == true)
	{

		// set Motor-PWM-Bit (HIGH)
		port0 |= MOTORCLOCK;
	
	}
	else
	{
		// delete Motor-PWM-Bit (LOW)
		port0 &= (255 - MOTORCLOCK);
	}
	
	usbPort->writePort(0, port0);
}
*/


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
	}
}


void Motor::setMotorSpeed(int motor, int speed)
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
			if (interface1->sendChar(MOTOR1_SPEED_SET) == true)
			{
				// send the value to the microcontroller
				if (interface1->sendChar(speed) == false)
				{
					//qDebug("ERROR sending to serial port (Motor)");
					return;
				}
			}
			else	
			{
				//qDebug("ERROR sending to serial port (Motor)");
				return;
			}
			break;
			
		case 2:
			// store the speed
			motor2Speed = speed;
			// send the command to the microcontroller (MOTOR 2)
			if (interface1->sendChar(MOTOR2_SPEED_SET) == true)
			{
				// send the value to the microcontroller
				if (interface1->sendChar(speed) == false)
				{
					//qDebug("ERROR sending to serial port (Motor)");
					return;
				}
			}
			else	
			{
				//qDebug("ERROR sending to serial port (Motor)");
				return;
			}
			break;
	
		case 3:
			// store the speed
			motor3Speed = speed;
			// send the command to the microcontroller (MOTOR 3)
			if (interface1->sendChar(MOTOR3_SPEED_SET) == true)
			{
				// send the value to the microcontroller
				if (interface1->sendChar(speed) == false)
				{
					//qDebug("ERROR sending to serial port (Motor)");
					return;
				}
			}
			else	
			{
				//qDebug("ERROR sending to serial port (Motor)");
				return;
			}
			break;
			
		case 4:
			// store the speed
			motor4Speed = speed;
			// send the command to the microcontroller (MOTOR 4)
			if (interface1->sendChar(MOTOR4_SPEED_SET) == true)
			{
				// send the value to the microcontroller
				if (interface1->sendChar(speed) == false)
				{
					//qDebug("ERROR sending to serial port (Motor)");
					return;
				}
			}
			else	
			{
				//qDebug("ERROR sending to serial port (Motor)");
				return;
			}
			break;
	} // switch
}


int Motor::getMotorSpeed(int motor)
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


void Motor::flashlight(bool state)
{
	if (state == ON)
	{
		if (interface1->sendChar(FLASHLIGHT_ON) == false)
		{
			//qDebug("ERROR sending to serial port (Motor)");
			return;
		}
	}
	else
	{
		if (interface1->sendChar(FLASHLIGHT_OFF) == false)
		{
			//qDebug("ERROR sending to serial port (Motor)");
			return;
		}
	}
}
