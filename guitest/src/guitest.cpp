/*************************************************************************
 *   Copyright (C) 2011 by Markus Knapp                                  *
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
	Q_INIT_RESOURCE(guitest);

	// The QApplication class manages the GUI application's control flow and main settings.
	QApplication app(argc, argv);

	// create the Direcs-avrsim class object
	GuiTest d(consoleMode);

	// init direcs
	d.init();

	return app.exec();
}


GuiTest::GuiTest(bool bConsoleMode)
{
	Q_UNUSED(bConsoleMode);

	//------------------------------------------------------------------
	// create the objects
	//------------------------------------------------------------------
	gui = new Gui();
//	splash = new QSplashScreen(QPixmap(":/images/images/splash.png"));
}


void GuiTest::init()
{
//	splashPosition = Qt::AlignHCenter | Qt::AlignBottom;
//	splashColor = Qt::red;
//	serialPortMicrocontroller = "/dev/tty.SLAB_USBtoUART"; /// this is the seperate serial adapter, but the same as on the Atmel-Board!
	serialPortMicrocontroller = "/dev/tty.PL2303-003014FA"; /// this is the PL2303, old 'LaserScanerFront' adapter
//	serialPortMicrocontroller = "/dev/ttyLaserScannerFront"; /// this is the PL2303, old 'LaserScanerFront' adapter
//	serialPortMicrocontroller = "/dev/ttyAtmelBoard";
	robotSimulationMode = false;

	//--------------------------------------------------------------------------
	// show the splash screen
	//--------------------------------------------------------------------------
//	splash->show();

	//--------------------------------------------------------------------------
	// show splash messages in the splash screen
	//--------------------------------------------------------------------------
//	connect(this, SIGNAL( splashMessage(QString) ), this, SLOT( showSplashMessage(QString) ));


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
	// send status messages to the GUI
	//--------------------------------------------------------------------------
	connect(this, SIGNAL( message(QString, bool, bool, bool) ), gui, SLOT( appendLog(QString, bool, bool, bool) ));

	//--------------------------------------------------------------------------
	// shutdown Direcs-avrsim program on exit button
	// shutdown is also called, when the gui is closed
	//--------------------------------------------------------------------------
	connect(gui, SIGNAL(shutdown()), this, SLOT(shutdown()));

	//--------------------------------------------------------------------------
	// call (a) test method(s) when clicking the test button
	//--------------------------------------------------------------------------
	connect(gui, SIGNAL(test()), this, SLOT(test()));

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
//	QTimer::singleShot(SPLASHTIME, this, SLOT( finishSplash() ));




	//--------------------------
	// lets have fun, now
	//--------------------------

	// one time init for the laser view
//	gui->initLaserView();				// < < < < < < < < <
}


void GuiTest::shutdown()
{
	qDebug("Direcs-avrsim shutdown...");

//	splash->show();
//	emit splashMessage("Shutting down...");


	//-----------------------------
	// close serial port to mc
	//-----------------------------
	emit message("Closing serial port to microcontroller...");

	//-----------------------------
	// close serial port to mc
	//-----------------------------
	emit message("Closing serial port to microcontroller...");
}


GuiTest::~GuiTest()
{
	//--------------------------------------------------
	// clean up in reverse order (except from the gui)
	//--------------------------------------------------
	qDebug("Bye.");
//	delete splash;
	delete gui;
}


void GuiTest::showSplashMessage(QString text)
{
	splash->showMessage(text, splashPosition, splashColor);
	// for refreshing the splash...
	QApplication::processEvents();
}


void GuiTest::finishSplash()
{
	splash->finish(gui);
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
		qDebug("CONSOLE mode activated. Now passing all messages to the console.");
	}
}


void GuiTest::test()
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
