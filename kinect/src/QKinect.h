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

#ifndef _QKINECT_H
#define	_QKINECT_H
#include <QImage>
#include <QWidget>
#include <QList>
#include <QDebug>
#include "QKinectCaptureThread.h"
#include <libfreenect.h>

class QKinect : public QWidget{
public:
	QKinect();
	virtual ~QKinect();
	freenect_context *f_context;
	QList<freenect_device*> f_device_list;
	QList<QKinectCaptureThread*> thread_list;
	int num_devices;
	bool init;
	QImage getRGBQImageFromDevice(int device_number=0);
	freenect_pixel* getRGBRawFromDevice(int device_number=0);
	QImage getDepthQImageFromDevice(int device_number =0);
	freenect_depth* getDepthRawFromDevice(int device_number =0);
	int getNumberOfDevices();
private:
	bool initKinect();

};

#endif	/* _QKINECT_H */

