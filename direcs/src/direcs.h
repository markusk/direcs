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

#ifndef DIRECS_H
#define DIRECS_H

//-------------------------------------------------------------------
#include <QtGlobal> // for Q_OS_* Makro!
#include <QObject>
#include <QtDebug>
#include <QMutex>
#include <QSocketNotifier>
#include <QtGui>
#include <QSplashScreen>
#include <QMetaType>
#include <QtCore/QCoreApplication>
//-------------------------------------------------------------------
#include "consoleGui.h"
#include "gui.h"
#include "joystickDialog.h"
#include "settingsDialog.h"
#include "aboutDialog.h"

#ifndef BUILDFORROBOT
	#include "plotThread.h"
#endif

#include "circuit.h"
#include "interfaceAvr.h"
//#include "heartbeat.h"
#include "sensorThread.h"
#include "motor.h"
#include "servo.h"
#include "obstacleCheckThread.h"
#include "inifile.h"
#include "logfile.h"
#include "networkThread.h"
#include "laserThread.h"
#include "joystick.h"
#include "head.h"
//#include "camThread.h"
#include "speakThread.h"

#ifdef Q_OS_MAC // FIXME: Using OpenCV 2.1 under Mac OS X only.
	#include "glwidget.h"
#endif

//-------------------------------------------------------------------
#include <signal.h> // for SIGINT
using namespace std;

struct CleanExit
{
	CleanExit()
	{
		signal(SIGINT, &CleanExit::exitQt);
		signal(SIGTERM, &CleanExit::exitQt);
		// signal(SIGBREAK, &CleanExit::exitQt) ;
	}

	static void exitQt(int sig)
	{
		Q_UNUSED(sig);
		// this is now called in the console mode, when hitting ctrl+c (SIGINT)
		// emit a signal here to call shutdown?
		QCoreApplication::quit(); // FIXME: threads are not ended!
	}
};
//-------------------------------------------------------------------


/**
\brief The main class of the whole robot programm.

 All objects are created here. This method also starts the GUI and its splash screen.
*/
class Direcs : public QObject
{
	Q_OBJECT


	public:
		Direcs(bool bConsoleMode);
		~Direcs();


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
		Instructs the robot to drive FORWARD, BACKWARD, LEFT, RIGHT, TURNLEFT and TURNRIGHT. The commands START, STOP or WAIT are also possible.
		@param command
		*/
		void drive(const unsigned char command);

		/**
		Increases the driving speed to its maximum speed. This is used to have a smooth driveaway of the robot and works together with a timer.
		*/
		void increaseDrivingSpeed(void);

		/**
		The locialUnit handles the behavior of the robot. This slot is called if an obstacle is detected from the obstacleCheckThread.
		It lets the robot "react" depending on which sensor made the alarm. The number of alarms are also counted.
		@param sensorAlarm consists of the sum of all infrared and ultrasonic sensor numbers, which had an alarm.
		@param timestamp is the timestmap when the signal was emitted.
		*/
		void logicalUnit(int sensorAlarm, QDateTime timestamp);

		/**
		This method moves the camera and/or the robots eyes in the direction of a detected face.
		@sa camThread::faceDetected()
		 */
		void faceTracking(int faces, int faceX, int faceY, int faceRadius);

		/**
		Enables or disables the face tracking. When activated, a face will be tracked by the camera.
		@param state has to be Qt::Checked to enable the detection. All other states disable.
		*/
		void enableFaceTracking(int state);

		/**
		Shows the sensor data in a graphical way (progressBars) and as a number in the GUI.
		This slot is called from the Signal sensorDataComplete from the sensorThread.
		*/
		void showSensorData();

		/**
		Shows the yes-no exit dialog for the main program.
		This slot is called when massive communication errors occur while talking to the robots interface!
		*/
		void showExitDialog();

		/**
		Enables or disables the listening for the robot remote control.
		This slot is called from an event from the GUI (enable remote button).
		@param status
		*/
		void enableRemoteControlListening(bool status);

		/**
		This slot executes remote commands it receives via network (e.g. WLAN).
		*/
		void executeRemoteCommand(QString command);

		/**
		This slot executes commands from a connected joystick.
		*/
		void executeJoystickCommand(int axisNumber, int axisValue);
		void executeJoystickCommand(int buttonNumber, bool buttonState);

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

		/*
		/ **
		This slot sets (stores) the robots (circuits) state within this class.
		@param state can be ON or OFF
		 * /
		void setRobotState(bool state);
		*/

		/**
		Ends the programm, after asking "really?".
		Calls the destructor of this class, after saving settings and some other exit stuff.
		*/
		void shutdown();

		/**
		This slot catches all signals from the signal @sa systemerror
		*/
		void systemerrorcatcher(int errorlevel);

		/**
		The method for the test button in the GUI.
		 */
		void test();


	signals:
		/**
		Shows the preferred driving direction of the robot in the GUI
		@sa logicalUnit()
		*/
		void showPreferredDirection(QString direction);

		/**
		Let the other classes know if we are in the console mode (activated via command-line argument)
		@sa Gui::setConsoleMode()
		*/
		void publishConsoleMode(bool state);

		/**
		Shows the face tracking direction in a lable.
		 */
		void showFaceTrackDirection(QString direction);

		/**
		This is for the robots head.
		@param direction where to look can be LEFT, RIGHT...
		*/
		void look(QString direction);

		/**
		Emits a speak signal. This signal is sent to the speakThread.
		*/
		void speak(QString text);

		/**
		Initialize the robots basic circuit.
		*/
		void initCircuit();

		/**
		Sends a string over the network.
		*/
		void sendNetworkString(QString text);

		/**
		Emits a string to the GUI log / console.
		@param text is the message to be emitted
		@param CR is true when a CR/LF should be added at the end of the line (default)
		@param sayIt is true when the text for the log also should be spoken (default=false)
		*/
		void message(QString text, bool CR=true, bool sayIt=false);

		/**
		Emits a string to the splash screen.
		@param text is the message to be emitted
		*/
		void splashMessage(QString text);


	private:
		/**
		Reads all settings for the robot from an ini-file.
		*/
		void readSettings();

		/**
		Checks the command-line arguments passed to the program.
		*/
		void checkArguments();

		/**
		Resets the driving speed timer, so at the next drive command, the robots start with a smooth drive - not with maximum speed.
		@sa drive()
		@sa increaseDrivingSpeed()
		*/
		void resetDrivingSpeedTimer(void);


		mutable QMutex *mutex; // make the threads thread-safe (e.g. senorThread, servo...)

		//Events *myEvent; /// my event filter for grabbing ctrl+c in the console mode
		Gui *gui;
		ConsoleGui *consoleGui;
#ifndef _ARM_ // only include on _non_ ARM environments!
		SettingsDialog *settingsDialog; /// The settings dialog
		JoystickDialog *joystickDialog; /// The joystick dialog
		AboutDialog *aboutDialog;       /// The about dialog

#ifndef BUILDFORROBOT
		PlotThread *plotThread;
#endif

		QSplashScreen *splash;
#ifndef _ARM_ // only include on _non_ ARM environments!
		Qt::Alignment splashPosition; 	/// The splash screen position
		QColor splashTextColor; 			/// The splash screen text color
#endif
		QStringList arguments;			/// contains the list of command-line arguments
#endif
		Circuit *circuit1;
		InterfaceAvr *interface1;
		// TODO: Heartbeat *heartbeat;
		SensorThread *sensorThread;
		Motor *motors;
		Servo *servos;
		ObstacleCheckThread *obstCheckThread;
		Inifile *inifile1;
		NetworkThread *netThread;
		LaserThread *laserThread;
		//CamThread *camThread; todo: kinect stuff
		SpeakThread *speakThread;
		Joystick *joystick;
		Head *head;
		Logfile *logfile;
		QKinect *kinect; /// pointer to control the Kinect
//		GLWidget *glWidget; /// this is for the OpenCV stuff
		QString serialPortMicrocontroller;
		QString serialPortLaserscannerFront;
		QString serialPortLaserscannerRear;
		QString laserscannerTypeFront; // PLS, LMS, S300. just for reading the value here. @sa laserThread
		QString laserscannerTypeRear;  // PLS, LMS, S300. just for reading the value here. @sa laserThread
		int laserscannerAngleFront; // this values holds the range of the laserscanner viewing angle. e.g. 180 oder 270 degrees.  just for reading the value here. @sa laserThread
		int laserscannerAngleRear; // this values holds the range of the laserscanner viewing angle. e.g. 180 oder 270 degrees.  just for reading the value here. @sa laserThread
		QString laserscannerMounting; // just for reading the value here. @sa laserThread
		QString commaSeparator;
		QTimer *drivingSpeedTimer;
		//bool robotIsOn; /// Stores the robots (circuits) state.ON or OFF
		bool robotDrives; /// Stores the robots driving state. TRUE, when the robot drives.
		bool robotSimulationMode; /// Stores the robots simulation state
		bool robotRemoteMode; /// Stores the state, if the remote mode is enabled (control via LAN or joystick es enabled
		bool writeLogFile; /// if true, a logfile is written of all pgm messages (see ini-file)
		bool useCamera; /// if a camera is connected, but should not be used (just for testing, for a faster program start)
		bool laserScannerFrontFound;
		bool laserScannerRearFound;
		bool testDriveMode; /// is enabled, when the robot is controlled via joystick
		bool mecanumDriveMode; /// is enabled, when the robot is controlled via joystick and should use special mecanum wheel driving
		bool servoTestMode;
		bool eyeTestMode;
		int currentTestServo;
		bool cameraTestMode;
		bool faceTrackingIsEnabled;
		int mot1Speed;
		int mot2Speed;
		int mot3Speed;
		int mot4Speed;
		int minimumSpeed;
		int maximumSpeed;
		bool endSpeedMotor1Reached;
		bool endSpeedMotor2Reached;
		bool endSpeedMotor3Reached;
		bool endSpeedMotor4Reached;
		unsigned int value;
		float floatValue;
		bool consoleMode; /// is enabled if the argument 'console' was passed by command-line. Sends all GUI messages to the command line.
		bool shutdownAlreadyCalled;
		bool forceShutdown; /// this is set to true, when no ini-file was found at startup. This make the program leave without the stupid AreYouSure-Question.
		QList <QDateTime> obstacleAlarmFrontLeftList;			/// A list of obstacle alarms that occured left the last n times.
		QList <QDateTime> obstacleAlarmFrontRightList;		/// A list of obstacle alarms that occured right the last n times.
		//QList <QDateTime> obstacleAlarmLeftTimestampList;	/// A list of the timestamps of the obstacle alarms that left occured.
		//QList <QDateTime> obstacleAlarmRightTimestampList;	/// A list of the timestamps of the obstacle alarms that right occured.

		/// The splash screen time of view in ms
		static const unsigned int SPLASHTIME  = 2000;

		/// The time between one de/increase of the motor speed (to its minimum/maximum) and the next in ms
		static const unsigned int DRIVINGSPEEDINCREASER = 10;

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
		static const unsigned char ALLMOTORS		= 90; //! used the hole robot gets one command for all motors (like 'forwardall'). This is so save transmissions on the serial line.

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
		/// @sa Joystick()
		static const int JOYSTICKAXISY2 = 2; // ok
		static const int JOYSTICKAXISX3 = 3; // ok
		static const int JOYSTICKAXISX4 = 4;
		static const int JOYSTICKAXISY5 = 5;
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

		/**
		The GUI LED colors
		*/
		static const unsigned char LEDOFF	= 0;
		static const unsigned char RED		= 1;
		static const unsigned char GREEN	= 2;
};

#endif
