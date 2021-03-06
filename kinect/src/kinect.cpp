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
	Kinect k;

	return app.exec();
}


Kinect::Kinect()
{
	gui = new Gui();

	gui->show();

	// this is my test slot for whatever
	connect(gui, SIGNAL(test()), this, SLOT(kinectSlot()));

	// call this slot if the window is closed
	connect(gui, SIGNAL(shutdown()), this, SLOT(shutdown()));

	// here we go
	m_kinect = QKinect::instance();

	// Kinect detected?
	if (m_kinect->kinectDetected)
	{
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
	else
	{
		gui->appendLog("No Kinect detected.");
	}
}


void Kinect::shutdown()
{
	if (m_kinect->kinectDetected)
	{
		// shutdown kinect
		qDebug("Shutting down camera.");

		m_kinect->setLedOff();

		m_kinect->shutDownKinect();
	}
}


Kinect::~Kinect()
{
	delete gui;
}


void Kinect::kinectSlot()
{
	gui->appendLog("test");
}
