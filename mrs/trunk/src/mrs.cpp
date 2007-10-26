#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdio.h>
#include <stdlib.h>


#include "mrs.h"

//
// 01.01.2007: 4550 lines of code
//

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
	// FixMe: SIOD ERROR: the currently assigned stack limit has been exceded 
	//speakThread = new SpeakThread();
	#endif
	interface1 = new InterfaceAvr();
	circuit1 = new Circuit(interface1);
	motors = new Motor(interface1);
	servos = new Servo(interface1);
	sensorThread = new SensorThread(interface1, servos);
	laserThread = new LaserThread();
	obstCheckThread = new ObstacleCheckThread(sensorThread, laserThread);
	plotThread = new PlotThread(sensorThread);
	inifile1 = new Inifile();
	cam1 = new CamThread();
	netThread = new NetworkThread();
	
	
	gui1 = new Gui(this, sensorThread, plotThread, obstCheckThread, circuit1, cam1, motors, servos, netThread, laserThread);


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
		gui1->move( (desktop->width() - gui1->width())/2, (desktop->height() - gui1->height())/2 );

		// show the main window
		gui1->show();
		
		// delete the splash screen
		QTimer::singleShot(SPLASHTIME, this, SLOT( finishSplash() ));
	/*
	}
	else
	{
		// resolution too smal for this window. Maximizing...
		// show the main window
		gui1->showMaximized();
	}
	*/


	//------------------------------------------------------------------
	// nomen est omen
	//------------------------------------------------------------------
	serialPortMicrocontroller = "error1";
	robotIsOn = false;
	robotDrives = false;
	mot1Speed = 0;
	mot2Speed = 0;
	minObstDist = 0;
	noLaserScanner = false;
	robotSimulationMode = false;
	
	//------------------------------------------------------------------
	// Set the number format to "," for comma and 1000 separator to "."
	// For example: 1.234,00 EUR
	//------------------------------------------------------------------
	QLocale::setDefault(QLocale::German);
	commaSeparator = ",";


	//----------------------------------------------------------------------------
	// Initialize the speech engine festival
	//----------------------------------------------------------------------------
	#ifdef _TTY_POSIX_
	//      1 = we want the festival init files loaded
	// 500000 = default scheme heap size
	festival_initialize(1, 500000);
	#endif

	
	//--------------------------------------------------------------------------
	// Check for the current programm path 
	//--------------------------------------------------------------------------
	gui1->appendLog(QString("Current programm path: %1").arg(inifile1->checkPath()));
	
	
	//--------------------------------------------------------------------------
	// show a QMessage wth the possibility to exit the main programm, when errors occured!
	//--------------------------------------------------------------------------
	connect(interface1, SIGNAL(tooMuchErrors()), this, SLOT(showExitDialog()));
	
	
	//----------------------------------------------------------------------------
	// say a text 
	//----------------------------------------------------------------------------
	/*
	// FixMe: SIOD ERROR: the currently assigned stack limit has been exceded 
	#ifdef _TTY_POSIX_
	connect(this, SIGNAL( speak(QString) ), speakThread, SLOT( speak(QString) ));
		
	if (speakThread->isRunning() == false)
	{
		gui1->appendLog("Starting speak thread...", false);
		speakThread->start();
		gui1->appendLog("Sensor speak started.");
	}
	#endif
	*/
	
	
	//--------------------------------------------------------------------------
	// Check for the programm ini file
	// AND read the settings ! ! !
	//--------------------------------------------------------------------------
	if (inifile1->checkFiles() == false)
	{
		// file not found-Msg
		gui1->appendLog(QString("<b><font color=\"#FF0000\">File '%1' not found!</font></b>").arg(inifile1->getInifileName()));
	}
	else
	{
		// file found-Msg
		gui1->appendLog(QString("Using ini-File \"%1\".").arg(inifile1->getInifileName()));
		
		splash->showMessage(QObject::tr("Reading settings..."), somewhere, splashColor);
		//--------------------
		// read all settings
		//--------------------
		readSettings();
	}
	

	//-------------------------------------------------------
	// Open serial port for microcontroller communication
	//-------------------------------------------------------
	gui1->appendLog("Opening serial port for microcontroller communication...", false);
	
	if (interface1->openComPort(serialPortMicrocontroller) == true)
	{
		gui1->appendLog("Serial port opened.");
	}
	else
	{
		gui1->appendLog(QString("<font color=\"#FF0000\">Error opening serial port '%1'!</font>").arg(serialPortMicrocontroller));
	}
	
	
	//-------------------------------------------------------
	// Basic init for all the bits on the robot circuit
	// AND check, if the robot is "on" (it answers correct)
	//-------------------------------------------------------
	splash->showMessage(QObject::tr("Searching robot..."), somewhere, splashColor);
	
	if (circuit1->initCircuit() == true)
	{
		robotIsOn = true;
		gui1->appendLog("Robot is ON and answers.");
	}
	else
	{
		robotIsOn = false;
		gui1->appendLog("<font color=\"#FF0000\">The robot is OFF! Please turn it ON!</font>");
	}

	
	//-------------------------------------------------------
	// set the read motor speed
	//-------------------------------------------------------
	if (robotIsOn == true)
	{
		// tell the microcontroller the motor speed
		motors->setMotorSpeed(1, mot1Speed);
		// tell the microcontroller the motor speed
		motors->setMotorSpeed(2, mot2Speed);
		gui1->appendLog("Motor speed set in microcontroller");
	}

	
	//-----------------------------------------------------------
	// start the sensor thread ("clock" for reading the sensors)
	//-----------------------------------------------------------
	if (robotIsOn == true)
	{
		if (sensorThread->isRunning() == false)
		{
			gui1->appendLog("Starting sensor thread...", false);
			sensorThread->start();
			gui1->appendLog("Sensor thread started.");
		}
	}
	else
	{
		// show message
		gui1->appendLog("Robot is OFF! Sensor thread NOT started!");
	}


	//-----------------------------------------------------------
	// start the obstacle check thread
	//-----------------------------------------------------------
	if (robotIsOn == true)
	{
		if (obstCheckThread->isRunning() == false)
		{
			gui1->appendLog("Starting obstacle check thread...", false);
			obstCheckThread->start();
			obstCheckThread->setMinObstacleDistance(minObstDist);
			gui1->appendLog("Obstacle check thread started.");
		}
	}
	else
	{
		// show message
		gui1->appendLog("Robot is OFF! Obstacle check thread NOT started!");
	}



	//-----------------------------------------------------------
	// start the plot thread ("clock" for plotting the curves)
	//-----------------------------------------------------------
	if (robotIsOn == true)
	{
		if (plotThread->isRunning() == false)
		{
			gui1->appendLog("Starting plot thread...", false);
			plotThread->start();
			gui1->appendLog("Plot thread started.");
		}
	}
	else
	{
		// show message
		gui1->appendLog("Robot is OFF! Plot thread NOT started!");
	}
	

	// FixMe: check if the camera is on (getStatus)
	//-------------------------------
	// enable the camera controls
	//-------------------------------
	gui1->enableCamControls(true);
	
	
	//----------------------------------------------------------------------------
	// connect sensor signals to "show sensor data"
	// (Whenever the sensor data are completely read, show the result in the GUI)
	//----------------------------------------------------------------------------
	connect(sensorThread, SIGNAL(sensorDataComplete()), this, SLOT(showSensorData()));
	
	//------------------------------------------------------------------
	// connect obstacle check (alarm!) sensor signal to "logical unit"
	//------------------------------------------------------------------
	connect(obstCheckThread, SIGNAL(obstacleDetected(int)), SLOT(logicalUnit(int)));
	
	//----------------------------------------------------------------------------
	// connect remote control button from gui to remote control method here
	// (Whenever the button is pushed, enable the network remote control)
	//----------------------------------------------------------------------------
	connect(gui1, SIGNAL( enableRemoteControlListening(bool) ), this, SLOT( enableRemoteControlListening(bool) ));
	
	//----------------------------------------------------------------------------
	// execute the received remote commands
	//----------------------------------------------------------------------------
	connect(netThread, SIGNAL( dataReceived(QString) ), this, SLOT( executeRemoteCommand(QString) ));
	
	//----------------------------------------------------------------------------
	// connect laserThread signal to "dataReceived"
	// (Whenever data were received, the data are shown in the GUI)
	//----------------------------------------------------------------------------
	connect(laserThread, SIGNAL( laserDataComplete(float *, int *) ), gui1, SLOT( refreshLaserView(float *, int *) ));
	
	//----------------------------------------------------------------------------
	// connect simulation button from gui to activate the simulation mode
	// (sets the mrs an the threads to simulation mode)
	//----------------------------------------------------------------------------
	connect(gui1, SIGNAL( simulate(bool) ), this, SLOT( setSimulationMode(bool) ));
	connect(gui1, SIGNAL( simulate(bool) ), sensorThread, SLOT( setSimulationMode(bool) ));
	connect(gui1, SIGNAL( simulate(bool) ), laserThread, SLOT( setSimulationMode(bool) ));
	connect(gui1, SIGNAL( simulate(bool) ), obstCheckThread, SLOT( setSimulationMode(bool) ));
	

	//---------------------------------------------------------------------
	// check if laser scanner is enabled by ini-file and than start it
	//---------------------------------------------------------------------
	if (noLaserScanner == true)
	{
		// disable the control in the GUI
		gui1->enableLaserScannerControls(false);
		gui1->appendLog("Laser scanner module NOT started (see ini-file)!");
	}
	else
	{
		// TODO: nice exit point and error message
		if (!QGLFormat::hasOpenGL())
		{
			qDebug() << "This system has no OpenGL support" << endl;
			showExitDialog();
		}
		
		
		if (laserThread->isRunning() == false)
		{
			gui1->appendLog("Starting Laser thread...", false);
			laserThread->start();
			gui1->appendLog("Laser thread started.");
		}
		
		//--------------------------------------------
		// start the "ex-CARMEN" laser module ! ! !
		//--------------------------------------------
		splash->showMessage(QObject::tr("Starting laser scanner module..."), somewhere, splashColor);
		startlaser();
	}
	//splash->showMessage(QObject::tr("Ready."), somewhere, splashColor);
}


Mrs::~Mrs()
{
	/*
	if (ClientSocket1->Active == true)
	{
		ClientSocket1->Close();
		ClientSocket1->Active = false;
	}
*/

	//---------------------------------------------------------------
	// save changes to ini-file (if check box is checked!)
	//---------------------------------------------------------------
	if (gui1->getCheckBoxSaveSettings()== Qt::Checked)
	{
		gui1->appendLog("Writing settings...");

		// save gui slider values
		inifile1->writeSetting("Config", "motor1Speed", gui1->getSliderMotorSpeed(1));
		inifile1->writeSetting("Config", "motor2Speed", gui1->getSliderMotorSpeed(2));
		inifile1->writeSetting("Config", "minObstacleDistance", gui1->getSliderObstacleValue());

		// save check box status
		inifile1->writeSetting("Config", "saveOnExit", gui1->getCheckBoxSaveSettings());

		// Later...
		//
		//noHardwareErrorMessages 
		//exitDialog 

		// force writing *immediately*
		inifile1->sync();

		//QMessageBox::information(0, "mrs", "Settings written. :-)", QMessageBox::Ok);
		gui1->appendLog("Settings written.");
	}
	else
	{
		// THIS SETTING HAS TO BE SAVED ALWAYS!
		// ("Save the setting, that no settings shoud be saved")
		//
		// save check box status
		inifile1->writeSetting("Config", "saveOnExit", gui1->getCheckBoxSaveSettings());
	}


	// show dialog if set in ini-file
	if (exitDialog == true)
	{
		// ask user if he really wants to exit.
		if (QMessageBox::question(0,
									"Leaving program...",
									"Are you sure?",
									QMessageBox::Yes | QMessageBox::Default,
									QMessageBox::No | QMessageBox::Escape) == QMessageBox::No)
		{
			//---------
			// if NO
			//---------
			// don't leave! :-)
			return;
		}
	}

	
	//--------------------------------
	// quit the camThread
	//--------------------------------
	if (cam1->isRunning() == true)
	{
		gui1->appendLog("Stopping camera thread...");
		
		// my own stop routine :-)
		cam1->stop();
		
		// slowing thread down
		cam1->setPriority(QThread::IdlePriority);
		cam1->quit();
		
		//-------------------------------------------
		// start measuring time for timeout ckecking
		//-------------------------------------------
		QTime t;
		t.start();
		do
		{
		} while ((cam1->isFinished() == false) && (t.elapsed() <= 2000));

		if (cam1->isFinished() == true)
		{
			gui1->appendLog("Camera thread stopped.");
		}
		else
		{
			gui1->appendLog("Terminating camera thread because it doesn't answer...");
			cam1->terminate();
			cam1->wait(1000);
			gui1->appendLog("Camera thread terminated.");
		}
	}

	
	//--------------------------------
	// quit the laserThread
	//--------------------------------
	if (laserThread->isRunning() == true)
	{
		gui1->appendLog("Stopping Laser thread...");
		
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
			gui1->appendLog("Laser thread stopped.");
		}
		else
		{
			gui1->appendLog("Terminating Laser thread because it doesn't answer...");
			laserThread->terminate();
			laserThread->wait(1000);
			gui1->appendLog("Laser thread terminated.");
		}
	}
	
	
	// shutdown laser module ! ! !
	shutdown_laserview();

	
	#ifdef _TTY_POSIX_
	// FixMe: SIOD ERROR: the currently assigned stack limit has been exceded 
	/*
	//--------------------------------
	// quit the speakThread
	//--------------------------------
	if (speakThread->isRunning() == true)
	{
		gui1->appendLog("Stopping speak thread...");
		
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
			gui1->appendLog("Speak thread stopped.");
		}
		else
		{
			gui1->appendLog("Terminating speak thread because it doesn't answer...");
			speakThread->terminate();
			speakThread->wait(1000);
			gui1->appendLog("Speak thread terminated.");
		}
	}
	*/
	#endif
	
	
	//--------------------------------
	// quit the network thread
	//--------------------------------
	if (netThread->isRunning() == true)
	{
		gui1->appendLog("Stopping network thread...");
		
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
			gui1->appendLog("Network thread stopped.");
		}
		else
		{
			gui1->appendLog("Terminating network thread because it doesn't answer...");
			netThread->terminate();
			netThread->wait(1000);
			gui1->appendLog("Network thread terminated.");
		}
	}

	
	//--------------------------------
	// quit the plotThread
	//--------------------------------
	if (plotThread->isRunning() == true)
	{
		gui1->appendLog("Stopping Plot thread...");
		
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
			gui1->appendLog("Plot thread stopped.");
		}
		else
		{
			gui1->appendLog("Terminating Plot thread because it doesn't answer...");
			plotThread->terminate();
			plotThread->wait(1000);
			gui1->appendLog("Plot thread terminated.");
		}
	}
	
	


	//--------------------------------
	// quit the obstacle check thread
	//--------------------------------
	//qDebug("Starting to stop the obstacle check thread NOW!");
	if (obstCheckThread->isRunning() == true)
	{
		gui1->appendLog("Stopping obstacle check thread...");
		
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
			gui1->appendLog("Obstacle check thread stopped.");
		}
		else
		{
			gui1->appendLog("Terminating obstacle check thread because it doesn't answer...");
			obstCheckThread->terminate();
			obstCheckThread->wait(1000);
			gui1->appendLog("Obstacle check thread terminated.");
		}
	}


	/*
	//-----------------------------
	// quit the supersonic thread
	//-----------------------------
	//qDebug("Starting to stop the supersonic thread NOW!");
	if (supersonThread->isRunning() == true)
	{
		gui1->appendLog("Stopping supersonic thread...");
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
			gui1->appendLog("Supersonic thread stopped.");
		}
		else
		{
			gui1->appendLog("Terminating supersonic thread because it doesn't answer...");
			supersonThread->terminate();
			supersonThread->wait(1000);
			gui1->appendLog("Supersonic thread terminated.");
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
		gui1->appendLog("Stopping sensor thread...");
		
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
			gui1->appendLog("Sensor thread stopped.");
		}
		else
		{
			gui1->appendLog("Terminating sensor thread because it doesn't answer...");
			sensorThread->terminate();
			sensorThread->wait(1000);
			gui1->appendLog("Sensor thread terminated.");
		}
	}


	//-------------------------------------------------------
	// Last init for the robots circuits
	//-------------------------------------------------------
	gui1->appendLog("Last circuit init...");
	if (robotIsOn == true)
	{
		circuit1->initCircuit();
	}

	//-----------------------------
	// close serial port to mc
	//-----------------------------
	gui1->appendLog("Closing serial port to microcontroller...");
	interface1->closeComPort();

	// close the gui
	gui1->close();
	
	//--------------------------------------------------
	// clean up in reverse order (except from the gui)
	//--------------------------------------------------
	#ifdef _TTY_POSIX_
	// FixMe: SIOD ERROR: the currently assigned stack limit has been exceded 
	//delete speakThread;
	#endif
	delete laserThread;
	//delete winLIRC;
	delete netThread;
	delete cam1;
	delete inifile1;
	delete obstCheckThread;
	delete servos;
	delete motors;
	delete sensorThread;
	delete circuit1;
	delete interface1;
	delete gui1;

	//---------------
	// End program
	//---------------
	//exit(0);
}


void Mrs::showExitDialog()
{
		gui1->appendLog("<font color=\"#FF0000\">THERE IS A BIG COMMUNICATION PROBLEM WITH THE SERIAL PORT TO THE ROBOT!</font>");
		
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
	splash->finish(gui1);
}


void Mrs::logicalUnit(int sensorAlarm)
{
	//Todo: Check if there is only one short "alarm signal" this will be ignored!
	
	static int alarmCounter = 0;
	static short int lastSensorValue = -1; // < the initial value has to be different to ALL SENSOR-constants!!


	// first switch all (old) sensor alarms OFF
	gui1->showAlarm(SENSOR1, OFF);
	gui1->showAlarm(SENSOR2, OFF);
	gui1->showAlarm(SENSOR3, OFF);
	gui1->showAlarm(SENSOR4, OFF);
	gui1->showAlarm(SENSOR5, OFF);
	gui1->showAlarm(SENSOR6, OFF);
	gui1->showAlarm(SENSOR7, OFF);
	gui1->showAlarm(SENSOR8, OFF);
	gui1->showAlarm(SENSOR16, OFF);
	
	
	// if all sensor values are the same like the last, do nothing!	
	if (sensorAlarm == lastSensorValue)
	{
		// store this sensor alarm value
		lastSensorValue = sensorAlarm;
		alarmCounter++;
		return;
	}
	

	if (sensorAlarm == NONE)
	{

		if (robotDrives == true)
		{
			gui1->appendLog("No obstacle in front of any sensor. :-)");
			gui1->appendLog("Driving forward...");
			
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

	
	// ++++++++++++++++++++++++++++++++++++
	//
	//  START pasting logic code HERE !!!
	//
	// ++++++++++++++++++++++++++++++++++++
	
	if (sensorAlarm == 1)
	{
		gui1->showAlarm(SENSOR1, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 1 detected!</b>");
			gui1->appendLog("<b>Turning right...</b>");
			
			//----------------
			// right
			//----------------
			drive(RIGHT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 2)
	{
		gui1->showAlarm(SENSOR2, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 2 detected!</b>");
			gui1->appendLog("<b>Turning RIGHT...</b>");
			
			//----------------
			// left
			//----------------
			drive(RIGHT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 3)
	{
		gui1->showAlarm(SENSOR1, ON);
		gui1->showAlarm(SENSOR2, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 1, 2 detected!</b>");
			gui1->appendLog("<b>Turning right...</b>");
			
			//----------------
			// right
			//----------------
			drive(RIGHT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 4)
	{
		gui1->showAlarm(SENSOR3, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 3 detected!</b>");
			gui1->appendLog("<b>Driving forward...</b>");
			
			//----------------
			// forward
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
	
	
	if (sensorAlarm == 5)
	{
		gui1->showAlarm(SENSOR1, ON);
		gui1->showAlarm(SENSOR3, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 1, 3 detected!</b>");
			gui1->appendLog("<b>Turning right...</b>");
			
			//----------------
			// right
			//----------------
			drive(RIGHT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 6)
	{
		gui1->showAlarm(SENSOR2, ON);
		gui1->showAlarm(SENSOR3, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 2, 3 detected!</b>");
			gui1->appendLog("<b>Turning left...</b>");
			
			//----------------
			// left
			//----------------
			drive(LEFT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 7)
	{
		gui1->showAlarm(SENSOR1, ON);
		gui1->showAlarm(SENSOR2, ON);
		gui1->showAlarm(SENSOR3, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 1, 2, 3 detected!</b>");
			gui1->appendLog("<b>Turning left...</b>");
			
			//----------------
			// left
			//----------------
			drive(LEFT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 8)
	{
		gui1->showAlarm(SENSOR4, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 4 detected!</b>");
			gui1->appendLog("<b>Driving forward...</b>");
			
			//----------------
			// forward
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
	
	
	if (sensorAlarm == 9)
	{
		gui1->showAlarm(SENSOR1, ON);
		gui1->showAlarm(SENSOR4, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 1, 4 detected!</b>");
			gui1->appendLog("<b>Turning right...</b>");
			
			//----------------
			// right
			//----------------
			drive(RIGHT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 10)
	{
		gui1->showAlarm(SENSOR2, ON);
		gui1->showAlarm(SENSOR4, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 2, 4 detected!</b>");
			gui1->appendLog("<b>Turning RIGHT...</b>");
			
			//----------------
			// left
			//----------------
			drive(RIGHT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 11)
	{
		gui1->showAlarm(SENSOR1, ON);
		gui1->showAlarm(SENSOR2, ON);
		gui1->showAlarm(SENSOR4, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 1, 2, 4 detected!</b>");
			gui1->appendLog("<b>Turning right...</b>");
			
			//----------------
			// right
			//----------------
			drive(RIGHT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 12)
	{
		gui1->showAlarm(SENSOR3, ON);
		gui1->showAlarm(SENSOR4, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 3, 4 detected!</b>");
			gui1->appendLog("<b>Driving forward...</b>");
			
			//----------------
			// forward
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
	
	
	if (sensorAlarm == 13)
	{
		gui1->showAlarm(SENSOR1, ON);
		gui1->showAlarm(SENSOR3, ON);
		gui1->showAlarm(SENSOR4, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 1, 3, 4 detected!</b>");
			gui1->appendLog("<b>Turning right...</b>");
			
			//----------------
			// right
			//----------------
			drive(RIGHT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 14)
	{
		gui1->showAlarm(SENSOR2, ON);
		gui1->showAlarm(SENSOR3, ON);
		gui1->showAlarm(SENSOR4, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 2, 3, 4 detected!</b>");
			gui1->appendLog("<b>Turning left...</b>");
			
			//----------------
			// left
			//----------------
			drive(LEFT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 15)
	{
		gui1->showAlarm(SENSOR1, ON);
		gui1->showAlarm(SENSOR2, ON);
		gui1->showAlarm(SENSOR3, ON);
		gui1->showAlarm(SENSOR4, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 1, 2, 3, 4 detected!</b>");
			gui1->appendLog("<b>Waiting...</b>");
			
			//----------------
			// wait
			//----------------
			drive(WAIT);
			motors->flashlight(ON);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 16)
	{
		gui1->showAlarm(SENSOR5, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 5 detected!</b>");
			gui1->appendLog("<b>Driving forward...</b>");
			
			//----------------
			// forward
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
	
	
	if (sensorAlarm == 17)
	{
		gui1->showAlarm(SENSOR1, ON);
		gui1->showAlarm(SENSOR5, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 1, 5 detected!</b>");
			gui1->appendLog("<b>Turning right...</b>");
			
			//----------------
			// right
			//----------------
			drive(RIGHT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 18)
	{
		gui1->showAlarm(SENSOR2, ON);
		gui1->showAlarm(SENSOR5, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 2, 5 detected!</b>");
			gui1->appendLog("<b>Turning left...</b>");
			
			//----------------
			// left
			//----------------
			drive(LEFT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 19)
	{
		gui1->showAlarm(SENSOR1, ON);
		gui1->showAlarm(SENSOR2, ON);
		gui1->showAlarm(SENSOR5, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 1, 2, 5 detected!</b>");
			gui1->appendLog("<b>Turning right...</b>");
			
			//----------------
			// right
			//----------------
			drive(RIGHT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 20)
	{
		gui1->showAlarm(SENSOR3, ON);
		gui1->showAlarm(SENSOR5, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 3, 5 detected!</b>");
			gui1->appendLog("<b>Driving forward...</b>");
			
			//----------------
			// forward
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
	
	
	if (sensorAlarm == 21)
	{
		gui1->showAlarm(SENSOR1, ON);
		gui1->showAlarm(SENSOR3, ON);
		gui1->showAlarm(SENSOR5, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 1, 3, 5 detected!</b>");
			gui1->appendLog("<b>Turning right...</b>");
			
			//----------------
			// right
			//----------------
			drive(RIGHT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 22)
	{
		gui1->showAlarm(SENSOR2, ON);
		gui1->showAlarm(SENSOR3, ON);
		gui1->showAlarm(SENSOR5, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 2, 3, 5 detected!</b>");
			gui1->appendLog("<b>Turning left...</b>");
			
			//----------------
			// left
			//----------------
			drive(LEFT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 23)
	{
		gui1->showAlarm(SENSOR1, ON);
		gui1->showAlarm(SENSOR2, ON);
		gui1->showAlarm(SENSOR3, ON);
		gui1->showAlarm(SENSOR5, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 1, 2, 3, 5 detected!</b>");
			gui1->appendLog("<b>Turning right...</b>");
			
			//----------------
			// right
			//----------------
			drive(RIGHT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 24)
	{
		gui1->showAlarm(SENSOR4, ON);
		gui1->showAlarm(SENSOR5, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 4, 5 detected!</b>");
			gui1->appendLog("<b>Driving forward...</b>");
			
			//----------------
			// forward
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
	
	
	if (sensorAlarm == 25)
	{
		gui1->showAlarm(SENSOR1, ON);
		gui1->showAlarm(SENSOR4, ON);
		gui1->showAlarm(SENSOR5, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 1, 4, 5 detected!</b>");
			gui1->appendLog("<b>Turning right...</b>");
			
			//----------------
			// right
			//----------------
			drive(RIGHT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 26)
	{
		gui1->showAlarm(SENSOR2, ON);
		gui1->showAlarm(SENSOR4, ON);
		gui1->showAlarm(SENSOR5, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 2, 4, 5 detected!</b>");
			gui1->appendLog("<b>Turning left...</b>");
			
			//----------------
			// left
			//----------------
			drive(LEFT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 27)
	{
		gui1->showAlarm(SENSOR1, ON);
		gui1->showAlarm(SENSOR2, ON);
		gui1->showAlarm(SENSOR4, ON);
		gui1->showAlarm(SENSOR5, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 1, 2, 4, 5 detected!</b>");
			gui1->appendLog("<b>Turning right...</b>");
			
			//----------------
			// right
			//----------------
			drive(RIGHT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 28)
	{
		gui1->showAlarm(SENSOR3, ON);
		gui1->showAlarm(SENSOR4, ON);
		gui1->showAlarm(SENSOR5, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 3, 4, 5 detected!</b>");
			gui1->appendLog("<b>Driving forward...</b>");
			
			//----------------
			// forward
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
	
	
	if (sensorAlarm == 29)
	{
		gui1->showAlarm(SENSOR1, ON);
		gui1->showAlarm(SENSOR3, ON);
		gui1->showAlarm(SENSOR4, ON);
		gui1->showAlarm(SENSOR5, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 1, 3, 4, 5 detected!</b>");
			gui1->appendLog("<b>Turning right...</b>");
			
			//----------------
			// right
			//----------------
			drive(RIGHT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 30)
	{
		gui1->showAlarm(SENSOR2, ON);
		gui1->showAlarm(SENSOR3, ON);
		gui1->showAlarm(SENSOR4, ON);
		gui1->showAlarm(SENSOR5, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 2, 3, 4, 5 detected!</b>");
			gui1->appendLog("<b>Turning left...</b>");
			
			//----------------
			// left
			//----------------
			drive(LEFT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 31)
	{
		gui1->showAlarm(SENSOR1, ON);
		gui1->showAlarm(SENSOR2, ON);
		gui1->showAlarm(SENSOR3, ON);
		gui1->showAlarm(SENSOR4, ON);
		gui1->showAlarm(SENSOR5, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 1, 2, 3, 4, 5 detected!</b>");
			gui1->appendLog("<b>Turning right...</b>");
			
			//----------------
			// right
			//----------------
			drive(RIGHT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 32)
	{
		gui1->showAlarm(SENSOR6, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 6 detected!</b>");
			gui1->appendLog("<b>Turning right...</b>");
			
			//----------------
			// right
			//----------------
			drive(RIGHT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 33)
	{
		gui1->showAlarm(SENSOR1, ON);
		gui1->showAlarm(SENSOR6, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 1, 6 detected!</b>");
			gui1->appendLog("<b>Turning right...</b>");
			
			//----------------
			// right
			//----------------
			drive(RIGHT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 34)
	{
		gui1->showAlarm(SENSOR2, ON);
		gui1->showAlarm(SENSOR6, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 2, 6 detected!</b>");
			gui1->appendLog("<b>Turning left...</b>");
			
			//----------------
			// left
			//----------------
			drive(LEFT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 35)
	{
		gui1->showAlarm(SENSOR1, ON);
		gui1->showAlarm(SENSOR2, ON);
		gui1->showAlarm(SENSOR6, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 1, 2, 6 detected!</b>");
			gui1->appendLog("<b>Turning right...</b>");
			
			//----------------
			// right
			//----------------
			drive(RIGHT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 36)
	{
		gui1->showAlarm(SENSOR3, ON);
		gui1->showAlarm(SENSOR6, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 3, 6 detected!</b>");
			gui1->appendLog("<b>Turning left...</b>");
			
			//----------------
			// left
			//----------------
			drive(LEFT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 37)
	{
		gui1->showAlarm(SENSOR1, ON);
		gui1->showAlarm(SENSOR3, ON);
		gui1->showAlarm(SENSOR6, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 1, 3, 6 detected!</b>");
			gui1->appendLog("<b>Turning right...</b>");
			
			//----------------
			// right
			//----------------
			drive(RIGHT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 38)
	{
		gui1->showAlarm(SENSOR2, ON);
		gui1->showAlarm(SENSOR3, ON);
		gui1->showAlarm(SENSOR6, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 2, 3, 6 detected!</b>");
			gui1->appendLog("<b>Turning left...</b>");
			
			//----------------
			// left
			//----------------
			drive(LEFT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 39)
	{
		gui1->showAlarm(SENSOR1, ON);
		gui1->showAlarm(SENSOR2, ON);
		gui1->showAlarm(SENSOR3, ON);
		gui1->showAlarm(SENSOR6, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 1, 2, 3, 6 detected!</b>");
			gui1->appendLog("<b>Turning left...</b>");
			
			//----------------
			// left
			//----------------
			drive(LEFT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 40)
	{
		gui1->showAlarm(SENSOR4, ON);
		gui1->showAlarm(SENSOR6, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 4, 6 detected!</b>");
			gui1->appendLog("<b>Turning right...</b>");
			
			//----------------
			// right
			//----------------
			drive(RIGHT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 41)
	{
		gui1->showAlarm(SENSOR1, ON);
		gui1->showAlarm(SENSOR4, ON);
		gui1->showAlarm(SENSOR6, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 1, 4, 6 detected!</b>");
			gui1->appendLog("<b>Turning right...</b>");
			
			//----------------
			// right
			//----------------
			drive(RIGHT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 42)
	{
		gui1->showAlarm(SENSOR2, ON);
		gui1->showAlarm(SENSOR4, ON);
		gui1->showAlarm(SENSOR6, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 2, 4, 6 detected!</b>");
			gui1->appendLog("<b>Turning left...</b>");
			
			//----------------
			// left
			//----------------
			drive(LEFT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 43)
	{
		gui1->showAlarm(SENSOR1, ON);
		gui1->showAlarm(SENSOR2, ON);
		gui1->showAlarm(SENSOR4, ON);
		gui1->showAlarm(SENSOR6, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 1, 2, 4, 6 detected!</b>");
			gui1->appendLog("<b>Turning right...</b>");
			
			//----------------
			// right
			//----------------
			drive(RIGHT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 44)
	{
		gui1->showAlarm(SENSOR3, ON);
		gui1->showAlarm(SENSOR4, ON);
		gui1->showAlarm(SENSOR6, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 3, 4, 6 detected!</b>");
			gui1->appendLog("<b>Turning right...</b>");
			
			//----------------
			// right
			//----------------
			drive(RIGHT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 45)
	{
		gui1->showAlarm(SENSOR1, ON);
		gui1->showAlarm(SENSOR3, ON);
		gui1->showAlarm(SENSOR4, ON);
		gui1->showAlarm(SENSOR6, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 1, 3, 4, 6 detected!</b>");
			gui1->appendLog("<b>Turning right...</b>");
			
			//----------------
			// right
			//----------------
			drive(RIGHT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 46)
	{
		gui1->showAlarm(SENSOR2, ON);
		gui1->showAlarm(SENSOR3, ON);
		gui1->showAlarm(SENSOR4, ON);
		gui1->showAlarm(SENSOR6, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 2, 3, 4, 6 detected!</b>");
			gui1->appendLog("<b>Turning left...</b>");
			
			//----------------
			// left
			//----------------
			drive(LEFT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 47)
	{
		gui1->showAlarm(SENSOR1, ON);
		gui1->showAlarm(SENSOR2, ON);
		gui1->showAlarm(SENSOR3, ON);
		gui1->showAlarm(SENSOR4, ON);
		gui1->showAlarm(SENSOR6, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 1, 2, 3, 4, 6 detected!</b>");
			gui1->appendLog("<b>Turning right...</b>");
			
			//----------------
			// right
			//----------------
			drive(RIGHT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 48)
	{
		gui1->showAlarm(SENSOR5, ON);
		gui1->showAlarm(SENSOR6, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 5, 6 detected!</b>");
			gui1->appendLog("<b>Turning right...</b>");
			
			//----------------
			// right
			//----------------
			drive(RIGHT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 49)
	{
		gui1->showAlarm(SENSOR1, ON);
		gui1->showAlarm(SENSOR5, ON);
		gui1->showAlarm(SENSOR6, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 1, 5, 6 detected!</b>");
			gui1->appendLog("<b>Turning right...</b>");
			
			//----------------
			// right
			//----------------
			drive(RIGHT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 50)
	{
		gui1->showAlarm(SENSOR2, ON);
		gui1->showAlarm(SENSOR5, ON);
		gui1->showAlarm(SENSOR6, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 2, 5, 6 detected!</b>");
			gui1->appendLog("<b>Turning left...</b>");
			
			//----------------
			// left
			//----------------
			drive(LEFT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 51)
	{
		gui1->showAlarm(SENSOR1, ON);
		gui1->showAlarm(SENSOR2, ON);
		gui1->showAlarm(SENSOR5, ON);
		gui1->showAlarm(SENSOR6, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 1, 2, 5, 6 detected!</b>");
			gui1->appendLog("<b>Turning right...</b>");
			
			//----------------
			// right
			//----------------
			drive(RIGHT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 52)
	{
		gui1->showAlarm(SENSOR3, ON);
		gui1->showAlarm(SENSOR5, ON);
		gui1->showAlarm(SENSOR6, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 3, 5, 6 detected!</b>");
			gui1->appendLog("<b>Turning right...</b>");
			
			//----------------
			// right
			//----------------
			drive(RIGHT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 53)
	{
		gui1->showAlarm(SENSOR1, ON);
		gui1->showAlarm(SENSOR3, ON);
		gui1->showAlarm(SENSOR5, ON);
		gui1->showAlarm(SENSOR6, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 1, 3, 5, 6 detected!</b>");
			gui1->appendLog("<b>Turning right...</b>");
			
			//----------------
			// right
			//----------------
			drive(RIGHT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 54)
	{
		gui1->showAlarm(SENSOR2, ON);
		gui1->showAlarm(SENSOR3, ON);
		gui1->showAlarm(SENSOR5, ON);
		gui1->showAlarm(SENSOR6, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 2, 3, 5, 6 detected!</b>");
			gui1->appendLog("<b>Turning right...</b>");
			
			//----------------
			// right
			//----------------
			drive(RIGHT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 55)
	{
		gui1->showAlarm(SENSOR1, ON);
		gui1->showAlarm(SENSOR2, ON);
		gui1->showAlarm(SENSOR3, ON);
		gui1->showAlarm(SENSOR5, ON);
		gui1->showAlarm(SENSOR6, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 1, 2, 3, 5, 6 detected!</b>");
			gui1->appendLog("<b>Turning right...</b>");
			
			//----------------
			// right
			//----------------
			drive(RIGHT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 56)
	{
		gui1->showAlarm(SENSOR4, ON);
		gui1->showAlarm(SENSOR5, ON);
		gui1->showAlarm(SENSOR6, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 4, 5, 6 detected!</b>");
			gui1->appendLog("<b>Turning right...</b>");
			
			//----------------
			// right
			//----------------
			drive(RIGHT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 57)
	{
		gui1->showAlarm(SENSOR1, ON);
		gui1->showAlarm(SENSOR4, ON);
		gui1->showAlarm(SENSOR5, ON);
		gui1->showAlarm(SENSOR6, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 1, 4, 5, 6 detected!</b>");
			gui1->appendLog("<b>Turning right...</b>");
			
			//----------------
			// right
			//----------------
			drive(RIGHT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 58)
	{
		gui1->showAlarm(SENSOR2, ON);
		gui1->showAlarm(SENSOR4, ON);
		gui1->showAlarm(SENSOR5, ON);
		gui1->showAlarm(SENSOR6, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 2, 4, 5, 6 detected!</b>");
			gui1->appendLog("<b>Turning left...</b>");
			
			//----------------
			// left
			//----------------
			drive(LEFT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 59)
	{
		gui1->showAlarm(SENSOR1, ON);
		gui1->showAlarm(SENSOR2, ON);
		gui1->showAlarm(SENSOR4, ON);
		gui1->showAlarm(SENSOR5, ON);
		gui1->showAlarm(SENSOR6, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 1, 2, 4, 5, 6 detected!</b>");
			gui1->appendLog("<b>Turning right...</b>");
			
			//----------------
			// right
			//----------------
			drive(RIGHT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 60)
	{
		gui1->showAlarm(SENSOR3, ON);
		gui1->showAlarm(SENSOR4, ON);
		gui1->showAlarm(SENSOR5, ON);
		gui1->showAlarm(SENSOR6, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 3, 4, 5, 6 detected!</b>");
			gui1->appendLog("<b>Turning right...</b>");
			
			//----------------
			// right
			//----------------
			drive(RIGHT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 61)
	{
		gui1->showAlarm(SENSOR1, ON);
		gui1->showAlarm(SENSOR3, ON);
		gui1->showAlarm(SENSOR4, ON);
		gui1->showAlarm(SENSOR5, ON);
		gui1->showAlarm(SENSOR6, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 1, 3, 4, 5, 6 detected!</b>");
			gui1->appendLog("<b>Turning right...</b>");
			
			//----------------
			// right
			//----------------
			drive(RIGHT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 62)
	{
		gui1->showAlarm(SENSOR2, ON);
		gui1->showAlarm(SENSOR3, ON);
		gui1->showAlarm(SENSOR4, ON);
		gui1->showAlarm(SENSOR5, ON);
		gui1->showAlarm(SENSOR6, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 2, 3, 4, 5, 6 detected!</b>");
			gui1->appendLog("<b>Turning left...</b>");
			
			//----------------
			// left
			//----------------
			drive(LEFT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 63)
	{
		gui1->showAlarm(SENSOR1, ON);
		gui1->showAlarm(SENSOR2, ON);
		gui1->showAlarm(SENSOR3, ON);
		gui1->showAlarm(SENSOR4, ON);
		gui1->showAlarm(SENSOR5, ON);
		gui1->showAlarm(SENSOR6, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 1, 2, 3, 4, 5, 6 detected!</b>");
			gui1->appendLog("<b>Turning right...</b>");
			
			//----------------
			// right
			//----------------
			drive(RIGHT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 64)
	{
		gui1->showAlarm(SENSOR7, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 7 detected!</b>");
			gui1->appendLog("<b>Driving forward...</b>");
			
			//----------------
			// forward
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
	
	
	if (sensorAlarm == 65)
	{
		gui1->showAlarm(SENSOR1, ON);
		gui1->showAlarm(SENSOR7, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 1, 7 detected!</b>");
			gui1->appendLog("<b>Turning right...</b>");
			
			//----------------
			// right
			//----------------
			drive(RIGHT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 66)
	{
		gui1->showAlarm(SENSOR2, ON);
		gui1->showAlarm(SENSOR7, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 2, 7 detected!</b>");
			gui1->appendLog("<b>Turning left...</b>");
			
			//----------------
			// left
			//----------------
			drive(LEFT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 67)
	{
		gui1->showAlarm(SENSOR1, ON);
		gui1->showAlarm(SENSOR2, ON);
		gui1->showAlarm(SENSOR7, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 1, 2, 7 detected!</b>");
			gui1->appendLog("<b>Turning left...</b>");
			
			//----------------
			// left
			//----------------
			drive(LEFT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 68)
	{
		gui1->showAlarm(SENSOR3, ON);
		gui1->showAlarm(SENSOR7, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 3, 7 detected!</b>");
			gui1->appendLog("<b>Driving forward...</b>");
			
			//----------------
			// forward
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
	
	
	if (sensorAlarm == 69)
	{
		gui1->showAlarm(SENSOR1, ON);
		gui1->showAlarm(SENSOR3, ON);
		gui1->showAlarm(SENSOR7, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 1, 3, 7 detected!</b>");
			gui1->appendLog("<b>Turning right...</b>");
			
			//----------------
			// right
			//----------------
			drive(RIGHT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 70)
	{
		gui1->showAlarm(SENSOR2, ON);
		gui1->showAlarm(SENSOR3, ON);
		gui1->showAlarm(SENSOR7, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 2, 3, 7 detected!</b>");
			gui1->appendLog("<b>Turning left...</b>");
			
			//----------------
			// left
			//----------------
			drive(LEFT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 71)
	{
		gui1->showAlarm(SENSOR1, ON);
		gui1->showAlarm(SENSOR2, ON);
		gui1->showAlarm(SENSOR3, ON);
		gui1->showAlarm(SENSOR7, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 1, 2, 3, 7 detected!</b>");
			gui1->appendLog("<b>Turning left...</b>");
			
			//----------------
			// left
			//----------------
			drive(LEFT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 72)
	{
		gui1->showAlarm(SENSOR4, ON);
		gui1->showAlarm(SENSOR7, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 4, 7 detected!</b>");
			gui1->appendLog("<b>Driving forward...</b>");
			
			//----------------
			// forward
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
	
	
	if (sensorAlarm == 73)
	{
		gui1->showAlarm(SENSOR1, ON);
		gui1->showAlarm(SENSOR4, ON);
		gui1->showAlarm(SENSOR7, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 1, 4, 7 detected!</b>");
			gui1->appendLog("<b>Turning right...</b>");
			
			//----------------
			// right
			//----------------
			drive(RIGHT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 74)
	{
		gui1->showAlarm(SENSOR2, ON);
		gui1->showAlarm(SENSOR4, ON);
		gui1->showAlarm(SENSOR7, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 2, 4, 7 detected!</b>");
			gui1->appendLog("<b>Turning left...</b>");
			
			//----------------
			// left
			//----------------
			drive(LEFT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 75)
	{
		gui1->showAlarm(SENSOR1, ON);
		gui1->showAlarm(SENSOR2, ON);
		gui1->showAlarm(SENSOR4, ON);
		gui1->showAlarm(SENSOR7, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 1, 2, 4, 7 detected!</b>");
			gui1->appendLog("<b>Turning left...</b>");
			
			//----------------
			// left
			//----------------
			drive(LEFT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 76)
	{
		gui1->showAlarm(SENSOR3, ON);
		gui1->showAlarm(SENSOR4, ON);
		gui1->showAlarm(SENSOR7, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 3, 4, 7 detected!</b>");
			gui1->appendLog("<b>Driving forward...</b>");
			
			//----------------
			// forward
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
	
	
	if (sensorAlarm == 77)
	{
		gui1->showAlarm(SENSOR1, ON);
		gui1->showAlarm(SENSOR3, ON);
		gui1->showAlarm(SENSOR4, ON);
		gui1->showAlarm(SENSOR7, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 1, 3, 4, 7 detected!</b>");
			gui1->appendLog("<b>Turning right...</b>");
			
			//----------------
			// right
			//----------------
			drive(RIGHT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 78)
	{
		gui1->showAlarm(SENSOR2, ON);
		gui1->showAlarm(SENSOR3, ON);
		gui1->showAlarm(SENSOR4, ON);
		gui1->showAlarm(SENSOR7, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 2, 3, 4, 7 detected!</b>");
			gui1->appendLog("<b>Turning left...</b>");
			
			//----------------
			// left
			//----------------
			drive(LEFT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 79)
	{
		gui1->showAlarm(SENSOR1, ON);
		gui1->showAlarm(SENSOR2, ON);
		gui1->showAlarm(SENSOR3, ON);
		gui1->showAlarm(SENSOR4, ON);
		gui1->showAlarm(SENSOR7, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 1, 2, 3, 4, 7 detected!</b>");
			gui1->appendLog("<b>Turning left...</b>");
			
			//----------------
			// left
			//----------------
			drive(LEFT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 80)
	{
		gui1->showAlarm(SENSOR5, ON);
		gui1->showAlarm(SENSOR7, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 5, 7 detected!</b>");
			gui1->appendLog("<b>Driving forward...</b>");
			
			//----------------
			// forward
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
	
	
	if (sensorAlarm == 81)
	{
		gui1->showAlarm(SENSOR1, ON);
		gui1->showAlarm(SENSOR5, ON);
		gui1->showAlarm(SENSOR7, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 1, 5, 7 detected!</b>");
			gui1->appendLog("<b>Turning right...</b>");
			
			//----------------
			// right
			//----------------
			drive(RIGHT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 82)
	{
		gui1->showAlarm(SENSOR2, ON);
		gui1->showAlarm(SENSOR5, ON);
		gui1->showAlarm(SENSOR7, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 2, 5, 7 detected!</b>");
			gui1->appendLog("<b>Turning left...</b>");
			
			//----------------
			// left
			//----------------
			drive(LEFT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 83)
	{
		gui1->showAlarm(SENSOR1, ON);
		gui1->showAlarm(SENSOR2, ON);
		gui1->showAlarm(SENSOR5, ON);
		gui1->showAlarm(SENSOR7, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 1, 2, 5, 7 detected!</b>");
			gui1->appendLog("<b>Waiting...</b>");
			
			//----------------
			// wait
			//----------------
			drive(WAIT);
			motors->flashlight(ON);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 84)
	{
		gui1->showAlarm(SENSOR3, ON);
		gui1->showAlarm(SENSOR5, ON);
		gui1->showAlarm(SENSOR7, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 3, 5, 7 detected!</b>");
			gui1->appendLog("<b>Driving forward...</b>");
			
			//----------------
			// forward
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
	
	
	if (sensorAlarm == 85)
	{
		gui1->showAlarm(SENSOR1, ON);
		gui1->showAlarm(SENSOR3, ON);
		gui1->showAlarm(SENSOR5, ON);
		gui1->showAlarm(SENSOR7, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 1, 3, 5, 7 detected!</b>");
			gui1->appendLog("<b>Turning right...</b>");
			
			//----------------
			// right
			//----------------
			drive(RIGHT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 86)
	{
		gui1->showAlarm(SENSOR2, ON);
		gui1->showAlarm(SENSOR3, ON);
		gui1->showAlarm(SENSOR5, ON);
		gui1->showAlarm(SENSOR7, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 2, 3, 5, 7 detected!</b>");
			gui1->appendLog("<b>Turning left...</b>");
			
			//----------------
			// left
			//----------------
			drive(LEFT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 87)
	{
		gui1->showAlarm(SENSOR1, ON);
		gui1->showAlarm(SENSOR2, ON);
		gui1->showAlarm(SENSOR3, ON);
		gui1->showAlarm(SENSOR5, ON);
		gui1->showAlarm(SENSOR7, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 1, 2, 3, 5, 7 detected!</b>");
			gui1->appendLog("<b>Waiting...</b>");
			
			//----------------
			// wait
			//----------------
			drive(WAIT);
			motors->flashlight(ON);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 88)
	{
		gui1->showAlarm(SENSOR4, ON);
		gui1->showAlarm(SENSOR5, ON);
		gui1->showAlarm(SENSOR7, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 4, 5, 7 detected!</b>");
			gui1->appendLog("<b>Driving forward...</b>");
			
			//----------------
			// forward
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
	
	
	if (sensorAlarm == 89)
	{
		gui1->showAlarm(SENSOR1, ON);
		gui1->showAlarm(SENSOR4, ON);
		gui1->showAlarm(SENSOR5, ON);
		gui1->showAlarm(SENSOR7, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 1, 4, 5, 7 detected!</b>");
			gui1->appendLog("<b>Turning right...</b>");
			
			//----------------
			// right
			//----------------
			drive(RIGHT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 90)
	{
		gui1->showAlarm(SENSOR2, ON);
		gui1->showAlarm(SENSOR4, ON);
		gui1->showAlarm(SENSOR5, ON);
		gui1->showAlarm(SENSOR7, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 2, 4, 5, 7 detected!</b>");
			gui1->appendLog("<b>Turning left...</b>");
			
			//----------------
			// left
			//----------------
			drive(LEFT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 91)
	{
		gui1->showAlarm(SENSOR1, ON);
		gui1->showAlarm(SENSOR2, ON);
		gui1->showAlarm(SENSOR4, ON);
		gui1->showAlarm(SENSOR5, ON);
		gui1->showAlarm(SENSOR7, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 1, 2, 4, 5, 7 detected!</b>");
			gui1->appendLog("<b>Waiting...</b>");
			
			//----------------
			// wait
			//----------------
			drive(WAIT);
			motors->flashlight(ON);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 92)
	{
		gui1->showAlarm(SENSOR3, ON);
		gui1->showAlarm(SENSOR4, ON);
		gui1->showAlarm(SENSOR5, ON);
		gui1->showAlarm(SENSOR7, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 3, 4, 5, 7 detected!</b>");
			gui1->appendLog("<b>Driving forward...</b>");
			
			//----------------
			// forward
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
	
	
	if (sensorAlarm == 93)
	{
		gui1->showAlarm(SENSOR1, ON);
		gui1->showAlarm(SENSOR3, ON);
		gui1->showAlarm(SENSOR4, ON);
		gui1->showAlarm(SENSOR5, ON);
		gui1->showAlarm(SENSOR7, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 1, 3, 4, 5, 7 detected!</b>");
			gui1->appendLog("<b>Turning right...</b>");
			
			//----------------
			// right
			//----------------
			drive(RIGHT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 94)
	{
		gui1->showAlarm(SENSOR2, ON);
		gui1->showAlarm(SENSOR3, ON);
		gui1->showAlarm(SENSOR4, ON);
		gui1->showAlarm(SENSOR5, ON);
		gui1->showAlarm(SENSOR7, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 2, 3, 4, 5, 7 detected!</b>");
			gui1->appendLog("<b>Turning left...</b>");
			
			//----------------
			// left
			//----------------
			drive(LEFT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 95)
	{
		gui1->showAlarm(SENSOR1, ON);
		gui1->showAlarm(SENSOR2, ON);
		gui1->showAlarm(SENSOR3, ON);
		gui1->showAlarm(SENSOR4, ON);
		gui1->showAlarm(SENSOR5, ON);
		gui1->showAlarm(SENSOR7, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 1, 2, 3, 4, 5, 7 detected!</b>");
			gui1->appendLog("<b>Waiting...</b>");
			
			//----------------
			// wait
			//----------------
			drive(WAIT);
			motors->flashlight(ON);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 96)
	{
		gui1->showAlarm(SENSOR6, ON);
		gui1->showAlarm(SENSOR7, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 6, 7 detected!</b>");
			gui1->appendLog("<b>Turning left...</b>");
			
			//----------------
			// left
			//----------------
			drive(LEFT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 97)
	{
		gui1->showAlarm(SENSOR1, ON);
		gui1->showAlarm(SENSOR6, ON);
		gui1->showAlarm(SENSOR7, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 1, 6, 7 detected!</b>");
			gui1->appendLog("<b>Turning right...</b>");
			
			//----------------
			// right
			//----------------
			drive(RIGHT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 98)
	{
		gui1->showAlarm(SENSOR2, ON);
		gui1->showAlarm(SENSOR6, ON);
		gui1->showAlarm(SENSOR7, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 2, 6, 7 detected!</b>");
			gui1->appendLog("<b>Turning left...</b>");
			
			//----------------
			// left
			//----------------
			drive(LEFT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 99)
	{
		gui1->showAlarm(SENSOR1, ON);
		gui1->showAlarm(SENSOR2, ON);
		gui1->showAlarm(SENSOR6, ON);
		gui1->showAlarm(SENSOR7, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 1, 2, 6, 7 detected!</b>");
			gui1->appendLog("<b>Turning left...</b>");
			
			//----------------
			// left
			//----------------
			drive(LEFT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 100)
	{
		gui1->showAlarm(SENSOR3, ON);
		gui1->showAlarm(SENSOR6, ON);
		gui1->showAlarm(SENSOR7, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 3, 6, 7 detected!</b>");
			gui1->appendLog("<b>Turning left...</b>");
			
			//----------------
			// left
			//----------------
			drive(LEFT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 101)
	{
		gui1->showAlarm(SENSOR1, ON);
		gui1->showAlarm(SENSOR3, ON);
		gui1->showAlarm(SENSOR6, ON);
		gui1->showAlarm(SENSOR7, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 1, 3, 6, 7 detected!</b>");
			gui1->appendLog("<b>Turning right...</b>");
			
			//----------------
			// right
			//----------------
			drive(RIGHT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 102)
	{
		gui1->showAlarm(SENSOR2, ON);
		gui1->showAlarm(SENSOR3, ON);
		gui1->showAlarm(SENSOR6, ON);
		gui1->showAlarm(SENSOR7, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 2, 3, 6, 7 detected!</b>");
			gui1->appendLog("<b>Turning left...</b>");
			
			//----------------
			// left
			//----------------
			drive(LEFT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 103)
	{
		gui1->showAlarm(SENSOR1, ON);
		gui1->showAlarm(SENSOR2, ON);
		gui1->showAlarm(SENSOR3, ON);
		gui1->showAlarm(SENSOR6, ON);
		gui1->showAlarm(SENSOR7, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 1, 2, 3, 6, 7 detected!</b>");
			gui1->appendLog("<b>Turning left...</b>");
			
			//----------------
			// left
			//----------------
			drive(LEFT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 104)
	{
		gui1->showAlarm(SENSOR4, ON);
		gui1->showAlarm(SENSOR6, ON);
		gui1->showAlarm(SENSOR7, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 4, 6, 7 detected!</b>");
			gui1->appendLog("<b>Turning left...</b>");
			
			//----------------
			// left
			//----------------
			drive(LEFT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 105)
	{
		gui1->showAlarm(SENSOR1, ON);
		gui1->showAlarm(SENSOR4, ON);
		gui1->showAlarm(SENSOR6, ON);
		gui1->showAlarm(SENSOR7, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 1, 4, 6, 7 detected!</b>");
			gui1->appendLog("<b>Turning left...</b>");
			
			//----------------
			// left
			//----------------
			drive(LEFT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 106)
	{
		gui1->showAlarm(SENSOR2, ON);
		gui1->showAlarm(SENSOR4, ON);
		gui1->showAlarm(SENSOR6, ON);
		gui1->showAlarm(SENSOR7, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 2, 4, 6, 7 detected!</b>");
			gui1->appendLog("<b>Turning left...</b>");
			
			//----------------
			// left
			//----------------
			drive(LEFT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 107)
	{
		gui1->showAlarm(SENSOR1, ON);
		gui1->showAlarm(SENSOR2, ON);
		gui1->showAlarm(SENSOR4, ON);
		gui1->showAlarm(SENSOR6, ON);
		gui1->showAlarm(SENSOR7, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 1, 2, 4, 6, 7 detected!</b>");
			gui1->appendLog("<b>Turning left...</b>");
			
			//----------------
			// left
			//----------------
			drive(LEFT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 108)
	{
		gui1->showAlarm(SENSOR3, ON);
		gui1->showAlarm(SENSOR4, ON);
		gui1->showAlarm(SENSOR6, ON);
		gui1->showAlarm(SENSOR7, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 3, 4, 6, 7 detected!</b>");
			gui1->appendLog("<b>Turning left...</b>");
			
			//----------------
			// left
			//----------------
			drive(LEFT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 109)
	{
		gui1->showAlarm(SENSOR1, ON);
		gui1->showAlarm(SENSOR3, ON);
		gui1->showAlarm(SENSOR4, ON);
		gui1->showAlarm(SENSOR6, ON);
		gui1->showAlarm(SENSOR7, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 1, 3, 4, 6, 7 detected!</b>");
			gui1->appendLog("<b>Turning left...</b>");
			
			//----------------
			// left
			//----------------
			drive(LEFT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 110)
	{
		gui1->showAlarm(SENSOR2, ON);
		gui1->showAlarm(SENSOR3, ON);
		gui1->showAlarm(SENSOR4, ON);
		gui1->showAlarm(SENSOR6, ON);
		gui1->showAlarm(SENSOR7, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 2, 3, 4, 6, 7 detected!</b>");
			gui1->appendLog("<b>Turning left...</b>");
			
			//----------------
			// left
			//----------------
			drive(LEFT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 111)
	{
		gui1->showAlarm(SENSOR1, ON);
		gui1->showAlarm(SENSOR2, ON);
		gui1->showAlarm(SENSOR3, ON);
		gui1->showAlarm(SENSOR4, ON);
		gui1->showAlarm(SENSOR6, ON);
		gui1->showAlarm(SENSOR7, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 1, 2, 3, 4, 6, 7 detected!</b>");
			gui1->appendLog("<b>Turning left...</b>");
			
			//----------------
			// left
			//----------------
			drive(LEFT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 112)
	{
		gui1->showAlarm(SENSOR5, ON);
		gui1->showAlarm(SENSOR6, ON);
		gui1->showAlarm(SENSOR7, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 5, 6, 7 detected!</b>");
			gui1->appendLog("<b>Turning right...</b>");
			
			//----------------
			// right
			//----------------
			drive(RIGHT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 113)
	{
		gui1->showAlarm(SENSOR1, ON);
		gui1->showAlarm(SENSOR5, ON);
		gui1->showAlarm(SENSOR6, ON);
		gui1->showAlarm(SENSOR7, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 1, 5, 6, 7 detected!</b>");
			gui1->appendLog("<b>Turning right...</b>");
			
			//----------------
			// right
			//----------------
			drive(RIGHT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 114)
	{
		gui1->showAlarm(SENSOR2, ON);
		gui1->showAlarm(SENSOR5, ON);
		gui1->showAlarm(SENSOR6, ON);
		gui1->showAlarm(SENSOR7, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 2, 5, 6, 7 detected!</b>");
			gui1->appendLog("<b>Turning left...</b>");
			
			//----------------
			// left
			//----------------
			drive(LEFT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 115)
	{
		gui1->showAlarm(SENSOR1, ON);
		gui1->showAlarm(SENSOR2, ON);
		gui1->showAlarm(SENSOR5, ON);
		gui1->showAlarm(SENSOR6, ON);
		gui1->showAlarm(SENSOR7, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 1, 2, 5, 6, 7 detected!</b>");
			gui1->appendLog("<b>Waiting...</b>");
			
			//----------------
			// wait
			//----------------
			drive(WAIT);
			motors->flashlight(ON);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 116)
	{
		gui1->showAlarm(SENSOR3, ON);
		gui1->showAlarm(SENSOR5, ON);
		gui1->showAlarm(SENSOR6, ON);
		gui1->showAlarm(SENSOR7, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 3, 5, 6, 7 detected!</b>");
			gui1->appendLog("<b>Turning left...</b>");
			
			//----------------
			// left
			//----------------
			drive(LEFT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 117)
	{
		gui1->showAlarm(SENSOR1, ON);
		gui1->showAlarm(SENSOR3, ON);
		gui1->showAlarm(SENSOR5, ON);
		gui1->showAlarm(SENSOR6, ON);
		gui1->showAlarm(SENSOR7, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 1, 3, 5, 6, 7 detected!</b>");
			gui1->appendLog("<b>Turning right...</b>");
			
			//----------------
			// right
			//----------------
			drive(RIGHT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 118)
	{
		gui1->showAlarm(SENSOR2, ON);
		gui1->showAlarm(SENSOR3, ON);
		gui1->showAlarm(SENSOR5, ON);
		gui1->showAlarm(SENSOR6, ON);
		gui1->showAlarm(SENSOR7, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 2, 3, 5, 6, 7 detected!</b>");
			gui1->appendLog("<b>Turning left...</b>");
			
			//----------------
			// left
			//----------------
			drive(LEFT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 119)
	{
		gui1->showAlarm(SENSOR1, ON);
		gui1->showAlarm(SENSOR2, ON);
		gui1->showAlarm(SENSOR3, ON);
		gui1->showAlarm(SENSOR5, ON);
		gui1->showAlarm(SENSOR6, ON);
		gui1->showAlarm(SENSOR7, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 1, 2, 3, 5, 6, 7 detected!</b>");
			gui1->appendLog("<b>Waiting...</b>");
			
			//----------------
			// wait
			//----------------
			drive(WAIT);
			motors->flashlight(ON);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 120)
	{
		gui1->showAlarm(SENSOR4, ON);
		gui1->showAlarm(SENSOR5, ON);
		gui1->showAlarm(SENSOR6, ON);
		gui1->showAlarm(SENSOR7, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 4, 5, 6, 7 detected!</b>");
			gui1->appendLog("<b>Turning right...</b>");
			
			//----------------
			// right
			//----------------
			drive(RIGHT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 121)
	{
		gui1->showAlarm(SENSOR1, ON);
		gui1->showAlarm(SENSOR4, ON);
		gui1->showAlarm(SENSOR5, ON);
		gui1->showAlarm(SENSOR6, ON);
		gui1->showAlarm(SENSOR7, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 1, 4, 5, 6, 7 detected!</b>");
			gui1->appendLog("<b>Turning right...</b>");
			
			//----------------
			// right
			//----------------
			drive(RIGHT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 122)
	{
		gui1->showAlarm(SENSOR2, ON);
		gui1->showAlarm(SENSOR4, ON);
		gui1->showAlarm(SENSOR5, ON);
		gui1->showAlarm(SENSOR6, ON);
		gui1->showAlarm(SENSOR7, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 2, 4, 5, 6, 7 detected!</b>");
			gui1->appendLog("<b>Turning left...</b>");
			
			//----------------
			// left
			//----------------
			drive(LEFT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 123)
	{
		gui1->showAlarm(SENSOR1, ON);
		gui1->showAlarm(SENSOR2, ON);
		gui1->showAlarm(SENSOR4, ON);
		gui1->showAlarm(SENSOR5, ON);
		gui1->showAlarm(SENSOR6, ON);
		gui1->showAlarm(SENSOR7, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 1, 2, 4, 5, 6, 7 detected!</b>");
			gui1->appendLog("<b>Waiting...</b>");
			
			//----------------
			// wait
			//----------------
			drive(WAIT);
			motors->flashlight(ON);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 124)
	{
		gui1->showAlarm(SENSOR3, ON);
		gui1->showAlarm(SENSOR4, ON);
		gui1->showAlarm(SENSOR5, ON);
		gui1->showAlarm(SENSOR6, ON);
		gui1->showAlarm(SENSOR7, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 3, 4, 5, 6, 7 detected!</b>");
			gui1->appendLog("<b>Turning right...</b>");
			
			//----------------
			// right
			//----------------
			drive(RIGHT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 125)
	{
		gui1->showAlarm(SENSOR1, ON);
		gui1->showAlarm(SENSOR3, ON);
		gui1->showAlarm(SENSOR4, ON);
		gui1->showAlarm(SENSOR5, ON);
		gui1->showAlarm(SENSOR6, ON);
		gui1->showAlarm(SENSOR7, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 1, 3, 4, 5, 6, 7 detected!</b>");
			gui1->appendLog("<b>Turning right...</b>");
			
			//----------------
			// right
			//----------------
			drive(RIGHT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 126)
	{
		gui1->showAlarm(SENSOR2, ON);
		gui1->showAlarm(SENSOR3, ON);
		gui1->showAlarm(SENSOR4, ON);
		gui1->showAlarm(SENSOR5, ON);
		gui1->showAlarm(SENSOR6, ON);
		gui1->showAlarm(SENSOR7, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 2, 3, 4, 5, 6, 7 detected!</b>");
			gui1->appendLog("<b>Turning left...</b>");
			
			//----------------
			// left
			//----------------
			drive(LEFT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 127)
	{
		gui1->showAlarm(SENSOR1, ON);
		gui1->showAlarm(SENSOR2, ON);
		gui1->showAlarm(SENSOR3, ON);
		gui1->showAlarm(SENSOR4, ON);
		gui1->showAlarm(SENSOR5, ON);
		gui1->showAlarm(SENSOR6, ON);
		gui1->showAlarm(SENSOR7, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 1, 2, 3, 4, 5, 6, 7 detected!</b>");
			gui1->appendLog("<b>Waiting...</b>");
			
			//----------------
			// wait
			//----------------
			drive(WAIT);
			motors->flashlight(ON);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 128)
	{
		gui1->showAlarm(SENSOR8, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 8 detected!</b>");
			gui1->appendLog("<b>Driving forward...</b>");
			
			//----------------
			// forward
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
	
	
	if (sensorAlarm == 129)
	{
		gui1->showAlarm(SENSOR1, ON);
		gui1->showAlarm(SENSOR8, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 1, 8 detected!</b>");
			gui1->appendLog("<b>Turning right...</b>");
			
			//----------------
			// right
			//----------------
			drive(RIGHT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 130)
	{
		gui1->showAlarm(SENSOR2, ON);
		gui1->showAlarm(SENSOR8, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 2, 8 detected!</b>");
			gui1->appendLog("<b>Turning left...</b>");
			
			//----------------
			// left
			//----------------
			drive(LEFT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 131)
	{
		gui1->showAlarm(SENSOR1, ON);
		gui1->showAlarm(SENSOR2, ON);
		gui1->showAlarm(SENSOR8, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 1, 2, 8 detected!</b>");
			gui1->appendLog("<b>Turning right...</b>");
			
			//----------------
			// right
			//----------------
			drive(RIGHT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 132)
	{
		gui1->showAlarm(SENSOR3, ON);
		gui1->showAlarm(SENSOR8, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 3, 8 detected!</b>");
			gui1->appendLog("<b>Driving forward...</b>");
			
			//----------------
			// forward
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
	
	
	if (sensorAlarm == 133)
	{
		gui1->showAlarm(SENSOR1, ON);
		gui1->showAlarm(SENSOR3, ON);
		gui1->showAlarm(SENSOR8, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 1, 3, 8 detected!</b>");
			gui1->appendLog("<b>Turning right...</b>");
			
			//----------------
			// right
			//----------------
			drive(RIGHT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 134)
	{
		gui1->showAlarm(SENSOR2, ON);
		gui1->showAlarm(SENSOR3, ON);
		gui1->showAlarm(SENSOR8, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 2, 3, 8 detected!</b>");
			gui1->appendLog("<b>Turning left...</b>");
			
			//----------------
			// left
			//----------------
			drive(LEFT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 135)
	{
		gui1->showAlarm(SENSOR1, ON);
		gui1->showAlarm(SENSOR2, ON);
		gui1->showAlarm(SENSOR3, ON);
		gui1->showAlarm(SENSOR8, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 1, 2, 3, 8 detected!</b>");
			gui1->appendLog("<b>Turning left...</b>");
			
			//----------------
			// left
			//----------------
			drive(LEFT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 136)
	{
		gui1->showAlarm(SENSOR4, ON);
		gui1->showAlarm(SENSOR8, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 4, 8 detected!</b>");
			gui1->appendLog("<b>Driving forward...</b>");
			
			//----------------
			// forward
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
	
	
	if (sensorAlarm == 137)
	{
		gui1->showAlarm(SENSOR1, ON);
		gui1->showAlarm(SENSOR4, ON);
		gui1->showAlarm(SENSOR8, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 1, 4, 8 detected!</b>");
			gui1->appendLog("<b>Turning right...</b>");
			
			//----------------
			// right
			//----------------
			drive(RIGHT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 138)
	{
		gui1->showAlarm(SENSOR2, ON);
		gui1->showAlarm(SENSOR4, ON);
		gui1->showAlarm(SENSOR8, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 2, 4, 8 detected!</b>");
			gui1->appendLog("<b>Turning left...</b>");
			
			//----------------
			// left
			//----------------
			drive(LEFT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 139)
	{
		gui1->showAlarm(SENSOR1, ON);
		gui1->showAlarm(SENSOR2, ON);
		gui1->showAlarm(SENSOR4, ON);
		gui1->showAlarm(SENSOR8, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 1, 2, 4, 8 detected!</b>");
			gui1->appendLog("<b>Turning right...</b>");
			
			//----------------
			// right
			//----------------
			drive(RIGHT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 140)
	{
		gui1->showAlarm(SENSOR3, ON);
		gui1->showAlarm(SENSOR4, ON);
		gui1->showAlarm(SENSOR8, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 3, 4, 8 detected!</b>");
			gui1->appendLog("<b>Driving forward...</b>");
			
			//----------------
			// forward
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
	
	
	if (sensorAlarm == 141)
	{
		gui1->showAlarm(SENSOR1, ON);
		gui1->showAlarm(SENSOR3, ON);
		gui1->showAlarm(SENSOR4, ON);
		gui1->showAlarm(SENSOR8, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 1, 3, 4, 8 detected!</b>");
			gui1->appendLog("<b>Turning right...</b>");
			
			//----------------
			// right
			//----------------
			drive(RIGHT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 142)
	{
		gui1->showAlarm(SENSOR2, ON);
		gui1->showAlarm(SENSOR3, ON);
		gui1->showAlarm(SENSOR4, ON);
		gui1->showAlarm(SENSOR8, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 2, 3, 4, 8 detected!</b>");
			gui1->appendLog("<b>Turning left...</b>");
			
			//----------------
			// left
			//----------------
			drive(LEFT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 143)
	{
		gui1->showAlarm(SENSOR1, ON);
		gui1->showAlarm(SENSOR2, ON);
		gui1->showAlarm(SENSOR3, ON);
		gui1->showAlarm(SENSOR4, ON);
		gui1->showAlarm(SENSOR8, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 1, 2, 3, 4, 8 detected!</b>");
			gui1->appendLog("<b>Waiting...</b>");
			
			//----------------
			// wait
			//----------------
			drive(WAIT);
			motors->flashlight(ON);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 144)
	{
		gui1->showAlarm(SENSOR5, ON);
		gui1->showAlarm(SENSOR8, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 5, 8 detected!</b>");
			gui1->appendLog("<b>Driving forward...</b>");
			
			//----------------
			// forward
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
	
	
	if (sensorAlarm == 145)
	{
		gui1->showAlarm(SENSOR1, ON);
		gui1->showAlarm(SENSOR5, ON);
		gui1->showAlarm(SENSOR8, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 1, 5, 8 detected!</b>");
			gui1->appendLog("<b>Turning right...</b>");
			
			//----------------
			// right
			//----------------
			drive(RIGHT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 146)
	{
		gui1->showAlarm(SENSOR2, ON);
		gui1->showAlarm(SENSOR5, ON);
		gui1->showAlarm(SENSOR8, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 2, 5, 8 detected!</b>");
			gui1->appendLog("<b>Turning left...</b>");
			
			//----------------
			// left
			//----------------
			drive(LEFT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 147)
	{
		gui1->showAlarm(SENSOR1, ON);
		gui1->showAlarm(SENSOR2, ON);
		gui1->showAlarm(SENSOR5, ON);
		gui1->showAlarm(SENSOR8, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 1, 2, 5, 8 detected!</b>");
			gui1->appendLog("<b>Turning right...</b>");
			
			//----------------
			// right
			//----------------
			drive(RIGHT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 148)
	{
		gui1->showAlarm(SENSOR3, ON);
		gui1->showAlarm(SENSOR5, ON);
		gui1->showAlarm(SENSOR8, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 3, 5, 8 detected!</b>");
			gui1->appendLog("<b>Driving forward...</b>");
			
			//----------------
			// forward
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
	
	
	if (sensorAlarm == 149)
	{
		gui1->showAlarm(SENSOR1, ON);
		gui1->showAlarm(SENSOR3, ON);
		gui1->showAlarm(SENSOR5, ON);
		gui1->showAlarm(SENSOR8, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 1, 3, 5, 8 detected!</b>");
			gui1->appendLog("<b>Turning right...</b>");
			
			//----------------
			// right
			//----------------
			drive(RIGHT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 150)
	{
		gui1->showAlarm(SENSOR2, ON);
		gui1->showAlarm(SENSOR3, ON);
		gui1->showAlarm(SENSOR5, ON);
		gui1->showAlarm(SENSOR8, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 2, 3, 5, 8 detected!</b>");
			gui1->appendLog("<b>Turning left...</b>");
			
			//----------------
			// left
			//----------------
			drive(LEFT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 151)
	{
		gui1->showAlarm(SENSOR1, ON);
		gui1->showAlarm(SENSOR2, ON);
		gui1->showAlarm(SENSOR3, ON);
		gui1->showAlarm(SENSOR5, ON);
		gui1->showAlarm(SENSOR8, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 1, 2, 3, 5, 8 detected!</b>");
			gui1->appendLog("<b>Turning right...</b>");
			
			//----------------
			// right
			//----------------
			drive(RIGHT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 152)
	{
		gui1->showAlarm(SENSOR4, ON);
		gui1->showAlarm(SENSOR5, ON);
		gui1->showAlarm(SENSOR8, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 4, 5, 8 detected!</b>");
			gui1->appendLog("<b>Driving forward...</b>");
			
			//----------------
			// forward
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
	
	
	if (sensorAlarm == 153)
	{
		gui1->showAlarm(SENSOR1, ON);
		gui1->showAlarm(SENSOR4, ON);
		gui1->showAlarm(SENSOR5, ON);
		gui1->showAlarm(SENSOR8, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 1, 4, 5, 8 detected!</b>");
			gui1->appendLog("<b>Turning right...</b>");
			
			//----------------
			// right
			//----------------
			drive(RIGHT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 154)
	{
		gui1->showAlarm(SENSOR2, ON);
		gui1->showAlarm(SENSOR4, ON);
		gui1->showAlarm(SENSOR5, ON);
		gui1->showAlarm(SENSOR8, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 2, 4, 5, 8 detected!</b>");
			gui1->appendLog("<b>Turning right...</b>");
			
			//----------------
			// right
			//----------------
			drive(RIGHT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 155)
	{
		gui1->showAlarm(SENSOR1, ON);
		gui1->showAlarm(SENSOR2, ON);
		gui1->showAlarm(SENSOR4, ON);
		gui1->showAlarm(SENSOR5, ON);
		gui1->showAlarm(SENSOR8, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 1, 2, 4, 5, 8 detected!</b>");
			gui1->appendLog("<b>Turning right...</b>");
			
			//----------------
			// right
			//----------------
			drive(RIGHT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 156)
	{
		gui1->showAlarm(SENSOR3, ON);
		gui1->showAlarm(SENSOR4, ON);
		gui1->showAlarm(SENSOR5, ON);
		gui1->showAlarm(SENSOR8, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 3, 4, 5, 8 detected!</b>");
			gui1->appendLog("<b>Driving forward...</b>");
			
			//----------------
			// forward
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
	
	
	if (sensorAlarm == 157)
	{
		gui1->showAlarm(SENSOR1, ON);
		gui1->showAlarm(SENSOR3, ON);
		gui1->showAlarm(SENSOR4, ON);
		gui1->showAlarm(SENSOR5, ON);
		gui1->showAlarm(SENSOR8, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 1, 3, 4, 5, 8 detected!</b>");
			gui1->appendLog("<b>Turning right...</b>");
			
			//----------------
			// right
			//----------------
			drive(RIGHT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 158)
	{
		gui1->showAlarm(SENSOR2, ON);
		gui1->showAlarm(SENSOR3, ON);
		gui1->showAlarm(SENSOR4, ON);
		gui1->showAlarm(SENSOR5, ON);
		gui1->showAlarm(SENSOR8, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 2, 3, 4, 5, 8 detected!</b>");
			gui1->appendLog("<b>Turning left...</b>");
			
			//----------------
			// left
			//----------------
			drive(LEFT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 159)
	{
		gui1->showAlarm(SENSOR1, ON);
		gui1->showAlarm(SENSOR2, ON);
		gui1->showAlarm(SENSOR3, ON);
		gui1->showAlarm(SENSOR4, ON);
		gui1->showAlarm(SENSOR5, ON);
		gui1->showAlarm(SENSOR8, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 1, 2, 3, 4, 5, 8 detected!</b>");
			gui1->appendLog("<b>Turning right...</b>");
			
			//----------------
			// right
			//----------------
			drive(RIGHT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 160)
	{
		gui1->showAlarm(SENSOR6, ON);
		gui1->showAlarm(SENSOR8, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 6, 8 detected!</b>");
			gui1->appendLog("<b>Turning right...</b>");
			
			//----------------
			// right
			//----------------
			drive(RIGHT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 161)
	{
		gui1->showAlarm(SENSOR1, ON);
		gui1->showAlarm(SENSOR6, ON);
		gui1->showAlarm(SENSOR8, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 1, 6, 8 detected!</b>");
			gui1->appendLog("<b>Turning right...</b>");
			
			//----------------
			// right
			//----------------
			drive(RIGHT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 162)
	{
		gui1->showAlarm(SENSOR2, ON);
		gui1->showAlarm(SENSOR6, ON);
		gui1->showAlarm(SENSOR8, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 2, 6, 8 detected!</b>");
			gui1->appendLog("<b>Turning left...</b>");
			
			//----------------
			// left
			//----------------
			drive(LEFT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 163)
	{
		gui1->showAlarm(SENSOR1, ON);
		gui1->showAlarm(SENSOR2, ON);
		gui1->showAlarm(SENSOR6, ON);
		gui1->showAlarm(SENSOR8, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 1, 2, 6, 8 detected!</b>");
			gui1->appendLog("<b>Turning right...</b>");
			
			//----------------
			// right
			//----------------
			drive(RIGHT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 164)
	{
		gui1->showAlarm(SENSOR3, ON);
		gui1->showAlarm(SENSOR6, ON);
		gui1->showAlarm(SENSOR8, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 3, 6, 8 detected!</b>");
			gui1->appendLog("<b>Turning left...</b>");
			
			//----------------
			// left
			//----------------
			drive(LEFT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 165)
	{
		gui1->showAlarm(SENSOR1, ON);
		gui1->showAlarm(SENSOR3, ON);
		gui1->showAlarm(SENSOR6, ON);
		gui1->showAlarm(SENSOR8, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 1, 3, 6, 8 detected!</b>");
			gui1->appendLog("<b>Turning right...</b>");
			
			//----------------
			// right
			//----------------
			drive(RIGHT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 166)
	{
		gui1->showAlarm(SENSOR2, ON);
		gui1->showAlarm(SENSOR3, ON);
		gui1->showAlarm(SENSOR6, ON);
		gui1->showAlarm(SENSOR8, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 2, 3, 6, 8 detected!</b>");
			gui1->appendLog("<b>Turning left...</b>");
			
			//----------------
			// left
			//----------------
			drive(LEFT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 167)
	{
		gui1->showAlarm(SENSOR1, ON);
		gui1->showAlarm(SENSOR2, ON);
		gui1->showAlarm(SENSOR3, ON);
		gui1->showAlarm(SENSOR6, ON);
		gui1->showAlarm(SENSOR8, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 1, 2, 3, 6, 8 detected!</b>");
			gui1->appendLog("<b>Turning left...</b>");
			
			//----------------
			// left
			//----------------
			drive(LEFT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 168)
	{
		gui1->showAlarm(SENSOR4, ON);
		gui1->showAlarm(SENSOR6, ON);
		gui1->showAlarm(SENSOR8, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 4, 6, 8 detected!</b>");
			gui1->appendLog("<b>Turning right...</b>");
			
			//----------------
			// right
			//----------------
			drive(RIGHT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 169)
	{
		gui1->showAlarm(SENSOR1, ON);
		gui1->showAlarm(SENSOR4, ON);
		gui1->showAlarm(SENSOR6, ON);
		gui1->showAlarm(SENSOR8, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 1, 4, 6, 8 detected!</b>");
			gui1->appendLog("<b>Turning right...</b>");
			
			//----------------
			// right
			//----------------
			drive(RIGHT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 170)
	{
		gui1->showAlarm(SENSOR2, ON);
		gui1->showAlarm(SENSOR4, ON);
		gui1->showAlarm(SENSOR6, ON);
		gui1->showAlarm(SENSOR8, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 2, 4, 6, 8 detected!</b>");
			gui1->appendLog("<b>Turning left...</b>");
			
			//----------------
			// left
			//----------------
			drive(LEFT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 171)
	{
		gui1->showAlarm(SENSOR1, ON);
		gui1->showAlarm(SENSOR2, ON);
		gui1->showAlarm(SENSOR4, ON);
		gui1->showAlarm(SENSOR6, ON);
		gui1->showAlarm(SENSOR8, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 1, 2, 4, 6, 8 detected!</b>");
			gui1->appendLog("<b>Turning right...</b>");
			
			//----------------
			// right
			//----------------
			drive(RIGHT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 172)
	{
		gui1->showAlarm(SENSOR3, ON);
		gui1->showAlarm(SENSOR4, ON);
		gui1->showAlarm(SENSOR6, ON);
		gui1->showAlarm(SENSOR8, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 3, 4, 6, 8 detected!</b>");
			gui1->appendLog("<b>Turning right...</b>");
			
			//----------------
			// right
			//----------------
			drive(RIGHT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 173)
	{
		gui1->showAlarm(SENSOR1, ON);
		gui1->showAlarm(SENSOR3, ON);
		gui1->showAlarm(SENSOR4, ON);
		gui1->showAlarm(SENSOR6, ON);
		gui1->showAlarm(SENSOR8, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 1, 3, 4, 6, 8 detected!</b>");
			gui1->appendLog("<b>Turning right...</b>");
			
			//----------------
			// right
			//----------------
			drive(RIGHT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 174)
	{
		gui1->showAlarm(SENSOR2, ON);
		gui1->showAlarm(SENSOR3, ON);
		gui1->showAlarm(SENSOR4, ON);
		gui1->showAlarm(SENSOR6, ON);
		gui1->showAlarm(SENSOR8, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 2, 3, 4, 6, 8 detected!</b>");
			gui1->appendLog("<b>Turning left...</b>");
			
			//----------------
			// left
			//----------------
			drive(LEFT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 175)
	{
		gui1->showAlarm(SENSOR1, ON);
		gui1->showAlarm(SENSOR2, ON);
		gui1->showAlarm(SENSOR3, ON);
		gui1->showAlarm(SENSOR4, ON);
		gui1->showAlarm(SENSOR6, ON);
		gui1->showAlarm(SENSOR8, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 1, 2, 3, 4, 6, 8 detected!</b>");
			gui1->appendLog("<b>Turning right...</b>");
			
			//----------------
			// right
			//----------------
			drive(RIGHT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 176)
	{
		gui1->showAlarm(SENSOR5, ON);
		gui1->showAlarm(SENSOR6, ON);
		gui1->showAlarm(SENSOR8, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 5, 6, 8 detected!</b>");
			gui1->appendLog("<b>Turning right...</b>");
			
			//----------------
			// right
			//----------------
			drive(RIGHT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 177)
	{
		gui1->showAlarm(SENSOR1, ON);
		gui1->showAlarm(SENSOR5, ON);
		gui1->showAlarm(SENSOR6, ON);
		gui1->showAlarm(SENSOR8, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 1, 5, 6, 8 detected!</b>");
			gui1->appendLog("<b>Turning right...</b>");
			
			//----------------
			// right
			//----------------
			drive(RIGHT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 178)
	{
		gui1->showAlarm(SENSOR2, ON);
		gui1->showAlarm(SENSOR5, ON);
		gui1->showAlarm(SENSOR6, ON);
		gui1->showAlarm(SENSOR8, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 2, 5, 6, 8 detected!</b>");
			gui1->appendLog("<b>Turning right...</b>");
			
			//----------------
			// right
			//----------------
			drive(RIGHT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 179)
	{
		gui1->showAlarm(SENSOR1, ON);
		gui1->showAlarm(SENSOR2, ON);
		gui1->showAlarm(SENSOR5, ON);
		gui1->showAlarm(SENSOR6, ON);
		gui1->showAlarm(SENSOR8, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 1, 2, 5, 6, 8 detected!</b>");
			gui1->appendLog("<b>Turning right...</b>");
			
			//----------------
			// right
			//----------------
			drive(RIGHT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 180)
	{
		gui1->showAlarm(SENSOR3, ON);
		gui1->showAlarm(SENSOR5, ON);
		gui1->showAlarm(SENSOR6, ON);
		gui1->showAlarm(SENSOR8, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 3, 5, 6, 8 detected!</b>");
			gui1->appendLog("<b>Turning right...</b>");
			
			//----------------
			// right
			//----------------
			drive(RIGHT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 181)
	{
		gui1->showAlarm(SENSOR1, ON);
		gui1->showAlarm(SENSOR3, ON);
		gui1->showAlarm(SENSOR5, ON);
		gui1->showAlarm(SENSOR6, ON);
		gui1->showAlarm(SENSOR8, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 1, 3, 5, 6, 8 detected!</b>");
			gui1->appendLog("<b>Turning right...</b>");
			
			//----------------
			// right
			//----------------
			drive(RIGHT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 182)
	{
		gui1->showAlarm(SENSOR2, ON);
		gui1->showAlarm(SENSOR3, ON);
		gui1->showAlarm(SENSOR5, ON);
		gui1->showAlarm(SENSOR6, ON);
		gui1->showAlarm(SENSOR8, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 2, 3, 5, 6, 8 detected!</b>");
			gui1->appendLog("<b>Turning right...</b>");
			
			//----------------
			// right
			//----------------
			drive(RIGHT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 183)
	{
		gui1->showAlarm(SENSOR1, ON);
		gui1->showAlarm(SENSOR2, ON);
		gui1->showAlarm(SENSOR3, ON);
		gui1->showAlarm(SENSOR5, ON);
		gui1->showAlarm(SENSOR6, ON);
		gui1->showAlarm(SENSOR8, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 1, 2, 3, 5, 6, 8 detected!</b>");
			gui1->appendLog("<b>Turning right...</b>");
			
			//----------------
			// right
			//----------------
			drive(RIGHT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 184)
	{
		gui1->showAlarm(SENSOR4, ON);
		gui1->showAlarm(SENSOR5, ON);
		gui1->showAlarm(SENSOR6, ON);
		gui1->showAlarm(SENSOR8, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 4, 5, 6, 8 detected!</b>");
			gui1->appendLog("<b>Turning right...</b>");
			
			//----------------
			// right
			//----------------
			drive(RIGHT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 185)
	{
		gui1->showAlarm(SENSOR1, ON);
		gui1->showAlarm(SENSOR4, ON);
		gui1->showAlarm(SENSOR5, ON);
		gui1->showAlarm(SENSOR6, ON);
		gui1->showAlarm(SENSOR8, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 1, 4, 5, 6, 8 detected!</b>");
			gui1->appendLog("<b>Turning right...</b>");
			
			//----------------
			// right
			//----------------
			drive(RIGHT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 186)
	{
		gui1->showAlarm(SENSOR2, ON);
		gui1->showAlarm(SENSOR4, ON);
		gui1->showAlarm(SENSOR5, ON);
		gui1->showAlarm(SENSOR6, ON);
		gui1->showAlarm(SENSOR8, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 2, 4, 5, 6, 8 detected!</b>");
			gui1->appendLog("<b>Turning right...</b>");
			
			//----------------
			// right
			//----------------
			drive(RIGHT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 187)
	{
		gui1->showAlarm(SENSOR1, ON);
		gui1->showAlarm(SENSOR2, ON);
		gui1->showAlarm(SENSOR4, ON);
		gui1->showAlarm(SENSOR5, ON);
		gui1->showAlarm(SENSOR6, ON);
		gui1->showAlarm(SENSOR8, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 1, 2, 4, 5, 6, 8 detected!</b>");
			gui1->appendLog("<b>Turning right...</b>");
			
			//----------------
			// right
			//----------------
			drive(RIGHT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 188)
	{
		gui1->showAlarm(SENSOR3, ON);
		gui1->showAlarm(SENSOR4, ON);
		gui1->showAlarm(SENSOR5, ON);
		gui1->showAlarm(SENSOR6, ON);
		gui1->showAlarm(SENSOR8, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 3, 4, 5, 6, 8 detected!</b>");
			gui1->appendLog("<b>Turning right...</b>");
			
			//----------------
			// right
			//----------------
			drive(RIGHT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 189)
	{
		gui1->showAlarm(SENSOR1, ON);
		gui1->showAlarm(SENSOR3, ON);
		gui1->showAlarm(SENSOR4, ON);
		gui1->showAlarm(SENSOR5, ON);
		gui1->showAlarm(SENSOR6, ON);
		gui1->showAlarm(SENSOR8, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 1, 3, 4, 5, 6, 8 detected!</b>");
			gui1->appendLog("<b>Turning right...</b>");
			
			//----------------
			// right
			//----------------
			drive(RIGHT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 190)
	{
		gui1->showAlarm(SENSOR2, ON);
		gui1->showAlarm(SENSOR3, ON);
		gui1->showAlarm(SENSOR4, ON);
		gui1->showAlarm(SENSOR5, ON);
		gui1->showAlarm(SENSOR6, ON);
		gui1->showAlarm(SENSOR8, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 2, 3, 4, 5, 6, 8 detected!</b>");
			gui1->appendLog("<b>Turning right...</b>");
			
			//----------------
			// right
			//----------------
			drive(RIGHT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 191)
	{
		gui1->showAlarm(SENSOR1, ON);
		gui1->showAlarm(SENSOR2, ON);
		gui1->showAlarm(SENSOR3, ON);
		gui1->showAlarm(SENSOR4, ON);
		gui1->showAlarm(SENSOR5, ON);
		gui1->showAlarm(SENSOR6, ON);
		gui1->showAlarm(SENSOR8, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 1, 2, 3, 4, 5, 6, 8 detected!</b>");
			gui1->appendLog("<b>Turning right...</b>");
			
			//----------------
			// right
			//----------------
			drive(RIGHT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 192)
	{
		gui1->showAlarm(SENSOR7, ON);
		gui1->showAlarm(SENSOR8, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 7, 8 detected!</b>");
			gui1->appendLog("<b>Driving forward...</b>");
			
			//----------------
			// forward
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
	
	
	if (sensorAlarm == 193)
	{
		gui1->showAlarm(SENSOR1, ON);
		gui1->showAlarm(SENSOR7, ON);
		gui1->showAlarm(SENSOR8, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 1, 7, 8 detected!</b>");
			gui1->appendLog("<b>Turning right...</b>");
			
			//----------------
			// right
			//----------------
			drive(RIGHT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 194)
	{
		gui1->showAlarm(SENSOR2, ON);
		gui1->showAlarm(SENSOR7, ON);
		gui1->showAlarm(SENSOR8, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 2, 7, 8 detected!</b>");
			gui1->appendLog("<b>Turning left...</b>");
			
			//----------------
			// left
			//----------------
			drive(LEFT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 195)
	{
		gui1->showAlarm(SENSOR1, ON);
		gui1->showAlarm(SENSOR2, ON);
		gui1->showAlarm(SENSOR7, ON);
		gui1->showAlarm(SENSOR8, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 1, 2, 7, 8 detected!</b>");
			gui1->appendLog("<b>Turning left...</b>");
			
			//----------------
			// left
			//----------------
			drive(LEFT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 196)
	{
		gui1->showAlarm(SENSOR3, ON);
		gui1->showAlarm(SENSOR7, ON);
		gui1->showAlarm(SENSOR8, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 3, 7, 8 detected!</b>");
			gui1->appendLog("<b>Driving forward...</b>");
			
			//----------------
			// forward
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
	
	
	if (sensorAlarm == 197)
	{
		gui1->showAlarm(SENSOR1, ON);
		gui1->showAlarm(SENSOR3, ON);
		gui1->showAlarm(SENSOR7, ON);
		gui1->showAlarm(SENSOR8, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 1, 3, 7, 8 detected!</b>");
			gui1->appendLog("<b>Turning right...</b>");
			
			//----------------
			// right
			//----------------
			drive(RIGHT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 198)
	{
		gui1->showAlarm(SENSOR2, ON);
		gui1->showAlarm(SENSOR3, ON);
		gui1->showAlarm(SENSOR7, ON);
		gui1->showAlarm(SENSOR8, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 2, 3, 7, 8 detected!</b>");
			gui1->appendLog("<b>Turning left...</b>");
			
			//----------------
			// left
			//----------------
			drive(LEFT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 199)
	{
		gui1->showAlarm(SENSOR1, ON);
		gui1->showAlarm(SENSOR2, ON);
		gui1->showAlarm(SENSOR3, ON);
		gui1->showAlarm(SENSOR7, ON);
		gui1->showAlarm(SENSOR8, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 1, 2, 3, 7, 8 detected!</b>");
			gui1->appendLog("<b>Turning left...</b>");
			
			//----------------
			// left
			//----------------
			drive(LEFT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 200)
	{
		gui1->showAlarm(SENSOR4, ON);
		gui1->showAlarm(SENSOR7, ON);
		gui1->showAlarm(SENSOR8, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 4, 7, 8 detected!</b>");
			gui1->appendLog("<b>Driving forward...</b>");
			
			//----------------
			// forward
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
	
	
	if (sensorAlarm == 201)
	{
		gui1->showAlarm(SENSOR1, ON);
		gui1->showAlarm(SENSOR4, ON);
		gui1->showAlarm(SENSOR7, ON);
		gui1->showAlarm(SENSOR8, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 1, 4, 7, 8 detected!</b>");
			gui1->appendLog("<b>Turning right...</b>");
			
			//----------------
			// right
			//----------------
			drive(RIGHT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 202)
	{
		gui1->showAlarm(SENSOR2, ON);
		gui1->showAlarm(SENSOR4, ON);
		gui1->showAlarm(SENSOR7, ON);
		gui1->showAlarm(SENSOR8, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 2, 4, 7, 8 detected!</b>");
			gui1->appendLog("<b>Turning left...</b>");
			
			//----------------
			// left
			//----------------
			drive(LEFT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 203)
	{
		gui1->showAlarm(SENSOR1, ON);
		gui1->showAlarm(SENSOR2, ON);
		gui1->showAlarm(SENSOR4, ON);
		gui1->showAlarm(SENSOR7, ON);
		gui1->showAlarm(SENSOR8, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 1, 2, 4, 7, 8 detected!</b>");
			gui1->appendLog("<b>Turning left...</b>");
			
			//----------------
			// left
			//----------------
			drive(LEFT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 204)
	{
		gui1->showAlarm(SENSOR3, ON);
		gui1->showAlarm(SENSOR4, ON);
		gui1->showAlarm(SENSOR7, ON);
		gui1->showAlarm(SENSOR8, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 3, 4, 7, 8 detected!</b>");
			gui1->appendLog("<b>Driving forward...</b>");
			
			//----------------
			// forward
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
	
	
	if (sensorAlarm == 205)
	{
		gui1->showAlarm(SENSOR1, ON);
		gui1->showAlarm(SENSOR3, ON);
		gui1->showAlarm(SENSOR4, ON);
		gui1->showAlarm(SENSOR7, ON);
		gui1->showAlarm(SENSOR8, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 1, 3, 4, 7, 8 detected!</b>");
			gui1->appendLog("<b>Turning right...</b>");
			
			//----------------
			// right
			//----------------
			drive(RIGHT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 206)
	{
		gui1->showAlarm(SENSOR2, ON);
		gui1->showAlarm(SENSOR3, ON);
		gui1->showAlarm(SENSOR4, ON);
		gui1->showAlarm(SENSOR7, ON);
		gui1->showAlarm(SENSOR8, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 2, 3, 4, 7, 8 detected!</b>");
			gui1->appendLog("<b>Turning left...</b>");
			
			//----------------
			// left
			//----------------
			drive(LEFT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 207)
	{
		gui1->showAlarm(SENSOR1, ON);
		gui1->showAlarm(SENSOR2, ON);
		gui1->showAlarm(SENSOR3, ON);
		gui1->showAlarm(SENSOR4, ON);
		gui1->showAlarm(SENSOR7, ON);
		gui1->showAlarm(SENSOR8, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 1, 2, 3, 4, 7, 8 detected!</b>");
			gui1->appendLog("<b>Turning left...</b>");
			
			//----------------
			// left
			//----------------
			drive(LEFT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 208)
	{
		gui1->showAlarm(SENSOR5, ON);
		gui1->showAlarm(SENSOR7, ON);
		gui1->showAlarm(SENSOR8, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 5, 7, 8 detected!</b>");
			gui1->appendLog("<b>Driving forward...</b>");
			
			//----------------
			// forward
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
	
	
	if (sensorAlarm == 209)
	{
		gui1->showAlarm(SENSOR1, ON);
		gui1->showAlarm(SENSOR5, ON);
		gui1->showAlarm(SENSOR7, ON);
		gui1->showAlarm(SENSOR8, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 1, 5, 7, 8 detected!</b>");
			gui1->appendLog("<b>Turning right...</b>");
			
			//----------------
			// right
			//----------------
			drive(RIGHT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 210)
	{
		gui1->showAlarm(SENSOR2, ON);
		gui1->showAlarm(SENSOR5, ON);
		gui1->showAlarm(SENSOR7, ON);
		gui1->showAlarm(SENSOR8, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 2, 5, 7, 8 detected!</b>");
			gui1->appendLog("<b>Turning left...</b>");
			
			//----------------
			// left
			//----------------
			drive(LEFT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 211)
	{
		gui1->showAlarm(SENSOR1, ON);
		gui1->showAlarm(SENSOR2, ON);
		gui1->showAlarm(SENSOR5, ON);
		gui1->showAlarm(SENSOR7, ON);
		gui1->showAlarm(SENSOR8, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 1, 2, 5, 7, 8 detected!</b>");
			gui1->appendLog("<b>Waiting...</b>");
			
			//----------------
			// wait
			//----------------
			drive(WAIT);
			motors->flashlight(ON);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 212)
	{
		gui1->showAlarm(SENSOR3, ON);
		gui1->showAlarm(SENSOR5, ON);
		gui1->showAlarm(SENSOR7, ON);
		gui1->showAlarm(SENSOR8, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 3, 5, 7, 8 detected!</b>");
			gui1->appendLog("<b>Driving forward...</b>");
			
			//----------------
			// forward
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
	
	
	if (sensorAlarm == 213)
	{
		gui1->showAlarm(SENSOR1, ON);
		gui1->showAlarm(SENSOR3, ON);
		gui1->showAlarm(SENSOR5, ON);
		gui1->showAlarm(SENSOR7, ON);
		gui1->showAlarm(SENSOR8, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 1, 3, 5, 7, 8 detected!</b>");
			gui1->appendLog("<b>Turning right...</b>");
			
			//----------------
			// right
			//----------------
			drive(RIGHT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 214)
	{
		gui1->showAlarm(SENSOR2, ON);
		gui1->showAlarm(SENSOR3, ON);
		gui1->showAlarm(SENSOR5, ON);
		gui1->showAlarm(SENSOR7, ON);
		gui1->showAlarm(SENSOR8, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 2, 3, 5, 7, 8 detected!</b>");
			gui1->appendLog("<b>Turning left...</b>");
			
			//----------------
			// left
			//----------------
			drive(LEFT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 215)
	{
		gui1->showAlarm(SENSOR1, ON);
		gui1->showAlarm(SENSOR2, ON);
		gui1->showAlarm(SENSOR3, ON);
		gui1->showAlarm(SENSOR5, ON);
		gui1->showAlarm(SENSOR7, ON);
		gui1->showAlarm(SENSOR8, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 1, 2, 3, 5, 7, 8 detected!</b>");
			gui1->appendLog("<b>Waiting...</b>");
			
			//----------------
			// wait
			//----------------
			drive(WAIT);
			motors->flashlight(ON);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 216)
	{
		gui1->showAlarm(SENSOR4, ON);
		gui1->showAlarm(SENSOR5, ON);
		gui1->showAlarm(SENSOR7, ON);
		gui1->showAlarm(SENSOR8, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 4, 5, 7, 8 detected!</b>");
			gui1->appendLog("<b>Driving forward...</b>");
			
			//----------------
			// forward
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
	
	
	if (sensorAlarm == 217)
	{
		gui1->showAlarm(SENSOR1, ON);
		gui1->showAlarm(SENSOR4, ON);
		gui1->showAlarm(SENSOR5, ON);
		gui1->showAlarm(SENSOR7, ON);
		gui1->showAlarm(SENSOR8, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 1, 4, 5, 7, 8 detected!</b>");
			gui1->appendLog("<b>Turning right...</b>");
			
			//----------------
			// right
			//----------------
			drive(RIGHT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 218)
	{
		gui1->showAlarm(SENSOR2, ON);
		gui1->showAlarm(SENSOR4, ON);
		gui1->showAlarm(SENSOR5, ON);
		gui1->showAlarm(SENSOR7, ON);
		gui1->showAlarm(SENSOR8, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 2, 4, 5, 7, 8 detected!</b>");
			gui1->appendLog("<b>Turning left...</b>");
			
			//----------------
			// left
			//----------------
			drive(LEFT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 219)
	{
		gui1->showAlarm(SENSOR1, ON);
		gui1->showAlarm(SENSOR2, ON);
		gui1->showAlarm(SENSOR4, ON);
		gui1->showAlarm(SENSOR5, ON);
		gui1->showAlarm(SENSOR7, ON);
		gui1->showAlarm(SENSOR8, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 1, 2, 4, 5, 7, 8 detected!</b>");
			gui1->appendLog("<b>Waiting...</b>");
			
			//----------------
			// wait
			//----------------
			drive(WAIT);
			motors->flashlight(ON);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 220)
	{
		gui1->showAlarm(SENSOR3, ON);
		gui1->showAlarm(SENSOR4, ON);
		gui1->showAlarm(SENSOR5, ON);
		gui1->showAlarm(SENSOR7, ON);
		gui1->showAlarm(SENSOR8, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 3, 4, 5, 7, 8 detected!</b>");
			gui1->appendLog("<b>Driving forward...</b>");
			
			//----------------
			// forward
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
	
	
	if (sensorAlarm == 221)
	{
		gui1->showAlarm(SENSOR1, ON);
		gui1->showAlarm(SENSOR3, ON);
		gui1->showAlarm(SENSOR4, ON);
		gui1->showAlarm(SENSOR5, ON);
		gui1->showAlarm(SENSOR7, ON);
		gui1->showAlarm(SENSOR8, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 1, 3, 4, 5, 7, 8 detected!</b>");
			gui1->appendLog("<b>Turning right...</b>");
			
			//----------------
			// right
			//----------------
			drive(RIGHT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 222)
	{
		gui1->showAlarm(SENSOR2, ON);
		gui1->showAlarm(SENSOR3, ON);
		gui1->showAlarm(SENSOR4, ON);
		gui1->showAlarm(SENSOR5, ON);
		gui1->showAlarm(SENSOR7, ON);
		gui1->showAlarm(SENSOR8, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 2, 3, 4, 5, 7, 8 detected!</b>");
			gui1->appendLog("<b>Turning left...</b>");
			
			//----------------
			// left
			//----------------
			drive(LEFT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 223)
	{
		gui1->showAlarm(SENSOR1, ON);
		gui1->showAlarm(SENSOR2, ON);
		gui1->showAlarm(SENSOR3, ON);
		gui1->showAlarm(SENSOR4, ON);
		gui1->showAlarm(SENSOR5, ON);
		gui1->showAlarm(SENSOR7, ON);
		gui1->showAlarm(SENSOR8, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 1, 2, 3, 4, 5, 7, 8 detected!</b>");
			gui1->appendLog("<b>Waiting...</b>");
			
			//----------------
			// wait
			//----------------
			drive(WAIT);
			motors->flashlight(ON);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 224)
	{
		gui1->showAlarm(SENSOR6, ON);
		gui1->showAlarm(SENSOR7, ON);
		gui1->showAlarm(SENSOR8, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 6, 7, 8 detected!</b>");
			gui1->appendLog("<b>Turning left...</b>");
			
			//----------------
			// left
			//----------------
			drive(LEFT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 225)
	{
		gui1->showAlarm(SENSOR1, ON);
		gui1->showAlarm(SENSOR6, ON);
		gui1->showAlarm(SENSOR7, ON);
		gui1->showAlarm(SENSOR8, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 1, 6, 7, 8 detected!</b>");
			gui1->appendLog("<b>Turning left...</b>");
			
			//----------------
			// left
			//----------------
			drive(LEFT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 226)
	{
		gui1->showAlarm(SENSOR2, ON);
		gui1->showAlarm(SENSOR6, ON);
		gui1->showAlarm(SENSOR7, ON);
		gui1->showAlarm(SENSOR8, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 2, 6, 7, 8 detected!</b>");
			gui1->appendLog("<b>Turning left...</b>");
			
			//----------------
			// left
			//----------------
			drive(LEFT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 227)
	{
		gui1->showAlarm(SENSOR1, ON);
		gui1->showAlarm(SENSOR2, ON);
		gui1->showAlarm(SENSOR6, ON);
		gui1->showAlarm(SENSOR7, ON);
		gui1->showAlarm(SENSOR8, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 1, 2, 6, 7, 8 detected!</b>");
			gui1->appendLog("<b>Turning left...</b>");
			
			//----------------
			// left
			//----------------
			drive(LEFT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 228)
	{
		gui1->showAlarm(SENSOR3, ON);
		gui1->showAlarm(SENSOR6, ON);
		gui1->showAlarm(SENSOR7, ON);
		gui1->showAlarm(SENSOR8, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 3, 6, 7, 8 detected!</b>");
			gui1->appendLog("<b>Turning left...</b>");
			
			//----------------
			// left
			//----------------
			drive(LEFT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 229)
	{
		gui1->showAlarm(SENSOR1, ON);
		gui1->showAlarm(SENSOR3, ON);
		gui1->showAlarm(SENSOR6, ON);
		gui1->showAlarm(SENSOR7, ON);
		gui1->showAlarm(SENSOR8, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 1, 3, 6, 7, 8 detected!</b>");
			gui1->appendLog("<b>Turning left...</b>");
			
			//----------------
			// left
			//----------------
			drive(LEFT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 230)
	{
		gui1->showAlarm(SENSOR2, ON);
		gui1->showAlarm(SENSOR3, ON);
		gui1->showAlarm(SENSOR6, ON);
		gui1->showAlarm(SENSOR7, ON);
		gui1->showAlarm(SENSOR8, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 2, 3, 6, 7, 8 detected!</b>");
			gui1->appendLog("<b>Turning left...</b>");
			
			//----------------
			// left
			//----------------
			drive(LEFT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 231)
	{
		gui1->showAlarm(SENSOR1, ON);
		gui1->showAlarm(SENSOR2, ON);
		gui1->showAlarm(SENSOR3, ON);
		gui1->showAlarm(SENSOR6, ON);
		gui1->showAlarm(SENSOR7, ON);
		gui1->showAlarm(SENSOR8, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 1, 2, 3, 6, 7, 8 detected!</b>");
			gui1->appendLog("<b>Turning left...</b>");
			
			//----------------
			// left
			//----------------
			drive(LEFT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 232)
	{
		gui1->showAlarm(SENSOR4, ON);
		gui1->showAlarm(SENSOR6, ON);
		gui1->showAlarm(SENSOR7, ON);
		gui1->showAlarm(SENSOR8, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 4, 6, 7, 8 detected!</b>");
			gui1->appendLog("<b>Turning left...</b>");
			
			//----------------
			// left
			//----------------
			drive(LEFT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 233)
	{
		gui1->showAlarm(SENSOR1, ON);
		gui1->showAlarm(SENSOR4, ON);
		gui1->showAlarm(SENSOR6, ON);
		gui1->showAlarm(SENSOR7, ON);
		gui1->showAlarm(SENSOR8, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 1, 4, 6, 7, 8 detected!</b>");
			gui1->appendLog("<b>Turning left...</b>");
			
			//----------------
			// left
			//----------------
			drive(LEFT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 234)
	{
		gui1->showAlarm(SENSOR2, ON);
		gui1->showAlarm(SENSOR4, ON);
		gui1->showAlarm(SENSOR6, ON);
		gui1->showAlarm(SENSOR7, ON);
		gui1->showAlarm(SENSOR8, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 2, 4, 6, 7, 8 detected!</b>");
			gui1->appendLog("<b>Turning left...</b>");
			
			//----------------
			// left
			//----------------
			drive(LEFT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 235)
	{
		gui1->showAlarm(SENSOR1, ON);
		gui1->showAlarm(SENSOR2, ON);
		gui1->showAlarm(SENSOR4, ON);
		gui1->showAlarm(SENSOR6, ON);
		gui1->showAlarm(SENSOR7, ON);
		gui1->showAlarm(SENSOR8, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 1, 2, 4, 6, 7, 8 detected!</b>");
			gui1->appendLog("<b>Turning left...</b>");
			
			//----------------
			// left
			//----------------
			drive(LEFT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 236)
	{
		gui1->showAlarm(SENSOR3, ON);
		gui1->showAlarm(SENSOR4, ON);
		gui1->showAlarm(SENSOR6, ON);
		gui1->showAlarm(SENSOR7, ON);
		gui1->showAlarm(SENSOR8, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 3, 4, 6, 7, 8 detected!</b>");
			gui1->appendLog("<b>Turning left...</b>");
			
			//----------------
			// left
			//----------------
			drive(LEFT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 237)
	{
		gui1->showAlarm(SENSOR1, ON);
		gui1->showAlarm(SENSOR3, ON);
		gui1->showAlarm(SENSOR4, ON);
		gui1->showAlarm(SENSOR6, ON);
		gui1->showAlarm(SENSOR7, ON);
		gui1->showAlarm(SENSOR8, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 1, 3, 4, 6, 7, 8 detected!</b>");
			gui1->appendLog("<b>Turning left...</b>");
			
			//----------------
			// left
			//----------------
			drive(LEFT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 238)
	{
		gui1->showAlarm(SENSOR2, ON);
		gui1->showAlarm(SENSOR3, ON);
		gui1->showAlarm(SENSOR4, ON);
		gui1->showAlarm(SENSOR6, ON);
		gui1->showAlarm(SENSOR7, ON);
		gui1->showAlarm(SENSOR8, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 2, 3, 4, 6, 7, 8 detected!</b>");
			gui1->appendLog("<b>Turning left...</b>");
			
			//----------------
			// left
			//----------------
			drive(LEFT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 239)
	{
		gui1->showAlarm(SENSOR1, ON);
		gui1->showAlarm(SENSOR2, ON);
		gui1->showAlarm(SENSOR3, ON);
		gui1->showAlarm(SENSOR4, ON);
		gui1->showAlarm(SENSOR6, ON);
		gui1->showAlarm(SENSOR7, ON);
		gui1->showAlarm(SENSOR8, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 1, 2, 3, 4, 6, 7, 8 detected!</b>");
			gui1->appendLog("<b>Turning left...</b>");
			
			//----------------
			// left
			//----------------
			drive(LEFT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 240)
	{
		gui1->showAlarm(SENSOR5, ON);
		gui1->showAlarm(SENSOR6, ON);
		gui1->showAlarm(SENSOR7, ON);
		gui1->showAlarm(SENSOR8, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 5, 6, 7, 8 detected!</b>");
			gui1->appendLog("<b>Waiting...</b>");
			
			//----------------
			// wait
			//----------------
			drive(WAIT);
			motors->flashlight(ON);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 241)
	{
		gui1->showAlarm(SENSOR1, ON);
		gui1->showAlarm(SENSOR5, ON);
		gui1->showAlarm(SENSOR6, ON);
		gui1->showAlarm(SENSOR7, ON);
		gui1->showAlarm(SENSOR8, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 1, 5, 6, 7, 8 detected!</b>");
			gui1->appendLog("<b>Turning right...</b>");
			
			//----------------
			// right
			//----------------
			drive(RIGHT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 242)
	{
		gui1->showAlarm(SENSOR2, ON);
		gui1->showAlarm(SENSOR5, ON);
		gui1->showAlarm(SENSOR6, ON);
		gui1->showAlarm(SENSOR7, ON);
		gui1->showAlarm(SENSOR8, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 2, 5, 6, 7, 8 detected!</b>");
			gui1->appendLog("<b>Turning left...</b>");
			
			//----------------
			// left
			//----------------
			drive(LEFT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 243)
	{
		gui1->showAlarm(SENSOR1, ON);
		gui1->showAlarm(SENSOR2, ON);
		gui1->showAlarm(SENSOR5, ON);
		gui1->showAlarm(SENSOR6, ON);
		gui1->showAlarm(SENSOR7, ON);
		gui1->showAlarm(SENSOR8, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 1, 2, 5, 6, 7, 8 detected!</b>");
			gui1->appendLog("<b>Waiting...</b>");
			
			//----------------
			// wait
			//----------------
			drive(WAIT);
			motors->flashlight(ON);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 244)
	{
		gui1->showAlarm(SENSOR3, ON);
		gui1->showAlarm(SENSOR5, ON);
		gui1->showAlarm(SENSOR6, ON);
		gui1->showAlarm(SENSOR7, ON);
		gui1->showAlarm(SENSOR8, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 3, 5, 6, 7, 8 detected!</b>");
			gui1->appendLog("<b>Turning left...</b>");
			
			//----------------
			// left
			//----------------
			drive(LEFT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 245)
	{
		gui1->showAlarm(SENSOR1, ON);
		gui1->showAlarm(SENSOR3, ON);
		gui1->showAlarm(SENSOR5, ON);
		gui1->showAlarm(SENSOR6, ON);
		gui1->showAlarm(SENSOR7, ON);
		gui1->showAlarm(SENSOR8, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 1, 3, 5, 6, 7, 8 detected!</b>");
			gui1->appendLog("<b>Turning right...</b>");
			
			//----------------
			// right
			//----------------
			drive(RIGHT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 246)
	{
		gui1->showAlarm(SENSOR2, ON);
		gui1->showAlarm(SENSOR3, ON);
		gui1->showAlarm(SENSOR5, ON);
		gui1->showAlarm(SENSOR6, ON);
		gui1->showAlarm(SENSOR7, ON);
		gui1->showAlarm(SENSOR8, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 2, 3, 5, 6, 7, 8 detected!</b>");
			gui1->appendLog("<b>Turning left...</b>");
			
			//----------------
			// left
			//----------------
			drive(LEFT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 247)
	{
		gui1->showAlarm(SENSOR1, ON);
		gui1->showAlarm(SENSOR2, ON);
		gui1->showAlarm(SENSOR3, ON);
		gui1->showAlarm(SENSOR5, ON);
		gui1->showAlarm(SENSOR6, ON);
		gui1->showAlarm(SENSOR7, ON);
		gui1->showAlarm(SENSOR8, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 1, 2, 3, 5, 6, 7, 8 detected!</b>");
			gui1->appendLog("<b>Waiting...</b>");
			
			//----------------
			// wait
			//----------------
			drive(WAIT);
			motors->flashlight(ON);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 248)
	{
		gui1->showAlarm(SENSOR4, ON);
		gui1->showAlarm(SENSOR5, ON);
		gui1->showAlarm(SENSOR6, ON);
		gui1->showAlarm(SENSOR7, ON);
		gui1->showAlarm(SENSOR8, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 4, 5, 6, 7, 8 detected!</b>");
			gui1->appendLog("<b>Turning right...</b>");
			
			//----------------
			// right
			//----------------
			drive(RIGHT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 249)
	{
		gui1->showAlarm(SENSOR1, ON);
		gui1->showAlarm(SENSOR4, ON);
		gui1->showAlarm(SENSOR5, ON);
		gui1->showAlarm(SENSOR6, ON);
		gui1->showAlarm(SENSOR7, ON);
		gui1->showAlarm(SENSOR8, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 1, 4, 5, 6, 7, 8 detected!</b>");
			gui1->appendLog("<b>Turning right...</b>");
			
			//----------------
			// right
			//----------------
			drive(RIGHT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 250)
	{
		gui1->showAlarm(SENSOR2, ON);
		gui1->showAlarm(SENSOR4, ON);
		gui1->showAlarm(SENSOR5, ON);
		gui1->showAlarm(SENSOR6, ON);
		gui1->showAlarm(SENSOR7, ON);
		gui1->showAlarm(SENSOR8, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 2, 4, 5, 6, 7, 8 detected!</b>");
			gui1->appendLog("<b>Turning left...</b>");
			
			//----------------
			// left
			//----------------
			drive(LEFT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 251)
	{
		gui1->showAlarm(SENSOR1, ON);
		gui1->showAlarm(SENSOR2, ON);
		gui1->showAlarm(SENSOR4, ON);
		gui1->showAlarm(SENSOR5, ON);
		gui1->showAlarm(SENSOR6, ON);
		gui1->showAlarm(SENSOR7, ON);
		gui1->showAlarm(SENSOR8, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 1, 2, 4, 5, 6, 7, 8 detected!</b>");
			gui1->appendLog("<b>Waiting...</b>");
			
			//----------------
			// wait
			//----------------
			drive(WAIT);
			motors->flashlight(ON);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 252)
	{
		gui1->showAlarm(SENSOR3, ON);
		gui1->showAlarm(SENSOR4, ON);
		gui1->showAlarm(SENSOR5, ON);
		gui1->showAlarm(SENSOR6, ON);
		gui1->showAlarm(SENSOR7, ON);
		gui1->showAlarm(SENSOR8, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 3, 4, 5, 6, 7, 8 detected!</b>");
			gui1->appendLog("<b>Turning right...</b>");
			
			//----------------
			// right
			//----------------
			drive(RIGHT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 253)
	{
		gui1->showAlarm(SENSOR1, ON);
		gui1->showAlarm(SENSOR3, ON);
		gui1->showAlarm(SENSOR4, ON);
		gui1->showAlarm(SENSOR5, ON);
		gui1->showAlarm(SENSOR6, ON);
		gui1->showAlarm(SENSOR7, ON);
		gui1->showAlarm(SENSOR8, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 1, 3, 4, 5, 6, 7, 8 detected!</b>");
			gui1->appendLog("<b>Turning right...</b>");
			
			//----------------
			// right
			//----------------
			drive(RIGHT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 254)
	{
		gui1->showAlarm(SENSOR2, ON);
		gui1->showAlarm(SENSOR3, ON);
		gui1->showAlarm(SENSOR4, ON);
		gui1->showAlarm(SENSOR5, ON);
		gui1->showAlarm(SENSOR6, ON);
		gui1->showAlarm(SENSOR7, ON);
		gui1->showAlarm(SENSOR8, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 2, 3, 4, 5, 6, 7, 8 detected!</b>");
			gui1->appendLog("<b>Turning left...</b>");
			
			//----------------
			// left
			//----------------
			drive(LEFT);
			motors->flashlight(OFF);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}
	
	
	if (sensorAlarm == 255)
	{
		gui1->showAlarm(SENSOR1, ON);
		gui1->showAlarm(SENSOR2, ON);
		gui1->showAlarm(SENSOR3, ON);
		gui1->showAlarm(SENSOR4, ON);
		gui1->showAlarm(SENSOR5, ON);
		gui1->showAlarm(SENSOR6, ON);
		gui1->showAlarm(SENSOR7, ON);
		gui1->showAlarm(SENSOR8, ON);
	
		if (robotDrives == true)
		{
			gui1->appendLog("<b>Obstacle in front of sensor 1, 2, 3, 4, 5, 6, 7, 8 detected!</b>");
			gui1->appendLog("<b>Waiting...</b>");
			
			//----------------
			// wait
			//----------------
			drive(WAIT);
			motors->flashlight(ON);
	
			
			// store this sensor alarm value
			lastSensorValue = sensorAlarm;
			// reset the alarm counter
			alarmCounter = 0;
		}
		return;
	}

	
	
	// ++++++++++++++++++++++++++++++++++++
	//
	//  STOP pasting logic code HERE !!!
	//
	// ++++++++++++++++++++++++++++++++++++
}


void Mrs::showSensorData()
{
	//----------------------------------------
	// show sensor values with progress bars
	//----------------------------------------
	gui1->showDistanceGraphical(SENSOR1, sensorThread->getDistance(SENSOR1));
	gui1->showDistanceGraphical(SENSOR2, sensorThread->getDistance(SENSOR2));
	gui1->showDistanceGraphical(SENSOR3, sensorThread->getDistance(SENSOR3));
	gui1->showDistanceGraphical(SENSOR4, sensorThread->getDistance(SENSOR4));
	gui1->showDistanceGraphical(SENSOR5, sensorThread->getDistance(SENSOR5));
	gui1->showDistanceGraphical(SENSOR6, sensorThread->getDistance(SENSOR6));
	gui1->showDistanceGraphical(SENSOR7, sensorThread->getDistance(SENSOR7));
	gui1->showDistanceGraphical(SENSOR8, sensorThread->getDistance(SENSOR8));
	gui1->showDistanceGraphical(SENSOR16, sensorThread->getUsSensorValue(SENSOR16));
	
	/*
	//-------------------------------------------------------
	// show distance value in a text label (in centimeters!)
	//-------------------------------------------------------
	gui1->showDistance(SENSOR1, sensorThread->getDistance(SENSOR1));
	gui1->showDistance(SENSOR2, sensorThread->getDistance(SENSOR2));
	gui1->showDistance(SENSOR3, sensorThread->getDistance(SENSOR3));
	gui1->showDistance(SENSOR4, sensorThread->getDistance(SENSOR4));
	gui1->showDistance(SENSOR5, sensorThread->getDistance(SENSOR5));
	gui1->showDistance(SENSOR6, sensorThread->getDistance(SENSOR6));
	gui1->showDistance(SENSOR7, sensorThread->getDistance(SENSOR7));
	gui1->showDistance(SENSOR8, sensorThread->getDistance(SENSOR8));
	gui1->showDistance(SENSOR16, sensorThread->getUsSensorValue(SENSOR16));
	*/
	
	//--------------------------------------------------------------
	// show driven distance value in a text label (in centimeters!)
	//--------------------------------------------------------------
	gui1->showDrivenDistance(MOTORSENSOR1, sensorThread->getDrivenDistance(MOTORSENSOR1));
	gui1->showDrivenDistance(MOTORSENSOR2, sensorThread->getDrivenDistance(MOTORSENSOR2));
}


void Mrs::drive(const unsigned char command)
{
	switch (command)
	{
		case FORWARD:
			gui1->showMotorStatus(MOTOR1, SAME, CLOCKWISE);
			gui1->showMotorStatus(MOTOR2, SAME, CLOCKWISE);
			motors->motorControl(MOTOR1, SAME, CLOCKWISE);
			motors->motorControl(MOTOR2, SAME, CLOCKWISE);
			return;
			break;
		case BACKWARD:
			gui1->showMotorStatus(MOTOR1, SAME, COUNTERCLOCKWISE);
			gui1->showMotorStatus(MOTOR2, SAME, COUNTERCLOCKWISE);
			motors->motorControl(MOTOR1, SAME, COUNTERCLOCKWISE);
			motors->motorControl(MOTOR2, SAME, COUNTERCLOCKWISE);
			return;
			break;
		case LEFT:
			gui1->showMotorStatus(MOTOR1, SAME, COUNTERCLOCKWISE);
			gui1->showMotorStatus(MOTOR2, SAME, CLOCKWISE);
			motors->motorControl(MOTOR1, SAME, COUNTERCLOCKWISE);
			motors->motorControl(MOTOR2, SAME, CLOCKWISE);
			return;
			break;
		case RIGHT:
			gui1->showMotorStatus(MOTOR1, SAME, CLOCKWISE);
			gui1->showMotorStatus(MOTOR2, SAME, COUNTERCLOCKWISE);
			motors->motorControl(MOTOR1, SAME, CLOCKWISE);
			motors->motorControl(MOTOR2, SAME, COUNTERCLOCKWISE);
			return;
			break;
		case START:
			if (robotIsOn == true)
			{
				robotDrives = true;
				gui1->appendLog("Starting to drive forward...");
				// set the motors to "drive FORWARD"
				gui1->showMotorStatus(MOTOR1, ON, CLOCKWISE);
				gui1->showMotorStatus(MOTOR2, ON, CLOCKWISE);

				motors->motorControl(MOTOR1, ON, CLOCKWISE);
				motors->motorControl(MOTOR2, ON, CLOCKWISE);
			}
			else
			{
				// show message
				gui1->appendLog("<font color=\"#FF0000\">Robot is OFF!</font>");
				robotDrives = false;
			}
			return;
			break;
		case WAIT:
			gui1->showMotorStatus(MOTOR1, OFF, SAME);
			gui1->showMotorStatus(MOTOR2, OFF, SAME);
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
			gui1->showMotorStatus(MOTOR1, OFF, SAME);
			gui1->showMotorStatus(MOTOR2, OFF, SAME);
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
	gui1->appendLog("Reading settings...");


	//---------------------------------------------------------------------
	// read setting
	serialPortMicrocontroller = inifile1->readString("Config", "serialPortMicrocontroller");
	
	if (serialPortMicrocontroller == "error2")
	{
		gui1->appendLog("<font color=\"#FF0000\">ini-file is not writeable!</font>");
	}
	else
	{
		if (serialPortMicrocontroller == "error1")
		{
			gui1->appendLog("<font color=\"#FF0000\">Value \"serialPortMicrocontroller\" not found in ini-file!</font>");
		}
		else
		{
			//
			// everything okay
			//
			gui1->appendLog(QString("Serial port for microcontroller set to %1.").arg(serialPortMicrocontroller));
		}
	}
	
	
	//---------------------------------------------------------------------
	// read setting / and error handling
	switch (inifile1->readSetting("Config", "noHardwareErrorMessages"))
	{
		case -2:
			gui1->appendLog("<font color=\"#FF0000\">ini-file is not writeable!</font>");
			break;
		case -1:
			gui1->appendLog("<font color=\"#FF0000\">Value \"noHardwareErrorMessages\"not found in ini-file!</font>");
			break;
		case 0:
			noHardwareErrorMessages = false;
			break;
		case 1:
			noHardwareErrorMessages = true;
			gui1->appendLog("<font color=\"#808080\">Suppressing hardware error messages (see ini-file)</font>");
			break;
	}

	//---------------------------------------------------------------------
	// read setting
	switch (inifile1->readSetting("Config", "saveOnExit"))
	{
		case -2:
			gui1->appendLog("<font color=\"#FF0000\">ini-file is not writeable!</font>");
			break;
		case -1:
			gui1->appendLog("<font color=\"#FF0000\">Value \"saveOnExit\"not found in ini-file!</font>");
			break;
		case Qt::Unchecked:
			// uncheck checkbox
			gui1->setCheckBoxSaveSettings(Qt::Unchecked);
			break;
		case Qt::Checked:
			// set checkbox
			gui1->setCheckBoxSaveSettings(Qt::Checked);
			break;
	}

	//---------------------------------------------------------------------
	// read setting
	switch (inifile1->readSetting("Config", "exitDialog"))
	{
		case -2:
			gui1->appendLog("<font color=\"#FF0000\">ini-file is not writeable!</font>");
			break;
		case -1:
			gui1->appendLog("<font color=\"#FF0000\">Value \"exitDialog\"not found in ini-file!</font>");
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
	minObstDist = inifile1->readSetting("Config", "minObstacleDistance");
	
	switch (minObstDist)
	{
		case -2:
			gui1->appendLog("<font color=\"#FF0000\">ini-file is not writeable!</font>");
			minObstDist = 0;
			break;
		case -1:
			gui1->appendLog("<font color=\"#FF0000\">Value \"minObstacleDistance\"not found in ini-file!</font>");
			minObstDist = 0;
			break;
		default:
			// set slider to the read value
			gui1->setSliderObstacleValue(minObstDist);
			// tell the  obstacle check thread the distance
			obstCheckThread->setMinObstacleDistance(minObstDist);
			// show text
			gui1->appendLog(QString("Obstacle distance set to %1 cm.").arg(minObstDist));
			break;
	}


	//---------------------------------------------------------------------
	// read setting
	int minObstDistLaserScanner = inifile1->readSetting("Config", "minObstDistLaserScanner");
	
	switch (minObstDist)
	{
		case -2:
			gui1->appendLog("<font color=\"#FF0000\">ini-file is not writeable!</font>");
			minObstDist = 0;
			break;
		case -1:
			gui1->appendLog("<font color=\"#FF0000\">Value \"minObstDistLaserScanner\"not found in ini-file!</font>");
			minObstDist = 0;
			break;
		default:
			// set slider to the read value
			gui1->setSliderObstacleLaserScannerValue(minObstDistLaserScanner);
			// tell the  obstacle check thread the distance
			obstCheckThread->setMinObstacleDistance(minObstDistLaserScanner);
			// show text
			gui1->appendLog(QString("Obstacle distance Laser Scanner set to %1 cm.").arg(minObstDistLaserScanner));
			break;
	}


	/*
	
	//
	// only for CMUCAM
	//
	
	//---------------------------------------------------------------------
	// read setting
	QString serialPortCamera;
	serialPortCamera = inifile1->readString("Config", "serialPortCamera");
	
	if (serialPortCamera == "error2")
	{
		gui1->appendLog("<font color=\"#FF0000\">ini-file is not writeable!</font>");
	}
	else
	{
		if (serialPortCamera == "error1")
		{
			gui1->appendLog("<font color=\"#FF0000\">Value \"serialPort\" not found in ini-file!</font>");
		}
		else
		{
			//
			// everything okay
			//
			gui1->appendLog(QString("Serial port for camera set to %1.").arg(serialPortCamera));
		}
	}
	*/
	
	
	//---------------------------------------------------------------------
	// read setting
	mot1Speed = inifile1->readSetting("Config", "motor1Speed");
	
	switch (mot1Speed)
	{
		case -2:
			gui1->appendLog("<font color=\"#FF0000\">ini-file is not writeable!</font>");
			mot1Speed = 0;
			break;
		case -1:
			gui1->appendLog("<font color=\"#FF0000\">Value \"motor1Speed\" not found in ini-file!</font>");
			mot1Speed = 0;
			break;
		default:
			if (mot1Speed > 254)
			{
				gui1->appendLog("<font color=\"#FF0000\">Value \"motor1Speed\" is greater than 255!! Value set to 255!</font>");
				mot1Speed = 255;
			}
			
			// set slider to the read value
			gui1->setSliderMotorSpeed(1, mot1Speed);
			// show text
			gui1->appendLog(QString("Motor1 speed set to %1.").arg(mot1Speed));
			break;
	}
	
	
	//---------------------------------------------------------------------
	// read setting
	mot2Speed = inifile1->readSetting("Config", "motor2Speed");
	
	switch (mot2Speed)
	{
		case -2:
			gui1->appendLog("<font color=\"#FF0000\">ini-file is not writeable!</font>");
			mot2Speed = 0;
			break;
		case -1:
			gui1->appendLog("<font color=\"#FF0000\">Value \"motor2Speed\" not found in ini-file!</font>");
			mot2Speed = 0;
			break;
		default:
			if (mot2Speed > 254)
			{
				gui1->appendLog("<font color=\"#FF0000\">Value \"motor2Speed\" is greater than 255!! Value set to 255!</font>");
				mot2Speed = 255;
			}
			
			// set slider to the read value
			gui1->setSliderMotorSpeed(2, mot2Speed);
			// show text
			gui1->appendLog(QString("Motor2 speed set to %1.").arg(mot2Speed));
			break;
	}
	
	//---------------------------------------------------------------------
	// read setting / and error handling
	int hello=inifile1->readSetting("Config", "noLaserScanner");
	
	switch (hello)
	{
		case -2:
			gui1->appendLog("<font color=\"#FF0000\">ini-file is not writeable!</font>");
			break;
		case -1:
			gui1->appendLog("<font color=\"#FF0000\">Value \"noLaserScanner\" not found in ini-file!</font>");
			break;
		case 0:
			// use the laser scanner
			noLaserScanner = false;
			break;
		case 1:
			gui1->appendLog("no laser scanner = true (1)");
			noLaserScanner = true;
			break;
	}


/*	
	//---------------------------------------------------------------------
	// read setting
	int servoPositionSensor1 = inifile1->readSetting("Config", "servoPositionSensor1");
	
	switch (servoPositionSensor1)
	{
		case -2:
			gui1->appendLog("<font color=\"#FF0000\">ini-file is not writeable!</font>");
			servoPositionSensor1 = 4;
			break;
		case -1:
			gui1->appendLog("<font color=\"#FF0000\">Value \"servoPositionSensor1\" not found in ini-file!</font>");
			servoPositionSensor1 = 4;
			break;
		default:
			// set slider to the read value
			gui1->setSliderPositionServo1(servoPositionSensor1);
			// show text
			gui1->appendLog(QString("Servo position sensor 1 set to %1.").arg(servoPositionSensor1));
			break;
	}


	//---------------------------------------------------------------------
	// read setting
	int servoPositionSensor2 = inifile1->readSetting("Config", "servoPositionSensor2");
	
	switch (servoPositionSensor2)
	{
		case -2:
			gui1->appendLog("<font color=\"#FF0000\">ini-file is not writeable!</font>");
			servoPositionSensor2 = 4;
			break;
		case -1:
			gui1->appendLog("<font color=\"#FF0000\">Value \"servoPositionSensor2\" not found in ini-file!</font>");
			servoPositionSensor2 = 4;
			break;
		default:
			// set slider to the read value
			gui1->setSliderPositionServo2(servoPositionSensor2);
			// show text
			gui1->appendLog(QString("Servo position sensor 2 set to %1.").arg(servoPositionSensor2));
			break;
	}
*/
}


void Mrs::enableRemoteControlListening(bool status)
{
	//-----------------------------------------------------------
	// start the network thread (getting commands via network)
	//-----------------------------------------------------------
	if (status == true)
	{
		//if (robotIsOn == true)
		//{
		if (netThread->isRunning() == false)
		{
			gui1->appendLog("Starting network thread...", false);
			netThread->start();
			gui1->appendLog("Network thread started.");
		}
		/*	
		}
			else
			{
				// show message
				gui1->appendLog("<font color=\"#FF0000\">Robot is OFF! Network thread NOT started!</font>");
			}
		*/
	}
	else
	{
		if (netThread->isRunning() == true)
		{
			gui1->appendLog("Stopping network thread...", false);
			netThread->stop();
			gui1->appendLog("Network thread stopped.");
		}
	}
}


void Mrs::executeRemoteCommand(QString command)
{
	//---------------------------------------------
	// this method executes the received commands
	//--------------------------------------------
	if (command == "start")
	{
		gui1->appendLog(tr("<font color=\"#0000FF\">Executing remote command \"%1\".</font>").arg(command));
		drive(START);
		return;
	}
	
	
	if (command == "forward")
	{
		gui1->appendLog(tr("<font color=\"#0000FF\">Executing remote command \"%1\".</font>").arg(command));
		drive(FORWARD);
		return;
	}
	
	
	if (command == "backward")
	{
		gui1->appendLog(tr("<font color=\"#0000FF\">Executing remote command \"%1\".</font>").arg(command));
		drive(BACKWARD);
		return;
	}


	if (command == "stop")
	{
		gui1->appendLog(tr("<font color=\"#0000FF\">Executing remote command \"%1\".</font>").arg(command));
		drive(STOP);
		return;
	}


	if (command == "left")
	{
		gui1->appendLog(tr("<font color=\"#0000FF\">Executing remote command \"%1\".</font>").arg(command));
		drive(LEFT);
		return;
	}


	if (command == "right")
	{
		gui1->appendLog(tr("<font color=\"#0000FF\">Executing remote command \"%1\".</font>").arg(command));
		drive(RIGHT);
		return;
	}


	if (command == "increasespeedmotor1")
	{
		gui1->appendLog(tr("<font color=\"#0000FF\">Executing remote command \"%1\".</font>").arg(command));
		
		int newSpeed = motors->getMotorSpeed(1) + 1;
		motors->setMotorSpeed(1, newSpeed);
		gui1->setSliderMotorSpeed(1, newSpeed);
		return;
	}


	if (command == "increasespeedmotor2")
	{
		gui1->appendLog(tr("<font color=\"#0000FF\">Executing remote command \"%1\".</font>").arg(command));
		
		int newSpeed = motors->getMotorSpeed(2) + 1;
		motors->setMotorSpeed(2, newSpeed);
		gui1->setSliderMotorSpeed(2, newSpeed);
		return;
	}


	if (command == "reducespeedmotor1")
	{
		gui1->appendLog(tr("<font color=\"#0000FF\">Executing remote command \"%1\".</font>").arg(command));
		
		int newSpeed = motors->getMotorSpeed(1) - 1;
		motors->setMotorSpeed(1, newSpeed);
		gui1->setSliderMotorSpeed(1, newSpeed);
		return;
	}


	if (command == "reducespeedmotor2")
	{
		gui1->appendLog(tr("<font color=\"#0000FF\">Executing remote command \"%1\".</font>").arg(command));
		
		int newSpeed = motors->getMotorSpeed(2) - 1;
		motors->setMotorSpeed(2, newSpeed);
		gui1->setSliderMotorSpeed(2, newSpeed);
		return;
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
		gui1->appendLog("<font color=\"#0000FF\">Simulation mode enabled!!</front>");
	
		if (laserThread->isRunning() == false)
		{
			gui1->appendLog("Starting Laser thread...", false);
			laserThread->start();
			gui1->appendLog("Laser thread started.");
		}
		
		if (sensorThread->isRunning() == false)
		{
			gui1->appendLog("Starting Sensor thread...", false);
			sensorThread->start();
			gui1->appendLog("Sensor thread started.");
		}
		
		if (plotThread->isRunning() == false)
		{
			gui1->appendLog("Starting plot thread...", false);
			plotThread->start();
			gui1->appendLog("Plot thread started.");
		}
		
		if (obstCheckThread->isRunning() == false)
		{
			gui1->appendLog("Starting obstacle check thread...", false);
			obstCheckThread->start();
			gui1->appendLog("Obstacle check thread started.");
		}
	}
	else
	{
		if (obstCheckThread->isRunning() == true)
		{
			gui1->appendLog("Stopping obstacle check thread...", false);
			obstCheckThread->stop();
			gui1->appendLog("Obstacle check thread stopped.");
		}
		
		if (plotThread->isRunning() == true)
		{
			gui1->appendLog("Stopping Plot thread...", false);
			plotThread->stop();
			gui1->appendLog("Plot thread stopped.");
		}
		
		if (sensorThread->isRunning() == true)
		{
			gui1->appendLog("Stopping Sensor thread...", false);
			sensorThread->stop();
			gui1->appendLog("Sensor thread stopped.");
		}
		
		if (laserThread->isRunning() == true)
		{
			gui1->appendLog("Stopping Laser thread...", false);
			laserThread->stop();
			gui1->appendLog("Laser thread stopped.");
		}
		
		
		gui1->appendLog("<font color=\"#0000FF\">Simulation mode disabled.</font>");
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
	static unsigned char toggle = OFF;
	
	if (toggle == OFF)
		toggle = ON;
	else
		toggle = OFF;
	
	//motors->flashlight(toggle);
	
	#ifdef _TTY_POSIX_
	// Say some text;
	QDateTime now = QDateTime::currentDateTime();
	emit speak(tr("Hello Markus. Today it's the %1 of %2, %3. The time is %4:%5.").arg(now.toString("d")).arg(now.toString("MMMM")).arg(now.toString("yyyy")).arg(now.toString("h")).arg(now.toString("m")));
	#endif

}
