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

	/*
	// create an MDI area for the two windows
	m_mdiArea = new QMdiArea;
	m_mdiArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	m_mdiArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

	setCentralWidget(m_mdiArea);
	this->setWindowTitle(QString("Kinect Demo"));

	// create a window for the RGB data (mode 0 = RGB draw)
	m_rgb= new RGBWindow(this);
	m_rgb->setMode(0);
	QMdiSubWindow *subWindow1 = new QMdiSubWindow;
	subWindow1->setWidget(m_rgb); // < < < < < < < < < < < <
	subWindow1->setAttribute(Qt::WA_DeleteOnClose);
	subWindow1->setWindowTitle("RGB Output");
	subWindow1->resize(640,480);
	m_mdiArea->addSubWindow(subWindow1);
	subWindow1->show();

	/// create a window for our depth draw (1 = depth)
	m_depth= new RGBWindow(this);
	m_depth->setMode(1);
	QMdiSubWindow *subWindow2 = new QMdiSubWindow;
	subWindow2->setWidget(m_depth);
	subWindow2->setAttribute(Qt::WA_DeleteOnClose);
	subWindow2->setWindowTitle("RGB Output");
	subWindow2->resize(640,480);
	m_mdiArea->addSubWindow(subWindow2);
	subWindow2->show();
	m_mdiArea->tileSubWindows();
	*/
}


kinect::~kinect()
{
	delete gui;
}


void kinect::closeEvent(QCloseEvent *event)
{
	// shutdown kinect
	m_kinect->shutDownKinect();

	Q_UNUSED(event);

	/*
	if (maybeSave()) {
			writeSettings();
			event->accept();
	} else {
			event->ignore();
	}
	*/
}

void kinect::newFile()
{
	if (maybeSave()) {
			textEdit->clear();
			setCurrentFile("");
	}
}

void kinect::open()
{
	if (maybeSave()) {
			QString fileName = QFileDialog::getOpenFileName(this);
			if (!fileName.isEmpty())
			loadFile(fileName);
	}
}

bool kinect::save()
{
	if (curFile.isEmpty()) {
			return saveAs();
	} else {
			return saveFile(curFile);
	}
}

bool kinect::saveAs()
{
	QString fileName = QFileDialog::getSaveFileName(this);
	if (fileName.isEmpty())
			return false;

	return saveFile(fileName);
}

void kinect::about()
{
	QMessageBox::about(this, tr("About Application"),
			tr("The <b>Application</b> example demonstrates how to "
				"write modern GUI applications using Qt, with a menu bar, "
				"toolbars, and a status bar."));
}

void kinect::documentWasModified()
{
	setWindowModified(true);
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
