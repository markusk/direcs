/*************************************************************************
 *   Copyright (C) 2008 by Markus Knapp                                  *
 *   mrs @ direcs.de                                                     *
 *                                                                       *
 *   This file is part of mrs.                                           *
 *                                                                       *
 *   mrs is free software: you can redistribute it and/or modify it      *
 *   under the terms of the GNU General Public License as published      *
 *   by the Free Software Foundation, version 3 of the License.          *
 *                                                                       *
 *   mrs is distributed in the hope that it will be useful,              *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of      *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the        *
 *   GNU General Public License for more details.                        *
 *                                                                       *
 *   You should have received a copy of the GNU General Public License   *
 *   along with mrs. If not, see <http://www.gnu.org/licenses/>.         *
 *                                                                       *
 *************************************************************************/

#include "simThread.h"

SimThread::SimThread(InterfaceAvr *i, QMutex *m)
{
	stopped = false;
	simulationMode = false;
	
	// copy the pointer from the original object
	interface1 = i;
	mutex = m;
}


SimThread::~SimThread()
{
}


void SimThread::stop()
{
	stopped = true;
}


void SimThread::run()
{
	
	
	//
	//  start "threading"...
	//
	while (!stopped)
	{
		// let the thread sleep some time
		// for having more time for the other threads
		msleep(THREADSLEEPTIME);
		
		// Lock the mutex. If another thread has locked the mutex then this call will block until that thread has unlocked it.
		mutex->lock();
			
/*
		//------------------------------------------------------
		// read value from sensor 1
		//------------------------------------------------------
		if (interface1->sendChar(READ_SENSOR_1) == false)
		{
			// Unlock the mutex.
			mutex->unlock();
			qDebug("ERROR s1 sending to serial port (SimThread)");
			return;
		}
		
		// receive the 16 Bit answer from the MC
		if (interface1->receiveInt(&value) == false)
		{
			// Unlock the mutex.
			mutex->unlock();
		}
*/
		//-----------------------------------------
		// waiting "endlessly" for serial data...
		// react on the received command
		//--------------------------------
		if (interface1->receiveChar(&character) == false)
		{
			// wait...
		}
		
		// convert to int (for switch/case)
		value = character;
		qDebug("char received");
		
		switch (value)
		{
			case INIT:
				if (redLEDtoggle == 0)
				{
					redLEDtoggle = 1;
					// yelow LED on (low active!)
					PORTC &= ~(1<<PIN0);
				}
				else
				{
					redLEDtoggle = 0;
					// yelow LED off (low active!)
					PORTC |= (1<<PIN0);
				}
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
				PORTC &= ~(1<<PIN1);
				/*
				setServoPosition(1, 17); // <- exact position now in the mrs.ini!
				setServoPosition(2, 19); // <- exact position now in the mrs.ini!
				setServoPosition(3, 23); // <- exact position now in the mrs.ini!
				setServoPosition(4, 19); // <- exact position now in the mrs.ini!
				setServoPosition(5, 19); // <- exact position now in the mrs.ini!
				setServoPosition(6, 22); // <- exact position now in the mrs.ini!
				*/
				// "answer" with "@" [Ascii Dezimal @ = 64]
				// this answer is used to see if the robot is "on"
					UsartTransmit( (uint8_t)(64) );
					break;
			
			//-------------------------------
			case READ_SENSOR_1:
				// read value from the analog digital converter (ADC)
				value = readADC(SENSOR1);
				// send MS-Byte
				UsartTransmit( (uint8_t)(value >> 8) );
				// send LS-Byte
				UsartTransmit( (uint8_t)(value) );
				break;

			case READ_SENSOR_2:
				// read value from the analog digital converter (ADC)
				value = readADC(SENSOR2);
				// send MS-Byte
				UsartTransmit( (uint8_t)(value >> 8) );
				// send LS-Byte
				UsartTransmit( (uint8_t)(value) );
				break;

			case READ_SENSOR_3:
				// read value from the analog digital converter (ADC)
				value = readADC(SENSOR3);
				// send MS-Byte
				UsartTransmit( (uint8_t)(value >> 8) );
				// send LS-Byte
				UsartTransmit( (uint8_t)(value) );
				break;

			case READ_SENSOR_4:
				// read value from the analog digital converter (ADC)
				value = readADC(SENSOR4);
				// send MS-Byte
				UsartTransmit( (uint8_t)(value >> 8) );
				// send LS-Byte
				UsartTransmit( (uint8_t)(value) );
				break;

			case READ_SENSOR_5:
				// read value from the analog digital converter (ADC)
				value = readADC(SENSOR5);
				// send MS-Byte
				UsartTransmit( (uint8_t)(value >> 8) );
				// send LS-Byte
				UsartTransmit( (uint8_t)(value) );
				break;

			case READ_SENSOR_6:
				// read value from the analog digital converter (ADC)
				value = readADC(SENSOR6);
				// send MS-Byte
				UsartTransmit( (uint8_t)(value >> 8) );
				// send LS-Byte
				UsartTransmit( (uint8_t)(value) );
				break;

			case READ_SENSOR_7:
				// read value from the analog digital converter (ADC)
				value = readADC(SENSOR7);
				// send MS-Byte
				UsartTransmit( (uint8_t)(value >> 8) );
				// send LS-Byte
				UsartTransmit( (uint8_t)(value) );
				break;

			case READ_SENSOR_8:
				// read value from the analog digital converter (ADC)
				value = readADC(SENSOR8);
				// send MS-Byte
				UsartTransmit( (uint8_t)(value >> 8) );
				// send LS-Byte
				UsartTransmit( (uint8_t)(value) );
				break;
			
			case READ_SENSOR_16:
				// ultra sonic sensor !!
				value = ultraschall_messung();
				// send MS-Byte
				UsartTransmit( (uint8_t)(value >> 8) );
				// send LS-Byte
				UsartTransmit( (uint8_t)(value) );
				break;
			
			case READ_MOTOR_SENSOR1:
				// motor sensor
				value = readADC(SENSORMOTOR1);
				// send MS-Byte
				UsartTransmit( (uint8_t)(value >> 8) );
				// send LS-Byte
				UsartTransmit( (uint8_t)(value) );
				break;
			
			case READ_MOTOR_SENSOR2:
				// motor sensor
				value = readADC(SENSORMOTOR2);
				// send MS-Byte
				UsartTransmit( (uint8_t)(value >> 8) );
				// send LS-Byte
				UsartTransmit( (uint8_t)(value) );
				break;
		
			case READ_MOTOR_DISTANCE1:
				// driven distance of motor 1 (encoder)
				value = rightDistanceCounter;
				// send MS-Byte
				UsartTransmit( (uint8_t)(value >> 8) );
				// send LS-Byte
				UsartTransmit( (uint8_t)(value) );
				break;
		
			case READ_MOTOR_DISTANCE2:
				// driven distance of motor 2 (encoder)
				value = leftDistanceCounter;
				// send MS-Byte
				UsartTransmit( (uint8_t)(value >> 8) );
				// send LS-Byte
				UsartTransmit( (uint8_t)(value) );
				break;
		
			case RESET_MOTOR_DISTANCE1:
				// reset driven distance of motor 1 (encoder)
				rightDistanceCounter = 0;
				rightWheelCounter = 0;
				break;
		
			case RESET_MOTOR_DISTANCE2:
				// reset driven distance of motor 1 (encoder)
				leftDistanceCounter = 0;
				leftWheelCounter = 0;
				break;

			//-------------------------------
			case MOTOR1_OFF:
				// delete Motor1 A bit
				PORTL &= ~(1<<PIN0);
				// delete Motor1 B bit
				PORTL &= ~(1<<PIN1);
				break;

			case MOTOR1_CLOCKWISE:
				// delete Motor1 A bit
				PORTL &= ~(1<<PIN0);
				// set Motor1 B bit
				PORTL |= (1<<PIN1);
				break;

			case MOTOR1_COUNTERCLOCKWISE:
				// set Motor1 A bit
				PORTL |= (1<<PIN0);
				// delete Motor1 B bit
				PORTL &= ~(1<<PIN1);
				break;

			case MOTOR1_SPEED_SET:
				// wait for the (second) value to set the pwm!
				value = UsartReceive();
				setPWMwidth(1, value);
				break;

			//-------------------------------
			case MOTOR2_OFF:
				// delete Motor2 A bit
				PORTL &= ~(1<<PIN2);
				// delete Motor2 B bit
				PORTL &= ~(1<<PIN3);
				break;

			case MOTOR2_CLOCKWISE:
				// delete Motor2 A bit
				PORTL &= ~(1<<PIN2);
				// set Motor2 B bit
				PORTL |= (1<<PIN3);
				break;

				case MOTOR2_COUNTERCLOCKWISE:
				// set Motor2 A bit
				PORTL |= (1<<PIN2);
				// delete Motor2 B bit
				PORTL &= ~(1<<PIN3);
				break;

			case MOTOR2_SPEED_SET:
				// wait for the (second) value to set the pwm!
				value = UsartReceive();
				setPWMwidth(2, value);
				break;

			//-------------------------------
			case MOTOR3_OFF:
				// delete Motor3 A bit
				PORTL &= ~(1<<PIN6);
				// delete Motor3 B bit
				PORTL &= ~(1<<PIN7);
				break;

			case MOTOR3_CLOCKWISE: // cam pan R
				// only, when end switch is clear
				if ( bit_is_clear(PINK,PIN2) )
				{
					// delete Motor3 A bit
					PORTL &= ~(1<<PIN6);
					// set Motor3 B bit
					PORTL |= (1<<PIN7);
				}
				break;

			case MOTOR3_COUNTERCLOCKWISE: // cam pan L
				// only, when end switch is clear
				if ( bit_is_clear(PINK,PIN3) )
				{
					// set Motor3 A bit
					PORTL |= (1<<PIN6);
					// delete Motor3 B bit
					PORTL &= ~(1<<PIN7);
				}
				break;

			case MOTOR3_SPEED_SET:
				// wait for the (second) value to set the pwm!
				value = UsartReceive();
				setPWMwidth(3, value);
				break;

			//-------------------------------
			case MOTOR4_OFF:
				// delete Motor4 A bit
				PORTD &= ~(1<<PIN6);
				// delete Motor4 B bit
				PORTD &= ~(1<<PIN7);
				break;

			case MOTOR4_CLOCKWISE: // cam tilt top
				// only, when end switch is clear
				if ( bit_is_clear(PINK,PIN1) )
				{
					// delete Motor4 A bit
					PORTD &= ~(1<<PIN6);
					// set Motor4 B bit
					PORTD |= (1<<PIN7);
				}
				break;

			case MOTOR4_COUNTERCLOCKWISE: // cam tilt bottom
				if ( bit_is_clear(PINK,PIN0) )
				{
					// set Motor4 A bit
					PORTD |= (1<<PIN6);
					// delete Motor4 B bit
					PORTD &= ~(1<<PIN7);
				}
				break;

			case MOTOR4_SPEED_SET:
				// wait for the (second) value to set the pwm!
				value = UsartReceive();
				setPWMwidth(4, value);
				break;

			//-------------------------------
			case SET_SERVO1:
				// wait for the (second) value to set the pwm!
				value = UsartReceive();
				setServoPosition(1, value);
				break;

			case SET_SERVO2:
				// wait for the (second) value to set the pwm!
				value = UsartReceive();
				setServoPosition(2, value);
				break;

			case SET_SERVO3:
				// wait for the (second) value to set the pwm!
				value = UsartReceive();
				setServoPosition(3, value);
				break;


			case SET_SERVO4:
				// wait for the (second) value to set the pwm!
				value = UsartReceive();
				setServoPosition(4, value);
				break;


			case SET_SERVO5:
				// wait for the (second) value to set the pwm!
				value = UsartReceive();
				setServoPosition(5, value);
				break;


			case SET_SERVO6:
				// wait for the (second) value to set the pwm!
				value = UsartReceive();
				setServoPosition(6, value);
				break;

			//-------------------------------
			case FLASHLIGHT_ON:
				// set flashlight bit
				PORTC |= (1<<PIN1);
				PORTC &= ~(1<<PIN0); // < yellow led
				break;
			case FLASHLIGHT_OFF:
				// delete flashlight bit
				PORTC &= ~(1<<PIN1);
				PORTC |= (1<<PIN0); // < yellow led
				break;

			//-------------------------------
			case READ_CONTACT1:
				// contact cam tilt R/BOTTOM
				// send 1 Byte (8 bit!)
				UsartTransmit( (uint8_t) bit_is_set(PINK,PIN3) );
				break;

			case READ_CONTACT2:
				// contact cam tilt L/TOP
				// send 1 Byte (8 bit!)
				UsartTransmit( (uint8_t) bit_is_set(PINK,PIN2) );
				break;

			case READ_CONTACT3:
				// contact cam pan R
				// send 1 Byte (8 bit!)
				UsartTransmit( (uint8_t) bit_is_set(PINK,PIN1) );
				break;

			case READ_CONTACT4:
				// contact cam pan L
				// send 1 Byte (8 bit!)
				UsartTransmit( (uint8_t) bit_is_set(PINK,PIN0) );
				break;
	
			default:
				//
				// red 'traffic' LED
				//
				if (redLEDtoggle == 0)
				{
					redLEDtoggle = 1;
					// yelow LED on (low active!)
					PORTC &= ~(1<<PIN0);
				}
				else
				{
					redLEDtoggle = 0;
					// yelow LED off (low active!)
					PORTC |= (1<<PIN0);
				}
				break;
			} // switch
	
	
			// Unlock the mutex.
			mutex->unlock();

	
		//====================================================================
		//  e m i t  Signal
		//====================================================================
		//emit sensorDataComplete();
	}
	stopped = false;
}
