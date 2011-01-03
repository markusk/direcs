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


#ifndef KINECT_H
#define KINECT_H

#include <QMainWindow>
#include <QCloseEvent>
#include <QtDebug>
#include <QMutex>
#include <QMetaType> // for qRegisterMetaType

//---------------------------------------------------------------------------------------------------
// kinect stuff
//---------------------------------------------------------------------------------------------------
#include <QWidget>
#include <QGridLayout>
#include <QImage>
#include <QLabel>
#include <QTimer>
#include <QPixmap>
//---------------------------------------------------------------------------------------------------
#include "QKinect.h"
#include "RGBWindow.h"
//---------------------------------------------------------------------------------------------------


class QAction;
class QMenu;
class QTextEdit;

class kinect:public QMainWindow
{
	Q_OBJECT

public:
	kinect();
	~kinect();

protected:
	void closeEvent(QCloseEvent *event);

public slots:
	void appendLog(QString message);


private slots:
	void newFile();
	void open();
	bool save();
	bool saveAs();
	void about();
	void documentWasModified();
	void kinectSlot();      //    < < < <   this is the kinect slot where all magic happens

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
	QMenu *kinectMenu;
	QToolBar *fileToolBar;
	QToolBar *editToolBar;
	QToolBar *kinectToolBar;
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
	QAction *kinectAct;      //    < < < <   this is the kinect action for me and anyone who may need it ;-)  It calls the "kinectSlot" method.

	//---------------------------------------------------------------------------------------------------
	/// @brief the mid area our windows are placed into
	QMdiArea *m_mdiArea;
	/// @brief pointer to our kinect object not really used in this one
	QKinect *m_kinect;
	/// @brief our rgb drawing window
	RGBWindow *m_rgb;
	/// @brief our depth drawing window
	RGBWindow *m_depth;
};

#endif
