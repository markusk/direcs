/***************************************************************************
 *   Copyright (C) 2011 by Markus Knapp                                    *
 *   www.direcs.de                                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/


#include "kinect.h"


int main(int argc, char *argv[])
{
	// Check for command-line argument "console".
	// A deeper argument check will be done later within the class Direcs!
	if (argc > 1)
	{
		qDebug() << argc - 1 << "argument(s) passed...";

		for (int i=1; i<argc; i++)
		{
			// now search for the "console" parameter (case insensitive)
			if (strcasecmp(argv[i], "hello") == 0)
			{
				qDebug() << "hello world";
			}
		}
	}

	// Initialize the resource file
	Q_INIT_RESOURCE(application);

	// The QApplication class manages the GUI application's control flow and main settings.
	QApplication app(argc, argv);

	// create the kinect class object
	kinect k;

	return app.exec();
}


kinect::kinect()
{
	gui = new Gui();

	gui->show();

	// this is my test slot for whatever
	connect(gui, SIGNAL(test()), this, SLOT(kinectSlot()));

	// call this slot if the window is closed
	connect(gui, SIGNAL(shutdown()), this, SLOT(shutdown()));


	//---------------------------------------------------------------------------------------------------
	// kinect stuff
	//---------------------------------------------------------------------------------------------------
	m_kinect = QKinect::instance();


	// the signals for the LED actions
	connect(gui, SIGNAL(setLedOff()), m_kinect, SLOT(setLedOff()));
	connect(gui, SIGNAL(setRedLed()), m_kinect, SLOT(setRedLed()));
	connect(gui, SIGNAL(setGreenLed()), m_kinect, SLOT(setGreenLed()));
	connect(gui, SIGNAL(setYellowLed()), m_kinect, SLOT(setYellowLed()));
	connect(gui, SIGNAL(setRedLedFlash()), m_kinect, SLOT(setRedLedFlash()));
	connect(gui, SIGNAL(setGreenLedFlash()), m_kinect, SLOT(setGreenLedFlash()));
	connect(gui, SIGNAL(setYellowLedFlash()), m_kinect, SLOT(setYellowLedFlash()));

	// the signal for setting the camera angle
	connect(gui, SIGNAL(setAngle(double)), m_kinect, SLOT(setAngle(double)));

	// the signal for resetting the camera angle
	connect(gui, SIGNAL(resetAngle()), m_kinect, SLOT(resetAngle()));

	// the signal for setting the video mode
	connect(gui, SIGNAL(setVideoMode(int)), m_kinect, SLOT(setVideoMode(int)));
}


void kinect::shutdown()
{
	// shutdown kinect
	qDebug("Shutting down camera.");

	m_kinect->setLedOff();

	m_kinect->shutDownKinect();
}


kinect::~kinect()
{
	delete gui;
}


void kinect::kinectSlot()
{
	gui->appendLog("test");
}
