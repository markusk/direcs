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
#include "timerThread.h"
#include "sensorThread.h"

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
	  void about();
	  void testSlot();      //    < < < <   this is the test slot where all magic happens
	  /**
	   * This slot sets (stores) the robots (circuits) state within this class.
	   * This slot is also called, when the robot / Arduino was detected. This is event driven, caused by @sa initCircuit();
	   * @param state can be ON or OFF
	   */
	  void setRobotState(bool state);
	  /**
	  This slot catches all signals from the signal @sa systemerror
	  */
	  void systemerrorcatcher(int errorlevel);

signals:
	  /**
	  Emits a string to the GUI log / console.
	  @sa this::appendLog()
	  @param text is the message to be emitted
	  */
	  void message(QString text);

private:
	  void createActions();
	  void createMenus();
	  void createToolBars();
	  void createStatusBar();
	  void readSettings();
	  void writeSettings();

	  QTextEdit *textEdit;
	  QString curFile;

	  QMenu *fileMenu;
	  QMenu *editMenu;
	  QMenu *helpMenu;
	  QMenu *testMenu;
	  QToolBar *editToolBar;
	  QToolBar *testToolBar;
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
	  LaserThread *laserThread;
	  TimerThread *timerThread;
	  SensorThread *sensorThread;
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
