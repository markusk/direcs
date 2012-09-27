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

#ifndef GUITEST_H
#define GUITEST_H

//-------------------------------------------------------------------
#include <QtGlobal> // for Q_OS_* Makro!
#include <QObject>
#include <QtDebug>
#include <QtGui>
#include <QMetaType>
#include <QtCore/QCoreApplication>


//-------------------------------------------------------------------
#include "gui.h"
//-------------------------------------------------------------------



/**
\author Markus Knapp
\brief The main class of the whole robot programm.

 All objects are created here. This method also starts the GUI and its splash screen.
*/
class GuiTest : public QObject
{
	Q_OBJECT


	public:
		/**
		The parameters are given from a main method which gets them from the console (startup arguments).
		*/
		GuiTest(bool bConsoleMode, bool bForceSmallGUI, bool bForceLargeGUI);
		~GuiTest();


		/**
		Initalises the complete program and robot. Conntect all signals and slots.
		*/
		void init();

		/**
		Returns the status of the main program. True, if the simulationMode ist active.
		*/
		bool simulationMode() const;

		/**
		Configures if an exit dialog should be shown, when exiting the main program.
		*/
		bool exitDialog;


	public slots:
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
		This slot enables or disables the simulation mode.
		@param status
		*/
		void setSimulationMode(bool status);

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
		Emits a string to the GUI log / console.
		@sa Gui::appendLog()
		@param text is the message to be emitted
		@param CR is true when a CR/LF should be added at the end of the line (default)
		@param sayIt is true when the text for the log also should be spoken (default=false)
		@param addTimestamp If true, the a timestamp is added in front of the text. This parameter is optional.
		*/
		void message(QString text, bool CR=true, bool sayIt=false, bool addTimestamp=true);


	private:
		/**
		Checks the command-line arguments passed to the program.
		*/
		void checkArguments();

		//Events *myEvent; /// my event filter for grabbing ctrl+c in the console mode
		Gui *gui;
		QStringList arguments;			/// contains the list of command-line arguments

		QString laserscannerTypeFront; // PLS, LMS, S300. just for reading the value here. @sa laserThread
		QString laserscannerTypeRear;  // PLS, LMS, S300. just for reading the value here. @sa laserThread
		int laserscannerAngleFront; // this values holds the range of the laserscanner viewing angle. e.g. 180 oder 270 degrees.  just for reading the value here. @sa laserThread
		int laserscannerAngleRear; // this values holds the range of the laserscanner viewing angle. e.g. 180 oder 270 degrees.  just for reading the value here. @sa laserThread
		int laserscannerIgnoreAreaStart; // this is only temporary needed when reading a setting
		int laserscannerIgnoreAreaEnd; // this is only temporary needed when reading a setting
		QString laserscannerMounting; // just for reading the value here. @sa laserThread
		int preferredDrivingDirection; /// @sa logicalUnit
		bool robotDrives; /// Stores the robots driving state. TRUE, when the robot drives.
		bool robotSimulationMode; /// Stores the robots simulation state
		bool laserScannerFrontFound;
		bool laserScannerRearFound;
		int  currentPreferredDirection;
		unsigned int value;
		float floatValue;
		bool consoleMode; /// is enabled if the argument 'console' was passed by command-line. Sends all GUI messages to the command line.
		bool shutdownAlreadyCalled;
		bool forceShutdown; /// this is set to true, when no ini-file was found at startup. This make the program leave without the stupid AreYouSure-Question.

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

		/// Value, if no sensor has a value to react
		static const short int NONE = 0;

		static const short int OBSTACLEFRONTLEFT = 512;
		static const short int OBSTACLEFRONTRIGHT = 1024;
		static const short int OBSTACLESEVERYWHEREINFRONT = 2048;

		static const short int MAXFALSEALARMS = 2; /// number of false alarms which are allowed in the logical unit

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
