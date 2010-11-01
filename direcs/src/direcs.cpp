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

#include "direcs.h"


int main(int argc, char *argv[])
{
	bool consoleMode = false;


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
		Direcs d(consoleMode);

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
		Direcs d(consoleMode);

		// init direcs
		d.init();

		return app.exec();
	}
}


Direcs::Direcs(bool bConsoleMode)
{
	// store mode from main method
	consoleMode = bConsoleMode;

	//------------------------------------------------------------------
	// create the objects
	//------------------------------------------------------------------
	logfile = new Logfile();
#ifdef Q_OS_LINUX // currently supported only under linux (no MAC OS at the moment)
	speakThread = new SpeakThread();
#endif

	if (consoleMode)
	{
		consoleGui = new ConsoleGui();
//		myEvent = new Events();
		// TODO: install an event filter to grab CTRL+C
//  		consoleGui->installEventFilter(myEvent);
//			this->installEventFilter(myEvent);
	}
	else
	{
		settingsDialog = new SettingsDialog();
		joystickDialog = new JoystickDialog();
		aboutDialog = new AboutDialog();
		gui = new Gui(settingsDialog, joystickDialog, aboutDialog);
		splash = new QSplashScreen(QPixmap(":/images/images/splash.png"));
	}


	mutex = new QMutex();
	interface1 = new InterfaceAvr();
	circuit1 = new Circuit(interface1, mutex);
//	TODO: heartbeat = new Heartbeat(interface1, mutex);
	motors = new Motor(interface1, mutex);
	sensorThread = new SensorThread(interface1, mutex);
	servos = new Servo(interface1, mutex);
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
	if (!consoleMode)
	{
		camThread = new CamThread();
	}
	joystick = new Joystick();
	head = new Head(servos);

	drivingSpeedTimer = new QTimer();
}


void Direcs::init()
{
	if (!consoleMode)
	{
		aboutDialog->setVersion("0.9"); // TODO: put this at a nicer place // this is also shown in the about dialog
		splashPosition = Qt::AlignHCenter | Qt::AlignBottom;
		splashTextColor = Qt::white;
	}
	forceShutdown = false;
	inifile1->setFilename("direcs.ini");
	logfile->setFilename("direcs.log");
	// TODO: direcs->setLogFileName("direcs.log");
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
	servoTestMode = false;
	testDriveMode = false;
	mecanumDriveMode = false;
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
	// show the splash screen
	//--------------------------------------------------------------------------
	if (!consoleMode)
	{
		splash->show();
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
	// TODO: not in use anymore! connect(this, SIGNAL(publishConsoleMode(bool)), gui, SLOT(setConsoleMode(bool)));

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
		connect(this, SIGNAL( message(QString) ), gui, SLOT( appendLog(QString) ));
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
	connect(interface1,	SIGNAL( robotState(bool) ), servos,			SLOT( setRobotState(bool) ));
//	connect(interface1,	SIGNAL( robotState(bool) ), this,			SLOT( setRobotState(bool) ));
	if (!consoleMode)
	{
		connect(interface1,	SIGNAL( robotState(bool) ), gui,			SLOT( setRobotControls(bool) ));
	}

	// also set the robot to OFF, when there are problems with the circuit
	connect(circuit1,	SIGNAL( robotState(bool) ), motors,			SLOT( setRobotState(bool) ));
	connect(circuit1,	SIGNAL( robotState(bool) ), sensorThread,	SLOT( setRobotState(bool) ));
//	connect(circuit1,	SIGNAL( robotState(bool) ), heartbeat,		SLOT( setRobotState(bool) ));
	connect(circuit1,	SIGNAL( robotState(bool) ), servos,			SLOT( setRobotState(bool) ));
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
	// currently not in use:
	//connect(gui, SIGNAL(test()), camThread, SLOT(test()));

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
		connect(settingsDialog, SIGNAL(setRobotSlot(int)), obstCheckThread, SLOT(setRobotSlot(int)));
		connect(settingsDialog, SIGNAL(setRobotSlotWidth(int)), obstCheckThread, SLOT(setRobotSlotWidth(int)));

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
		connect(settingsDialog, SIGNAL(setMinObstacleDistanceLaser(int)), obstCheckThread, SLOT(setMinObstacleDistanceLaser(int)));

		//--------------------------------------------------------------------------
		// let the GUI show servo messages in the log
		//--------------------------------------------------------------------------
		connect(servos, SIGNAL(message(QString)), gui, SLOT(appendLog(QString)));

		//-------------------------------------------------------------------------------------
		// disable face detection in the GUI, on error with loading haar cascade in CamThread
		// Must be before readSettings!
		//-------------------------------------------------------------------------------------
		connect(camThread, SIGNAL( disableFaceDetection() ), gui, SLOT( disableFaceDetection() ));

		//-------------------------------------------------------------------------------------
		// disable camera controls in the GUI, on error opeing the camera in the CamThread
		// Must be before readSettings!
		//-------------------------------------------------------------------------------------
		connect(camThread, SIGNAL( disableCamera() ), gui, SLOT( disableCamera() ));
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
		#ifdef Q_OS_LINUX // currently supported only under linux (no MAC OS at the moment)
		connect(this, SIGNAL( speak(QString) ), speakThread, SLOT( speak(QString) ));

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
		#endif


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
			connect(plotThread, SIGNAL( plotDataComplete1(double *, double *, int) ), gui, SLOT( setPlotData1(double *, double *, int) ));
			connect(plotThread, SIGNAL( plotDataComplete2(double *, double *, int) ), gui, SLOT( setPlotData2(double *, double *, int) ));
			connect(plotThread, SIGNAL( plotDataComplete3(double *, double *, int) ), gui, SLOT( setPlotData3(double *, double *, int) ));
			connect(plotThread, SIGNAL( plotDataComplete4(double *, double *, int) ), gui, SLOT( setPlotData4(double *, double *, int) ));
			connect(plotThread, SIGNAL( plotDataComplete5(double *, double *, int) ), gui, SLOT( setPlotData5(double *, double *, int) ));
			connect(plotThread, SIGNAL( plotDataComplete6(double *, double *, int) ), gui, SLOT( setPlotData6(double *, double *, int) ));
			connect(plotThread, SIGNAL( plotDataCompleteHeartbeat(double *, double *, int) ), gui, SLOT( setPlotDataHeartbeat(double*, double*, int) ));
		}
#endif

		//----------------------------------------------------------------------------
		// let the GUI show messages in the log docks
		//----------------------------------------------------------------------------
		if (!consoleMode)
		{
			connect(joystick, SIGNAL(message(QString)), gui, SLOT(appendLog(QString)));
			connect(interface1, SIGNAL(message(QString)), gui, SLOT(appendSerialLog(QString)));
		}
		else
		{
			connect(joystick, SIGNAL(message(QString)), consoleGui, SLOT(appendLog(QString)));
			connect(interface1, SIGNAL(message(QString)), consoleGui, SLOT(appendSerialLog(QString)));
		}

		// also emit messages to the logfile
		connect(interface1, SIGNAL(message(QString)), logfile, SLOT(appendLog(QString))); // FIXME: to fast in case of error for writing the logfile!
//		connect(joystick, SIGNAL(message(QString)), logfile, SLOT(appendLog(QString))); // TODO: check if this is okay for the logfile writer in case of error TO FAST for logfile!!!

		//-------------------------------------------------------
		// start the network thread (waiting for commands)
		//-------------------------------------------------------
		enableRemoteControlListening(true);

		//-------------------------------------------------------
		// Open serial port for microcontroller communication
		//-------------------------------------------------------
		emit splashMessage("Opening serial port for microcontroller communication...");
		emit message("Opening serial port for microcontroller communication...", false);

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
				QMessageBox msgbox(QMessageBox::Warning, tr("Error with robots serial port"), tr("Error opening serial port %1").arg(serialPortMicrocontroller), QMessageBox::Ok | QMessageBox::Default);
				msgbox.exec();
			}
			*/

			if (!consoleMode)
			{
				// set GUI LED for compass module
				// has to be OFF, since the Atmel circuit is OFF
				gui->setLEDCompass(LEDOFF);
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
				connect(gui, SIGNAL( initServos() ), servos, SLOT( init() ) );
			}


			//==========================
			// init the robots circuit
			//==========================
			emit splashMessage("Searching robot...");

			if (circuit1->initCircuit() == true)
			{
				emit message("Robot is <font color=\"#00FF00\">ON</font> and answers.");
				logfile->appendLog("Robot is ON and answers.");

				// check compass module
				if (circuit1->initCompass() == true)
				{
					gui->appendLog("3D compass module detected.");
					if (!consoleMode)
					{
						gui->setLEDCompass(GREEN);
					}
				}
				else
				{
					gui->appendLog("<font color=\"#FF0000\">3D compass module not connected!</font>");
					if (!consoleMode)
					{
						gui->setLEDCompass(RED);
					}
				}

				//-------------------------------------------------------
				// set the read motor speed
				//-------------------------------------------------------
				motors->setMotorSpeed(1, mot1Speed);
				motors->setMotorSpeed(2, mot2Speed);
				motors->setMotorSpeed(3, mot3Speed);
				motors->setMotorSpeed(4, mot4Speed);
				emit message("Motor speed set in microcontroller");

				//-------------------------------------------------------
				// move all servos in their default positions
				//-------------------------------------------------------
				/* TODO: temporarily deactivated (no servos mounted on the current robot)
				servos->init();
				emit message("Servos moved to default positions");
				*/

				// TODO: start heartbeat thread and see, whats going on there! Also to do: define atmel code for an "heartbeat answer / action" !!!!!
				//-----------------------------------------------------------
				// start the heartbeat thread
				//-----------------------------------------------------------
				/*
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
			} // init was successfull
			else
			{
				logfile->appendLog("Robot is OFF! Please turn it ON!");
				emit message("<font color=\"#FF0000\">The robot is OFF! Please turn it ON!</font>");
				emit message("Heartbeat thread NOT started!");
				emit message("Sensor thread NOT started!");
				emit message("Plot thread NOT started!");
			}
		} // robot is ON


		//-----------------------------------------------------------
		// check if a joystick is connected
		//-----------------------------------------------------------
		if (joystick->isConnected())
		{
			// start the joystick thread
			if (joystick->isRunning() == false)
			{
				emit splashMessage("Starting joystick thread...");
				emit message("Starting joystick thread...", false);
				joystick->start();
				emit message("Joystick thread started.");
			}
		}

		if (!consoleMode)
		{
			//----------------------------------------------------------------------------
			// drive in the direction which was emited from the gui
			//----------------------------------------------------------------------------
			connect(gui, SIGNAL( drive(unsigned char) ), this, SLOT( drive(unsigned char) ));
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
		// 	connect(sensorThread, SIGNAL( heartbeat(unsigned char)), logfile, SLOT( writeHeartbeat(unsigned char) ) );  // FIXME: this is too often!! Because of 10ms sensor thread!!


		if (!consoleMode)
		{
			//----------------------------------------------------------------------------
			// connect sensor contact signals to "show contact alarm"
			// (Whenever the an alarm contact was closed, show the result in the cam image)
			//----------------------------------------------------------------------------
			connect(sensorThread, SIGNAL(contactAlarm(char, bool)), camThread, SLOT(drawContactAlarm(char, bool)));

#ifdef Q_OS_LINUX // currently supported only under linux (no MAC OS at the moment)
			//----------------------------------------------------------------------------
			// connect camDataComplete from the cam thread to signal "setCamImage"
			// (Whenever the image is complete, the image is shown in the GUI)
			//----------------------------------------------------------------------------
			connect(camThread, SIGNAL( camDataComplete(IplImage*) ), gui, SLOT( setCamImage(IplImage*) ));
#endif

			//--------------------------------------------------------------------------------------------------------
			// connect faceDetected from the camThread to the faceTracking unit and to the GUI (to show some values)
			//--------------------------------------------------------------------------------------------------------
			connect(camThread, SIGNAL( faceDetected(int, int, int, int, int, int) ), this, SLOT( faceTracking(int, int, int, int) ));
			connect(camThread, SIGNAL( faceDetected(int, int, int, int, int, int) ),  gui, SLOT( showFaceTrackData(int, int, int, int, int, int) ));

			//----------------------------------------------------------------------------
			// enable face detection, when activated in the GUI
			//----------------------------------------------------------------------------
			connect(gui, SIGNAL( enableFaceDetection(int) ), camThread, SLOT( enableFaceDetection(int) ));

			//----------------------------------------------------------------------------
			// enable face tracking, when activated in the GUI
			//----------------------------------------------------------------------------
			connect(gui, SIGNAL( enableFaceTracking(int) ), this, SLOT( enableFaceTracking(int) ));

			//----------------------------------------------------------------------------
			// show the face track direction in the gui
			//----------------------------------------------------------------------------
			connect(this, SIGNAL( showFaceTrackDirection(QString) ), gui, SLOT( showFaceTrackDirection(QString)) );
		}


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

		if (!consoleMode)
		{
			//-----------------------------------------------------------
			// check if camera is connected
			//-----------------------------------------------------------
			if (camThread->isConnected())
			{
				if (camThread->isRunning() == false)
				{
					emit message("Starting camera thread...", false);
					camThread->start();
					emit message("Camera thread started.");
					if (camThread->isConnected())
					{
						emit message(QString("Camera resolution is %1x%2.").arg(camThread->imageWidth()).arg(camThread->imageHeight()));
						// tell the gui the image size and depth
						gui->setCamImageData(camThread->imageWidth(), camThread->imageHeight(), camThread->imagePixelDepth());
					}
				}
			}
			else
			{
				emit message("Camera thread NOT started!");
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
				// TODO: nice exit point and error message
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
				emit splashMessage("Starting obstacle check thread...");
				emit message("Starting obstacle check thread...", false);
				obstCheckThread->start();
				emit message("Obstacle check thread started.");
			}
		}
		else
		{
			emit message("<font color=\"#FF0000\">NO laser scanners found! Thread NOT started!</font>");
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
			// delete the splash screen
			finishSplash();
		}
	} // ini-file found
	else
	{
		if (!consoleMode)
		{
			// file not found-Msg
			QMessageBox msgbox(QMessageBox::Critical, tr("direcs"), tr("Required configuration file \"%1\" not found! File perhaps not in the same directory?\n\nSorry, exiting direcs NOW...").arg(inifile1->getInifileName()), QMessageBox::Ok | QMessageBox::Default);
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
			inifile1->writeSetting("Config", "robotSlot", settingsDialog->getSliderRobotSlotValue());
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
		// TODO: ask for exit on console!
	}


	emit message("STOPPING drive!");
	drive(STOP); // FIXME: what if the robot (serial communication hangs here?!?) tmeout?!?


	// TODO: a universal quit-threads-method
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


#ifdef Q_OS_LINUX // currently supported only under linux (no MAC OS at the moment)
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
#endif


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
	// TODO: quit the heartbeat thread
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


	//-------------------------------------------------------
	// Last init for the robots circuits
	//-------------------------------------------------------
	emit message("TODO: Last circuit init...");
// 	circuit1->initCircuit(); // FIXME: what, if the robote serial communication hangs here? timeout check?


	if (forceShutdown==false) // don't close a serial port when we have a forced shutdown
	{
		//-----------------------------
		// close serial port to mc
		//-----------------------------
		emit message("Closing serial port to microcontroller...");
		interface1->closeComPort();
	}


	//-----------------------------
	// last log file message
	//-----------------------------
	if (writeLogFile)
		logfile->appendLog("-------------------------------------------------------------------------------------------------- stop -");


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
	delete logfile;
	#ifdef Q_OS_LINUX // currently supported only under linux (no MAC OS at the moment)
	delete speakThread;
	#endif
	delete laserThread;
	delete netThread;
	if (!consoleMode)
	{
		delete camThread;
	}
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
	delete motors;
	delete sensorThread;
	// TODO: delete heartbeat;
	delete circuit1;
	delete interface1;
	if (!consoleMode)
	{
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

		//Todo: Is is the correct method to call the destrucotr, to end the program?!?
		QApplication::exit();
		*/
}


void Direcs::showSplashMessage(QString text)
{
	if (!consoleMode)
	{
		splash->showMessage(text, splashPosition, splashTextColor);
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


	// if all sensor values are the same like the last, do nothing!
	if (sensorAlarm == lastSensorValue)
	{
		// store this sensor alarm value
		lastSensorValue = sensorAlarm;
		//alarmCounter++;
		return;
	}


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
		emit showPreferredDirection("*NONE*");

		// clear alarm buffers
		obstacleAlarmFrontLeftList.clear();
		obstacleAlarmFrontRightList.clear();

		// store this sensor alarm value
		lastSensorValue = sensorAlarm;
		// reset the alarm counter
		//alarmCounter = 0;

		if (robotDrives)
		{
			emit message("<b>Obstacles everywhere in front of the robot. Waiting.</b>");

			//----------------
			// drive WAIT
			//----------------
			drive(WAIT);
			motors->flashlight(ON);
			emit speak("I think, I need some help!");
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
	if (!consoleMode)
	{
		Q_UNUSED (faces) // not in use, at the moment

		// TODO: put values to consts or ini
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
				/*
				motors->motorControl(MOTOR3, OFF, SAME);
				motors->motorControl(MOTOR4, OFF, SAME);
				*/
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
				/*
				motors->motorControl(MOTOR3, ON, COUNTERCLOCKWISE);
				motors->motorControl(MOTOR4, OFF, SAME);
				*/
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
				/*
				motors->motorControl(MOTOR3, ON, CLOCKWISE);
				motors->motorControl(MOTOR4, OFF, SAME);
				*/
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
				/*
				motors->motorControl(MOTOR3, OFF, SAME);
				motors->motorControl(MOTOR4, ON, CLOCKWISE);
				*/
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
				/*
				motors->motorControl(MOTOR3, ON, COUNTERCLOCKWISE);
				motors->motorControl(MOTOR4, ON, CLOCKWISE);
				*/
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
				/*
				motors->motorControl(MOTOR3, ON, CLOCKWISE);
				motors->motorControl(MOTOR4, ON, CLOCKWISE);
				*/
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
				/*
				motors->motorControl(MOTOR3, OFF, SAME);
				motors->motorControl(MOTOR4, ON, COUNTERCLOCKWISE);
				*/
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
				/*
				motors->motorControl(MOTOR3, ON, COUNTERCLOCKWISE);
				motors->motorControl(MOTOR4, ON, COUNTERCLOCKWISE);
				*/
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
				/*
				motors->motorControl(MOTOR3, ON, CLOCKWISE);
				motors->motorControl(MOTOR4, ON, COUNTERCLOCKWISE);
				*/
			}
			emit showFaceTrackDirection("DOWNRIGHT");
			return;
		}
	}
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
	// TODO: do a check and emit this to the gui to show the lables in red!
	//-------------------------------------------------------
	// TODO: is this the best place for the following lines?
	static bool marker=false; // TODO: do this with a timer or something, for repeating it

	if (sensorThread->getVoltage(VOLTAGESENSOR1) < MINIMUMVOLTAGE1)
	{

		if (!marker)
		{
			marker = true;
			emit speak("The 12 volt batteries are empty.");
			// TODO: maybe also do a shutdown or a 'stop" while driving...?!?
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
			// TODO: maybe also do a shutdown or a 'stop" while driving...?!?
		}
	}
	else
	{
		marker = false;
	}
*/
}


void Direcs::drive(const unsigned char command)
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
			emit message("FORWARD");
			if (!consoleMode)
			{
				gui->showMotorStatus(MOTOR1, SAME, CLOCKWISE);
				gui->showMotorStatus(MOTOR2, SAME, CLOCKWISE);
				gui->showMotorStatus(MOTOR3, SAME, CLOCKWISE);
				gui->showMotorStatus(MOTOR4, SAME, CLOCKWISE);
			}
//			/* TODO: this is a test mode for sending only -one- serial command to the mc
			motors->motorControl(MOTOR1, SAME, CLOCKWISE);
			motors->motorControl(MOTOR2, SAME, CLOCKWISE);
			motors->motorControl(MOTOR3, SAME, CLOCKWISE);
			motors->motorControl(MOTOR4, SAME, CLOCKWISE);
//			*/
//			motors->motorControl(ALLMOTORS, SAME, command);
			return;
			break;

		case BACKWARD:
			emit message("BACKWARD");
			if (!consoleMode)
			{
				gui->showMotorStatus(MOTOR1, SAME, COUNTERCLOCKWISE);
				gui->showMotorStatus(MOTOR2, SAME, COUNTERCLOCKWISE);
				gui->showMotorStatus(MOTOR3, SAME, COUNTERCLOCKWISE);
				gui->showMotorStatus(MOTOR4, SAME, COUNTERCLOCKWISE);
			}
//			/* TODO: this is a test mode for sending only -one- serial command to the mc
			motors->motorControl(MOTOR1, SAME, COUNTERCLOCKWISE);
			motors->motorControl(MOTOR2, SAME, COUNTERCLOCKWISE);
			motors->motorControl(MOTOR3, SAME, COUNTERCLOCKWISE);
			motors->motorControl(MOTOR4, SAME, COUNTERCLOCKWISE);
//			*/
//			motors->motorControl(ALLMOTORS, SAME, command);
			return;
			break;

		case LEFT:
			emit message("LEFT");
			if (!consoleMode)
			{
				gui->showMotorStatus(MOTOR1, SAME, COUNTERCLOCKWISE);
				gui->showMotorStatus(MOTOR2, SAME, CLOCKWISE);
				gui->showMotorStatus(MOTOR3, SAME, CLOCKWISE);
				gui->showMotorStatus(MOTOR4, SAME, COUNTERCLOCKWISE);
			}
//			/* //TODO: this is a test mode for sending only -one- serial command to the mc
			motors->motorControl(MOTOR1, SAME, COUNTERCLOCKWISE);
			motors->motorControl(MOTOR2, SAME, CLOCKWISE);
			motors->motorControl(MOTOR3, SAME, CLOCKWISE);
			motors->motorControl(MOTOR4, SAME, COUNTERCLOCKWISE);
// 			*/
// 			motors->motorControl(ALLMOTORS, SAME, command);
			return;
			break;

		case RIGHT:
			emit message("RIGHT");
			if (!consoleMode)
			{
				gui->showMotorStatus(MOTOR1, SAME, CLOCKWISE);
				gui->showMotorStatus(MOTOR2, SAME, COUNTERCLOCKWISE);
				gui->showMotorStatus(MOTOR3, SAME, COUNTERCLOCKWISE);
				gui->showMotorStatus(MOTOR4, SAME, CLOCKWISE);
			}
//			/* //TODO: this is a test mode for sending only -one- serial command to the mc
			motors->motorControl(MOTOR1, SAME, CLOCKWISE);
			motors->motorControl(MOTOR2, SAME, COUNTERCLOCKWISE);
			motors->motorControl(MOTOR3, SAME, COUNTERCLOCKWISE);
			motors->motorControl(MOTOR4, SAME, CLOCKWISE);
// 			*/
// 			motors->motorControl(ALLMOTORS, SAME, command);
			return;
			break;

		case TURNLEFT:
			emit message("TURNLEFT");
			if (!consoleMode)
			{
				gui->showMotorStatus(MOTOR1, SAME, COUNTERCLOCKWISE);
				gui->showMotorStatus(MOTOR2, SAME, CLOCKWISE);
				gui->showMotorStatus(MOTOR3, SAME, COUNTERCLOCKWISE);
				gui->showMotorStatus(MOTOR4, SAME, CLOCKWISE);
			}
			/**/ //TODO: this is a test mode for sending only -one- serial command to the mc
			motors->motorControl(MOTOR1, SAME, COUNTERCLOCKWISE);
			motors->motorControl(MOTOR2, SAME, CLOCKWISE);
			motors->motorControl(MOTOR3, SAME, COUNTERCLOCKWISE);
			motors->motorControl(MOTOR4, SAME, CLOCKWISE);
// 			*/
// 			motors->motorControl(ALLMOTORS, SAME, command);
			return;
			break;

		case TURNRIGHT:
			emit message("TURNRIGHT");
			if (!consoleMode)
			{
				gui->showMotorStatus(MOTOR1, SAME, CLOCKWISE);
				gui->showMotorStatus(MOTOR2, SAME, COUNTERCLOCKWISE);
				gui->showMotorStatus(MOTOR3, SAME, CLOCKWISE);
				gui->showMotorStatus(MOTOR4, SAME, COUNTERCLOCKWISE);
			}
			/**/ //TODO: this is a test mode for sending only -one- serial command to the mc
			motors->motorControl(MOTOR1, SAME, CLOCKWISE);
			motors->motorControl(MOTOR2, SAME, COUNTERCLOCKWISE);
			motors->motorControl(MOTOR3, SAME, CLOCKWISE);
			motors->motorControl(MOTOR4, SAME, COUNTERCLOCKWISE);
// 			*/
// 			motors->motorControl(ALLMOTORS, SAME, command);
			return;
			break;

		case START:
			robotDrives = true;
			emit message("Starting to drive forward...");
			emit message("START");
			// set the motors to "drive FORWARD"
			if (!consoleMode)
			{
				gui->showMotorStatus(MOTOR1, ON, CLOCKWISE);
				gui->showMotorStatus(MOTOR2, ON, CLOCKWISE);
				gui->showMotorStatus(MOTOR3, ON, CLOCKWISE);
				gui->showMotorStatus(MOTOR4, ON, CLOCKWISE);
			}
/*	FIXME: to much data over serial port?!?
			motors->setMotorSpeed(MOTOR1, 0); // TODO: check if this works
			motors->setMotorSpeed(MOTOR2, 0); // TODO: check if this works
			motors->setMotorSpeed(MOTOR3, 0); // TODO: check if this works
			motors->setMotorSpeed(MOTOR4, 0); // TODO: check if this works

			resetDrivingSpeedTimer();
			drivingSpeedTimer->start(DRIVINGSPEEDINCREASER); // TODO: put that to a slider in the config menu / file.   TODO 2: make this speed increaser optional!
*/
			/* TODO: this is a test mode for sending only -one- serial command to the mc
			motors->motorControl(MOTOR1, ON, CLOCKWISE);
			motors->motorControl(MOTOR2, ON, CLOCKWISE);
			motors->motorControl(MOTOR3, ON, CLOCKWISE);
			motors->motorControl(MOTOR4, ON, CLOCKWISE);
			*/
			motors->motorControl(ALLMOTORS, SAME, command);
			return;
			break;

		case WAIT:
			emit message("WAIT");
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
			// turning motors off
			/* TODO: this is a test mode for sending only -one- serial command to the mc
			motors->motorControl(MOTOR1, OFF, SAME);
			motors->motorControl(MOTOR2, OFF, SAME);
			motors->motorControl(MOTOR3, OFF, SAME);
			motors->motorControl(MOTOR4, OFF, SAME);
			*/
			motors->motorControl(ALLMOTORS, SAME, command);

			//
			// Don't stop the motThread (PWM)!
			// Only switching motors off!
			//
			return;
			break;

		case STOP:
			emit message("STOP");
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
			// turning motors off
			/* TODO: this is a test mode for sending only -one- serial command to the mc
			motors->motorControl(MOTOR1, OFF, SAME);
			motors->motorControl(MOTOR2, OFF, SAME);
			motors->motorControl(MOTOR3, OFF, SAME);
			motors->motorControl(MOTOR4, OFF, SAME);
			*/
			motors->motorControl(ALLMOTORS, SAME, command);
			//
			// Don't stop the motThread (clock)!
			// Only switching motors off!
			//
			robotDrives = false;
			return;
			break;

		case MOTOR1FW: // for the test widget in the GUI!!
			emit message("Motor 1 forward");
			if (!consoleMode)
			{
				gui->showMotorStatus(MOTOR1, ON, COUNTERCLOCKWISE);
			}
			if (!motors->motorControl(MOTOR1, ON, COUNTERCLOCKWISE))
				emit message("ERROR motor 1 CCW");
			return;
			break;

		case MOTOR1BW: // for the test widget in the GUI!!
			emit message("Motor 1 backward");
			if (!consoleMode)
			{
				gui->showMotorStatus(MOTOR1, ON, CLOCKWISE);
			}
			if (!motors->motorControl(MOTOR1, ON, CLOCKWISE))
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
				gui->showMotorStatus(MOTOR2, ON, COUNTERCLOCKWISE);
			}
			motors->motorControl(MOTOR2, ON, COUNTERCLOCKWISE);
			return;
			break;

		case MOTOR2BW: // for the test widget in the GUI!!
			emit message("Motor 2 backward");
			if (!consoleMode)
			{
				gui->showMotorStatus(MOTOR2, ON, CLOCKWISE);
			}
			motors->motorControl(MOTOR2, ON, CLOCKWISE);
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
				gui->showMotorStatus(MOTOR3, ON, COUNTERCLOCKWISE);
			}
			motors->motorControl(MOTOR3, ON, COUNTERCLOCKWISE);
			return;
			break;

		case MOTOR3BW: // for the test widget in the GUI!!
			emit message("Motor 3 backward");
			if (!consoleMode)
			{
				gui->showMotorStatus(MOTOR3, ON, CLOCKWISE);
			}
			motors->motorControl(MOTOR3, ON, CLOCKWISE);
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
				gui->showMotorStatus(MOTOR4, ON, COUNTERCLOCKWISE);
			}
			motors->motorControl(MOTOR4, ON, COUNTERCLOCKWISE);
			return;
			break;

		case MOTOR4BW: // for the test widget in the GUI!!
			emit message("Motor 4 backward");
			if (!consoleMode)
			{
				gui->showMotorStatus(MOTOR4, ON, CLOCKWISE);
			}
			motors->motorControl(MOTOR4, ON, CLOCKWISE);
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
	int increaseInterval = qRound(maximumSpeed * 0.1); // increase interval is 10%. TODO: start with which minimumSpeed?


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

	// all motors at their end speed? // TODO: check if end speed is refreshed in this class, when changed via gui!!
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
		case -2:
			emit message("<font color=\"#FF0000\">ini-file is not writeable!</font>");
			break;
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
			//-----------------------------
			// first log file message
			//-----------------------------
			logfile->appendLog("- start -------------------------------------------------------------------------------------------------");
			break;
	}


	//---------------------------------------------------------------------
	// read setting
	serialPortMicrocontroller = inifile1->readString("Config", "serialPortMicrocontroller");

	if (serialPortMicrocontroller == "error2")
	{
		emit message("<font color=\"#FF0000\">ini-file is not writeable!</font>");
	}
	else
	{
		if (serialPortMicrocontroller == "error1")
		{
			emit message("<font color=\"#FF0000\">Value \"serialPortMicrocontroller\" not found in ini-file!</font>");
		}
		else
		{
			//
			// everything okay
			//
			emit message(QString("Serial port for microcontroller set to <b>%1</b>.").arg(serialPortMicrocontroller));
		}
	}


	//---------------------------------------------------------------------
	// read first FRONT laser setting
	laserscannerTypeFront = inifile1->readString("Config", "laserscannerTypeFront");

	if (laserscannerTypeFront == "error2")
	{
		laserThread->setType(LASER1, "none");
		emit message("<font color=\"#FF0000\">ini-file is not writeable!</font>");
	}
	else
	{
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
			serialPortLaserscannerFront = inifile1->readString("Config", "serialPortLaserscannerFront");

			if (serialPortLaserscannerFront == "error2")
			{
				laserThread->setSerialPort(LASER1, "none");
				emit message("<font color=\"#FF0000\">ini-file is not writeable!</font>");
			}
			else
			{
				if (serialPortLaserscannerFront == "error1")
				{
					laserThread->setSerialPort(LASER1, "none");
					emit message("<font color=\"#FF0000\">Value \"serialPortLaserscannerFront\" not found in ini-file!</font>");
				}
				else
				{
					// everything okay
					laserThread->setSerialPort(LASER1, serialPortLaserscannerFront);
					emit message(QString("Front laser scanner port set to <b>%1</b>.").arg(serialPortLaserscannerFront));

					//---------------------------------------------------------------------
					// read next laser setting
					laserscannerMounting = inifile1->readString("Config", "laserscannerMountingFront");

					if (laserscannerMounting == "error2")
					{
						laserThread->setMounting(LASER1, "normal");
						emit message("<font color=\"#FF0000\">ini-file is not writeable!</font>");
					}
					else
					{
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
							case -2:
								emit message("<font color=\"#FF0000\">ini-file is not writeable!</font>");
								break;
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
								}
								break;
							}
						}
					}
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
							case -2:
								emit message("<font color=\"#FF0000\">ini-file is not writeable!</font>");
								break;
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
		case -2:
			emit message("<font color=\"#FF0000\">ini-file is not writeable!</font>");
			break;
		case -1:
			emit message("<font color=\"#FF0000\">Value \"useCamera\"not found in ini-file!</font>");
			break;
		case 0:
			useCamera = false;
			if (!consoleMode)
			{
				// turning "off" camera
				camThread->setCameraDevice(-2);
				gui->disableCamera();
			}
			emit message("<font color=\"#FF0000\">No camera usage! (see ini-file)</font>");
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
				camThread->setCameraDevice(-2);
				emit message("<font color=\"#FF0000\">ini-file is not writeable!</font>");
			}
			else
			{
				if (cameraDevice == -1)
				{
					camThread->setCameraDevice(-2);
					emit message("<font color=\"#FF0000\">Value \"cameraDevice\" not found in ini-file!</font>");
				}
				else
				{
					//
					// everything okay
					//
					// set it in the cam thread
					camThread->setCameraDevice(cameraDevice);

					emit message(QString("Camera file set to <b>%1</b>.").arg(cameraDevice));


					//---------------------------------------------------------------------
					// read setting
					QString haarClassifierCascade = inifile1->readString("Config", "haarClassifierCascade");

					if (haarClassifierCascade == "error2")
					{
						camThread->setCascadePath("none");
						emit message("<font color=\"#FF0000\">ini-file is not writeable!</font>");
					}
					else
					{
						if (haarClassifierCascade == "error1")
						{
							camThread->setCascadePath("none");
							emit message("<font color=\"#FF0000\">Value \"haarClassifierCascade\" not found in ini-file!</font>");
						}
						else
						{
							//
							// everything okay
							//
							// set it in the cam thread
							camThread->setCascadePath(haarClassifierCascade);
							emit message(QString("Haar classifier cascade file set to<br><b>%1</b>.").arg(haarClassifierCascade));
							emit splashMessage("Initialising camera...");

							// initialise the cam
							if (camThread->init())
							{
								emit message("Camera initialised.");
							}
							else
							{
								emit message("Error initialising camera.");
							}
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
		case -2:
			emit message("<font color=\"#FF0000\">ini-file is not writeable!</font>");
			break;
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
		case -2:
			emit message("<font color=\"#FF0000\">ini-file is not writeable!</font>");
			break;
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
		case -2:
			emit message("<font color=\"#FF0000\">ini-file is not writeable!</font>");
			break;
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
		case -2:
			emit message("<font color=\"#FF0000\">ini-file is not writeable!</font>");
			break;
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
	int minObstacleDistanceLaserScanner = inifile1->readSetting("Config", "minObstacleDistanceLaserScanner");

	switch (minObstacleDistanceLaserScanner)
	{
		case -2:
			emit message("<font color=\"#FF0000\">ini-file is not writeable!</font>");
			break;
		case -1:
			emit message("<font color=\"#FF0000\">Value \"minObstacleDistanceLaserScanner\"not found in ini-file!</font>");
			break;
		default:
			if (!consoleMode)
			{
				// set slider to the read value
				settingsDialog->setSliderObstacleLaserScannerValue(minObstacleDistanceLaserScanner);
			}

			// tell it the obstacle check thread
			obstCheckThread->setMinObstacleDistanceLaser(minObstacleDistanceLaserScanner);
			// show text
			emit message(QString("Min. obstacle distance Laser Scanner set to <b>%1 cm</b>.").arg(minObstacleDistanceLaserScanner));
			break;
	}

	//---------------------------------------------------------------------
	// read setting
	int robotSlot = inifile1->readSetting("Config", "robotSlot");

	switch (robotSlot)
	{
		case -2:
			emit message("<font color=\"#FF0000\">ini-file is not writeable!</font>");
			break;
		case -1:
			emit message("<font color=\"#FF0000\">Value \"robotSlot\"not found in ini-file!</font>");
			break;
		default:
			if (!consoleMode)
			{
				// set slider to the read value
				settingsDialog->setSliderRobotSlot(robotSlot);
			}

			// tell it the obstacle check thread
			obstCheckThread->setRobotSlot(robotSlot);
			// show text
			emit message(QString("Robot slot set to <b>%1 deg.</b>").arg(robotSlot));
			break;
	}

	//---------------------------------------------------------------------
	// read setting
	int robotSlotWidth = inifile1->readSetting("Config", "robotSlotWidth");

	switch (robotSlotWidth)
	{
		case -2:
			emit message("<font color=\"#FF0000\">ini-file is not writeable!</font>");
			break;
		case -1:
			emit message("<font color=\"#FF0000\">Value \"robotSlotWidth\"not found in ini-file!</font>");
			break;
		default:
			if (!consoleMode)
			{
				// set slider to the read value
				settingsDialog->setSliderRobotSlotWidth(robotSlotWidth);
			}

			// tell it the obstacle check thread
			obstCheckThread->setRobotSlotWidth(robotSlotWidth);
			// show text
			emit message(QString("Robot slot width set to <b>%1 cm.</b>").arg(robotSlotWidth));
			break;
	}

	//---------------------------------------------------------------------
	// read setting
	int straightForwardDeviation = inifile1->readSetting("Config", "straightForwardDeviation");

	switch (straightForwardDeviation)
	{
		case -2:
			emit message("<font color=\"#FF0000\">ini-file is not writeable!</font>");
			break;
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
		case -2:
			emit message("<font color=\"#FF0000\">ini-file is not writeable!</font>");
			mot1Speed = 0;
			break;
		case -1:
			emit message("<font color=\"#FF0000\">Value \"motor1Speed\" not found in ini-file!</font>");
			mot1Speed = 0;
			break;
		default:
			if (mot1Speed > 254)
			{
				emit message("<font color=\"#FF0000\">Value \"motor1Speed\" is greater than 255!! Value set to 255!</font>");
				mot1Speed = 255;
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
		case -2:
			emit message("<font color=\"#FF0000\">ini-file is not writeable!</font>");
			mot2Speed = 0;
			break;
		case -1:
			emit message("<font color=\"#FF0000\">Value \"motor2Speed\" not found in ini-file!</font>");
			mot2Speed = 0;
			break;
		default:
			if (mot2Speed > 254)
			{
				emit message("<font color=\"#FF0000\">Value \"motor2Speed\" is greater than 255!! Value set to 255!</font>");
				mot2Speed = 255;
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
		case -2:
			emit message("<font color=\"#FF0000\">ini-file is not writeable!</font>");
			mot3Speed = 0;
			break;
		case -1:
			emit message("<font color=\"#FF0000\">Value \"motor3Speed\" not found in ini-file!</font>");
			mot3Speed = 0;
			break;
		default:
			if (mot3Speed > 254)
			{
				emit message("<font color=\"#FF0000\">Value \"motor3Speed\" is greater than 255!! Value set to 255!</font>");
				mot3Speed = 255;
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
		case -2:
			emit message("<font color=\"#FF0000\">ini-file is not writeable!</font>");
			mot4Speed = 0;
			break;
		case -1:
			emit message("<font color=\"#FF0000\">Value \"motor4Speed\" not found in ini-file!</font>");
			mot4Speed = 0;
			break;
		default:
			if (mot4Speed > 254)
			{
				emit message("<font color=\"#FF0000\">Value \"motor4Speed\" is greater than 255!! Value set to 255!</font>");
				mot4Speed = 255;
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
		case -2:
			emit message("<font color=\"#FF0000\">ini-file is not writeable!</font>");
			minimumSpeed = 0;
			break;
		case -1:
			emit message("<font color=\"#FF0000\">Value \"minimumSpeed\" not found in ini-file!</font>");
			minimumSpeed = 0;
			break;
		default:
			if (minimumSpeed > 254)
			{
				emit message("<font color=\"#FF0000\">Value \"minimumSpeed\" is greater than 255!! Value set to 255!</font>");
				minimumSpeed = 255;
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
		case -2:
			emit message("<font color=\"#FF0000\">ini-file is not writeable!</font>");
			maximumSpeed = 0;
			break;
		case -1:
			emit message("<font color=\"#FF0000\">Value \"maximumSpeed\" not found in ini-file!</font>");
			maximumSpeed = 0;
			break;
		default:
			if (maximumSpeed > 254)
			{
				emit message("<font color=\"#FF0000\">Value \"maximumSpeed\" is greater than 255!! Value set to 255!</font>");
				maximumSpeed = 255;
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
			case -2:
				emit message("<font color=\"#FF0000\">ini-file is not writeable!</font>");
				settingValue = 0;
				break;
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
			case -2:
				emit message("<font color=\"#FF0000\">ini-file is not writeable!</font>");
				settingValue = 0;
				break;
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
			case -2:
				emit message("<font color=\"#FF0000\">ini-file is not writeable!</font>");
				settingValue = 0;
				break;
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
			case -2:
				emit message("<font color=\"#FF0000\">ini-file is not writeable!</font>");
				settingValue = 0;
				break;
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
			case -2:
				emit message("<font color=\"#FF0000\">ini-file is not writeable!</font>");
				settingValue = 0;
				break;
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
	// read setting
	value = inifile1->readSetting("Config", "networkPort");

	switch (value)
	{
		case -2:
			emit message("<font color=\"#FF0000\">ini-file is not writeable!</font>");
			value = 0;
			break;
		case -1:
			emit message("<font color=\"#FF0000\">Value \"networkPort\" not found in ini-file!</font>");
			value = 0;
			break;
		default:
			// set value in networkThread
			netThread->setPort(value);
			// show text
			emit message(QString("Network port to <b>%1</b>.").arg(value));
			break;
	}
}


void Direcs::enableRemoteControlListening(bool state)
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
			emit message("Starting network thread...", false);
			if (!consoleMode)
			{
				gui->appendNetworkLog("Starting network thread...");
			}
			netThread->start();
			emit message("Network thread started.");
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


void Direcs::executeRemoteCommand(QString command)
{
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
						drive(START);
					}

					drive(BACKWARD);
//				}
			}


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
					drive(START);
				}

				drive(FORWARD);
			}


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
					drive(START);
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
					drive(START);
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
				// TODO: check if this makes sense...
				drive(START);
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
				// TODO: check if this makes sense...
				drive(START);
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
//				servo1Pos++;
			}

			//------------------
			// servo left
			//------------------
			if (axisValue < 0)
			{
//				servo1Pos--;
			}

			// only move, when button is pressed - not, when released (=0)
			if (axisValue != 0)
			{
//				servos->moveServo(SERVO1, servo1Pos);
//				emit message(QString("Servo 1 moved to %1.").arg(servo1Pos));
			}
			return;
		} // servo test mode

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
				// do a right turn in a circle!
				drive(TURNRIGHT);
			}
			else
			{
				if (robotDrives == false)
				{
					// TODO: check if this makes sense...
					drive(START);
				}

				drive(RIGHT);
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
				// do a right turn in a circle!
				drive(TURNLEFT);
			}
			else
			{
				if (robotDrives == false)
				{
					// TODO: check if this makes sense...
					drive(START);
				}

				drive(LEFT);
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
	static bool toggle4 = false;
	//static bool toggle5 = false;
	static bool toggle10 = false;
	static bool toggle11 = false;


	// TODO: put button numbers to ini-file
	switch (buttonNumber)
	{
		case 0: // 1 on js
			if (buttonState==true)
			{
				if (toggle0 == false)
				{
					eyeTestMode=true;
					emit message("<font color=\"#0000FF\">Eye test mode ON.</front>");
					emit speak("Eye test mode.");
				}
				else
				{
					eyeTestMode=false;
					head->look("FORWARD");
					head->look("NORMAL");
					emit message("<font color=\"#0000FF\">Eye test mode OFF.</front>");
					emit speak("Eye test mode disabled.");
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
			if (buttonState==true)
			{
				if (toggle4 == false)
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
				toggle4 = !toggle4;
			}
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
					testDriveMode = true;
					emit message("<font color=\"#0000FF\">Test drive mode ON.</front>");
					emit speak("Test drive mode.");
				}
				else
				{
					testDriveMode = false;
					emit message("<font color=\"#0000FF\">Test drive mode OFF.</front>");
					emit speak("Test drive mode disabled.");
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
					cameraTestMode = true;
					emit message("<font color=\"#0000FF\">Camera test mode ON.</front>");
					emit speak("Camerea test mode.");
				}
				else
				{
					cameraTestMode = false;
					emit message("<font color=\"#0000FF\">Camera test mode OFF.</front>");
					emit speak("Camerea test mode disabled.");
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


void Direcs::test()
{
	static bool toggle = OFF;


	if (toggle == OFF)
	{
		toggle = ON;
		//head->look("LEFT");
		emit sendNetworkString("ON");
#ifdef Q_OS_LINUX // currently supported only under linux (no MAC OS at the moment)
		speakThread->setLanguage("en");
		//speakThread->setVoice(1, 200); // 1=male, 'age'=255
		// Say some text;
		QDateTime now = QDateTime::currentDateTime();
		emit speak(tr("Hello Markus. Today it's the %1 of %2, %3. The time is %4 %5.").arg(now.toString("d")).arg(now.toString("MMMM")).arg(now.toString("yyyy")).arg(now.toString("h")).arg(now.toString("m")));
#endif
	}
	else
	{
		toggle = OFF;
		emit sendNetworkString("OFF");

#ifdef Q_OS_LINUX // currently supported only under linux (no MAC OS at the moment)
		speakThread->setLanguage("de");
		//speakThread->setVoice(2, 5); // 2=female, 'age'=5
		// Say some text;
		QDateTime now = QDateTime::currentDateTime();
		emit speak(tr("und das ganze geht auch auf Deutsch. Heute ist der %1te. %2, %3. Es ist jetzt %4 Uhr %5.").arg(now.toString("d")).arg(now.toString("MMMM")).arg(now.toString("yyyy")).arg(now.toString("h")).arg(now.toString("m")));
#endif
	}
		//head->look("RIGHT");


	motors->flashlight(toggle);
}
