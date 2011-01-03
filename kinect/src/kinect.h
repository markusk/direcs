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
#include "gui.h"
//---------------------------------------------------------------------------------------------------


class QAction;
class QMenu;
class QTextEdit;

class kinect:public QObject
{
	Q_OBJECT


public:
	kinect();
	~kinect();


public slots:
	void shutdown();


private slots:
	void kinectSlot();      //    < < < <   this is the kinect slot where all magic happens


private:
	Gui *gui;

	/// @brief the mid area our windows are placed into
	QMdiArea *m_mdiArea;
	/// @brief pointer to our kinect object not really used in this one
	QKinect *m_kinect;
};

#endif
