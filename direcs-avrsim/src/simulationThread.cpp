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

#include "simulationThread.h"

SimulationThread::SimulationThread(InterfaceAvr *i, QMutex *m)
{
	stopped = false;
	simulationMode = false;

	// copy the pointer from the original object
	interface1 = i;
	mutex = m;

	robotState = ON; // Wer're thinking positive. The robot is ON untill whe know nothing other. :-)
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
	unsigned char character = 0;
	bool heartbeatToggle = false;
	const int maxStringLength = 32; /// @sa direcs-avr/usart.h: uart_buffer_size
	static int charCounter = 0;
	QString receiveString;
	QString commandString;
	bool stringStarted = false;
	bool commandCompleted = false;
	static bool redLEDtoggle = false;


	//
	//  start "threading"...
	//
	while (!stopped)
	{
		// let the thread sleep some time
		// for having more time for the other threads
		msleep(THREADSLEEPTIME);


		if ( (robotState == ON) && (simulationMode == false) )
		{
			//--------------------------
			// wait for chars from Atmel
			//--------------------------
			emit message("Waiting for Atmel command string to start...");

			while (stopped == false)
			{
				// Lock the mutex. If another thread has locked the mutex then this call will block until that thread has unlocked it.
				mutex->lock();
				while (interface1->charsAvailable() == false)
				{
					if (stopped)
						break;
				}

				// get char
				interface1->receiveChar(&character);
				mutex->unlock();


				// toggling the red LED on and off with every received serial commmand
				redLEDtoggle = !redLEDtoggle;
				emit redLED(redLEDtoggle);


				if (commandCompleted == false)
				{
					// serial buffer overflow?
					if (charCounter <= maxStringLength)
					{
						// command string from Atmel starts?
						if (character == starter)
						{
							stringStarted = true;
							commandCompleted = false;

							// start QString
							receiveString.clear();

							// complete command string
							receiveString.append((char *) &character);

							charCounter++;

							emit greenLED(ON);

							// send text with no CR to GUI
							emit message("*", false);

						} // command started
						else
						{
							// command string from Atmel completed?
							if (character == terminator)
							{
								// complete command string
								receiveString.append((char *) &character);

								commandCompleted = true;
								stringStarted = false;

								// reset char counter
								charCounter = 0;

								emit greenLED(OFF);

								// send text to GUI
								emit message("#");


								//-----------------------------------------------
								emit message(QString("Atmel command string: %1.").arg(receiveString));
								//-----------------------------------------------

								// copy string for command check
								commandString = receiveString;

								//-------------------------------------------------
								// reset received string for next upcoming command
								//-------------------------------------------------
								charCounter = 0;
								receiveString.clear();
								stringStarted = false;
								commandCompleted = false;

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
									// answer with "ok"
									// this answer is used to see if the robot is "on"
									mutex->lock();
									interface1->sendString("*ok#");
									mutex->unlock();

									// show string in GUI
									emit answer("*ok#");

// e n a b l e  watchdog!
/// @todo			watchdog(ENABLE);
								} // *re#


								/*

								// SLEEP (and turn off watchdog)
								if (strcmp(stringbuffer, "*sl#") == 0)
								{
									// turn all drive motor bits off (except PWM bits)
									PORTL &= ~(1<<PIN0);
									PORTL &= ~(1<<PIN1);
									PORTL &= ~(1<<PIN2);
									PORTL &= ~(1<<PIN3);
									PORTL &= ~(1<<PIN6);
									PORTL &= ~(1<<PIN7);
									PORTD &= ~(1<<PIN6);
									PORTD &= ~(1<<PIN7);

									// flashlight off
									relais(OFF);

									// red LED off
									redLED(OFF);

									// d i s a b l e  watchdog!
									watchdog(DISABLE);

									// answer
									put_string("*sl#");
								}
										 else
										 // READ_SENSOR_1
										 if (strcmp(stringbuffer, "*s1#") == 0)
										 {
											 // read ADC and send answer over serial port
											 sendUInt( readADC(SENSOR1) );
										 }
										 else
										 // READ_SENSOR_2
										 if (strcmp(stringbuffer, "*s2#") == 0)
										 {
											 // read ADC and send answer over serial port
											 sendUInt( readADC(SENSOR2) );
										 }
										 else
										 // READ_SENSOR_3
										 if (strcmp(stringbuffer, "*s3#") == 0)
										 {
											 // read ADC and send answer over serial port
											 sendUInt( readADC(SENSOR3) );
										 }
										 else
										 // READ_SENSOR_4
										 if (strcmp(stringbuffer, "*s4#") == 0)
										 {
											 // read ADC and send answer over serial port
											 sendUInt( readADC(SENSOR4) );
										 }
										 else
										 // READ_SENSOR_5
										 if (strcmp(stringbuffer, "*s5#") == 0)
										 {
											 // read ADC and send answer over serial port
											 sendUInt( readADC(SENSOR5) );
										 }
										 else
										 // READ_SENSOR_6
										 if (strcmp(stringbuffer, "*s6#") == 0)
										 {
											 // read ADC and send answer over serial port
											 sendUInt( readADC(SENSOR6) );
										 }
										 else
										 // READ_SENSOR_7 (24 V supply)
										 if (strcmp(stringbuffer, "*s7#") == 0)
										 {
											 // read ADC and send answer over serial port
											 sendUInt( readADC(SENSOR7) );
										 }
										 else
										 // READ_SENSOR_8 (12 V supply)
										 if (strcmp(stringbuffer, "*s8#") == 0)
										 {
											 // read ADC and send answer over serial port
											 sendUInt( readADC(SENSOR8) );
										 }
										 else
										 // READ_SENSOR_16
										 // ultra sonic sensor !!
										 if (strcmp(stringbuffer, "*s16#") == 0)
										 {
											 // read ADC and send answer over serial port
											 sendUInt( ultraschall_messung() );
										 }
										 else
										 // READ 3D COMPASS CONNECTION
										 if (strcmp(stringbuffer, "*cc#") == 0)
										 {
											 // check if micromag is connected to Atmel-Board (PB6 = high)
											 if ( bit_is_set(PINB,PIN6) )
											 {
												 put_string("*ok#");
											 }
											 else
											 {
												 // not connected.
												 put_string("*er#");
											 }
										 }
										 else
										 // READ_AXIS_X
										 if (strcmp(stringbuffer, "*cx#") == 0)
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
										 if (strcmp(stringbuffer, "*cy#") == 0)
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
										 if (strcmp(stringbuffer, "*cz#") == 0)
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
										 else
										 // READ_MOTOR_SENSOR1
										 if (strcmp(stringbuffer, "*ms1#") == 0)
										 {
											 // read ADC and send answer over serial port
											 sendUInt( readADC(SENSORMOTOR1) );
										 }
										 else
										 // READ_MOTOR_SENSOR2
										 if (strcmp(stringbuffer, "*ms2#") == 0)
										 {
											 // read ADC and send answer over serial port
											 sendUInt( readADC(SENSORMOTOR2) );
										 }
										 else
										 // READ_MOTOR_DISTANCE1
										 if (strcmp(stringbuffer, "*dd1#") == 0)
										 {
											 // send driven distance of motor 1 encoder over serial port
											 sendUInt( rightDistanceCounter );
										 }
										 else
										 // READ_MOTOR_DISTANCE2
										 if (strcmp(stringbuffer, "*dd2#") == 0)
										 {
											 // send driven distance of motor 2 encoder over serial port
											 sendUInt( leftDistanceCounter );
										 }
										 else
										 // RESET_MOTOR_DISTANCE1
										 if (strcmp(stringbuffer, "*id1#") == 0)
										 {
											 // init driven distance of motor 1 (encoder)
											 rightDistanceCounter = 0;
											 rightWheelCounter = 0;
											 // answer with "ok"
											 put_string("*ok#");
										 }
										 else
										 // RESET_MOTOR_DISTANCE2
										 if (strcmp(stringbuffer, "*id2#") == 0)
										 {
											 // init driven distance of motor 2 (encoder)
											 leftDistanceCounter = 0;
											 leftWheelCounter = 0;
											 // answer with "ok"
											 put_string("*ok#");
										 }
*/
										 else
										 // FLASHLIGHT ON
										 if (commandString == "*f0on#")
										 {
											 relais(ON);
//											 yellowLED(ON);
											 emit answer("*f0on#");
										 }
										 else
										 // FLASHLIGHT OFF
											 if (commandString == "*f0of#")
										 {
											 relais(OFF);
//											 yellowLED(OFF);
											 emit answer("*f0of#");
										 }
/*
										 else
										 // MOTOR1_OFF
										 if (strcmp(stringbuffer, "*mp1of#") == 0)
										 {
											 // delete Motor1 A bit
											 PORTL &= ~(1<<PIN0);
											 // delete Motor1 B bit
											 PORTL &= ~(1<<PIN1);
											 // answer with "ok"
											 put_string("*mp1of#");
										 }
										 else
										 // MOTOR 1 CLOCKWISE = forward
										 if (strcmp(stringbuffer, "*md1cw#") == 0)
										 {
											 // delete Motor1 A bit
											 PORTL &= ~(1<<PIN0);
											 // set Motor1 B bit
											 PORTL |= (1<<PIN1);

											 // answer with "ok"
											 put_string("*md1cw#");
										 }
										 else
										 // MOTOR 1 COUNTERCLOCKWISE = backward
										 if (strcmp(stringbuffer, "*md1cc#") == 0)
										 {
											 // set Motor1 A bit
											 PORTL |= (1<<PIN0);
											 // delete Motor1 B bit
											 PORTL &= ~(1<<PIN1);

											 // answer with "ok"
											 put_string("*md1cc#");
										 }
										 else
										 // MOTOR1_SPEED_SET
										 if (strncmp(stringbuffer, "*mv1", 4) == 0)
										 {
											 // change first chars for upcoming string conversion
											 stringbuffer[0] = '0';
											 stringbuffer[1] = '0';
											 stringbuffer[2] = '0';
											 stringbuffer[3] = '0';
											 // get value from string and set speed
											 setPWMwidth(1, atoi(stringbuffer));
											 // answer with "ok"
											 put_string("*mv1#");
										 }
										 else
										 // MOTOR2_OFF
										 if (strcmp(stringbuffer, "*mp2of#") == 0)
										 {
											 // delete Motor2 A bit
											 PORTL &= ~(1<<PIN2);
											 // delete Motor2 B bit
											 PORTL &= ~(1<<PIN3);
											 // answer with "ok"
											 put_string("*mp2of#");
										 }
										 else
										 // MOTOR 2 CLOCKWISE = forward
										 if (strcmp(stringbuffer, "*md2cw#") == 0)
										 {
											 // delete Motor2 A bit
											 PORTL &= ~(1<<PIN2);
											 // set Motor2 B bit
											 PORTL |= (1<<PIN3);

											 // answer with "ok"
											 put_string("*md2cw#");
										 }
										 else
										 // MOTOR 2 COUNTERCLOCKWISE = backward
										 if (strcmp(stringbuffer, "*md2cc#") == 0)
										 {
											 // set Motor2 A bit
											 PORTL |= (1<<PIN2);
											 // delete Motor2 B bit
											 PORTL &= ~(1<<PIN3);

											 // answer with "ok"
											 put_string("*md2cc#");
										 }
										 else
										 // MOTOR2_SPEED_SET
										 if (strncmp(stringbuffer, "*mv2", 4) == 0)
										 {
											 // change first chars for upcoming string conversion
											 stringbuffer[0] = '0';
											 stringbuffer[1] = '0';
											 stringbuffer[2] = '0';
											 stringbuffer[3] = '0';
											 // get value from string and set speed
											 setPWMwidth(2, atoi(stringbuffer));
											 // answer with "ok"
											 put_string("*mv2#");
										 }
										 else
										 // MOTOR3_OFF
										 if (strcmp(stringbuffer, "*mp3of#") == 0)
										 {
											 // delete Motor3 A bit
											 PORTL &= ~(1<<PIN6);
											 // delete Motor3 B bit
											 PORTL &= ~(1<<PIN7);
											 // answer with "ok"
											 put_string("*mp3of#");
										 }
										 else
										 // MOTOR 3 CLOCKWISE = forward
										 if (strcmp(stringbuffer, "*md3cw#") == 0)
										 {
											 // delete Motor3 A bit
											 PORTL &= ~(1<<PIN6);
											 // set Motor3 B bit
											 PORTL |= (1<<PIN7);

											 // answer with "ok"
											 put_string("*md3cw#");
										 }
										 else
										 // MOTOR 3 COUNTERCLOCKWISE = backward
										 if (strcmp(stringbuffer, "*md3cc#") == 0)
										 {
											 // set Motor3 A bit
											 PORTL |= (1<<PIN6);
											 // delete Motor3 B bit
											 PORTL &= ~(1<<PIN7);

											 // answer with "ok"
											 put_string("*md3cc#");
										 }
										 else
										 // MOTOR3_SPEED_SET
										 if (strncmp(stringbuffer, "*mv3", 4) == 0)
										 {
											 // change first chars for upcoming string conversion
											 stringbuffer[0] = '0';
											 stringbuffer[1] = '0';
											 stringbuffer[2] = '0';
											 stringbuffer[3] = '0';
											 // get value from string and set speed
											 setPWMwidth(3, atoi(stringbuffer));
											 // answer with "ok"
											 put_string("*mv3#");
										 }
										 else
										 // MOTOR4_OFF
										 if (strcmp(stringbuffer, "*mp4of#") == 0)
										 {
											 // delete Motor4 A bit
											 PORTD &= ~(1<<PIN6);
											 // delete Motor4 B bit
											 PORTD &= ~(1<<PIN7);
											 // answer with "ok"
											 put_string("*mp4of#");
										 }
										 else
										 // MOTOR 4 CLOCKWISE = forward
										 if (strcmp(stringbuffer, "*md4cw#") == 0)
										 {
											 // delete Motor4 A bit
											 PORTD &= ~(1<<PIN6);
											 // set Motor4 B bit
											 PORTD |= (1<<PIN7);

											 // answer with "ok"
											 put_string("*md4cw#");
										 }
										 else
										 // MOTOR 4 COUNTERCLOCKWISE = backward
										 if (strcmp(stringbuffer, "*md4cc#") == 0)
										 {
											 // set Motor4 A bit
											 PORTD |= (1<<PIN6);
											 // delete Motor4 B bit
											 PORTD &= ~(1<<PIN7);

											 // answer with "ok"
											 put_string("*md4cc#");
										 }
										 else
										 // MOTOR4_SPEED_SET
										 if (strncmp(stringbuffer, "*mv4", 4) == 0)
										 {
											 // change first chars for upcoming string conversion
											 stringbuffer[0] = '0';
											 stringbuffer[1] = '0';
											 stringbuffer[2] = '0';
											 stringbuffer[3] = '0';
											 // get value from string and set speed
											 setPWMwidth(4, atoi(stringbuffer));
											 // answer with "ok"
											 put_string("*mv4#");
										 }
										 else
										 // SPEED_SET_ALLMOTORS
										 if (strncmp(stringbuffer, "*mv0", 4) == 0)
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
											 put_string("*mv0#");
										 }
										 else
										 // BOTSTOP
										 if (strcmp(stringbuffer, "*bst#") == 0)
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
											 put_string("*bst#");
										 }
										 else
										 // BOTWAIT
										 if (strcmp(stringbuffer, "*bwa#") == 0)
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
											 put_string("*bwa#");
										 }
										 else
										 // BOTSTART = "bot go"
										 if (strcmp(stringbuffer, "*bgo#") == 0)
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
											 put_string("*bgo#");
										 }
										 else
										 // BOTFORWARD = "bot drive forward"
										 if (strcmp(stringbuffer, "*bdf#") == 0)
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
											 put_string("*bdf#");
										 }
										 else
										 // BOTBACKWARD = "bot drive backward"
										 if (strcmp(stringbuffer, "*bdb#") == 0)
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
											 put_string("*bdb#");
										 }
										 else
										 // BOTLEFT = "bot drive left"
										 if (strcmp(stringbuffer, "*bdl#") == 0)
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
											 put_string("*bdl#");
										 }
										 else
										 // BOTRIGHT = "bot drive right"
										 if (strcmp(stringbuffer, "*bdr#") == 0)
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
											 put_string("*bdr#");
										 }
										 else
										 // TURNLEFT = "bot turn left"
										 if (strcmp(stringbuffer, "*btl#") == 0)
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
											 put_string("*btl#");
										 }
										 else
										 // TURNRIGHT = "bot turn right"
										 if (strcmp(stringbuffer, "*btr#") == 0)
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
											 put_string("*btr#");
										 }
										 else
										 // SET SERVO 1
										 if (strncmp(stringbuffer, "*sv1", 4) == 0)
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
											 put_string("*sv1#");
										 }
										 else
										 // SET SERVO 2
										 if (strncmp(stringbuffer, "*sv2", 4) == 0)
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
											 put_string("*sv2#");
										 }
										 else
										 // SET SERVO 3
										 if (strncmp(stringbuffer, "*sv3", 4) == 0)
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
											 put_string("*sv3#");
										 }
										 else
										 // SET SERVO 4
										 if (strncmp(stringbuffer, "*sv4", 4) == 0)
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
											 put_string("*sv4#");
										 }
										 else
										 // SET SERVO 5
										 if (strncmp(stringbuffer, "*sv5", 4) == 0)
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
											 put_string("*sv5#");
										 }
										 else
										 // SET SERVO 6
										 if (strncmp(stringbuffer, "*sv6", 4) == 0)
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
											 put_string("*sv6#");
										 }


										//====================================================================
										//====================================================================
										//====================================================================
								*/


								else
								{
									charCounter = 0;
									commandCompleted = false;
									stringStarted = false;

									// delete string!
									receiveString.clear();

									emit message("+++ Unknown Atmel command string! +++");
								}



							} // terminator?
							else
							{
								// we are in the middle of the command string...
								charCounter++;

								commandCompleted = false;

								// build command string
								receiveString.append((char *) &character);

								// send char to GUI
								emit message(QString("%1").arg((char *) &character), false);
							}

						}
					} // buffer okay
					else
					{
						// string 'buffer overflow'

						charCounter = 0;
						commandCompleted = false;
						stringStarted = false;

						// delete string!
						receiveString.clear();

						emit message("+++ String size exceeded. Discarding received chars. +++");
						emit message("Waiting for Atmel command string to start...");
					}
				} // commmand completed


			} // thread runs




		} // simulation = false

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
}
