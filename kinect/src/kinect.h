/*************************************************************************
 *   Copyright (C) 2011 by Markus Knapp                                  *
 *   www.direcs.de                                                       *
 *                                                                       *
 *   This file is part of direcs.                                        *
 *                                                                       *
 *   direcs is free software: you can redistribute it and/or modify it   *
 *   under the terms of the GNU General Public License as published      *
 *   by the Free Software Foundation, version 3 of the License.          *
 *                                                                       *
 *   direcs is distributed in the hope that it will be useful,           *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of      *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the        *
 *   GNU General Public License for more details.                        *
 *                                                                       *
 *   You should have received a copy of the GNU General Public License   *
 *   along with direcs. If not, see <http://www.gnu.org/licenses/>.      *
 *                                                                       *
 *************************************************************************/


#ifndef KINECT_H
#define KINECT_H

#include <QMainWindow>
#include <QCloseEvent>
#include <QtDebug>
#include <QMutex>

//---------------------------------------------------------------------------------------------------
#include "QKinect.h"
#include "gui.h"
//---------------------------------------------------------------------------------------------------


class Kinect:public QObject
{
	Q_OBJECT


public:
	Kinect();
	~Kinect();


public slots:
	void shutdown();


private slots:
	void kinectSlot();


private:
	Gui *gui;

	/// @brief pointer to our kinect object not really used in this one
	QKinect *m_kinect;
};

#endif
