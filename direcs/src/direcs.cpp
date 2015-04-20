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

#include "direcs.h"


int main(int argc, char *argv[])
{
	bool consoleMode = false;
	bool forceSmallGUI = false;
	bool forceLargeGUI = false;


	// Check for command-line argument "console".
	// A deeper argument check will be done later within the class Direcs!
	if (argc > 1)
	{
		qDebug() << argc - 1 << "argument(s) passed...";

		for (int i=1; i<argc; i++)
		{
			// now search for the "console" parameter (case insensitive)
			if (strcasecmp(argv[i], "console") == 0)
			{
				consoleMode = true;
				qDebug() << "Console mode enabled.";
			}

			// force small GUI ?
			if (strcasecmp(argv[i], "small") == 0)
			{
				forceSmallGUI = true;
				qDebug() << "Using small GUI.";
			}

			// force large GUI ?
			if (strcasecmp(argv[i], "large") == 0)
			{
				forceLargeGUI = true;
				qDebug() << "Using large GUI.";
			}

			// now search for the "console" parameter (case insensitive)
			if ((strcasecmp(argv[i], "-h") == 0) || (strcasecmp(argv[i], "-help") == 0) || (strcasecmp(argv[i], "help") == 0) || (strcasecmp(argv[i], "?") == 0) || (strcasecmp(argv[i], "-?") == 0))
			{
				qDebug() << "Possible parameters:\n\n";
				qDebug() << "console\tuse console mode";
				qDebug() << "small\tforce small GUI";
				qDebug() << "large\tforce large GUI\n\n";
			}
		}
	}


	if (consoleMode)
	{
		//----------------------
		// CREATE A CONSOLE APP
		//----------------------

		// The QCoreApplication class provides an event loop for console Qt applications.
		QCoreApplication app(argc, argv);

		CleanExit cleanExit;

		// create Direcs class object
		Direcs d(consoleMode, forceSmallGUI, forceLargeGUI);

		// init direcs
		d.init();

		return app.exec();
	}
	else
	{
		//----------------------
		// CREATE A GUI APP
		//----------------------

		// Initialize the resource file
		Q_INIT_RESOURCE(direcs);

		// The QApplication class manages the GUI application's control flow and main settings.
		QApplication app(argc, argv);

		// create the Direcs class object
		Direcs d(consoleMode, forceSmallGUI, forceLargeGUI);

		// init direcs
		d.init();

		return app.exec();
	}
}


Direcs::Direcs(bool bConsoleMode, bool bForceSmallGUI, bool bForceLargeGUI)
{
	// store mode from main method
	consoleMode = bConsoleMode;
	forceSmallGUI = bForceSmallGUI;
	forceLargeGUI = bForceLargeGUI;

	//-------------------------
	// Creating logfile object
	//-------------------------
	logfile = new Logfile();
	logfile->setFilename("direcs.log");
	logfile->appendLog("- start -------------------------------------------------------------------------------------------");


	//----------------------
	// check local hostname
	//----------------------
	QString hostname = QHostInfo::localHostName();

	logfile->appendLog(QString("The hostname of this machine is %1").arg(hostname));

	if (hostname.contains("robot"))
	{
		logfile->appendLog("Hostname contains 'robot'. We will use the small robot GUI.");
		useSmallGUI = true;
	}
	else
	{
		logfile->appendLog("Hostname does not contain 'robot'. We will use the large GUI.");
		useSmallGUI = false;
	}

	if (forceSmallGUI)
	{
		logfile->appendLog("Overriding GUI use due to command line argument. We will use the small robot GUI!");
		useSmallGUI = true;
	}

	if (forceLargeGUI)
	{
		logfile->appendLog("Overriding GUI use due to command line argument. We will use the large robot GUI!");
		useSmallGUI = false;
	}


	//--------------------------
	// create all other objects
	//--------------------------
	speakThread = new SpeakThread();

	if (consoleMode)
	{
		consoleGui = new ConsoleGui();
//		myEvent = new Events();
		/// \todo install an event filter to grab CTRL+C
//  		consoleGui->installEventFilter(myEvent);
//			this->installEventFilter(myEvent);
	}
	else
	{
		settingsDialog = new SettingsDialog();
		joystickDialog = new JoystickDialog();
		aboutDialog = new AboutDialog();
		gui = new Gui(useSmallGUI, settingsDialog, joystickDialog, aboutDialog);
		splash = new QSplashScreen(QPixmap(":/images/images/splash.png"));
	}


	mutex = new QMutex();
	interface1 = new InterfaceAvr();
	circuit1 = new Circuit(interface1, mutex);
///	\todo heartbeat = new Heartbeat(interface1, mutex);
	motors = new Motor(interface1, mutex);
	sensorThread = new SensorThread(interface1, mutex);
	servos = new Servo(interface1, mutex);
	rgbLeds = new RgbLed(interface1, mutex);
	laserThread = new LaserThread();
	obstCheckThread = new ObstacleCheckThread(sensorThread, laserThread);

#ifndef BUILDFORROBOT
	if (!consoleMode)
	{
		plotThread = new PlotThread(sensorThread);
	}
#endif

	inifile1 = new Inifile();
	netThread = new NetworkThread();
	// creation of kinect instance moved to init method to see something on the splash screen
	joystick = new Joystick();
	// head = new Head(servos);  disabled since the head is not in use use!

	if (!consoleMode)
	{
		camThread = new CamThread();
	}

	timerThread = new TimerThread();

	drivingSpeedTimer = new QTimer();

	// create Phonon media player
	mediaObject = new Phonon::MediaObject();

	demoThread = new DemoThread();
}


void Direcs::init()
{
	if (!consoleMode)
	{
		aboutDialog->setVersion("1.0"); /// \todo put this at a nicer place // this is also shown in the about dialog
		splashTextColor = Qt::white;
	}
	forceShutdown = false;
	inifile1->setFilename("direcs.ini");
	useSmallGUI=false;
	serialPortMicrocontroller = "error1";
	serialPortLaserscannerFront = "error1";
	laserscannerTypeFront= "error1";
	laserscannerTypeRear= "error1";
	writeLogFile = false;
	// 	robotIsOn = false;
	robotDrives = false;
	mot1Speed = 0;
	mot2Speed = 0;
	mot3Speed = 0;
	mot4Speed = 0;
	robotSimulationMode = false;
	robotRemoteMode = false;
	iAmTheMaster = true;
	firstDataReceived = false;
	servoTestMode = false;
	testDriveMode = false;
	mecanumDriveMode = false; // if false, a joystick move to the right, lets the robot "turn right". If true, the robots "moves right" - without turning around itself.
	eyeTestMode = false;
	currentTestServo = SERVO1;
	useCamera = false;
	cameraTestMode = false;
	faceTrackingIsEnabled = false;
	laserScannerFrontFound = false;
	laserScannerRearFound = false;
	laserscannerAngleFront=0;
	laserscannerAngleRear=0;
	endSpeedMotor1Reached = false;
	endSpeedMotor2Reached = false;
	endSpeedMotor3Reached = false;
	endSpeedMotor4Reached = false;
	preferredDrivingDirection = FORWARD; // this is to allow the robot to drive forward, if the obstacleCheckThread is not running (because of offline laser). Mainly for testing reasons!
	demoMode = false;


	//-----------------------------------------------------------------------------------------------------------------------------------------
	// check which OS we have. ACTUALLY we check for the WINDOW system, since the OS check doesn't worked (unix was recognized under mac os x)
	//-----------------------------------------------------------------------------------------------------------------------------------------
	#ifdef Q_OS_UNIX
	qDebug("UNIX OS detected.");
	logfile->appendLog("UNIX OS detected.");
	#endif

	#ifdef Q_OS_LINUX
	qDebug("Linux OS detected.");
	logfile->appendLog("Linux OS detected.");
	#endif

	#ifdef Q_WS_X11
	qDebug("X11 detected.");
	logfile->appendLog("X11 detected.");
	#endif

	#ifdef Q_OS_MAC
	qDebug("MAC OS detected.");
	logfile->appendLog("MAC OS detected.");
	#endif

	#ifdef Q_WS_MAC
	qDebug("MAC OS WS detected.");
	logfile->appendLog("MAC OS WS detected.");
	#endif

	//--------------------------------------------------------------------------
	// show the splash screen and set application name
	//--------------------------------------------------------------------------
	if (!consoleMode)
	{
		splash->show();

		// this is for the Phonon library to export audio output names through the DBUS interface
		QCoreApplication::setApplicationName("direcs");
	}

	//--------------------------------------------------------------------------
	// show splash messages in the splash screen
	//--------------------------------------------------------------------------
	connect(this, SIGNAL( splashMessage(QString) ), this, SLOT( showSplashMessage(QString) ));

	//--------------------------------------------------------------------------
	// let the splash screen also show laser init messages
	//--------------------------------------------------------------------------
	connect(laserThread, SIGNAL(message(QString)), this, SLOT(showSplashMessage(QString)));

	//--------------------------------------------------------------------------
	// let the GUI (or console) also show laser init messages
	//--------------------------------------------------------------------------
	if (consoleMode)
	{
		connect(laserThread, SIGNAL(message(QString)), consoleGui, SLOT(appendLog(QString)));
	}
	else
	{
		connect(laserThread, SIGNAL(message(QString)), gui, SLOT(appendLog(QString)));
	}

	//--------------------------------------------------------------------------
	// let some other classes know if we are in the console mode
	//--------------------------------------------------------------------------
	/// \todo not in use anymore! connect(this, SIGNAL(publishConsoleMode(bool)), gui, SLOT(setConsoleMode(bool)));

	/*
	not in use at the moment...

	//--------------------------------------------------------------------------------
	// create the commmand line arguments list
	//--------------------------------------------------------------------------------
	arguments = QCoreApplication::arguments();
	int count = arguments.count() - 1;

	// check if arguments were passed on the command-line
	if (count > 0)
	{
		qDebug() << count << "argument(s) passed...";
		// check the arguments
		checkArguments();
	}
	*/

	//------------------------------------------------------------------
	// Set the number format to "," for comma and 1000 separator to "."
	// For example: 1.234,00 EUR
	//------------------------------------------------------------------
	QLocale::setDefault(QLocale::German);
	commaSeparator = ",";

	//--------------------------------------------------------------------------
	// show messages in the GUI log or in the console
	//--------------------------------------------------------------------------
	if (consoleMode)
	{
		connect(this, SIGNAL( message(QString) ), consoleGui, SLOT( appendLog(QString) ));
	}
	else
	{
		connect(this, SIGNAL( message(QString, bool, bool, bool) ), gui, SLOT( appendLog(QString, bool, bool, bool) ));
	}

	//--------------------------------------------------------------------------
	// Check for the current programm path
	//--------------------------------------------------------------------------
	emit splashMessage("Loading config file...");
	emit message(QString("Current path: %1").arg(inifile1->checkPath()));

	//--------------------------------------------------------------------------
	// show a QMessage wth the possibility to exit the main programm, when errors occured!
	//--------------------------------------------------------------------------
	connect(interface1, SIGNAL(tooMuchErrors()), this, SLOT(showExitDialog()));

	//--------------------------------------------------------------------------
	// let some classes know the robots state
	//--------------------------------------------------------------------------
	// this is needed, when the first openCOMPort method fails:
	connect(interface1,	SIGNAL( robotState(bool) ), circuit1,		SLOT( setRobotState(bool) ));
	connect(interface1,	SIGNAL( robotState(bool) ), motors,			SLOT( setRobotState(bool) ));
	connect(interface1,	SIGNAL( robotState(bool) ), sensorThread,	SLOT( setRobotState(bool) ));
//	connect(interface1,	SIGNAL( robotState(bool) ), heartbeat,		SLOT( setRobotState(bool) ));
	connect(interface1,	SIGNAL( robotState(bool) ), rgbLeds,			SLOT( setRobotState(bool) ));

	// let the tiemr thread know, so it does not call arduino init via signal
	connect(interface1, SIGNAL( robotState(bool) ), timerThread,		SLOT( setRobotState(bool) ));

	if (!consoleMode)
	{
		connect(interface1,	SIGNAL( robotState(bool) ), gui,			SLOT( setRobotControls(bool) ));
	}

	// the following slot is also used, when the robot ewas detected (event driven by initCircuit!)
	connect(circuit1,	SIGNAL( robotState(bool) ), this,			SLOT( setRobotState(bool) ));

	// also set the robot to OFF, when there are problems with the circuit
	connect(circuit1,	SIGNAL( robotState(bool) ), motors,			SLOT( setRobotState(bool) ));
	connect(circuit1,	SIGNAL( robotState(bool) ), sensorThread,	SLOT( setRobotState(bool) ));
//	connect(circuit1,	SIGNAL( robotState(bool) ), heartbeat,		SLOT( setRobotState(bool) ));
	connect(circuit1,	SIGNAL( robotState(bool) ), rgbLeds,			SLOT( setRobotState(bool) ));
//	connect(circuit1,	SIGNAL( robotState(bool) ), this,			SLOT( setRobotState(bool) ));
	if (!consoleMode)
	{
		connect(circuit1,	SIGNAL( robotState(bool) ), gui,			SLOT( setRobotControls(bool) ));
	}

	// let the sensorthread know, if only the compass is not connected
	connect(circuit1,	SIGNAL( compassState(bool) ), sensorThread,	SLOT( setCompassState(bool) ));


	if (!consoleMode)
	{
		//--------------------------------------------------------------------------
		// shutdown Direcs program on exit button
		// shutdown is also called, when the gui is closed
		//--------------------------------------------------------------------------
		connect(gui, SIGNAL(shutdown()), this, SLOT(shutdown()));

		//--------------------------------------------------------------------------
		// call (a) test method(s) when clicking the test button
		//--------------------------------------------------------------------------
		connect(gui, SIGNAL(test()), this, SLOT(test()));
	}

	if (!consoleMode)
	{
		//--------------------------------------------------------------------------
		// resets the driven distance, when signal comes from Gui
		//--------------------------------------------------------------------------
		connect(gui, SIGNAL(resetDrivenDistance(int)), sensorThread, SLOT(resetDrivenDistance(int)));
	}

	if (!consoleMode)
	{
		//--------------------------------------------------------------------------
		// set the motor speed, when signal comes from Gui
		//--------------------------------------------------------------------------
		connect(settingsDialog, SIGNAL(setMotorSpeed(int, int)), motors, SLOT(setMotorSpeed(int, int)));

		//--------------------------------------------------------------------------
		// set the maximum robot speed, when signal comes from Gui
		//--------------------------------------------------------------------------
		connect(settingsDialog, SIGNAL(setMaximumSpeed(int)), motors, SLOT(setMaximumSpeed(int)));

		//--------------------------------------------------------------------------
		// set the robot slot, when signal comes from Gui
		//--------------------------------------------------------------------------
		connect(settingsDialog, SIGNAL(setPassageWidth(int)), obstCheckThread, SLOT(setPassageWidth(int)));

		//--------------------------------------------------------------------------
		// set the straight forward deviation, when signal comes from Gui
		//--------------------------------------------------------------------------
		connect(settingsDialog, SIGNAL(setStraightForwardDeviation(int)), obstCheckThread, SLOT(setStraightForwardDeviation(int)));

		//--------------------------------------------------------------------------
		// set the minimum distance, when signal comes from Gui
		//--------------------------------------------------------------------------
		connect(settingsDialog, SIGNAL(setMinObstacleDistance(int)), obstCheckThread, SLOT(setMinObstacleDistance(int)));

		//--------------------------------------------------------------------------
		// set the minimum laser distance, when signal comes from Gui
		//--------------------------------------------------------------------------
		connect(settingsDialog, SIGNAL(setMinObstacleDistanceLaserFront(int)), obstCheckThread, SLOT(setMinObstacleDistanceLaserFront(int)));
		/// @todo implement signal forwarding for second laser!

		//--------------------------------------------------------------------------
		// let the GUI show servo messages in the log
		//--------------------------------------------------------------------------
		connect(rgbLeds, SIGNAL(message(QString)), gui, SLOT(appendLog(QString)));
	}


	//--------------------------------------------------------------------------
	// Check for the programm ini file
	// AND read the settings  AND   d o   a l l   t h e   r e s t  ! ! !
	//--------------------------------------------------------------------------
	if (inifile1->checkFiles() == true)
	{
		//----------------------------------------------------------------------------
		// say a text
		//----------------------------------------------------------------------------
		// a phase can also be emitted to the regarding slot (int parameter here)
		connect(this, SIGNAL( speak(QString, int) ), speakThread, SLOT( speak(QString, int) ));

		if (!consoleMode)
		{
			connect(gui,  SIGNAL( speak(QString) ), speakThread, SLOT( speak(QString) ));
		}


		if (speakThread->isRunning() == false)
		{
			emit message("Starting speak thread...", false);
			speakThread->start();
			emit message("Speak thread started.");
		}

		// file found-Msg
		emit message(QString("Using ini-File \"%1\".").arg(inifile1->getInifileName()));

		//=========================
		// read all settings
		//=========================
		emit splashMessage("Reading settings...");
		readSettings();

#ifndef BUILDFORROBOT
		if (!consoleMode)
		{
			//----------------------------------------------------------------------------
			// connect plotThread signal to "setPlotData"
			// (Whenever the plot thread has new data, the data are show in the GUI)
			//----------------------------------------------------------------------------
/* plot of motor currents disabled
			connect(plotThread, SIGNAL( plotDataComplete1(double *, double *, int) ), gui, SLOT( setPlotData1(double *, double *, int) ));
			connect(plotThread, SIGNAL( plotDataComplete2(double *, double *, int) ), gui, SLOT( setPlotData2(double *, double *, int) ));
			connect(plotThread, SIGNAL( plotDataComplete3(double *, double *, int) ), gui, SLOT( setPlotData3(double *, double *, int) ));
			connect(plotThread, SIGNAL( plotDataComplete4(double *, double *, int) ), gui, SLOT( setPlotData4(double *, double *, int) ));
*/
			connect(plotThread, SIGNAL( plotDataComplete5(double *, double *, int) ), gui, SLOT( setPlotData5(double *, double *, int) ));
			connect(plotThread, SIGNAL( plotDataComplete6(double *, double *, int) ), gui, SLOT( setPlotData6(double *, double *, int) ));
			connect(plotThread, SIGNAL( plotDataCompleteHeartbeat(double *, double *, int) ), gui, SLOT( setPlotDataHeartbeat(double*, double*, int) ));
		}
#endif

		//----------------------------------------------------------------------------
		// let the GUI show messages in the logs
		//----------------------------------------------------------------------------
		if (!consoleMode)
		{
			connect(joystick, SIGNAL(message(QString)), gui, SLOT(appendLog(QString)));
			connect(interface1, SIGNAL(message(QString)), gui, SLOT(appendSerialLog(QString)));
			connect(circuit1, SIGNAL(message(QString)), gui, SLOT(appendSerialLog(QString)));
			connect(obstCheckThread, SIGNAL(message(QString)), gui, SLOT(appendLog(QString)));
			connect(timerThread, SIGNAL(message(QString)), gui, SLOT(appendLog(QString)));
			connect(demoThread, SIGNAL(message(QString)), gui, SLOT(appendLog(QString)));
		}
		else
		{
			connect(joystick, SIGNAL(message(QString)), consoleGui, SLOT(appendLog(QString)));
			connect(interface1, SIGNAL(message(QString)), consoleGui, SLOT(appendSerialLog(QString)));
			connect(obstCheckThread, SIGNAL(message(QString)), consoleGui, SLOT(appendLog(QString)));
			connect(timerThread, SIGNAL(message(QString)), consoleGui, SLOT(appendLog(QString)));
			connect(demoThread, SIGNAL(message(QString)), consoleGui, SLOT(appendLog(QString)));
		}

		// also emit interface class messages to the *logfile*
		connect(interface1,			SIGNAL(message(QString)), logfile, SLOT(appendLog(QString))); // FIXME: to fast in case of error for writing the logfile!
		connect(circuit1,			SIGNAL(message(QString)), logfile, SLOT(appendLog(QString)));
		connect(obstCheckThread,   SIGNAL(message(QString)), logfile, SLOT(appendLog(QString)));
		connect(timerThread,       SIGNAL(message(QString)), logfile, SLOT(appendLog(QString)));
		connect(demoThread,       SIGNAL(message(QString)), logfile, SLOT(appendLog(QString)));

		/// \todo check if this is okay for the logfile writer in case of error TO FAST for logfile!!!
		//		connect(joystick, SIGNAL(message(QString)), logfile, SLOT(appendLog(QString)));

		//----------------------------------------------------------------------------
		// connect demo button from gui to activate the demo mode
		//----------------------------------------------------------------------------
		connect(gui, SIGNAL( demo(bool) ), this, SLOT( setDemoMode(bool) ));

		//----------------------------------------------------------------------------
		// also let the speakThread inform the nextDemoPhase Slot about that the speech was completed to enter the next demo phase
		//----------------------------------------------------------------------------
		connect(speakThread, SIGNAL( speechCompleted(int) ), this, SLOT( nextDemoPhase(int) ));

		//-------------------------------------------------------
		// start the network thread (waiting for commands)
		//-------------------------------------------------------

		// we swap the ports for first listening for a network master
		netThread->swapPorts();
		enableRemoteControlListening(true);

		//-------------------------------------------------------
		// Open serial port for microcontroller communication
		//-------------------------------------------------------
		emit splashMessage("Opening serial port for microcontroller communication...");
		emit message(QString("Opening serial port %1 for microcontroller communication...").arg(serialPortMicrocontroller));

		if (interface1->openComPort(serialPortMicrocontroller) == false)
		{
			//*****************************
			//* ERROR opening serial port *
			//*****************************
			// Error message will be send via signals to the GUI or Console!
			emit message("<font color=\"#FF0000\">The robot is OFF!</font>");
			if (consoleMode)
			{
				consoleGui->appendSerialLog("<font color=\"#FF0000\">+++++++++++++++++++++</font>");
				consoleGui->appendSerialLog("<font color=\"#FF0000\">+ The robot is OFF! +</font>");
				consoleGui->appendSerialLog("<font color=\"#FF0000\">+++++++++++++++++++++</font>");
			}
			else
			{
				gui->appendSerialLog("<font color=\"#FF0000\">The robot is OFF!</font>");
			}
			logfile->appendLog("ERROR: The robot is OFF!");

			/* not in use. Don't show a message box
			if (!consoleMode)
			{
				// show a warning dialog!
				QMessageBox msgbox(QMessageBox::Warning,	tr("Error with robots serial port"),
															tr("Error opening serial port %1").arg(serialPortMicrocontroller),
															QMessageBox::Ok | QMessageBox::Default);
				msgbox.exec();
			}
			*/

			if (!consoleMode)
			{
				// set GUI LED for compass module
				// has to be OFF, since the Atmel circuit is OFF
				gui->setLEDCompass(LEDOFF);
				gui->disableCompass();
			}
		}
		else
		{
			//**********************
			//* Serial port opened *
			//**********************
			emit message("Serial port opened.");
			if (consoleMode)
			{
				consoleGui->appendSerialLog("Serial port opened.");
			}
			else
			{
				gui->appendSerialLog("Serial port opened.");
			}
			logfile->appendLog("Serial port opened.");


			//-------------------------------------------------------
			// Basic init for all the bits on the robot circuit
			// AND check, if the robot is "on" (it answers correct)
			//-------------------------------------------------------
			if (!consoleMode)
			{
				// init the circuit & Co. when hitting the button in the GUI
				connect(gui, SIGNAL( initCircuit() ), circuit1, SLOT( initCircuit() ) );
				connect(gui, SIGNAL( initCircuit() ), this, SLOT( resetMotorSpeed() ) ); // also reset motor speed to ini-file values < < @todo: workaround!
///				connect(gui, SIGNAL( initServos() ), servos, SLOT( init() ) ); @todo: servos currently not in use. Would cause an error message on reset.
//				connect(gui, SIGNAL( initServos() ), rgbLeds, SLOT( init() ) ); // @todo: build this < < < < < < <
			}

			//******************************************************************************************************
			//
			// all init stuff from here moved to setRobotState Slot which is now called by Signal from initCicuit!
			//
			//******************************************************************************************************

		} // serial port opened

		//-----------------------------------------------------------
		// start the timer thread
		//-----------------------------------------------------------
		if (timerThread->isRunning() == false)
		{
			emit splashMessage("Starting timer thread...");
			emit message("Starting timer thread...", false);
			timerThread->start();
			emit message("Timer thread started.");
		}


		//-----------------------------------------------------------
		// check if a joystick is connected
		//-----------------------------------------------------------
		if (joystick->isConnected())
		{
			// set GUI LED
			if (!consoleMode)
				gui->setLEDJoystick(GREEN);

			// start the joystick thread
			if (joystick->isRunning() == false)
			{
				emit splashMessage("Starting joystick thread...");
				emit message("Starting joystick thread...", false);
				joystick->start();
				emit message("Joystick thread started.");
			}
		}
		else
		{
			// set GUI LED
			if (!consoleMode)
				gui->setLEDJoystick(RED);

			emit message("Joystick thread NOT started.");
		}

		if (!consoleMode)
		{
			//----------------------------------------------------------------------------
			// drive in the direction which was emited from the gui
			//----------------------------------------------------------------------------
			connect(gui, SIGNAL( drive(int) ), this, SLOT( drive(int) ));
		}

		//----------------------------------------------------------------------------
		// increase the speed by an interval defined as a const in the header file
		// this is used when starting to drive
		//----------------------------------------------------------------------------
		connect(drivingSpeedTimer, SIGNAL( timeout() ), this, SLOT( increaseDrivingSpeed() ));

		//----------------------------------------------------------------------------
		// connect sensor signals to "show sensor data"
		// (Whenever the sensor data are completely read, show the result in the GUI)
		//----------------------------------------------------------------------------
		connect(sensorThread, SIGNAL( sensorDataComplete() ), this, SLOT( showSensorData() ) );
		connect(sensorThread, SIGNAL( sendNetworkString(QString) ), netThread, SLOT( sendNetworkCommand(QString) ) );
		// show error messages in GUI, too
		connect(sensorThread, SIGNAL( message(QString, bool, bool, bool) ), gui, SLOT( appendLog(QString, bool, bool, bool) ));
		// write error messages to logfile
		connect(sensorThread, SIGNAL( message(QString, bool) ), logfile, SLOT( appendLog(QString, bool) ));

		if (!consoleMode)
		{
			connect(sensorThread, SIGNAL( compassDataComplete(float, float, float, float) ), gui, SLOT( showCompassData(float, float, float, float) ) );
		}


		//----------------------------------------------------------------------------
		// connect sensor signals to "show that the robot is still alive"
		// (Whenever a specifiv sensor data is received, show the result in the GUI)
		//----------------------------------------------------------------------------
		if (!consoleMode)
		{
			connect(sensorThread, SIGNAL( heartbeat(unsigned char)), gui, SLOT( setLEDHeartbeat(unsigned char) ) );
		}
		//----------------------------------------------------------------------------
		// connect sensor signals and write the heartbeat to the logfile
		// (Whenever a specific sensor data is received, write this to the logfile)
		//----------------------------------------------------------------------------
		// 	connect(sensorThread, SIGNAL( heartbeat(unsigned char)), logfile, SLOT( writeHeartbeat(unsigned char) ) );
		// FIXME: this is too often!! Because of 10ms sensor thread!!

		//----------------------------------------------------------------------------
		// connect obstacle check (alarm!) sensor signal to "logical unit"
		//----------------------------------------------------------------------------
		connect(obstCheckThread, SIGNAL(obstacleDetected(int, QDateTime)), this, SLOT(logicalUnit(int, QDateTime)));

		if (!consoleMode)
		{
			//----------------------------------------------------------------------------
			// show the angle where to drive in a GUI label
			//----------------------------------------------------------------------------
			connect(obstCheckThread, SIGNAL(newDrivingAngleSet(int, int, int, float)), gui, SLOT(showLaserFrontAngles(int, int, int, float)));

			//----------------------------------------------------------------------------
			// show the preferred driving direction in a GUI label
			//----------------------------------------------------------------------------
			connect(this, SIGNAL(showPreferredDirection(QString)), gui, SLOT(showPreferredDirection(QString)));

			//----------------------------------------------------------------------------
			// connect remote control button from gui to remote control method here
			// (Whenever the button is pushed, enable the network remote control)
			//----------------------------------------------------------------------------
			connect(gui, SIGNAL( enableRemoteControlListening(bool) ), this, SLOT( enableRemoteControlListening(bool) ));
		}

		//----------------------------------------------------------------------------
		// execute the received remote commands
		//----------------------------------------------------------------------------
		connect(netThread, SIGNAL( dataReceived(QString) ), this, SLOT( executeRemoteCommand(QString) ));

		//---------------------------------------------------------------------------------------
		// check the network state after some seconds after pgm start (are we master or slave?)
		//---------------------------------------------------------------------------------------
		connect(timerThread, SIGNAL(checkNetworkState()), this, SLOT(setNetworkState()));

		// connect signal from timerThread to networkThread (enable sending 'master' signal every second)
		connect(timerThread, SIGNAL(checkNetworkState()), netThread, SLOT(setNetworkMaster()));

		//--------
		// try to init Arduino after some seconds, not during exact start
		//--------
		// call Slot initArduino on Signal checkArduinoState
		connect(timerThread, SIGNAL(checkArduinoState()), circuit1, SLOT(initArduino()));

		// show network master/slave state in GUI LED
		if (!consoleMode)
		{
			connect(netThread, SIGNAL(heartbeat(unsigned char)), gui, SLOT( setLEDMasterSlave(unsigned char) ) );
		}

		//----------------------------------------------------------------------------
		// connect networkThread signal to "dataReceived"
		// (Whenever data were received, the data are shown in the GUI)
		//----------------------------------------------------------------------------
		if (!consoleMode)
		{
			connect(netThread, SIGNAL( dataReceived(QString) ), gui, SLOT( appendNetworkLog(QString) ));
		}
		else
		{
			connect(netThread, SIGNAL( dataReceived(QString) ), consoleGui, SLOT( appendNetworkLog(QString) ));
		}

		//----------------------------------------------------------------------------
		// connect sendNetworkString signal to netThreads slot "sendNetworkCommand"
		// (Whenever the signal is emmited, send the given string over the net)
		//----------------------------------------------------------------------------
		connect(this, SIGNAL(sendNetworkString(QString) ), netThread, SLOT( sendNetworkCommand(QString) ));

		// connect signal from demoThread to rgbLed class (enable setting RGB LEDs from demoThread)
		connect(demoThread, SIGNAL(setRGBLEDBrightness(unsigned char, unsigned char)), rgbLeds, SLOT(setBrightness(unsigned char,unsigned char)));

#ifdef ACTIVELASERVIEW
		if (!consoleMode)
		{
			//----------------------------------------------------------------------------
			// connect laserThread signal to "dataReceived"
			// (Whenever data were received, the data are shown in the GUI)
			//----------------------------------------------------------------------------
			qRegisterMetaType < QList <float> > ("QList <float>");
			qRegisterMetaType < QList <int> > ("QList <int>");

			connect(laserThread, SIGNAL( laserDataCompleteFront(QList <float>, QList <int>) ), gui, SLOT( refreshLaserViewFront(QList <float>, QList <int>) ));
			connect(laserThread, SIGNAL( laserDataCompleteRear(QList <float>, QList <int>) ), gui, SLOT( refreshLaserViewRear(QList <float>, QList <int>) ));
		}
#endif

		//------------------------------------------------------------------------------
		// connect laserThread signal to networkThread
		// (Whenever laserscanner data are read, send the data over the network thread)
		//------------------------------------------------------------------------------
		connect(laserThread, SIGNAL( sendNetworkString(QString) ), netThread, SLOT( sendNetworkCommand(QString) ) );

		// whenever there is an error in the laser sensor, stop the obstacle check thread!
		connect(laserThread, SIGNAL( systemerror(int) ), obstCheckThread, SLOT( systemerrorcatcher(int) ) );

		// whenever there is an error in the laser sensor, turn the laser GUI LED off
		connect(laserThread, SIGNAL( systemerror(int) ), gui, SLOT( systemerrorcatcher(int) ) );

		// whenever there is an error in the laser sensor, stop driving, or so.
		connect(laserThread, SIGNAL( systemerror(int) ), this, SLOT( systemerrorcatcher(int) ) );

		// write laser thread messages to logfile, too
		connect(laserThread, SIGNAL(message(QString)), logfile, SLOT(appendLog(QString)));

		//----------------------------------------------------------------------------
		// connect joystick signals to "show joystick data"
		// (Whenever the joystick is moved or a button is pressed, show the result in the GUI)
		//----------------------------------------------------------------------------
		if (!consoleMode)
		{
			connect(joystick, SIGNAL(joystickMoved(int, int)), joystickDialog, SLOT(showJoystickAxes(int, int)));
			connect(joystick, SIGNAL(joystickButtonPressed(int, bool)), joystickDialog, SLOT(showJoystickButtons(int, bool)));
#ifdef Q_OS_MAC // for Mac OS only:
			connect(joystick, SIGNAL(joystickPOVButtonPressed(int)), joystickDialog, SLOT(showJoystickPOVButtons(int)));
#endif
		}

		connect(joystick, SIGNAL(joystickMoved(int, int)), this, SLOT(executeJoystickCommand(int, int)));
		connect(joystick, SIGNAL(joystickButtonPressed(int, bool)), this, SLOT(executeJoystickCommand(int, bool)));

		if ((!consoleMode) && (useCamera))
		{
			emit splashMessage("Detecting Kinect camera...");
			emit message("Detecting Kinect camera...");

			// show Kinect messages in GUI
			connect(camThread, SIGNAL(message(QString)), logfile, SLOT(appendLog(QString)));
			connect(camThread, SIGNAL(message(QString)), gui, SLOT(appendLog(QString)));


			//-----------------------------------------------------------
			// check if Kinect camera is connected
			//-----------------------------------------------------------
			emit splashMessage("Initialising Kinect camera...");
			emit message("Initialising Kinect camera...");

			if (camThread->init() == true)
			{
				emit splashMessage("Kinect found.");
				emit message("Kinect found.");

				// connect sensor contact signals to "show contact alarm"
				// (Whenever the an alarm contact was closed, show the result in the cam image)
				// connect(sensorThread, SIGNAL(contactAlarm(char, bool)), camThread, SLOT(drawContactAlarm(char, bool)));

				// connect camDataComplete from the cam thread to signal "setCamImage"
				// (Whenever the image is complete, the image is shown in the GUI)
				connect(camThread, SIGNAL( camImageComplete(QImage*) ), gui, SLOT( setCamImage(QImage*) ));
				connect(camThread, SIGNAL( camImageDepthComplete(QImage*) ), gui, SLOT( setCamImageDepth(QImage*) ));
				connect(camThread, SIGNAL( camImageOpenCVComplete(QImage*) ), gui, SLOT( setCamImageOpenCV(QImage*) ));

				// connect faceDetected from the camThread to the faceTracking unit and to the GUI (to show some values)
				connect(camThread, SIGNAL( faceDetected(int, int, int, int, int, int) ), this, SLOT( faceTracking(int, int, int, int) ));
				connect(camThread, SIGNAL( faceDetected(int, int, int, int, int, int) ),  gui, SLOT( showFaceTrackData(int, int, int, int, int, int) ));

				// enable face detection, when activated in the GUI
				connect(gui, SIGNAL( enableFaceDetection(int) ), camThread, SLOT( enableFaceDetection(int) ));

				// enable face tracking, when activated in the GUI
				connect(gui, SIGNAL( enableFaceTracking(int) ), this, SLOT( enableFaceTracking(int) ));

				// show the face track direction in the gui
				connect(this, SIGNAL( showFaceTrackDirection(QString) ), gui, SLOT( showFaceTrackDirection(QString)) );

				//-------------------------------------------------------------------------------------
				// disable face detection in the GUI, on error with loading haar cascade in CamThread
				// Must be before readSettings!
				//-------------------------------------------------------------------------------------
		///		connect(camThread, SIGNAL( disableFaceDetection() ), gui, SLOT( disableFaceDetection() )); \todo kinect stuff

				//-------------------------------------------------------------------------------------
				// disable camera controls in the GUI, on error opeing the camera in the CamThread
				// Must be before readSettings!
				//-------------------------------------------------------------------------------------
		///		connect(camThread, SIGNAL( disableCamera() ), gui, SLOT( disableCamera() )); \todo kinect stuff

				// send error messages to the gui
				connect(camThread, SIGNAL(message(QString)), gui, SLOT(appendLog(QString)));

				// set the Threshold in the camThread when changed in GUI
				connect(gui, SIGNAL(setThreshold(int)), camThread, SLOT(setThreshold(int)));

				//--------------------------------------------
				// start threading and grabbing live pictures
				//--------------------------------------------
				emit splashMessage("Starting camThread...");
				emit message("Starting camThread...", false);
				camThread->start();

				// show kinect camera state in gui
				gui->setLEDCamera(GREEN);

				// the signal for setting the Kinect camera angle
				connect(gui, SIGNAL(setKinectAngle(int)), camThread, SLOT(setAngle(int)));

				// the signal for setting the video mode
				// connect(gui, SIGNAL(setKinectVideoMode(int)), kinect, SLOT(setVideoMode(int)));
			}
			else
			{
				emit splashMessage("Kinect not found.");
				emit message("Kinect camera not found.", false);

				emit splashMessage("Kinect DISABLED.");
				emit message("Kinect camera DISABLED.", false);

				// show kinect camera state in gui
				gui->setLEDCamera(RED);
				gui->hideCameraControls();

				//gui->disableCamera();
				emit message("No Kinect detected.");
			}
		}

		if (!consoleMode)
		{
			//----------------------------------------------------------------------------
			// connect simulation button from gui to activate the simulation mode
			// (sets the direcs an the threads to simulation mode)
			//----------------------------------------------------------------------------
			connect(gui, SIGNAL( simulate(bool) ), this, SLOT( setSimulationMode(bool) ));
			connect(gui, SIGNAL( simulate(bool) ), sensorThread, SLOT( setSimulationMode(bool) ));
			connect(gui, SIGNAL( simulate(bool) ), laserThread, SLOT( setSimulationMode(bool) ));
			connect(gui, SIGNAL( simulate(bool) ), obstCheckThread, SLOT( setSimulationMode(bool) ));

			// let the laserThread write a data file from its laser values if requested by the GUI
			connect(gui, SIGNAL( writeLaserData() ), laserThread, SLOT( saveLaserData() ));
		}


		//---------------------------------------------------------------------
		// check if laser scanners are connected
		//---------------------------------------------------------------------
		// check FRONT laser
		emit splashMessage("Searching front laser...");
		laserScannerFrontFound = laserThread->isConnected(LASER1);

		// check REAR laser
		emit splashMessage("Searching rear laser...");
		laserScannerRearFound = laserThread->isConnected(LASER2);

		if (laserScannerFrontFound || laserScannerRearFound)
		{
			if (laserScannerFrontFound)
			{
				if (!consoleMode)
				{
					gui->setLEDLaser(GREEN);
				}
				emit message("Front laser scanner found.");
			}
			else
			{
				emit message("Front laser scanner NOT found.");
			}

			if (laserScannerRearFound)
			{
				emit message("Rear laser scanner found.");
			}
			else
			{
				emit message("Rear laser scanner NOT found.");
			}

			if (!consoleMode)
			{
				/// \todo nice exit point and error message
				if (!QGLFormat::hasOpenGL())
				{
					qDebug() << "This system has no OpenGL support" << endl;
					showExitDialog();
				}
			}


			// start the laserThread
			if (laserThread->isRunning() == false)
			{
				emit splashMessage("Starting Laser thread...");
				emit message("Starting Laser thread...", false);
				laserThread->start();
				emit message("Laser thread started.");
			}


			if (obstCheckThread->isRunning() == false)
			{
				emit splashMessage("Initializing obstacle check thread...");
				emit message("Initializing obstacle check thread...", false);
				obstCheckThread->init();
				emit message("Obstacle check thread initialized.");

				emit splashMessage("Starting obstacle check thread...");
				emit message("Starting obstacle check thread...", false);
				obstCheckThread->start();
				emit message("Obstacle check thread started.");
			}
		}
		else
		{
			emit message("<font color=\"#FF0000\">NO laser scanners found! Thread NOT started!</font>");

			if (!consoleMode)
			{
				// choose a red instead of a off LED since this looks more important
				gui->setLEDLaser(RED);
			}
		}


		if (!consoleMode)
		{
			// start random number generator
			srand(time(NULL));

			// generate one number
			int number = rand() % 7 +1; // (1 to 7)
			qDebug("file number %d", number);

			// create phonon player and set filename
//			music->Phonon::createPlayer(Phonon::MusicCategory, Phonon::MediaSource("../../../../dr.mp3"));

			/// @todo put media filename or list somewehre else
//            mediaObject->setCurrentSource(Phonon::MediaSource("../../../../dr.mp3"));
//            mediaObject->setCurrentSource(Phonon::MediaSource("../../../../media/1"));
#ifdef Q_OS_LINUX
			mediaObject->setCurrentSource(Phonon::MediaSource(QString("../../../../media/%1").arg(number)));
#else
			mediaObject->setCurrentSource(Phonon::MediaSource(QString("../../../../media/%1.mp3").arg(number)));
#endif

			Phonon::AudioOutput *audioOutput = new Phonon::AudioOutput(Phonon::MusicCategory, this);
			Phonon::createPath(mediaObject, audioOutput);

			// let the media player restart when files endend and demo mode is enabled
			connect(mediaObject, SIGNAL( finished() ), this, SLOT( mediaPlayerFinished() ));

			// let the media player skip a track if the button is pressed in the GUI
			connect(gui, SIGNAL( mediaSkip() ), this, SLOT( mediaPlayerFinished() ));
		}

		if (!consoleMode)
		{
			//------------------------------------------------------------------
			// hide some dialogues
			//------------------------------------------------------------------
			settingsDialog->hide();
			joystickDialog->hide();
			aboutDialog->hide();

			//------------------------------------------------------------------
			// for getting the screen resolution
			//------------------------------------------------------------------
			QDesktopWidget *desktop = QApplication::desktop();

			//------------------------------------------------------------------
			// place gui window at a nice position on the screen
			//------------------------------------------------------------------
			if (desktop->width() > 1024)
			{
				// move mainWindow to the center of the screen
				gui->move( (desktop->width() - gui->width())/2, (desktop->height() - gui->height())/2 );

				// show the gui
				gui->show();

				// delete the splash screen
				//QTimer::singleShot(SPLASHTIME, this, SLOT( finishSplash() ));
			}
			else
			{
				// resolution too smal for this window. Maximizing...
				// show the main window
				gui->showMaximized();

				// delete the splash screen
				// QTimer::singleShot(SPLASHTIME, this, SLOT( finishSplash() ));
			}

#ifdef ACTIVELASERVIEW
			// one time init for the laser view
			gui->initLaserView();
#endif


			//-----------------------------------------------------------
			// check voltages and write them to logfile
			//-----------------------------------------------------------
			emit splashMessage("Checking voltages...");
			logfile->appendLog(QString("INFO: Voltage of Sensor %1 is %2 Volt.").arg(VOLTAGESENSOR1).arg(sensorThread->getVoltage(VOLTAGESENSOR1)));
			logfile->appendLog(QString("INFO: Voltage of Sensor %1 is %2 Volt.").arg(VOLTAGESENSOR2).arg(sensorThread->getVoltage(VOLTAGESENSOR2)));


			// delete the splash screen
			finishSplash();
		}
	} // ini-file found
	else
	{
		if (!consoleMode)
		{
			// file not found-Msg
			QMessageBox msgbox(QMessageBox::Critical,
							   tr("direcs"),
//							   tr("Required configuration file \"%1\" not found! File perhaps not in the following directory: %2\n\nSorry, exiting direcs NOW...").arg(inifile1->getInifileName()).arg( QDir::currentPath() ),
							   tr("Required configuration file \"%1\" not found! File perhaps not in the following directory: %2\n\nSorry, exiting direcs NOW...").arg(inifile1->getInifileName()).arg( inifile1->checkPath() ),
							   QMessageBox::Ok | QMessageBox::Default);
			msgbox.exec();
			forceShutdown = true; // don't ask for AreYouSure, later when shutting down
			emit message(QString("<b><font color=\"#FF0000\">File '%1' not found!</font></b>").arg(inifile1->getInifileName()));

			// call my own exit routine
			shutdown();

			// here we're back from the shutdown method, so bye bye
			// QCoreApplication::exit(-1); does not work!
			exit(1); // FIXME: works, but doesn't call the destructor :-(
		}
	}
}


void Direcs::setRobotState(bool state)
{
	// init circuit was okay
	if (state == true)
	{
		emit message("Robot is <font color=\"#00FF00\">ON</font> and answers.");

/*
		// check compass module
		if (circuit1->initCompass() == true)
		{
			emit message("3D compass module detected.");
			if (!consoleMode)
			{
				gui->setLEDCompass(GREEN);
			}
		}
		else
		{
			emit message("<font color=\"#FF0000\">3D compass module not connected!</font>");
			if (!consoleMode)
			{
				gui->setLEDCompass(RED);
			}
		}
*/

/* >>> Arduino to be ported

		//-------------------------------------------------------
		// set the read motor speed
		//-------------------------------------------------------
		emit message("Setting motor speed in microcontroller");
		motors->setMotorSpeed(MOTOR1, mot1Speed);
		motors->setMotorSpeed(MOTOR2, mot2Speed);
		motors->setMotorSpeed(MOTOR3, mot3Speed);
		motors->setMotorSpeed(MOTOR4, mot4Speed);
		emit message("Motor speed set.");
*/

/*
		//-------------------------------------------------------
		// move all Servos in their default positions
		//-------------------------------------------------------
		servos->init();
		emit message("Servos moved to default positions");
*/

/* >>> Arduino to be ported
		//-------------------------------------------------------
		// set all RGB LEDs in their default brightness
		//-------------------------------------------------------
		rgbLeds->init();
		emit message("RGB LEDs set to default brightness");
*/

		/// \todo start heartbeat thread and see, whats going on there! Also to do: define atmel code for an "heartbeat answer / action" !!!!!
		/// currently done by sensor thread a bit...
/*
		//-----------------------------------------------------------
		// start the heartbeat thread
		//-----------------------------------------------------------
		if (heartbeat->isRunning() == false)
		{
			emit splashMessage("Starting heartbeat thread...");

			emit message("Starting heartbeat thread...", false);
			heartbeat->start();
			emit message("Heartbeat thread started.");
		}
*/

		//-----------------------------------------------------------
		// start the sensor thread for reading the sensors)
		//-----------------------------------------------------------
		if (sensorThread->isRunning() == false)
		{
			emit splashMessage("Starting sensor thread...");
			emit message("Starting sensor thread...", false);
			sensorThread->start();
			emit message("Sensor thread started.");

			// whenever there is a material error, react!
			connect(sensorThread, SIGNAL( systemerror(int) ), this, SLOT( systemerrorcatcher(int) ) );
		}

#ifndef BUILDFORROBOT
		if (!consoleMode)
		{
			//-----------------------------------------------------------
			// start the plot thread ("clock" for plotting the curves)
			//-----------------------------------------------------------
			if (plotThread->isRunning() == false)
			{
				emit splashMessage("Starting plot thread...");
				emit message("Starting plot thread...", false);
				plotThread->start();
				emit message("Plot thread started.");
			}
		}
#endif
	} // state / init circuit was successfull
	else
	{
		logfile->appendLog("Robot is OFF! Please turn it ON!");
		emit message("<font color=\"#FF0000\">The robot is OFF! Please turn it ON!</font>");
		emit message("Heartbeat thread NOT started!");
		emit message("Sensor thread NOT started!");
		emit message("Plot thread NOT started!");
	} // state = false
}


void Direcs::shutdown()
{
	emit message("----------------");
	emit message("Shutting down...");
	emit message("----------------");

	if (!consoleMode)
	{
		//-----------------------------------------
		// hide all dialogs, except the mainDialog
		//-----------------------------------------
		if (settingsDialog->isVisible())
			settingsDialog->hide();

		if (joystickDialog->isVisible())
			joystickDialog->hide();

		if (aboutDialog->isVisible())
			aboutDialog->hide();

		splash->show();
		emit splashMessage("Shutting down...");

		// stop phonon media player
		if (mediaObject->state() ==  Phonon::PlayingState)
		{
			emit splashMessage("Stopping media player...");
			mediaObject->stop();
		}
	}

	// just 4 fun
// 	head->look("NORMAL"); no head mounted at the moment...
// 	head->look("DOWN");   no head mounted at the moment...

	//---------------------------------------------------------------
	// save changes to ini-file (if check box is checked!)
	//---------------------------------------------------------------
	// THIS SETTING HAS TO BE SAVED ALWAYS!
	// "Save the setting, that no settings shoud be saved"
	//
	// save check box status
	if (!consoleMode)
	{
		if (forceShutdown==false) // don't write this setting when we have a forced shutdown (so the ini-file with only one line won't be created anymore)
		{
			inifile1->writeSetting("Config", "saveOnExit", settingsDialog->getCheckBoxSaveSettings());
		}


		if (settingsDialog->getCheckBoxSaveSettings() == Qt::Checked)
		{
			emit message("Writing settings...");
			emit splashMessage("Writing settings...");

			// save gui slider values
			inifile1->writeSetting("Config", "motor1Speed", settingsDialog->getSliderMotorSpeed(1));
			inifile1->writeSetting("Config", "motor2Speed", settingsDialog->getSliderMotorSpeed(2));
			inifile1->writeSetting("Config", "minimumSpeed", settingsDialog->getSliderMinimumSpeed());
			inifile1->writeSetting("Config", "maximumSpeed", settingsDialog->getSliderMaximumSpeed());
			inifile1->writeSetting("Config", "minObstacleDistance", settingsDialog->getSliderObstacleValue());
			inifile1->writeSetting("Config", "minObstacleDistanceLaserScanner", settingsDialog->getSliderObstacleLaserScannerValue());
			inifile1->writeSetting("Config", "robotSlotWidth", settingsDialog->getSliderPassageWidth());
			inifile1->writeSetting("Config", "straightForwardDeviation", settingsDialog->getSliderStraightForwardDeviationValue());

			// save check box status
			inifile1->writeSetting("Config", "saveOnExit", settingsDialog->getCheckBoxSaveSettings());

			// Later...
			//
			//noHardwareErrorMessages
			//exitDialog

			// force writing *immediately*
			inifile1->sync();

			emit message("Settings written.");
		}
	}


	// show dialog if set in ini-file
	if (exitDialog == true)
	{
		if (!consoleMode)
		{
			if (forceShutdown==false) // this is true, if no ini-file was found at startup
			{
				// ask user if he really wants to exit.
				if (QMessageBox::question(0, "Exiting...", "Are you sure?", QMessageBox::Yes | QMessageBox::Default, QMessageBox::No | QMessageBox::Escape) == QMessageBox::No)
				{
					//---------
					// if NO
					//---------
					// don't leave! :-)
					return;
				}
			}
		}
		/// \todo ask for exit on console!
	}


//	emit message("STOPPING drive!");
//	drive(STOP); /// \todo what if the robot (serial communication hangs here?!?) tmeout?!?

	if (forceShutdown==false)
	{
		// turn off all RGB LEDs
		rgbLeds->setBrightness(RGBLED1, 1);
		rgbLeds->setBrightness(RGBLED2, 1);
		rgbLeds->setBrightness(RGBLED3, 1);
		rgbLeds->setBrightness(RGBLED4, 1);
		rgbLeds->setBrightness(RGBLED5, 1);
		rgbLeds->setBrightness(RGBLED6, 1);
	}


	/// \todo a universal quit-threads-method
	if (!consoleMode)
	{
		//--------------------------------
		// quit the camThread
		//--------------------------------
		if (camThread->isRunning() == true)
		{
			emit message("Stopping camera thread...");
			emit splashMessage("Stopping camera thread...");

			// my own stop routine :-)
			camThread->stop();

			// slowing thread down
			camThread->setPriority(QThread::IdlePriority);
			camThread->quit();

			//-------------------------------------------
			// start measuring time for timeout ckecking
			//-------------------------------------------
			QTime t;
			t.start();
			do
			{
			} while ((camThread->isFinished() == false) && (t.elapsed() <= 2000));

			if (camThread->isFinished() == true)
			{
				emit message("Camera thread stopped.");
			}
			else
			{
				emit message("ERROR: Terminating camera thread because it doesn't answer...");
				emit splashMessage("Terminating camera thread because it doesn't answer...");
				camThread->terminate();
				camThread->wait(1000);
				emit message("Camera thread terminated.");
			}
		}
	}



	//--------------------------------
	// quit the obstacle check thread
	//--------------------------------
	//qDebug("Starting to stop the obstacle check thread NOW!");
	if (obstCheckThread->isRunning() == true)
	{
		emit message("Stopping obstacle check thread...");
		emit splashMessage("Stopping obstacle check thread...");

		// my own stop routine :-)
		obstCheckThread->stop();

		// slowing thread down
		obstCheckThread->setPriority(QThread::IdlePriority);
		obstCheckThread->quit();

		//-------------------------------------------
		// start measuring time for timeout ckecking
		//-------------------------------------------
		QTime t;
		t.start();
		do
		{
		} while ((obstCheckThread->isFinished() == false) && (t.elapsed() <= 2000));

		if (obstCheckThread->isFinished() == true)
		{
			emit message("Obstacle check thread stopped.");
		}
		else
		{
			emit message("ERROR: Terminating obstacle check thread because it doesn't answer...");
			emit splashMessage("Terminating obstacle check thread because it doesn't answer...");
			obstCheckThread->terminate();
			obstCheckThread->wait(1000);
			emit message("Obstacle check thread terminated.");
		}
	}


	//--------------------------------
	// quit the laserThread
	//--------------------------------
	if (laserThread->isRunning() == true)
	{
		emit message("Stopping laser thread...");
		emit splashMessage("Stopping laser thread...");

		// my own stop routine :-)
		laserThread->stop();

		// slowing thread down
		laserThread->setPriority(QThread::IdlePriority);
		laserThread->quit();

		//-------------------------------------------
		// start measuring time for timeout ckecking
		//-------------------------------------------
		QTime t;
		t.start();
		do
		{
		} while ((laserThread->isFinished() == false) && (t.elapsed() <= 2000));

		if (laserThread->isFinished() == true)
		{
			emit message("Laser thread stopped.");
		}
		else
		{
			emit message("ERROR: Terminating laser thread because it doesn't answer...");
			emit splashMessage("Terminating laser thread because it doesn't answer...");
			laserThread->terminate();
			laserThread->wait(1000);
			emit message("Laser thread terminated.");
		}
	}


	//--------------------------------
	// quit the speakThread
	//--------------------------------
	if (speakThread->isRunning() == true)
	{
		emit message("Stopping speak thread...");
		emit splashMessage("Stopping speak thread...");

		// my own stop routine :-)
		speakThread->stop();

		// slowing thread down
		speakThread->setPriority(QThread::IdlePriority);
		speakThread->quit();

		//-------------------------------------------
		// start measuring time for timeout ckecking
		//-------------------------------------------
		QTime t;
		t.start();
		do
		{
		} while ((speakThread->isFinished() == false) && (t.elapsed() <= 2000));

		if (speakThread->isFinished() == true)
		{
			emit message("Speak thread stopped.");
		}
		else
		{
			emit message("ERROR: Terminating speak thread because it doesn't answer...");
			emit splashMessage("Terminating speak thread because it doesn't answer...");
			speakThread->terminate();
			speakThread->wait(1000);
			emit message("Speak thread terminated.");
		}
	}


	//--------------------------------
	// quit the joystick thread
	//--------------------------------
	if (joystick->isRunning() == true)
	{
		emit message("Stopping joystick thread...");
		emit splashMessage("Stopping joystick thread...");

		// my own stop routine :-)
		joystick->stop();

		// slowing thread down
		joystick->setPriority(QThread::IdlePriority);
		joystick->quit();

		//-------------------------------------------
		// start measuring time for timeout ckecking
		//-------------------------------------------
		QTime t;
		t.start();
		do
		{
		} while ((joystick->isFinished() == false) && (t.elapsed() <= 2000));

		if (joystick->isFinished() == true)
		{
			emit message("Joystick thread stopped.");
		}
		else
		{
			emit message("ERROR: Terminating joystick thread because it doesn't answer...");
			emit splashMessage("Terminating joystick thread because it doesn't answer...");
			joystick->terminate();
			joystick->wait(1000);
			emit message("Joystick thread terminated.");
		}
	}


	//--------------------------------
	// quit the timerThread
	//--------------------------------
	if (timerThread->isRunning() == true)
	{
		emit message("Stopping timer thread...");
		emit splashMessage("Stopping timer thread...");

		// my own stop routine :-)
		timerThread->stop();

		// slowing thread down
		timerThread->setPriority(QThread::IdlePriority);
		timerThread->quit();

		//-------------------------------------------
		// start measuring time for timeout ckecking
		//-------------------------------------------
		QTime t;
		t.start();
		do
		{
		} while ((timerThread->isFinished() == false) && (t.elapsed() <= 2000));

		if (timerThread->isFinished() == true)
		{
			emit message("Timer thread stopped.");
		}
		else
		{
			emit message("ERROR: Terminating timer thread because it doesn't answer...");
			emit splashMessage("Terminating timer thread because it doesn't answer...");
			timerThread->terminate();
			timerThread->wait(1000);
			emit message("Timer thread terminated.");
		}
	}


	//--------------------------------
	// quit the demoThread
	//--------------------------------
	if (demoThread->isRunning() == true)
	{
		emit message("Stopping demo thread...");
		emit splashMessage("Stopping demo thread...");

		// my own stop routine :-)
		demoThread->stop();

		// slowing thread down
		demoThread->setPriority(QThread::IdlePriority);
		demoThread->quit();

		//-------------------------------------------
		// start measuring time for timeout ckecking
		//-------------------------------------------
		QTime t;
		t.start();
		do
		{
		} while ((demoThread->isFinished() == false) && (t.elapsed() <= 2000));

		if (demoThread->isFinished() == true)
		{
			emit message("Timer thread stopped.");
		}
		else
		{
			emit message("ERROR: Terminating demo thread because it doesn't answer...");
			emit splashMessage("Terminating demo thread because it doesn't answer...");
			demoThread->terminate();
			demoThread->wait(1000);
			emit message("Demo thread terminated.");
		}
	}



#ifndef BUILDFORROBOT
	if (!consoleMode)
	{
		//--------------------------------
		// quit the plotThread
		//--------------------------------
		if (plotThread->isRunning() == true)
		{
			emit message("Stopping Plot thread...");
			emit splashMessage("Stopping Plot thread...");

			// my own stop routine :-)
			plotThread->stop();

			// slowing thread down
			plotThread->setPriority(QThread::IdlePriority);
			plotThread->quit();

			//-------------------------------------------
			// start measuring time for timeout ckecking
			//-------------------------------------------
			QTime t;
			t.start();
			do
			{
			} while ((plotThread->isFinished() == false) && (t.elapsed() <= 2000));

			if (plotThread->isFinished() == true)
			{
				emit message("Plot thread stopped.");
			}
			else
			{
				emit message("ERROR: Terminating Plot thread because it doesn't answer...");
				emit splashMessage("Terminating Plot thread because it doesn't answer...");
				plotThread->terminate();
				plotThread->wait(1000);
				emit message("Plot thread terminated.");
			}
		}
	}
#endif


	//--------------------------
	// quit the sensor thread
	//--------------------------
	//qDebug("Starting to stop the sensor thread NOW!");
	if (sensorThread->isRunning() == true)
	{
		emit message("Stopping sensor thread...");
		emit splashMessage("Stopping sensor thread...");

		// my own stop routine :-)
		sensorThread->stop();

		// slowing thread down
		sensorThread->setPriority(QThread::IdlePriority);
		sensorThread->quit();

		//-------------------------------------------
		// start measuring time for timeout ckecking
		//-------------------------------------------
		QTime t;
		t.start();
		do
		{
		} while ((sensorThread->isFinished() == false) && (t.elapsed() <= 2000));

		if (sensorThread->isFinished() == true)
		{
			emit message("Sensor thread stopped.");
		}
		else
		{
			emit message("ERROR: Terminating sensor thread because it doesn't answer...");
			emit splashMessage("Terminating sensor thread because it doesn't answer...");
			sensorThread->terminate();
			sensorThread->wait(1000);
			emit message("Sensor thread terminated.");
		}
	}


/*
	//--------------------------
	/// \todo quit the heartbeat thread
	//--------------------------
	if (heartbeat->isRunning() == true)
	{
		emit message("Stopping heartbeat thread...");

		// my own stop routine :-)
		heartbeat->stop();

		// slowing thread down
		heartbeat->setPriority(QThread::IdlePriority);
		heartbeat->quit();

		//-------------------------------------------
		// start measuring time for timeout ckecking
		//-------------------------------------------
		QTime t;
		t.start();
		do
		{
		} while ((heartbeat->isFinished() == false) && (t.elapsed() <= 2000));

		if (heartbeat->isFinished() == true)
		{
			emit message("Heartbeat thread stopped.");
		}
		else
		{
			emit message("ERROR: Terminating heartbeat thread because it doesn't answer...");
			emit splashMessage("Terminating heartbeat thread because it doesn't answer...");
			heartbeat->terminate();
			heartbeat->wait(1000);
			emit message("Heartbeat thread terminated.");
		}
	}
*/


	//--------------------------------
	// quit the network thread
	//--------------------------------
	if (netThread->isRunning() == true)
	{
		emit message("Stopping network thread...");
		if (!consoleMode)
		{
			gui->appendNetworkLog("Stopping network thread...");
		}
		emit splashMessage("Stopping network thread...");

		// my own stop routine :-)
		netThread->stop();

		// slowing thread down
		netThread->setPriority(QThread::IdlePriority);
		netThread->quit();

		//-------------------------------------------
		// start measuring time for timeout ckecking
		//-------------------------------------------
		QTime t;
		t.start();
		do
		{
		} while ((netThread->isFinished() == false) && (t.elapsed() <= 2000));

		if (netThread->isFinished() == true)
		{
			emit message("Network thread stopped.");
		}
		else
		{
			emit message("ERROR: Terminating network thread because it doesn't answer...");
			emit splashMessage("Terminating network thread because it doesn't answer...");
			netThread->terminate();
			netThread->wait(1000);
			emit message("Network thread terminated.");
		}
	}


	if (forceShutdown==false)
	{
		//-------------------------------------------------------
		// Last init for the robots circuits
		//-------------------------------------------------------
		emit splashMessage("Putting robot to sleep...");
		emit message("Putting robot to sleep...");
		circuit1->sleep(); /// \todo what, if the robote serial communication hangs here? timeout check?

		//-----------------------------
		// close serial port to mc
		//-----------------------------
		emit message("Closing serial port to microcontroller...");
		interface1->closeComPort();
	}
	else
	{
		emit splashMessage("Shutdown forced, no circuit communication anymore...");
		emit message("Shutdown forced, no circuit communication anymore...");
	}


	//-----------------------------
	// last log file message
	//-----------------------------
	if (writeLogFile)
		logfile->appendLog("-------------------------------------------------------------------------------------------- stop -");


	//--------------------------------
	// close all except the mainDialog
	//--------------------------------
	if (!consoleMode)
	{
		if (settingsDialog->isVisible())
			settingsDialog->close();

		if (joystickDialog->isVisible())
			joystickDialog->close();

		if (aboutDialog->isVisible())
			aboutDialog->close();
	}


	if (consoleMode)
	{
		// In the gui mode the quit is done automatically by the close signal.
		// In the Console mode, the following line automaticall calls the Direcs destructor.
		QCoreApplication::quit();
	}
}


Direcs::~Direcs()
{
	//--------------------------------------------------
	// clean up in reverse order (except from the gui)
	//--------------------------------------------------
	delete demoThread;
	delete timerThread;
	delete logfile;
	delete speakThread;
	delete laserThread;
	delete netThread;
	delete joystick;

#ifndef BUILDFORROBOT
	if (!consoleMode)
	{
		delete plotThread;
	}
#endif

	delete inifile1;
	delete obstCheckThread;
	delete servos;
	delete rgbLeds;
	delete motors;
	delete sensorThread;
	// \todo delete heartbeat;
	delete circuit1;
	delete interface1;
	if (!consoleMode)
	{
		delete camThread;
		delete aboutDialog;
		delete joystickDialog;
		delete settingsDialog;
	}

	qDebug("Bye.");

	if (consoleMode)
	{
		delete consoleGui;
	}
	else
	{
		delete splash;
		delete gui;
	}
}


void Direcs::showExitDialog()
{
		emit message("<font color=\"#FF0000\">THERE IS A BIG COMMUNICATION PROBLEM WITH THE SERIAL PORT TO THE ROBOT!</font>");

		/*
		// ask user if he really wants to exit.
		if (QMessageBox::question(0, "Too much errors!",
									"Exit?",
									QMessageBox::Yes | QMessageBox::Default,
									QMessageBox::No | QMessageBox::Escape) == QMessageBox::No)
		{
			//---------
			// if NO
			//---------
			// don't leave! :-)
			return;
		}

		//\todo Is is the correct method to call the destrucotr, to end the program?!?
		QApplication::exit();
		*/
}


void Direcs::showSplashMessage(QString text)
{
	if (!consoleMode)
	{
		splash->showMessage(text, Qt::AlignHCenter | Qt::AlignBottom, splashTextColor);
		// for refreshing the splash...
		QApplication::processEvents();
	}
	else
	{
		QByteArray textForConsole;

		//------------------------------
		// remove HTML tags from string
		//------------------------------
		int start= -1;
		do
		{
			// search for the first HTML "<"
			start = text.indexOf("<");

			if (start != 1)
			{
				text.remove(start, text.indexOf(">") + 1 - start );
			}
		} while (text.contains(">"));
		// till the last HTML ">" is found

		// print text to console
		// qDebug() << text; is NOT used, because it adds quotation marks to all strings
		textForConsole = text.toLatin1();
		qDebug("%s", textForConsole.data());
	}
}


void Direcs::finishSplash()
{
	if (!consoleMode)
	{
		splash->finish(gui);
	}
}


void Direcs::logicalUnit(int sensorAlarm, QDateTime timestamp)
{
	//Q_UNUSED(timestamp);

	//static int alarmCounter = 0;
	static short int lastSensorValue = -1; // < the initial value has to be different to ALL SENSOR-constants!!


	if (!consoleMode)
	{
		// first switch all (old) sensor alarms OFF
		gui->showAlarm(SENSOR1, OFF);
		gui->showAlarm(SENSOR2, OFF);
		gui->showAlarm(SENSOR3, OFF);
		gui->showAlarm(SENSOR4, OFF);
		gui->showAlarm(SENSOR5, OFF);
		gui->showAlarm(SENSOR6, OFF);
		gui->showAlarm(SENSOR7, OFF);
		gui->showAlarm(SENSOR8, OFF);
		gui->showAlarm(SENSOR16, OFF);
	}


	// If all sensor values/the last alarm are/is the same like the last *and* the robot drives (already),
	// do nothing!
	//
	// So if the robot does *not* drive, we skip this step to store the 'prefrerred driving direction'.
	// This will be then used in the drive method, when the roboter later received the START command.
//	if ((sensorAlarm == lastSensorValue) && (robotDrives))  ///  < < < < < < < < robotDrives = false, in WAIT state
	if (sensorAlarm == lastSensorValue)
	{
		// store this sensor alarm value
		lastSensorValue = sensorAlarm;
		//alarmCounter++;
		return;
	}


	/// set motor speed again (if this was reset via GUI slider or by reset button)  < < < <  @todo move motor speed to slot when robot is reset via GUI!!
	//
	emit message("Setting motor speed in microcontroller");
	motors->setMotorSpeed(MOTOR1, mot1Speed);
	motors->setMotorSpeed(MOTOR2, mot2Speed);
	motors->setMotorSpeed(MOTOR3, mot3Speed);
	motors->setMotorSpeed(MOTOR4, mot4Speed);
	emit message("Motor speed set.");


	if (sensorAlarm == NONE)
	{
		// show new driving direction in the GUI
		emit showPreferredDirection("FORWARD");

		// clear alarm buffers
		obstacleAlarmFrontLeftList.clear();
		obstacleAlarmFrontRightList.clear();

		// store this sensor alarm value
		lastSensorValue = sensorAlarm;
		// reset the alarm counter
		//alarmCounter = 0;

		// store the preferred driving direction for the drive() slot, when we use the START command
		preferredDrivingDirection = FORWARD;

		if (robotDrives)
		{
			//emit message("No obstacle in front of any sensor. :-)");
			//emit message("Driving forward...");

			//----------------
			// drive forward
			//----------------
			drive(FORWARD);
			motors->flashlight(OFF);
		}

		return;
	}


	if (sensorAlarm == OBSTACLEFRONTLEFT)
	{
		// increase "alarm left" buffer
		obstacleAlarmFrontLeftList.append(timestamp);

		// delete the oldest "alarm right" from alarm list
		if (!obstacleAlarmFrontRightList.isEmpty())
		{
			obstacleAlarmFrontRightList.removeFirst();
		}

		// check if this is a real or false alarm
		if (obstacleAlarmFrontLeftList.count() > MAXFALSEALARMS)
		{
			// show new driving direction in the GUI
			emit showPreferredDirection("RIGHT");

			// clear alarm buffer
			obstacleAlarmFrontLeftList.clear();

			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			//alarmCounter = 0;

			// store the preferred driving direction for the drive() slot, when we use the START command
			preferredDrivingDirection = TURNRIGHT;
			if (robotDrives)
			{
				emit message("<b>Obstacle front left. Turning right.</b>");

				//----------------
				// drive right
				//----------------
				drive(TURNRIGHT);
				motors->flashlight(OFF);
			}
		}

		return;
	}


	if (sensorAlarm == OBSTACLEFRONTRIGHT)
	{
		// increase "alarm right" buffer
		obstacleAlarmFrontRightList.append(timestamp);

		// delete the oldest "alarm left" from alarm list
		if (!obstacleAlarmFrontLeftList.isEmpty())
		{
			obstacleAlarmFrontLeftList.removeFirst();
		}

		// check if this is a real or false alarm
		if (obstacleAlarmFrontRightList.count() > MAXFALSEALARMS)
		{
			// show new driving direction in the GUI
			emit showPreferredDirection("LEFT");

			// clear alarm buffer
			obstacleAlarmFrontRightList.clear();

			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			//alarmCounter = 0;

			// store the preferred driving direction for the drive() slot, when we use the START command
			preferredDrivingDirection = TURNLEFT;
			if (robotDrives)
			{
				emit message("<b>Obstacle front right. Turning left.</b>");

				//----------------
				// drive left
				//----------------
				drive(TURNLEFT);
				motors->flashlight(OFF);
			}
		}

		return;
	}



	if (sensorAlarm == OBSTACLESEVERYWHEREINFRONT)
	{
		// show driving direction in the GUI
		emit showPreferredDirection("NONE");

		// clear alarm buffers
		obstacleAlarmFrontLeftList.clear();
		obstacleAlarmFrontRightList.clear();

		// store this sensor alarm value
		lastSensorValue = sensorAlarm;
		// reset the alarm counter
		//alarmCounter = 0;

		// store the preferred driving direction for the drive() slot, when we use the START command
		preferredDrivingDirection = WAIT;

		if (robotDrives)
		{
			emit message("<b>Obstacles everywhere in front of the robot. Waiting.</b>");

			//----------------
			// drive WAIT
			//----------------
			drive(WAIT); /// < < < < sets robotDrives = false !!
			motors->flashlight(ON);
			emit speak("Could you please clear the way for me? Thanks.");
		}

		return;
	}
}


void Direcs::enableFaceTracking(int state)
{
	if (state == Qt::Checked)
	{
		faceTrackingIsEnabled = true;
	}
	else
	{
		faceTrackingIsEnabled = false;
	}
}


void Direcs::faceTracking(int faces, int faceX, int faceY, int faceRadius)
{
	Q_UNUSED(faces);
	Q_UNUSED(faceX);
	Q_UNUSED(faceY);
	Q_UNUSED(faceRadius);

/*

\todo kinect stuff

	if (!consoleMode)
	{
		Q_UNUSED (faces) // not in use, at the moment

		// \todo put values to consts or ini
		int xLevelRight = (camThread->imageWidth()  / 2) + faceRadius;
		int xLevelLeft  = (camThread->imageWidth()  / 2) - faceRadius;
		int yLevelUp    = (camThread->imageHeight() / 2) - faceRadius;
		int yLevelDown  = (camThread->imageHeight() / 2) + faceRadius;


		// track nowhere (face is in the middle) or faceRadius is 0 -> no faces detected
		if (
				(faceRadius==0) || ((faceX > xLevelLeft) && ((faceX < xLevelRight)) && (faceY > yLevelUp) && (faceY < yLevelDown) ) ||
				(faceRadius == 0)
		)
		{
			//head->look("FORWARD");
			//head->look("NORMAL");
			emit showFaceTrackDirection("NONE");

			if (faceTrackingIsEnabled)
			{
				//motors->motorControl(MOTOR3, OFF, SAME);
				//motors->motorControl(MOTOR4, OFF, SAME);
			}
			return;
		}


		// face detected :-)
		//head->look("CURIOUS");


		// track left
		if ( (faceX < xLevelLeft) && (faceY > yLevelUp) && (faceY < yLevelDown) )
		{
			if (faceTrackingIsEnabled)
			{
				head->look("LEFT");
				//motors->motorControl(MOTOR3, ON, COUNTERCLOCKWISE);
				//motors->motorControl(MOTOR4, OFF, SAME);
			}
			emit showFaceTrackDirection("LEFT");
			return;
		}

		// track right
		if ( (faceX > xLevelRight) && (faceY > yLevelUp) && (faceY < yLevelDown) )
		{
			if (faceTrackingIsEnabled)
			{
				head->look("RIGHT");
				//motors->motorControl(MOTOR3, ON, CLOCKWISE);
				//motors->motorControl(MOTOR4, OFF, SAME);
			}
			emit showFaceTrackDirection("RIGHT");
			return;
		}

		// track up
		if ( (faceX > xLevelLeft) && (faceX < xLevelRight) && (faceY < yLevelUp) )
		{
			if (faceTrackingIsEnabled)
			{
				head->look("UP");
				//motors->motorControl(MOTOR3, OFF, SAME);
				//motors->motorControl(MOTOR4, ON, CLOCKWISE);
			}
			emit showFaceTrackDirection("UP");
			return;
		}

		// track up left
		if ( (faceX < xLevelLeft) && (faceY < yLevelUp) )
		{
			if (faceTrackingIsEnabled)
			{
				head->look("UPLEFT");
				//motors->motorControl(MOTOR3, ON, COUNTERCLOCKWISE);
				//motors->motorControl(MOTOR4, ON, CLOCKWISE);
			}
			emit showFaceTrackDirection("UPLEFT");
			return;
		}

		// track up right
		if ( (faceX > xLevelLeft) && (faceY < yLevelUp) )
		{
			if (faceTrackingIsEnabled)
			{
				head->look("UPRIGHT");
				//motors->motorControl(MOTOR3, ON, CLOCKWISE);
				//motors->motorControl(MOTOR4, ON, CLOCKWISE);
			}
			emit showFaceTrackDirection("UPRIGHT");
			return;
		}

		// track down
		if ( (faceX > xLevelLeft) && (faceX < xLevelRight) && (faceY > yLevelDown) )
		{
			if (faceTrackingIsEnabled)
			{
				head->look("DOWN");
				//motors->motorControl(MOTOR3, OFF, SAME);
				//motors->motorControl(MOTOR4, ON, COUNTERCLOCKWISE);
			}
			emit showFaceTrackDirection("DOWN");
			return;
		}

		// track down left
		if ( (faceX < xLevelLeft) && (faceY > yLevelDown) )
		{
			if (faceTrackingIsEnabled)
			{
				head->look("DOWNLEFT");
				//motors->motorControl(MOTOR3, ON, COUNTERCLOCKWISE);
				//motors->motorControl(MOTOR4, ON, COUNTERCLOCKWISE);
			}
			emit showFaceTrackDirection("DOWNLEFT");
			return;
		}

		// track down right
		if ( (faceX > xLevelRight) && (faceY > yLevelDown) )
		{
			if (faceTrackingIsEnabled)
			{
				head->look("DOWNRIGHT");
				//motors->motorControl(MOTOR3, ON, CLOCKWISE);
				//motors->motorControl(MOTOR4, ON, COUNTERCLOCKWISE);
			}
			emit showFaceTrackDirection("DOWNRIGHT");
			return;
		}
	}
*/
}


void Direcs::showSensorData()
{
	if (!consoleMode)
	{
		/*
		//----------------------------------------
		// show sensor values with progress bars
		//----------------------------------------
		gui->showDistanceGraphical(SENSOR1, sensorThread->getDistance(SENSOR1));
		gui->showDistanceGraphical(SENSOR2, sensorThread->getDistance(SENSOR2));
		gui->showDistanceGraphical(SENSOR3, sensorThread->getDistance(SENSOR3));
		gui->showDistanceGraphical(SENSOR4, sensorThread->getDistance(SENSOR4));
		gui->showDistanceGraphical(SENSOR5, sensorThread->getDistance(SENSOR5));
		gui->showDistanceGraphical(SENSOR6, sensorThread->getDistance(SENSOR6));
		gui->showDistanceGraphical(SENSOR7, sensorThread->getDistance(SENSOR7));
		gui->showDistanceGraphical(SENSOR8, sensorThread->getDistance(SENSOR8));
		gui->showDistanceGraphical(SENSOR16, sensorThread->getUsSensorValue(SENSOR16));

		//-------------------------------------------------------
		// show distance value in a text label (in centimeters!)
		//-------------------------------------------------------
		gui->showDistance(SENSOR1, sensorThread->getDistance(SENSOR1));
		gui->showDistance(SENSOR2, sensorThread->getDistance(SENSOR2));
		gui->showDistance(SENSOR3, sensorThread->getDistance(SENSOR3));
		gui->showDistance(SENSOR4, sensorThread->getDistance(SENSOR4));
		gui->showDistance(SENSOR5, sensorThread->getDistance(SENSOR5));
		gui->showDistance(SENSOR6, sensorThread->getDistance(SENSOR6));
		gui->showDistance(SENSOR7, sensorThread->getDistance(SENSOR7));
		gui->showDistance(SENSOR8, sensorThread->getDistance(SENSOR8));
		gui->showDistance(SENSOR16, sensorThread->getUsSensorValue(SENSOR16));
		*/

		//--------------------------------------------------------------
		// show driven distance value in a text label (in centimeters!)
		//--------------------------------------------------------------
		gui->showDrivenDistance( MOTORSENSOR1, sensorThread->getDrivenDistance(MOTORSENSOR1) );
		gui->showDrivenDistance( MOTORSENSOR2, sensorThread->getDrivenDistance(MOTORSENSOR2) );

		//-------------------------------------------------------
		// show voltage value in a text label
		//-------------------------------------------------------
		gui->showVoltage(VOLTAGESENSOR1, sensorThread->getVoltage(VOLTAGESENSOR1) );
		gui->showVoltage(VOLTAGESENSOR2, sensorThread->getVoltage(VOLTAGESENSOR2) );
	}

/*
	//-------------------------------------------------------
	// Voltage alarm!
	// \todo do a check and emit this to the gui to show the lables in red!
	//-------------------------------------------------------
	// \todo is this the best place for the following lines?
	static bool marker=false; // \todo do this with a timer or something, for repeating it

	if (sensorThread->getVoltage(VOLTAGESENSOR1) < MINIMUMVOLTAGE1)
	{

		if (!marker)
		{
			marker = true;
			emit speak("The 12 volt batteries are empty.");
			// \todo maybe also do a shutdown or a 'stop" while driving...?!?
		}
	}
	else
	{
		marker = false;
	}

	if (sensorThread->getVoltage(VOLTAGESENSOR2) < MINIMUMVOLTAGE2)
	{

		if (!marker)
		{
			marker = true;
			emit speak("The 24 volt batteries are empty.");
			// \todo maybe also do a shutdown or a 'stop" while driving...?!?
		}
	}
	else
	{
		marker = false;
	}
*/
}


void Direcs::drive(const int command)
{
	static unsigned char lastCommand = 255;


	// if its the same command, do nothing
	if (command == lastCommand)
	{
		return;
	}

	// store the current command
	lastCommand = command;

	switch (command)
	{
		case FORWARD:
			emit message("FORWARD... ", false);
			if (!consoleMode)
			{
				gui->showMotorStatus(MOTOR1, SAME, FORWARD);
				gui->showMotorStatus(MOTOR2, SAME, FORWARD);
				gui->showMotorStatus(MOTOR3, SAME, FORWARD);
				gui->showMotorStatus(MOTOR4, SAME, FORWARD);
			}

			drivingLight(GREEN);

			if (motors->motorControl(ALLMOTORS, SAME, command))
			{
				emit message("ok", true, false, false);
			}
			else
			{
				emit message("Atmel-Error!");
			}
			return;
			break;

		case BACKWARD:
			emit message("BACKWARD... ", false);
			if (!consoleMode)
			{
				gui->showMotorStatus(MOTOR1, SAME, BACKWARD);
				gui->showMotorStatus(MOTOR2, SAME, BACKWARD);
				gui->showMotorStatus(MOTOR3, SAME, BACKWARD);
				gui->showMotorStatus(MOTOR4, SAME, BACKWARD);
			}

			drivingLight(GREEN);

			if (motors->motorControl(ALLMOTORS, SAME, command))
			{
				emit message("ok", true, false, false);
			}
			else
			{
				emit message("Atmel-Error!");
			}
			return;
			break;

		case LEFT:
			emit message("LEFT... ", false);
			if (!consoleMode)
			{
				gui->showMotorStatus(MOTOR1, SAME, BACKWARD);
				gui->showMotorStatus(MOTOR2, SAME, FORWARD);
				gui->showMotorStatus(MOTOR3, SAME, FORWARD);
				gui->showMotorStatus(MOTOR4, SAME, BACKWARD);
			}

			drivingLight(GREEN);

			if (motors->motorControl(ALLMOTORS, SAME, command))
			{
				emit message("ok", true, false, false);
			}
			else
			{
				emit message("Atmel-Error!");
			}
			return;
			break;

		case RIGHT:
			emit message("RIGHT... ", false);
			if (!consoleMode)
			{
				gui->showMotorStatus(MOTOR1, SAME, FORWARD);
				gui->showMotorStatus(MOTOR2, SAME, BACKWARD);
				gui->showMotorStatus(MOTOR3, SAME, BACKWARD);
				gui->showMotorStatus(MOTOR4, SAME, FORWARD);
			}

			drivingLight(GREEN);

			if (motors->motorControl(ALLMOTORS, SAME, command))
			{
				emit message("ok", true, false, false);
			}
			else
			{
				emit message("Atmel-Error!");
			}
			return;
			break;

		case TURNLEFT:
			emit message("TURNLEFT... ", false);
			if (!consoleMode)
			{
				gui->showMotorStatus(MOTOR1, SAME, FORWARD);
				gui->showMotorStatus(MOTOR2, SAME, BACKWARD);
				gui->showMotorStatus(MOTOR3, SAME, FORWARD);
				gui->showMotorStatus(MOTOR4, SAME, BACKWARD);
			}

			drivingLight(GREEN);

			if (motors->motorControl(ALLMOTORS, SAME, command))
			{
				emit message("ok", true, false, false);
			}
			else
			{
				emit message("Atmel-Error!");
			}
			return;
			break;

		case TURNRIGHT:
			emit message("TURNRIGHT... ", false);
			if (!consoleMode)
			{
				gui->showMotorStatus(MOTOR1, SAME, BACKWARD);
				gui->showMotorStatus(MOTOR2, SAME, FORWARD);
				gui->showMotorStatus(MOTOR3, SAME, BACKWARD);
				gui->showMotorStatus(MOTOR4, SAME, FORWARD);
			}

			drivingLight(GREEN);

			if (motors->motorControl(ALLMOTORS, SAME, command))
			{
				emit message("ok", true, false, false);
			}
			else
			{
				emit message("Atmel-Error!");
			}
			return;
			break;

		case START:
			robotDrives = true;
			emit message("Starting to drive...");
			emit message("START... ", false);

		/*
			// set the motors to "drive FORWARD"
			if (!consoleMode)
			{
				gui->showMotorStatus(MOTOR1, ON, FORWARD);
				gui->showMotorStatus(MOTOR2, ON, FORWARD);
				gui->showMotorStatus(MOTOR3, ON, FORWARD);
				gui->showMotorStatus(MOTOR4, ON, FORWARD);
			}
			/ *	FIXME: to much data over serial port?!?
			motors->setMotorSpeed(MOTOR1, 0); /// \todo check if this works
			motors->setMotorSpeed(MOTOR2, 0); /// \todo check if this works
			motors->setMotorSpeed(MOTOR3, 0); /// \todo check if this works
			motors->setMotorSpeed(MOTOR4, 0); /// \todo check if this works

			resetDrivingSpeedTimer();
			drivingSpeedTimer->start(DRIVINGSPEEDINCREASER);
			/// \TODO put that to a slider in the config menu / file.
			/// \TODO make this speed increaser optional!
			* /
*/
			drivingLight(GREEN);
/*
			if (motors->motorControl(ALLMOTORS, SAME, command))
			{
				emit message("ok", true, false, false);
			}
			else
			{
				emit message("Atmel-Error!");
			}
*/

			// use the preferred driving start direction, already set in the logicalUnit
			drive(preferredDrivingDirection);

			return;
			break;

		case WAIT:
			emit message("WAIT... ", false);
			/*	FIXME: to much data over serial port?!?
			resetDrivingSpeedTimer();
			*/
			if (!consoleMode)
			{
				gui->showMotorStatus(MOTOR1, OFF, SAME);
				gui->showMotorStatus(MOTOR2, OFF, SAME);
				gui->showMotorStatus(MOTOR3, OFF, SAME);
				gui->showMotorStatus(MOTOR4, OFF, SAME);
			}

			drivingLight(WHITE);

			// turning motors off
			if (motors->motorControl(ALLMOTORS, SAME, command))
			{
				emit message("ok", true, false, false);
			}
			else
			{
				emit message("Atmel-Error!");
			}

			// Don't stop the motThread (PWM)!
			// Only switching motors off!
///			robotDrives = false;				@todo TEST: DISABLED TO BRING THE ROBOT TO DRIVE AGAIN when obstacles are gone!!
			return;
			break;

		case STOP:
			emit message("STOP... ", false);
			/*	FIXME: to much data over serial port?!?
			resetDrivingSpeedTimer();
			*/
			if (!consoleMode)
			{
				gui->showMotorStatus(MOTOR1, OFF, SAME);
				gui->showMotorStatus(MOTOR2, OFF, SAME);
				gui->showMotorStatus(MOTOR3, OFF, SAME);
				gui->showMotorStatus(MOTOR4, OFF, SAME);
			}

			drivingLight(WHITE);

			// turning motors off
			if (motors->motorControl(ALLMOTORS, SAME, command))
			{
				emit message("ok", true, false, false);
			}
			else
			{
				emit message("Atmel-Error!");
			}

			// Don't stop the motThread (clock)!
			// Only switching motors off!
			robotDrives = false;
			return;
			break;

		case MOTOR1FW: // for the test widget in the GUI!!
			emit message("Motor 1 forward");
			if (!consoleMode)
			{
				gui->showMotorStatus(MOTOR1, ON, BACKWARD);
			}
			if (!motors->motorControl(MOTOR1, ON, BACKWARD))
				emit message("ERROR motor 1 CCW");
			return;
			break;

		case MOTOR1BW: // for the test widget in the GUI!!
			emit message("Motor 1 backward");
			if (!consoleMode)
			{
				gui->showMotorStatus(MOTOR1, ON, FORWARD);
			}
			if (!motors->motorControl(MOTOR1, ON, FORWARD))
				emit message("ERROR motor 1 CW");
			return;
			break;

		case MOTOR1OFF: // for the test widget in the GUI!!
			emit message("Motor 1 OFF");
			if (!consoleMode)
			{
				gui->showMotorStatus(MOTOR1, OFF, SAME);
			}
			if (!motors->motorControl(MOTOR1, OFF, SAME))
				emit message("ERROR motor 1 OFF");
			return;
			break;

		case MOTOR2FW: // for the test widget in the GUI!!
			emit message("Motor 2 forward");
			if (!consoleMode)
			{
				gui->showMotorStatus(MOTOR2, ON, BACKWARD);
			}
			motors->motorControl(MOTOR2, ON, BACKWARD);
			return;
			break;

		case MOTOR2BW: // for the test widget in the GUI!!
			emit message("Motor 2 backward");
			if (!consoleMode)
			{
				gui->showMotorStatus(MOTOR2, ON, FORWARD);
			}
			motors->motorControl(MOTOR2, ON, FORWARD);
			return;
			break;

		case MOTOR2OFF: // for the test widget in the GUI!!
			emit message("Motor 2 OFF");
			if (!consoleMode)
			{
				gui->showMotorStatus(MOTOR2, OFF, SAME);
			}
			motors->motorControl(MOTOR2, OFF, SAME);
			return;
			break;

		case MOTOR3FW: // for the test widget in the GUI!!
			emit message("Motor 3 forward");
			if (!consoleMode)
			{
				gui->showMotorStatus(MOTOR3, ON, BACKWARD);
			}
			motors->motorControl(MOTOR3, ON, BACKWARD);
			return;
			break;

		case MOTOR3BW: // for the test widget in the GUI!!
			emit message("Motor 3 backward");
			if (!consoleMode)
			{
				gui->showMotorStatus(MOTOR3, ON, FORWARD);
			}
			motors->motorControl(MOTOR3, ON, FORWARD);
			return;
			break;

		case MOTOR3OFF: // for the test widget in the GUI!!
			emit message("Motor 3 OFF");
			if (!consoleMode)
			{
				gui->showMotorStatus(MOTOR3, OFF, SAME);
			}
			motors->motorControl(MOTOR3, OFF, SAME);
			return;
			break;

		case MOTOR4FW: // for the test widget in the GUI!!
			emit message("Motor 4 forward");
			if (!consoleMode)
			{
				gui->showMotorStatus(MOTOR4, ON, BACKWARD);
			}
			motors->motorControl(MOTOR4, ON, BACKWARD);
			return;
			break;

		case MOTOR4BW: // for the test widget in the GUI!!
			emit message("Motor 4 backward");
			if (!consoleMode)
			{
				gui->showMotorStatus(MOTOR4, ON, FORWARD);
			}
			motors->motorControl(MOTOR4, ON, FORWARD);
			return;
			break;

		case MOTOR4OFF: // for the test widget in the GUI!!
			emit message("Motor 4 OFF");
			if (!consoleMode)
			{
				gui->showMotorStatus(MOTOR4, OFF, SAME);
			}
			motors->motorControl(MOTOR4, OFF, SAME);
			return;
			break;
		default:
			// this case should never occur
			emit message(QString("<font color=\"#FF0000\">ERROR: Driving direction '%1' not implemented or set in Drive()!</font>").arg(command));
			return;
			break;
	}
}


void Direcs::resetDrivingSpeedTimer(void)
{
	endSpeedMotor1Reached = false;
	endSpeedMotor2Reached = false;
	endSpeedMotor3Reached = false;
	endSpeedMotor4Reached = false;
}


void Direcs::increaseDrivingSpeed(void)
{
	int currentSpeed1 = 0;
	int currentSpeed2 = 0;
	int currentSpeed3 = 0;
	int currentSpeed4 = 0;
	int increaseInterval = qRound(maximumSpeed * 0.1); /// increase interval is 10%. \todo start with which minimumSpeed?


	currentSpeed1 = motors->getMotorSpeed(MOTOR1);
	if (currentSpeed1 < maximumSpeed)
	{
		endSpeedMotor1Reached = false;
		currentSpeed1 += increaseInterval;
		motors->setMotorSpeed(MOTOR1, currentSpeed1);
// 		qDebug("increasing from speed %d to %d @ motor %d.", currentSpeed1, maximumSpeed, MOTOR1);
	}
	else
	{
		endSpeedMotor1Reached = true;
	}

	currentSpeed2 = motors->getMotorSpeed(MOTOR2);
	if (currentSpeed2 < maximumSpeed)
	{
		endSpeedMotor2Reached = false;
		currentSpeed2 += increaseInterval;
		motors->setMotorSpeed(MOTOR2, currentSpeed2);
	}
	else
	{
		endSpeedMotor2Reached = true;
	}

	currentSpeed3 = motors->getMotorSpeed(MOTOR3);
	if (currentSpeed3 < maximumSpeed)
	{
		endSpeedMotor3Reached = false;
		currentSpeed3 += increaseInterval;
		motors->setMotorSpeed(MOTOR3, currentSpeed3);
	}
	else
	{
		endSpeedMotor3Reached = true;
	}

	currentSpeed4 = motors->getMotorSpeed(MOTOR4);
	if (currentSpeed4 < maximumSpeed)
	{
		endSpeedMotor4Reached = false;
		currentSpeed4 += increaseInterval;
		motors->setMotorSpeed(MOTOR4, currentSpeed4);
	}
	else
	{
		endSpeedMotor4Reached = true;
	}

	// all motors at their end speed? /// \todo check if end speed is refreshed in this class, when changed via gui!!
	if (endSpeedMotor1Reached && endSpeedMotor2Reached && endSpeedMotor3Reached && endSpeedMotor4Reached)
	{
		drivingSpeedTimer->stop();
		// qDebug("**driving speed timer stopped**");
	}
}


void Direcs::readSettings()
{
	//---------------------------------------------------------------------
	// get the programm settings and set the items on the gui (sliders...)
	//---------------------------------------------------------------------
	emit message("Reading settings...");


	//---------------------------------------------------------------------
	// read setting
	switch (inifile1->readSetting("Config", "writeLogFile"))
	{
		case -1:
			emit message("<font color=\"#FF0000\">Value \"writeLogFile\"not found in ini-file!</font>");
			break;
		case 0:
			writeLogFile = false;
			emit message("Not writing a logfile.");
			break;
		case 1:
			writeLogFile = true;
			emit message("Writing a logfile!");
			//--------------------------------------------------------------------------
			// write messages for the GUI or fotthe console ALSO to a logfile
			//--------------------------------------------------------------------------
			connect(this, SIGNAL( message(QString) ), logfile, SLOT( appendLog(QString) ));
			break;
	}


	//---------------------------------------------------------------------
	// read setting
	#ifdef Q_OS_LINUX
		QString portString = "serialPortMicrocontrollerLinux";
	#endif

	#ifdef Q_OS_MAC
		QString portString = "serialPortMicrocontrollerMac";
	#endif

	serialPortMicrocontroller = inifile1->readString("Config", portString);

	if (serialPortMicrocontroller == "error1")
	{
		emit message(QString("<font color=\"#FF0000\">Value \"%1\" not found in ini-file!</font>").arg(portString));
	}
	else
	{
		//
		// everything okay
		//
		emit message(QString("Serial port for microcontroller set to <b>%1</b>.").arg(serialPortMicrocontroller));
	}


	//---------------------------------------------------------------------
	// read first FRONT laser setting
	laserscannerTypeFront = inifile1->readString("Config", "laserscannerTypeFront");

	if (laserscannerTypeFront == "error1")
	{
		laserThread->setType(LASER1, "none");
		emit message("<font color=\"#FF0000\">Value \"laserscannerTypeFront\" not found in ini-file!</font>");
	}
	else
	{
		// everything okay
		laserThread->setType(LASER1, laserscannerTypeFront);
		emit message(QString("Front laser scanner type set to <b>%1</b>.").arg(laserscannerTypeFront));


		//---------------------------------------------------------------------
		// read next laser setting
		#ifdef Q_OS_LINUX
			portString = "serialPortLaserscannerFrontLinux";
		#endif

		#ifdef Q_OS_MAC
			portString = "serialPortLaserscannerFrontMac";
		#endif

		serialPortLaserscannerFront = inifile1->readString("Config", portString);

		if (serialPortLaserscannerFront == "error1")
		{
			laserThread->setSerialPort(LASER1, "none");
			emit message(QString("<font color=\"#FF0000\">Value \"%1\" not found in ini-file!</font>").arg(portString));
		}
		else
		{
			// everything okay
			laserThread->setSerialPort(LASER1, serialPortLaserscannerFront);
			emit message(QString("Front laser scanner port set to <b>%1</b>.").arg(serialPortLaserscannerFront));

			//---------------------------------------------------------------------
			// read next laser setting
			laserscannerMounting = inifile1->readString("Config", "laserscannerMountingFront");

			if (laserscannerMounting == "error1")
			{
				laserThread->setMounting(LASER1, "normal");
				emit message("<font color=\"#FF0000\">Value \"laserscannerMountingFront\" not found in ini-file!</font>");
			}
			else
			{
				// everything okay
				laserThread->setMounting(LASER1, laserscannerMounting);
				emit message(QString("Front laser scanner mounting set to <b>%1</b>.").arg(laserscannerMounting));

				//---------------------------------------------------------------------
				// read next setting
				laserscannerAngleFront = inifile1->readSetting("Config", "laserscannerAngleFront");

				switch (laserscannerAngleFront)
				{
					case -1:
						emit message("<font color=\"#FF0000\">Value \"laserscannerAngleFront\"not found in ini-file!</font>");
						break;
					default:
						laserThread->setAngle(LASER1, laserscannerAngleFront);
						if (consoleMode)
						{
							consoleGui->setLaserscannerAngle(LASER1, laserscannerAngleFront);
						}
						else
						{
							gui->setLaserscannerAngle(LASER1, laserscannerAngleFront);
						}
						emit message(QString("Front laser scanner angle set to <b>%1</b>.").arg(laserscannerAngleFront));

						//---------------------------------------------------------------------
						// read next setting
						floatValue = inifile1->readFloat("Config", "laserscannerResolutionFront");

						if (floatValue == -1)
						{
							emit message("<font color=\"#FF0000\">Value \"laserscannerResolutionFront\" not found in ini-file!</font>");
						}
						else
						{
							// everything okay
							laserThread->setResolution(LASER1, floatValue);
							if (consoleMode)
							{
								consoleGui->setLaserscannerResolution(LASER1, floatValue);
							}
							else
							{
								gui->setLaserscannerResolution(LASER1, floatValue);
							}
							emit message(QString("Front laser scanner resolution set to <b>%1</b>.").arg(floatValue));


							//---------------------------------------------------------------------
							// read next setting
							laserscannerIgnoreAreaStart = inifile1->readSetting("Config", "laserscannerFrontIgnoreArea1Start");

							if (laserscannerIgnoreAreaStart == -1)
							{
								emit message("<font color=\"#FF0000\">Value \"laserscannerFrontIgnoreArea1Start\"not found in ini-file!</font>");
							}
							else
							{

								//---------------------------------------------------------------------
								// read next setting
								laserscannerIgnoreAreaEnd = inifile1->readSetting("Config", "laserscannerFrontIgnoreArea1End");

								if (laserscannerIgnoreAreaEnd == -1)
								{
									emit message("<font color=\"#FF0000\">Value \"laserscannerFrontIgnoreArea1End\"not found in ini-file!</font>");
								}
								else
								{

									if (consoleMode)
									{
										/// @todo add this: consoleGui->setLaserscannerAngle(LASER1, laserscanner...Front);
									}
									else
									{
										/// @todo gui->setLaserscannerAngle(LASER1, laserscanner...Front);
									}
									emit message(QString("Front laser scanner ignore area %1 set to <b>%2-%3%1</b>.").arg(AREA1).arg(laserscannerIgnoreAreaStart).arg(laserscannerIgnoreAreaEnd));

									// store settings in obstacle check thread
									obstCheckThread->setIgnoreArea(LASER1, AREA1, laserscannerIgnoreAreaStart, laserscannerIgnoreAreaEnd);
								}
							}


							//---------------------------------------------------------------------
							// read next setting
							laserscannerIgnoreAreaStart = inifile1->readSetting("Config", "laserscannerFrontIgnoreArea2Start");

							if (laserscannerIgnoreAreaStart == -1)
							{
								emit message("<font color=\"#FF0000\">Value \"laserscannerFrontIgnoreArea2Start\"not found in ini-file!</font>");
							}
							else
							{

								//---------------------------------------------------------------------
								// read next setting
								laserscannerIgnoreAreaEnd = inifile1->readSetting("Config", "laserscannerFrontIgnoreArea2End");

								if (laserscannerIgnoreAreaEnd == -1)
								{
									emit message("<font color=\"#FF0000\">Value \"laserscannerFrontIgnoreArea2End\"not found in ini-file!</font>");
								}
								else
								{

									if (consoleMode)
									{
										/// @todo add this: consoleGui->setLaserscannerAngle(LASER1, laserscanner...Front);
									}
									else
									{
										/// @todo gui->setLaserscannerAngle(LASER1, laserscanner...Front);
									}
									emit message(QString("Front laser scanner ignore area %1 set to <b>%2-%3%1</b>.").arg(AREA2).arg(laserscannerIgnoreAreaStart).arg(laserscannerIgnoreAreaEnd));

									// store settings in obstacle check thread
									obstCheckThread->setIgnoreArea(LASER1, AREA2, laserscannerIgnoreAreaStart, laserscannerIgnoreAreaEnd);
								}
							}
						}
					break;
				}
			}
		}
	}

	//---------------------------------------------------------------------
	// read first REAR laser setting
	laserscannerTypeRear = inifile1->readString("Config", "laserscannerTypeRear");

	if (laserscannerTypeRear == "error2")
	{
		laserThread->setType(LASER2, "none");
		emit message("<font color=\"#FF0000\">ini-file is not writeable!</font>");
	}
	else
	{
		if (laserscannerTypeRear == "error1")
		{
			laserThread->setType(LASER2, "none");
			emit message("<font color=\"#FF0000\">Value \"laserscannerTypeRear\" not found in ini-file!</font>");
		}
		else
		{
			// everything okay
			laserThread->setType(LASER2, laserscannerTypeRear);
			emit message(QString("Rear laser scanner type set to <b>%1</b>.").arg(laserscannerTypeRear));

			//---------------------------------------------------------------------
			// read next laser setting
			serialPortLaserscannerRear = inifile1->readString("Config", "serialPortLaserscannerRear");

			if (serialPortLaserscannerRear == "error2")
			{
				laserThread->setSerialPort(LASER2, "none");
				emit message("<font color=\"#FF0000\">ini-file is not writeable!</font>");
			}
			else
			{
				if (serialPortLaserscannerRear == "error1")
				{
					laserThread->setSerialPort(LASER2, "none");
					emit message("<font color=\"#FF0000\">Value \"serialPortLaserscannerRear\" not found in ini-file!</font>");
				}
				else
				{
					// everything okay
					laserThread->setSerialPort(LASER2, serialPortLaserscannerRear);
					emit message(QString("Rear laser scanner port set to <b>%1</b>.").arg(serialPortLaserscannerRear));

					//---------------------------------------------------------------------
					// read next laser setting
					laserscannerMounting = inifile1->readString("Config", "laserscannerMountingRear");

					if (laserscannerMounting == "error2")
					{
						laserThread->setMounting(LASER2, "normal");
						emit message("<font color=\"#FF0000\">ini-file is not writeable!</font>");
					}
					else
					{
						if (laserscannerMounting == "error1")
						{
							laserThread->setMounting(LASER2, "normal");
							emit message("<font color=\"#FF0000\">Value \"laserscannerMountingRear\" not found in ini-file!</font>");
						}
						else
						{
							// everything okay
							laserThread->setMounting(LASER2, laserscannerMounting);
							emit message(QString("Rear laser scanner mounting set to <b>%1</b>.").arg(laserscannerMounting));

							//---------------------------------------------------------------------
							// read next setting
							laserscannerAngleRear = inifile1->readSetting("Config", "laserscannerAngleRear");

							switch (laserscannerAngleRear)
							{
							case -1:
								emit message("<font color=\"#FF0000\">Value \"laserscannerAngleRear\"not found in ini-file!</font>");
								break;
							default:
								laserThread->setAngle(LASER2, laserscannerAngleRear);
								if (consoleMode)
								{
									consoleGui->setLaserscannerAngle(LASER2, laserscannerAngleRear);
								}
								else
								{
									gui->setLaserscannerAngle(LASER2, laserscannerAngleRear);
								}
								emit message(QString("Rear laser scanner angle set to <b>%1</b>.").arg(laserscannerAngleRear));

								//---------------------------------------------------------------------
								// read next setting
								floatValue = inifile1->readFloat("Config", "laserscannerResolutionRear");

								if (floatValue == -1)
								{
									emit message("<font color=\"#FF0000\">Value \"laserscannerResolutionRear\" not found in ini-file!</font>");
								}
								else
								{
									// everything okay
									laserThread->setResolution(LASER2, floatValue);
									if (consoleMode)
									{
										consoleGui->setLaserscannerResolution(LASER2, floatValue);
									}
									else
									{
										gui->setLaserscannerResolution(LASER2, floatValue);
									}
									emit message(QString("Rear laser scanner resolution set to <b>%1</b>.").arg(floatValue));

									//---------------------------------------------------------------------
									// read next setting
									laserscannerIgnoreAreaStart = inifile1->readSetting("Config", "laserscannerRearIgnoreArea1Start");

									if (laserscannerIgnoreAreaStart == -1)
									{
										emit message("<font color=\"#FF0000\">Value \"laserscannerRearIgnoreArea1Start\"not found in ini-file!</font>");
									}
									else
									{

										//---------------------------------------------------------------------
										// read next setting
										laserscannerIgnoreAreaEnd = inifile1->readSetting("Config", "laserscannerRearIgnoreArea1End");

										if (laserscannerIgnoreAreaEnd == -1)
										{
											emit message("<font color=\"#FF0000\">Value \"laserscannerRearIgnoreArea1End\"not found in ini-file!</font>");
										}
										else
										{

											if (consoleMode)
											{
												/// @todo add this: consoleGui->setLaserscannerAngle(LASER2, laserscanner...Rear);
											}
											else
											{
												/// @todo gui->setLaserscannerAngle(LASER1, laserscanner...Rear);
											}
											emit message(QString("Rear laser scanner ignore area %1 set to <b>%2-%3%1</b>.").arg(AREA1).arg(laserscannerIgnoreAreaStart).arg(laserscannerIgnoreAreaEnd));

											// store settings in obstacle check thread
											obstCheckThread->setIgnoreArea(LASER2, AREA1, laserscannerIgnoreAreaStart, laserscannerIgnoreAreaEnd);
										}
									}

									//---------------------------------------------------------------------
									// read next setting
									laserscannerIgnoreAreaStart = inifile1->readSetting("Config", "laserscannerRearIgnoreArea2Start");

									if (laserscannerIgnoreAreaStart == -1)
									{
										emit message("<font color=\"#FF0000\">Value \"laserscannerRearIgnoreArea2Start\"not found in ini-file!</font>");
									}
									else
									{

										//---------------------------------------------------------------------
										// read next setting
										laserscannerIgnoreAreaEnd = inifile1->readSetting("Config", "laserscannerRearIgnoreArea2End");

										if (laserscannerIgnoreAreaEnd == -1)
										{
											emit message("<font color=\"#FF0000\">Value \"laserscannerRearIgnoreArea2End\"not found in ini-file!</font>");
										}
										else
										{

											if (consoleMode)
											{
												/// @todo add this: consoleGui->setLaserscannerAngle(LASER2, laserscanner...Rear);
											}
											else
											{
												/// @todo gui->setLaserscannerAngle(LASER1, laserscanner...Rear);
											}
											emit message(QString("Rear laser scanner ignore area %1 set to <b>%2-%3%1</b>.").arg(AREA2).arg(laserscannerIgnoreAreaStart).arg(laserscannerIgnoreAreaEnd));

											// store settings in obstacle check thread
											obstCheckThread->setIgnoreArea(LASER2, AREA2, laserscannerIgnoreAreaStart, laserscannerIgnoreAreaEnd);
										}
									}

								}
								break;
							}
						}
					}
				}
			}
		}
	}

#ifdef ACTIVELASERVIEW
	//---------------------------------------------------------------------
	// Create the laser lines and pixmaps in the GUI and place them nicely
	//---------------------------------------------------------------------
	if (!consoleMode)
	{
		gui->initLaserStuff();
	}
#endif

	//---------------------------------------------------------------------
	// read setting
	switch (inifile1->readSetting("Config", "useCamera"))
	{
		case -1:
			emit message("<font color=\"#FF0000\">Value \"useCamera\"not found in ini-file!</font>");
			break;
		case 0:
			useCamera = false;
			if (!consoleMode)
			{
				// turning "off" camera
				camThread->disable();
			}
			emit message("<font color=\"#FF0000\">No camera usage! (Disabled in ini-file)</font>");
			break;
		case 1:
			useCamera = true;
			break;
	}

	if (!consoleMode)
	{
		if (useCamera)
		{
			//---------------------------------------------------------------------
			// read setting
			int cameraDevice = inifile1->readSetting("Config", "cameraDevice");

			if (cameraDevice == -2)
			{
				// turning "off" camera
				camThread->disable();
				emit message("<font color=\"#FF0000\">ini-file is not writeable!</font>");
			}
			else
			{
				if (cameraDevice == -1)
				{
					// turning "off" camera
					camThread->disable();
					emit message("<font color=\"#FF0000\">Value \"cameraDevice\" not found in ini-file!</font>");
				}
				else
				{
					//
					// everything okay
					//
					// set it in the cam thread
//					emit message(QString("Camera file set to <b>%1</b>.").arg(cameraDevice));


					//---------------------------------------------------------------------
					// read setting
					QString haarClassifierCascade = inifile1->readString("Config", "haarClassifierCascade");

					if (haarClassifierCascade == "error2")
					{
						emit message("<font color=\"#FF0000\">ini-file is not writeable!</font>");
					}
					else
					{
						if (haarClassifierCascade == "error1")
						{
							emit message("<font color=\"#FF0000\">Value \"haarClassifierCascade\" not found in ini-file!</font>");
						}
						else
						{
							//
							// everything okay
							//
							// set it in the cam thread
							emit message(QString("Haar classifier cascade file set to<br><b>%1</b>.").arg(haarClassifierCascade));
						}
					}
					//---------------------------------------------------------------------
				}
			}
		} // use camera
	} // no console mode

	//---------------------------------------------------------------------
	// read setting / and error handling
	switch (inifile1->readSetting("Config", "noHardwareErrorMessages"))
	{
		case -1:
			emit message("<font color=\"#FF0000\">Value \"noHardwareErrorMessages\"not found in ini-file!</font>");
			break;
		case 0:
			noHardwareErrorMessages = false;
			break;
		case 1:
			noHardwareErrorMessages = true;
			emit message("<font color=\"#808080\">Suppressing hardware error messages (see ini-file)</font>");
			break;
	}

	//---------------------------------------------------------------------
	// read setting
	switch (inifile1->readSetting("Config", "saveOnExit"))
	{
		case -1:
			emit message("<font color=\"#FF0000\">Value \"saveOnExit\"not found in ini-file!</font>");
			break;
		case Qt::Unchecked:
			if (!consoleMode)
			{
				// uncheck checkbox
				settingsDialog->setCheckBoxSaveSettings(Qt::Unchecked);
			}
			break;
		case Qt::Checked:
			if (!consoleMode)
			{
				// set checkbox
				settingsDialog->setCheckBoxSaveSettings(Qt::Checked);
			}
			break;
	}

	//---------------------------------------------------------------------
	// read setting
	switch (inifile1->readSetting("Config", "exitDialog"))
	{
		case -1:
			emit message("<font color=\"#FF0000\">Value \"exitDialog\"not found in ini-file!</font>");
			break;
		case 0:
			exitDialog = false;
			break;
		case 1:
			exitDialog = true;
			break;
	}

	//---------------------------------------------------------------------
	// read setting
	int minObstacleDistance = inifile1->readSetting("Config", "minObstacleDistance");

	switch (minObstacleDistance)
	{
		case -1:
			emit message("<font color=\"#FF0000\">Value \"minObstacleDistance\"not found in ini-file!</font>");
			break;
		default:
			if (!consoleMode)
			{
				// set slider to the read value
				settingsDialog->setSliderObstacleValue(minObstacleDistance);
			}

			// tell the  obstacle check thread the distance
			obstCheckThread->setMinObstacleDistance(minObstacleDistance);
			// show text
			emit message(QString("Min. obstacle distance set to <b>%1 cm</b>.").arg(minObstacleDistance));
			break;
	}

	//---------------------------------------------------------------------
	// read setting
	int minObstacleDistanceLaserScannerFront = inifile1->readSetting("Config", "minObstacleDistanceLaserScannerFront");

	switch (minObstacleDistanceLaserScannerFront)
	{
		case -1:
			emit message("<font color=\"#FF0000\">Value \"minObstacleDistanceLaserScannerFront\"not found in ini-file!</font>");
			break;
		default:
			if (!consoleMode)
			{
				// set slider to the read value
				settingsDialog->setSliderObstacleLaserScannerValue(minObstacleDistanceLaserScannerFront);
			}

			// tell it the obstacle check thread
			obstCheckThread->setMinObstacleDistanceLaserFront(minObstacleDistanceLaserScannerFront);
			// show text
			emit message(QString("Min. obstacle distance front laser scanner set to <b>%1 cm</b>.").arg(minObstacleDistanceLaserScannerFront));
			break;
	}

	//---------------------------------------------------------------------
	// read setting
	int minObstacleDistanceLaserScannerRear = inifile1->readSetting("Config", "minObstacleDistanceLaserScannerRear");

	switch (minObstacleDistanceLaserScannerRear)
	{
		case -1:
			emit message("<font color=\"#FF0000\">Value \"minObstacleDistanceLaserScannerRear\"not found in ini-file!</font>");
			break;
		default:
			if (!consoleMode)
			{
				// set slider to the read value
// @todo				settingsDialog->setSliderObstacleLaserScannerValue(minObstacleDistanceLaserScannerRear);
			}

			// tell it the obstacle check thread
			obstCheckThread->setMinObstacleDistanceLaserRear(minObstacleDistanceLaserScannerRear);
			// show text
			emit message(QString("Min. obstacle distance rear laser scanner set to <b>%1 cm</b>.").arg(minObstacleDistanceLaserScannerRear));
			break;
	}

	//---------------------------------------------------------------------
	// read setting
	int passageWidth = inifile1->readSetting("Config", "robotPassageWidth");

	switch (passageWidth)
	{
		case -1:
			emit message("<font color=\"#FF0000\">Value \"robotPassageWidth\"not found in ini-file!</font>");
			break;
		default:
			if (!consoleMode)
			{
				// set slider to the read value
				settingsDialog->setSliderPassageWidth(passageWidth);
			}

			// tell it the obstacle check thread
			obstCheckThread->setPassageWidth(passageWidth);
			// show text
			emit message(QString("Robot passage width width set to <b>%1 cm.</b>").arg(passageWidth));
			break;
	}

	//---------------------------------------------------------------------
	// read setting
	int straightForwardDeviation = inifile1->readSetting("Config", "straightForwardDeviation");

	switch (straightForwardDeviation)
	{
		case -1:
			emit message("<font color=\"#FF0000\">Value \"straightForwardDeviation\"not found in ini-file!</font>");
			break;
		default:
			if (!consoleMode)
			{
				// set slider to the read value
				settingsDialog->setSliderStraightForwardDeviation(straightForwardDeviation);
			}

			// tell it the obstacle check thread
			obstCheckThread->setStraightForwardDeviation(straightForwardDeviation);
			// show text
			emit message(QString("Straight forward deviation set to <b>%1 deg.</b>").arg(straightForwardDeviation));
			break;
	}

	//---------------------------------------------------------------------
	// read setting
	QString joystickPort = inifile1->readString("Config", "joystickPort");

	if (joystickPort == "error2")
	{
		emit message("<font color=\"#FF0000\">ini-file is not writeable!</font>");
	}
	else
	{
		if (joystickPort == "error1")
		{
			emit message("<font color=\"#FF0000\">Value \"joystickPort\" not found in ini-file!</font>");
		}
		else
		{
			//
			// everything okay
			//
			// tell the  obstacle check thread the distance
			joystick->setPort(joystickPort);
			emit message(QString("Joystick port set to <b>%1</b>.").arg(joystickPort));
		}
	}

	//---------------------------------------------------------------------
	// read setting
	mot1Speed = inifile1->readSetting("Config", "motor1Speed");

	switch (mot1Speed)
	{
		case -1:
			emit message("<font color=\"#FF0000\">Value \"motor1Speed\" not found in ini-file!</font>");
			mot1Speed = 0;
			break;
		default:
			if (mot1Speed > MAXPWM)
			{
				emit message(QString("<font color=\"#FF0000\">Value \"motor1Speed\" is greater than %1!! Value set to %1!</font>").arg(MAXPWM));
				mot1Speed = MAXPWM;
			}

			if (!consoleMode)
			{
				// set slider to the read value
				settingsDialog->setSliderMotorSpeed(1, mot1Speed);
			}

			// show text
			emit message(QString("Motor1 speed set to <b>%1</b>.").arg(mot1Speed));
			break;
	}

	//---------------------------------------------------------------------
	// read setting
	mot2Speed = inifile1->readSetting("Config", "motor2Speed");

	switch (mot2Speed)
	{
		case -1:
			emit message("<font color=\"#FF0000\">Value \"motor2Speed\" not found in ini-file!</font>");
			mot2Speed = 0;
			break;
		default:
			if (mot2Speed > MAXPWM)
			{
				emit message(QString("<font color=\"#FF0000\">Value \"motor2Speed\" is greater than %1!! Value set to %1!</font>").arg(MAXPWM));
				mot2Speed = MAXPWM;
			}

			if (!consoleMode)
			{
				// set slider to the read value
				settingsDialog->setSliderMotorSpeed(2, mot2Speed);
			}

			// show text
			emit message(QString("Motor2 speed set to <b>%1</b>.").arg(mot2Speed));
			break;
	}

	//---------------------------------------------------------------------
	// read setting
	mot3Speed = inifile1->readSetting("Config", "motor3Speed");

	switch (mot3Speed)
	{
		case -1:
			emit message("<font color=\"#FF0000\">Value \"motor3Speed\" not found in ini-file!</font>");
			mot3Speed = 0;
			break;
		default:
			if (mot3Speed > MAXPWM)
			{
				emit message(QString("<font color=\"#FF0000\">Value \"motor3Speed\" is greater than %1!! Value set to %1!</font>").arg(MAXPWM));
				mot3Speed = MAXPWM;
			}

			if (!consoleMode)
			{
				// set slider to the read value
				settingsDialog->setSliderMotorSpeed(3, mot3Speed);
			}

			// show text
			emit message(QString("Motor3 speed set to <b>%1</b>.").arg(mot3Speed));
			break;
	}

	//---------------------------------------------------------------------
	// read setting
	mot4Speed = inifile1->readSetting("Config", "motor4Speed");

	switch (mot4Speed)
	{
		case -1:
			emit message("<font color=\"#FF0000\">Value \"motor4Speed\" not found in ini-file!</font>");
			mot4Speed = 0;
			break;
		default:
			if (mot4Speed > MAXPWM)
			{
				emit message(QString("<font color=\"#FF0000\">Value \"motor1Speed\" is greater than %1!! Value set to %1!</font>").arg(MAXPWM));
				mot4Speed = MAXPWM;
			}

			if (!consoleMode)
			{
				// set slider to the read value
				settingsDialog->setSliderMotorSpeed(4, mot4Speed);
			}

			// show text
			emit message(QString("Motor4 speed set to <b>%1</b>.").arg(mot4Speed));
			break;
	}

	//---------------------------------------------------------------------
	// read setting
	minimumSpeed = inifile1->readSetting("Config", "minimumSpeed");

	switch (minimumSpeed)
	{
		case -1:
			emit message("<font color=\"#FF0000\">Value \"minimumSpeed\" not found in ini-file!</font>");
			minimumSpeed = 0;
			break;
		default:
			if (minimumSpeed > MAXPWM)
			{
				emit message(QString("<font color=\"#FF0000\">Value \"minimumSpeed\" is greater than %1!! Value set to %1!</font>").arg(MAXPWM));
				minimumSpeed = MAXPWM;
			}

			if (!consoleMode)
			{
				// set slider to the read value
				settingsDialog->setSliderMinimumSpeed(minimumSpeed);
			}

			// show text
			emit message(QString("Minimum speed speed set to <b>%1</b>.").arg(minimumSpeed));
			break;
	}

	//---------------------------------------------------------------------
	// read setting
	maximumSpeed = inifile1->readSetting("Config", "maximumSpeed");

	switch (maximumSpeed)
	{
		case -1:
			emit message("<font color=\"#FF0000\">Value \"maximumSpeed\" not found in ini-file!</font>");
			maximumSpeed = 0;
			break;
		default:
			if (maximumSpeed > MAXPWM)
			{
				emit message(QString("<font color=\"#FF0000\">Value \"maximumSpeed\" is greater than %1!! Value set to %1!</font>").arg(MAXPWM));
				maximumSpeed = MAXPWM;
			}

			if (!consoleMode)
			{
				// set slider to the read value
				settingsDialog->setSliderMaximumSpeed(maximumSpeed);
			}

			// show text
			emit message(QString("Maximum speed speed set to <b>%1</b>.").arg(maximumSpeed));
			break;
	}

	//---------------------------------------------------------------------
	// read servo START settings
	for (int servo=0; servo<NUMBEROFSERVOS; servo++)
	{
		QString settingName = QString("servo%1").arg(servo+1).append("StartPos");
		int settingValue = inifile1->readSetting("Config", settingName);

		switch (settingValue)
		{
			case -1:
				emit message(QString("<font color=\"#FF0000\">Value \"%1\" not found in ini-file!</font>").arg(settingName));
				settingValue = 0;
				break;
			default:
				if (settingValue > 254)
				{
					emit message(QString("<font color=\"#FF0000\">Value \"%1\" is greater than 255!! Value set to 255!</font>").arg(settingName));
					settingValue = 255;
				}

				// store the servo values
				servos->setServoPosition(servo, SVSTART, settingValue);

				// show text
				//emit message(QString("%1 set to <b>%2</b>.").arg(settingName).arg(settingValue));
				break;
		}
	}
	emit message("Servo start settings read and set.");

	//---------------------------------------------------------------------
	// read servo END settings
	for (int servo=0; servo<NUMBEROFSERVOS; servo++)
	{
		QString settingName = QString("servo%1").arg(servo+1).append("EndPos");
		int settingValue = inifile1->readSetting("Config", settingName);

		switch (settingValue)
		{
			case -1:
				emit message(QString("<font color=\"#FF0000\">Value \"%1\" not found in ini-file!</font>").arg(settingName));
				settingValue = 0;
				break;
			default:
				if (settingValue > 254)
				{
					emit message(QString("<font color=\"#FF0000\">Value \"%1\" is greater than 255!! Value set to 255!</font>").arg(settingName));
					settingValue = 255;
				}

				// store the servo values
				servos->setServoPosition(servo, SVEND, settingValue);

				// show text
				//emit message(QString("%1 set to <b>%2</b>.").arg(settingName).arg(settingValue));
				break;
		}
	}
	emit message("Servo end settings read and set.");

	//---------------------------------------------------------------------
	// read servo MIN settings
	for (int servo=0; servo<NUMBEROFSERVOS; servo++)
	{
		QString settingName = QString("servo%1").arg(servo+1).append("MinPos");
		int settingValue = inifile1->readSetting("Config", settingName);

		switch (settingValue)
		{
			case -1:
				emit message(QString("<font color=\"#FF0000\">Value \"%1\" not found in ini-file!</font>").arg(settingName));
				settingValue = 0;
				break;
			default:
				if (settingValue > 254)
				{
					emit message(QString("<font color=\"#FF0000\">Value \"%1\" is greater than 255!! Value set to 255!</font>").arg(settingName));
					settingValue = 255;
				}

				// store the servo values
				servos->setServoPosition(servo, SVMIN, settingValue);

				// show text
				//emit message(QString("%1 set to <b>%2</b>.").arg(settingName).arg(settingValue));
				break;
		}
	}
	emit message("Servo min. settings read and set.");

	//---------------------------------------------------------------------
	// read servo MAX settings
	for (int servo=0; servo<NUMBEROFSERVOS; servo++)
	{
		QString settingName = QString("servo%1").arg(servo+1).append("MaxPos");
		int settingValue = inifile1->readSetting("Config", settingName);

		switch (settingValue)
		{
			case -1:
				emit message(QString("<font color=\"#FF0000\">Value \"%1\" not found in ini-file!</font>").arg(settingName));
				settingValue = 0;
				break;
			default:
				if (settingValue > 254)
				{
					emit message(QString("<font color=\"#FF0000\">Value \"%1\" is greater than 255!! Value set to 255!</font>").arg(settingName));
					settingValue = 255;
				}

				// store the servo values
				servos->setServoPosition(servo, SVMAX, settingValue);

				// show text
				//emit message(QString("%1 set to <b>%2</b>.").arg(settingName).arg(settingValue));
				break;
		}
	}
	emit message("Servo max. settings read and set.");

	//---------------------------------------------------------------------
	// read servo DEFAULT settings
	for (int servo=0; servo<NUMBEROFSERVOS; servo++)
	{
		QString settingName = QString("servo%1").arg(servo+1).append("DefaultPos");
		int settingValue = inifile1->readSetting("Config", settingName);

		switch (settingValue)
		{
			case -1:
				emit message(QString("<font color=\"#FF0000\">Value \"%1\" not found in ini-file!</font>").arg(settingName));
				settingValue = 0;
				break;
			default:
				if (settingValue > 254)
				{
					emit message(QString("<font color=\"#FF0000\">Value \"%1\" is greater than 255!! Value set to 255!</font>").arg(settingName));
					settingValue = 255;
				}

				// store the servo values
				servos->setServoPosition(servo, SVDEFAULT, settingValue);

				// show text
				//emit message(QString("%1 set to <b>%2</b>.").arg(settingName).arg(settingValue));
				break;
		}
	}
	emit message("Servo default settings read and set.");



	//---------------------------------------------------------------------
	// read RGB LED MIN brightness
	for (int led=0; led<NUMBEROFRGBLEDS; led++)
	{
		QString settingName = QString("RGBLED%1").arg(led+1).append("MinBrightness");
		int settingValue = inifile1->readSetting("Config", settingName);

		switch (settingValue)
		{
			case -1:
				emit message(QString("<font color=\"#FF0000\">Value \"%1\" not found in ini-file!</font>").arg(settingName));
				settingValue = 0;
				break;
			default:
				if (settingValue > MAXPWM)
				{
					emit message(QString("<font color=\"#FF0000\">Value \"%1\" is greater than %2!! Value set to %2!</font>").arg(settingName).arg(MAXPWM));
					settingValue = MAXPWM;
				}

				// store the LED values
				rgbLeds->storeBrightness(led, RGBLEDMIN, settingValue);

				// show text
				emit message(QString("%1 set to <b>%2</b>.").arg(settingName).arg(settingValue));
				break;
		}
	}
	emit message("RGB LED min. settings read and set.");

	//---------------------------------------------------------------------
	// read RGB LED MAX brightness
	for (int led=0; led<NUMBEROFRGBLEDS; led++)
	{
		QString settingName = QString("RGBLED%1").arg(led+1).append("MaxBrightness");
		int settingValue = inifile1->readSetting("Config", settingName);

		switch (settingValue)
		{
			case -1:
				emit message(QString("<font color=\"#FF0000\">Value \"%1\" not found in ini-file!</font>").arg(settingName));
				settingValue = 0;
				break;
			default:
				if (settingValue > MAXPWM)
				{
					emit message(QString("<font color=\"#FF0000\">Value \"%1\" is greater than %2!! Value set to %2!</font>").arg(settingName).arg(MAXPWM));
					settingValue = MAXPWM;
				}

				// store the LED values
				rgbLeds->storeBrightness(led, RGBLEDMAX, settingValue);

				// show text
				emit message(QString("%1 set to <b>%2</b>.").arg(settingName).arg(settingValue));
				break;
		}
	}
	emit message("RGB LED max. settings read and set.");

	//---------------------------------------------------------------------
	// read RGB LED DEFAULT brightness
	for (int led=0; led<NUMBEROFRGBLEDS; led++)
	{
		QString settingName = QString("RGBLED%1").arg(led+1).append("DefaultBrightness");
		int settingValue = inifile1->readSetting("Config", settingName);

		switch (settingValue)
		{
			case -1:
				emit message(QString("<font color=\"#FF0000\">Value \"%1\" not found in ini-file!</font>").arg(settingName));
				settingValue = 0;
				break;
			default:
				if (settingValue > MAXPWM)
				{
					emit message(QString("<font color=\"#FF0000\">Value \"%1\" is greater than %2!! Value set to %2!</font>").arg(settingName).arg(MAXPWM));
					settingValue = MAXPWM;
				}

				// store the LED values
				rgbLeds->storeBrightness(led, RGBLEDDEFAULT, settingValue);

				// show text
				emit message(QString("%1 set to <b>%2</b>.").arg(settingName).arg(settingValue));
				break;
		}
	}
	emit message("RGB LED default settings read and set.");


	//---------------------------------------------------------------------
	// read setting
	networkPortListen = inifile1->readSetting("Config", "networkPortListen");

	switch (networkPortListen)
	{
		case -1:
			emit message("<font color=\"#FF0000\">Value \"networkPortListen\" not found in ini-file!</font>");
			value = 0;
			break;

		default:
			//---------------------------------------------------------------------
			// read next network port
			networkPortSend = inifile1->readSetting("Config", "networkPortSend");

			switch (networkPortSend)
			{
				case -1:
					emit message("<font color=\"#FF0000\">Value \"networkPortSend\" not found in ini-file!</font>");
					networkPortListen = 0;
					break;

				default:
					// set value in networkThread, bind port
					if (netThread->init(networkPortListen, networkPortSend) == true)
					{
						emit message(QString("Network ports set to <b>%1 and %2</b>.").arg(networkPortListen).arg(networkPortSend));
						if (!consoleMode)
						{
							gui->setLEDNetwork(GREEN);
						}
					}
					else
					{
						// Error
						emit message(QString("<font color=\"#FF0000\">Error setting network ports to <b>%1 and %2</b>.</font>").arg(value).arg(networkPortSend));
						if (!consoleMode)
						{
							gui->setLEDNetwork(RED);
						}
					}

					break;
			}
			break;
	}

	//---------------------------------------------------------------------
	// read setting
	value = inifile1->readSetting("Config", "threshold");

	switch (value)
	{
		case -1:
			emit message("<font color=\"#FF0000\">Value \"threshold\" not found in ini-file!</font>");
			value = 0;
			break;
		default:
			if (!consoleMode)
			{
				// set value in camThread and GUI
//				camThread->setThreshold(value);
				gui->showThreshold(value);
				emit message(QString("Setting threshold to <b>%1</b>.").arg(value));
			}
			break;
	}
}


void Direcs::enableRemoteControlListening(bool state) /// @todo check if we still need the enableRemoteControlListening method
{
	// store the state gobal to Direcs
	robotRemoteMode = state;

	if (state == true)
	{
		//-----------------------------------------------------------
		// start the network thread (getting commands via network)
		//-----------------------------------------------------------
		if (netThread->isRunning() == false)
		{
			emit message("Starting network thread... ", false);
			if (!consoleMode)
			{
				gui->appendNetworkLog("Starting network thread... ", false);
			}
			netThread->start();
			emit message("started.", true, false, false);
			if (!consoleMode)
			{
				gui->appendNetworkLog("started");
			}
		}
	}
	else
	{
		if (netThread->isRunning() == true)
		{
			emit message("Stopping network thread...", false);
			if (!consoleMode)
			{
				gui->appendNetworkLog("Stopping network thread...");
			}
			netThread->stop();
			emit message("Network thread stopped.");
		}
	}
}


void Direcs::setNetworkState()
{
	// if still no data received (in executeRemoteCommand()
	if (firstDataReceived == false)
	{
		// from now on, we're no longer waiting for data (looking for a 'master
		firstDataReceived = true;

		// 'master' *not* received, so we are master now!
		iAmTheMaster = true;

		// show state in GUI
		gui->setLabelMasterSlave("Master");

		gui->appendNetworkLog("No network data received.");
		gui->appendNetworkLog("I am the master.");

		// we swap the ports back (to initial values) after listening for a network master
		netThread->swapPorts();
	}
}


void Direcs::executeRemoteCommand(QString command)
{
	// if still no first and one time check if this is the master or the slave program
	if (firstDataReceived == false)
	{
		// is there already a master, sending its data over network?
		if (command.contains("*master#")) /// @todo add *master# string to ini-file
		{
			firstDataReceived = true;

			// 'master' received, so we are slave now!
			iAmTheMaster = false;

			// show state in GUI
			gui->setLabelMasterSlave("Slave");

			gui->appendNetworkLog(QString("%1 received.").arg(command));
			gui->appendNetworkLog("I am the slave now.");

			// let the networkThread know this
			netThread->setNetworkSlave();
		}
		else
		{
			firstDataReceived = true;

			// 'master' *not* received, so we are master now!
			iAmTheMaster = true;

			// show state in GUI
			gui->setLabelMasterSlave("Master");

			gui->appendNetworkLog(QString("%1 received.").arg(command));
			gui->appendNetworkLog("I am the master.");

			// we swap the ports back (to initial values) after listening for a network master
			netThread->swapPorts();
		}
	}


	// only react, when remote mode is activated in the GUI!
	if (robotRemoteMode==true)
	{
		if (command == "start")
		{
			emit message(tr("<font color=\"#0000FF\">Executing remote command \"%1\".</font>").arg(command));
			drive(START);
			return;
		}


		if (command == "forward")
		{
			emit message(tr("<font color=\"#0000FF\">Executing remote command \"%1\".</font>").arg(command));
			drive(FORWARD);
			return;
		}


		if (command == "backward")
		{
			emit message(tr("<font color=\"#0000FF\">Executing remote command \"%1\".</font>").arg(command));
			drive(BACKWARD);
			return;
		}


		if (command == "stop")
		{
			emit message(tr("<font color=\"#0000FF\">Executing remote command \"%1\".</font>").arg(command));
			drive(STOP);
			return;
		}


		if (command == "left")
		{
			emit message(tr("<font color=\"#0000FF\">Executing remote command \"%1\".</font>").arg(command));
			drive(LEFT);
			return;
		}


		if (command == "right")
		{
			emit message(tr("<font color=\"#0000FF\">Executing remote command \"%1\".</font>").arg(command));
			drive(RIGHT);
			return;
		}


		if (command == "increasespeedmotor1")
		{
			emit message(tr("<font color=\"#0000FF\">Executing remote command \"%1\".</font>").arg(command));

			int newSpeed = motors->getMotorSpeed(1) + 1;
			motors->setMotorSpeed(1, newSpeed);

			if (!consoleMode)
			{
				settingsDialog->setSliderMotorSpeed(1, newSpeed);
			}

			return;
		}


		if (command == "increasespeedmotor2")
		{
			emit message(tr("<font color=\"#0000FF\">Executing remote command \"%1\".</font>").arg(command));

			int newSpeed = motors->getMotorSpeed(2) + 1;
			motors->setMotorSpeed(2, newSpeed);

			if (!consoleMode)
			{
				settingsDialog->setSliderMotorSpeed(2, newSpeed);
			}

			return;
		}


		if (command == "reducespeedmotor1")
		{
			emit message(tr("<font color=\"#0000FF\">Executing remote command \"%1\".</font>").arg(command));

			int newSpeed = motors->getMotorSpeed(1) - 1;
			motors->setMotorSpeed(1, newSpeed);

			if (!consoleMode)
			{
				settingsDialog->setSliderMotorSpeed(1, newSpeed);
			}

			return;
		}


		if (command == "reducespeedmotor2")
		{
			emit message(tr("<font color=\"#0000FF\">Executing remote command \"%1\".</font>").arg(command));

			int newSpeed = motors->getMotorSpeed(2) - 1;
			motors->setMotorSpeed(2, newSpeed);

			if (!consoleMode)
			{
				settingsDialog->setSliderMotorSpeed(2, newSpeed);
			}

			return;
		}

		if (command == "shutdown")
		{
			emit message(tr("<font color=\"#0000FF\">Executing remote command \"%1\".</font>").arg(command));
			emit shutdown();
			return;
		}
	}
}


void Direcs::executeJoystickCommand(int axisNumber, int axisValue)
{
	static unsigned char servo1Pos = servos->getServoPosition(SERVO1);

	//
	// Y axis
	//
	if (axisNumber == JOYSTICKAXISY2)
	{
		//------
		// down
		//------
		if (axisValue > 0)
		{
			//=========================================================
			// robot test drive mode
			// only drive, when activated and controlled via joystick!
			//=========================================================
			if (testDriveMode)
			{
				//
				// DRIVE backward
				//

				if (!consoleMode)
				{
					//speed = (axisValue / JOYSTICKDIVISOR);
					settingsDialog->setSliderMotorSpeed( 1, (axisValue / JOYSTICKDIVISOR) );
					settingsDialog->setSliderMotorSpeed( 2, (axisValue / JOYSTICKDIVISOR) );
				}

//				if (robotIsOn)
//				{
					motors->setMotorSpeed( 1, (axisValue / JOYSTICKDIVISOR) );
					motors->setMotorSpeed( 2, (axisValue / JOYSTICKDIVISOR) );

					if (robotDrives == false)
					{
						/// \todo check if this makes sense...
						// drive(START);
					}

					drive(BACKWARD);
//				}
			}


			// ###############################################################################
			// disabled since the head is not in use use!
			// ###############################################################################
			//==================
			// eye test mode
			//==================
			if (eyeTestMode==true)
			{
				// eyes down
				servos->setServoPosition( SERVO2, SVCURRENT, (axisValue / JOYSTICKDIVISOR) );
				servos->setServoPosition( SERVO5, SVCURRENT, (axisValue / JOYSTICKDIVISOR) );

				servos->moveServo(SERVO2, servos->getServoPosition(SERVO2));
				servos->moveServo(SERVO5, servos->getServoPosition(SERVO5));
			}

			return;
		}

		//----
		// up
		//----
		if (axisValue < 0)
		{
			//=========================================================
			// robot test drive mode
			// only drive, when activated and controlled via joystick!
			//=========================================================
			if (testDriveMode)
			{
				//
				// DRIVE forward
				//
				if (!consoleMode)
				{
					//speed = (-axisValue / JOYSTICKDIVISOR);
					settingsDialog->setSliderMotorSpeed( 1, (-axisValue / JOYSTICKDIVISOR) );
					settingsDialog->setSliderMotorSpeed( 2, (-axisValue / JOYSTICKDIVISOR) );
				}

				motors->setMotorSpeed( 1, (-axisValue / JOYSTICKDIVISOR) );
				motors->setMotorSpeed( 2, (-axisValue / JOYSTICKDIVISOR) );

				if (robotDrives == false)
				{
					/// \todo check if this makes sense...
					// drive(START);
				}

				drive(FORWARD);
			}


			// ###############################################################################
			// disabled since the head is not in use use!
			// ###############################################################################
			//==================
			// eye test mode
			//==================
			if (eyeTestMode==true)
			{
				// eyes down
				// left eye
				servos->setServoPosition( SERVO2, SVCURRENT, (-axisValue / JOYSTICKDIVISOR) );
				// right eye
				servos->setServoPosition( SERVO5, SVCURRENT, (-axisValue / JOYSTICKDIVISOR) );

				servos->moveServo(SERVO2, servos->getServoPosition(SERVO2));
				servos->moveServo(SERVO5, servos->getServoPosition(SERVO5));
			}

			return;
		}

		//------------------
		// STOP
		//------------------
		if (axisValue == 0)
		{
			//=========================================================
			// robot test drive mode
			// only drive, when activated and controlled via joystick!
			//=========================================================
			if (testDriveMode)
			{
				drive(WAIT);
			}
			return;
		}
	}


	//
	// X axis
	//
	if (axisNumber == JOYSTICKAXISX3)
	{
		//-------
		// right
		//-------
		if (axisValue > 0)
		{
			//=========================================================
			// robot test drive mode
			// only drive, when activated and controlled via joystick!
			//=========================================================
			if (testDriveMode)
			{
				//
				// DRIVE RIGHT
				//
				if (!consoleMode)
				{
					//speed = (axisValue / JOYSTICKDIVISOR);
					settingsDialog->setSliderMotorSpeed( 1, (axisValue / JOYSTICKDIVISOR) );
					settingsDialog->setSliderMotorSpeed( 2, (axisValue / JOYSTICKDIVISOR) );
				}

				motors->setMotorSpeed( 1, (axisValue / JOYSTICKDIVISOR) );
				motors->setMotorSpeed( 2, (axisValue / JOYSTICKDIVISOR) );

				if (robotDrives == false)
				{
					/// \todo check if this makes sense...
					// drive(START);
				}

				drive(RIGHT);
			}
			return;
		}

		//------
		// left
		//------
		if (axisValue < 0)
		{
			//=========================================================
			// robot test drive mode
			// only drive, when activated and controlled via joystick!
			//=========================================================
			if (testDriveMode)
			{
				//
				// DRIVE left
				//
				if (!consoleMode)
				{
					//speed = (-axisValue / JOYSTICKDIVISOR);
					settingsDialog->setSliderMotorSpeed( 1, (-axisValue / JOYSTICKDIVISOR) );
					settingsDialog->setSliderMotorSpeed( 2, (-axisValue / JOYSTICKDIVISOR) );
				}

				motors->setMotorSpeed( 1, (-axisValue / JOYSTICKDIVISOR) );
				motors->setMotorSpeed( 2, (-axisValue / JOYSTICKDIVISOR) );

				if (robotDrives == false)
				{
					/// \todo check if this makes sense...
					// drive(START);
				}

				drive(LEFT);
			}
			return;
		}

		//------------------
		// STOP
		//------------------
		if (axisValue == 0)
		{
			//=========================================================
			// robot test drive mode
			// only drive, when activated and controlled via joystick!
			//=========================================================
			if (testDriveMode)
			{
				drive(WAIT);
			}
			return;
		}
	}


	//
	// Y axis "buttons"
	//
	if (axisNumber == JOYSTICKAXISY5)
	{
		//==================
		// servo test mode
		//==================
		if (servoTestMode==true)
		{
			//------------------
			// servo down (servo2)
			//------------------
			if (axisValue > 0)
			{
				// --
				int wert = servos->getServoPosition(currentTestServo);
				servos->setServoPosition( currentTestServo, SVCURRENT, wert-1 );
			}

			//------------------
			// servo up (servo2)
			//------------------
			if (axisValue < 0)
			{
				// ++
				int wert = servos->getServoPosition(currentTestServo);
				servos->setServoPosition( currentTestServo, SVCURRENT, wert+1 );
				//servos->setServoPosition( currentTestServo, SVCURRENT, (servos->getServoPosition(currentTestServo))+1 );
			}

			// only move, when button is pressed - not, when released (=0)
			if (axisValue != 0)
			{
				servos->moveServo(currentTestServo, servos->getServoPosition(currentTestServo));
				emit message(QString("Servo %1 moved to %2.").arg(currentTestServo+1).arg(servos->getServoPosition(currentTestServo)));
			}

			return;
		}


/*
		// ###############################################################################
		// disabled since this would use the motor 3 and 4 which are now in different use!
		// ###############################################################################
		//==================
		// camera test mode
		//==================
		if (cameraTestMode==true)
		{
			//------------------
			// camera up [tilt]
			//------------------
			if (axisValue > 0)
			{
				motors->motorControl(MOTOR4, ON, CLOCKWISE);
				//emit message("motor 4 on CW");
			}

			//------------------
			// camera down [tilt]
			//------------------
			if (axisValue < 0)
			{
				motors->motorControl(MOTOR4, ON, COUNTERCLOCKWISE);
				//emit message("motor 4 on CCW");
			}

			// move, when button is pressed
			if (axisValue != 0)
			{
				//emit message("Tilting Cam...");
			}

			// stop, when button is pressed!
			if (axisValue == 0)
			{
				//emit message("Tilt stop.");
				motors->motorControl(MOTOR4, OFF, SAME);
			}
			return;
		} // cam test mode [tilt]
*/

/*
		// ###############################################################################
		// disabled since the head is not in use use!
		// ###############################################################################
		//==================
		// eye test mode
		//==================
		if (eyeTestMode==true)
		{
			//------------------
			// eyes down
			//------------------
			if (axisValue > 0)
			{
				head->look("DOWN");
				head->look("CURIOUS");
			}

			//------------------
			// eyes up
			//------------------
			if (axisValue < 0)
			{
				head->look("UP");
				head->look("ANGRY");
			}

			if (axisValue == 0)
			{
			}
		}
*/

		//==================
		// drive test mode
		//==================
		//------------------------------------------------------
		// drive backward (Y5 axis down/button down)
		//------------------------------------------------------
		if (axisValue > 0)
		{
			if (robotDrives == false)
			{
				/// \todo check if this makes sense...
				// drive(START);
			}

			drive(BACKWARD);
		}

		//------------------------------------------------------
		// drive forward (Y5 axis/button up)
		//------------------------------------------------------
		if (axisValue < 0)
		{
			if (robotDrives == false)
			{
				/// \todo check if this makes sense...
				// drive(START);
			}

			drive(FORWARD);
		}

		if (axisValue == 0)
		{
			drive(WAIT);
		}

		return;
	}


	//
	// X axis "buttons"
	//
	if (axisNumber == JOYSTICKAXISX4)
	{
		//==================
		// servo test mode
		//==================
		if (servoTestMode==true)
		{
			//------------------
			// servo right
			//------------------
			if (axisValue > 0)
			{
				servo1Pos++;
			}

			//------------------
			// servo left
			//------------------
			if (axisValue < 0)
			{
				servo1Pos--;
			}

			// only move, when button is pressed - not, when released (=0)
			if (axisValue != 0)
			{
				servos->moveServo(SERVO1, servo1Pos);
				emit message(QString("Servo 1 moved to %1.").arg(servo1Pos));
			}
			return;
		} // servo test mode

/*
		// ###############################################################################
		// disabled since this would use the motor 3 and 4 which are now in different use!
		// ###############################################################################
		//==================
		// camera test mode
		//==================
		if (cameraTestMode==true)
		{
			//------------------
			// camera right [pan]
			//------------------
			if (axisValue > 0)
			{
				motors->motorControl(MOTOR3, ON, CLOCKWISE);
				//emit message("motor 3 on CW");
			}

			//------------------
			// camera left [pan]
			//------------------
			if (axisValue < 0)
			{
				motors->motorControl(MOTOR3, ON, COUNTERCLOCKWISE);
				//emit message("motor 3 on CCW");
			}

			// move, when button is pressed
			if (axisValue != 0)
			{
				//emit message("Panning Cam...");
			}

			// stop, when button is pressed!
			if (axisValue == 0)
			{
				motors->motorControl(MOTOR3, OFF, SAME);
				//emit message("Pan stop.");
			}
			return;
		} // cam test mode [pan]
*/

/*
		// ###############################################################################
		// disabled since the head is not in use use!
		// ###############################################################################
		//==================
		// eye test mode
		//==================
		if (eyeTestMode==true)
		{
			//------------------
			// eyes right
			//------------------
			if (axisValue > 0)
			{
				head->look("RIGHT");
			}

			//------------------
			// eyes left
			//------------------
			if (axisValue < 0)
			{
				head->look("LEFT");
			}

			if (axisValue == 0)
			{
			}
		}
*/

		//==================
		// drive test mode
		//==================
		//------------------------------------------------------
		// drive right (X5 axis/button right)
		//------------------------------------------------------
		if (axisValue > 0)
		{
			if (mecanumDriveMode)
			{
				// moving to the right, without turning
				drive(RIGHT);
			}
			else
			{
				if (robotDrives == false)
				{
					/// \todo check if this makes sense...
					// drive(START);
				}

				// do a right turn in a circle!
				drive(TURNRIGHT);
			}
			return;
		}

		//------------------------------------------------------
		// drive left (X5 axis/button left)
		//------------------------------------------------------
		if (axisValue < 0)
		{
			if (mecanumDriveMode)
			{
				// moving to the right, without turning
				drive(LEFT);
			}
			else
			{
				if (robotDrives == false)
				{
					/// \todo check if this makes sense...
					// drive(START);
				}

				// do a left turn in a circle!
				drive(TURNLEFT);
			}
			return;
		}

		//------------------------------------------------------
		// wait (X5 axis/button)
		//------------------------------------------------------
		if (axisValue == 0)
		{
			drive(WAIT);
		}

		return;
	}
}


void Direcs::executeJoystickCommand(int buttonNumber, bool buttonState)
{
	/// @sa Gui::showJoystickButtons
	static bool toggle0 = false;
	static bool toggle1 = false;
	//static bool toggle2 = false;
	//static bool toggle3 = false;
	//static bool toggle4 = false;
	//static bool toggle5 = false;
	static bool toggle10 = false;
	static bool toggle11 = false;


	/// \todo put button numbers to ini-file
	switch (buttonNumber)
	{
		case 0: // 1 on js
			if (buttonState==true)
			{
				if (toggle0 == false)
				{
					// ###############################################################################
					// disabled since the head is not in use use!
					// ###############################################################################
//					eyeTestMode=true;
//					emit message("<font color=\"#0000FF\">Eye test mode ON.</front>");
//					emit speak("Eye test mode.");
				}
				else
				{
					// ###############################################################################
					// disabled since the head is not in use use!
					// ###############################################################################
//					eyeTestMode=false;
//					head->look("FORWARD");
//					head->look("NORMAL");
//					emit message("<font color=\"#0000FF\">Eye test mode OFF.</front>");
//					emit speak("Eye test mode disabled.");
				}
				toggle0 = !toggle0;
			}
			break;
		case 1: // 2 on js
			if (buttonState==true)
			{
				if (toggle1 == false)
				{
					servoTestMode = true;
					emit message("<font color=\"#0000FF\">Servo test mode ON.</front>");
					emit message(QString("Servo %1 selected.").arg(currentTestServo+1));
					emit speak("Servo test mode");
				}
				else
				{
					servoTestMode = false;
					emit message("<font color=\"#0000FF\">Servo test mode OFF.</front>");
					emit speak("Servo test mode disabled");
				}
				toggle1 = !toggle1;
			}
			break;
		case 2: // 3 on js
			break;
		case 3: // 4 on js
			break;
		case 4: // 5 on js
			break;
		case 5: // 6 on js
			break;
		case 6: // 7 on js
			// left front button
			if (buttonState==true)
			{
				// servo test mode
				if (servoTestMode==true)
				{
					emit message(QString("Servo %1 selected.").arg((--currentTestServo)+1));
				}
			}
			break;
		case 7: // 8 on js
			// right front button
			if (buttonState==true)
			{
				// servo test mode
				if (servoTestMode==true)
				{
					emit message(QString("Servo %1 selected.").arg((++currentTestServo)+1));
				}
			}
			break;
		case 10:
			//
			// red button, right side
			//
			if (buttonState==true)
			{
				if (toggle10 == false)
				{
					mecanumDriveMode=true;
					emit message("<font color=\"#0000FF\">Mecanum test mode enabled.</front>");
					emit speak("Mecanum test mode.");
				}
				else
				{
					mecanumDriveMode=false;
					emit message("<font color=\"#0000FF\">Mecanum test mode disabled.</front>");
					emit speak("Mecanum test mode disabled.");
				}
				toggle10 = !toggle10;
			}
			break;
		case 11:
			//
			// button top, middle
			//
			if (buttonState==true)
			{
				if (toggle11 == false)
				{
					// ###############################################################################
					// disabled since this would use the motor 3 and 4 which are now in different use!
					// ###############################################################################
//					cameraTestMode = true;
//					emit message("<font color=\"#0000FF\">Camera test mode ON.</front>");
//					emit speak("Camerea test mode.");
				}
				else
				{
//					cameraTestMode = false;
//					emit message("<font color=\"#0000FF\">Camera test mode OFF.</front>");
//					emit speak("Camerea test mode disabled.");
				}
				toggle11 = !toggle11;
			}
			break;
	}
}


bool Direcs::simulationMode() const
{
	return robotSimulationMode;
}


void Direcs::setSimulationMode(bool status)
{
	robotSimulationMode = status;

	if (status == true)
	{
		emit message("<font color=\"#0000FF\">Simulation mode enabled!!</front>");

		if (laserThread->isRunning() == false)
		{
			emit message("Starting Laser thread...", false);
			laserThread->start();
			emit message("Started.");
		}

		if (sensorThread->isRunning() == false)
		{
			emit message("Starting Sensor thread...", false);
			sensorThread->start();
			emit message("Started.");
		}

#ifndef BUILDFORROBOT
		if (!consoleMode)
		{
			if (plotThread->isRunning() == false)
			{
				emit message("Starting plot thread...", false);
				plotThread->start();
				emit message("Started.");
			}
		}
#endif

		if (obstCheckThread->isRunning() == false)
		{
			emit message("Initializing obstacle check thread...", false);
			obstCheckThread->init();
			emit message("Initialized.");
			emit message("Starting obstacle check thread...", false);
			obstCheckThread->start();
			emit message("Started.");
		}
	}
	else
	{
		// FIXME: wozu das??? was ist mit dem plothread? merken welche geatartet waren? if robot is OFF stop all threads which read from the circuit!
// 		if (robotIsOn == false)
// 		{
// 			sensorThread->stop();
// 			emit message("Sensor thread stopped.");
// 		}
		emit message("<font color=\"#0000FF\">Simulation mode disabled.</font>");
	}
}


/*
void Direcs::setRobotState(bool state)
{
	robotIsOn = state;
}
*/


void Direcs::checkArguments()
{
	if (arguments.contains("console", Qt::CaseInsensitive))
	{
		consoleMode = true;
		emit publishConsoleMode(consoleMode);
		qDebug("CONSOLE mode activated. Now passing all messages to the console.");
	}
}


void Direcs::nextDemoPhase(int phase)
{
	static int iPhase = phase;
	static bool introductionComplete = false;




	if (demoMode)
	{
		// do not speak the text any longer, if spoken before
		if (introductionComplete)
			phase = 4;

		emit message(QString("demo phase=%1").arg(iPhase));

		switch (phase)
		{
		// do some weird stuff here
		// mabye talk a bit
		// drive around for some seconds
		// blink
		case 1:
			// say something, next phase is no. 2
			emit speak("Hello Makers. I will introduce myself now for you guys. My name is direx 1. And I hope everything works fine now in the next minutes. Since Marcus updated my software yesterday night very late.", ++phase);
			break;
		case 2:
			// say something, next phase is no. 3
			emit speak("I am able of driving around autonomically without hitting any obstacles. Well. Hopefully. I am using a laserscanner. and I can show my status with some R G B LEDs. I can also send my data over wireless network to show them on another laptop, or so.", ++phase);
			break;
		case 3:
			// say something, next phase is no. 3
			emit speak("But I do not want to bore you any longer. See me driving around and enjoy it. We should do some party now. I will play music by random. So if the next song is still boring, Markus cann skip it on the touchscreen. Okay. lets go! Put your hands up in the air and wave them like you just dont care!", ++phase);

			// speech completed! do not say it again, if called!
			introductionComplete = true;

			break;
		case 4:
			// play some music
			mediaObject->play();
			break;
		case 5:
/*
			if (mediaObject->state() ==  Phonon::PlayingState)
			{
				mediaObject->pause();
			}
*/
			break;
		default:
//            emit speak("Thanks you guys. Thats it.", -1);
			break;
		}
	}
}


void Direcs::setDemoMode(bool status)
{
	// save state locally
	demoMode = status;


	if (status == true)
	{
		emit message("<font color=\"#0000FF\">Demo mode enabled!!</front>");

		if (demoThread->isRunning() == false)
		{
			emit message("Starting demo thread...", false);
			demoThread->start();
			emit message("Started.");

emit speak("Okay, here we go.", 1);

			// play some music
//			mediaObject->play();
		}
	}
	else
	{
		emit message("<font color=\"#0000FF\">Demo mode disabled!!</front>");

		// stop the music
		if (mediaObject->state() ==  Phonon::PlayingState)
		{
			mediaObject->pause();
		}

		emit message("Stopping demo thread...", false);
		demoThread->stop();
		emit message("Stopped.");
	}
}


void Direcs::mediaPlayerFinished()
{
	static int number = 0;
		   int randNumber = 0;


	if (demoMode)
	{
		do
		{
			randNumber = rand() % 7 +1; // (1 to 7)
		} while (randNumber == number);

		// store current number for next call of this method
		number = randNumber;

		emit message(QString("Media player restart with file %1.").arg(number));

#ifdef Q_OS_LINUX
			mediaObject->setCurrentSource(Phonon::MediaSource(QString("../../../../media/%1").arg(number)));
#else
			mediaObject->setCurrentSource(Phonon::MediaSource(QString("../../../../media/%1.mp3").arg(number)));
#endif

		// restart music, since the player finished
		mediaObject->play();
	}
}


void Direcs::systemerrorcatcher(int errorlevel)
{
	switch (errorlevel)
	{
	case -1: // error with laserscanner thread
		if (robotDrives)
		{
			emit message("<font color=\"#0000FF\"Emergency stop due to laserscanner problems!/font>");
			logfile->appendLog("Emergency stop due to laserscanner problems!");

			// stop driving
			drive(STOP);

			// flashlight ON
			motors->flashlight(ON);
		}
		break;

	case -2: // error with sensor thread -> error in atmel read / write port!
		emit message("Error with sensor thread. Forcing shutdown when exitng direcs later (No last hardware init).");
		logfile->appendLog("Error with sensor thread. Forcing shutdown when exitng direcs later (No last hardware init).");
#ifndef BUILDFORROBOT
		if (!consoleMode)
		{
			/// @todo why stopping plot thread here?
			plotThread->stop();
		}
#endif
		// force shutdown and do no circuit inits etc, when exiting direcs
		forceShutdown = true;
		break;
	}
}


void Direcs::drivingLight(unsigned char color)
{
	switch (color)
	{
		case RED:
			rgbLeds->setBrightness(RGBLED1, MAXPWM);
			rgbLeds->setBrightness(RGBLED2, MINPWM);
			rgbLeds->setBrightness(RGBLED3, MINPWM);
			rgbLeds->setBrightness(RGBLED4, MAXPWM);
			rgbLeds->setBrightness(RGBLED5, MINPWM);
			rgbLeds->setBrightness(RGBLED6, MINPWM);
			break;
		case GREEN:
			rgbLeds->setBrightness(RGBLED1, MINPWM);
			rgbLeds->setBrightness(RGBLED2, MAXPWM);
			rgbLeds->setBrightness(RGBLED3, MINPWM);
			rgbLeds->setBrightness(RGBLED4, MINPWM);
			rgbLeds->setBrightness(RGBLED5, MAXPWM);
			rgbLeds->setBrightness(RGBLED6, MINPWM);
			break;

		case BLUE:
			rgbLeds->setBrightness(RGBLED1, MINPWM);
			rgbLeds->setBrightness(RGBLED2, MINPWM);
			rgbLeds->setBrightness(RGBLED3, MAXPWM);
			rgbLeds->setBrightness(RGBLED4, MINPWM);
			rgbLeds->setBrightness(RGBLED5, MINPWM);
			rgbLeds->setBrightness(RGBLED6, MAXPWM);
			break;

		case WHITE:
			rgbLeds->setBrightness(RGBLED1, MAXPWM);
			rgbLeds->setBrightness(RGBLED2, MAXPWM);
			rgbLeds->setBrightness(RGBLED3, MAXPWM);
			rgbLeds->setBrightness(RGBLED4, MAXPWM);
			rgbLeds->setBrightness(RGBLED5, MAXPWM);
			rgbLeds->setBrightness(RGBLED6, MAXPWM);
			break;

		case LEDOFF:
			rgbLeds->setBrightness(RGBLED1, MINPWM);
			rgbLeds->setBrightness(RGBLED2, MINPWM);
			rgbLeds->setBrightness(RGBLED3, MINPWM);
			rgbLeds->setBrightness(RGBLED4, MINPWM);
			rgbLeds->setBrightness(RGBLED5, MINPWM);
			rgbLeds->setBrightness(RGBLED6, MINPWM);
			break;
	}
}

void Direcs::resetMotorSpeed()
{
	// when speed already read from ini-file
	if (mot1Speed != 0)
	{
		//-------------------------------------------------------
		// set the read motor speed
		//-------------------------------------------------------
		emit message("Setting motor speed in microcontroller");
		motors->setMotorSpeed(MOTOR1, mot1Speed);
		motors->setMotorSpeed(MOTOR2, mot2Speed);
		motors->setMotorSpeed(MOTOR3, mot3Speed);
		motors->setMotorSpeed(MOTOR4, mot4Speed);
		emit message("Motor speed set.");
	}
	else
	{
		emit message("ERROR: Motor speed not read! (Direcs::resetMotorSpeed)");
	}
}


void Direcs::test()
{
/*
	#ifdef Q_OS_LINUX
	Phonon::MediaObject *music = Phonon::createPlayer(Phonon::MusicCategory, Phonon::MediaSource("../../../../dr.mp3"));
	#endif

	#ifdef Q_OS_MAC
	Phonon::MediaObject *music = Phonon::createPlayer(Phonon::MusicCategory, Phonon::MediaSource("../../../../dr.mp3"));
	#endif
*/

	/*
	if (mediaObject->state() !=  Phonon::PlayingState)
	{
		mediaObject->play();
		return;
	}

	if (mediaObject->state() ==  Phonon::PlayingState)
	{
		mediaObject->pause();
		return;
	}
*/

//	static bool toggle = false;


#ifdef Q_OS_LINUX
    speakThread->setLanguage("en");
#endif
//    emit speak(tr("Hello world"));
    emit speak(tr("Hallo Welt!"));
//    emit speak(tr("The voltage for battery %1 is %2 Volt. For battery %3 it is %4 Volt.").arg( 1 ).arg( sensorThread->getVoltage(VOLTAGESENSOR1) ).arg( 2 ).arg( sensorThread->getVoltage(VOLTAGESENSOR2) ));
    emit speak(tr("Die Akkuspannung %1 ist %2 Volt. Akku %3 hat noch %4 Volt.").arg( 1 ).arg( sensorThread->getVoltage(VOLTAGESENSOR1) ).arg( 2 ).arg( sensorThread->getVoltage(VOLTAGESENSOR2) ));
    emit speak(tr("Alles klar, los geht's!"));

//	toggle = !toggle;
/*
	laserThread->setMeasureMode(toggle);
	emit message(QString("Measure mode = %1").arg(toggle));


	static int color = 0;


	if (color==0)
	{
		color++;
		drivingLight(RED);
		return;
	}

	if (color==1)
	{
		color++;
		drivingLight(GREEN);
		return;
	}

	if (color==2)
	{
		color++;
		drivingLight(BLUE);
		return;
	}

	if (color==3)
	{
		color++;
		drivingLight(WHITE);
		return;
	}



	if (color==4)
	{
		color++;
		rgbLeds->setBrightness(RGBLED1, MAXPWM);
		rgbLeds->setBrightness(RGBLED2, 1);
		rgbLeds->setBrightness(RGBLED3, 1);

		rgbLeds->setBrightness(RGBLED4, 1);
		rgbLeds->setBrightness(RGBLED5, MAXPWM);
		rgbLeds->setBrightness(RGBLED6, 1);
		return;
	}

	if (color==5)
	{
		color++;
		rgbLeds->setBrightness(RGBLED1, 1);
		rgbLeds->setBrightness(RGBLED2, MAXPWM);
		rgbLeds->setBrightness(RGBLED3, 1);

		rgbLeds->setBrightness(RGBLED4, 1);
		rgbLeds->setBrightness(RGBLED5, 1);
		rgbLeds->setBrightness(RGBLED6, MAXPWM);
		return;
	}

	if (color==6)
	{
		color++;
		rgbLeds->setBrightness(RGBLED1, 1);
		rgbLeds->setBrightness(RGBLED2, 1);
		rgbLeds->setBrightness(RGBLED3, MAXPWM);

		rgbLeds->setBrightness(RGBLED4, MAXPWM);
		rgbLeds->setBrightness(RGBLED5, 1);
		rgbLeds->setBrightness(RGBLED6, 1);
		return;
	}

	if (color==7)
	{
		color=0;
		drivingLight(LEDOFF);
		return;
	}
*/


/*
	static bool toggle = OFF;


	if (toggle == OFF)
	{
		toggle = ON;
		//head->look("LEFT");
		emit sendNetworkString("ON");
#ifdef Q_OS_LINUX // currently supported only under linux (no MAC OS at the moment)
		speakThread->setLanguage("en");
		//speakThread->setVoice(1, 200); // 1=male, 'age'=255
#endif
        // Say some text;
		QDateTime now = QDateTime::currentDateTime();
		emit speak(tr("Hello Markus. Today it's the %1 of %2, %3. The time is %4 %5.")
				   .arg(now.toString("d"))
				   .arg(now.toString("MMMM"))
				   .arg(now.toString("yyyy"))
				   .arg(now.toString("h"))
				   .arg(now.toString("m")));
	}
	else
	{
		toggle = OFF;
		emit sendNetworkString("OFF");

#ifdef Q_OS_LINUX // currently supported only under linux (no MAC OS at the moment)
		speakThread->setLanguage("de");
#endif
        //speakThread->setVoice(2, 5); // 2=female, 'age'=5
		// Say some text;
		QDateTime now = QDateTime::currentDateTime();
		emit speak(tr("und das ganze geht auch auf Deutsch. Heute ist der %1te. %2, %3. Es ist jetzt %4 Uhr %5.")
				   .arg(now.toString("d"))
				   .arg(now.toString("MMMM"))
				   .arg(now.toString("yyyy"))
				   .arg(now.toString("h"))
				   .arg(now.toString("m")));
	}
		//head->look("RIGHT");


	motors->flashlight(toggle);
	*/
}
