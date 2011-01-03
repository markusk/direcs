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

	  // connect(textEdit->document(), SIGNAL(contentsChanged()), this, SLOT(documentWasModified()));

	  setCurrentFile("");


/*
	  //---------------------------------------------------------------------------------------------------
	  // test qthread and qlist stuff (queueing Signals for Slots)

	  laserThread = new LaserThread();

	  qRegisterMetaType < QList <float> > ("QList <float>");
	  qRegisterMetaType < QList <int> > ("QList <int>");

	  connect(laserThread, SIGNAL( laserDataCompleteFront(QList <float>, QList <int>) ), this, SLOT( refreshLaserViewFront(QList <float>, QList <int>) ));
	  //---------------------------------------------------------------------------------------------------
*/



	  //---------------------------------------------------------------------------------------------------
	  // SICK laser S300 test stuff

	  sickS300 = new SickS300();

	  // send messages from the other class to this class (to the GUI)
	  connect(sickS300, SIGNAL( message(QString) ), this, SLOT( appendLog(QString) ));

	  // "/dev/tty.USA19Hfa141P1.1" // Keyspan
	  // "/dev/tty.PL2303-000013FD" // Prolific
	  // /dev/ttyLaserScannerFront

	  textEdit->append("Opening serial port for S300...");

	  sickS300->setDevicePort("/dev/ttyLaserScannerFront");

	  if (sickS300->openComPort() == false)
	  {
		  textEdit->append(QString("ERROR opening serial port %1").arg(serialPortPath));
	  }
	  else
	  {
		  // * The laser is ON *
		  appendLog("OKAY");

		  // init the laser
		  textEdit->append("S300 setup...");

		  sickS300->setup();

	  }

	  //---------------------------------------------------------------------------------------------------


	  /*
	  //---------------------------------------------------------------------------------------------------
	  // AtmelBoard test stuff

	  mutex = new QMutex();
	  interface1 = new InterfaceAvr();
	  circuit1 = new Circuit(interface1, mutex);

	  serialPortPath = "/dev/tty.SLAB_USBtoUART"; // Original driver "CP210x Macintosh OSX Driver v2." from SiLabs used.

	  // send messages from the other class to this class (to the GUI)
	  connect(interface1, SIGNAL( emitMessage(QString) ), this, SLOT( appendLog(QString) ));

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
	  */
}


test::~test()
{
	/*
	// AtmelBoard test stuff
	delete circuit1;
	delete interface1;
	delete mutex;
	*/

	// SICK laser S300 test stuff
	delete sickS300;
/*
	// stop the thread
	if (laserThread->isRunning())
	{
		laserThread->stop();
	}
	delete laserThread;
*/
}


void test::closeEvent(QCloseEvent *event)
{
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

void test::newFile()
{
	  if (maybeSave()) {
			textEdit->clear();
			setCurrentFile("");
	  }
}

void test::open()
{
	  if (maybeSave()) {
			QString fileName = QFileDialog::getOpenFileName(this);
			if (!fileName.isEmpty())
			loadFile(fileName);
	  }
}

bool test::save()
{
	  if (curFile.isEmpty()) {
			return saveAs();
	  } else {
			return saveFile(curFile);
	  }
}

bool test::saveAs()
{
	  QString fileName = QFileDialog::getSaveFileName(this);
	  if (fileName.isEmpty())
			return false;

	  return saveFile(fileName);
}

void test::about()
{
	  QMessageBox::about(this, tr("About Application"),
			tr("The <b>Application</b> example demonstrates how to "
				  "write modern GUI applications using Qt, with a menu bar, "
				  "toolbars, and a status bar."));
}

void test::documentWasModified()
{
	  setWindowModified(true);
}

void test::createActions()
{
	  newAct = new QAction(QIcon(":/filenew.xpm"), tr("&New"), this);
	  newAct->setShortcut(tr("Ctrl+N"));
	  newAct->setStatusTip(tr("Create a new file"));
	  connect(newAct, SIGNAL(triggered()), this, SLOT(newFile()));

	  openAct = new QAction(QIcon(":/fileopen.xpm"), tr("&Open..."), this);
	  openAct->setShortcut(tr("Ctrl+O"));
	  openAct->setStatusTip(tr("Open an existing file"));
	  connect(openAct, SIGNAL(triggered()), this, SLOT(open()));

	  saveAct = new QAction(QIcon(":/filesave.xpm"), tr("&Save"), this);
	  saveAct->setShortcut(tr("Ctrl+S"));
	  saveAct->setStatusTip(tr("Save the document to disk"));
	  connect(saveAct, SIGNAL(triggered()), this, SLOT(save()));

	  saveAsAct = new QAction(tr("Save &As..."), this);
	  saveAsAct->setStatusTip(tr("Save the document under a new name"));
	  connect(saveAsAct, SIGNAL(triggered()), this, SLOT(saveAs()));

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
	  fileMenu->addAction(newAct);
	  fileMenu->addAction(openAct);
	  fileMenu->addAction(saveAct);
	  fileMenu->addAction(saveAsAct);
	  fileMenu->addSeparator();
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
	  fileToolBar = addToolBar(tr("File"));
	  fileToolBar->addAction(newAct);
	  fileToolBar->addAction(openAct);
	  fileToolBar->addAction(saveAct);

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

bool test::maybeSave()
{
	  if (textEdit->document()->isModified()) {
			int ret = QMessageBox::warning(this, tr("Application"),
						tr("The document has been modified.\n"
						"Do you want to save your changes?"),
						QMessageBox::Yes | QMessageBox::Default,
						QMessageBox::No,
						QMessageBox::Cancel | QMessageBox::Escape);
			if (ret == QMessageBox::Yes)
			return save();
			else if (ret == QMessageBox::Cancel)
			return false;
	  }
	  return true;
}

void test::loadFile(const QString &fileName)
{
	  QFile file(fileName);
	  if (!file.open(QFile::ReadOnly | QFile::Text)) {
			QMessageBox::warning(this, tr("Application"),
							  tr("Cannot read file %1:\n%2.")
							  .arg(fileName)
							  .arg(file.errorString()));
			return;
	  }

	  QTextStream in(&file);
	  QApplication::setOverrideCursor(Qt::WaitCursor);
	  textEdit->setPlainText(in.readAll());
	  QApplication::restoreOverrideCursor();

	  setCurrentFile(fileName);
	  statusBar()->showMessage(tr("File loaded"), 2000);
}

bool test::saveFile(const QString &fileName)
{
	  QFile file(fileName);
	  if (!file.open(QFile::WriteOnly | QFile::Text)) {
			QMessageBox::warning(this, tr("Application"),
							  tr("Cannot write file %1:\n%2.")
							  .arg(fileName)
							  .arg(file.errorString()));
			return false;
	  }

	  QTextStream out(&file);
	  QApplication::setOverrideCursor(Qt::WaitCursor);
	  out << textEdit->toPlainText();
	  QApplication::restoreOverrideCursor();

	  setCurrentFile(fileName);
	  statusBar()->showMessage(tr("File saved"), 2000);
	  return true;
}


void test::setCurrentFile(const QString &fileName)
{
	  curFile = fileName;
	  textEdit->document()->setModified(false);
	  setWindowModified(false);

	  QString shownName;
	  if (curFile.isEmpty())
			shownName = "untitled.txt";
	  else
			shownName = strippedName(curFile);

	  setWindowTitle(tr("%1[*] - %2").arg(shownName).arg(tr("Application")));
}


QString test::strippedName(const QString &fullFileName)
{
	  return QFileInfo(fullFileName).fileName();
}


void test::testSlot()
{
	textEdit->append("click");
/*
	if (sickS300->readUnknownTelegram() == -1)
	{
		appendLog("unknown telegram ERROR");
	}
	else
	{
		appendLog("unknown data OKAY");
	}
*/
	if (sickS300->readRequestTelegram() == -1)
	{
		appendLog("scan ERROR");
	}
	else
	{
		appendLog("scan OKAY");
	}


	/*
	// start the thread and let it emit some values
	if (laserThread->isRunning() == false)
	{
		textEdit->append("thread started");
		laserThread->start();
	}
	else
	{
		laserThread->stop();
		textEdit->append("thread stopped");
	}
	*/
}


void test::appendLog(QString message)
{
	textEdit->append(message);
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