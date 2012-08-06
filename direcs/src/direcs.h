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
#include <Phonon>
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
#include "rgbLed.h"
#include "obstacleCheckThread.h"
#include "inifile.h"
#include "logfile.h"
#include "networkThread.h"
#include "laserThread.h"
#include "joystick.h"
#include "head.h"
#include "camThread.h"
#include "speakThread.h"
#include "timerThread.h"
#include "partyThread.h"


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
\author Markus Knapp
\brief The main class of the whole robot programm.

 All objects are created here. This method also starts the GUI and its splash screen.
*/
class Direcs : public QObject
{
	Q_OBJECT


	public:
		/**
		The parameters are given from a main method which gets them from the console (startup arguments).
		*/
		Direcs(bool bConsoleMode, bool bForceSmallGUI, bool bForceLargeGUI);
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
		void drive(const int command);

		/**
		Increases the driving speed to its maximum speed. This is used to have a smooth driveaway of the robot and works together with a timer.
		*/
		void increaseDrivingSpeed(void);

		/**
		The locialUnit handles the behavior of the robot. This slot is called if an obstacle is detected from the obstacleCheckThread.
		It lets the robot "react" depending on which sensor made the alarm. The number of alarms are also counted.
		The class member preferredDrivingDirection is also set here and will be used when the robot gets the @sa drive() command START.
		@param sensorAlarm consists of the sum of all infrared and ultrasonic sensor numbers, which had an alarm  *or* if we use a laser scanner, a value like OBSTACLEFRONTLEFT.
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
		Sets the network state of this program.
		If it receives / received a 'master' strin form another direcs program, running on a robot,
		it sets *this* program to slave mode. Otherwise we will run in (normal) master mode.
		*/
		void setNetworkState();

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
		@sa Gui::appendLog()
		@param text is the message to be emitted
		@param CR is true when a CR/LF should be added at the end of the line (default)
		@param sayIt is true when the text for the log also should be spoken (default=false)
		@param addTimestamp If true, the a timestamp is added in front of the text. This parameter is optional.
		*/
		void message(QString text, bool CR=true, bool sayIt=false, bool addTimestamp=true);

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

		/**
		Turns the driving light to RED, GREEN, BLUE, WHITE or LEDOFF.
		@param color
		@todo Move the drivingLight method to a better file/place.
		*/
		void drivingLight(unsigned char color);


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
		QColor splashTextColor; 			/// The splash screen text color
#endif
		QStringList arguments;			/// contains the list of command-line arguments
#endif
		Circuit *circuit1;
		InterfaceAvr *interface1;
		/// \todo Heartbeat *heartbeat;
		SensorThread *sensorThread;
		Motor *motors;
		RgbLed *rgbLeds;
		Servo *servos;
		ObstacleCheckThread *obstCheckThread;
		Inifile *inifile1;
		NetworkThread *netThread;
		LaserThread *laserThread;
		CamThread *camThread;
		SpeakThread *speakThread;
		Joystick *joystick;
		Head *head;
		Logfile *logfile;
		TimerThread *timerThread;
		PartyThread *partyThread;
		QString serialPortMicrocontroller;
		QString serialPortLaserscannerFront;
		QString serialPortLaserscannerRear;
		QString laserscannerTypeFront; // PLS, LMS, S300. just for reading the value here. @sa laserThread
		QString laserscannerTypeRear;  // PLS, LMS, S300. just for reading the value here. @sa laserThread
		int laserscannerAngleFront; // this values holds the range of the laserscanner viewing angle. e.g. 180 oder 270 degrees.  just for reading the value here. @sa laserThread
		int laserscannerAngleRear; // this values holds the range of the laserscanner viewing angle. e.g. 180 oder 270 degrees.  just for reading the value here. @sa laserThread
		int laserscannerIgnoreAreaStart; // this is only temporary needed when reading a setting
		int laserscannerIgnoreAreaEnd; // this is only temporary needed when reading a setting
		QString laserscannerMounting; // just for reading the value here. @sa laserThread
		QString commaSeparator;
		QTimer *drivingSpeedTimer;
		int preferredDrivingDirection; /// @sa logicalUnit
		//bool robotIsOn; /// Stores the robots (circuits) state.ON or OFF
		bool robotDrives; /// Stores the robots driving state. TRUE, when the robot drives.
		bool robotSimulationMode; /// Stores the robots simulation state
		bool robotRemoteMode; /// Stores the state, if the remote mode is enabled (control via LAN or joystick es enabled
		bool iAmTheMaster; /// Indicates if this program is the master and sends data from the robot over the network.
		bool firstDataReceived; /// Indicates that network data were received already and we do not check if we are master or slave
		unsigned int networkPortListen;
		unsigned int networkPortSend;
		bool writeLogFile; /// if true, a logfile is written of all pgm messages (see ini-file)
		bool useCamera; /// if a camera is connected, but should not be used (just for testing, for a faster program start)
		bool laserScannerFrontFound;
		bool laserScannerRearFound;
		bool testDriveMode; /// is enabled, when the robot is controlled via joystick
		bool mecanumDriveMode; /// is enabled, when the robot is controlled via joystick and should use special mecanum wheel driving
		int  currentPreferredDirection;
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
		bool forceSmallGUI; /// is enabled if the argument 'small' was passed by command-line. This will result in a startup with the small GUI - regardless of the hostname.
		bool forceLargeGUI; /// is enabled if the argument 'large' was passed by command-line. This will result in a startup with the large GUI - regardless of the hostname.
		bool shutdownAlreadyCalled;
		bool forceShutdown; /// this is set to true, when no ini-file was found at startup. This make the program leave without the stupid AreYouSure-Question.
		bool useSmallGUI; /// if true, we will use a small GUI for fitting on a small onboard TFT. By default this is decided by the local hostname of the system. @sa Gui::Gui()
		QList <QDateTime> obstacleAlarmFrontLeftList;			/// A list of obstacle alarms that occured left the last n times.
		QList <QDateTime> obstacleAlarmFrontRightList;		/// A list of obstacle alarms that occured right the last n times.
		//QList <QDateTime> obstacleAlarmLeftTimestampList;	/// A list of the timestamps of the obstacle alarms that left occured.
		//QList <QDateTime> obstacleAlarmRightTimestampList;	/// A list of the timestamps of the obstacle alarms that right occured.
		Phonon::MediaObject *mediaObject; /// A Phonon media player for e.g. playing a mp3 file
		bool partyMode;

		/// The splash screen time of view in ms
		static const unsigned int SPLASHTIME  = 2000;

		/// The time between one de/increase of the motor speed (to its minimum/maximum) and the next in ms
		static const unsigned int DRIVINGSPEEDINCREASER = 10;

		static const bool ON  = true;   /** For motor or robot "ON" */
		static const bool OFF = false;  /** For motor or robot "OFF" */

		/// Some driving directions *and* motor directions for the robot. @sa Direcs::drive() [Slot]
		static const int FORWARD    = 10; /// Motor direction (formerly "clockwise")
		static const int BACKWARD   = 20; /// Motor direction (formerly "counterclockwise"
		static const int LEFT		= 30;
		static const int RIGHT		= 40;
		static const int TURNLEFT	= 50;
		static const int TURNRIGHT	= 60;
		static const int START		= 70;
		static const int STOP		= 80;
		static const int WAIT		= 90;
		// static const int DIAGONAL_FORWARD_LEFT
		// static const int DIAGONAL_FORWARD_RIGHT
		// static const int DIAGONAL_BACKWARD_LEFT
		// static const int DIAGONAL_BACKWARD_RIGHT

		static const int SAME      = 200; /// Motor direction/power "same like before"
		static const int MOTOR1    = 210; /// Motor 1 front left
		static const int MOTOR2    = 220; /// Motor 2 front right
		static const int MOTOR3    = 230; /// Motor 3 back left
		static const int MOTOR4    = 240; /// Motor 4 back right
		static const int ALLMOTORS = 250; /// used for letting the robot getting only one command for all motors (like 'forward all'). This is to reduce commands on the serial line.

		static const int MOTOR1FW	= 300;
		static const int MOTOR1BW	= 310;
		static const int MOTOR1OFF	= 320;
		static const int MOTOR2FW	= 330;
		static const int MOTOR2BW	= 340;
		static const int MOTOR2OFF	= 350;
		static const int MOTOR3FW	= 360;
		static const int MOTOR3BW	= 370;
		static const int MOTOR3OFF	= 380;
		static const int MOTOR4FW	= 390;
		static const int MOTOR4BW	= 400;
		static const int MOTOR4OFF	= 410;

		static const int READ_AXIS_X = 500;
		static const int READ_AXIS_Y = 510;
		static const int READ_AXIS_Z = 520;

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


		/// The motor sensors
		static const short int MOTORSENSOR1 = 0;
		static const short int MOTORSENSOR2 = 1;

		/// The voltage sensors
		static const short int VOLTAGESENSOR1 = 0;
		static const short int VOLTAGESENSOR2 = 1;

		/// The minimum voltages for an acoustic and optical alarm. @sa Gui::showVoltage
		static const float MINIMUMVOLTAGE1 = 10.80; /// \todo put min. voltage 1 in the ini-file
		static const float MINIMUMVOLTAGE2 = 21.60; /// \todo put min. voltage 2 in the ini-file

		/// The servos
		static const unsigned char NUMBEROFSERVOS = 6; /// @todo also defined in servo.h !!!
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

		/// the RGB LED numbers
		static const unsigned char NUMBEROFRGBLEDS = 6; /// @todo also defined in rgbled.h !!!
		static const unsigned char RGBLED1 = 0;
		static const unsigned char RGBLED2 = 1;
		static const unsigned char RGBLED3 = 2;
		static const unsigned char RGBLED4 = 3;
		static const unsigned char RGBLED5 = 4;
		static const unsigned char RGBLED6 = 5;
		/// the possible rgbLed data
		static const unsigned char RGBLEDDEFAULT = 0;
		static const unsigned char RGBLEDACTUAL  = 1;
		static const unsigned char RGBLEDMIN     = 2;
		static const unsigned char RGBLEDMAX     = 3;
		static const int MINPWM = 1;  /// this is analog to the value in  microcontroller software @sa direcsSTM
		static const int MAXPWM = 99; /// this is analog to the value in  microcontroller software @sa direcsSTM

		/// \todo put axis numbers to ini-file
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
		For the obstacleCheckThread
		*/
		static const int AREA1 = 1;
		static const int AREA2 = 2;

		/**
		The GUI LED colors. Also used for @sa drivingLight()
		*/
		static const unsigned char LEDOFF	= 0;
		static const unsigned char RED		= 1;
		static const unsigned char GREEN	= 2;
		static const unsigned char BLUE	    = 3;
		static const unsigned char WHITE	= 4;
};

#endif
