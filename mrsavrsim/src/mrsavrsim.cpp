/***************************************************************************
 *   Copyright (C) 2008/home/markus/develop/mrs/mrs/trunk/src by Markus Knapp   *
 *      *
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
#include <QTextEdit>
#include <QTextStream>
#include <QCloseEvent>
#include <QFileDialog>

#include "mrsavrsim.h"


Mrsavrsim::Mrsavrsim()
{
	textEdit = new QTextEdit;
	setCentralWidget(textEdit);
	
	mutex = new QMutex();
	interface1 = new InterfaceAvr();
	simThread = new SimThread(interface1, mutex);
	
	createActions();
	createMenus();
	createToolBars();
	createStatusBar();
	
	readSettings();
	
	connect(textEdit->document(), SIGNAL(contentsChanged()), this, SLOT(documentWasModified()));
	
	setCurrentFile("");


	serialPortMicrocontroller = "/dev/ttyLaserscannerRear";
	
	//-------------------------------------------------------
	// Open serial port for microcontroller communication
	//-------------------------------------------------------
	textEdit->append(QString("Opening serial port '%1' for microcontroller communication...").arg(serialPortMicrocontroller));
	
	if (interface1->openComPort(serialPortMicrocontroller) == false)
	{
		qDebug() << "Error opening serial port" << serialPortMicrocontroller;
		textEdit->append(QString("<font color=\"#FF0000\">Error opening serial port '%1'!</font>").arg(serialPortMicrocontroller));

		// show a warning dialog!
		//QMessageBox msgbox(QMessageBox::Warning, tr("Error with robots serial port"), tr("Error opening serial port %1").arg(serialPortMicrocontroller), QMessageBox::Ok | QMessageBox::Default);
		//msgbox.exec();
	}
	else
	{
		textEdit->append("Serial port opened.");
	}
}


void Mrsavrsim::closeEvent(QCloseEvent *event)
{
      if (maybeSave()) {
            writeSettings();
            event->accept();
      } else {
            event->ignore();
      }
}


void Mrsavrsim::simulateRobot()
{
	static bool toggle = false;
	
	
	toggle = !toggle;
	setCurrentFile("");
	
	if (toggle)
	{
		//-------------------------
		// start the sim thread
		//-------------------------
		if (simThread->isRunning() == false)
		{
			textEdit->append("Starting robot simulation...");
			simThread->start();
			textEdit->append("Robot simulation succesfuly startet.");
		}
	}
	else
	{
		//--------------------------
		// quit the sim thread
		//--------------------------
		//qDebug("Starting to stop the sim thread NOW!");
		if (simThread->isRunning() == true)
		{
			textEdit->append("Stopping simulation thread...");

			// my own stop routine :-)
			simThread->stop();

			// slowing thread down
			simThread->setPriority(QThread::IdlePriority);
			simThread->quit();

			//-------------------------------------------
			// start measuring time for timeout ckecking
			//-------------------------------------------
			QTime t;
			t.start();
			do
			{
			} while ((simThread->isFinished() == false) && (t.elapsed() <= 2000));

			if (simThread->isFinished() == true)
			{
				textEdit->append("Robot simulation stopped.");
			}
			else
			{
				textEdit->append("Terminating simulation thread because it doesn't answer...");
				simThread->terminate();
				simThread->wait(1000);
				textEdit->append("Robot simulation terminated.");
			}
		}
	}
}


void Mrsavrsim::newFile()
{
	if (maybeSave()) {
		textEdit->clear();
		setCurrentFile("");
	}
}


void Mrsavrsim::open()
{
      if (maybeSave()) {
            QString fileName = QFileDialog::getOpenFileName(this);
            if (!fileName.isEmpty())
            loadFile(fileName);
      }
}


bool Mrsavrsim::save()
{
      if (curFile.isEmpty()) {
            return saveAs();
      } else {
            return saveFile(curFile);
      }
}


bool Mrsavrsim::saveAs()
{
      QString fileName = QFileDialog::getSaveFileName(this);
      if (fileName.isEmpty())
            return false;

      return saveFile(fileName);
}


void Mrsavrsim::about()
{
      QMessageBox::about(this, tr("About Application"),
            tr("The <b>Application</b> example demonstrates how to "
                  "write modern GUI applications using Qt, with a menu bar, "
                  "toolbars, and a status bar."));
}


void Mrsavrsim::documentWasModified()
{
      setWindowModified(true);
}


void Mrsavrsim::createActions()
{
	simBot = new QAction(QIcon(":/underFootOne.png"), tr("Simulate &Robot (Ctrl+R)"), this);
	simBot->setShortcut(tr("Ctrl+R"));
	simBot->setStatusTip(tr("Start the robot simulation mode (Ctrl+R)"));
	connect(simBot, SIGNAL(triggered()), this, SLOT(simulateRobot()));

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
	exitAct->setShortcut(tr("Alt+X"));
	exitAct->setStatusTip(tr("Exit the application"));
	connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));
	
	cutAct = new QAction(QIcon(":/editcut.xpm"), tr("Cu&t"), this);
	cutAct->setShortcut(tr("Ctrl+X"));
	cutAct->setStatusTip(tr("Cut the current selection's contents to the clipboard"));
	connect(cutAct, SIGNAL(triggered()), textEdit, SLOT(cut()));
	
	copyAct = new QAction(QIcon(":/editcopy.xpm"), tr("&Copy"), this);
	copyAct->setShortcut(tr("Ctrl+C"));
	copyAct->setStatusTip(tr("Copy the current selection's contents to the clipboard"));
	connect(copyAct, SIGNAL(triggered()), textEdit, SLOT(copy()));
	
	pasteAct = new QAction(QIcon(":/editpaste.xpm"), tr("&Paste"), this);
	pasteAct->setShortcut(tr("Ctrl+V"));
	pasteAct->setStatusTip(tr("Paste the clipboard's contents into the current selection"));
	connect(pasteAct, SIGNAL(triggered()), textEdit, SLOT(paste()));
	
	aboutAct = new QAction(tr("&About"), this);
	aboutAct->setStatusTip(tr("Show the application's About box"));
	connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));
	
	aboutQtAct = new QAction(tr("About &Qt"), this);
	aboutQtAct->setStatusTip(tr("Show the Qt library's About box"));
	connect(aboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
	
	cutAct->setEnabled(false);
	copyAct->setEnabled(false);
	connect(textEdit, SIGNAL(copyAvailable(bool)), cutAct, SLOT(setEnabled(bool)));
	connect(textEdit, SIGNAL(copyAvailable(bool)), copyAct, SLOT(setEnabled(bool)));
}


void Mrsavrsim::createMenus()
{
      fileMenu = menuBar()->addMenu(tr("&File"));
      fileMenu->addAction(simBot);
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
}


void Mrsavrsim::createToolBars()
{
      fileToolBar = addToolBar(tr("File"));
      fileToolBar->addAction(simBot);
      fileToolBar->addAction(newAct);
      fileToolBar->addAction(openAct);
      fileToolBar->addAction(saveAct);

      editToolBar = addToolBar(tr("Edit"));
      editToolBar->addAction(cutAct);
      editToolBar->addAction(copyAct);
      editToolBar->addAction(pasteAct);
}


void Mrsavrsim::createStatusBar()
{
      statusBar()->showMessage(tr("Ready"));
}


void Mrsavrsim::readSettings()
{
	QSettings settings("direcs", "mrsavrsim");
	QPoint pos = settings.value("pos", QPoint(200, 200)).toPoint();
	QSize size = settings.value("size", QSize(400, 400)).toSize();
	resize(size);
	move(pos);
}


void Mrsavrsim::writeSettings()
{
	QSettings settings("direcs", "mrsavrsim");
	settings.setValue("pos", pos());
	settings.setValue("size", size());
}


bool Mrsavrsim::maybeSave()
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


void Mrsavrsim::loadFile(const QString &fileName)
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


bool Mrsavrsim::saveFile(const QString &fileName)
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


void Mrsavrsim::setCurrentFile(const QString &fileName)
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


QString Mrsavrsim::strippedName(const QString &fullFileName)
{
      return QFileInfo(fullFileName).fileName();
}


Mrsavrsim::~Mrsavrsim()
{
	//-----------------------------
	// close serial port
	//-----------------------------
	textEdit->append("Closing serial port...");
	interface1->closeComPort();
	qDebug("Bye.");
	
	delete simThread;
	delete interface1;
}
