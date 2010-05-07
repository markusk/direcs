/*************************************************************************
 *   Copyright (C) 2010 by Markus Knapp                                  *
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

#ifndef CONSOLEGUI_H
#define CONSOLEGUI_H

#include <QObject>
#include <QDebug>


/**
\brief The 'gui' class in the console mode.
*/
class ConsoleGui : public QObject
{
	Q_OBJECT

	public:
		ConsoleGui();
		~ConsoleGui();

		/*
		Shows a sensor distance in centimeters (cm) in a text label.
		@param sensor is the sensor number.
		@param distance is the distance in cm.
		void showDistance(int sensor, int distance);
		*/

		/**
		Shows a sensor distance in a progress bar.
		@param sensor is the sensor number.
		@param distance is the distance in cm.
		@sa SensorThread::convertToSensorValue(), SensorThread::convertToDistance()
		*/
		void showDistanceGraphical(int sensor, int distance);

		/**
		Shows the driven distance in a text label.
		@param sensor is the sensor number.
		@param distance is the distance in cm.
		*/
		void showDrivenDistance(int sensor, int distance);

		/**
		Shows a sensor alarm (turns progressBars red, text in bold etc.)
		@param sensor is the sensor number.
		@param state can be ON or OFF.
		*/
		void showAlarm(short int sensor, bool state);

		/**
		Shows the status and direction of a motor (ON, OFF, RIGHT, LEFT)
		@param motor is the motor number.
		@param power can be ON or OFF.
		@param direction can be FORWARD or BACKWARD.
		*/
		void showMotorStatus(unsigned char motor, bool power, unsigned char direction);


	public slots:
		/**
		Only store the robots state locally.
		@param state can be ON or OFF
		 */
		void setRobotControls(bool state);
		
		/**
		Appends text to the main log in the main window.
		@param text is the text to be displayed.
		@param CR adds a carriage return (CR) to the text, if true (default). This parameter is optional!
		@param sayIt If true, the text is also spoken (default=false). This parameter is optional!
		 */
		void appendLog(QString text, bool CR=true, bool sayIt=false);

		/**
		Append text to the network log in the main window
		@param text is the text to be displayed.
		@param CR adds a carriage return (CR) to the text, if true (default). This parameter is optional!
		@param sayIt If true, the text is also spoken (default=false). This parameter is optional!
		@sa appendLog()
		*/
		void appendNetworkLog(QString text, bool CR=true, bool sayIt=false);

		/**
		Refreshes the view of the lines from the front laser scanner.
		*/
		void refreshLaserViewFront(QList <float> laserScannerValues, QList <int> laserScannerFlags);
		
		/**
		Refreshes the view of the lines from the rear laser scanner.
		*/
		void refreshLaserViewRear(QList <float> laserScannerValues, QList <int> laserScannerFlags);
		
		/**
		Shows the angles of the free area where to drive in lables.
		@param largestFreeAreaStart
		@param largestFreeAreaEnd
		@param centerOfFreeWay
		@param width of the estimated drive-tru area
		*/
		void showLaserFrontAngles(int largestFreeAreaStart, int largestFreeAreaEnd, int centerOfFreeWay, float width);

		/**
		Shows the preferred driving direction in a lable.
		@param direction can be FORWARD, BACKWARD, LEFT or RIGHT
		*/
		void showPreferredDirection(QString direction);

		/**
		Gets the angle of a laser from the @sa laserThread for drawing the correct laser scanner angles (e.g. 180 or 270 degrees)
		@param laserScanner can be LASER1 or LASER2
		@param angle in degrees
		*/
		void setLaserscannerAngle(short int laserscanner, int angle);

		/**
		Gets the resolution of a laser from the @sa laserThread for drawing the correct laser scanner angles (e.g. 0.5 or 1.0 degrees)
		@param laserScanner can be LASER1 or LASER2
		@param angle in degrees
		*/
		void setLaserscannerResolution(short int laserscanner, float resolution);


	signals:
		/**
		Exits the whole programm.
		 */
		void shutdown();

		/**
		Initializes the robots basic circuit.
		 */
		void initCircuit();

		/**
		Initializes the servos.
		 */
		void initServos();

		/**
		Instructs the robot to drive FORWARD, BACKWARD, LEFT and RIGHT. The commands START, STOP or WAIT are also possible.
		@param command
		 */
		void drive(const unsigned char command);

		/**
		Resets the driven distance to 0.
		@param sensor is the sensor number.
		*/
		void resetDrivenDistance(int sensor);

		/**
		Enables or disables the listening for the robot remote control. This signal is sent from the remote control button.
		@param state can be true or false.
		@sa NetworkThread()
		*/
		void enableRemoteControlListening(bool state);

		/**
		Enables or disables the robots simulation mode.
		This signal is sent from the simulation button and received from all threads which deliver real signals from the bot.
		Once the signal is received, the slots switches to simulation mode.
		@param state can be true or false.
		@sa Direcs::setSimulationMode()
		@sa SensorThread::setSimulationMode()
		@sa LaserThread::setSimulationMode()
		@sa ObstackeCheckThread::setSimulationMode()
		*/
		void simulate(bool state);

		/**
		Emits a speak signal. This signal is sent to the speakThread.
		*/
		void speak(QString text);

		/**
		Emits a signal to call the test method in the Direcs class..
		 */
		void test();

	private:
/* 
	TODO:	void closeEvent();
		*/
		/**
		Creates all objects, lines, scene, view etc.
		*/
		void createLaserScannerObjects();
		void createLaserDistanceObjects();

		bool robotIsOn; //! Stores the robots (circuits) state.ON or OFF

		static const int SENSORPROGRESSBARMAXIR = 50; /** max value in cm for ir sensor */
		static const int SENSORPROGRESSBARMAXUS = 400; /** max value in cm for us sensor */
		
		static const float AMPERESMAXPLOTCURVE1 = 3000.0; /// The maximum axis Y value in Amperes (A) for plot curve number 1
		static const float AMPERESMAXPLOTCURVE2 = 3000.0; /// The maximum axis Y value in Amperes (A) for plot curve number 2
		static const float AMPERESSTEPPLOTCURVE1 = 500.0; /// The step value for the y axis for plot curve number 1
		static const float AMPERESSTEPPLOTCURVE2 = 500.0; /// The step value for the y axis for plot curve number 1

		static const bool ON  = true;   /** For motor "ON" */
		static const bool OFF = false;  /** For motor "OFF" */

		/// @sa Direcs::drive() [Slot]
		static const unsigned char START = 7; //! For button signal "start driving"
		static const unsigned char STOP  = 8; //! For button signal "stop driving"
		static const unsigned char MOTOR1FW		= 10; /// for the test widget
		static const unsigned char MOTOR1BW		= 11; /// for the test widget
		static const unsigned char MOTOR1OFF	= 12; /// for the test widget
		static const unsigned char MOTOR2FW		= 13; /// for the test widget
		static const unsigned char MOTOR2BW		= 14; /// for the test widget
		static const unsigned char MOTOR2OFF	= 15; /// for the test widget
		static const unsigned char MOTOR3FW		= 16; /// for the test widget
		static const unsigned char MOTOR3BW		= 17; /// for the test widget
		static const unsigned char MOTOR3OFF	= 18; /// for the test widget
		static const unsigned char MOTOR4FW		= 19; /// for the test widget
		static const unsigned char MOTOR4BW		= 20; /// for the test widget
		static const unsigned char MOTOR4OFF	= 21; /// for the test widget

		static const unsigned char CLOCKWISE        = 0;  /// Motor direction "CLOCKWISE"
		static const unsigned char COUNTERCLOCKWISE = 1;  /// Motor direction "COUNTERCLOCKWISE
		static const unsigned char SAME             = 3;  /// Motor direction/power "same like before"
		static const unsigned char MOTOR1           = 10; /// Motor 1 front left
		static const unsigned char MOTOR2           = 20; /// Motor 2 front right
		static const unsigned char MOTOR3           = 30; /// Motor 3 back left
		static const unsigned char MOTOR4           = 40; /// Motor 4 back right

		/**
		Give the sensors some names

		DONT CHANGE THIS NUMBERS!
		THEY ARE ALSO USED TO ADRESS THE ARRAY "iRSensorValue[]" !!
		*/
		static const short int SENSOR1 = 1;
		static const short int SENSOR2 = 2;
		static const short int SENSOR3 = 4;
		static const short int SENSOR4 = 8;
		static const short int SENSOR5 = 16;
		static const short int SENSOR6 = 32;
		static const short int SENSOR7 = 64;
		static const short int SENSOR8 = 128;

		/** ultrasonic sensor */
		static const short int SENSOR16 = 256;
		/** Value if no sensor has a value to react */
		static const short int NONE = 0;

		static const short int MOTORSENSOR1 = 0;
		static const short int MOTORSENSOR2 = 1;

		/**
		Factor for fitting 6 meters (measured from the laser scanner) into a frame with a height of 270 pixels at a default zoom level of 5 !!
		Example:

		laservalue = 0.18 cm
		fit factor = 45
		zoom level = 5

		0.18 x 45 x 5 = 40.5 Pixel -> 41 Pixel is the length of the laser line in the GUI
		*/
		static const int FITTOFRAMEFACTOR = 45;
		static const int STARTZOOMLEVEL = 3;

		//! the initial Y position of the laser lines and the robot image in the GUI
		static const int INITIALLASERYPOSFRONT = 100;
		static const int INITIALLASERYPOSREAR  = 100;

		//! Number of distance semi circles
		static const int LASERDISTANCECIRCLES = 27;
		//! the size (diameter) of the first (innerst) circle in pixels
		static const int LASERDISTANCEFIRSTCIRCLE = 82;
		//! Distances between the lines to show the laser distances in pixels
		static const int LASERDISTANCEDISTANCE = 60;

		// the tags for the laser lines
		static const int FREEWAY = 0;
		static const int OBSTACLE = 1;
		static const int LARGESTFREEWAY = 2;
		static const int CENTEROFLARGESTFREEWAY = 3;

		// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		/**
		Defines the size of the laserLines[] array
		This es equal to the number of degrees.
		See also laserThread.h */
		//static const unsigned char LASERSCANNERARRAYSIZE = 181;

		/**
		For the laserThread
		*/
		static const short int LASER1 = 1;
		static const short int LASER2 = 2;
		static const short int LASER3 = 4;
		static const short int LASER4 = 8;
		static const short int LASER5 = 16;
};
#endif
