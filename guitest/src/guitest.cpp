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

#include "guitest.h"


int main(int argc, char *argv[])
{
	// Initialize the resource file
	Q_INIT_RESOURCE(guitest);

	// The QApplication class manages the GUI application's control flow and main settings.
	QApplication app(argc, argv);

	// create the Direcs class object
	GuiTest g();

	// init direcs
	g.init();

	return app.exec();
}


GuiTest::GuiTest()
{
	// store mode from main method
	consoleMode = false;

	//--------------------------
	// create all other objects
	//--------------------------
	gui = new Gui(false);
}


void GuiTest::init()
{
	forceShutdown = false;
	laserscannerTypeFront= "error1";
	laserscannerTypeRear= "error1";
	// 	robotIsOn = false;
	robotDrives = false;
	robotSimulationMode = false;
	laserScannerFrontFound = false;
	laserScannerRearFound = false;
	laserscannerAngleFront=0;
	laserscannerAngleRear=0;
	preferredDrivingDirection = FORWARD; // this is to allow the robot to drive forward, if the obstacleCheckThread is not running (because of offline laser). Mainly for testing reasons!

	//------------------------------------------------------------------
	// Set the number format to "," for comma and 1000 separator to "."
	// For example: 1.234,00 EUR
	//------------------------------------------------------------------
	QLocale::setDefault(QLocale::German);

	//--------------------------------------------------------------------------
	connect(this, SIGNAL( message(QString, bool, bool, bool) ), gui, SLOT( appendLog(QString, bool, bool, bool) ));


	//--------------------------------------------------------------------------
	// show a QMessage wth the possibility to exit the main programm, when errors occured!
	//--------------------------------------------------------------------------


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


	//--------------------------------------------------------------------------
	// Check for the programm ini file
	// AND read the settings  AND   d o   a l l   t h e   r e s t  ! ! !
	//--------------------------------------------------------------------------
	if (true)
	{

		if (!consoleMode)
		{
			//----------------------------------------------------------------------------
			// show the preferred driving direction in a GUI label
			//----------------------------------------------------------------------------
			connect(this, SIGNAL(showPreferredDirection(QString)), gui, SLOT(showPreferredDirection(QString)));
		}


#ifdef ACTIVELASERVIEW
		if (!consoleMode)
		{
			//----------------------------------------------------------------------------
			// connect laserThread signal to "dataReceived"
			// (Whenever data were received, the data are shown in the GUI)
			//----------------------------------------------------------------------------
			qRegisterMetaType < QList <float> > ("QList <float>");
			qRegisterMetaType < QList <int> > ("QList <int>");
		}
#endif


		if (!consoleMode)
		{
			//----------------------------------------------------------------------------
			// connect simulation button from gui to activate the simulation mode
			// (sets the direcs an the threads to simulation mode)
			//----------------------------------------------------------------------------
			connect(gui, SIGNAL( simulate(bool) ), this, SLOT( setSimulationMode(bool) ));
		}


		//---------------------------------------------------------------------
		// check if laser scanners are connected
		//---------------------------------------------------------------------
		// check FRONT laser
		laserScannerFrontFound = true;

		// check REAR laser
		laserScannerRearFound = false;

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


		//------------------------------------------------------------------
		// place gui window at a nice position on the screen
		//------------------------------------------------------------------
		// move mainWindow to the center of the screen
		gui->move( (desktop->width() - gui->width())/2, (desktop->height() - gui->height())/2 );

		// show the gui
		gui->show();


		// one time init for the laser view
		gui->initLaserView();


	} // ini-file found
}


void GuiTest::shutdown()
{
	emit message("----------------");
	emit message("Shutting down...");
	emit message("----------------");





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


	/// \todo a universal quit-threads-method

	if (consoleMode)
	{
		// In the gui mode the quit is done automatically by the close signal.
		// In the Console mode, the following line automaticall calls the Direcs destructor.
		QCoreApplication::quit();
	}
}


GuiTest::~GuiTest()
{
	//--------------------------------------------------
	// clean up in reverse order (except from the gui)
	//--------------------------------------------------

	qDebug("Bye.");

	delete gui;
}


void GuiTest::showExitDialog()
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


void GuiTest::readSettings()
{
	//---------------------------------------------------------------------
	// get the programm settings and set the items on the gui (sliders...)
	//---------------------------------------------------------------------
	emit message("Reading settings...");


	//---------------------------------------------------------------------
	// read first FRONT laser setting
	laserscannerTypeFront = "S300";

	if (true)
	{

		if (true)
		{
			// everything okay

			//---------------------------------------------------------------------
			// read next laser setting
			laserscannerMounting = "normal";

			if (true)
			{
				emit message(QString("Front laser scanner mounting set to <b>%1</b>.").arg(laserscannerMounting));

				//---------------------------------------------------------------------
				// read next setting
				laserscannerAngleFront = 270;

				switch (laserscannerAngleFront)
				{
					default:
							gui->setLaserscannerAngle(LASER1, laserscannerAngleFront);
						emit message(QString("Front laser scanner angle set to <b>%1</b>.").arg(laserscannerAngleFront));

						//---------------------------------------------------------------------
						// read next setting
						floatValue = 0.5;

						if (1)
						{
							gui->setLaserscannerResolution(LASER1, floatValue);
							emit message(QString("Front laser scanner resolution set to <b>%1</b>.").arg(floatValue));
						}
					break;
				}
			}
		}
	}


	//---------------------------------------------------------------------
	// Create the laser lines and pixmaps in the GUI and place them nicely
	//---------------------------------------------------------------------
	gui->initLaserStuff();
}


bool GuiTest::simulationMode() const
{
	return robotSimulationMode;
}


void GuiTest::setSimulationMode(bool status)
{
	robotSimulationMode = status;

	if (status == true)
	{
		emit message("<font color=\"#0000FF\">Simulation mode enabled!!</front>");
	}
	else
	{
		emit message("<font color=\"#0000FF\">Simulation mode disabled.</font>");
	}
}


void GuiTest::checkArguments()
{
	if (arguments.contains("console", Qt::CaseInsensitive))
	{
		consoleMode = true;
		qDebug("CONSOLE mode activated. Now passing all messages to the console.");
	}
}


void GuiTest::test()
{
//	static bool toggle = false;


//	toggle = !toggle;
}
