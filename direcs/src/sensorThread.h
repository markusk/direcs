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

#ifndef SENSORTHREAD_H
#define SENSORTHREAD_H

//-------------------------------------------------------------------
#include "interfaceAvr.h"
//-------------------------------------------------------------------
#include <QThread>
#include <QMutex>
//-------------------------------------------------------------------

/**
\brief Responsible for getting all data from all sensors - except laserscanners.

The SensorThread class is a thread, responsible for getting all data from all sensors (infrared, ultrasonic, motor current sensors, wheel encoders, voltage sensors and the 3D compass) and to store them.
It also gives read/write access to this data.
*/
class SensorThread : public QThread
{
    Q_OBJECT

	public:
		SensorThread(InterfaceAvr *i, QMutex *m);
		~SensorThread();

		/**
		Stops the thread.
		*/
		void stop();

		/**
		Starts the thread.
		*/
		virtual void run();

		/**
		@param sensor is the sensor number.
		@return The value of an infrared sensor.
		*/
		int getIrSensorValue(int sensor);

		/**
		@param sensor is the sensor number.
		@return The value of an ultrasonic sensor.
		*/
		int getUsSensorValue(int sensor);

		/**
		@param sensor is the sensor number.
		@return The value of a motor sensor.
		*/
		int getMotorSensorValue(int sensor);

		/**
		@param sensor is the sensor number.
		@return The distance in cm, of a sensor.
		*/
		int getDistance(int sensor);

		/**
		@return The driven distance in cm, of a wheel encoder.
		@param sensor is the sensor number.
		*/
		int getDrivenDistance(int sensor);

		/**
		Converts a infrared sensor value to a distance in cm.
		@param sensorValue is the measured sensor value.
		@return The distance in cm.
		@sa convertToSensorValue()
		*/
		int convertToDistance(int sensorValue);

		/**
		Converts a distance in cm to an infrared sensor value.
		@param distance is the distance in cm.
		@return The sensor value.
		@sa convertToDistance()
		*/
		int convertToSensorValue(int distance);

		/**
		@return The measured motor sensor value in milli Ampere (mA).
		@param motor is the motor number.
		*/
		int getMAmpere(int motor);

		/**
		@param sensor is the sensor number and can be VOLTAGESENSOR1 or VOLTAGESENSOR2.
		@return The (already converted) voltage in Volt (V).
		*/
		float getVoltage(int sensor);

		/**
		@param contact is the contact number.
		@return The 'measured' contact value. 0 if OFF; unequal to 0 if set.
		*/
		int getContactValue(int contact);

		/**
		@param axis is the x, y or z axis.
		@return The measured x, y and z value from the 3D magnetic compass module (micromag3).
		*/
		int getCompassValue(unsigned char axis);


	public slots:
		/**
		Resets the driven distance to 0.
		@param sensor is the sensor number.
		*/
		void resetDrivenDistance(int sensor);

		/**
		This slot enables or disables the simulation mode.
		In the simulation mode all sensor values are set to a fixed value. No real sensor values are read from the robot, when set to true.
		@param status can be true or false.
		@sa Gui::simulate()
		*/
		void setSimulationMode(bool state);
		
		/**
		This slots takes the robot (circuit) state, to know if the robot is ON or OFF.
		When the class knows this, unnecessary communication with the interface can be avoided.
		@param state can be ON or OFF
		*/
		void setRobotState(bool state);


	signals:
		/**
		This signal is emitted when all sensors were read.
		@sa Direcs::showSensorData()
		*/
		void sensorDataComplete();

		/**
		This signal is emitted when a contact value is read. The result (an alarm) will be displayed immediately in the camera live image.
		@sa Direcs::showContactAlarm()
		*/
		void contactAlarm(char position, bool state);

		/**
		Sends a string over the network.
		*/
		void sendNetworkString(QString text);
		
		/**
		This signal is emitted when all compass values were read.
		@param x is the x axis value
		@param y is the y axis value
		@param z is the z axis value
		@sa Direcs::showCompassAxes()
		*/
		void compassDataComplete(float x, float y, float z);
		
		/**
		This signal is emitted every TODO: seconds, when a specific value from the microcontroller was received.
		@sa Gui::setLEDHeartbeat()
		@sa Logfile::writeHeartbeat()
		*/
		void heartbeat(bool alive);


	private:
		/**
		Converts the compass sensor value (16 bit integer) to degrees (0 to 360).
		@param sensorValue
		@return The given value in degrees (0 to 360).
		*/
		float convertToDegree(int sensorValue);

		/**
		Converts a voltage sensor value to a value in Volt (V).
		@param sensor is the sensor number.
		@return The voltage in Volt.
		*/
		float convertToVolt(int sensor);
		
		mutable QMutex *mutex; // make this class thread-safe
		InterfaceAvr *interface1;
		volatile bool stopped;
		bool simulationMode;
		bool robotState; // stores the robot state within this class
	
		// Every thread sleeps some time, for having a bit more time fo the other threads!
		// Time in milliseconds
		static const unsigned long THREADSLEEPTIME = 10; // Default: 25 ms

		// Give the infrared sensors some names
		//
		// DONT CHANGE THIS NUMBERS!
		// THEY ARE ALSO USED TO ADRESS THE ARRAY "iRSensorValue[]" !!
		//
		static const short int SENSOR1 = 1;
		static const short int SENSOR2 = 2;
		static const short int SENSOR3 = 4;
		static const short int SENSOR4 = 8;
		static const short int SENSOR5 = 16;
		static const short int SENSOR6 = 32;
		static const short int SENSOR7 = 64;
		static const short int SENSOR8 = 128;

		// ultrasonic sensor
		static const short int SENSOR16 = 256;

		// 5 sensors + 1 = 16
		// This just for convenience to call the array index by the sensor names.
		// (wastin' some memory ;-) )
		//
		// #define SENSOR4 = 8
		//
		// For example:  iRSensorValue[SENSOR4] = 0;
		//
		int iRSensorValue[SENSOR8+1]; // ToDo: array mit 129 Werten statt 8 für 8 Sensoren !!! Überleg dir was !!!

		/** defines the size of the iRDistance[] array !! */
		static const unsigned char IRSENSORARRAYSIZE = 40;
		int iRDistance[IRSENSORARRAYSIZE];

		/**
		for storing the voltage values from the accumulators (power supply)
		*/
		int voltageSensorValue[2];

		static const short int VOLTAGESENSOR1 = 0;
		static const short int VOLTAGESENSOR2 = 1;

		/**
		Defines the size of the usSensorValue[] array
		This es equal to the number of ultrasonic sensors.
		*/
		static const unsigned char USSENSORARRAYSIZE = 1;
		int usSensorValue[USSENSORARRAYSIZE];

		/**
		Defines the size of the motorSensorValue[] array
		This es equal to the number of motor sensors.
		*/
		static const unsigned char MOTORSENSORARRAYSIZE = 4;
		int motorSensorValue[MOTORSENSORARRAYSIZE];

		static const short int MOTORSENSOR1 = 0;
		static const short int MOTORSENSOR2 = 1;
		static const short int MOTORSENSOR3 = 2;
		static const short int MOTORSENSOR4 = 3;

		// defines the size of the drivenDistance[] array
		// This es equal to the number of motors!
		static const unsigned char DRIVENDISTANCEARRAYSIZE = 2;
		int drivenDistance[DRIVENDISTANCEARRAYSIZE];

		/**
		Defines the conversion factor for the motor sensors to convert the sensor value in a "real world" value.
		For example, a measured sensor value for a motor sensor is 100, this multiplied with a conversion factor 29 results in 290 mA.
		*/
		static const unsigned char CONVERSIONFACTORMOTORSENSOR = 29;

		/**
		Defines the conversion factor for the voltage sensors to convert the sensor value in a "real world" value.
		For example, a measured sensor value for a voltage sensor is 685, this divided by a conversion factor 57.0 results in 12 V.
		*/
		static const float CONVERSIONFACTORVOLTAGESENSOR1 = 57.000;
		static const float CONVERSIONFACTORVOLTAGESENSOR2 = 36.125;

		/**
		The x, y and z  axis value from the 3D magnetic sensor
		 */
		float xAxis;
		float yAxis;
		float zAxis;

		static const bool ON  = true;   /** For motor or robot "ON" */
		static const bool OFF = false;  /** For motor or robot "OFF" */

		/**
		 Give the contacts for the pan tilt cam end positions some nice names
		 */
		static const unsigned char CONTACT1 = 0;
		static const unsigned char CONTACT2 = 1;
		static const unsigned char CONTACT3 = 2;
		static const unsigned char CONTACT4 = 3;

		// The position for the contact alarm in the camera image
		static const char LEFT   = 0;
		static const char RIGHT  = 1;
		static const char TOP    = 2;
		static const char BOTTOM = 3;

		static const unsigned char CONTACTARRAYSIZE = 4;
		char contactValue[CONTACTARRAYSIZE];

		//
		// the "serial" commands for the MC
		//
		static const unsigned char READ_SENSOR_1  = 1;
		static const unsigned char READ_SENSOR_2  = 2;
		static const unsigned char READ_SENSOR_3  = 3;
		static const unsigned char READ_SENSOR_4  = 4;
		static const unsigned char READ_SENSOR_5  = 5;
		static const unsigned char READ_SENSOR_6  = 6;
		static const unsigned char READ_SENSOR_7  = 7;
		static const unsigned char READ_SENSOR_8  = 8;
		static const unsigned char READ_SENSOR_16 = 16;

		static const unsigned char READ_MOTOR_SENSOR1 = 28;
		static const unsigned char READ_MOTOR_SENSOR2 = 29;

		static const unsigned char READ_MOTOR_DISTANCE1	= 30;
		static const unsigned char READ_MOTOR_DISTANCE2	= 31;

		static const short int RESET_MOTOR_DISTANCE1 = 32; // TODO: why int instead of char? Where else?
		static const short int RESET_MOTOR_DISTANCE2 = 33; // TODO: why int instead of char? Where else?

		static const unsigned char READ_AXIS_X = 61;
		static const unsigned char READ_AXIS_Y = 62;
		static const unsigned char READ_AXIS_Z = 63;

		static const unsigned char READ_CONTACT1 = 70;
		static const unsigned char READ_CONTACT2 = 71;
		static const unsigned char READ_CONTACT3 = 72;
		static const unsigned char READ_CONTACT4 = 73;
};

#endif
