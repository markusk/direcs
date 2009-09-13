/*************************************************************************
 *   Copyright (C) 2009 by Markus Knapp                                  *
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

#include "direcs-avrsim.h"


int main(int argc, char *argv[])
{
	bool consoleMode = false;
	
	
	// Check for command-line argument "console".
	// A deeper argument check will be done later within the class Direcs-avrsim!
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
	
	
	//----------------------
	// CREATE A GUI APP
	//----------------------
	
	// Initialize the resource file
	Q_INIT_RESOURCE(direcsAvrsim);

	// The QApplication class manages the GUI application's control flow and main settings.
	QApplication app(argc, argv);

	// create the Direcs-avrsim class object
	DirecsAvrsim d(consoleMode);
	
	// init direcs
	d.init();
	
	return app.exec();
}


DirecsAvrsim::DirecsAvrsim(bool bConsoleMode)
{
	Q_UNUSED(bConsoleMode);
	
	//------------------------------------------------------------------
	// create the objects
	//------------------------------------------------------------------
	gui = new Gui();
	splash = new QSplashScreen(QPixmap(":/images/images/splash.png"));

	mutex = new QMutex();
	interface1 = new InterfaceAvr();
}


void DirecsAvrsim::init()
{
	splashPosition = Qt::AlignHCenter | Qt::AlignBottom;
	splashColor = Qt::red;
	serialPortMicrocontroller = "/dev/ttyLaserScannerFront";
	robotSimulationMode = false;
	
	//--------------------------------------------------------------------------
	// show the splash screen
	//--------------------------------------------------------------------------
	splash->show();
	
	//--------------------------------------------------------------------------
	// show splash messages in the splash screen
	//--------------------------------------------------------------------------
	connect(this, SIGNAL( splashMessage(QString) ), this, SLOT( showSplashMessage(QString) ));

	
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

	connect(this, SIGNAL( message(QString) ), gui, SLOT( appendLog(QString) ));

	//--------------------------------------------------------------------------
	// let some classes know the robots state
	//--------------------------------------------------------------------------
	// this is needed, when the first openCOMPort method fails:
	//connect(interface1,	SIGNAL( robotState(bool) ), gui, SLOT( setRobotControls(bool) ));
	
	//--------------------------------------------------------------------------
	// shutdown Direcs-avrsim program on exit button
	// shutdown is also called, when the gui is closed
	//--------------------------------------------------------------------------
	connect(gui, SIGNAL(shutdown()), this, SLOT(shutdown()));

	//--------------------------------------------------------------------------
	// call (a) test method(s) when clicking the test button
	//--------------------------------------------------------------------------
	connect(gui, SIGNAL(test()), this, SLOT(test()));
	


	//-------------------------------------------------------
	// Open serial port for microcontroller communication
	//-------------------------------------------------------
	emit message("Opening serial port for microcontroller communication...", false);

	if (interface1->openComPort(serialPortMicrocontroller) == false)
	{
		//qDebug() << "Error opening serial port" << serialPortMicrocontroller;
		emit message(QString("<font color=\"#FF0000\">*** ERROR opening serial port '%1'! ***</font>").arg(serialPortMicrocontroller));

		// show a warning dialog!
		QMessageBox msgbox(QMessageBox::Warning, tr("Error with robots serial port"), tr("Error opening serial port %1").arg(serialPortMicrocontroller), QMessageBox::Ok | QMessageBox::Default);
		msgbox.exec();

		// no serial port, no robot :-(
// 		robotIsOn = false;
	}
	else
	{
		// *******************
		// * The robot is ON *
		// *******************
		emit message("Serial port opened.");
	} // robot is ON

	
	//----------------------------------------------------------------------------
	// connect simulation button from gui to activate the simulation mode
	// (sets the direcs an the threads to simulation mode)
	//----------------------------------------------------------------------------
	connect(gui, SIGNAL( simulate(bool) ), this, SLOT( setSimulationMode(bool) ));

	
	//----------------------------------------------------------------------------
	// show the gui
	//----------------------------------------------------------------------------
	gui->show();

	// delete the splash screen
 	QTimer::singleShot(SPLASHTIME, this, SLOT( finishSplash() ));
}


void DirecsAvrsim::shutdown()
{
	qDebug("Direcs-avrsim shutdown...");

	splash->show();
	emit splashMessage("Shutting down...");

	//-----------------------------
	// close serial port to mc
	//-----------------------------
	emit message("Closing serial port to microcontroller...");

	//-----------------------------
	// close serial port to mc
	//-----------------------------
	emit message("Closing serial port to microcontroller...");
	interface1->closeComPort();
}


DirecsAvrsim::~DirecsAvrsim()
{
	//--------------------------------------------------
	// clean up in reverse order (except from the gui)
	//--------------------------------------------------
	qDebug("Bye.");
	delete splash;
	delete gui;
}


void DirecsAvrsim::showSplashMessage(QString text)
{
	splash->showMessage(text, splashPosition, splashColor);
	// for refreshing the splash...
	QApplication::processEvents();
}


void DirecsAvrsim::finishSplash()
{
	splash->finish(gui);
}


bool DirecsAvrsim::simulationMode() const
{
	return robotSimulationMode;
}


void DirecsAvrsim::setSimulationMode(bool status)
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


void DirecsAvrsim::checkArguments()
{
	if (arguments.contains("console", Qt::CaseInsensitive))
	{
		qDebug("CONSOLE mode activated. Now passing all messages to the console.");
	}
}


void DirecsAvrsim::test()
{
	static bool toggle = OFF;


	if (toggle == OFF)
	{
		toggle = ON;
	}
	else
	{
		toggle = OFF;
	}
}