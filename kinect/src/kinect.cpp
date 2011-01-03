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
#include "kinect.h"

#include <QTextEdit>
#include <QTextStream>
#include <QCloseEvent>
#include <QFileDialog>


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
	kinect * mw = new kinect();

	mw->show();

	return app.exec();
}


kinect::kinect()
{
	gui = new Gui();

	gui->show();

/*
	textEdit = new QTextEdit;
	setCentralWidget(textEdit);

	createActions();
	createMenus();
	createToolBars();
	createStatusBar();
	readSettings();
	// connect(textEdit->document(), SIGNAL(contentsChanged()), this, SLOT(documentWasModified()));
	setCurrentFile("");
*/

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

	kinectAct = new QAction(QIcon(":/utilities-system-monitor-active.png"), tr("&Test"), this);
	kinectAct->setShortcut(tr("Ctrl+T"));
	kinectAct->setStatusTip(tr("Test"));
	connect(kinectAct, SIGNAL(triggered()), this, SLOT(kinectSlot()));
}

void kinect::createMenus()
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

	kinectMenu = menuBar()->addMenu(tr("&Test"));
	kinectMenu->addAction(kinectAct);
}

void kinect::createToolBars()
{
	fileToolBar = addToolBar(tr("File"));
	fileToolBar->addAction(newAct);
	fileToolBar->addAction(openAct);
	fileToolBar->addAction(saveAct);

	editToolBar = addToolBar(tr("Edit"));
	editToolBar->addAction(cutAct);
	editToolBar->addAction(copyAct);
	editToolBar->addAction(pasteAct);

	kinectToolBar = addToolBar(tr("Test"));
	kinectToolBar->addAction(kinectAct);
}

void kinect::createStatusBar()
{
	statusBar()->showMessage(tr("Ready"));
}

void kinect::readSettings()
{
	QSettings settings("Trolltech", "Application Example");
	QPoint pos = settings.value("pos", QPoint(200, 200)).toPoint();
	QSize size = settings.value("size", QSize(400, 400)).toSize();
	resize(size);
	move(pos);
}

void kinect::writeSettings()
{
	QSettings settings("Trolltech", "Application Example");
	settings.setValue("pos", pos());
	settings.setValue("size", size());
}

bool kinect::maybeSave()
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

void kinect::loadFile(const QString &fileName)
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

bool kinect::saveFile(const QString &fileName)
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


void kinect::setCurrentFile(const QString &fileName)
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


QString kinect::strippedName(const QString &fullFileName)
{
	return QFileInfo(fullFileName).fileName();
}


void kinect::kinectSlot()
{
	textEdit->append("click");
}


void kinect::appendLog(QString message)
{
	textEdit->append(message);
}
