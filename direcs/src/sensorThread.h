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

#ifndef SENSORTHREAD_H
#define SENSORTHREAD_H

//-------------------------------------------------------------------
#include "interfaceAvr.h"
//-------------------------------------------------------------------
#include <QThread>
#include <QMutex>
#include <QTime>
#include <QTimer>
#include <math.h>
//-------------------------------------------------------------------

/**
\author Markus Knapp
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

		/**
		@return The heartbeat value which should look like high=5 or low=0 Volt.
		*/
		int getHeartbeatValue();


	public slots:
		/**
		Resets the driven distance to 0.
		@param sensor is the sensor number.
		@return true on success, false on error
		*/
		bool resetDrivenDistance(int sensor);

		/**
		This slot enables or disables the simulation mode.
		In the simulation mode all sensor values are set to a fixed value. No real sensor values are read from the robot, when set to true.
		@param state can be true or false.
		@sa Gui::simulate()
		*/
		void setSimulationMode(bool state);

		/**
		This slots takes the robot (circuit) state, to know if the robot is ON or OFF.
		When the class knows this, unnecessary communication with the interface can be avoided.
		@param state can be ON or OFF
		*/
		void setRobotState(bool state);

		/**
		This slots takes the compass circuit state, to know if the compas module is connected or not.
		When the class knows this it is unnecessary to communicate with the interface.
		@param state can be true or false
		*/
		void setCompassState(bool state);

		/**
		This slot takes the received answers from the Atmel after sending a command string to it.
		@param atmelAnswer contains the answer
		@param caller is the name of the class which called this slot
		*/
		void takeCommandAnswer(QString atmelAnswer, QString caller);


	private slots:
		/**
		This Slot is called if we never get an answer from the Atmel
		*/
		void timeout();


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
		@param heading is the heading of the compass
		@sa Direcs::showCompassData() SensorThread::calculateHeading()
		*/
		void compassDataComplete(float x, float y, float z, float heading);

		/**
		This signal is emitted every \todo seconds, when a specific value from the microcontroller was received.
		@sa Gui::setLEDHeartbeat()
		@sa Logfile::writeHeartbeat()
		*/
		void heartbeat(unsigned char state);

		/**
		Emits a string to the GUI log / console.
		@sa Gui::appendLog()
		@param text is the message to be emitted
		@param CR is true when a CR/LF should be added at the end of the line (default)
		@param sayIt is true when the text for the log also should be spoken (default=false)
		@param addTimestamp If true, the a timestamp is added in front of the text. This parameter is optional.
		*/
		void message(QString text, bool CR=true, bool sayIt=false, bool addTimestamp=true);

		/**
		Emits a emergency signal for letting other modules know that we have a massive sensor error. So in that case an emergency stop or so could be initiated.
		@param errorlevel needs to be defined. Temporariliy we use -2 in case of error in this thread.
		*/
		void systemerror(int errorlevel);


	private:
		/**
		Converts the compass sensor value (16 bit integer 2's complement format) to degrees (0 to 360).
		@param sensorValue
		@return The given value in degrees (0 to 360).
		*/
		float convertToDegree(int sensorValue);

		/**
		Translates the x and y data from the 3D compass into a single orientation reading.
		*/
		void calculateHeading(void);

		/**
		Converts a voltage sensor value to a value in Volt (V).
		@param sensor is the sensor number.
		@return The voltage in Volt.
		*/
		float convertToVolt(int sensor);

		/**
		Reads an infrared sensor value from the microcontroller
		@param sensor is the sensor number (SENSOR1 to SENSOR6)
		@return true on success, false on error
		*/
		bool readInfraredSensor(short int sensor);

		/**
		Reads an ultrasonic sensor value from the microcontroller
		@param sensor is the sensor number (ULTRASONICSENSOR1)
		@return true on success, false on error
		*/
		bool readUltrasonicSensor(short int sensor);

		/**
		Reads a voltage sensor value from the microcontroller
		@param sensor is the sensor number (VOLTAGESENSOR1 to VOLTAGESENSOR2)
		*/
		void readVoltageSensor(short int sensor);

		/**
		Reads a motor sensor value from the microcontroller
		@param sensor is the sensor number (MOTORSENSOR1 to MOTORSENSOR4)
		@return true on success, false on error
		*/
		bool readMotorSensor(short int sensor);

		/**
		Reads a driven distance value from the microcontroller
		@param sensor is the sensor number (MOTORDISTANCE1 to MOTORDISTANCE2)
		@return true on success, false on error
		*/
		bool readDrivenDistance(short int sensor);

		/**
		Reads an axis value from the 3D compass from the microcontroller
		@param axis is the axis number (XAXIS, YAXIS or ZAXIS)
		@return true on success, false on error
		*/
		bool readCompassAxis(short int axis);

		/**
		Reads a contact value from the microcontroller (end contacts, when puhsing something or so)
		@param contact is the contact number (CONTACT1 to CONTACT4)
		@return true on success, false on error
		*/
		bool readContact(short int contact);

		QString thisClass;	/// this will contain the name of this class at runtime. @sa takeCommandAnswer()
		mutable QMutex *mutex; // make this class thread-safe
		InterfaceAvr *interface1;
		volatile bool stopped;
		volatile bool stillInProgress;
		bool simulationMode;
		bool robotState; // stores the robot state within this class
		bool compassState; // stores the robot state within this class
		bool commandExecutedSuccessfull; /// set to true, if command executed successfull. In this case a later timeout slot will check this first!

		QString commandReadVoltageSensor1;	/// *s7#	s7 = voltage sensor 1 = 24 Volt
		QString commandReadVoltageSensor2;	/// *s8#	s8 = voltage sensor 1 = 12 Volt

		QString atmelCommand; /// this is the command for the Atmel
		mutable QMutex varMutex; // this is for the var atmelCommand

		QTime duration; /// for measuring between sending an command to Atmel and the time it needs till the Atmel answers
		static const int ATMELTIMEOUT = 500; /// timeout in ms

		// Every thread sleeps some time, for having a bit more time fo the other threads!
		// Time in milliseconds
		static const unsigned long THREADSLEEPTIME = 750; // Default: 100 ms

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
		int iRSensorValue[SENSOR8+1]; /// \todo array mit 129 Werten statt 8 für 8 Sensoren !!!

		/** defines the size of the iRDistance[] array !! */
		static const unsigned char IRSENSORARRAYSIZE = 40;
		int iRDistance[IRSENSORARRAYSIZE];

		/**
		for storing the voltage values from the accumulators (power supply)
		*/
		int voltageSensorValue[2];

		static const short int VOLTAGESENSOR1 = 0;
		static const short int VOLTAGESENSOR2 = 1;

		static const int MAXIMUMPLOTHEARTBEAT = 5; /// theoratically 5 Volt for heartbeat "high" @sa Gui::initPlot

		/**
		Defines the size of the usSensorValue[] array
		This es equal to the number of ultrasonic sensors.
		*/
		static const unsigned char USSENSORARRAYSIZE = 1;
		int usSensorValue[USSENSORARRAYSIZE];

		static const short int ULTRASONICSENSOR1 = 0;

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

		static const short int DRIVENDISTANCE1 = 0;
		static const short int DRIVENDISTANCE2 = 1;

		/**
		Since a heartbeat can only high or low. we store only 1 value in this array. But we need an array for the plotThread.
		*/
		int heartbeatValue[1];

		/**
		Defines the conversion factor for the motor sensors to convert the sensor value in a "real world" value.
		For example, a measured sensor value for a motor sensor is 100, this multiplied with a conversion factor 29 results in 290 mA.
		*/
		static const unsigned char CONVERSIONFACTORMOTORSENSOR = 29;

		/**
		Defines the conversion factor for the voltage sensors to convert the sensor value in a "real world" value.
		For example, a measured sensor value for a voltage sensor is 685, this divided by a conversion factor 57.0 results in 12 V.
		*/
		static const float CONVERSIONFACTORVOLTAGESENSOR1 = 57.000; // 12 V, command *s8#
		static const float CONVERSIONFACTORVOLTAGESENSOR2 = 36.125; // 24 V, command *s7#

		/**
		The x, y, z axis value from the 3D magnetic sensor and the heading
		 */
		float xAxis;
		float yAxis;
		float zAxis;
		float heading;

		static const short int XAXIS = 0;
		static const short int YAXIS = 1;
		static const short int ZAXIS = 2;

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

		static const short int RESET_MOTOR_DISTANCE1 = 32; /// \todo why int instead of char? Where else?
		static const short int RESET_MOTOR_DISTANCE2 = 33; /// \todo why int instead of char? Where else?

		static const unsigned char READ_AXIS_X = 61;
		static const unsigned char READ_AXIS_Y = 62;
		static const unsigned char READ_AXIS_Z = 63;

		static const unsigned char READ_CONTACT1 = 70;
		static const unsigned char READ_CONTACT2 = 71;
		static const unsigned char READ_CONTACT3 = 72;
		static const unsigned char READ_CONTACT4 = 73;

		/**
		This is for the @sa heartbeat signal (the GUI LED colors)
		*/
		static const unsigned char LEDOFF	= 0;
		static const unsigned char RED		= 1;
		static const unsigned char GREEN	= 2;
};

#endif
