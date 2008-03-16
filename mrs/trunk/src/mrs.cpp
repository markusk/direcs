#include "mrs.h"


QSplashScreen *splash;
Qt::Alignment somewhere = Qt::AlignHCenter | Qt::AlignBottom;
QColor splashColor = Qt::red;

int main(int argc, char *argv[])
{
	
	// initialize the resource file
	Q_INIT_RESOURCE(mrs);

	QApplication app(argc, argv);

	//----------------------
	// the splash screen
	//----------------------
	splash = new QSplashScreen();
	splash->setPixmap(QPixmap(":/images/images/splash.png"));
	splash->show();
	splash->showMessage(QObject::tr("Loading config file..."), somewhere, splashColor);
	
	// create Mrs class object
	Mrs *m = new Mrs();
	
	return app.exec();
}


/*!
\brief The main class of the whole robot programm

All objects are created here. This method also starts the GUI.
*/
Mrs::Mrs()
{
	//------------------------------------------------------------------
	// create the objects
	//------------------------------------------------------------------
	#ifdef _TTY_POSIX_
	//speakThread = new SpeakThread();
	#endif
	interface1 = new InterfaceAvr();
	circuit1 = new Circuit(interface1);
	motors = new Motor(interface1);
	servos = new Servo(interface1);
	sensorThread = new SensorThread(interface1);
	laserThread = new LaserThread();
	obstCheckThread = new ObstacleCheckThread(sensorThread, laserThread);
	plotThread = new PlotThread(sensorThread);
	inifile1 = new Inifile();
	netThread = new NetworkThread();
	camThread = new CamThread();
	joystick = new Joystick();
	
	gui = new Gui();

	
	
	//------------------------------------------------------------------
	// nomen est omen
	//------------------------------------------------------------------
	serialPortMicrocontroller = "error1";
	serialPortLaserscannerFront = "error1";
	robotDrives = false;
	mot1Speed = 0;
	mot2Speed = 0;
	mot3Speed = 0;
	mot4Speed = 0;
	robotSimulationMode = false;
	robotRemoteMode = false;
	servoTestMode = false;
	currentTestServo = SERVO1;
	cameraTestMode = false;
	faceTrackingIsEnabled = false;
	
	//------------------------------------------------------------------
	// Set the number format to "," for comma and 1000 separator to "."
	// For example: 1.234,00 EUR
	//------------------------------------------------------------------
	QLocale::setDefault(QLocale::German);
	commaSeparator = ",";

	
	//--------------------------------------------------------------------------
	// Check for the current programm path 
	//--------------------------------------------------------------------------
	gui->appendLog(QString("Current path: %1").arg(inifile1->checkPath()));
	
	//--------------------------------------------------------------------------
	// show a QMessage wth the possibility to exit the main programm, when errors occured!
	//--------------------------------------------------------------------------
	connect(interface1, SIGNAL(tooMuchErrors()), this, SLOT(showExitDialog()));
	
	//--------------------------------------------------------------------------
	// shutdown Mrs program on exit button
	//--------------------------------------------------------------------------
	connect(gui, SIGNAL(shutdown()), this, SLOT(shutdown()));
	// FIXME: check, how to make a clean exit, when the window is closed via window menu, not via exit button!
	//connect(this, SIGNAL(QApplication::lastWindowClosed()), this, SLOT(shutdown()));
	
	//--------------------------------------------------------------------------
	// call the test method at the test button
	//--------------------------------------------------------------------------
	connect(gui, SIGNAL(test()), this, SLOT(test()));
	
	//--------------------------------------------------------------------------
	// set the motor speed, when signal comes from Gui
	//--------------------------------------------------------------------------
	connect(gui, SIGNAL(setMotorSpeed(int, int)), motors, SLOT(setMotorSpeed(int, int)));
	
	//--------------------------------------------------------------------------
	// resets the driven distance, when signal comes from Gui
	//--------------------------------------------------------------------------
	connect(gui, SIGNAL(resetDrivenDistance(int)), sensorThread, SLOT(resetDrivenDistance(int)));
	
	//--------------------------------------------------------------------------
	// set the robot slot, when signal comes from Gui
	//--------------------------------------------------------------------------
	connect(gui, SIGNAL(setRobotSlot(int)), obstCheckThread, SLOT(setRobotSlot(int)));
	
	//--------------------------------------------------------------------------
	// set the straight forward deviation, when signal comes from Gui
	//--------------------------------------------------------------------------
	connect(gui, SIGNAL(setStraightForwardDeviation(int)), obstCheckThread, SLOT(setStraightForwardDeviation(int)));
	
	//--------------------------------------------------------------------------
	// set the minimum distance, when signal comes from Gui
	//--------------------------------------------------------------------------
	connect(gui, SIGNAL(setMinObstacleDistance(int)), obstCheckThread, SLOT(setMinObstacleDistance(int)));
	
	//--------------------------------------------------------------------------
	// set the minimum laser distance, when signal comes from Gui
	//--------------------------------------------------------------------------
	connect(gui, SIGNAL(setMinObstacleDistanceLaser(int)), obstCheckThread, SLOT(setMinObstacleDistanceLaser(int)));
	
	//--------------------------------------------------------------------------
	// let the GUI show servo messages in the log
	//--------------------------------------------------------------------------
	connect(servos, SIGNAL(message(QString)), gui, SLOT(appendLog(QString)));
	
	
	//----------------------------------------------------------------------------
	// say a text
	//----------------------------------------------------------------------------
	#ifdef _TTY_POSIX_
	// FIXME: SIOD ERROR: the currently assigned stack limit has been exceded
	//connect(this, SIGNAL( speak(QString) ), speakThread, SLOT( speak(QString) ));
	//connect(gui, SIGNAL( speak(QString) ), speakThread, SLOT( speak(QString) ));
	connect(gui, SIGNAL( speak(QString) ), this, SLOT( speak(QString) ));
	
	//----------------------------------------------------------------------------
	// Initialize the speech engine festival
	//----------------------------------------------------------------------------
	//      1 = we want the festival init files loaded
	// 500000 = default scheme heap size
	festival_initialize(1, 500000);
	
	// FIXME: SIOD ERROR: the currently assigned stack limit has been exceded
	/*
	if (speakThread->isRunning() == false)
	{
		gui->appendLog("Starting speak thread...", false);
		speakThread->start();
		gui->appendLog("Speak thread started.");
	}
	*/
	#endif
	
	
	//--------------------------------------------------------------------------
	// Check for the programm ini file
	// AND read the settings ! ! !
	//--------------------------------------------------------------------------
	if (inifile1->checkFiles() == false)
	{
		// file not found-Msg
		gui->appendLog(QString("<b><font color=\"#FF0000\">File '%1' not found!</font></b>").arg(inifile1->getInifileName()));
	}
	else
	{
		// file found-Msg
		gui->appendLog(QString("Using ini-File \"%1\".").arg(inifile1->getInifileName()));
		
		splash->showMessage(QObject::tr("Reading settings..."), somewhere, splashColor);
		
		//================================================================================================================================================================
		// read all settings
		//================================================================================================================================================================
		readSettings();
	}


	//-------------------------------------------------------
	// Open serial port for microcontroller communication
	//-------------------------------------------------------
	gui->appendLog("Opening serial port for microcontroller communication...");
	
	if (interface1->openComPort(serialPortMicrocontroller) == true)
	{
		gui->appendLog("Serial port opened.");
	}
	else
	{
		gui->appendLog(QString("<font color=\"#FF0000\">Error opening serial port '%1'!</font>").arg(serialPortMicrocontroller));
	}
	
	
	//-------------------------------------------------------
	// Basic init for all the bits on the robot circuit
	// AND check, if the robot is "on" (it answers correct)
	//-------------------------------------------------------
	splash->showMessage(QObject::tr("Searching robot..."), somewhere, splashColor);
	
	// init the circuit & Co. when hitting the button in the GUI
	connect(gui, SIGNAL( initCircuit() ), circuit1, SLOT( initCircuit() ) );
	connect(gui, SIGNAL( initServos() ), servos, SLOT( init() ) );
	
	
	// init the robots circuit
	circuit1->initCircuit();
	
	if (circuit1->isConnected() == true)
	{
		gui->appendLog("Robot is ON and answers.");
	}
	else
	{
		gui->appendLog("<font color=\"#FF0000\">The robot is OFF! Please turn it ON!</font>");
	}

	
	//-------------------------------------------------------
	// set the read motor speed
	//-------------------------------------------------------
	if (circuit1->isConnected())
	{
		// tell the microcontroller the motor speed
		motors->setMotorSpeed(1, mot1Speed);
		motors->setMotorSpeed(2, mot2Speed);
		motors->setMotorSpeed(3, mot3Speed);
		motors->setMotorSpeed(4, mot4Speed);
		gui->appendLog("Motor speed set in microcontroller");
	}
	
	
	//-------------------------------------------------------
	// move all servos in their default positions
	//-------------------------------------------------------
	if (circuit1->isConnected())
	{
		servos->init();
		gui->appendLog("Servos moved to default positions");
	}

	
	//-----------------------------------------------------------
	// start the sensor thread ("clock" for reading the sensors)
	//-----------------------------------------------------------
	if (circuit1->isConnected())
	{
		if (sensorThread->isRunning() == false)
		{
			splash->showMessage(QObject::tr("Starting sensor thread..."), somewhere, splashColor);
			gui->appendLog("Starting sensor thread...", false);
			sensorThread->start();
			gui->appendLog("Sensor thread started.");
		}
	}
	else
	{
		// show message
		gui->appendLog("Sensor thread NOT started!");
	}


	//-----------------------------------------------------------
	// start the plot thread ("clock" for plotting the curves)
	//-----------------------------------------------------------
	if (circuit1->isConnected())
	{
		if (plotThread->isRunning() == false)
		{
			splash->showMessage(QObject::tr("Starting plot thread..."), somewhere, splashColor);
			gui->appendLog("Starting plot thread...", false);
			plotThread->start();
			gui->appendLog("Plot thread started.");
		}
	}
	else
	{
		// show message
		gui->appendLog("Plot thread NOT started!");
	}

	//----------------------------------------------------------------------------
	// connect plotThread signal to "setPlotData"
	// (Whenever the plot thread has new data, the data are show in the GUI)
	//----------------------------------------------------------------------------
	connect(plotThread, SIGNAL( plotDataComplete1(double *, double *, int) ), gui, SLOT( setPlotData1(double *, double *, int) ));
	connect(plotThread, SIGNAL( plotDataComplete2(double *, double *, int) ), gui, SLOT( setPlotData2(double *, double *, int) ));

	
	//-----------------------------------------------------------
	// check if joystick is connected
	//-----------------------------------------------------------
	// let the GUI show messages in the log
	connect(joystick, SIGNAL(emitMessage(QString)), gui, SLOT(appendLog(QString)));
	
	if (joystick->isConnected())
	{
		// start the joystick thread
		if (joystick->isRunning() == false)
		{
			splash->showMessage(QObject::tr("Starting joystick thread..."), somewhere, splashColor);
			gui->appendLog("Starting joystick thread...", false);
			joystick->start();
			gui->appendLog("Joystick thread started.");
		}
	}
	else
	{
		gui->appendLog("Joystick thread NOT started!");
	}

	//----------------------------------------------------------------------------
	// let the GUI show messages in the log (e.g. when special buttons pressed)
	//----------------------------------------------------------------------------
	connect(joystick, SIGNAL(emitMessage(QString)), gui, SLOT(appendLog(QString)));
	
	//----------------------------------------------------------------------------
	// drive in the direction which was emited
	//----------------------------------------------------------------------------
	connect(gui, SIGNAL( drive(unsigned char) ), this, SLOT( drive(unsigned char) ));
	
	//----------------------------------------------------------------------------
	// connect sensor signals to "show sensor data"
	// (Whenever the sensor data are completely read, show the result in the GUI)
	//----------------------------------------------------------------------------
	connect(sensorThread, SIGNAL(sensorDataComplete()), this, SLOT(showSensorData()));
	
	//----------------------------------------------------------------------------
	// connect sensor contact signals to "show contact alarm"
	// (Whenever the an alarm contact was closed, show the result in the cam image)
	//----------------------------------------------------------------------------
	connect(sensorThread, SIGNAL(contactAlarm(char, bool)), camThread, SLOT(drawContactAlarm(char, bool)));
	
	//----------------------------------------------------------------------------
	// connect camDataComplete from the cam thread to signal "setCamImage"
	// (Whenever the image is complete, the image is shown in the GUI)
	//----------------------------------------------------------------------------
	connect(camThread, SIGNAL( camDataComplete(IplImage*, int, int, int) ), gui, SLOT( setCamImage(IplImage*, int, int, int) ));
	
	//----------------------------------------------------------------------------
	// connect camDataComplete from the cam thread to the faceTracking unit
	//----------------------------------------------------------------------------
	connect(camThread, SIGNAL( camDataComplete(IplImage*, int, int, int) ), SLOT( faceTracking(IplImage*, int, int, int) ));
	
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
	
	//----------------------------------------------------------------------------
	// connect obstacle check (alarm!) sensor signal to "logical unit"
	//----------------------------------------------------------------------------
	connect(obstCheckThread, SIGNAL(obstacleDetected(int, QDateTime)), SLOT(logicalUnit(int, QDateTime)));
	
	//----------------------------------------------------------------------------
	// show the angle where to drive in a GUI label
	//----------------------------------------------------------------------------
	connect(obstCheckThread, SIGNAL(newDrivingAngleSet(int, int, int)), gui, SLOT(showLaserFrontAngles(int, int, int)));
	
	//----------------------------------------------------------------------------
	// show the preferred driving direction in a GUI label
	//----------------------------------------------------------------------------
	connect(this, SIGNAL(showPreferredDirection(QString)), gui, SLOT(showPreferredDirection(QString)));
	
	//----------------------------------------------------------------------------
	// connect remote control button from gui to remote control method here
	// (Whenever the button is pushed, enable the network remote control)
	//----------------------------------------------------------------------------
	connect(gui, SIGNAL( enableRemoteControlListening(bool) ), this, SLOT( enableRemoteControlListening(bool) ));
	
	//----------------------------------------------------------------------------
	// execute the received remote commands
	//----------------------------------------------------------------------------
	connect(netThread, SIGNAL( dataReceived(QString) ), this, SLOT( executeRemoteCommand(QString) ));
	//----------------------------------------------------------------------------
	// connect networkThread signal to "dataReceived"
	// (Whenever data were received, the data are shown in the GUI)
	//----------------------------------------------------------------------------
	connect(netThread, SIGNAL( dataReceived(QString) ), gui, SLOT( appendNetworkLog(QString) ));
	
	//----------------------------------------------------------------------------
	// connect laserThread signal to "dataReceived"
	// (Whenever data were received, the data are shown in the GUI)
	//----------------------------------------------------------------------------
	connect(laserThread, SIGNAL( laserDataCompleteFront(float *, int *) ), gui, SLOT( refreshLaserViewFront(float *, int *) ));
	connect(laserThread, SIGNAL( laserDataCompleteRear(float *, int *) ), gui, SLOT( refreshLaserViewRear(float *, int *) ));
	
	//----------------------------------------------------------------------------
	// connect joystick signals to "show joystick data"
	// (Whenever the joystick is moved or a button is pressed, show the result in the GUI)
	//----------------------------------------------------------------------------
	connect(joystick, SIGNAL(joystickMoved(int, int)), gui, SLOT(showJoystickAxes(int, int)));
	connect(joystick, SIGNAL(joystickMoved(int, int)), this, SLOT(executeJoystickCommand(int, int)));
	
	connect(joystick, SIGNAL(joystickButtonPressed(int, bool)), gui, SLOT(showJoystickButtons(int, bool)));
	connect(joystick, SIGNAL(joystickButtonPressed(int, bool)), this, SLOT(executeJoystickCommand(int, bool)));

	
	//-----------------------------------------------------------
	// check if camera is connected
	//-----------------------------------------------------------
	if (camThread->isConnected())
	{
		if (camThread->isRunning() == false)
		{
			gui->appendLog("Starting camera thread...", false);
			camThread->start();
			gui->appendLog("Camera thread started.");
			if (camThread->isConnected())
				gui->appendLog(QString("Camera resolution is %1x%2.").arg(camThread->imageWidth()).arg(camThread->imageHeight()));
		}
	}
	else
	{
		gui->appendLog("Camera thread NOT started!");
	}

	
	//----------------------------------------------------------------------------
	// connect simulation button from gui to activate the simulation mode
	// (sets the mrs an the threads to simulation mode)
	//----------------------------------------------------------------------------
	connect(gui, SIGNAL( simulate(bool) ), this, SLOT( setSimulationMode(bool) ));
	connect(gui, SIGNAL( simulate(bool) ), sensorThread, SLOT( setSimulationMode(bool) ));
	connect(gui, SIGNAL( simulate(bool) ), laserThread, SLOT( setSimulationMode(bool) ));
	connect(gui, SIGNAL( simulate(bool) ), obstCheckThread, SLOT( setSimulationMode(bool) ));


	//---------------------------------------------------------------------
	// check if laser scanners are connected
	//---------------------------------------------------------------------
	// check FRONT laser
	splash->showMessage(QObject::tr("Searching front laser..."), somewhere, splashColor);
	bool scanner1found = laserThread->isConnected(LASER1);
	
	// check REAR laser
	splash->showMessage(QObject::tr("Searching rear laser..."), somewhere, splashColor);
	bool scanner2found = laserThread->isConnected(LASER2);
	
	if (scanner1found || scanner2found)
	{
		if (scanner1found)
		{
			gui->appendLog("Front laser scanner found.");
		}
		else
		{
			gui->appendLog("Front laser scanner NOT found.");
		}
	
		if (scanner2found)
		{
			gui->appendLog("Rear laser scanner found.");
		}
		else
		{
			gui->appendLog("Rear laser scanner NOT found.");
		}

		
		// TODO: nice exit point and error message
		if (!QGLFormat::hasOpenGL())
		{
			qDebug() << "This system has no OpenGL support" << endl;
			showExitDialog();
		}
		
		
		// start the laserThread
		if (laserThread->isRunning() == false)
		{
			splash->showMessage(QObject::tr("Starting Laser thread..."), somewhere, splashColor);
			gui->appendLog("Starting Laser thread...", false);
			laserThread->start();
			gui->appendLog("Laser thread started.");
		}
		

		if (obstCheckThread->isRunning() == false)
		{
			splash->showMessage(QObject::tr("Starting obstacle check thread..."), somewhere, splashColor);
			gui->appendLog("Starting obstacle check thread...", false);
			obstCheckThread->start();
			gui->appendLog("Obstacle check thread started.");
		}
	}
	else
	{
		// turn off laser splash
		gui->laserSplash(false, LASER1);
		gui->laserSplash(false, LASER2);
		gui->appendLog("<font color=\"#FF0000\">NO laser scanners found! Thread NOT started!</font>");
	}
	
	
	//------------------------------------------------------------------
	// for getting the screen resolution
	//------------------------------------------------------------------
	QDesktopWidget *desktop = QApplication::desktop();

	//------------------------------------------------------------------
	// place gui window at a nice position on the screen
	//------------------------------------------------------------------
	/*
	if (desktop->width() > 1024)
	{
	*/
		// move mainWindow to the center of the screen
		gui->move( (desktop->width() - gui->width())/2, (desktop->height() - gui->height())/2 );

		// show the main window
		gui->show();
		
		// delete the splash screen
		QTimer::singleShot(SPLASHTIME, this, SLOT( finishSplash() ));
	/*
	}
	else
	{
		// resolution too smal for this window. Maximizing...
		// show the main window
		gui->showMaximized();
	}
	*/
}


void Mrs::shutdown()
{
	qDebug("Mrs shutdown...");
	// FIXME: not called, when closed via KDE GUI!!
	
	/*
	if (ClientSocket1->Active == true)
	{
	ClientSocket1->Close();
	ClientSocket1->Active = false;
	}
	*/

	// TODO: put this to a saveSetings()
	//---------------------------------------------------------------
	// save changes to ini-file (if check box is checked!)
	//---------------------------------------------------------------
	if (gui->getCheckBoxSaveSettings()== Qt::Checked)
	{
		gui->appendLog("Writing settings...");

		// save gui slider values
		inifile1->writeSetting("Config", "motor1Speed", gui->getSliderMotorSpeed(1));
		inifile1->writeSetting("Config", "motor2Speed", gui->getSliderMotorSpeed(2));
		inifile1->writeSetting("Config", "minimumSpeed", gui->getSliderMinimumSpeed());
		inifile1->writeSetting("Config", "maximumSpeed", gui->getSliderMaximumSpeed());
		inifile1->writeSetting("Config", "minObstacleDistance", gui->getSliderObstacleValue());
		inifile1->writeSetting("Config", "minObstacleDistanceLaserScanner", gui->getSliderObstacleLaserScannerValue());
		inifile1->writeSetting("Config", "robotSlot", gui->getSliderRobotSlotValue());
		inifile1->writeSetting("Config", "straightForwardDeviation", gui->getSliderStraightForwardDeviationValue());

		// save check box status
		inifile1->writeSetting("Config", "saveOnExit", gui->getCheckBoxSaveSettings());

		// Later...
		//
		//noHardwareErrorMessages 
		//exitDialog 

		// force writing *immediately*
		inifile1->sync();

		//QMessageBox::information(0, "mrs", "Settings written. :-)", QMessageBox::Ok);
		gui->appendLog("Settings written.");
	}
	else
	{
		// THIS SETTING HAS TO BE SAVED ALWAYS!
		// "Save the setting, that no settings shoud be saved"
		//
		// save check box status
		inifile1->writeSetting("Config", "saveOnExit", gui->getCheckBoxSaveSettings());
	}


	// show dialog if set in ini-file
	if (exitDialog == true)
	{
		// ask user if he really wants to exit.
		if (QMessageBox::question(0, "Leaving program...", "Are you sure?", QMessageBox::Yes | QMessageBox::Default, QMessageBox::No | QMessageBox::Escape) == QMessageBox::No)
		{
			//---------
			// if NO
			//---------
			// don't leave! :-)
			return;
		}
	}

	
	// TODO: a universal quit-threads-method
	//--------------------------------
	// quit the camThread
	//--------------------------------
	if (camThread->isRunning() == true)
	{
		gui->appendLog("Stopping camera thread...");
		
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
			gui->appendLog("Camera thread stopped.");
		}
		else
		{
			gui->appendLog("Terminating camera thread because it doesn't answer...");
			camThread->terminate();
			camThread->wait(1000);
			gui->appendLog("Camera thread terminated.");
		}
	}
	
	
	//--------------------------------
	// quit the laserThread
	//--------------------------------
	if (laserThread->isRunning() == true)
	{
		gui->appendLog("Stopping laser thread...");
		
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
			gui->appendLog("Laser thread stopped.");
		}
		else
		{
			gui->appendLog("Terminating laser thread because it doesn't answer...");
			laserThread->terminate();
			laserThread->wait(1000);
			gui->appendLog("Laser thread terminated.");
		}
	}
	
	
#ifdef _TTY_POSIX_
	/*
	//--------------------------------
	// quit the speakThread
	//--------------------------------
	if (speakThread->isRunning() == true)
	{
	gui->appendLog("Stopping speak thread...");
		
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
	gui->appendLog("Speak thread stopped.");
}
	else
	{
	gui->appendLog("Terminating speak thread because it doesn't answer...");
	speakThread->terminate();
	speakThread->wait(1000);
	gui->appendLog("Speak thread terminated.");
}
}
	*/
#endif
	
	//--------------------------------
	// quit the network thread
	//--------------------------------
	if (netThread->isRunning() == true)
	{
		gui->appendLog("Stopping network thread...");
		
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
			gui->appendLog("Network thread stopped.");
		}
		else
		{
			gui->appendLog("Terminating network thread because it doesn't answer...");
			netThread->terminate();
			netThread->wait(1000);
			gui->appendLog("Network thread terminated.");
		}
	}

	
	//--------------------------------
	// quit the joystick thread
	//--------------------------------
	if (joystick->isRunning() == true)
	{
		gui->appendLog("Stopping joystick thread...");
		
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
			gui->appendLog("Joystick thread stopped.");
		}
		else
		{
			gui->appendLog("Terminating joystick thread because it doesn't answer...");
			joystick->terminate();
			joystick->wait(1000);
			gui->appendLog("Joystick thread terminated.");
		}
	}

	
	//--------------------------------
	// quit the plotThread
	//--------------------------------
	if (plotThread->isRunning() == true)
	{
		gui->appendLog("Stopping Plot thread...");
		
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
			gui->appendLog("Plot thread stopped.");
		}
		else
		{
			gui->appendLog("Terminating Plot thread because it doesn't answer...");
			plotThread->terminate();
			plotThread->wait(1000);
			gui->appendLog("Plot thread terminated.");
		}
	}



	//--------------------------------
	// quit the obstacle check thread
	//--------------------------------
	//qDebug("Starting to stop the obstacle check thread NOW!");
	if (obstCheckThread->isRunning() == true)
	{
		gui->appendLog("Stopping obstacle check thread...");
		
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
			gui->appendLog("Obstacle check thread stopped.");
		}
		else
		{
			gui->appendLog("Terminating obstacle check thread because it doesn't answer...");
			obstCheckThread->terminate();
			obstCheckThread->wait(1000);
			gui->appendLog("Obstacle check thread terminated.");
		}
	}


	/*
	//-----------------------------
	// quit the supersonic thread
	//-----------------------------
	//qDebug("Starting to stop the supersonic thread NOW!");
	if (supersonThread->isRunning() == true)
	{
	gui->appendLog("Stopping supersonic thread...");
		// slowing thread down
	supersonThread->setPriority(QThread::IdlePriority);
	supersonThread->quit();
		
		//-------------------------------------------
		// start measuring time for timeout ckecking
		//-------------------------------------------
	QTime t;
	t.start();
	do
	{
} while ((supersonThread->isFinished() == false) && (t.elapsed() <= 2000));

	if (supersonThread->isFinished() == true)
	{
	gui->appendLog("Supersonic thread stopped.");
}
	else
	{
	gui->appendLog("Terminating supersonic thread because it doesn't answer...");
	supersonThread->terminate();
	supersonThread->wait(1000);
	gui->appendLog("Supersonic thread terminated.");
}
}
	*/
	
	// Todo: CamThread sauber beenden!!

	//--------------------------
	// quit the sensor thread
	//--------------------------
	//qDebug("Starting to stop the sensor thread NOW!");
	if (sensorThread->isRunning() == true)
	{
		gui->appendLog("Stopping sensor thread...");
		
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
			gui->appendLog("Sensor thread stopped.");
		}
		else
		{
			gui->appendLog("Terminating sensor thread because it doesn't answer...");
			sensorThread->terminate();
			sensorThread->wait(1000);
			gui->appendLog("Sensor thread terminated.");
		}
	}


	//-------------------------------------------------------
	// Last init for the robots circuits
	//-------------------------------------------------------
	gui->appendLog("Last circuit init...");
	if (circuit1->isConnected())
	{
		emit initCircuit();
	}

	//-----------------------------
	// close serial port to mc
	//-----------------------------
	gui->appendLog("Closing serial port to microcontroller...");
	interface1->closeComPort();

	// close the gui
	gui->close();

	
	this->~Mrs();
}


Mrs::~Mrs()
{
	qDebug("Bye.");
	
	//--------------------------------------------------
	// clean up in reverse order (except from the gui)
	//--------------------------------------------------
	#ifdef _TTY_POSIX_
	//delete speakThread;
	#endif
	delete laserThread;
	delete netThread;
	delete camThread;
	delete joystick;
	delete plotThread;
	delete inifile1;
	delete obstCheckThread;
	delete servos;
	delete motors;
	delete sensorThread;
	delete circuit1;
	delete interface1;
	delete gui;
}


void Mrs::showExitDialog()
{
		gui->appendLog("<font color=\"#FF0000\">THERE IS A BIG COMMUNICATION PROBLEM WITH THE SERIAL PORT TO THE ROBOT!</font>");
		
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


void Mrs::finishSplash()
{
	splash->finish(gui);
}


void Mrs::logicalUnit(int sensorAlarm, QDateTime timestamp)
{
	//TODO (in progress!): Check if there is only one short "alarm signal" this false alarm will be ignored!
	
	static int alarmCounter = 0;
	static short int lastSensorValue = -1; // < the initial value has to be different to ALL SENSOR-constants!!


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
	
	
	// if all sensor values are the same like the last, do nothing!
	if (sensorAlarm == lastSensorValue)
	{
		// store this sensor alarm value
		lastSensorValue = sensorAlarm;
		alarmCounter++;
		
		// FIXME: check this method in general !! < < < < < < < <
		return;
	}


	if (sensorAlarm == NONE)
	{
		emit showPreferredDirection("FORWARD");
		
		// TODO: use *one* list and make a "count" in the list?!
		// clear alarm buffers
		obstacleAlarmFrontLeftList.clear();
		obstacleAlarmFrontRightList.clear();
		
		if (robotDrives == true)
		{
			gui->appendLog("No obstacle in front of any sensor. :-)");
			gui->appendLog("Driving forward...");
			
			//----------------
			// drive forward
			//----------------
			drive(FORWARD);
			motors->flashlight(OFF);
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}


	if (sensorAlarm == OBSTACLESEVERYWHEREINFRONT)
	{
		emit showPreferredDirection("*NONE*");
		
		// TODO: use *one* list and make a "count" in the list?!
		// clear alarm buffers
		obstacleAlarmFrontLeftList.clear();
		obstacleAlarmFrontRightList.clear();
		
		if (robotDrives == true)
		{
			gui->appendLog(QString("<b>Obstacle %1 everywhere in front of the robot. Waiting.</b>").arg(alarmCounter));
			
			//----------------
			// drive WAIT
			//----------------
			drive(WAIT);
			motors->flashlight(ON);
			emit speak("I think, I need some help!");
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}


	if (sensorAlarm == OBSTACLEFRONTLEFT)
	{
		//------------------------
		// check for false alarms
		//------------------------
		// TODO: use *one* list and make a "count" in the list?!
		// increase alarm left buffer
		obstacleAlarmFrontLeftList.append(sensorAlarm);
		// clear other alarm buffer
		obstacleAlarmFrontRightList.clear();
		
		// REAL ALARM!
		// TODO: use a const / ini-file-value / timestamp !
		if (obstacleAlarmFrontLeftList.count() >= 2)
		{
			emit showPreferredDirection("RIGHT");
			
			// clear alarm buffer
			obstacleAlarmFrontLeftList.clear();
			
			if (robotDrives == true)
			{
				gui->appendLog(QString("<b>Obstacle %1 front left. Turning right.</b>").arg(alarmCounter));
				
				//----------------
				// drive right
				//----------------
				drive(RIGHT);
				motors->flashlight(OFF);
				
				// store this sensor alarm value
				lastSensorValue = sensorAlarm;
				// reset the alarm counter
				alarmCounter = 0;
			}
		}
		
		return;
	}


	if (sensorAlarm == OBSTACLEFRONTRIGHT)
	{
		//------------------------
		// check for false alarms
		//------------------------
		// TODO: use *one* list and make a "count" in the list?!
		// increase alarm right buffer
		obstacleAlarmFrontRightList.append(sensorAlarm);
		// clear other buffer
		obstacleAlarmFrontRightList.clear();
		
		// REAL ALARM!
		// TODO: use a const / ini-file-value / timestamp !
		if (obstacleAlarmFrontRightList.count() >= 2)
		{
			emit showPreferredDirection("LEFT");
			
			// clear alarm buffer
			obstacleAlarmFrontRightList.clear();
			
			if (robotDrives == true)
			{
				gui->appendLog("<b>Obstacle front right. Turning left.</b>");
				
				//----------------
				// drive left
				//----------------
				drive(LEFT);
				motors->flashlight(OFF);
				
				// store this sensor alarm value
				lastSensorValue = sensorAlarm;
				// reset the alarm counter
				alarmCounter = 0;
			}
		}
		return;
	}
}


void Mrs::enableFaceTracking(int state)
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


void Mrs::faceTracking(IplImage* frame, int faceX, int faceY, int faceRadius)
{
	// TODO: put values to consts or ini
	int xLevelRight = (camThread->imageWidth()  / 2) + faceRadius;
	int xLevelLeft  = (camThread->imageWidth()  / 2) - faceRadius;
	int yLevelUp    = (camThread->imageHeight() / 2) - faceRadius;
	int yLevelDown  = (camThread->imageHeight() / 2) + faceRadius;

	
	// track nowhere (face is in the middle) or faceRadius is 0 -> no faces detected
	if ( (faceRadius==0) || ((faceX > xLevelLeft) && ((faceX < xLevelRight)) && (faceY > yLevelUp) && (faceY < yLevelDown) ) )
	{
		if ( circuit1->isConnected() && (faceTrackingIsEnabled) )
		{
			motors->motorControl(MOTOR3, OFF, SAME);
			motors->motorControl(MOTOR4, OFF, SAME);
		}
		emit showFaceTrackDirection("NONE");
		return;
	}
	
	// track left
	if ( (faceX < xLevelLeft) && (faceY > yLevelUp) && (faceY < yLevelDown) )
	{
		if ( circuit1->isConnected() && (faceTrackingIsEnabled) )
		{
			motors->motorControl(MOTOR3, ON, COUNTERCLOCKWISE);
			motors->motorControl(MOTOR4, OFF, SAME);
		}
		emit showFaceTrackDirection("LEFT");
		return;
	}
	
	// track right
	if ( (faceX > xLevelRight) && (faceY > yLevelUp) && (faceY < yLevelDown) )
	{
		if ( circuit1->isConnected() && (faceTrackingIsEnabled) )
		{
			motors->motorControl(MOTOR3, ON, CLOCKWISE);
			motors->motorControl(MOTOR4, OFF, SAME);
		}
		emit showFaceTrackDirection("RIGHT");
		return;
	}
	
	// track up
	if ( (faceX > xLevelLeft) && (faceX < xLevelRight) && (faceY < yLevelUp) )
	{
		if ( circuit1->isConnected() && (faceTrackingIsEnabled) )
		{
			motors->motorControl(MOTOR3, OFF, SAME);
			motors->motorControl(MOTOR4, ON, CLOCKWISE);
		}
		emit showFaceTrackDirection("UP");
		return;
	}
	
	// track up left
	if ( (faceX < xLevelLeft) && (faceY < yLevelUp) )
	{
		if ( circuit1->isConnected() && (faceTrackingIsEnabled) )
		{
			motors->motorControl(MOTOR3, ON, COUNTERCLOCKWISE);
			motors->motorControl(MOTOR4, ON, CLOCKWISE);
		}
		emit showFaceTrackDirection("UPLEFT");
		return;
	}
	
	// track up right
	if ( (faceX > xLevelLeft) && (faceY < yLevelUp) )
	{
		if ( circuit1->isConnected() && (faceTrackingIsEnabled) )
		{
			motors->motorControl(MOTOR3, ON, CLOCKWISE);
			motors->motorControl(MOTOR4, ON, CLOCKWISE);
		}
		emit showFaceTrackDirection("UPRIGHT");
		return;
	}
	
	// track down
	if ( (faceX > xLevelLeft) && (faceX < xLevelRight) && (faceY > yLevelDown) )
	{
		if ( circuit1->isConnected() && (faceTrackingIsEnabled) )
		{
			motors->motorControl(MOTOR3, OFF, SAME);
			motors->motorControl(MOTOR4, ON, COUNTERCLOCKWISE);
		}
		emit showFaceTrackDirection("DOWN");
		return;
	}
	
	// track down left
	if ( (faceX < xLevelLeft) && (faceY > yLevelDown) )
	{
		if ( circuit1->isConnected() && (faceTrackingIsEnabled) )
		{
			motors->motorControl(MOTOR3, ON, COUNTERCLOCKWISE);
			motors->motorControl(MOTOR4, ON, COUNTERCLOCKWISE);
		}
		emit showFaceTrackDirection("DOWNLEFT");
		return;
	}
	
	// track down right
	if ( (faceX > xLevelRight) && (faceY > yLevelDown) )
	{
		if ( circuit1->isConnected() && (faceTrackingIsEnabled) )
		{
			motors->motorControl(MOTOR3, ON, CLOCKWISE);
			motors->motorControl(MOTOR4, ON, COUNTERCLOCKWISE);
		}
		emit showFaceTrackDirection("DOWNRIGHT");
		return;
	}
}


void Mrs::showSensorData()
{
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
	
	/*
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
	gui->showDrivenDistance(MOTORSENSOR1, sensorThread->getDrivenDistance(MOTORSENSOR1));
	gui->showDrivenDistance(MOTORSENSOR2, sensorThread->getDrivenDistance(MOTORSENSOR2));
}


void Mrs::drive(const unsigned char command)
{
	switch (command)
	{
		case FORWARD:
			gui->showMotorStatus(MOTOR1, SAME, CLOCKWISE);
			gui->showMotorStatus(MOTOR2, SAME, CLOCKWISE);
			motors->motorControl(MOTOR1, SAME, CLOCKWISE);
			motors->motorControl(MOTOR2, SAME, CLOCKWISE);
			return;
			break;
		case BACKWARD:
			gui->showMotorStatus(MOTOR1, SAME, COUNTERCLOCKWISE);
			gui->showMotorStatus(MOTOR2, SAME, COUNTERCLOCKWISE);
			motors->motorControl(MOTOR1, SAME, COUNTERCLOCKWISE);
			motors->motorControl(MOTOR2, SAME, COUNTERCLOCKWISE);
			return;
			break;
		case LEFT:
			gui->showMotorStatus(MOTOR1, SAME, COUNTERCLOCKWISE);
			gui->showMotorStatus(MOTOR2, SAME, CLOCKWISE);
			motors->motorControl(MOTOR1, SAME, COUNTERCLOCKWISE);
			motors->motorControl(MOTOR2, SAME, CLOCKWISE);
			return;
			break;
		case RIGHT:
			gui->showMotorStatus(MOTOR1, SAME, CLOCKWISE);
			gui->showMotorStatus(MOTOR2, SAME, COUNTERCLOCKWISE);
			motors->motorControl(MOTOR1, SAME, CLOCKWISE);
			motors->motorControl(MOTOR2, SAME, COUNTERCLOCKWISE);
			return;
			break;
		case START:
			if (circuit1->isConnected())
			{
				robotDrives = true;
				gui->appendLog("Starting to drive forward...");
				// set the motors to "drive FORWARD"
				gui->showMotorStatus(MOTOR1, ON, CLOCKWISE);
				gui->showMotorStatus(MOTOR2, ON, CLOCKWISE);

				motors->motorControl(MOTOR1, ON, CLOCKWISE);
				motors->motorControl(MOTOR2, ON, CLOCKWISE);
			}
			else
			{
				// show message
				gui->appendLog("<font color=\"#FF0000\">Robot is OFF!</font>");
				robotDrives = false;
			}
			return;
			break;
		case WAIT:
			gui->showMotorStatus(MOTOR1, OFF, SAME);
			gui->showMotorStatus(MOTOR2, OFF, SAME);
			// turning motors off
			motors->motorControl(MOTOR1, OFF, SAME);
			motors->motorControl(MOTOR2, OFF, SAME);
			//
			// Don't stop the motThread (PWM)!
			// Only switching motors off!
			//
			return;
			break;
		case STOP:
			gui->showMotorStatus(MOTOR1, OFF, SAME);
			gui->showMotorStatus(MOTOR2, OFF, SAME);
			// turning motors off
			motors->motorControl(MOTOR1, OFF, SAME);
			motors->motorControl(MOTOR2, OFF, SAME);
			//
			// Don't stop the motThread (clock)!
			// Only switching motors off!
			//
			robotDrives = false;
			return;
			break;
	}
}



void Mrs::readSettings()
{
	//---------------------------------------------------------------------
	// get the programm settings and set the items on the gui (sliders...)
	//---------------------------------------------------------------------
	gui->appendLog("Reading settings...");


	//---------------------------------------------------------------------
	// read setting
	serialPortMicrocontroller = inifile1->readString("Config", "serialPortMicrocontroller");
	
	if (serialPortMicrocontroller == "error2")
	{
		gui->appendLog("<font color=\"#FF0000\">ini-file is not writeable!</font>");
	}
	else
	{
		if (serialPortMicrocontroller == "error1")
		{
			gui->appendLog("<font color=\"#FF0000\">Value \"serialPortMicrocontroller\" not found in ini-file!</font>");
		}
		else
		{
			//
			// everything okay
			//
			gui->appendLog(QString("Serial port for microcontroller set to <b>%1</b>.").arg(serialPortMicrocontroller));
		}
	}

	//---------------------------------------------------------------------
	// read setting
	serialPortLaserscannerFront = inifile1->readString("Config", "serialPortLaserscannerFront");
	
	if (serialPortLaserscannerFront == "error2")
	{
		laserThread->setSerialPort(LASER1, "none");
		gui->appendLog("<font color=\"#FF0000\">ini-file is not writeable!</font>");
	}
	else
	{
		if (serialPortLaserscannerFront == "error1")
		{
			laserThread->setSerialPort(LASER1, "none");
			gui->appendLog("<font color=\"#FF0000\">Value \"serialPortLaserscannerFront\" not found in ini-file!</font>");
		}
		else
		{
			//
			// everything okay
			//
			laserThread->setSerialPort(LASER1, serialPortLaserscannerFront);
			gui->appendLog(QString("Front laser scanner set to <b>%1</b>.").arg(serialPortLaserscannerFront));
		}
	}

	//---------------------------------------------------------------------
	// read setting
	serialPortLaserscannerRear = inifile1->readString("Config", "serialPortLaserscannerRear");
	
	if (serialPortLaserscannerRear == "error2")
	{
		laserThread->setSerialPort(LASER2, "none");
		gui->appendLog("<font color=\"#FF0000\">ini-file is not writeable!</font>");
	}
	else
	{
		if (serialPortLaserscannerRear == "error1")
		{
			laserThread->setSerialPort(LASER2, "none");
			gui->appendLog("<font color=\"#FF0000\">Value \"serialPortLaserscannerRear\" not found in ini-file!</font>");
		}
		else
		{
			//
			// everything okay
			//
			laserThread->setSerialPort(LASER2, serialPortLaserscannerRear);
			gui->appendLog(QString("Rear laser scanner set to <b>%1</b>.").arg(serialPortLaserscannerRear));
		}
	}
	
	//---------------------------------------------------------------------
	// read setting / and error handling
	switch (inifile1->readSetting("Config", "noHardwareErrorMessages"))
	{
		case -2:
			gui->appendLog("<font color=\"#FF0000\">ini-file is not writeable!</font>");
			break;
		case -1:
			gui->appendLog("<font color=\"#FF0000\">Value \"noHardwareErrorMessages\"not found in ini-file!</font>");
			break;
		case 0:
			noHardwareErrorMessages = false;
			break;
		case 1:
			noHardwareErrorMessages = true;
			gui->appendLog("<font color=\"#808080\">Suppressing hardware error messages (see ini-file)</font>");
			break;
	}

	//---------------------------------------------------------------------
	// read setting
	switch (inifile1->readSetting("Config", "saveOnExit"))
	{
		case -2:
			gui->appendLog("<font color=\"#FF0000\">ini-file is not writeable!</font>");
			break;
		case -1:
			gui->appendLog("<font color=\"#FF0000\">Value \"saveOnExit\"not found in ini-file!</font>");
			break;
		case Qt::Unchecked:
			// uncheck checkbox
			gui->setCheckBoxSaveSettings(Qt::Unchecked);
			break;
		case Qt::Checked:
			// set checkbox
			gui->setCheckBoxSaveSettings(Qt::Checked);
			break;
	}

	//---------------------------------------------------------------------
	// read setting
	switch (inifile1->readSetting("Config", "exitDialog"))
	{
		case -2:
			gui->appendLog("<font color=\"#FF0000\">ini-file is not writeable!</font>");
			break;
		case -1:
			gui->appendLog("<font color=\"#FF0000\">Value \"exitDialog\"not found in ini-file!</font>");
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
			gui->appendLog("<font color=\"#FF0000\">ini-file is not writeable!</font>");
			break;
		case -1:
			gui->appendLog("<font color=\"#FF0000\">Value \"minObstacleDistance\"not found in ini-file!</font>");
			break;
		default:
			// set slider to the read value
			gui->setSliderObstacleValue(minObstacleDistance);
			// tell the  obstacle check thread the distance
			obstCheckThread->setMinObstacleDistance(minObstacleDistance);
			// show text
			gui->appendLog(QString("Min. obstacle distance set to <b>%1 cm</b>.").arg(minObstacleDistance));
			break;
	}

	//---------------------------------------------------------------------
	// read setting
	int minObstacleDistanceLaserScanner = inifile1->readSetting("Config", "minObstacleDistanceLaserScanner");
	
	switch (minObstacleDistanceLaserScanner)
	{
		case -2:
			gui->appendLog("<font color=\"#FF0000\">ini-file is not writeable!</font>");
			break;
		case -1:
			gui->appendLog("<font color=\"#FF0000\">Value \"minObstacleDistanceLaserScanner\"not found in ini-file!</font>");
			break;
		default:
			// set slider to the read value
			gui->setSliderObstacleLaserScannerValue(minObstacleDistanceLaserScanner);
			// tell it the obstacle check thread
			obstCheckThread->setMinObstacleDistanceLaser(minObstacleDistanceLaserScanner);
			// show text
			gui->appendLog(QString("Min. obstacle distance Laser Scanner set to <b>%1 cm</b>.").arg(minObstacleDistanceLaserScanner));
			break;
	}

	//---------------------------------------------------------------------
	// read setting
	int robotSlot = inifile1->readSetting("Config", "robotSlot");
	
	switch (robotSlot)
	{
		case -2:
			gui->appendLog("<font color=\"#FF0000\">ini-file is not writeable!</font>");
			break;
		case -1:
			gui->appendLog("<font color=\"#FF0000\">Value \"robotSlot\"not found in ini-file!</font>");
			break;
		default:
			// set slider to the read value
			gui->setSliderRobotSlot(robotSlot);
			// tell it the obstacle check thread
			obstCheckThread->setRobotSlot(robotSlot);
			// show text
			gui->appendLog(QString("Robot slot set to <b>%1 deg.</b>").arg(robotSlot));
			break;
	}

	//---------------------------------------------------------------------
	// read setting
	int straightForwardDeviation = inifile1->readSetting("Config", "straightForwardDeviation");
	
	switch (straightForwardDeviation)
	{
		case -2:
			gui->appendLog("<font color=\"#FF0000\">ini-file is not writeable!</font>");
			break;
		case -1:
			gui->appendLog("<font color=\"#FF0000\">Value \"straightForwardDeviation\"not found in ini-file!</font>");
			break;
		default:
			// set slider to the read value
			gui->setSliderStraightForwardDeviation(straightForwardDeviation);
			// tell it the obstacle check thread
			obstCheckThread->setStraightForwardDeviation(straightForwardDeviation);
			// show text
			gui->appendLog(QString("Straight forward deviation set to <b>%1 deg.</b>").arg(straightForwardDeviation));
			break;
	}

	//---------------------------------------------------------------------
	// read setting
	QString joystickPort = inifile1->readString("Config", "joystickPort");
	
	if (joystickPort == "error2")
	{
		gui->appendLog("<font color=\"#FF0000\">ini-file is not writeable!</font>");
	}
	else
	{
		if (joystickPort == "error1")
		{
			gui->appendLog("<font color=\"#FF0000\">Value \"joystickPort\" not found in ini-file!</font>");
		}
		else
		{
			//
			// everything okay
			//
			// tell the  obstacle check thread the distance
			joystick->setPort(joystickPort);
			gui->appendLog(QString("Joystick port set to <b>%1</b>.").arg(joystickPort));
		}
	}
	
	//---------------------------------------------------------------------
	// read setting
	mot1Speed = inifile1->readSetting("Config", "motor1Speed");
	
	switch (mot1Speed)
	{
		case -2:
			gui->appendLog("<font color=\"#FF0000\">ini-file is not writeable!</font>");
			mot1Speed = 0;
			break;
		case -1:
			gui->appendLog("<font color=\"#FF0000\">Value \"motor1Speed\" not found in ini-file!</font>");
			mot1Speed = 0;
			break;
		default:
			if (mot1Speed > 254)
			{
				gui->appendLog("<font color=\"#FF0000\">Value \"motor1Speed\" is greater than 255!! Value set to 255!</font>");
				mot1Speed = 255;
			}
			
			// set slider to the read value
			gui->setSliderMotorSpeed(1, mot1Speed);
			// show text
			gui->appendLog(QString("Motor1 speed set to <b>%1</b>.").arg(mot1Speed));
			break;
	}
	
	//---------------------------------------------------------------------
	// read setting
	mot2Speed = inifile1->readSetting("Config", "motor2Speed");
	
	switch (mot2Speed)
	{
		case -2:
			gui->appendLog("<font color=\"#FF0000\">ini-file is not writeable!</font>");
			mot2Speed = 0;
			break;
		case -1:
			gui->appendLog("<font color=\"#FF0000\">Value \"motor2Speed\" not found in ini-file!</font>");
			mot2Speed = 0;
			break;
		default:
			if (mot2Speed > 254)
			{
				gui->appendLog("<font color=\"#FF0000\">Value \"motor2Speed\" is greater than 255!! Value set to 255!</font>");
				mot2Speed = 255;
			}
			
			// set slider to the read value
			gui->setSliderMotorSpeed(2, mot2Speed);
			// show text
			gui->appendLog(QString("Motor2 speed set to <b>%1</b>.").arg(mot2Speed));
			break;
	}
	
	//---------------------------------------------------------------------
	// read setting
	mot3Speed = inifile1->readSetting("Config", "motor3Speed");
	
	switch (mot3Speed)
	{
		case -2:
			gui->appendLog("<font color=\"#FF0000\">ini-file is not writeable!</font>");
			mot3Speed = 0;
			break;
		case -1:
			gui->appendLog("<font color=\"#FF0000\">Value \"motor3Speed\" not found in ini-file!</font>");
			mot3Speed = 0;
			break;
		default:
			if (mot3Speed > 254)
			{
				gui->appendLog("<font color=\"#FF0000\">Value \"motor3Speed\" is greater than 255!! Value set to 255!</font>");
				mot3Speed = 255;
			}
			
			// set slider to the read value
			gui->setSliderMotorSpeed(3, mot3Speed);
			// show text
			gui->appendLog(QString("Motor3 speed set to <b>%1</b>.").arg(mot3Speed));
			break;
	}
	
	//---------------------------------------------------------------------
	// read setting
	mot4Speed = inifile1->readSetting("Config", "motor4Speed");
	
	switch (mot4Speed)
	{
		case -2:
			gui->appendLog("<font color=\"#FF0000\">ini-file is not writeable!</font>");
			mot4Speed = 0;
			break;
		case -1:
			gui->appendLog("<font color=\"#FF0000\">Value \"motor4Speed\" not found in ini-file!</font>");
			mot4Speed = 0;
			break;
		default:
			if (mot4Speed > 254)
			{
				gui->appendLog("<font color=\"#FF0000\">Value \"motor4Speed\" is greater than 255!! Value set to 255!</font>");
				mot4Speed = 255;
			}
			
			// set slider to the read value
			gui->setSliderMotorSpeed(4, mot4Speed);
			// show text
			gui->appendLog(QString("Motor4 speed set to <b>%1</b>.").arg(mot4Speed));
			break;
	}
	
	//---------------------------------------------------------------------
	// read setting
	minimumSpeed = inifile1->readSetting("Config", "minimumSpeed");
	
	switch (minimumSpeed)
	{
		case -2:
			gui->appendLog("<font color=\"#FF0000\">ini-file is not writeable!</font>");
			minimumSpeed = 0;
			break;
		case -1:
			gui->appendLog("<font color=\"#FF0000\">Value \"minimumSpeed\" not found in ini-file!</font>");
			minimumSpeed = 0;
			break;
		default:
			if (minimumSpeed > 254)
			{
				gui->appendLog("<font color=\"#FF0000\">Value \"minimumSpeed\" is greater than 255!! Value set to 255!</font>");
				minimumSpeed = 255;
			}
			
			// set slider to the read value
			gui->setSliderMinimumSpeed(minimumSpeed);
			// show text
			gui->appendLog(QString("Minimum speed speed set to <b>%1</b>.").arg(minimumSpeed));
			break;
	}
	
	//---------------------------------------------------------------------
	// read setting
	maximumSpeed = inifile1->readSetting("Config", "maximumSpeed");
	
	switch (maximumSpeed)
	{
		case -2:
			gui->appendLog("<font color=\"#FF0000\">ini-file is not writeable!</font>");
			maximumSpeed = 0;
			break;
		case -1:
			gui->appendLog("<font color=\"#FF0000\">Value \"maximumSpeed\" not found in ini-file!</font>");
			maximumSpeed = 0;
			break;
		default:
			if (maximumSpeed > 254)
			{
				gui->appendLog("<font color=\"#FF0000\">Value \"maximumSpeed\" is greater than 255!! Value set to 255!</font>");
				maximumSpeed = 255;
			}
			
			// set slider to the read value
			gui->setSliderMaximumSpeed(maximumSpeed);
			// show text
			gui->appendLog(QString("Maximum speed speed set to <b>%1</b>.").arg(maximumSpeed));
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
				gui->appendLog("<font color=\"#FF0000\">ini-file is not writeable!</font>");
				settingValue = 0;
				break;
			case -1:
				gui->appendLog(QString("<font color=\"#FF0000\">Value \"%1\" not found in ini-file!</font>").arg(settingName));
				settingValue = 0;
				break;
			default:
				if (settingValue > 254)
				{
					gui->appendLog(QString("<font color=\"#FF0000\">Value \"%1\" is greater than 255!! Value set to 255!</font>").arg(settingName));
					settingValue = 255;
				}
	
				// store the servo values
				servos->setServoPosition(servo, SVSTART, settingValue);
		
				// show text
				gui->appendLog(QString("%1 set to <b>%2</b>.").arg(settingName).arg(settingValue));
				break;
		}
	}
	//---------------------------------------------------------------------
	// read servo END settings
	for (int servo=0; servo<NUMBEROFSERVOS; servo++)
	{
		QString settingName = QString("servo%1").arg(servo+1).append("EndPos");
		int settingValue = inifile1->readSetting("Config", settingName);
		
		switch (settingValue)
		{
			case -2:
				gui->appendLog("<font color=\"#FF0000\">ini-file is not writeable!</font>");
				settingValue = 0;
				break;
			case -1:
				gui->appendLog(QString("<font color=\"#FF0000\">Value \"%1\" not found in ini-file!</font>").arg(settingName));
				settingValue = 0;
				break;
			default:
				if (settingValue > 254)
				{
					gui->appendLog(QString("<font color=\"#FF0000\">Value \"%1\" is greater than 255!! Value set to 255!</font>").arg(settingName));
					settingValue = 255;
				}
	
				// store the servo values
				servos->setServoPosition(servo, SVEND, settingValue);
		
				// show text
				gui->appendLog(QString("%1 set to <b>%2</b>.").arg(settingName).arg(settingValue));
				break;
		}
	}
	//---------------------------------------------------------------------
	// read servo DEFAULT settings
	for (int servo=0; servo<NUMBEROFSERVOS; servo++)
	{
		QString settingName = QString("servo%1").arg(servo+1).append("DefaultPos");
		int settingValue = inifile1->readSetting("Config", settingName);
		
		switch (settingValue)
		{
			case -2:
				gui->appendLog("<font color=\"#FF0000\">ini-file is not writeable!</font>");
				settingValue = 0;
				break;
			case -1:
				gui->appendLog(QString("<font color=\"#FF0000\">Value \"%1\" not found in ini-file!</font>").arg(settingName));
				settingValue = 0;
				break;
			default:
				if (settingValue > 254)
				{
					gui->appendLog(QString("<font color=\"#FF0000\">Value \"%1\" is greater than 255!! Value set to 255!</font>").arg(settingName));
					settingValue = 255;
				}
	
				// store the servo values
				servos->setServoPosition(servo, SVDEFAULT, settingValue);
		
				// show text
				gui->appendLog(QString("%1 set to <b>%2</b>.").arg(settingName).arg(settingValue));
				break;
		}
	}
}


void Mrs::enableRemoteControlListening(bool status)
{
	// store the state gobal to Mrs
	robotRemoteMode = status;

	if (status == true)
	{
		//-----------------------------------------------------------
		// start the network thread (getting commands via network)
		//-----------------------------------------------------------
		if (netThread->isRunning() == false)
		{
			gui->appendLog("Starting network thread...", false);
			netThread->start();
			gui->appendLog("Network thread started.");
		}
		
		
		//-----------------------------------------------------------
		// check if joystick is connected
		//-----------------------------------------------------------
		if (joystick->isConnected())
		{
			// start the joystick thread
			if (joystick->isRunning() == false)
			{
				gui->appendLog("Starting joystick thread...", false);
				joystick->start();
				gui->appendLog("Joystick thread started.");
			}
		}
		else
		{
			gui->appendLog("Joystick thread NOT started!");
		}

	}
	else
	{
		if (netThread->isRunning() == true)
		{
			gui->appendLog("Stopping network thread...", false);
			netThread->stop();
			gui->appendLog("Network thread stopped.");
		}
		
		//
		// NOT stopping joystick thread!
		// It' still nice to see it in the GUI, when being moved, :-)
		// So just dosconnect the Signal from the receiver here.
		// TODO: connect again, when needed! realy? Solved via global "robotRemoteMode" variable?
		// FIXME: Object::disconnect: No such signal Mrs::joystickMoved(int,int)
		//this->disconnect(SIGNAL(joystickMoved(int, int)));
		//this->disconnect(SIGNAL(joystickButtonPressed(int, bool)));
	}
}


void Mrs::executeRemoteCommand(QString command)
{
	// only react, when remote mode is activated in the GUI!
	if (robotRemoteMode==true)
	{
		if (command == "start")
		{
			gui->appendLog(tr("<font color=\"#0000FF\">Executing remote command \"%1\".</font>").arg(command));
			drive(START);
			return;
		}
		
		
		if (command == "forward")
		{
			gui->appendLog(tr("<font color=\"#0000FF\">Executing remote command \"%1\".</font>").arg(command));
			drive(FORWARD);
			return;
		}
		
		
		if (command == "backward")
		{
			gui->appendLog(tr("<font color=\"#0000FF\">Executing remote command \"%1\".</font>").arg(command));
			drive(BACKWARD);
			return;
		}
	
	
		if (command == "stop")
		{
			gui->appendLog(tr("<font color=\"#0000FF\">Executing remote command \"%1\".</font>").arg(command));
			drive(STOP);
			return;
		}
	
	
		if (command == "left")
		{
			gui->appendLog(tr("<font color=\"#0000FF\">Executing remote command \"%1\".</font>").arg(command));
			drive(LEFT);
			return;
		}
	
	
		if (command == "right")
		{
			gui->appendLog(tr("<font color=\"#0000FF\">Executing remote command \"%1\".</font>").arg(command));
			drive(RIGHT);
			return;
		}
	
	
		if (command == "increasespeedmotor1")
		{
			gui->appendLog(tr("<font color=\"#0000FF\">Executing remote command \"%1\".</font>").arg(command));
			
			int newSpeed = motors->getMotorSpeed(1) + 1;
			motors->setMotorSpeed(1, newSpeed);
			gui->setSliderMotorSpeed(1, newSpeed);
			return;
		}
	
	
		if (command == "increasespeedmotor2")
		{
			gui->appendLog(tr("<font color=\"#0000FF\">Executing remote command \"%1\".</font>").arg(command));
			
			int newSpeed = motors->getMotorSpeed(2) + 1;
			motors->setMotorSpeed(2, newSpeed);
			gui->setSliderMotorSpeed(2, newSpeed);
			return;
		}
	
	
		if (command == "reducespeedmotor1")
		{
			gui->appendLog(tr("<font color=\"#0000FF\">Executing remote command \"%1\".</font>").arg(command));
			
			int newSpeed = motors->getMotorSpeed(1) - 1;
			motors->setMotorSpeed(1, newSpeed);
			gui->setSliderMotorSpeed(1, newSpeed);
			return;
		}
	
	
		if (command == "reducespeedmotor2")
		{
			gui->appendLog(tr("<font color=\"#0000FF\">Executing remote command \"%1\".</font>").arg(command));
			
			int newSpeed = motors->getMotorSpeed(2) - 1;
			motors->setMotorSpeed(2, newSpeed);
			gui->setSliderMotorSpeed(2, newSpeed);
			return;
		}
	}
}


void Mrs::executeJoystickCommand(int axisNumber, int axisValue)
{
	int speed = 0;
	/*
	static unsigned char servo1Pos = 10;
	static unsigned char servo2Pos = 10;
	static unsigned char servo3Pos = 10;
	static unsigned char servo4Pos = 10;
	static unsigned char servo5Pos = 10;
	static unsigned char servo6Pos = 10;
	*/
	
	//
	// Y axis
	//
	if (axisNumber == JOYSTICKAXISY)
	{
		//------------------
		// DRIVE backward
		//------------------
		if (axisValue > 0)
		{
			speed = (axisValue / JOYSTICKDIVISOR);
			
			gui->setSliderMotorSpeed(1, speed);
			gui->setSliderMotorSpeed(2, speed);
			
			// only drive, when remote mode is activated in the GUI!
			if (robotRemoteMode==true)
			{
				motors->setMotorSpeed(1, speed);
				motors->setMotorSpeed(2, speed);
			
				if (robotDrives == false)
				{
					drive(START);
				}
				
				drive(BACKWARD);
			}
			return;
		}
		
		//------------------
		// DRIVE forward
		//------------------
		if (axisValue < 0)
		{
			speed = (-axisValue / JOYSTICKDIVISOR);
			
			gui->setSliderMotorSpeed(1, speed);
			gui->setSliderMotorSpeed(2, speed);
			
			// only drive, when remote mode is activated in the GUI!
			if (robotRemoteMode==true)
			{
				motors->setMotorSpeed(1, speed);
				motors->setMotorSpeed(2, speed);
			
				if (robotDrives == false)
				{
					drive(START);
				}
				
				drive(FORWARD);
			}	
			return;
		}
		
		//------------------
		// STOP
		//------------------
		if (axisValue == 0)
		{
			// only drive, when remote mode is activated in the GUI!
			if (robotRemoteMode==true)
			{
				drive(WAIT);
			}
			return;
		}
	}

	
	//
	// X axis
	//
	if (axisNumber == JOYSTICKAXISX) 
	{
		//------------------
		// DRIVE right
		//------------------
		if (axisValue > 0)
		{
			speed = (axisValue / JOYSTICKDIVISOR);
			
			gui->setSliderMotorSpeed(1, speed);
			gui->setSliderMotorSpeed(2, speed);
			
			// only drive, when remote mode is activated in the GUI!
			if (robotRemoteMode==true)
			{
				motors->setMotorSpeed(1, speed);
				motors->setMotorSpeed(2, speed);
			
				if (robotDrives == false)
				{
					drive(START);
				}
			
				drive(RIGHT);
			}
			return;
		}
		
		//------------------
		// DRIVE left
		//------------------
		if (axisValue < 0)
		{
			speed = (-axisValue / JOYSTICKDIVISOR);
			
			gui->setSliderMotorSpeed(1, speed);
			gui->setSliderMotorSpeed(2, speed);
			
			// only drive, when remote mode is activated in the GUI!
			if (robotRemoteMode==true)
			{
				motors->setMotorSpeed(1, speed);
				motors->setMotorSpeed(2, speed);
				
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
			// only drive, when remote mode is activated in the GUI!
			if (robotRemoteMode==true)
			{
				drive(WAIT);
			}
			return;
		}
	}
	
	
	//
	// Y axis "buttons"
	//
	if (axisNumber == JOYSTICKAXIS2Y)
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
				if (circuit1->isConnected() == true)
				{
					servos->moveServo(currentTestServo, servos->getServoPosition(currentTestServo));
				}
				gui->appendLog(QString("Servo %1 moved to %2.").arg(currentTestServo+1).arg(servos->getServoPosition(currentTestServo)));
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
				//gui->appendLog("motor 4 on CW");
			}
			
			//------------------
			// camera down [tilt]
			//------------------
			if (axisValue < 0)
			{
				motors->motorControl(MOTOR4, ON, COUNTERCLOCKWISE);
				//gui->appendLog("motor 4 on CCW");
			}
			
			// move, when button is pressed
			if (axisValue != 0)
			{
				if (circuit1->isConnected() == true)
				{
					//gui->appendLog("Tilting Cam...");
				}
			}
			
			// stop, when button is pressed!
			if (axisValue == 0)
			{
				if (circuit1->isConnected() == true)
				{
					//gui->appendLog("Tilt stop.");
					motors->motorControl(MOTOR4, OFF, SAME);
				}
			}
			return;
		} // cam test mode [tilt] 

		return;
	}
	
	
	//
	// X axis "buttons"
	//
	if (axisNumber == JOYSTICKAXIS2X)
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
				if (circuit1->isConnected() == true)
				{
//					servos->moveServo(SERVO1, servo1Pos);
				}
//				gui->appendLog(QString("Servo 1 moved to %1.").arg(servo1Pos));
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
				//gui->appendLog("motor 3 on CW");
			}
			
			//------------------
			// camera left [pan]
			//------------------
			if (axisValue < 0)
			{
				motors->motorControl(MOTOR3, ON, COUNTERCLOCKWISE);
				//gui->appendLog("motor 3 on CCW");
			}
			
			// move, when button is pressed
			if (axisValue != 0)
			{
				if (circuit1->isConnected() == true)
				{
					//gui->appendLog("Panning Cam...");
				}
			}
			
			// stop, when button is pressed!
			if (axisValue == 0)
			{
				if (circuit1->isConnected() == true)
				{
					//gui->appendLog("Pan stop.");
					motors->motorControl(MOTOR3, OFF, SAME);
				}
			}
			return;
		} // cam test mode [pan]
		
		return;
	}
}


void Mrs::executeJoystickCommand(int buttonNumber, bool buttonState)
{
	/*
	static bool toggle0 = false;
	static bool toggle1 = false;
	static bool toggle2 = false;
	static bool toggle3 = false;
	static bool toggle4 = false;
	static bool toggle5 = false;
	*/
	static bool toggle10 = false;
	static bool toggle11 = false;
	
	
	// TODO: put button numbers to ini-file
	switch (buttonNumber)
	{
		case 0: // 1 on js
			break;
		case 1: // 2 on js
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
					gui->appendLog(QString("Servo %1 selected.").arg((--currentTestServo)+1));
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
					gui->appendLog(QString("Servo %1 selected.").arg((++currentTestServo)+1));
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
					toggle10=true;
					servoTestMode = true;
					gui->appendLog("<font color=\"#0000FF\">Servo test mode ON</front>");
					gui->appendLog(QString("Servo %1 selected.").arg(currentTestServo+1));
				}
				else
				{
					toggle10=false;
					servoTestMode = false;
					gui->appendLog("<font color=\"#0000FF\">Servo test mode OFF</front>");
				}
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
					toggle11=true;
					cameraTestMode = true;
					gui->appendLog("<font color=\"#0000FF\">Camera test mode ON</front>");
				}
				else
				{
					toggle11=false;
					cameraTestMode = false;
					gui->appendLog("<font color=\"#0000FF\">Camera test mode OFF</front>");
				}
			}
			break;
	}
}


bool Mrs::simulationMode() const
{
	return robotSimulationMode;
}


void Mrs::setSimulationMode(bool status)
{
	robotSimulationMode = status;
	
	if (status == true)
	{
		gui->appendLog("<font color=\"#0000FF\">Simulation mode enabled!!</front>");
	
		if (laserThread->isRunning() == false)
		{
			gui->appendLog("Starting Laser thread...", false);
			laserThread->start();
			gui->appendLog("Laser thread started.");
		}
		
		if (sensorThread->isRunning() == false)
		{
			gui->appendLog("Starting Sensor thread...", false);
			sensorThread->start();
			gui->appendLog("Sensor thread started.");
		}
		
		if (plotThread->isRunning() == false)
		{
			gui->appendLog("Starting plot thread...", false);
			plotThread->start();
			gui->appendLog("Plot thread started.");
		}
		
		if (obstCheckThread->isRunning() == false)
		{
			gui->appendLog("Starting obstacle check thread...", false);
			obstCheckThread->start();
			gui->appendLog("Obstacle check thread started.");
		}
	}
	else
	{
		gui->appendLog("<font color=\"#0000FF\">Simulation mode disabled.</font>");
	}
}


void Mrs::speak(QString text)
{
	#ifdef _TTY_POSIX_
	int start= -1;
	
	
	//------------------------------
	// remove HTML tags from string
	//------------------------------
	do
	{
		// search for the first HTML "<"
		start = text.indexOf("<");

		if (start != 1)
		{
			text.remove(start, text.indexOf(">")+1 - start);
		}
	} while (text.contains(">"));
	// till the last HTML ">" is found
	
	// convert QString to EST_String (defined in EST_String.h)
	EST_String textForFestival (text.toAscii());
	
	// say the cleaned text
	festival_say_text(textForFestival);
	#endif
}


void Mrs::test()
{
	static bool toggle = OFF;
	
	int pos1 = 10; // 10 is toll
	int pos2 = 8;
	
	if (toggle == OFF)
	{
		toggle = ON;
		// servo1 ist der große
		servos->moveServo(SERVO1, pos1);
		// servo2 ist der kleine
		servos->moveServo(SERVO2, pos1);
		gui->appendLog("pos1");
	}
	else
	{
		toggle = OFF;
		servos->moveServo(SERVO1, pos2);
		servos->moveServo(SERVO2, pos2);
		gui->appendLog("pos2");
	}
	
	//motors->flashlight(toggle);
	
	#ifdef _TTY_POSIX_
	// Say some text;
	QDateTime now = QDateTime::currentDateTime();
//	emit speak(tr("Hello Markus. Today it's the %1 of %2, %3. The time is %4:%5.").arg(now.toString("d")).arg(now.toString("MMMM")).arg(now.toString("yyyy")).arg(now.toString("h")).arg(now.toString("m")));
	#endif

}
