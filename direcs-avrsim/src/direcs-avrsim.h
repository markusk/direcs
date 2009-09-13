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

#ifndef DIRECSAVRSIM_H
#define DIRECSAVRSIM_H

//-------------------------------------------------------------------
#include "gui.h"
#include "interfaceAvr.h"
//-------------------------------------------------------------------
#include <QObject>
#include <QtDebug>
#include <QMutex>
#include <QSocketNotifier>
#include <QtGui>
#include <QSplashScreen>
//-------------------------------------------------------------------

/**
\brief The main class of the whole robot simulation programm.

 All objects are created here. This method also starts the GUI and its splash screen.
*/
class DirecsAvrsim : public QObject
{
    Q_OBJECT


	public:
		DirecsAvrsim(bool bConsoleMode);
		~DirecsAvrsim();


		/**
		Initalises the complete program and robot. Conntect all signals and slots.
		*/
		void init();

		/**
		Returns the status of the main program. True, if the simulationMode ist active.
		*/
		bool simulationMode() const;

		/**
		Currently not in use...
		*/
		bool noHardwareErrorMessages;

		/**
		Configures if an exit dialog should be shown, when exiting the main program.
		*/
		bool exitDialog;


	public slots:
		/**
		This slot enables or disables the simulation mode.
		@param status
		*/
		void setSimulationMode(bool status);

		/**
		Shows a text on the current splash screen.
		*/
		void showSplashMessage(QString text);

		/**
		Finishes the splash screen and displays the main window.
		*/
		void finishSplash();

		/**
		Ends the programm, after asking "really?".
		Calls the destructor of this class, after saving settings and some other exit stuff.
		*/
		void shutdown();

		/**
		The method for the test button in the GUI.
		 */
		void test();


	signals:
		/**
		Emits a string to the GUI log / console.
		@param text is the message to be emitted
		*/
		void message(QString text, bool CR=true, bool sayIt=false);

		/**
		Emits a string to the splash screen.
		@param text is the message to be emitted
		*/
		void splashMessage(QString text);


	private:
		/**
		Checks the command-line arguments passed to the program.
		*/
		void checkArguments();

		mutable QMutex *mutex; // make the threads thread-safe (e.g. senorThread, servo...)

		//Events *myEvent; /// my event filter for grabbing ctrl+c in the console mode
		Gui *gui;
		QSplashScreen *splash;
		Qt::Alignment splashPosition; 	/// The splash screen position
		QColor splashColor; 			/// The splash screen text color
		QStringList arguments;			/// contains the list of command-line arguments
		InterfaceAvr *interface1;
		QString serialPortMicrocontroller;
		QString commaSeparator;
		bool robotSimulationMode; /// Stores the robots simulation state
		unsigned int value;

		/// The splash screen time of view in ms
		static const unsigned int SPLASHTIME  = 500;

		/// The time between one de/increase of the motor speed (to its minimum/maximum) and the next in ms
		static const unsigned int DRIVINGSPEEDINCREASER = 200;

		static const bool ON  = true;   /** For motor or robot "ON" */
		static const bool OFF = false;  /** For motor or robot "OFF" */

		/// Some driving directions for the robot. @sa drive() [Slot]
		static const unsigned char FORWARD		= 1;
		static const unsigned char BACKWARD		= 2;
		static const unsigned char LEFT			= 3;
		static const unsigned char RIGHT		= 4;
		static const unsigned char TURNLEFT		= 5;
		static const unsigned char TURNRIGHT	= 6;
		static const unsigned char START		= 7;
		static const unsigned char STOP			= 8;
		static const unsigned char WAIT			= 9;
		// static const unsigned char DIAGONAL_FORWARD_LEFT
		// static const unsigned char DIAGONAL_FORWARD_RIGHT
		// static const unsigned char DIAGONAL_BACKWARD_LEFT
		// static const unsigned char DIAGONAL_BACKWARD_RIGHT
		static const unsigned char MOTOR1FW		= 10;
		static const unsigned char MOTOR1BW		= 11;
		static const unsigned char MOTOR1OFF	= 12;
		static const unsigned char MOTOR2FW		= 13;
		static const unsigned char MOTOR2BW		= 14;
		static const unsigned char MOTOR2OFF	= 15;
		static const unsigned char MOTOR3FW		= 16;
		static const unsigned char MOTOR3BW		= 17;
		static const unsigned char MOTOR3OFF	= 18;
		static const unsigned char MOTOR4FW		= 19;
		static const unsigned char MOTOR4BW		= 20;
		static const unsigned char MOTOR4OFF	= 21;

		static const unsigned char READ_AXIS_X = 61;
		static const unsigned char READ_AXIS_Y = 62;
		static const unsigned char READ_AXIS_Z = 63;

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

		////The ultrasonic sensor
		static const short int SENSOR16 = 256;

		/// Value, if no sensor has a value to react
		static const short int NONE = 0;

		static const short int OBSTACLEFRONTLEFT = 512;
		static const short int OBSTACLEFRONTRIGHT = 1024;
		static const short int OBSTACLESEVERYWHEREINFRONT = 2048;

		static const short int MAXFALSEALARMS = 2; /// number of false alarms which are allowed in the logical unit

		static const unsigned char CLOCKWISE        = 0;  /// Motor direction "CLOCKWISE"
		static const unsigned char COUNTERCLOCKWISE = 1;  /// Motor direction "COUNTERCLOCKWISE"
		static const unsigned char SAME             = 3;  /// Motor direction/power "same like before"
		static const unsigned char MOTOR1           = 10; /// Motor 1 front left
		static const unsigned char MOTOR2           = 20; /// Motor 2 front right
		static const unsigned char MOTOR3           = 30; /// Motor 3 back left
		static const unsigned char MOTOR4           = 40; /// Motor 4 back right

		/// The motor sensors
		static const short int MOTORSENSOR1 = 0;
		static const short int MOTORSENSOR2 = 1;

		/// The voltage sensors
		static const short int VOLTAGESENSOR1 = 0;
		static const short int VOLTAGESENSOR2 = 1;
		
		/// The minimum voltages for an acoustic and optical alarm. @sa Gui::showVoltage
		static const float MINIMUMVOLTAGE1 = 10.80; // TODO: put min. voltage 1 in the ini-file
		static const float MINIMUMVOLTAGE2 = 21.60; // TODO: put min. voltage 2 in the ini-file

		/// The servos
		static const unsigned char NUMBEROFSERVOS = 6; // TODO: also defined in servo.h !!!
		/// the servo numbers
		static const unsigned char SERVO1 = 0;
		static const unsigned char SERVO2 = 1;
		static const unsigned char SERVO3 = 2;
		static const unsigned char SERVO4 = 3;
		static const unsigned char SERVO5 = 4;
		static const unsigned char SERVO6 = 5;

		/// the possible servo data
		static const unsigned char SVSTART   = 0;
		static const unsigned char SVEND     = 1;
		static const unsigned char SVDEFAULT = 2;
		static const unsigned char SVCURRENT = 3;
		static const unsigned char SVMIN     = 4;
		static const unsigned char SVMAX     = 5;

		// TODO: put axis numbers to ini-file
		/// Joystick axis numbers
		static const int JOYSTICKAXISY = 2;
		static const int JOYSTICKAXISX = 3;
		static const int JOYSTICKAXIS2X = 4;
		static const int JOYSTICKAXIS2Y = 5;
		/// Joystick conversion divisor. Converts the joystick axis value to the robot speed.
		static const int JOYSTICKDIVISOR = 128;

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
