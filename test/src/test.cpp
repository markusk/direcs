/***************************************************************************
 *   Copyright (C) Markus Knapp                                            *
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


#include <QtGui>
#include "test.h"

#include <QTextEdit>
#include <QTextStream>
#include <QCloseEvent>
#include <QFileDialog>

test::test()
{
	  textEdit = new QTextEdit;
	  setCentralWidget(textEdit);

	  createActions();
	  createMenus();
	  createToolBars();
	  createStatusBar();

	  readSettings();


/*
	  //---------------------------------------------------------------------------------------------------
	  // test qthread and qlist stuff (queueing Signals for Slots)

	  laserThread = new LaserThread();

	  qRegisterMetaType < QList <float> > ("QList <float>");
	  qRegisterMetaType < QList <int> > ("QList <int>");

	  connect(laserThread, SIGNAL( laserDataCompleteFront(QList <float>, QList <int>) ), this, SLOT( refreshLaserViewFront(QList <float>, QList <int>) ));
	  //---------------------------------------------------------------------------------------------------
*/


/*
	  //---------------------------------------------------------------------------------------------------
	  // SICK laser S300 test stuff

	  // sickS300 = new SickS300();

	  // send messages from the other class to this class (to the GUI)
	  // connect(sickS300, SIGNAL( message(QString) ), this, SLOT( appendLog(QString) ));

	  // "/dev/tty.USA19Hfa141P1.1" // Keyspan
	  // "/dev/tty.PL2303-000013FD" // Prolific
	  // "/dev/ttyLaserScannerFront"

	  // sickS300->setDevicePort("/dev/ttyLaserScannerFront");


	  // create laser thread
	  laserThread = new LaserThread();

	  // send messages from the other class to this class (to the GUI)
	  connect(laserThread, SIGNAL( message(QString) ), this, SLOT( appendLog(QString) ));

	  laserThread->setType(LASER1, "S300"); // this also creates the S300 object within the laser thread
	  laserThread->setAngle(LASER1, 270);
	  laserThread->setMounting(LASER1, "normal");
	  laserThread->setResolution(LASER1, 0.5);
	  laserThread->setSerialPort(LASER1, "/dev/ttyLaserScannerFront");

	  textEdit->append("Starting laser thread...");

	  // start laser thread
	  laserThread->start();

	  textEdit->append("Looking for S300...");

	  // init the laser
	  if (laserThread->isConnected(LASER1))
	  {
		  // * The laser is ON *
		  appendLog("OKAY");
	  }
	  else
	  {
		  appendLog("ERROR");
	  }

	  //---------------------------------------------------------------------------------------------------
*/


	  //---------------------------------------------------------------------------------------------------
	  // AtmelBoard test stuff

	  mutex = new QMutex();
	  interface1 = new InterfaceAvr();
	  circuit1 = new Circuit(interface1, mutex);

	 // serialPortPath = "/dev/tty.SLAB_USBtoUART"; // Original driver "CP210x Macintosh OSX Driver v2." from SiLabs used.
	  serialPortPath = "/dev/tty.usbserial-A900J1TU"; // ARM board with STM32F4 and FTDI RS232R chip

	  // send messages from the other class to this class (to the GUI)
	  connect(interface1, SIGNAL( message(QString) ), this, SLOT( appendLog(QString) ));

	  //--------------------------------------------------------------------------
	  // let some classes know the robots state
	  //--------------------------------------------------------------------------
	  // this is needed, when the first openCOMPort method fails:
	  connect(interface1,	SIGNAL( robotState(bool) ), circuit1,		SLOT( setRobotState(bool) ));


	  textEdit->append("Opening serial port for microcontroller communication...");

	  if (interface1->openComPort(serialPortPath) == false)
	  {
		  // ********************
		  // * The robot is OFF *
		  // ********************
		  textEdit->append(QString("Error opening serial port '%1'").arg(serialPortPath));

	  }
	  else
	  {
		  // *******************
		  // * The robot is ON *
		  // *******************
		  textEdit->append("Serial port opened.");


		  //==========================
		  // init the robots circuit
		  //==========================
		  textEdit->append("Searching robot...");

		  if (circuit1->initCircuit() == true)
		  {
			  textEdit->append("Robot is <font color=\"#00FF00\">ON</font> and answers.");
		  }
	  }
	  //---------------------------------------------------------------------------------------------------

}


test::~test()
{
	// AtmelBoard test stuff
	delete circuit1;
	delete interface1;
	delete mutex;

	/*
	// stop the thread
	if (laserThread->isRunning())
	{
		laserThread->stop();
	}
	delete laserThread;

	// SICK laser S300 test stuff
	delete sickS300;
	*/
}


void test::closeEvent(QCloseEvent *event)
{
	Q_UNUSED(event);
}


void test::about()
{
	  QMessageBox::about(this, tr("About Application"),
			tr("The <b>Application</b> example demonstrates how to "
				  "write modern GUI applications using Qt, with a menu bar, "
				  "toolbars, and a status bar."));
}


void test::createActions()
{
	  exitAct = new QAction(tr("E&xit"), this);
	  exitAct->setShortcut(tr("Ctrl+Q"));
	  exitAct->setStatusTip(tr("Exit the application"));
	  connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

	  cutAct = new QAction(QIcon(":/editcut.xpm"), tr("Cu&t"), this);
	  cutAct->setShortcut(tr("Ctrl+X"));
	  cutAct->setStatusTip(tr("Cut the current selection's contents to the "
							  "clipboard"));
	  connect(cutAct, SIGNAL(triggered()), textEdit, SLOT(cut()));

	  copyAct = new QAction(QIcon(":/editcopy.xpm"), tr("&Copy"), this);
	  copyAct->setShortcut(tr("Ctrl+C"));
	  copyAct->setStatusTip(tr("Copy the current selection's contents to the "
							  "clipboard"));
	  connect(copyAct, SIGNAL(triggered()), textEdit, SLOT(copy()));

	  pasteAct = new QAction(QIcon(":/editpaste.xpm"), tr("&Paste"), this);
	  pasteAct->setShortcut(tr("Ctrl+V"));
	  pasteAct->setStatusTip(tr("Paste the clipboard's contents into the current "
							  "selection"));
	  connect(pasteAct, SIGNAL(triggered()), textEdit, SLOT(paste()));

	  aboutAct = new QAction(tr("&About"), this);
	  aboutAct->setStatusTip(tr("Show the application's About box"));
	  connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

	  aboutQtAct = new QAction(tr("About &Qt"), this);
	  aboutQtAct->setStatusTip(tr("Show the Qt library's About box"));
	  connect(aboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

	  cutAct->setEnabled(false);
	  copyAct->setEnabled(false);
	  connect(textEdit, SIGNAL(copyAvailable(bool)),
			cutAct, SLOT(setEnabled(bool)));
	  connect(textEdit, SIGNAL(copyAvailable(bool)),
			copyAct, SLOT(setEnabled(bool)));

	  testAct = new QAction(QIcon(":/utilities-system-monitor-active.png"), tr("&Test"), this);
	  testAct->setShortcut(tr("Ctrl+T"));
	  testAct->setStatusTip(tr("Test"));
	  connect(testAct, SIGNAL(triggered()), this, SLOT(testSlot()));
}

void test::createMenus()
{
	  fileMenu = menuBar()->addMenu(tr("&File"));
	  fileMenu->addAction(exitAct);

	  editMenu = menuBar()->addMenu(tr("&Edit"));
	  editMenu->addAction(cutAct);
	  editMenu->addAction(copyAct);
	  editMenu->addAction(pasteAct);

	  menuBar()->addSeparator();

	  helpMenu = menuBar()->addMenu(tr("&Help"));
	  helpMenu->addAction(aboutAct);
	  helpMenu->addAction(aboutQtAct);

	  menuBar()->addSeparator();

	  testMenu = menuBar()->addMenu(tr("&Test"));
	  testMenu->addAction(testAct);
}


void test::createToolBars()
{
	  editToolBar = addToolBar(tr("Edit"));
	  editToolBar->addAction(cutAct);
	  editToolBar->addAction(copyAct);
	  editToolBar->addAction(pasteAct);

	  testToolBar = addToolBar(tr("Test"));
	  testToolBar->addAction(testAct);
}


void test::createStatusBar()
{
	  statusBar()->showMessage(tr("Ready"));
}


void test::readSettings()
{
	  QSettings settings("Trolltech", "Application Example");
	  QPoint pos = settings.value("pos", QPoint(200, 200)).toPoint();
	  QSize size = settings.value("size", QSize(400, 400)).toSize();
	  resize(size);
	  move(pos);
}


void test::writeSettings()
{
	  QSettings settings("Trolltech", "Application Example");
	  settings.setValue("pos", pos());
	  settings.setValue("size", size());
}


void test::appendLog(QString message)
{
	textEdit->append(message);
}


void test::testSlot()
{
	QString string;


	// clear content
	// textEdit->clear();

	/*
	// send re
	interface1->sendString("re");
	// debug msg
	textEdit->append( QString("%1: %2 sent").arg(QDateTime::currentDateTime().toString() ).arg(string) );

	// receive answer
	interface1->receiveString(string);
	// debug msg
	textEdit->append( QString("%1: %2 received.").arg(QDateTime::currentDateTime().toString() ).arg(string) );
*/



	// send s7
	interface1->sendString("s7");
	// debug msg
	textEdit->append( QString("%1: %2 sent").arg(QDateTime::currentDateTime().toString() ).arg(string) );

	// receive answer
	interface1->receiveString(string);
	// debug msg
	textEdit->append( QString("%1: %2 received.").arg(QDateTime::currentDateTime().toString() ).arg(string) );



	/*
	if (sickS300->readRequestTelegram() == -1)
	{
		appendLog("scan ERROR");
	}
	else
	{

		for (int angle=0; angle < 270*2; angle++)
		{
			textEdit->append( QString("%1: %2m | %3: %4m | %5: %6m | %7: %8m | %9: %10m | %11: %12m | %13: %14m | %15: %16m | %17: %18m")
														 .arg(angle,     3, 10, QChar('0')).arg( sickS300->getDistance(angle    ), 4, 'f', 2 )
														 .arg(angle + 1, 3, 10, QChar('0')).arg( sickS300->getDistance(angle + 1), 4, 'f', 2 )
														 .arg(angle + 2, 3, 10, QChar('0')).arg( sickS300->getDistance(angle + 2), 4, 'f', 2 )
														 .arg(angle + 3, 3, 10, QChar('0')).arg( sickS300->getDistance(angle + 3), 4, 'f', 2 )
														 .arg(angle + 4, 3, 10, QChar('0')).arg( sickS300->getDistance(angle + 4), 4, 'f', 2 )
														 .arg(angle + 5, 3, 10, QChar('0')).arg( sickS300->getDistance(angle + 5), 4, 'f', 2 )
														 .arg(angle + 6, 3, 10, QChar('0')).arg( sickS300->getDistance(angle + 6), 4, 'f', 2 )
														 .arg(angle + 7, 3, 10, QChar('0')).arg( sickS300->getDistance(angle + 7), 4, 'f', 2 )
														 .arg(angle + 8, 3, 10, QChar('0')).arg( sickS300->getDistance(angle + 8), 4, 'f', 2 )
														 );
			angle = angle + 8;

			if (angle>= 270*2)
				break;
		}
	}
*/
/*
	//
	// using laser thread values !
	//
	for (int angle=0; angle < 270*2; angle++)
	{
		textEdit->append( QString("%1: %2m | %3: %4m | %5: %6m | %7: %8m | %9: %10m | %11: %12m | %13: %14m | %15: %16m | %17: %18m")
													 .arg(angle,     3, 10, QChar('0')).arg( laserThread->getValue(LASER1, angle    ), 4, 'f', 2 )
													 .arg(angle + 1, 3, 10, QChar('0')).arg( laserThread->getValue(LASER1, angle + 1), 4, 'f', 2 )
													 .arg(angle + 2, 3, 10, QChar('0')).arg( laserThread->getValue(LASER1, angle + 2), 4, 'f', 2 )
													 .arg(angle + 3, 3, 10, QChar('0')).arg( laserThread->getValue(LASER1, angle + 3), 4, 'f', 2 )
													 .arg(angle + 4, 3, 10, QChar('0')).arg( laserThread->getValue(LASER1, angle + 4), 4, 'f', 2 )
													 .arg(angle + 5, 3, 10, QChar('0')).arg( laserThread->getValue(LASER1, angle + 5), 4, 'f', 2 )
													 .arg(angle + 6, 3, 10, QChar('0')).arg( laserThread->getValue(LASER1, angle + 6), 4, 'f', 2 )
													 .arg(angle + 7, 3, 10, QChar('0')).arg( laserThread->getValue(LASER1, angle + 7), 4, 'f', 2 )
													 .arg(angle + 8, 3, 10, QChar('0')).arg( laserThread->getValue(LASER1, angle + 8), 4, 'f', 2 )
													 );
		angle = angle + 8;

		if (angle>= 270*2)
			break;
	}
*/
}


void test::refreshLaserViewFront(QList <float> laserScannerValues, QList <int> laserScannerFlags)
{
//	appendLog(QString("laserScannerValues size: %1").arg(laserScannerValues.size()));
//	appendLog(QString("laserScannerFlags  size: %1").arg(laserScannerFlags.size()));
	Q_UNUSED(laserScannerFlags);

	for (int i=0; i<laserScannerValues.size(); i++)
	{
		appendLog(QString("laserScannerValue no. %1 = size: %2").arg(i).arg(laserScannerValues[i]));
	}

	appendLog("-----------------------------------------------");
}
