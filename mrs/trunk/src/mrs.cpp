/*************************************************************************
 *   Copyright (C) 2008 by Markus Knapp                                  *
 *   mrs @ direcs.de                                                     *
 *                                                                       *
 *   This file is part of mrs.                                           *
 *                                                                       *
 *   mrs is free software: you can redistribute it and/or modify it      *
 *   under the terms of the GNU General Public License as published      *
 *   by the Free Software Foundation, version 3 of the License.          *
 *                                                                       *
 *   mrs is distributed in the hope that it will be useful,              *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of      *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the        *
 *   GNU General Public License for more details.                        *
 *                                                                       *
 *   You should have received a copy of the GNU General Public License   *
 *   along with mrs. If not, see <http://www.gnu.org/licenses/>.         *
 *                                                                       *
 *************************************************************************/

#include "mrs.h"


int main(int argc, char *argv[])
{
	Q_UNUSED(argc);
	Q_UNUSED(argv);

#ifndef _ARM_ // only include on _non_ ARM environments!
	// Initialize the resource file
	Q_INIT_RESOURCE(mrs);
	
	// The QApplication class manages the GUI application's control flow and main settings.
	QApplication app(argc, argv);

	// create a splash screen
	QPixmap pixmap(":/images/images/splash.png");
	QSplashScreen splash(pixmap);

	// create Mrs class object
	Mrs m(&splash);

	// show the splash screen
	splash.show();
	splash.showMessage(QObject::tr("Loading config file..."), Mrs::splashPosition, Mrs::splashColor);
	
#else
	
	// The QCoreApplication class provides an event loop for console Qt applications.
	QCoreApplication app(argc, argv);
	
	// create Mrs class object
	Mrs m;
	
#endif
	
	// init mrs
	m.init();

#ifdef _ARM_ // only include on ARM environments!
	qDebug("-------------------------");
	qDebug("Ready for take off... ;-)");
	qDebug("-------------------------");
#endif
	return app.exec();
}


#ifndef _ARM_ // only include on _non_ ARM environments!
const Qt::Alignment Mrs::splashPosition = Qt::AlignHCenter | Qt::AlignBottom;
const QColor Mrs::splashColor = Qt::red;
#endif


#ifndef _ARM_ // only include on _non_ ARM environments!
Mrs::Mrs(QSplashScreen *splash)
#else
Mrs::Mrs()
#endif
{
	//------------------------------------------------------------------
	// create the objects
	//------------------------------------------------------------------
#ifdef _TTY_POSIX_
	//speakThread = new SpeakThread();
#endif

#ifndef _ARM_ // only include on _non_ ARM environments!
	this->splash = splash;
	settingsDialog = new SettingsDialog();
	joystickDialog = new JoystickDialog();
	aboutDialog = new AboutDialog();
	gui = new Gui(settingsDialog, joystickDialog, aboutDialog);
#else
	gui = new Gui();
#endif
	
	mutex = new QMutex();
	interface1 = new InterfaceAvr();
	circuit1 = new Circuit(interface1, mutex);
//	TODO: heartbeat = new Heartbeat(interface1, mutex);
	motors = new Motor(interface1, mutex);
	sensorThread = new SensorThread(interface1, mutex);
	servos = new Servo(interface1, mutex);
	laserThread = new LaserThread();
	obstCheckThread = new ObstacleCheckThread(sensorThread, laserThread);
#ifndef _ARM_ // only include on _non_ ARM environments!
	plotThread = new PlotThread(sensorThread);
#endif
	inifile1 = new Inifile();
	netThread = new NetworkThread();
#ifndef _ARM_ // only include on _non_ ARM environments!
	camThread = new CamThread();
#endif
	joystick = new Joystick();
	head = new Head(servos);
}


void Mrs::init()
{
#ifndef _ARM_ // only include on _non_ ARM environments!
	aboutDialog->setVersion("1.2.3"); // TODO: put this at a nicer place
#endif
	serialPortMicrocontroller = "error1";
	serialPortLaserscannerFront = "error1";
	robotIsOn = false;
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
	// let some classes know the robots state
	//--------------------------------------------------------------------------
	connect(circuit1, SIGNAL( robotState(bool) ), this, SLOT( setRobotState(bool) ));
//	connect(circuit1, SIGNAL( robotState(bool) ), heartbeat, SLOT( setRobotState(bool) ));
	connect(circuit1, SIGNAL( robotState(bool) ), gui, SLOT( setRobotControls(bool) ));

	//--------------------------------------------------------------------------
	// shutdown Mrs program on exit button
	// shutdown is also called, when the gui is closed
	//--------------------------------------------------------------------------
	connect(gui, SIGNAL(shutdown()), this, SLOT(shutdown()));

	//--------------------------------------------------------------------------
	// call (a) test method(s) when clicking the test button
	//--------------------------------------------------------------------------
	connect(gui, SIGNAL(test()), this, SLOT(test()));
#ifndef _ARM_ // only include on _non_ ARM environments!
	// currently not in use:
	//connect(gui, SIGNAL(test()), camThread, SLOT(test()));
#endif

	//--------------------------------------------------------------------------
	// resets the driven distance, when signal comes from Gui
	//--------------------------------------------------------------------------
	connect(gui, SIGNAL(resetDrivenDistance(int)), sensorThread, SLOT(resetDrivenDistance(int)));

#ifndef _ARM_ // only include on _non_ ARM environments!
	//--------------------------------------------------------------------------
	// set the motor speed, when signal comes from Gui
	//--------------------------------------------------------------------------
	connect(settingsDialog, SIGNAL(setMotorSpeed(int, int)), motors, SLOT(setMotorSpeed(int, int)));

	//--------------------------------------------------------------------------
	// set the robot slot, when signal comes from Gui
	//--------------------------------------------------------------------------
	connect(settingsDialog, SIGNAL(setRobotSlot(int)), obstCheckThread, SLOT(setRobotSlot(int)));

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
#endif
	
	//--------------------------------------------------------------------------
	// let the GUI show servo messages in the log
	//--------------------------------------------------------------------------
	connect(servos, SIGNAL(message(QString)), gui, SLOT(appendLog(QString)));

	//--------------------------------------------------------------------------
	// let the splash screen show laser init messages
	//--------------------------------------------------------------------------
	connect(laserThread, SIGNAL(message(QString)), this, SLOT(showSplashMessage(QString)));

	#ifdef _TTY_POSIX_ // only include in Linux environments, because OpenCV is not available for Windows (and does not make sense for ARM)
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
	#endif


	//----------------------------------------------------------------------------
	// say a text
	//----------------------------------------------------------------------------
	#ifdef _TTY_POSIX_
	// FIXME: SIOD ERROR: the currently assigned stack limit has been exceded
	//connect(this, SIGNAL( speak(QString) ), speakThread, SLOT( speak(QString) ));
	//connect(gui, SIGNAL( speak(QString) ), speakThread, SLOT( speak(QString) ));
	connect(gui, SIGNAL( speak(QString) ), this, SLOT( speak(QString) ));

	#ifndef _ARM_ // only include on _non_ ARM environments!
	//----------------------------------------------------------------------------
	// Initialize the speech engine festival
	//----------------------------------------------------------------------------
	//      1 = we want the festival init files loaded
	// 500000 = default scheme heap size
	festival_initialize(1, 500000);
	#endif

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
		#ifndef _ARM_ // only include on _non_ ARM environments!
		// file not found-Msg
		QMessageBox msgbox(QMessageBox::Critical, tr("mrs"), tr("Required configuration file %1 not found!\nIni-File perhaps not in the same directory?").arg(inifile1->getInifileName()), QMessageBox::Ok | QMessageBox::Default);
		msgbox.exec();
		#else
		qDebug() << "**** Error opening ini-file " << inifile1->getInifileName() << "****";
		#endif

		gui->appendLog(QString("<b><font color=\"#FF0000\">File '%1' not found!</font></b>").arg(inifile1->getInifileName()));
	}
	else
	{
		// file found-Msg
		gui->appendLog(QString("Using ini-File \"%1\".").arg(inifile1->getInifileName()));

		#ifndef _ARM_ // only include on _non_ ARM environments!
		splash->showMessage(QObject::tr("Reading settings..."), splashPosition, splashColor);
		#endif

		//================================================================================================================================================================
		// read all settings
		//================================================================================================================================================================
		readSettings();
	}


	#ifndef _ARM_ // only include on _non_ ARM environments!
	//----------------------------------------------------------------------------
	// connect plotThread signal to "setPlotData"
	// (Whenever the plot thread has new data, the data are show in the GUI)
	//----------------------------------------------------------------------------
	connect(plotThread, SIGNAL( plotDataComplete1(double *, double *, int) ), gui, SLOT( setPlotData1(double *, double *, int) ));
	connect(plotThread, SIGNAL( plotDataComplete2(double *, double *, int) ), gui, SLOT( setPlotData2(double *, double *, int) ));
	connect(plotThread, SIGNAL( plotDataComplete3(double *, double *, int) ), gui, SLOT( setPlotData3(double *, double *, int) ));
	connect(plotThread, SIGNAL( plotDataComplete4(double *, double *, int) ), gui, SLOT( setPlotData4(double *, double *, int) ));
	#endif

	//----------------------------------------------------------------------------
	// let the GUI show messages in the log (e.g. when special buttons pressed)
	//----------------------------------------------------------------------------
	connect(joystick, SIGNAL(emitMessage(QString)), gui, SLOT(appendLog(QString)));

	#ifdef _ARM_ // only include on ARM environments!
	//-------------------------------------------------------
	// start the network thread (waiting for commands)
	//-------------------------------------------------------
	enableRemoteControlListening(true);
	#endif

	//-------------------------------------------------------
	// Open serial port for microcontroller communication
	//-------------------------------------------------------
	gui->appendLog("Opening serial port for microcontroller communication...", false);

	if (interface1->openComPort(serialPortMicrocontroller) == false)
	{
		qDebug() << "Error opening serial port" << serialPortMicrocontroller;
		gui->appendLog(QString("<font color=\"#FF0000\">Error opening serial port '%1'!</font>").arg(serialPortMicrocontroller));

		#ifndef _ARM_ // only include on _non_ ARM environments!
		// show a warning dialog!
		QMessageBox msgbox(QMessageBox::Warning, tr("Error with robots serial port"), tr("Error opening serial port %1").arg(serialPortMicrocontroller), QMessageBox::Ok | QMessageBox::Default);
		msgbox.exec();
		#else
		qDebug() << "**** Error opening serial port" << serialPortMicrocontroller << "****";
		#endif

		// no serial port, no robot :-(
		robotIsOn = false;
	}
	else
	{
		gui->appendLog("Serial port opened.");


		//-------------------------------------------------------
		// Basic init for all the bits on the robot circuit
		// AND check, if the robot is "on" (it answers correct)
		//-------------------------------------------------------
		#ifndef _ARM_ // only include on _non_ ARM environments!
		splash->showMessage(QObject::tr("Searching robot..."), splashPosition, splashColor);
		// for refreshing the splash...
		QApplication::processEvents();
		#endif

		// init the circuit & Co. when hitting the button in the GUI
		connect(gui, SIGNAL( initCircuit() ), circuit1, SLOT( initCircuit() ) );
		connect(gui, SIGNAL( initServos() ), servos, SLOT( init() ) );

		//==========================================
		// init the robots circuit
		//==========================================
		circuit1->initCircuit();
	}


	if (robotIsOn)
	{
		gui->appendLog("Robot is <font color=\"#00FF00\">ON</font> and answers.");

		//-------------------------------------------------------
		// set the read motor speed
		//-------------------------------------------------------
		motors->setMotorSpeed(1, mot1Speed);
		motors->setMotorSpeed(2, mot2Speed);
		motors->setMotorSpeed(3, mot3Speed);
		motors->setMotorSpeed(4, mot4Speed);
		gui->appendLog("Motor speed set in microcontroller");

		//-------------------------------------------------------
		// move all servos in their default positions
		//-------------------------------------------------------
		/* TODO: temporarily deactivated (no servos mounted on the current robot)
		servos->init();
		gui->appendLog("Servos moved to default positions");
		*/

		// TODO: start heartbeat thread and see, whats going on there! Also to do: define atmel code for an "heartbeat answer / action" !!!!!
		//-----------------------------------------------------------
		// start the heartbeat thread
		//-----------------------------------------------------------
		/*
		if (heartbeat->isRunning() == false)
		{
			#ifndef _ARM_ // only include on _non_ ARM environments!
			splash->showMessage(QObject::tr("Starting heartbeat thread..."), splashPosition, splashColor);
			// for refreshing the splash...
			QApplication::processEvents();
			#endif
		
			gui->appendLog("Starting heartbeat thread...", false);
			heartbeat->start();
			gui->appendLog("Heartbeat thread started.");
		}
		*/

		//-----------------------------------------------------------
		// start the sensor thread for reading the sensors)
		//-----------------------------------------------------------
		if (sensorThread->isRunning() == false)
		{
			#ifndef _ARM_ // only include on _non_ ARM environments!
			splash->showMessage(QObject::tr("Starting sensor thread..."), splashPosition, splashColor);
			// for refreshing the splash...
			QApplication::processEvents();
			#endif
			
			gui->appendLog("Starting sensor thread...", false);
			sensorThread->start();
			gui->appendLog("Sensor thread started.");
		}

		#ifndef _ARM_ // only include on _non_ ARM environments!
		//-----------------------------------------------------------
		// start the plot thread ("clock" for plotting the curves)
		//-----------------------------------------------------------
		if (plotThread->isRunning() == false)
		{
			splash->showMessage(QObject::tr("Starting plot thread..."), splashPosition, splashColor);
			// for refreshing the splash...
			QApplication::processEvents();
			
			gui->appendLog("Starting plot thread...", false);
			plotThread->start();
			gui->appendLog("Plot thread started.");
		}
		#endif
	}
	else
	{
		gui->appendLog("<font color=\"#FF0000\">The robot is OFF! Please turn it ON!</font>");
		gui->appendLog("Heartbeat thread NOT started!");
		gui->appendLog("Sensor thread NOT started!");
		gui->appendLog("Plot thread NOT started!");
	}


	//-----------------------------------------------------------
	// check if a joystick is connected
	//-----------------------------------------------------------
	// start the joystick thread
	if (joystick->isRunning() == false)
	{
		#ifndef _ARM_ // only include on _non_ ARM environments!
		splash->showMessage(QObject::tr("Starting joystick thread..."), splashPosition, splashColor);
		// for refreshing the splash...
		QApplication::processEvents();
		#endif
		
		gui->appendLog("Starting joystick thread...", false);
		joystick->start();
		gui->appendLog("Joystick thread started.");
	}

	//----------------------------------------------------------------------------
	// drive in the direction which was emited
	//----------------------------------------------------------------------------
	connect(gui, SIGNAL( drive(unsigned char) ), this, SLOT( drive(unsigned char) ));

	//----------------------------------------------------------------------------
	// connect sensor signals to "show sensor data"
	// (Whenever the sensor data are completely read, show the result in the GUI)
	//----------------------------------------------------------------------------
	connect(sensorThread, SIGNAL(sensorDataComplete()), this, SLOT(showSensorData()));

	#ifdef _TTY_POSIX_ // only include in Linux environments, because OpenCV is not available for Windows (and does not make sense for ARM)
	//----------------------------------------------------------------------------
	// connect sensor contact signals to "show contact alarm"
	// (Whenever the an alarm contact was closed, show the result in the cam image)
	//----------------------------------------------------------------------------
	connect(sensorThread, SIGNAL(contactAlarm(char, bool)), camThread, SLOT(drawContactAlarm(char, bool)));

	//----------------------------------------------------------------------------
	// connect camDataComplete from the cam thread to signal "setCamImage"
	// (Whenever the image is complete, the image is shown in the GUI)
	//----------------------------------------------------------------------------
	connect(camThread, SIGNAL( camDataComplete(IplImage*) ), gui, SLOT( setCamImage(IplImage*) ));
	//connect(camThread, SIGNAL( camDataComplete(QImage*) ), gui, SLOT( setCamImage(QImage*) ));

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
	#endif

	//----------------------------------------------------------------------------
	// connect obstacle check (alarm!) sensor signal to "logical unit"
	//----------------------------------------------------------------------------
	connect(obstCheckThread, SIGNAL(obstacleDetected(int, QDateTime)), this, SLOT(logicalUnit(int, QDateTime)));

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

	#ifndef _ARM_ // only include in _non_ ARM environments!
	//----------------------------------------------------------------------------
	// connect laserThread signal to "dataReceived"
	// (Whenever data were received, the data are shown in the GUI)
	//----------------------------------------------------------------------------
	connect(laserThread, SIGNAL( laserDataCompleteFront(float *, int *) ), gui, SLOT( refreshLaserViewFront(float *, int *) ));
	connect(laserThread, SIGNAL( laserDataCompleteRear(float *, int *) ), gui, SLOT( refreshLaserViewRear(float *, int *) ));
	#endif

	//----------------------------------------------------------------------------
	// connect joystick signals to "show joystick data"
	// (Whenever the joystick is moved or a button is pressed, show the result in the GUI)
	//----------------------------------------------------------------------------
	#ifndef _ARM_ // only include in _non_ ARM environments!
	connect(joystick, SIGNAL(joystickMoved(int, int)), joystickDialog, SLOT(showJoystickAxes(int, int)));
	connect(joystick, SIGNAL(joystickButtonPressed(int, bool)), joystickDialog, SLOT(showJoystickButtons(int, bool)));
	#endif
	connect(joystick, SIGNAL(joystickMoved(int, int)), this, SLOT(executeJoystickCommand(int, int)));
	connect(joystick, SIGNAL(joystickButtonPressed(int, bool)), this, SLOT(executeJoystickCommand(int, bool)));


	#ifndef _ARM_ // only include on _non_ ARM environments!
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
			{
				gui->appendLog(QString("Camera resolution is %1x%2.").arg(camThread->imageWidth()).arg(camThread->imageHeight()));
				// tell the gui the image size and depth
				gui->setCamImageData(camThread->imageWidth(), camThread->imageHeight(), camThread->imagePixelDepth());
			}
		}
	}
	else
	{
		gui->appendLog("Camera thread NOT started!");
	}
	#endif


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
	#ifndef _ARM_ // only include on _non_ ARM environments!
	splash->showMessage(QObject::tr("Searching front laser..."), splashPosition, splashColor);
	// for refreshing the splash...
	QApplication::processEvents();
	#else
	qDebug() << "Searching front laser...";
	#endif
	
	// check FRONT laser
	laserScannerFrontFound = laserThread->isConnected(LASER1);

	#ifndef _ARM_ // only include on _non_ ARM environments!
	splash->showMessage(QObject::tr("Searching rear laser..."), splashPosition, splashColor);
	// for refreshing the splash...
	QApplication::processEvents();
	#else
	qDebug() << "Searching rear laser...";
	#endif
	
	// check REAR laser
	laserScannerRearFound = laserThread->isConnected(LASER2);

	if (laserScannerFrontFound || laserScannerRearFound)
	{
		if (laserScannerFrontFound)
		{
			gui->appendLog("Front laser scanner found.");
		}
		else
		{
			gui->appendLog("Front laser scanner NOT found.");
		}

		if (laserScannerRearFound)
		{
			gui->appendLog("Rear laser scanner found.");
		}
		else
		{
			gui->appendLog("Rear laser scanner NOT found.");
		}

		#ifndef _ARM_ // only include on _non_ ARM environments!
		// TODO: nice exit point and error message
		if (!QGLFormat::hasOpenGL())
		{
			qDebug() << "This system has no OpenGL support" << endl;
			showExitDialog();
		}
		#endif


		// start the laserThread
		if (laserThread->isRunning() == false)
		{
			#ifndef _ARM_ // only include on _non_ ARM environments!
			splash->showMessage(QObject::tr("Starting Laser thread..."), splashPosition, splashColor);
			// for refreshing the splash...
			QApplication::processEvents();
			#endif
			
			gui->appendLog("Starting Laser thread...", false);
			laserThread->start();
			gui->appendLog("Laser thread started.");
		}


		if (obstCheckThread->isRunning() == false)
		{
			#ifndef _ARM_ // only include on _non_ ARM environments!
			splash->showMessage(QObject::tr("Starting obstacle check thread..."), splashPosition, splashColor);
			// for refreshing the splash...
			QApplication::processEvents();
			#endif
			
			gui->appendLog("Starting obstacle check thread...", false);
			obstCheckThread->start();
			gui->appendLog("Obstacle check thread started.");
		}
	}
	else
	{
		#ifndef _ARM_ // only include on _non_ ARM environments!
		// turn off laser splash
		gui->laserSplash(false, LASER1);
		gui->laserSplash(false, LASER2);
		#endif
		gui->appendLog("<font color=\"#FF0000\">NO laser scanners found! Thread NOT started!</font>");
	}


	#ifndef _ARM_ // only include in _non_ ARM environments!
	//------------------------------------------------------------------
	// hide some dialogues
	//------------------------------------------------------------------
	settingsDialog->hide();
	joystickDialog->hide();
	aboutDialog->hide();
	
	//------------------------------------------------------------------
	// for getting the screen resolution
	//------------------------------------------------------------------
	//QDesktopWidget *desktop = QApplication::desktop();

	//------------------------------------------------------------------
	// place gui window at a nice position on the screen
	//------------------------------------------------------------------
	/*
	if (desktop->width() > 1024)
	{
		// move mainWindow to the center of the screen
		gui->move( (desktop->width() - gui->width())/2, (desktop->height() - gui->height())/2 );

		// show the gui
		gui->show();

		// delete the splash screen
		QTimer::singleShot(SPLASHTIME, this, SLOT( finishSplash() ));
	}
	else
	{
	*/
		// resolution too smal for this window. Maximizing...
		// show the main window
		gui->showMaximized();

		// delete the splash screen
		QTimer::singleShot(SPLASHTIME, this, SLOT( finishSplash() ));
	/*
	}
	*/

	// one time init for the laser view
	gui->initLaserView();
	#endif
}


/*
bool Mrs::event(QEvent *event)
{
	if (event->type() == QEvent::KeyPress)
	{
		QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
		qDebug() << "Ate key press" << keyEvent->key();
		return true;
	}
	else
	{
		return false;
	}
}
*/


void Mrs::shutdown()
{
		qDebug("Mrs shutdown...");

		#ifndef _ARM_ // only include on _non_ ARM environments!
		splash->show();
		splash->showMessage(QObject::tr("Shutting down..."), splashPosition, splashColor);
		// for refreshing the splash...
		QApplication::processEvents();
		#endif
		
		// just 4 fun
		if (robotIsOn)
		{
			head->look("NORMAL");
			head->look("DOWN");
		}

		//---------------------------------------------------------------
		// save changes to ini-file (if check box is checked!)
		//---------------------------------------------------------------
		// THIS SETTING HAS TO BE SAVED ALWAYS!
		// "Save the setting, that no settings shoud be saved"
		//
		// save check box status
	#ifndef _ARM_ // only include in _non_ ARM environments!
		inifile1->writeSetting("Config", "saveOnExit", settingsDialog->getCheckBoxSaveSettings());


		if (settingsDialog->getCheckBoxSaveSettings() == Qt::Checked)
		{
			gui->appendLog("Writing settings...");
			#ifndef _ARM_ // only include on _non_ ARM environments!
			splash->showMessage(QObject::tr("Writing settings..."), splashPosition, splashColor);
			// for refreshing the splash...
			QApplication::processEvents();
			#endif
			
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

			gui->appendLog("Settings written.");
		}
	#endif


		// show dialog if set in ini-file
		if (exitDialog == true)
		{
			#ifndef _ARM_ // only include on _non_ ARM environments!
			// ask user if he really wants to exit.
			if (QMessageBox::question(0, "Leaving program...", "Are you sure?", QMessageBox::Yes | QMessageBox::Default, QMessageBox::No | QMessageBox::Escape) == QMessageBox::No)
			{
				//---------
				// if NO
				//---------
				// don't leave! :-)
				return;
			}
			#endif
			// TODO: ask for exit on console!
		}


		// TODO: a universal quit-threads-method
		#ifndef _ARM_ // only include on _non_ ARM environments!
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
		#endif


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


		/*
		#ifdef _TTY_POSIX_
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
		#endif
		*/

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


		#ifndef _ARM_ // only include in _non_ ARM environments!
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
		#endif



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


		/*
		//--------------------------
		// TODO: quit the heartbeat thread
		//--------------------------
		if (heartbeat->isRunning() == true)
		{
			gui->appendLog("Stopping heartbeat thread...");

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
				gui->appendLog("Heartbeat thread stopped.");
			}
			else
			{
				gui->appendLog("Terminating heartbeat thread because it doesn't answer...");
				heartbeat->terminate();
				heartbeat->wait(1000);
				gui->appendLog("Heartbeat thread terminated.");
			}
		}
		*/
/*
		removed for still "head looking down"!!
		//-------------------------------------------------------
		// Last init for the robots circuits
		//-------------------------------------------------------
		gui->appendLog("Last circuit init...");
		if (robotIsOn)
		{
			circuit1->initCircuit();
		}
*/
		//-----------------------------
		// close serial port to mc
		//-----------------------------
		gui->appendLog("Closing serial port to microcontroller...");
		interface1->closeComPort();
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
	#ifndef _ARM_ // only include on _non_ ARM environments!
	delete camThread;
	#endif
	delete joystick;
	#ifndef _ARM_ // only include in _non_ ARM environments!
	delete plotThread;
	#endif
	delete inifile1;
	delete obstCheckThread;
	delete servos;
	delete motors;
	delete sensorThread;
	// TODO: delete heartbeat;
	delete circuit1;
	delete interface1;
	#ifndef _ARM_ // only include in _non_ ARM environments!
	delete aboutDialog;
	delete joystickDialog;
	delete settingsDialog;
	#endif
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


void Mrs::showSplashMessage(QString text)
{
	#ifndef _ARM_ // only include on _non_ ARM environments!
	splash->showMessage(text, splashPosition, splashColor);
	// for refreshing the splash...
	QApplication::processEvents();
	#else
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
	#endif
}


void Mrs::finishSplash()
{
	#ifndef _ARM_ // only include on _non_ ARM environments!
	splash->finish(gui);
	#endif
}


void Mrs::logicalUnit(int sensorAlarm, QDateTime timestamp)
{
	//Q_UNUSED(timestamp);

	//static int alarmCounter = 0;
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
			//gui->appendLog("No obstacle in front of any sensor. :-)");
			//gui->appendLog("Driving forward...");

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
				gui->appendLog("<b>Obstacle front left. Turning right.</b>");

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
				gui->appendLog("<b>Obstacle front right. Turning left.</b>");

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
			gui->appendLog("<b>Obstacles everywhere in front of the robot. Waiting.</b>");

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


void Mrs::faceTracking(int faces, int faceX, int faceY, int faceRadius)
{
#ifndef _ARM_ // only include on _non_ ARM environments!
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

		if ( robotIsOn && (faceTrackingIsEnabled) )
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
		if ( robotIsOn && (faceTrackingIsEnabled) )
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
		if ( robotIsOn && (faceTrackingIsEnabled) )
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
		if ( robotIsOn && (faceTrackingIsEnabled) )
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
		if ( robotIsOn && (faceTrackingIsEnabled) )
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
		if ( robotIsOn && (faceTrackingIsEnabled) )
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
		if ( robotIsOn && (faceTrackingIsEnabled) )
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
		if ( robotIsOn && (faceTrackingIsEnabled) )
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
		if ( robotIsOn && (faceTrackingIsEnabled) )
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
#else
	// now usage on ARM and Windoze systems
	Q_UNUSED (faces)
	Q_UNUSED (faceX)
	Q_UNUSED (faceY)
	Q_UNUSED (faceRadius)
#endif
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
			gui->appendLog("FORWARD");
			gui->showMotorStatus(MOTOR1, SAME, COUNTERCLOCKWISE);
			gui->showMotorStatus(MOTOR2, SAME, COUNTERCLOCKWISE);
			gui->showMotorStatus(MOTOR3, SAME, COUNTERCLOCKWISE);
			gui->showMotorStatus(MOTOR4, SAME, COUNTERCLOCKWISE);
			motors->motorControl(MOTOR1, SAME, COUNTERCLOCKWISE);
			motors->motorControl(MOTOR2, SAME, COUNTERCLOCKWISE);
			motors->motorControl(MOTOR3, SAME, COUNTERCLOCKWISE);
			motors->motorControl(MOTOR4, SAME, COUNTERCLOCKWISE);
			return;
			break;
		case BACKWARD:
			gui->appendLog("BACKWARD");
			gui->showMotorStatus(MOTOR1, SAME, CLOCKWISE);
			gui->showMotorStatus(MOTOR2, SAME, CLOCKWISE);
			gui->showMotorStatus(MOTOR3, SAME, CLOCKWISE);
			gui->showMotorStatus(MOTOR4, SAME, CLOCKWISE);
			motors->motorControl(MOTOR1, SAME, CLOCKWISE);
			motors->motorControl(MOTOR2, SAME, CLOCKWISE);
			motors->motorControl(MOTOR3, SAME, CLOCKWISE);
			motors->motorControl(MOTOR4, SAME, CLOCKWISE);
			return;
			break;
		case LEFT:
			gui->appendLog("LEFT");
			gui->showMotorStatus(MOTOR1, SAME, CLOCKWISE);
			gui->showMotorStatus(MOTOR2, SAME, COUNTERCLOCKWISE);
			gui->showMotorStatus(MOTOR3, SAME, COUNTERCLOCKWISE);
			gui->showMotorStatus(MOTOR4, SAME, CLOCKWISE);
			
			motors->motorControl(MOTOR1, SAME, CLOCKWISE);
			motors->motorControl(MOTOR2, SAME, COUNTERCLOCKWISE);
			motors->motorControl(MOTOR3, SAME, COUNTERCLOCKWISE);
			motors->motorControl(MOTOR4, SAME, CLOCKWISE);
			return;
			break;
		case RIGHT:
			gui->appendLog("RIGHT");
			gui->showMotorStatus(MOTOR1, SAME, COUNTERCLOCKWISE);
			gui->showMotorStatus(MOTOR2, SAME, CLOCKWISE);
			gui->showMotorStatus(MOTOR3, SAME, CLOCKWISE);
			gui->showMotorStatus(MOTOR4, SAME, COUNTERCLOCKWISE);
			
			motors->motorControl(MOTOR1, SAME, COUNTERCLOCKWISE);
			motors->motorControl(MOTOR2, SAME, CLOCKWISE);
			motors->motorControl(MOTOR3, SAME, CLOCKWISE);
			motors->motorControl(MOTOR4, SAME, COUNTERCLOCKWISE);
			return;
			break;
		case TURNLEFT:
			gui->appendLog("TURNLEFT");
			gui->showMotorStatus(MOTOR1, SAME, CLOCKWISE);
			gui->showMotorStatus(MOTOR2, SAME, COUNTERCLOCKWISE);
			gui->showMotorStatus(MOTOR3, SAME, CLOCKWISE);
			gui->showMotorStatus(MOTOR4, SAME, COUNTERCLOCKWISE);
			
			motors->motorControl(MOTOR1, SAME, CLOCKWISE);
			motors->motorControl(MOTOR2, SAME, COUNTERCLOCKWISE);
			motors->motorControl(MOTOR3, SAME, CLOCKWISE);
			motors->motorControl(MOTOR4, SAME, COUNTERCLOCKWISE);
			return;
			break;
		case TURNRIGHT:
			gui->appendLog("TURNRIGHT");
			gui->showMotorStatus(MOTOR1, SAME, COUNTERCLOCKWISE);
			gui->showMotorStatus(MOTOR2, SAME, CLOCKWISE);
			gui->showMotorStatus(MOTOR3, SAME, COUNTERCLOCKWISE);
			gui->showMotorStatus(MOTOR4, SAME, CLOCKWISE);
			
			motors->motorControl(MOTOR1, SAME, COUNTERCLOCKWISE);
			motors->motorControl(MOTOR2, SAME, CLOCKWISE);
			motors->motorControl(MOTOR3, SAME, COUNTERCLOCKWISE);
			motors->motorControl(MOTOR4, SAME, CLOCKWISE);
			return;
			break;
		case START:
			if (robotIsOn)
			{
				robotDrives = true;
				gui->appendLog("Starting to drive forward...");
				gui->appendLog("START");
				// set the motors to "drive FORWARD"
				gui->showMotorStatus(MOTOR1, ON, CLOCKWISE);
				gui->showMotorStatus(MOTOR2, ON, CLOCKWISE);
				gui->showMotorStatus(MOTOR3, ON, CLOCKWISE);
				gui->showMotorStatus(MOTOR4, ON, CLOCKWISE);
				motors->motorControl(MOTOR1, ON, CLOCKWISE);
				motors->motorControl(MOTOR2, ON, CLOCKWISE);
				motors->motorControl(MOTOR3, ON, CLOCKWISE);
				motors->motorControl(MOTOR4, ON, CLOCKWISE);
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
			gui->appendLog("WAIT");
			gui->showMotorStatus(MOTOR1, OFF, SAME);
			gui->showMotorStatus(MOTOR2, OFF, SAME);
			gui->showMotorStatus(MOTOR3, OFF, SAME);
			gui->showMotorStatus(MOTOR4, OFF, SAME);
			// turning motors off
			motors->motorControl(MOTOR1, OFF, SAME);
			motors->motorControl(MOTOR2, OFF, SAME);
			motors->motorControl(MOTOR3, OFF, SAME);
			motors->motorControl(MOTOR4, OFF, SAME);
			//
			// Don't stop the motThread (PWM)!
			// Only switching motors off!
			//
			return;
			break;
		case STOP:
			gui->appendLog("STOP");
			gui->showMotorStatus(MOTOR1, OFF, SAME);
			gui->showMotorStatus(MOTOR2, OFF, SAME);
			gui->showMotorStatus(MOTOR3, OFF, SAME);
			gui->showMotorStatus(MOTOR4, OFF, SAME);
			// turning motors off
			motors->motorControl(MOTOR1, OFF, SAME);
			motors->motorControl(MOTOR2, OFF, SAME);
			motors->motorControl(MOTOR3, OFF, SAME);
			motors->motorControl(MOTOR4, OFF, SAME);
			//
			// Don't stop the motThread (clock)!
			// Only switching motors off!
			//
			robotDrives = false;
			return;
			break;
		case MOTOR1FW: // for the test widget in the GUI!!
			if (robotIsOn)
			{
				gui->appendLog("Motor 1 forward");
				gui->showMotorStatus(MOTOR1, ON, COUNTERCLOCKWISE);
				motors->motorControl(MOTOR1, ON, COUNTERCLOCKWISE);
			}
			break;
		case MOTOR1BW: // for the test widget in the GUI!!
			if (robotIsOn)
			{
				gui->appendLog("Motor 1 backward");
				gui->showMotorStatus(MOTOR1, ON, CLOCKWISE);
				motors->motorControl(MOTOR1, ON, CLOCKWISE);
			}
			break;
		case MOTOR1OFF: // for the test widget in the GUI!!
			if (robotIsOn)
			{
				gui->appendLog("Motor 1 OFF");
				gui->showMotorStatus(MOTOR1, OFF, SAME);
				motors->motorControl(MOTOR1, OFF, SAME);
			}
			break;
		case MOTOR2FW: // for the test widget in the GUI!!
			if (robotIsOn)
			{
				gui->appendLog("Motor 2 forward");
				gui->showMotorStatus(MOTOR2, ON, COUNTERCLOCKWISE);
				motors->motorControl(MOTOR2, ON, COUNTERCLOCKWISE);
			}
			break;
		case MOTOR2BW: // for the test widget in the GUI!!
			if (robotIsOn)
			{
				gui->appendLog("Motor 2 backward");
				gui->showMotorStatus(MOTOR2, ON, CLOCKWISE);
				motors->motorControl(MOTOR2, ON, CLOCKWISE);
			}
			break;
		case MOTOR2OFF: // for the test widget in the GUI!!
			if (robotIsOn)
			{
				gui->appendLog("Motor 2 OFF");
				gui->showMotorStatus(MOTOR2, OFF, SAME);
				motors->motorControl(MOTOR2, OFF, SAME);
			}
			break;
		case MOTOR3FW: // for the test widget in the GUI!!
			if (robotIsOn)
			{
				gui->appendLog("Motor 3 forward");
				gui->showMotorStatus(MOTOR3, ON, COUNTERCLOCKWISE);
				motors->motorControl(MOTOR3, ON, COUNTERCLOCKWISE);
			}
			break;
		case MOTOR3BW: // for the test widget in the GUI!!
			if (robotIsOn)
			{
				gui->appendLog("Motor 3 backward");
				gui->showMotorStatus(MOTOR3, ON, CLOCKWISE);
				motors->motorControl(MOTOR3, ON, CLOCKWISE);
			}
			break;
		case MOTOR3OFF: // for the test widget in the GUI!!
			if (robotIsOn)
			{
				gui->appendLog("Motor 3 OFF");
				gui->showMotorStatus(MOTOR3, OFF, SAME);
				motors->motorControl(MOTOR3, OFF, SAME);
			}
			break;
		case MOTOR4FW: // for the test widget in the GUI!!
			if (robotIsOn)
			{
				gui->appendLog("Motor 4 forward");
				gui->showMotorStatus(MOTOR4, ON, COUNTERCLOCKWISE);
				motors->motorControl(MOTOR4, ON, COUNTERCLOCKWISE);
			}
			break;
		case MOTOR4BW: // for the test widget in the GUI!!
			if (robotIsOn)
			{
				gui->appendLog("Motor 4 backward");
				gui->showMotorStatus(MOTOR4, ON, CLOCKWISE);
				motors->motorControl(MOTOR4, ON, CLOCKWISE);
			}
			break;
		case MOTOR4OFF: // for the test widget in the GUI!!
			if (robotIsOn)
			{
				gui->appendLog("Motor 4 OFF");
				gui->showMotorStatus(MOTOR4, OFF, SAME);
				motors->motorControl(MOTOR4, OFF, SAME);
			}
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
			// everything okay
			laserThread->setSerialPort(LASER1, serialPortLaserscannerFront);
			gui->appendLog(QString("Front laser scanner set to <b>%1</b>.").arg(serialPortLaserscannerFront));
			
			//---------------------------------------------------------------------
			// read next laser setting
			mountingLaserscanner = inifile1->readString("Config", "mountingLaserscannerFront");
		
			if (mountingLaserscanner == "error2")
			{
				laserThread->setMounting(LASER1, "normal");
				gui->appendLog("<font color=\"#FF0000\">ini-file is not writeable!</font>");
			}
			else
			{
				if (mountingLaserscanner == "error1")
				{
					laserThread->setMounting(LASER1, "normal");
					gui->appendLog("<font color=\"#FF0000\">Value \"mountingLaserscannerFront\" not found in ini-file!</font>");
				}
				else
				{
					// everything okay
					laserThread->setMounting(LASER1, mountingLaserscanner);
					gui->appendLog(QString("Front laser scanner mounting set to <b>%1</b>.").arg(mountingLaserscanner));
				}
			}
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
			// everything okay
			laserThread->setSerialPort(LASER2, serialPortLaserscannerRear);
			gui->appendLog(QString("Rear laser scanner set to <b>%1</b>.").arg(serialPortLaserscannerRear));
			
			//---------------------------------------------------------------------
			// read next laser setting
			mountingLaserscanner = inifile1->readString("Config", "mountingLaserscannerRear");
		
			if (mountingLaserscanner == "error2")
			{
				laserThread->setMounting(LASER2, "normal");
				gui->appendLog("<font color=\"#FF0000\">ini-file is not writeable!</font>");
			}
			else
			{
				if (mountingLaserscanner == "error1")
				{
					laserThread->setMounting(LASER2, "normal");
					gui->appendLog("<font color=\"#FF0000\">Value \"mountingLaserscannerRear\" not found in ini-file!</font>");
				}
				else
				{
					// everything okay
					laserThread->setMounting(LASER2, mountingLaserscanner);
					gui->appendLog(QString("Rear laser scanner mounting set to <b>%1</b>.").arg(mountingLaserscanner));
				}
			}
		}
	}

	//---------------------------------------------------------------------
	// read setting
    switch (inifile1->readSetting("Config", "useCamera"))
	{
		case -2:
			gui->appendLog("<font color=\"#FF0000\">ini-file is not writeable!</font>");
			break;
		case -1:
            gui->appendLog("<font color=\"#FF0000\">Value \"useCamera\"not found in ini-file!</font>");
			break;
		case 0:
            useCamera = false;
			#ifndef _ARM_ // only include on _non_ ARM environments!
            // turning "off" camera
            camThread->setCameraDevice(-2);
            gui->disableCamera();
            #endif
            gui->appendLog("<font color=\"#FF0000\">No camera usage! (see ini-file)</font>");
            break;
		case 1:
            useCamera = true;
			break;
	}

	#ifndef _ARM_ // only include on _non_ ARM environments!
    if (useCamera)
	{
		//---------------------------------------------------------------------
		// read setting
		int cameraDevice = inifile1->readSetting("Config", "cameraDevice");

		if (cameraDevice == -2)
		{
			camThread->setCameraDevice(-2);
			gui->appendLog("<font color=\"#FF0000\">ini-file is not writeable!</font>");
		}
		else
		{
			if (cameraDevice == -1)
			{
				camThread->setCameraDevice(-2);
				gui->appendLog("<font color=\"#FF0000\">Value \"cameraDevice\" not found in ini-file!</font>");
			}
			else
			{
				//
				// everything okay
				//
				// set it in the cam thread
				camThread->setCameraDevice(cameraDevice);

				gui->appendLog(QString("Camera file set to <b>%1</b>.").arg(cameraDevice));


				//---------------------------------------------------------------------
				// read setting
				QString haarClassifierCascade = inifile1->readString("Config", "haarClassifierCascade");

				if (haarClassifierCascade == "error2")
				{
					camThread->setCascadePath("none");
					gui->appendLog("<font color=\"#FF0000\">ini-file is not writeable!</font>");
				}
				else
				{
					if (haarClassifierCascade == "error1")
					{
						camThread->setCascadePath("none");
						gui->appendLog("<font color=\"#FF0000\">Value \"haarClassifierCascade\" not found in ini-file!</font>");
					}
					else
					{
						//
						// everything okay
						//
						// set it in the cam thread
						camThread->setCascadePath(haarClassifierCascade);
						gui->appendLog(QString("Haar classifier cascade file set to<br><b>%1</b>.").arg(haarClassifierCascade));

						splash->showMessage(QObject::tr("Initialising camera..."), splashPosition, splashColor);
						// for refreshing the splash...
						QApplication::processEvents();
						
						// initialise the cam
						if (camThread->init())
						{
							gui->appendLog("Camera initialised.");
						}
						else
						{
							gui->appendLog("Error initialising camera.");
						}
					}
				}
				//---------------------------------------------------------------------
			}
		}
	} // dont use camera!
	#endif

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
			#ifndef _ARM_ // only include in _non_ ARM environments!
			// TODO: put this setting somewehere
			// uncheck checkbox
			settingsDialog->setCheckBoxSaveSettings(Qt::Unchecked);
			#endif
			break;
		case Qt::Checked:
			#ifndef _ARM_ // only include in _non_ ARM environments!
			// TODO: put this setting somewehere
			// set checkbox
			settingsDialog->setCheckBoxSaveSettings(Qt::Checked);
			#endif
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
			#ifndef _ARM_ // only include in _non_ ARM environments!
			// TODO: put this setting somewehere
			// set slider to the read value
			settingsDialog->setSliderObstacleValue(minObstacleDistance);
			#endif
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
			#ifndef _ARM_ // only include in _non_ ARM environments!
			// TODO: put this setting somewehere
			// set slider to the read value
			settingsDialog->setSliderObstacleLaserScannerValue(minObstacleDistanceLaserScanner);
			#endif
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
			#ifndef _ARM_ // only include in _non_ ARM environments!
			// TODO: put this setting somewehere
			// set slider to the read value
			settingsDialog->setSliderRobotSlot(robotSlot);
			#endif
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
			#ifndef _ARM_ // only include in _non_ ARM environments!
			// TODO: put this setting somewehere
			// set slider to the read value
			settingsDialog->setSliderStraightForwardDeviation(straightForwardDeviation);
			#endif
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

			#ifndef _ARM_ // only include in _non_ ARM environments!
			// TODO: put this setting somewehere
			// set slider to the read value
			settingsDialog->setSliderMotorSpeed(1, mot1Speed);
			#endif
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

			#ifndef _ARM_ // only include in _non_ ARM environments!
			// TODO: put this setting somewehere
			// set slider to the read value
			settingsDialog->setSliderMotorSpeed(2, mot2Speed);
			#endif
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

			#ifndef _ARM_ // only include in _non_ ARM environments!
			// TODO: put this setting somewehere
			// set slider to the read value
			settingsDialog->setSliderMotorSpeed(3, mot3Speed);
			#endif
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

			#ifndef _ARM_ // only include in _non_ ARM environments!
			// TODO: put this setting somewehere
			// set slider to the read value
			settingsDialog->setSliderMotorSpeed(4, mot4Speed);
			#endif
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

			#ifndef _ARM_ // only include in _non_ ARM environments!
			// TODO: put this setting somewehere
			// set slider to the read value
			settingsDialog->setSliderMinimumSpeed(minimumSpeed);
			#endif
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

			#ifndef _ARM_ // only include in _non_ ARM environments!
			// TODO: put this setting somewehere
			// set slider to the read value
			settingsDialog->setSliderMaximumSpeed(maximumSpeed);
			#endif
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
				//gui->appendLog(QString("%1 set to <b>%2</b>.").arg(settingName).arg(settingValue));
				break;
		}
	}
	gui->appendLog("Servo start settings read and set.");

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
				//gui->appendLog(QString("%1 set to <b>%2</b>.").arg(settingName).arg(settingValue));
				break;
		}
	}
	gui->appendLog("Servo end settings read and set.");
	//---------------------------------------------------------------------
	// read servo MIN settings
	for (int servo=0; servo<NUMBEROFSERVOS; servo++)
	{
		QString settingName = QString("servo%1").arg(servo+1).append("MinPos");
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
				servos->setServoPosition(servo, SVMIN, settingValue);

				// show text
				//gui->appendLog(QString("%1 set to <b>%2</b>.").arg(settingName).arg(settingValue));
				break;
		}
	}
	gui->appendLog("Servo min. settings read and set.");
	//---------------------------------------------------------------------
	// read servo MAX settings
	for (int servo=0; servo<NUMBEROFSERVOS; servo++)
	{
		QString settingName = QString("servo%1").arg(servo+1).append("MaxPos");
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
				servos->setServoPosition(servo, SVMAX, settingValue);

				// show text
				//gui->appendLog(QString("%1 set to <b>%2</b>.").arg(settingName).arg(settingValue));
				break;
		}
	}
	gui->appendLog("Servo max. settings read and set.");
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
				//gui->appendLog(QString("%1 set to <b>%2</b>.").arg(settingName).arg(settingValue));
				break;
		}
	}
	gui->appendLog("Servo default settings read and set.");
}


void Mrs::enableRemoteControlListening(bool state)
{
	// store the state gobal to Mrs
	robotRemoteMode = state;

	if (state == true)
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
	}
	else
	{
		if (netThread->isRunning() == true)
		{
			gui->appendLog("Stopping network thread...", false);
			netThread->stop();
			gui->appendLog("Network thread stopped.");
		}
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
			#ifndef _ARM_ // only include in _non_ ARM environments!
			// TODO: put this setting somewehere
			settingsDialog->setSliderMotorSpeed(1, newSpeed);
			#endif
			return;
		}


		if (command == "increasespeedmotor2")
		{
			gui->appendLog(tr("<font color=\"#0000FF\">Executing remote command \"%1\".</font>").arg(command));

			int newSpeed = motors->getMotorSpeed(2) + 1;
			motors->setMotorSpeed(2, newSpeed);
			#ifndef _ARM_ // only include in _non_ ARM environments!
			// TODO: put this setting somewehere
			settingsDialog->setSliderMotorSpeed(2, newSpeed);
			#endif
			return;
		}


		if (command == "reducespeedmotor1")
		{
			gui->appendLog(tr("<font color=\"#0000FF\">Executing remote command \"%1\".</font>").arg(command));

			int newSpeed = motors->getMotorSpeed(1) - 1;
			motors->setMotorSpeed(1, newSpeed);
			#ifndef _ARM_ // only include in _non_ ARM environments!
			// TODO: put this setting somewehere
			settingsDialog->setSliderMotorSpeed(1, newSpeed);
			#endif
			return;
		}


		if (command == "reducespeedmotor2")
		{
			gui->appendLog(tr("<font color=\"#0000FF\">Executing remote command \"%1\".</font>").arg(command));

			int newSpeed = motors->getMotorSpeed(2) - 1;
			motors->setMotorSpeed(2, newSpeed);
			#ifndef _ARM_ // only include in _non_ ARM environments!
			// TODO: put this setting somewehere
			settingsDialog->setSliderMotorSpeed(2, newSpeed);
			#endif
			return;
		}
		
		if (command == "shutdown")
		{
			gui->appendLog(tr("<font color=\"#0000FF\">Executing remote command \"%1\".</font>").arg(command));
			emit shutdown();
			return;
		}
	}
}


void Mrs::executeJoystickCommand(int axisNumber, int axisValue)
{
	//
	// Y axis
	//
	if (axisNumber == JOYSTICKAXISY)
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
				#ifndef _ARM_ // only include in _non_ ARM environments!
				// TODO: put this setting somewehere
				//speed = (axisValue / JOYSTICKDIVISOR);
				settingsDialog->setSliderMotorSpeed( 1, (axisValue / JOYSTICKDIVISOR) );
				settingsDialog->setSliderMotorSpeed( 2, (axisValue / JOYSTICKDIVISOR) );
				#endif

				if (robotIsOn)
				{
					motors->setMotorSpeed( 1, (axisValue / JOYSTICKDIVISOR) );
					motors->setMotorSpeed( 2, (axisValue / JOYSTICKDIVISOR) );

					if (robotDrives == false)
					{
						drive(START);
					}

					drive(BACKWARD);
				}
			}


			//==================
			// eye test mode
			//==================
			if (eyeTestMode==true)
			{
				// eyes down
				servos->setServoPosition( SERVO2, SVCURRENT, (axisValue / JOYSTICKDIVISOR) );
				servos->setServoPosition( SERVO5, SVCURRENT, (axisValue / JOYSTICKDIVISOR) );

				if (robotIsOn)
				{
					servos->moveServo(SERVO2, servos->getServoPosition(SERVO2));
					servos->moveServo(SERVO5, servos->getServoPosition(SERVO5));
				}
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
				#ifndef _ARM_ // only include in _non_ ARM environments!
				// TODO: put this setting somewehere
				//speed = (-axisValue / JOYSTICKDIVISOR);
				settingsDialog->setSliderMotorSpeed( 1, (-axisValue / JOYSTICKDIVISOR) );
				settingsDialog->setSliderMotorSpeed( 2, (-axisValue / JOYSTICKDIVISOR) );
				#endif

				if (robotIsOn)
				{
					motors->setMotorSpeed( 1, (-axisValue / JOYSTICKDIVISOR) );
					motors->setMotorSpeed( 2, (-axisValue / JOYSTICKDIVISOR) );

					if (robotDrives == false)
					{
						drive(START);
					}
	
					drive(FORWARD);
				}
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

				if (robotIsOn)
				{
					servos->moveServo(SERVO2, servos->getServoPosition(SERVO2));
					servos->moveServo(SERVO5, servos->getServoPosition(SERVO5));
				}
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
				if (robotIsOn)
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
				#ifndef _ARM_ // only include in _non_ ARM environments!
				// TODO: put this setting somewehere
				//speed = (axisValue / JOYSTICKDIVISOR);
				settingsDialog->setSliderMotorSpeed( 1, (axisValue / JOYSTICKDIVISOR) );
				settingsDialog->setSliderMotorSpeed( 2, (axisValue / JOYSTICKDIVISOR) );
				#endif
				
				if (robotIsOn)
				{
					motors->setMotorSpeed( 1, (axisValue / JOYSTICKDIVISOR) );
					motors->setMotorSpeed( 2, (axisValue / JOYSTICKDIVISOR) );
	
					if (robotDrives == false)
					{
						drive(START);
					}
	
					drive(RIGHT);
				}
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
				#ifndef _ARM_ // only include in _non_ ARM environments!
				// TODO: put this setting somewehere
				//speed = (-axisValue / JOYSTICKDIVISOR);
				settingsDialog->setSliderMotorSpeed( 1, (-axisValue / JOYSTICKDIVISOR) );
				settingsDialog->setSliderMotorSpeed( 2, (-axisValue / JOYSTICKDIVISOR) );
				#endif
				
				if (robotIsOn)
				{
					motors->setMotorSpeed( 1, (-axisValue / JOYSTICKDIVISOR) );
					motors->setMotorSpeed( 2, (-axisValue / JOYSTICKDIVISOR) );
		
					if (robotDrives == false)
					{
						drive(START);
					}
		
					drive(LEFT);
				}
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
				if (robotIsOn)
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
				if (robotIsOn)
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
				if (robotIsOn)
					motors->motorControl(MOTOR4, ON, CLOCKWISE);
				//gui->appendLog("motor 4 on CW");
			}

			//------------------
			// camera down [tilt]
			//------------------
			if (axisValue < 0)
			{
				if (robotIsOn)
				{
					motors->motorControl(MOTOR4, ON, COUNTERCLOCKWISE);
					//gui->appendLog("motor 4 on CCW");
				}
			}

			// move, when button is pressed
			if (axisValue != 0)
			{
				if (robotIsOn)
				{
					//gui->appendLog("Tilting Cam...");
				}
			}

			// stop, when button is pressed!
			if (axisValue == 0)
			{
				if (robotIsOn)
				{
					//gui->appendLog("Tilt stop.");
					motors->motorControl(MOTOR4, OFF, SAME);
				}
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
			if (robotIsOn)
			{
				if (robotDrives == false)
				{
					// TODO: check if this makes sense...
					drive(START);
				}

				drive(BACKWARD);
			}
		}

		//------------------------------------------------------
		// drive forward (Y5 axis/button up)
		//------------------------------------------------------
		if (axisValue < 0)
		{
			if (robotIsOn)
			{
				if (robotDrives == false)
				{
					// TODO: check if this makes sense...
					drive(START);
				}

				drive(FORWARD);
			}
		}

		if (axisValue == 0)
		{
			if (robotIsOn)
			{
				drive(WAIT);
			}
		}

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
				if (robotIsOn)
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
				if (robotIsOn)
				{
					motors->motorControl(MOTOR3, ON, CLOCKWISE);
					//gui->appendLog("motor 3 on CW");
				}
			}

			//------------------
			// camera left [pan]
			//------------------
			if (axisValue < 0)
			{
				if (robotIsOn)
				{
					motors->motorControl(MOTOR3, ON, COUNTERCLOCKWISE);
					//gui->appendLog("motor 3 on CCW");
				}
			}

			// move, when button is pressed
			if (axisValue != 0)
			{
				if (robotIsOn)
				{
					//gui->appendLog("Panning Cam...");
				}
			}

			// stop, when button is pressed!
			if (axisValue == 0)
			{
				if (robotIsOn)
				{
					if (robotIsOn)
					{
						motors->motorControl(MOTOR3, OFF, SAME);
						//gui->appendLog("Pan stop.");
					}
				}
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
			if (robotIsOn)
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
			}
			return;
		}
		
		//------------------------------------------------------
		// drive left (X5 axis/button left)
		//------------------------------------------------------
		if (axisValue < 0)
		{
			if (robotIsOn)
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
			}
			return;
		}
		
		//------------------------------------------------------
		// wait (X5 axis/button)
		//------------------------------------------------------
		if (axisValue == 0)
		{
			if (robotIsOn)
			{
				drive(WAIT);
			}
		}

		return;
	}
}


void Mrs::executeJoystickCommand(int buttonNumber, bool buttonState)
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
					gui->appendLog("<font color=\"#0000FF\">Eye test mode enabled.</front>");
				}
				else
				{
					eyeTestMode=false;
					head->look("FORWARD");
					head->look("NORMAL");
					gui->appendLog("<font color=\"#0000FF\">Eye test mode disabled.</front>");
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
					gui->appendLog("<font color=\"#0000FF\">Servo test mode ON.</front>");
					gui->appendLog(QString("Servo %1 selected.").arg(currentTestServo+1));
					// TODO: timing problem, when emitting speak signal.
					// restults in "Error reading joystick device!"
					emit speak("Servo test mode");
				}
				else
				{
					servoTestMode = false;
					gui->appendLog("<font color=\"#0000FF\">Servo test mode OFF.</front>");
					emit speak("Test mode off");
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
					gui->appendLog("<font color=\"#0000FF\">Mecanum test mode enabled.</front>");
				}
				else
				{
					mecanumDriveMode=false;
					gui->appendLog("<font color=\"#0000FF\">Mecanum test mode disabled.</front>");
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
					testDriveMode = true;
					gui->appendLog("<font color=\"#0000FF\">Test drive mode ON.</front>");
					// TODO: timing problem, when emitting speak signal.
					// restults in "Error reading joystick device!"
					emit speak("Test drive mode");
				}
				else
				{
					testDriveMode = false;
					gui->appendLog("<font color=\"#0000FF\">Test drive mode OFF.</front>");
					emit speak("Test drive mode off.");
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
					gui->appendLog("<font color=\"#0000FF\">Camera test mode ON.</front>");
				}
				else
				{
					cameraTestMode = false;
					gui->appendLog("<font color=\"#0000FF\">Camera test mode OFF.</front>");
				}
				toggle11 = !toggle11;
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
			gui->appendLog("Started.");
		}

		if (sensorThread->isRunning() == false)
		{
			gui->appendLog("Starting Sensor thread...", false);
			sensorThread->start();
			gui->appendLog("Started.");
		}

		#ifndef _ARM_ // only include in _non_ ARM environments!
		if (plotThread->isRunning() == false)
		{
			gui->appendLog("Starting plot thread...", false);
			plotThread->start();
			gui->appendLog("Started.");
		}
		#endif

		if (obstCheckThread->isRunning() == false)
		{
			gui->appendLog("Starting obstacle check thread...", false);
			obstCheckThread->start();
			gui->appendLog("Started.");
		}
	}
	else
	{
		// if robot is OFF stop all threads which read from the circuit!
		if (robotIsOn == false)
		{
			sensorThread->stop();
			gui->appendLog("Sensor thread stopped.");
		}
		gui->appendLog("<font color=\"#0000FF\">Simulation mode disabled.</font>");
	}
}


void Mrs::setRobotState(bool state)
{
	robotIsOn = state;
}


void Mrs::speak(QString text)
{
	Q_UNUSED(text);
	
	#ifdef _TTY_POSIX_ // only available on Linux! :-)
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

	// file: ~/.festivalrc
	// (Parameter.set 'Audio_Command "aplay -q -c 1 -t raw -f s16 -r $SR $FILE")
	// (Parameter.set 'Audio_Method 'Audio_Command)
	// for speaking while audio is already in use!
	//
	// say the cleaned text
	festival_say_text(textForFestival);
	#endif
}


void Mrs::test()
{
	static bool toggle = OFF;


	if (toggle == OFF)
	{
		toggle = ON;
		//head->look("LEFT");
	}
	else
	{
		toggle = OFF;
		//head->look("RIGHT");
	}

	motors->flashlight(toggle);


	#ifdef _TTY_POSIX_
	// Say some text;
	QDateTime now = QDateTime::currentDateTime();
//	emit speak(tr("Hello Markus. Today it's the %1 of %2, %3. The time is %4:%5.").arg(now.toString("d")).arg(now.toString("MMMM")).arg(now.toString("yyyy")).arg(now.toString("h")).arg(now.toString("m")));
	#endif

}
