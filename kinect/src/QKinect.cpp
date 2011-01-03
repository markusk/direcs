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

#include "QKinect.h"
#include <libfreenect.h>

QKinect::QKinect() {
	num_devices = 0;
	if (initKinect() == false) {
		qDebug("Unable to initialise kinect device\n");
		init = false;
	} else {
		init = true;
	}
}

QKinect::~QKinect() {
	for (int loop = 0; loop < thread_list.size(); loop++) {
		thread_list.at(loop)->stopThread();
	}
}

bool QKinect::initKinect() {
	//Attempt to Initialise
	if (freenect_init(&f_context, NULL) < 0) {
		qDebug() << "Unable to init kinect\n";
		return false;
	}

	//Find the number of kinect devices attached to the computer
	num_devices = getNumberOfDevices();
	qDebug() << "Number of kinect devices: " << num_devices;
	//Open the found devices and put them into a list
	f_device_list.clear();
	thread_list.clear();
	for (int device_number = 0; device_number < num_devices; device_number++) {
		freenect_device *current_device;
		f_device_list.push_back(current_device);
		if (freenect_open_device(f_context, &current_device, device_number) < 0)qDebug() << "Couldn't open device: " << device_number << "\n";
		QKinectCaptureThread *current_thread = new QKinectCaptureThread(f_context, current_device);
		thread_list.push_back(current_thread);
	}
	for (int device_number = 0; device_number < thread_list.size(); device_number++) {
		qDebug() << "starting thread: " << device_number;
		thread_list.at(device_number)->start();
	}
	return true;
}

int QKinect::getNumberOfDevices() {
	return freenect_num_devices(f_context);
}

QImage QKinect::getRGBQImageFromDevice(int device_number) {
	QImage rgb_image = QImage(640, 480, QImage::Format_RGB32);

	if (device_number > (num_devices - 1)) {
		qDebug("You done goofed: kinect device number out of range");
		return rgb_image;
	}
	freenect_pixel* rgb = thread_list.at(device_number)->getRawRGB();
	//This could all probably be speeded up somewhat
	for (int y = 0; y < 480; y++) {
		for (int x = 0; x < 640; x++) {
			//rgb_image.fromData(rgb,FREENECT_RGB_SIZE,)
			QRgb pixel;
			pixel = qRgb(rgb[640 * 3 * y + x * 3 + 0], rgb[640 * 3 * y + x * 3 + 1], rgb[640 * 3 * y + x * 3 + 2]);
			rgb_image.setPixel(x, y, pixel);
		}
	}
	return rgb_image;
}

freenect_pixel* QKinect::getRGBRawFromDevice(int device_number) {
	if (device_number > (num_devices - 1)) {
		qDebug("You done goofed: kinect device number out of range");
		/*think of something sensible to return here, null probably isnt the
		 * best idea*/
		return NULL;
	}
	freenect_pixel* rgb = thread_list.at(device_number)->getRawRGB();
	return rgb;
}

QImage QKinect::getDepthQImageFromDevice(int device_number) {
	QImage depth_image = QImage(640, 480, QImage::Format_RGB32);

	if (device_number > (num_devices - 1)) {
		qDebug("You done goofed: kinect device number out of range");
		return depth_image;
	}
	freenect_depth* depth = thread_list.at(device_number)->getRawDepth();
	//This could all probably be speeded up somewhat
	for (int y = 0; y < 480; y++) {
		for (int x = 0; x < 640; x++) {
			//rgb_image.fromData(rgb,FREENECT_RGB_SIZE,)
			QRgb pixel;
			pixel = qRgb(depth[640 * y + x] >> 3, depth[640 * y + x] >> 3, depth[640 * y + x] >> 3);
			depth_image.setPixel(x, y, pixel);
		}
	}
	return depth_image;
}

freenect_depth* QKinect::getDepthRawFromDevice(int device_number) {
	if (device_number > (num_devices - 1)) {
		qDebug("You done goofed: kinect device number out of range");
		/*think of something sensible to return here, null probably isnt the
		 * best idea*/
		return NULL;
	}
	freenect_depth* depth = thread_list.at(device_number)->getRawDepth();
	return depth;
}
