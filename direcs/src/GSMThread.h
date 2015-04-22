/*************************************************************************
 *   Copyright (C) Markus Knapp                                          *
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

#ifndef GSMTHREAD_H
#define GSMTHREAD_H

//-------------------------------------------------------------------
#include "interfaceAvr.h"
//-------------------------------------------------------------------
#include <QThread>
#include <QMutex>
#include <math.h>
//-------------------------------------------------------------------

/**
\author Markus Knapp
\brief Responsible for communicating with the GSM module.

The GSMThread class is a thread, responsible for getting all data from and to the GSM module.
Currently it checks if new SMS are received and emits a signal. This thread can also delete SMS on the GSM module.
*/
class GSMThread : public QThread
{
	Q_OBJECT

	public:
		GSMThread(InterfaceAvr *i, QMutex *m);
		~GSMThread();

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
		@sa Direcs::showCompassData() GSMThread::calculateHeading()
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
		Checks how many SMS are available.
		@return the amount of available SMS, or -1 in case of error
		*/
		int countSMS();

		/**
		Reads an ultrasonic sensor value from the microcontroller
		@param sensor is the sensor number (ULTRASONICSENSOR1)
		@return true on success, false on error
		*/
		bool readUltrasonicSensor(short int sensor);

		/**
		Reads a voltage sensor value from the microcontroller
		@param sensor is the sensor number (VOLTAGESENSOR1 to VOLTAGESENSOR2)
		@return true on success, false on error
		*/
		bool readVoltageSensor(short int sensor);

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


		QString className;	/// this will contain the name of _this_ class at runtime for debug messages
		mutable QMutex *mutex; // make this class thread-safe
		InterfaceAvr *interface1;
		volatile bool stopped;
		bool simulationMode;
		bool robotState; // stores the robot state within this class
		bool compassState; // stores the robot state within this class

		// Every thread sleeps some time, for having a bit more time fo the other threads!
		// Time in milliseconds
		static const unsigned long THREADSLEEPTIME = 1000; // Default: 100 ms


		/**
		for storing the voltage values from the accumulators (power supply)
		*/
		int voltageSensorValue[2];

		static const short int VOLTAGESENSOR1 = 0;
		static const short int VOLTAGESENSOR2 = 1;

		static const int MAXIMUMPLOTHEARTBEAT = 5; /// theoratically 5 Volt for heartbeat "high" @sa Gui::initPlot


		/**
		Since a heartbeat can only high or low. we store only 1 value in this array. But we need an array for the plotThread.
		*/
		int heartbeatValue[1];

		//
		// the "serial" commands for the MC
		//
		static const unsigned char SMS_COUNT  = 100;
		static const unsigned char SMS_READ   = 101;
		static const unsigned char SMS_SEND   = 102;
		static const unsigned char SMS_DELETE = 103;

		/**
		This is for the @sa heartbeat signal (the GUI LED colors)
		*/
		static const unsigned char LEDOFF	= 0;
		static const unsigned char RED		= 1;
		static const unsigned char GREEN	= 2;

		qint64 testCounter;
};

#endif
