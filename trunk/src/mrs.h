#ifndef MRS_H
#define MRS_H

//-------------------------------------------------------------------
#include "gui.h"
#include "speakThread.h"
#include "circuit.h"
#include "interface.h"
#include "sensorThread.h"
#include "plotThread.h"
#include "motor.h"
//#include "servo.h"
#include "obstacleCheckThread.h"
#include "inifile.h"
#include "camThread.h"
#include "networkThread.h"
#include "laser.h"
#include "laserThread.h"
//-------------------------------------------------------------------
#include <QtGui>
#include <QSplashScreen>
//-------------------------------------------------------------------

//-------------------------------------------------------------------
// forward declarations because of circular includes!
class Gui;
//-------------------------------------------------------------------

/*!
\brief The main class of the whole robot programm

All objects are created here. This method also starts the GUI.
*/

class Mrs : public QObject
{
    Q_OBJECT


	public:
		Mrs();
		~Mrs();
		void drive(unsigned char direction);
		void setMinObstacleDistance(short int value);
		//void startStopCommandClock(int command);
		void test();
		void speak(QString text);
		
		
		/*!
		Returns the status of the program. True, if the simulationMode ist active.
		*/
		bool simulationMode();
		
		bool noHardwareErrorMessages;
		bool exitDialog;


	public slots:
		/*!
		\brief The locialUnit handles the behavior of the robot.
			
		This slot and called if an obstacle is detected.from the obstacleCheckThread.
		Turn the robot depending on which sensor made the alarm or stop / start it.
		The number of alarms are also counted.
		*/
		void logicalUnit(int sensorAlarm);
		
		/*!
		\brief Shows the sensor data in a graphical way (progressBars) and as a number in the GUI.
		This slot is called from the Signal sensorDataComplete from the sensorThread.
		*/
		void showSensorData();
		
		/*!
		\brief Shows the yes-no exit dialog for the main program.
		
		This slot is called when massive errors occur!
		*/
		void showExitDialog();
		
		/*!
		\brief Enables or disables the listening for the robot remote control.
		
		This slot is called from an event from the GUI (enable remote button).
		*/
		void enableRemoteControlListening(bool status);
		
		/*!
		This slot executes the network remote commands.
		*/
		void executeRemoteCommand(QString command);

		/*!
		This slot enables or disables the simulation mode.
		*/
		void setSimulationMode(bool status);
		
		void finishSplash();


	signals:
		/*!
		\brief Emits a speak signal.
		
		This signal is sent to the speakThread.
		*/
		// FixMe: SIOD ERROR: the currently assigned stack limit has been exceded 
		//void speak(QString text);


	private:
		/*!
		Reads all settings for the robot
		*/
		void readSettings();
		
		QString serialPortMicrocontroller;
		Gui *gui1;
		Circuit *circuit1;
		InterfaceAvr *interface1;
		SensorThread *sensorThread;
		PlotThread *plotThread;
		Motor *motors;
		Servo *servos;
		ObstacleCheckThread *obstCheckThread;
		Inifile *inifile1;
		CamThread *cam1;
		NetworkThread *netThread;
		LaserThread *laserThread;
		SpeakThread *speakThread;
		QString commaSeparator;
		bool robotIsOn; /*! Stores the state of the robots circuit. TRUE, when the cirvuit is ON. */
		bool robotDrives; /*! Stores the robots driving state. TRUE, when the robot drives. */
		bool noLaserScanner; /*! Don't start the laser thread and the laser module, if this value is TRUE. Used in the ini file.*/
		bool robotSimulationMode; /*! Stores the robots simulation state */
		int mot1Speed;
		int mot2Speed;
		int minObstDist;


		/*! The splash screen time of view in ms */
		static const unsigned int SPLASHTIME  = 2000;
		
		static const unsigned char ON  = 1;  /*! For motor "ON" */
		static const unsigned char OFF = 0;  /*! For motor "OFF" */
		
		/*! Some driving directions for the robot */
		static const unsigned char FORWARD = 1;
		static const unsigned char BACKWARD = 2;
		static const unsigned char LEFT = 3;
		static const unsigned char RIGHT = 4;
		static const unsigned char START = 5;
		static const unsigned char STOP = 6;
		static const unsigned char WAIT = 7;
		
		/*! Give the sensors some names
		
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
		
		/*! The ultrasonic sensor */
		static const short int SENSOR16 = 256;
		/*! Value, if no sensor has a value to react */
		static const short int NONE = 0;
		
		static const unsigned char CLOCKWISE        = 0;  /*! Motor direction "CLOCKWISE" */
		static const unsigned char COUNTERCLOCKWISE = 1;  /*! Motor direction "COUNTERCLOCKWISE" */
		static const unsigned char SAME             = 3;  /*! Motor direction/power "same like before" */
		static const unsigned char MOTOR1           = 10; /*! Motor 1 */
		static const unsigned char MOTOR2           = 20; /*! Motor 2 */
		
		static const short int MOTORSENSOR1 = 0;
		static const short int MOTORSENSOR2 = 1;
};

#endif
