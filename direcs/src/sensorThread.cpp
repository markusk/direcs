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

#include "sensorThread.h"

SensorThread::SensorThread(InterfaceAvr *i, QMutex *m)
{
	stopped = false;
	simulationMode = false;

	// copy the pointer from the original object
	interface1 = i;
	mutex = m;

	// Array for storing the measured values from the infrared sensors
	iRSensorValue[SENSOR1] = 0;
	iRSensorValue[SENSOR2] = 0;
 	iRSensorValue[SENSOR3] = 0;
	iRSensorValue[SENSOR4] = 0;
	iRSensorValue[SENSOR5] = 0;
	iRSensorValue[SENSOR6] = 0;
// 	iRSensorValue[SENSOR7] = 0; -> now voltage 12 V (measuring the power supply / accumulators), sensor 1 !!
// 	iRSensorValue[SENSOR8] = 0; -> now voltage 24 V (measuring the power supply / accumulators)  sensor 2 !!
	
	// Array for storing the measured voltage values
	voltageSensorValue[VOLTAGESENSOR1] = 0;
	voltageSensorValue[VOLTAGESENSOR2] = 0;

	// initialisation
	for (int i=0; i<USSENSORARRAYSIZE; i++)
	{
		usSensorValue[i] = 0;
	}

	// initialisation
	for (int i=0; i<MOTORSENSORARRAYSIZE; i++)
	{
		motorSensorValue[i] = 0;
	}

	// Array for storing the measured contact values
	// initialisation
	for (int i=0; i<CONTACTARRAYSIZE; i++)
	{
		contactValue[i] = 0;
	}

	// initialisation
	for (int i=0; i<DRIVENDISTANCEARRAYSIZE; i++)
	{
		drivenDistance[i] = 0;
	}

	// init the 3D compass values
	xAxis = 0.0;
	yAxis = 0.0;
	zAxis = 0.0;


	// These are the measured values from the AD-Conversion from the IR-Sensors
	// Add 8 to the index, then you have the distance in centimeters (cm)
	//
	// Example:
	//
	// Value from AD converter = 1011
	// Index = 0
	//
	// 0 + 8 = 8 cm
	//
	// 8 cm = 1011
	//
	//
	// This means, that the sensor measures from 8 centimeters (0+8) to 47 (39+8) centimeters.
	//
	iRDistance[0]  = 1011;
	iRDistance[1]  = 993;
	iRDistance[2]  = 926;
	iRDistance[3]  = 854;
	iRDistance[4]  = 789;
	iRDistance[5]  = 733;
	iRDistance[6]  = 683;
	iRDistance[7]  = 639;
	iRDistance[8]  = 605;
	iRDistance[9]  = 577;
	iRDistance[10] = 547;
	iRDistance[11] = 520;
	iRDistance[12] = 497;
	iRDistance[13] = 470;
	iRDistance[14] = 452;
	iRDistance[15] = 430;
	iRDistance[16] = 418;
	iRDistance[17] = 400;
	iRDistance[18] = 388;
	iRDistance[19] = 374;
	iRDistance[20] = 359;
	iRDistance[21] = 343;
	iRDistance[22] = 338;
	iRDistance[23] = 326;
	iRDistance[24] = 313;
	iRDistance[25] = 306;
	iRDistance[26] = 299;
	iRDistance[27] = 291;
	iRDistance[28] = 277;
	iRDistance[29] = 272;
	iRDistance[30] = 266;
	iRDistance[31] = 262;
	iRDistance[32] = 253;
	iRDistance[33] = 249;
	iRDistance[34] = 240;
	iRDistance[35] = 234;
	iRDistance[36] = 230;
	iRDistance[37] = 227;
	iRDistance[38] = 217;
	iRDistance[39] = 210;
	
	robotState = ON; // Wer're thinking positive. The robot is ON untill whe know nothing other. :-)
}


SensorThread::~SensorThread()
{
}


void SensorThread::stop()
{
	stopped = true;
}


void SensorThread::run()
{
	bool heartbeatToggle = false;


	//  start "threading"...
	while (!stopped)
	{
		// let the thread sleep some time for having more time for the other threads
		msleep(THREADSLEEPTIME);

		if ( (robotState == ON) && (simulationMode == false) )
		{
			// Lock the mutex. If another thread has locked the mutex then this call will block until that thread has unlocked it.
			mutex->lock();

/*			infrared Sensors temporarily removed from robot!!

			//------------------
			// infrared sensors
			//------------------
			if (readInfraredSensor(SENSOR1) == false)
			{
				// Unlock the mutex.
				mutex->unlock();
				stop();
			}

			if (readInfraredSensor(SENSOR2) == false)
			{
				// Unlock the mutex.
				mutex->unlock();
				stop();
			}

			if (readInfraredSensor(SENSOR3) == false)
			{
				// Unlock the mutex.
				mutex->unlock();
				stop();
			}

			if (readInfraredSensor(SENSOR4) == false)
			{
				// Unlock the mutex.
				mutex->unlock();
				stop();
			}

			if (readInfraredSensor(SENSOR5) == false)
			{
				// Unlock the mutex.
				mutex->unlock();
				stop();
			}

			if (readInfraredSensor(SENSOR6) == false)
			{
				// Unlock the mutex.
				mutex->unlock();
				stop();
			}

			// infrared sensors 7 and 8 are now the voltage sensors!
*/

/*			ultrasonic Sensors temporarily removed from robot!!
			
			//---------------------
			// ulatrasonic sensors
			//---------------------
			if (readUltrasonicSensor(SENSOR16) == false)
			{
				// Unlock the mutex.
				mutex->unlock();
				stop();
			}
*/

			//-----------------
			// voltage sensors
			//-----------------
			if (readVoltageSensor(VOLTAGESENSOR1) == false) // sensor 8 is the former infrared sensor 8 ! This is now the 12 V battery!
			{
				// Unlock the mutex.
				mutex->unlock();
				stop();
			}
			// send value over the network
			// *0v42# means voltagesensor1 with 42 V (the digits after the decimal points are ignored here!)
			emit sendNetworkString( QString("*%1v%2#").arg(VOLTAGESENSOR1).arg( (int) voltageSensorValue[VOLTAGESENSOR1]));

			if (readVoltageSensor(VOLTAGESENSOR2) == false) // sensor 7 is the former infrared sensor 7 ! This is now the 24 V battery!
			{
				// Unlock the mutex.
				mutex->unlock();
				stop();
			}
			// send value over the network
			// *0v42# means voltagesensor1 with 42 V (the digits after the decimal points are ignored here!)
			emit sendNetworkString( QString("*%1v%2#").arg(VOLTAGESENSOR2).arg( (int) voltageSensorValue[VOLTAGESENSOR2]));
/*
			//---------------
			// motor sensors
			//---------------
			if (readMotorSensor(MOTORSENSOR1) == false)
			{
				// Unlock the mutex.
				mutex->unlock();
				stop();
			}
			// send value over the network
			// *0m42# means motorsensor1 with 42 mA
			emit sendNetworkString( QString("*%1m%2#").arg(MOTORSENSOR1).arg(getMAmpere(MOTORSENSOR1)));
*/
			//====================================================================
			// send an optical heartbeat signal to the GUI
			if (!heartbeatToggle)
			{
	 			emit heartbeat(GREEN);
			}
			else
			{
	 			emit heartbeat(LEDOFF);
			}
			heartbeatToggle = !heartbeatToggle;
			//====================================================================
/*
			if (readMotorSensor(MOTORSENSOR2) == false)
			{
				// Unlock the mutex.
				mutex->unlock();
				stop();
			}
			// send value over the network
			// *1m42# means motorsensor2 with 42 mA
			emit sendNetworkString( QString("*%1m%2#").arg(MOTORSENSOR2).arg(getMAmpere(MOTORSENSOR2)));
*/
/* TODO: implement reading of motor sensors 3 and 4 !
			
			if (readMotorSensor(MOTORSENSOR3) == false)
			{
				// Unlock the mutex.
				mutex->unlock();
				stop();
			}
			// send value over the network
			// *1m42# means motorsensor2 with 42 mA
			emit sendNetworkString( QString("*%1m%2#").arg(MOTORSENSOR3).arg(getMAmpere(MOTORSENSOR3)));

			if (readMotorSensor(MOTORSENSOR4) == false)
			{
				// Unlock the mutex.
				mutex->unlock();
				stop();
			}
			// send value over the network
			// *1m42# means motorsensor2 with 42 mA
			emit sendNetworkString( QString("*%1m%2#").arg(MOTORSENSOR4).arg(getMAmpere(MOTORSENSOR4)));


			//------------------------------------------------------
			// read driven distance from motor 1 (encoder sensor)
			//------------------------------------------------------
			if (interface1->sendChar(READ_MOTOR_DISTANCE1) == false)
			{
				// Unlock the mutex.
				mutex->unlock();
				qDebug("ERROR md1 sending to serial port (SensorThread)");
				return;
			}

			// receive the 16 Bit answer from the MC
			interface1->receiveInt(&value);
			//qDebug("Received value motor1: %d", value);

			// store measured values in the array
			drivenDistance[MOTORSENSOR1] = value;
			value = 0;


			//------------------------------------------------------
			// read driven distance from motor 2 (encoder sensor)
			//------------------------------------------------------
			if (interface1->sendChar(READ_MOTOR_DISTANCE2) == false)
			{
				// Unlock the mutex.
				mutex->unlock();
				qDebug("ERROR md2 sending to serial port (SensorThread)");
				return;
			}

			// receive the 16 Bit answer from the MC
			interface1->receiveInt(&value);
			//qDebug("Received value motor1: %d", value);

			// store measured values in the array
			drivenDistance[MOTORSENSOR2] = value;
			value = 0;
*/

			//====================================================================

/* contacts temporarily removed from robot!!

			//------------------------------------------------------
			// read value of contact 1 (cam pan L)
			//------------------------------------------------------
			if (interface1->sendChar(READ_CONTACT1) == false)
			{
				// Unlock the mutex.
				mutex->unlock();
				qDebug("ERROR c1 sending to serial port (SensorThread)");
				return;
			}

			// receive the 8 Bit answer from the MC **8 bit**
			interface1->receiveChar(&cValue);

			// store measured value
			contactValue[CONTACT1] = cValue;

			// emit
			if (cValue == 0)
			{
				emit contactAlarm(LEFT, false);
			}
			else
			{
				emit contactAlarm(LEFT, true);
			}

			cValue = 0;



			//------------------------------------------------------
			// read value of contact 2 (cam pan R)
			//------------------------------------------------------
			if (interface1->sendChar(READ_CONTACT2) == false)
			{
				// Unlock the mutex.
				mutex->unlock();
				qDebug("ERROR c2 sending to serial port (SensorThread)");
				return;
			}

			// receive the 8 Bit answer from the MC **8 bit**
			interface1->receiveChar(&cValue);

			// store measured value
			contactValue[CONTACT2] = cValue;

			// emit
			if (cValue == 0)
			{
				emit contactAlarm(RIGHT, false);
			}
			else
			{
				emit contactAlarm(RIGHT, true);
			}

			cValue = 0;


			//------------------------------------------------------
			// read value of contact 3 (cam tilt L/TOP)
			//------------------------------------------------------
			if (interface1->sendChar(READ_CONTACT3) == false)
			{
				// Unlock the mutex.
				mutex->unlock();
				qDebug("ERROR c3 sending to serial port (SensorThread)");
				return;
			}

			// receive the 8 Bit answer from the MC **8 bit**
			interface1->receiveChar(&cValue);

			// store measured value
			contactValue[CONTACT3] = cValue;

			// emit
			if (cValue == 0)
			{
				emit contactAlarm(TOP, false);
			}
			else
			{
				emit contactAlarm(TOP, true);
			}

			cValue = 0;


			//------------------------------------------------------
			// read value of contact 4 (cam tilt R/BOTTOM)
			//------------------------------------------------------
			if (interface1->sendChar(READ_CONTACT4) == false)
			{
				// Unlock the mutex.
				mutex->unlock();
				qDebug("ERROR c4 sending to serial port (SensorThread)");
				return;
			}

			// receive the 8 Bit answer from the MC **8 bit**
			interface1->receiveChar(&cValue);

			// store measured value
			contactValue[CONTACT4] = cValue;

			// emit
			if (cValue == 0)
			{
				emit contactAlarm(BOTTOM, false);
			}
			else
			{
				emit contactAlarm(BOTTOM, true);
			}

			cValue = 0;

contacts temporarily removed from robot!! */


			//====================================================================
/*

			//------------------------------------------------------
			// read x value from magnetic sensor
			//------------------------------------------------------
			if (interface1->sendChar(READ_AXIS_X) == false)
			{
				// Unlock the mutex.
				mutex->unlock();
				qDebug("ERROR x sending to serial port (SensorThread)");
				return;
			}

			// receive the 16 Bit answer from the MC
			interface1->receiveInt(&value);

			// convert the value to degrees and store the value in the class member
			xAxis =  convertToDegree(value);
// 			qDebug("From Atmel=%d / xAxis=%f", value, xAxis);
			value = 0;
			
			// send value over the network
			// *xc42# means axis x of the compass has 42°
			// CONVERT TO INT! Only for displaying!
			emit sendNetworkString( QString("*xc%1#").arg( (int) xAxis ));


			//------------------------------------------------------
			// read y value from magnetic sensor
			//------------------------------------------------------
			if (interface1->sendChar(READ_AXIS_Y) == false)
			{
				// Unlock the mutex.
				mutex->unlock();
				qDebug("ERROR y sending to serial port (SensorThread)");
				return;
			}

			// receive the 16 Bit answer from the MC
			interface1->receiveInt(&value);

			// convert the value to degrees and store the value in the class member
			yAxis = convertToDegree(value);
			value = 0;

			// send value over the network
			// *yc42# means axis y of the compass has 42°
			// CONVERT TO INT! Only for displaying!
			emit sendNetworkString( QString("*yc%1#").arg( (int) yAxis ));


			//------------------------------------------------------
			// read z value from magnetic sensor
			//------------------------------------------------------
			if (interface1->sendChar(READ_AXIS_Z) == false)
			{
				// Unlock the mutex.
				mutex->unlock();
				qDebug("ERROR z sending to serial port (SensorThread)");
				return;
			}

			// receive the 16 Bit answer from the MC
			interface1->receiveInt(&value);

			// convert the value to degrees and store the value in the class member
			zAxis = convertToDegree(value);
			value = 0;

			// send value over the network
			// *zc42# means axis z of the compass has 42°
			// CONVERT TO INT! Only for displaying!
			emit sendNetworkString( QString("*zc%1#").arg( (int) zAxis ));
*/			
			
			//====================================================================
			// Unlock the mutex.
			//====================================================================
			mutex->unlock();

		} // simulation = false
		
		if (simulationMode)
		{
			// TODO: now we're sleeping 500ms because ww're only simulating. Is this okay????
			msleep(500);
			
			// TODO: here or where, when simulating?????  send an optical heartbeat signal to the GUI
			if (!heartbeatToggle)
			{
	 			emit heartbeat(GREEN);
			}
			else
			{
	 			emit heartbeat(LEDOFF);
			}
			heartbeatToggle = !heartbeatToggle;
		}

		//------------------------------
		// emit ALL compass axes values
		//------------------------------
		emit compassDataComplete(xAxis, yAxis, zAxis);

		//====================================================================
		//  e m i t  Signal
		//====================================================================
		emit sensorDataComplete();
	}
	stopped = false;
}


int SensorThread::convertToDistance(int sensorValue)
{
	unsigned char calibrationValue = 8;


	// search the array for the measured value
	for (int n=0; n<=(IRSENSORARRAYSIZE-1); n++)
	{
		if (sensorValue >= iRDistance[n])
		{
			// return the index number plus calibration value
			// (this IS the measured distance in cm!!)
			return (n+calibrationValue);
		}
	}

	// value not found in the array
	// return the highest DISTANCE (the highest index number plus calibration value!)
	return (IRSENSORARRAYSIZE + calibrationValue);
}


int SensorThread::convertToSensorValue(int distance)
{
	unsigned char calibrationValue = 8;


	if ((distance-calibrationValue) <= 0)
	{
		return iRDistance[0];
	}

	if ((distance-calibrationValue) >= IRSENSORARRAYSIZE-1)
	{
		return iRDistance[IRSENSORARRAYSIZE-1];
	}

	return (iRDistance[distance-calibrationValue]);
}


int SensorThread::getMAmpere(int sensor)
{
	if ((sensor < MOTORSENSOR1) || (sensor > MOTORSENSOR4))
	{
		qDebug("ERROR sensorThread, getMAmpere: wrong motor sensor");
		return 0;
	}


	// convert the measured value to mili Ampere (mA)
	//
	// sensorValue 29 = 1000 mili Ampere (mA)
	//
	// return AND convert to mA !!
	return (motorSensorValue[sensor] * CONVERSIONFACTORMOTORSENSOR);
}


float SensorThread::getVoltage(int sensor)
{
	if ((sensor < VOLTAGESENSOR1) || (sensor > VOLTAGESENSOR2))
	{
		qDebug("ERROR sensorThread, getVoltage: wrong voltage sensor");
		return 0;
	}

	
	// return the converted measured value in Volt (V)
	return convertToVolt(sensor);
}


int SensorThread::getContactValue(int contact)
{
	if ((contact < CONTACT1) || (contact > CONTACT4))
	{
		qDebug("ERROR sensorThread, getContactValue: wrong contact number");
		return 0;
	}

	// typecasting for convenience!
	return (int) contactValue[contact];
}


int SensorThread::getIrSensorValue(int sensor)
{
	if ((sensor < SENSOR1) || (sensor > SENSOR8))
	{
		qDebug("ERROR: wrong ir sensor");
		return 0;
	}

	return iRSensorValue[sensor];
}


int SensorThread::getDistance(int sensor)
{
	int distance = 0;

	if ((sensor < SENSOR1) || (sensor > SENSOR8))
	{
		qDebug("ERROR: wrong ir sensor");
		return 0;
	}

	// get the stored distance
	distance = convertToDistance(iRSensorValue[sensor]);

	//qDebug("sensor: %1", sensor);

	//--------------------------------------------------------------------------------------
	// correct the value from sensors because of their position on the bot
	// e.g. the measured distance from the sensor is not the same then this from the bot.
	//--------------------------------------------------------------------------------------
	if (sensor == SENSOR1) // sensor front left
		distance -= 9;

	if (sensor == SENSOR2) // sensor front right
		distance -= 9;

	if (sensor == SENSOR3) // sensor back right
		distance -= 9;

	if (sensor == SENSOR4) // sensor back left
		distance -= 7;

	if (sensor == SENSOR5) // sensor left
		distance -= 1;

	if (sensor == SENSOR6) // front senor (under the laser)
		distance -= 19;

	if (sensor == SENSOR7) // sensor right
		distance -= 1;

	if (sensor == SENSOR8) // middle back senor
		distance -= 9;



	return distance;
}


int SensorThread::getDrivenDistance(int sensor)
{
	if ((sensor < MOTORSENSOR1) || (sensor > DRIVENDISTANCEARRAYSIZE-1))
	{
		qDebug("ERROR: wrong motor sensor");
		return 0;
	}

	return drivenDistance[sensor];
}


void SensorThread::resetDrivenDistance(int sensor)
{
	if ((sensor < MOTORSENSOR1) || (sensor > DRIVENDISTANCEARRAYSIZE-1))
	{
		qDebug("ERROR: wrong motor sensor");
		return;
	}

	// Lock the mutex. If another thread has locked the mutex then this call will block until that thread has unlocked it.
	mutex->lock();

	//------------------------------------------------------
	// reset
	//------------------------------------------------------
	switch (sensor)
	{
		case MOTORSENSOR1:
			if (interface1->sendChar(RESET_MOTOR_DISTANCE1) == false)
			{
				// Unlock the mutex.
				mutex->unlock();
				qDebug("ERROR sending to serial port (SensorThread)");
				return;
			}
			break;
		case MOTORSENSOR2:
			if (interface1->sendChar(RESET_MOTOR_DISTANCE2) == false)
			{
				// Unlock the mutex.
				mutex->unlock();
				qDebug("ERROR sending to serial port (SensorThread)");
				return;
			}
			break;
	}

	// Unlocks the mutex. Attempting to unlock a mutex in a different thread to the one that locked it results in an error.
	mutex->unlock();

}


/*!
Returns the stored value from an ultrasonic sensor in cm
*/
int SensorThread::getUsSensorValue(int sensor)
{
	// This value represents the distance from the sensor to the front of the robot in cm.
	// So this value is substracted from the returned result to give the distance from the robot to the next obstacle
	// (and not the distance from the sensor to the next obstacle)
	unsigned char calibrationValue = 53;


	if ((sensor < SENSOR16) || (sensor > SENSOR16))
	{
		qDebug("ERROR: wrong us sensor");
		return 0;
	}

	switch (sensor)
	{
		case SENSOR16:
			return (usSensorValue[0] - calibrationValue);
	}

	// this line is never reached
	return 0;
}


int SensorThread::getMotorSensorValue(int sensor)
{
	//
	// These values were NOT converted at any point in this method!
	//

	if ((sensor < MOTORSENSOR1) || (sensor > MOTORSENSORARRAYSIZE-1))
	{
		qDebug("ERROR: wrong motor sensor");
		return 0;
	}


	return motorSensorValue[sensor];
}


void SensorThread::setSimulationMode(bool state)
{
	simulationMode = state;

	// fill array with some nice values
	if (simulationMode == true)
	{
		// FIXME: Values are not correct / corresponding to values in GUI. Only in sim mode!
		// all values in cm
		iRSensorValue[SENSOR1] = convertToSensorValue(30);
		iRSensorValue[SENSOR2] = convertToSensorValue(60);
		iRSensorValue[SENSOR3] = convertToSensorValue(25);
		iRSensorValue[SENSOR4] = convertToSensorValue(27);
		iRSensorValue[SENSOR5] = convertToSensorValue(8);
		iRSensorValue[SENSOR6] = convertToSensorValue(40);
		iRSensorValue[SENSOR7] = convertToSensorValue(10);
		iRSensorValue[SENSOR8] = convertToSensorValue(59);

		// all values in cm
		usSensorValue[0] = 28;

		motorSensorValue[MOTORSENSOR1] = (int)1000/CONVERSIONFACTORMOTORSENSOR;
		motorSensorValue[MOTORSENSOR2] = (int)2000/CONVERSIONFACTORMOTORSENSOR;
		motorSensorValue[MOTORSENSOR3] = (int)3000/CONVERSIONFACTORMOTORSENSOR;
		motorSensorValue[MOTORSENSOR4] = (int)4000/CONVERSIONFACTORMOTORSENSOR;

		voltageSensorValue[VOLTAGESENSOR1] = 10.500000 * CONVERSIONFACTORVOLTAGESENSOR1; // simulate an empty 12 V battery
		voltageSensorValue[VOLTAGESENSOR2] = 24.000000 * CONVERSIONFACTORVOLTAGESENSOR2; // simulate a full 24 V battery

		// initialisation
		for (int i=0; i<CONTACTARRAYSIZE; i++)
		{
			contactValue[i] = 0;
		}

		// initialisation
		for (int i=0; i<DRIVENDISTANCEARRAYSIZE; i++)
		{
			drivenDistance[i] = 42;
		}

		// compass test values
		xAxis = READ_AXIS_X;
		yAxis = READ_AXIS_Y;
		zAxis = READ_AXIS_Z;
	}
	else
	{
		if (robotState==OFF)
			emit heartbeat(RED);
		
		iRSensorValue[SENSOR1] = 0;
		iRSensorValue[SENSOR2] = 0;
		iRSensorValue[SENSOR3] = 0;
		iRSensorValue[SENSOR4] = 0;
		iRSensorValue[SENSOR5] = 0;
		iRSensorValue[SENSOR6] = 0;
		iRSensorValue[SENSOR7] = 0;
		iRSensorValue[SENSOR8] = 0;

		// initialisation
		for (int i=0; i<USSENSORARRAYSIZE; i++)
		{
			usSensorValue[i] = 53; // +53 = 0cm from the bot
		}

		// initialisation
		for (int i=0; i<MOTORSENSORARRAYSIZE; i++)
		{
			motorSensorValue[i] = 0;
		}
		
		voltageSensorValue[VOLTAGESENSOR1] = 0;
		voltageSensorValue[VOLTAGESENSOR2] = 0;

		// initialisation
		for (int i=0; i<CONTACTARRAYSIZE; i++)
		{
			contactValue[i] = 0;
		}

		// initialisation
		for (int i=0; i<DRIVENDISTANCEARRAYSIZE; i++)
		{
			drivenDistance[i] = 0;
		}

		xAxis = 0;
		yAxis = 0;
		zAxis = 0;
	}
}


int SensorThread::getCompassValue(unsigned char axis)
{
	switch (axis)
	{
		case READ_AXIS_X:
			return xAxis;
			break;
		case READ_AXIS_Y:
			return yAxis;
			break;
		case READ_AXIS_Z:
			return zAxis;
			break;
	}

	// this line should never be reached!
	return -1;
}


void SensorThread::setRobotState(bool state)
{
	// store the state within this class
	robotState = state;
}


float SensorThread::convertToDegree(int sensorValue)
{
	// 'value' has to be between 0 and 65536, because we're using a 16 bit value
	//
	// A compass has 360 degrees, so the factor for 1 degree is 360/65536
	
	return (sensorValue * (360.0 / 65536.0));
}


float SensorThread::convertToVolt(int sensor)
{
	if ((sensor < VOLTAGESENSOR1) || (sensor > VOLTAGESENSOR2))
	{
		qDebug("ERROR sensorThread, convertToVolt: wrong voltage sensor");
		return 0;
	}

	
	if (sensor == VOLTAGESENSOR1)
	{
		// convert the measured value to Volt (V)
		return ( voltageSensorValue[sensor] / CONVERSIONFACTORVOLTAGESENSOR1 );
	}

	if (sensor == VOLTAGESENSOR2)
	{
		// convert the measured value to Volt (V)
		return ( voltageSensorValue[sensor] / CONVERSIONFACTORVOLTAGESENSOR2 );
	}
	
	return -1.0;
}


bool SensorThread::readInfraredSensor(short int sensor)
{
	int value = 0;
	
	switch (sensor)
	{
		case SENSOR1:
			// read sensor
			if (interface1->sendChar(READ_SENSOR_1) == true)
			{
				// receive the 16 Bit answer from the MC
				if (interface1->receiveInt(&value) == false)
				{
					iRSensorValue[SENSOR1] = 0;
					return false;
				}
	
				// store measured value
				iRSensorValue[SENSOR1] = value;
				return true;
			}
			else
			{
				qDebug("ERROR reading infrared sensor 1");
				return false;
			}
			break;
		case SENSOR2:
			// read sensor
			if (interface1->sendChar(READ_SENSOR_2) == true)
			{
				// receive the 16 Bit answer from the MC
				if (interface1->receiveInt(&value) == false)
				{
					iRSensorValue[SENSOR2] = 0;
					return false;
				}
	
				// store measured value
				iRSensorValue[SENSOR2] = value;
				return true;
			}
			else
			{
				qDebug("ERROR reading infrared sensor 2");
				return false;
			}
			break;
		case SENSOR3:
			// read sensor
			if (interface1->sendChar(READ_SENSOR_3) == true)
			{
				// receive the 16 Bit answer from the MC
				if (interface1->receiveInt(&value) == false)
				{
					iRSensorValue[SENSOR3] = 0;
					return false;
				}
	
				// store measured value
				iRSensorValue[SENSOR3] = value;
				return true;
			}
			else
			{
				qDebug("ERROR reading infrared sensor 3");
				return false;
			}
			break;
		case SENSOR4:
			// read sensor
			if (interface1->sendChar(READ_SENSOR_4) == true)
			{
				// receive the 16 Bit answer from the MC
				if (interface1->receiveInt(&value) == false)
				{
					iRSensorValue[SENSOR4] = 0;
					return false;
				}
	
				// store measured value
				iRSensorValue[SENSOR4] = value;
				return true;
			}
			else
			{
				qDebug("ERROR reading infrared sensor 4");
				return false;
			}
			break;
		case SENSOR5:
			// read sensor
			if (interface1->sendChar(READ_SENSOR_5) == true)
			{
				// receive the 16 Bit answer from the MC
				if (interface1->receiveInt(&value) == false)
				{
					iRSensorValue[SENSOR5] = 0;
					return false;
				}
	
				// store measured value
				iRSensorValue[SENSOR5] = value;
				return true;
			}
			else
			{
				qDebug("ERROR reading infrared sensor 5");
				return false;
			}
			break;
		case SENSOR6:
			// read sensor
			if (interface1->sendChar(READ_SENSOR_6) == true)
			{
				// receive the 16 Bit answer from the MC
				if (interface1->receiveInt(&value) == false)
				{
					iRSensorValue[SENSOR6] = 0;
					return false;
				}
	
				// store measured value
				iRSensorValue[SENSOR6] = value;
				return true;
			}
			else
			{
				qDebug("ERROR reading infrared sensor 6");
				return false;
			}
			break;
		case SENSOR7:
			// read sensor
			if (interface1->sendChar(READ_SENSOR_7) == true)
			{
				// receive the 16 Bit answer from the MC
				if (interface1->receiveInt(&value) == false)
				{
					iRSensorValue[SENSOR7] = 0;
					return false;
				}
	
				// store measured value
				iRSensorValue[SENSOR7] = value;
				return true;
			}
			else
			{
				qDebug("ERROR reading infrared sensor 7");
				return false;
			}
			break;
		case SENSOR8:
			// read sensor
			if (interface1->sendChar(READ_SENSOR_8) == true)
			{
				// receive the 16 Bit answer from the MC
				if (interface1->receiveInt(&value) == false)
				{
					iRSensorValue[SENSOR8] = 0;
					return false;
				}
	
				// store measured value
				iRSensorValue[SENSOR8] = value;
				return true;
			}
			else
			{
				qDebug("ERROR reading infrared sensor 8");
				return false;
			}
			break;
	}
	
	// this line should be never reached
	qDebug("WARNING: wrong sensor number in readInfraredSensor()");
	return false;
}


bool SensorThread::readUltrasonicSensor(short int sensor)
{
	int value = 0;
	
	switch (sensor)
	{
		case SENSOR16:
			// read sensor
			if (interface1->sendChar(READ_SENSOR_16) == true)
			{
				// receive the 16 Bit answer from the MC
				if (interface1->receiveInt(&value) == false)
				{
					usSensorValue[ULTRASONICSENSOR1] = 0;
					return false;
				}
	
				// store measured value
				usSensorValue[ULTRASONICSENSOR1] = value;
				return true;
			}
			else
			{
				qDebug("ERROR reading ultrasonic sensor 1");
				return false;
			}
			break;
	}
	
	// this line should be never reached
	qDebug("WARNING: wrong sensor number in readUltrasonicSensor()");
	return false;
}


bool SensorThread::readVoltageSensor(short int sensor)
{
	int value = 0;
	
	switch (sensor)
	{
		case VOLTAGESENSOR1:
			// read sensor
			if (interface1->sendChar(READ_SENSOR_8) == true) // sensor 8 is the former infrared sensor 8 ! This is now the 12 V battery!
			{
				// receive the 16 Bit answer from the MC
				if (interface1->receiveInt(&value) == false)
				{
					voltageSensorValue[VOLTAGESENSOR1] = 0;
					return false;
				}
	
				// store measured value
				voltageSensorValue[VOLTAGESENSOR1] = value;
				return true;
			}
			else
			{
				qDebug("ERROR reading voltage sensor 1");
				return false;
			}
			break;
		case VOLTAGESENSOR2:
			// read sensor
			if (interface1->sendChar(READ_SENSOR_7) == true) // sensor 7 is the former infrared sensor 7 ! This is now the 24 V battery!
			{
				// receive the 16 Bit answer from the MC
				if (interface1->receiveInt(&value) == false)
				{
					voltageSensorValue[VOLTAGESENSOR2] = 0;
					return false;
				}
	
				// store measured value
				voltageSensorValue[VOLTAGESENSOR2] = value;
				return true;
			}
			else
			{
				qDebug("ERROR reading voltage sensor 2");
				return false;
			}
			break;
	}
	
	// this line should be never reached
	qDebug("WARNING: wrong sensor number in readVoltageSensor()");
	return false;
}


bool SensorThread::readMotorSensor(short int sensor)
{
	int value = 0;
	
	switch (sensor)
	{
		case MOTORSENSOR1:
			// read sensor
			if (interface1->sendChar(READ_MOTOR_SENSOR1) == true)
			{
				// receive the 16 Bit answer from the MC
				if (interface1->receiveInt(&value) == false)
				{
					voltageSensorValue[MOTORSENSOR1] = 0;
					return false;
				}
	
				// store measured value
				voltageSensorValue[MOTORSENSOR1] = value;
				return true;
			}
			else
			{
				qDebug("ERROR reading motor sensor 1");
				return false;
			}
			break;
		case MOTORSENSOR2:
			// read sensor
			if (interface1->sendChar(READ_MOTOR_SENSOR2) == true)
			{
				// receive the 16 Bit answer from the MC
				if (interface1->receiveInt(&value) == false)
				{
					voltageSensorValue[MOTORSENSOR2] = 0;
					return false;
				}
	
				// store measured value
				voltageSensorValue[MOTORSENSOR2] = value;
				return true;
			}
			else
			{
				qDebug("ERROR reading motor sensor 2");
				return false;
			}
			break;
		case MOTORSENSOR3:
			qDebug("ERROR reading motor sensor 3 not implented yet!!"); // TODO implement reading motor sensor 3
			/*
			// read sensor
			if (interface1->sendChar(READ_MOTOR_SENSOR3) == true)
			{
				// receive the 16 Bit answer from the MC
				if (interface1->receiveInt(&value) == false)
				{
					voltageSensorValue[MOTORSENSOR3] = 0;
					return false;
				}
	
				// store measured value
				voltageSensorValue[MOTORSENSOR3] = value;
				return true;
			}
			else
			{
				qDebug("ERROR reading motor sensor 3");
				return false;
			}
			*/
			break;
		case MOTORSENSOR4:
			qDebug("ERROR reading motor sensor 4 not implented yet!!"); // TODO implement reading motor sensor 4
			/*
			// read sensor
			if (interface1->sendChar(READ_MOTOR_SENSOR4) == true)
			{
				// receive the 16 Bit answer from the MC
				if (interface1->receiveInt(&value) == false)
				{
					voltageSensorValue[MOTORSENSOR4] = 0;
					return false;
				}
	
				// store measured value
				voltageSensorValue[MOTORSENSOR4] = value;
				return true;
			}
			else
			{
				qDebug("ERROR reading motor sensor 4");
				return false;
			}
			*/
			break;
	}
	
	// this line should be never reached
	qDebug("WARNING: wrong sensor number in readVoltageSensor()");
	return false;
}
