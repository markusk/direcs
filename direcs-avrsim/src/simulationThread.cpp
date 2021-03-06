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

#include "simulationThread.h"

SimulationThread::SimulationThread(InterfaceAvr *i, QMutex *m)
{
	className = this->staticMetaObject.className();

	stopped = false;
	simulationMode = false;

	// copy the pointer from the original object
	interface1 = i;
	mutex = m;

	robotState = ON; // Wer're thinking positive. The robot is ON untill whe know nothing other. :-)

	leftDistanceCounter = 0;
	rightDistanceCounter = 0;

	commandComplete = false;
}


SimulationThread::~SimulationThread()
{
}


void SimulationThread::stop()
{
	stopped = true;
}


void SimulationThread::run()
{
	bool heartbeatToggle = false;


	//
	//  start "threading"...
	//
	while (!stopped)
	{
		// let the thread sleep some time
		// for having more time for the other threads
		msleep(THREADSLEEPTIME);


		if ( (robotState == ON) && (simulationMode == false) && (commandComplete == true))
		{

			// Everything's fine, so reset the watchdog timer (wdt).
			///	@todo		wdt_reset();

			//--------------------------
			// check what was received
			//--------------------------

			// RESET / INIT
			if (commandString == "*re#")
			{
				/*
				// turn all drive motor bits off (except PWM bits)
				PORTL &= ~(1<<PIN0);
				PORTL &= ~(1<<PIN1);
				PORTL &= ~(1<<PIN2);
				PORTL &= ~(1<<PIN3);
				PORTL &= ~(1<<PIN6);
				PORTL &= ~(1<<PIN7);
				PORTD &= ~(1<<PIN6);
				PORTD &= ~(1<<PIN7);
				*/
				// flashlight off
				relais(OFF);
				// red LED off. Know we know, that the program on the PC/Mac has initialised the Atmel
				redLED(OFF);

				/*
				// setServoPosition(1, 17); // <- exact position now in the mrs.ini!
				// setServoPosition(2, 19); // <- exact position now in the mrs.ini!
				// setServoPosition(3, 23); // <- exact position now in the mrs.ini!
				// setServoPosition(4, 19); // <- exact position now in the mrs.ini!
				// setServoPosition(5, 19); // <- exact position now in the mrs.ini!
				// setServoPosition(6, 22); // <- exact position now in the mrs.ini!
*/
				// answer with "re"
				// this answer is used to see if the robot is "on"
				sendToAtmel("*re#");

				// This sends the string to the GUI
				emit answer("*re#");

				// e n a b l e  watchdog!
				/// @todo			watchdog(ENABLE);
			} // *re#
								else
								// SLEEP (and turn off watchdog)
								if (commandString == "*sl#")
								{
									/*
									// turn all drive motor bits off (except PWM bits)
									PORTL &= ~(1<<PIN0);
									PORTL &= ~(1<<PIN1);
									PORTL &= ~(1<<PIN2);
									PORTL &= ~(1<<PIN3);
									PORTL &= ~(1<<PIN6);
									PORTL &= ~(1<<PIN7);
									PORTD &= ~(1<<PIN6);
									PORTD &= ~(1<<PIN7);
*/
									// flashlight off
									relais(OFF);

									// red LED off
									redLED(OFF);

									// d i s a b l e  watchdog!
									///	@todo		wdt_reset();

									// answer
									sendToAtmel("*sl#");
									emit answer("*sl#");
								}

								/*
								else
								// READ_SENSOR_1
								if (commandString == "*s1#")
								{
									// read ADC and send answer over serial port
									sendUInt( readADC(SENSOR1) );
								}
								else
								// READ_SENSOR_2
								if (commandString == "*s2#")
								{
									// read ADC and send answer over serial port
									sendUInt( readADC(SENSOR2) );
								}
								else
								// READ_SENSOR_3
								if (commandString == "*s3#")
								{
									// read ADC and send answer over serial port
									sendUInt( readADC(SENSOR3) );
								}
								else
								// READ_SENSOR_4
								if (commandString == "*s4#")
								{
									// read ADC and send answer over serial port
									sendUInt( readADC(SENSOR4) );
								}
								else
								// READ_SENSOR_5
								if (commandString == "*s5#")
								{
									// read ADC and send answer over serial port
									sendUInt( readADC(SENSOR5) );
								}
								else
								// READ_SENSOR_6
								if (commandString == "*s6#")
								{
									// read ADC and send answer over serial port
									sendUInt( readADC(SENSOR6) );
								}
								*/
								else
								// READ_SENSOR_7 (24 V supply)
								if (commandString == "*s7#")
								{
									// read ADC and send answer over serial port
									sendUInt( readADC(SENSOR7) );
								}
								else
								// READ_SENSOR_8 (12 V supply)
								if (commandString == "*s8#")
								{
									// read ADC and send answer over serial port
									sendUInt( readADC(SENSOR8) );
								}
								/*
								else
								// READ_SENSOR_16
								// ultra sonic sensor !!
								if (commandString == "*s16#")
								{
									// read ADC and send answer over serial port
									sendUInt( ultraschall_messung() );
								}
								*/
								else
								// READ 3D COMPASS CONNECTION
								if (commandString == "*cc#")
								{
									// answer with "ok"
									// this answer is used to see if the robot is "on"
									sendToAtmel("*cc#");

									// This sends the string to the GUI
									emit answer("*cc#");
								}
								/*
								else
								// READ_AXIS_X
								if (commandString == "*cx#")
								{
									// check if micromag is connected to Atmel-Board (PB6 = high)
									if ( bit_is_set(PINB,PIN6) )
									{
										// read axis from the micromag3 sensor and send answer over serial port
										sendUInt( readMicromag(READ_AXIS_X) );
									}
									else
									{
										// not connected.
										sendUInt(0);
									}
								}
								else
								// READ_AXIS_Y
								if (commandString == "*cy#")
								{
									// check if micromag is connected to Atmel-Board (PB6 = high)
									if ( bit_is_set(PINB,PIN6) )
									{
										// read axis from the micromag3 sensor and send answer over serial port
										sendUInt( readMicromag(READ_AXIS_Y) );
									}
									else
									{
										// not connected.
										sendUInt(0);
									}
								}
								else
								// READ_AXIS_Z
								if (commandString == "*cz#")
								{
									// check if micromag is connected to Atmel-Board (PB6 = high)
									if ( bit_is_set(PINB,PIN6) )
									{
										// read axis from the micromag3 sensor and send answer over serial port
										sendUInt( readMicromag(READ_AXIS_Z) );
									}
									else
									{
										// not connected.
										sendUInt(0);
									}
								}
								*/
								else
								// READ_MOTOR_SENSOR1
								if (commandString == "*ms1#")
								{
									// read ADC and send answer over serial port
									sendUInt( readADC(SENSORMOTOR1) );
								}
								else
								// READ_MOTOR_SENSOR2
								if (commandString == "*ms2#")
								{
									// read ADC and send answer over serial port
									sendUInt( readADC(SENSORMOTOR2) );
								}
								else
								// READ_MOTOR_DISTANCE1
								if (commandString == "*dd1#")
								{
									// send driven distance of motor 1 encoder over serial port
									sendUInt( rightDistanceCounter );
								}
								else
								// READ_MOTOR_DISTANCE2
								if (commandString == "*dd2#")
								{
									// send driven distance of motor 2 encoder over serial port
									sendUInt( leftDistanceCounter );
								}
								/*
								else
								// RESET_MOTOR_DISTANCE1
								if (commandString == "*id1#")
								{
									// init driven distance of motor 1 (encoder)
									rightDistanceCounter = 0;
									rightWheelCounter = 0;
									// answer with "ok"
									emit answer("*ok#");
								}
								else
								// RESET_MOTOR_DISTANCE2
								if (commandString == "*id2#")
								{
									// init driven distance of motor 2 (encoder)
									leftDistanceCounter = 0;
									leftWheelCounter = 0;
									// answer with "ok"
									emit answer("*ok#");
								}
*/
								else
								// FLASHLIGHT ON
								if (commandString == "*f0on#")
								{
									relais(ON);
//									yellowLED(ON);
									sendToAtmel("*f0on#");
									emit answer("*f0on#");
								}
								else
								// FLASHLIGHT OFF
								if (commandString == "*f0of#")
								{
									relais(OFF);
//									yellowLED(OFF);
									sendToAtmel("*#");
									emit answer("*f0of#");
								}
/*
								else
								// MOTOR1_OFF
								if (commandString == "*mp1of#")
								{
									// delete Motor1 A bit
									PORTL &= ~(1<<PIN0);
									// delete Motor1 B bit
									PORTL &= ~(1<<PIN1);
									// answer with "ok"
									emit answer("*mp1of#");
								}
								else
								// MOTOR 1 CLOCKWISE = forward
								if (commandString == "*md1cw#")
								{
									// delete Motor1 A bit
									PORTL &= ~(1<<PIN0);
									// set Motor1 B bit
									PORTL |= (1<<PIN1);

									// answer with "ok"
									emit answer("*md1cw#");
								}
								else
								// MOTOR 1 COUNTERCLOCKWISE = backward
								if (commandString == "*md1cc#")
								{
									// set Motor1 A bit
									PORTL |= (1<<PIN0);
									// delete Motor1 B bit
									PORTL &= ~(1<<PIN1);

									// answer with "ok"
									emit answer("*md1cc#");
								}
								else
								// MOTOR1_SPEED_SET
								if (commandString.startsWith("*mv1", Qt::CaseInsensitive))
								{
									// change first chars for upcoming string conversion
									stringbuffer[0] = '0';
									stringbuffer[1] = '0';
									stringbuffer[2] = '0';
									stringbuffer[3] = '0';
									// get value from string and set speed
									setPWMwidth(1, atoi(stringbuffer));
									// answer with "ok"
									emit answer("*mv1#");
								}
								else
								// MOTOR2_OFF
								if (commandString == "*mp2of#")
								{
									// delete Motor2 A bit
									PORTL &= ~(1<<PIN2);
									// delete Motor2 B bit
									PORTL &= ~(1<<PIN3);
									// answer with "ok"
									emit answer("*mp2of#");
								}
								else
								// MOTOR 2 CLOCKWISE = forward
								if (commandString == "*md2cw#")
								{
									// delete Motor2 A bit
									PORTL &= ~(1<<PIN2);
									// set Motor2 B bit
									PORTL |= (1<<PIN3);

									// answer with "ok"
									emit answer("*md2cw#");
								}
								else
								// MOTOR 2 COUNTERCLOCKWISE = backward
								if (commandString == "*md2cc#")
								{
									// set Motor2 A bit
									PORTL |= (1<<PIN2);
									// delete Motor2 B bit
									PORTL &= ~(1<<PIN3);

									// answer with "ok"
									emit answer("*md2cc#");
								}
								else
								// MOTOR2_SPEED_SET
								if (commandString.startsWith("*mv2", Qt::CaseInsensitive))
								{
									// change first chars for upcoming string conversion
									stringbuffer[0] = '0';
									stringbuffer[1] = '0';
									stringbuffer[2] = '0';
									stringbuffer[3] = '0';
									// get value from string and set speed
									setPWMwidth(2, atoi(stringbuffer));
									// answer with "ok"
									emit answer("*mv2#");
								}
								else
								// MOTOR3_OFF
								if (commandString == "*mp3of#")
								{
									// delete Motor3 A bit
									PORTL &= ~(1<<PIN6);
									// delete Motor3 B bit
									PORTL &= ~(1<<PIN7);
									// answer with "ok"
									emit answer("*mp3of#");
								}
								else
								// MOTOR 3 CLOCKWISE = forward
								if (commandString == "*md3cw#")
								{
									// delete Motor3 A bit
									PORTL &= ~(1<<PIN6);
									// set Motor3 B bit
									PORTL |= (1<<PIN7);

									// answer with "ok"
									emit answer("*md3cw#");
								}
								else
								// MOTOR 3 COUNTERCLOCKWISE = backward
								if (commandString == "*md3cc#")
								{
									// set Motor3 A bit
									PORTL |= (1<<PIN6);
									// delete Motor3 B bit
									PORTL &= ~(1<<PIN7);

									// answer with "ok"
									emit answer("*md3cc#");
								}
								else
								// MOTOR3_SPEED_SET
								if (commandString.startsWith("*mv3", Qt::CaseInsensitive))
								{
									// change first chars for upcoming string conversion
									stringbuffer[0] = '0';
									stringbuffer[1] = '0';
									stringbuffer[2] = '0';
									stringbuffer[3] = '0';
									// get value from string and set speed
									setPWMwidth(3, atoi(stringbuffer));
									// answer with "ok"
									emit answer("*mv3#");
								}
								else
								// MOTOR4_OFF
								if (commandString == "*mp4of#")
								{
									// delete Motor4 A bit
									PORTD &= ~(1<<PIN6);
									// delete Motor4 B bit
									PORTD &= ~(1<<PIN7);
									// answer with "ok"
									emit answer("*mp4of#");
								}
								else
								// MOTOR 4 CLOCKWISE = forward
								if (commandString == "*md4cw#")
								{
									// delete Motor4 A bit
									PORTD &= ~(1<<PIN6);
									// set Motor4 B bit
									PORTD |= (1<<PIN7);

									// answer with "ok"
									emit answer("*md4cw#");
								}
								else
								// MOTOR 4 COUNTERCLOCKWISE = backward
								if (commandString == "*md4cc#")
								{
									// set Motor4 A bit
									PORTD |= (1<<PIN6);
									// delete Motor4 B bit
									PORTD &= ~(1<<PIN7);

									// answer with "ok"
									emit answer("*md4cc#");
								}
								else
								// MOTOR4_SPEED_SET
								if (commandString.startsWith("*mv4", Qt::CaseInsensitive))
								{
									// change first chars for upcoming string conversion
									stringbuffer[0] = '0';
									stringbuffer[1] = '0';
									stringbuffer[2] = '0';
									stringbuffer[3] = '0';
									// get value from string and set speed
									setPWMwidth(4, atoi(stringbuffer));
									// answer with "ok"
									emit answer("*mv4#");
								}
								else
								// SPEED_SET_ALLMOTORS
								if (commandString.startsWith("*mv0", Qt::CaseInsensitive))
								{
									// change first chars for upcoming string conversion
									stringbuffer[0] = '0';
									stringbuffer[1] = '0';
									stringbuffer[2] = '0';
									stringbuffer[3] = '0';
									// get value from string and set speed
									setPWMwidth(1, atoi(stringbuffer));
									setPWMwidth(2, atoi(stringbuffer));
									setPWMwidth(3, atoi(stringbuffer));
									setPWMwidth(4, atoi(stringbuffer));
									// answer with "ok"
									emit answer("*mv0#");
								}
								else
								// BOTSTOP
								if (commandString == "*bst#")
								{
									// MOTOR 1 OFF
									// delete Motor1 A bit
									PORTL &= ~(1<<PIN0);
									// delete Motor1 B bit
									PORTL &= ~(1<<PIN1);
									// MOTOR 2 OFF
									// delete Motor2 A bit
									PORTL &= ~(1<<PIN2);
									// delete Motor2 B bit
									PORTL &= ~(1<<PIN3);
									// MOTOR 3 OFF
									// delete Motor3 A bit
									PORTL &= ~(1<<PIN6);
									// delete Motor3 B bit
									PORTL &= ~(1<<PIN7);
									// MOTOR 4 OFF
									// delete Motor4 A bit
									PORTD &= ~(1<<PIN6);
									// delete Motor4 B bit
									PORTD &= ~(1<<PIN7);

									// answer with "ok"
									emit answer("*bst#");
								}
								else
								// BOTWAIT
								if (commandString == "*bwa#")
								{
									// MOTOR 1 OFF
									// delete Motor1 A bit
									PORTL &= ~(1<<PIN0);
									// delete Motor1 B bit
									PORTL &= ~(1<<PIN1);
									// MOTOR 2 OFF
									// delete Motor2 A bit
									PORTL &= ~(1<<PIN2);
									// delete Motor2 B bit
									PORTL &= ~(1<<PIN3);
									// MOTOR 3 OFF
									// delete Motor3 A bit
									PORTL &= ~(1<<PIN6);
									// delete Motor3 B bit
									PORTL &= ~(1<<PIN7);
									// MOTOR 4 OFF
									// delete Motor4 A bit
									PORTD &= ~(1<<PIN6);
									// delete Motor4 B bit
									PORTD &= ~(1<<PIN7);

									// answer with "ok"
									emit answer("*bwa#");
								}
								else
								// BOTSTART = "bot go"
								if (commandString == "*bgo#")
								{
									// MOTOR 1 CLOCKWISE = forward
									// delete Motor1 A bit
									PORTL &= ~(1<<PIN0);
									// set Motor1 B bit
									PORTL |= (1<<PIN1);

									// MOTOR 2 CLOCKWISE = forward
									// delete Motor2 A bit
									PORTL &= ~(1<<PIN2);
									// set Motor2 B bit
									PORTL |= (1<<PIN3);

									// MOTOR 3 CLOCKWISE = forward
									// delete Motor3 A bit
									PORTL &= ~(1<<PIN6);
									// set Motor3 B bit
									PORTL |= (1<<PIN7);

									// MOTOR 4 CLOCKWISE = forward
									// delete Motor4 A bit
									PORTD &= ~(1<<PIN6);
									// set Motor4 B bit
									PORTD |= (1<<PIN7);

									// answer with "ok"
									emit answer("*bgo#");
								}
								else
								// BOTFORWARD = "bot drive forward"
								if (commandString == "*bdf#")
								{
									// MOTOR 1 CLOCKWISE = forward
									// delete Motor1 A bit
									PORTL &= ~(1<<PIN0);
									// set Motor1 B bit
									PORTL |= (1<<PIN1);

									// MOTOR 2 CLOCKWISE = forward
									// delete Motor2 A bit
									PORTL &= ~(1<<PIN2);
									// set Motor2 B bit
									PORTL |= (1<<PIN3);

									// MOTOR 3 CLOCKWISE = forward
									// delete Motor3 A bit
									PORTL &= ~(1<<PIN6);
									// set Motor3 B bit
									PORTL |= (1<<PIN7);

									// MOTOR 4 CLOCKWISE = forward
									// delete Motor4 A bit
									PORTD &= ~(1<<PIN6);
									// set Motor4 B bit
									PORTD |= (1<<PIN7);

									// answer with "ok"
									emit answer("*bdf#");
								}
								else
								// BOTBACKWARD = "bot drive backward"
								if (commandString == "*bdb#")
								{
									// MOTOR 1 COUNTERCLOCKWISE = backward
									// set Motor1 A bit
									PORTL |= (1<<PIN0);
									// delete Motor1 B bit
									PORTL &= ~(1<<PIN1);

									// MOTOR 2 COUNTERCLOCKWISE = backward
									// set Motor2 A bit
									PORTL |= (1<<PIN2);
									// delete Motor2 B bit
									PORTL &= ~(1<<PIN3);

									// MOTOR 3 COUNTERCLOCKWISE = backward
									// set Motor3 A bit
									PORTL |= (1<<PIN6);
									// delete Motor3 B bit
									PORTL &= ~(1<<PIN7);

									// MOTOR 4 COUNTERCLOCKWISE = backward
									// set Motor4 A bit
									PORTD |= (1<<PIN6);
									// delete Motor4 B bit
									PORTD &= ~(1<<PIN7);

									// answer with "ok"
									emit answer("*bdb#");
								}
								else
								// BOTLEFT = "bot drive left"
								if (commandString == "*bdl#")
								{
									// MOTOR 1 COUNTERCLOCKWISE = backward
									// set Motor1 A bit
									PORTL |= (1<<PIN0);
									// delete Motor1 B bit
									PORTL &= ~(1<<PIN1);

									// MOTOR 2 CLOCKWISE = forward
									// delete Motor2 A bit
									PORTL &= ~(1<<PIN2);
									// set Motor2 B bit
									PORTL |= (1<<PIN3);

									// MOTOR 3 CLOCKWISE = forward
									// delete Motor3 A bit
									PORTL &= ~(1<<PIN6);
									// set Motor3 B bit
									PORTL |= (1<<PIN7);

									// MOTOR 4 COUNTERCLOCKWISE = backward
									// set Motor4 A bit
									PORTD |= (1<<PIN6);
									// delete Motor4 B bit
									PORTD &= ~(1<<PIN7);

									// answer with "ok"
									emit answer("*bdl#");
								}
								else
								// BOTRIGHT = "bot drive right"
								if (commandString == "*bdr#")
								{
									// MOTOR 1 CLOCKWISE = forward
									// delete Motor1 A bit
									PORTL &= ~(1<<PIN0);
									// set Motor1 B bit
									PORTL |= (1<<PIN1);

									// MOTOR 2 COUNTERCLOCKWISE = backward
									// set Motor2 A bit
									PORTL |= (1<<PIN2);
									// delete Motor2 B bit
									PORTL &= ~(1<<PIN3);

									// MOTOR 3 COUNTERCLOCKWISE = backward
									// set Motor3 A bit
									PORTL |= (1<<PIN6);
									// delete Motor3 B bit
									PORTL &= ~(1<<PIN7);

									// MOTOR 4 CLOCKWISE = forward
									// delete Motor4 A bit
									PORTD &= ~(1<<PIN6);
									// set Motor4 B bit
									PORTD |= (1<<PIN7);

									// answer with "ok"
									emit answer("*bdr#");
								}
								else
								// TURNLEFT = "bot turn left"
								if (commandString == "*btl#")
								{
									// MOTOR 1 CLOCKWISE = forward
									// delete Motor1 A bit
									PORTL &= ~(1<<PIN0);
									// set Motor1 B bit
									PORTL |= (1<<PIN1);

									// MOTOR 2 COUNTERCLOCKWISE = backward
									// set Motor2 A bit
									PORTL |= (1<<PIN2);
									// delete Motor2 B bit
									PORTL &= ~(1<<PIN3);

									// MOTOR 3 CLOCKWISE = forward
									// delete Motor3 A bit
									PORTL &= ~(1<<PIN6);
									// set Motor3 B bit
									PORTL |= (1<<PIN7);

									// MOTOR 4 COUNTERCLOCKWISE = backward
									// set Motor4 A bit
									PORTD |= (1<<PIN6);
									// delete Motor4 B bit
									PORTD &= ~(1<<PIN7);

									// answer with "ok"
									emit answer("*btl#");
								}
								else
								// TURNRIGHT = "bot turn right"
								if (commandString == "*btr#")
								{
									// MOTOR 1 COUNTERCLOCKWISE = backward
									// set Motor1 A bit
									PORTL |= (1<<PIN0);
									// delete Motor1 B bit
									PORTL &= ~(1<<PIN1);

									// MOTOR 2 CLOCKWISE = forward
									// delete Motor2 A bit
									PORTL &= ~(1<<PIN2);
									// set Motor2 B bit
									PORTL |= (1<<PIN3);

									// MOTOR 3 COUNTERCLOCKWISE = backward
									// set Motor3 A bit
									PORTL |= (1<<PIN6);
									// delete Motor3 B bit
									PORTL &= ~(1<<PIN7);

									// MOTOR 4 CLOCKWISE = forward
									// delete Motor4 A bit
									PORTD &= ~(1<<PIN6);
									// set Motor4 B bit
									PORTD |= (1<<PIN7);

									// answer with "ok"
									emit answer("*btr#");
								}
								else
								// SET SERVO 1
								if (commandString.startsWith("*sv1", Qt::CaseInsensitive))
								{
											 // change first chars for upcoming string conversion
											 stringbuffer[0] = '0';
											 stringbuffer[1] = '0';
											 stringbuffer[2] = '0';
											 stringbuffer[3] = '0';
											 // get value from string
											 servoPosition = atoi(stringbuffer);
											 if (servoPosition == 0)
											 {
												 // stop PWM for servos 1, 2, 3 (due to same Atmel ports)
												 servoPWM123isOFF = 1;
												 stopPWMServo(1);
											 }
											 else
											 {
												 // turn on PWM if it was turned off before
												 if (servoPWM123isOFF == 1)
												 {
													 servoPWM123isOFF = 0;
													 // start PWM for servos 1, 2, 3 (due to same Atmel ports)
													 startPWMServo(1);
												 }
												 // set PWM
												 setServoPosition(1, servoPosition);
											 }
											 // answer with "ok"
											 emit answer("*sv1#");
								}
								else
								// SET SERVO 2
								if (commandString.startsWith("*sv2", Qt::CaseInsensitive))
								{
											 // change first chars for upcoming string conversion
											 stringbuffer[0] = '0';
											 stringbuffer[1] = '0';
											 stringbuffer[2] = '0';
											 stringbuffer[3] = '0';
											 // get value from string
											 servoPosition = atoi(stringbuffer);
											 if (servoPosition == 0)
											 {
												 // stop PWM for servos 1, 2, 3 (due to same Atmel ports)
												 servoPWM123isOFF = 1;
												 stopPWMServo(2);
											 }
											 else
											 {
												 // turn on PWM if it was turned off before
												 if (servoPWM123isOFF == 1)
												 {
													 servoPWM123isOFF = 0;
													 // start PWM for servos 1, 2, 3 (due to same Atmel ports)
													 startPWMServo(2);
												 }
												 // set PWM
												 setServoPosition(2, servoPosition);
											 }
											 // answer with "ok"
											 emit answer("*sv2#");
										 }
										 else
										 // SET SERVO 3
										 if (commandString.startsWith("*sv3", Qt::CaseInsensitive))
										 {
											 // change first chars for upcoming string conversion
											 stringbuffer[0] = '0';
											 stringbuffer[1] = '0';
											 stringbuffer[2] = '0';
											 stringbuffer[3] = '0';
											 // get value from string
											 servoPosition = atoi(stringbuffer);
											 if (servoPosition == 0)
											 {
												 // stop PWM for servos 1, 2, 3 (due to same Atmel ports)
												 servoPWM123isOFF = 1;
												 stopPWMServo(3);
											 }
											 else
											 {
												 // turn on PWM if it was turned off before
												 if (servoPWM123isOFF == 1)
												 {
													 servoPWM123isOFF = 0;
													 // start PWM for servos 1, 2, 3 (due to same Atmel ports)
													 startPWMServo(3);
												 }
												 // set PWM
												 setServoPosition(3, servoPosition);
											 }
											 // answer with "ok"
											 emit answer("*sv3#");
								}
								else
								// SET SERVO 4
								if (commandString.startsWith("*sv4", Qt::CaseInsensitive))
								{
											 // change first chars for upcoming string conversion
											 stringbuffer[0] = '0';
											 stringbuffer[1] = '0';
											 stringbuffer[2] = '0';
											 stringbuffer[3] = '0';
											 // get value from string
											 servoPosition = atoi(stringbuffer);
											 if (servoPosition == 0)
											 {
												 // stop PWM for servos 4
												 servoPWM4isOFF = 1;
												 stopPWMServo(4);
											 }
											 else
											 {
												 // turn on PWM if it was turned off before
												 if (servoPWM4isOFF == 1)
												 {
													 servoPWM4isOFF = 0;
													 // start PWM for servo 4
													 startPWMServo(4);
												 }
												 // set PWM
												 setServoPosition(4, servoPosition);
											 }
											 // answer with "ok"
											 emit answer("*sv4#");
								}
								else
								// SET SERVO 5
								if (commandString.startsWith("*sv5", Qt::CaseInsensitive))
								{
											 // change first chars for upcoming string conversion
											 stringbuffer[0] = '0';
											 stringbuffer[1] = '0';
											 stringbuffer[2] = '0';
											 stringbuffer[3] = '0';
											 // get value from string
											 servoPosition = atoi(stringbuffer);
											 if (servoPosition == 0)
											 {
												 // stop PWM for servos 5, 6 (due to same Atmel ports)
												 servoPWM56isOFF = 1;
												 stopPWMServo(5);
											 }
											 else
											 {
												 // turn on PWM if it was turned off before
												 if (servoPWM56isOFF == 1)
												 {
													 servoPWM56isOFF = 0;
													 // start PWM for servos 5, 6 (due to same Atmel ports)
													 startPWMServo(5);
												 }
												 // set PWM
												 setServoPosition(5, servoPosition);
											 }
											 // answer with "ok"
											 emit answer("*sv5#");
								}
								else
								// SET SERVO 6
								if (commandString.startsWith("*sv6", Qt::CaseInsensitive))
								{
											 // change first chars for upcoming string conversion
											 stringbuffer[0] = '0';
											 stringbuffer[1] = '0';
											 stringbuffer[2] = '0';
											 stringbuffer[3] = '0';
											 // get value from string
											 servoPosition = atoi(stringbuffer);
											 if (servoPosition == 0)
											 {
												 // stop PWM for servos 5, 6 (due to same Atmel ports)
												 servoPWM56isOFF = 1;
												 stopPWMServo(6);
											 }
											 else
											 {
												 // turn on PWM if it was turned off before
												 if (servoPWM56isOFF == 1)
												 {
													 servoPWM56isOFF = 0;
													 // start PWM for servos 5, 6 (due to same Atmel ports)
													 startPWMServo(6);
												 }
												 // set PWM
												 setServoPosition(6, servoPosition);
											 }
											 // answer with "ok"
											 emit answer("*sv6#");
								}

								//====================================================================
								//====================================================================
								//====================================================================
								*/


			else
			{
				//-----------------------------------------
				// command complete (*...#), but unknown!
				//-----------------------------------------
				emit message("++ Unknown Atmel command string! ++");

				// let the thread know, that the (next upcoming) Atmel command is not complete (wait for the next command!)
				commandComplete = false;
				commandString.clear();
			}

		} // simulation = false  &&  robot is on  && command complete

		if (simulationMode)
		{
			// FIXME: here or where, when simulating?????  send an optical heartbeat signal to the GUI
			// TODO: now we're sleeping 500ms because ww're only simulating. Is this okay????
			msleep(500);
			heartbeatToggle = !heartbeatToggle;

			emit heartbeat(heartbeatToggle);
		}

	}
	stopped = false;
}


void SimulationThread::setSimulationMode(bool state)
{
	simulationMode = state;

	// fill array with some nice values
	if (simulationMode == true)
	{
		// TODO: fill with content
	}
	else
	{
		// TODO: fill with content
	}
}


void SimulationThread::setRobotState(bool state)
{
	// store the state within this class
	robotState = state;

	emit message(QString("Robot state set to %1 in %2").arg(state).arg(className));
}


void SimulationThread::sendUInt(uint16_t value)
{
	QString stringbuffer;


	// start the answer string to send with a '*'
	stringbuffer[0] = starter;

	// convert int to ascii (to Basis 10)
	// (but don't overwrite the first char which is the 'starter' *.)
	QString num = QString().setNum(value, 10);
	stringbuffer.append(num);

	// add m string terminator '#' at the end of the buffer
	stringbuffer[stringbuffer.length()] = terminator;

	// String mit \0 terminieren
	// stringbuffer[length+1] = 0;

	// send answer
	emit answer(stringbuffer);
}


uint16_t SimulationThread::readADC(unsigned char channel)
{
	if (channel==SENSOR7) // 12V sensor
		return (uint16_t) (57.000 * 12);

	if (channel==SENSOR8) // 24V sensor
		return (uint16_t) (36.125 * 24);

	if (channel == SENSORMOTOR1)
		return (uint16_t) (1000 / 29); // 1000 mA

	if (channel == SENSORMOTOR2)
		return (uint16_t) (1000 / 29); // 1000 mA

	emit message("+++ Sensor not implemented in readADC! +++");
	return 0;
}


void SimulationThread::sendToAtmel(QString string)
{
	// let the thread know, that the (next upcoming) Atmel command is not complete (wait for the next command!)
	commandComplete = false;
	commandString.clear();


	// this should be always the case
	if (string.startsWith(starter))
		string.remove(0, 1);

	// this should be always the case
	if (string.endsWith(terminator))
		string.remove(string.length()-1, 1);

	mutex->lock();

	if (interface1->sendString(string) == false)
		qDebug("ERROR sending string @ SimulationThread::sendToAtmel()."); /// @todo react on errors within this thread. Use a signal which stops the thread or so.

	mutex->unlock();
}


void SimulationThread::commandReceived(QString string)
{
	commandComplete = true;
	commandString = string;
}
