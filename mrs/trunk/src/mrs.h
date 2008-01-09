#ifndef MRS_H
#define MRS_H

//-------------------------------------------------------------------
#include "gui.h"
//#include "speakThread.h"
#include "circuit.h"
#include "interface.h"
#include "sensorThread.h"
#include "plotThread.h"
#include "motor.h"
#include "obstacleCheckThread.h"
#include "inifile.h"
#include "networkThread.h"
#include "laserThread.h"
#include "joystick.h"
//-------------------------------------------------------------------
#include <festival.h> // FIXME: instead of speakThread-include!
#include <QtGui>
#include <QSplashScreen>
//-------------------------------------------------------------------

//-------------------------------------------------------------------
// forward declarations because of circular includes!
class Gui;
//-------------------------------------------------------------------

/**
\brief The main class of the whole robot programm.
 
 All objects are created here. This method also starts the GUI and its splash screen.
*/
class Mrs : public QObject
{
    Q_OBJECT


	public:		
		Mrs();
		~Mrs();
		
		/**
		Instructs the robot to drive FORWARD, BACKWARD, LEFT and RIGHT. The commands START, STOP or WAIT are also possible.
		@param command
		*/
		void drive(const unsigned char command);

		//void setMinObstacleDistance(short int value);
		//void startStopCommandClock(int command);

		/**
		The method for the test button in the GUI
		*/
		void test();
	
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
		The locialUnit handles the behavior of the robot. This slot is called if an obstacle is detected from the obstacleCheckThread.
		It lets the robot "react" depending on which sensor made the alarm. The number of alarms are also counted.
		@param sensorAlarm consists of the sum of all infrared and ultrasonic sensor numbers, which had an alarm.
		*/
		void logicalUnit(int sensorAlarm);
		
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
		Finishes the splash screen and displays the main window.
		*/
		void finishSplash();
		
		/**
		Speaks a text with festival. All HTML-Code in the parameter (text) is also removed internally).
		@param text is the text to speak.
		*/
		// FIXME: use speakThread instead!!
		void speak(QString text);


	signals:
		void showPreferredDirection(QString direction);
	
		/**
		Emits a speak signal. This signal is sent to the speakThread.
		*/
		//void speak(QString text);
		
		/**
		Initialize the robots basic circuit.
		*/
		void initCircuit();


	private:
		/**
		Reads all settings for the robot from an ini-file.
		*/
		void readSettings();
		
		QString serialPortMicrocontroller;
		QString serialPortLaserscannerFront;
		QString serialPortLaserscannerRear;
		Gui *gui1;
		Circuit *circuit1;
		InterfaceAvr *interface1;
		SensorThread *sensorThread;
		PlotThread *plotThread;
		Motor *motors;
		ObstacleCheckThread *obstCheckThread;
		Inifile *inifile1;
		NetworkThread *netThread;
		LaserThread *laserThread;
		//SpeakThread *speakThread;
		Joystick *joystick;
		QString commaSeparator;
		bool robotDrives; //! Stores the robots driving state. TRUE, when the robot drives.
		bool robotSimulationMode; //! Stores the robots simulation state
		int mot1Speed;
		int mot2Speed;
		int minimumSpeed;
		int maximumSpeed;


		//! The splash screen time of view in ms
		static const unsigned int SPLASHTIME  = 2000;
		
		static const unsigned char ON  = 1;  //! For motor "ON"
		static const unsigned char OFF = 0;  //! For motor "OFF"
		
		//! Some driving directions for the robot
		static const unsigned char FORWARD = 1;
		static const unsigned char BACKWARD = 2;
		static const unsigned char LEFT = 3;
		static const unsigned char RIGHT = 4;
		static const unsigned char START = 5;
		static const unsigned char STOP = 6;
		static const unsigned char WAIT = 7;
		
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
		
		//! The ultrasonic sensor
		// TODO: make these values nicer
		static const short int SENSOR16 = 256;
		
		//! Value, if no sensor has a value to react
		static const short int NONE = 0;
		
		// TODO: make these values nicer
		static const short int OBSTACLEFRONTLEFT = 512;
		static const short int OBSTACLEFRONTRIGHT = 1024;
		static const short int OBSTACLESEVERYWHEREINFRONT = 2048;
		
		static const unsigned char CLOCKWISE        = 0;  //! Motor direction "CLOCKWISE"
		static const unsigned char COUNTERCLOCKWISE = 1;  //! Motor direction "COUNTERCLOCKWISE"
		static const unsigned char SAME             = 3;  //! Motor direction/power "same like before"
		static const unsigned char MOTOR1           = 10; //! Motor 1
		static const unsigned char MOTOR2           = 20; //! Motor 2
		
		//! The motor sensors
		static const short int MOTORSENSOR1 = 0;
		static const short int MOTORSENSOR2 = 1;
		
		// TODO: put axis numbers to ini-file
		//! Joystick axis numbers
		static const int JOYSTICKAXISX = 3;
		static const int JOYSTICKAXISY = 2;
		//! Joystick conversion divisor. Converts the joystick axis value to the robot speed.
		static const int JOYSTICKDIVISOR = 128;
};

#endif
