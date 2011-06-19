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


#ifndef TEST_H
#define TEST_H

#include <QMainWindow>
#include <QCloseEvent>
#include <QtDebug>
#include <QMutex>
#include <QMetaType> // for qRegisterMetaType

//---------------------------------------------------------------------------------------------------

#include "circuit.h"
#include "interfaceAvr.h"
#include "laserSickS300.h"
#include "laserThread.h"

//---------------------------------------------------------------------------------------------------


class QAction;
class QMenu;
class QTextEdit;

class test:public QMainWindow
{
	  Q_OBJECT

public:
	  test();
	  ~test();

protected:
	  void closeEvent(QCloseEvent *event);

public slots:
	  void appendLog(QString message);
	  void refreshLaserViewFront(QList <float> laserScannerValues, QList <int> laserScannerFlags);


private slots:
	  void newFile();
	  void open();
	  bool save();
	  bool saveAs();
	  void about();
	  void documentWasModified();
	  void testSlot();      //    < < < <   this is the test slot where all magic happens

private:
	  void createActions();
	  void createMenus();
	  void createToolBars();
	  void createStatusBar();
	  void readSettings();
	  void writeSettings();
	  bool maybeSave();
	  void loadFile(const QString &fileName);
	  bool saveFile(const QString &fileName);
	  void setCurrentFile(const QString &fileName);
	  QString strippedName(const QString &fullFileName);

	  QTextEdit *textEdit;
	  QString curFile;

	  QMenu *fileMenu;
	  QMenu *editMenu;
	  QMenu *helpMenu;
	  QMenu *testMenu;
	  QToolBar *fileToolBar;
	  QToolBar *editToolBar;
	  QToolBar *testToolBar;
	  QAction *newAct;
	  QAction *openAct;
	  QAction *saveAct;
	  QAction *saveAsAct;
	  QAction *exitAct;
	  QAction *cutAct;
	  QAction *copyAct;
	  QAction *pasteAct;
	  QAction *aboutAct;
	  QAction *aboutQtAct;
	  QAction *testAct;      //    < < < <   this is the test action for me and anyone who may need it ;-)  It calls the "testSlot" method.

	  // QList <float> laserScannerValuesFront;  /// The measured distances from the front laser scanner.
	  QList <float> pointerFloatList;
	  QList <int *> pointerIntList;

	  mutable QMutex *mutex; // make the threads thread-safe (e.g. senorThread, servo...)
	  Circuit *circuit1;
	  InterfaceAvr *interface1;
//	  SickS300 *sickS300;
//	  LaserThread *laserThread;
	  QString serialPortPath; // something like /dev/ttyUSB0

	  /**
	  For the integrated DIRECS laser module
	  */
	  static const short int LASER1 = 1;
	  static const short int LASER2 = 2;
	  static const short int LASER3 = 4;
	  static const short int LASER4 = 8;
	  static const short int LASER5 = 16;
};

#endif
