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
	kinect k();

	return app.exec();
}


kinect::kinect()
{
	gui = new Gui();

	gui->show();

	createActions();


	//---------------------------------------------------------------------------------------------------
	// kinect stuff
	//---------------------------------------------------------------------------------------------------
	m_kinect=QKinect::instance();
}


kinect::~kinect()
{
	// shutdown kinect
	m_kinect->shutDownKinect();

	delete gui;
}


void kinect::createActions()
{
/*
	kinectAct = new QAction(QIcon(":/utilities-system-monitor-active.png"), tr("&Test"), this);
	kinectAct->setShortcut(tr("Ctrl+T"));
	kinectAct->setStatusTip(tr("Test"));
	connect(kinectAct, SIGNAL(triggered()), this, SLOT(kinectSlot()));
*/
}


void kinect::kinectSlot()
{
//	gui->appendLog("click");
}
