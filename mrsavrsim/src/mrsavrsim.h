/***************************************************************************
 *   Copyright (C) 2008 by Markus Knapp   *
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


#ifndef MRSAVRSIM_H
#define MRSAVRSIM_H

#include <QMainWindow>
#include <QCloseEvent>
#include <QMutex>
#include "interfaceAvr.h"
#include "simThread.h"

class QAction;
class QMenu;
class QTextEdit;

class Mrsavrsim:public QMainWindow
{
      Q_OBJECT
	
	public:
		Mrsavrsim();
		~Mrsavrsim();
	
	protected:
		void closeEvent(QCloseEvent *event);
	
	private slots:
		void newFile();
		void simulateRobot();
		void open();
		bool save();
		bool saveAs();
		void about();
		void documentWasModified();
	
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
		QToolBar *fileToolBar;
		QToolBar *editToolBar;
		QAction *simBot;
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
		
		mutable QMutex *mutex; // make the threads thread-safe (e.g. simThread)
		InterfaceAvr *interface1;
		SimThread *simThread;
		
		QString serialPortMicrocontroller;
};

#endif
