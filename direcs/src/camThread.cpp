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

#include "camThread.h"


CamThread::CamThread() : QThread()
{
	initDone = false;
	stopped = false;

	cameraIsOn = false;
	faceDetectionIsEnabled = false;
	faceDetectionWasActive = false;
	haarClassifierCascadeFilename = "none";
}


CamThread::~CamThread()
{
	stopped = true;
}


void CamThread::stop()
{
	stopped = true;
}


void CamThread::run()
{

	if (initDone==false)
	{
		init();
	}

	// create an empty image with OpenCV
	Mat rgbMat(Size(640, 480), CV_8UC3, Scalar(0));

	// show empty image with OpenCV
	// namedWindow("video");

	//  start "threading"...
	while (!stopped)
	{
//		if (cameraIsOn == true)
//		{

		// get RGB picture
		freenect_sync_get_video((void**)&data, &timestamp, 0, FREENECT_VIDEO_RGB);

		// convert image
		rgbMat.data = (uchar*) data;
		// cvtColor(rgbMat, rgbMat, CV_RGB2BGR); // only when shown in OpenCV window!

		// refresh image with OpenCV
		// imshow("video", rgbMat);

		// convert to QImage
		QImage tmp( (uchar*)rgbMat.data, rgbMat.cols, rgbMat.rows, rgbMat.step, QImage::Format_RGB888 );

		// send RGB image to GUI
		emit camDataComplete(&tmp);

		// send DEPTH image to GUI
	//	emit camDepthComplete(&tmp);

		// let the thread sleep some time
		msleep(THREADSLEEPTIME);

//		} // cameraIsOn
	} // while !stopped

	stopped = false;
}


bool CamThread::isConnected(void)
{
	return cameraIsOn;
}


void CamThread::enableFaceDetection(int state)
{
	if (state == Qt::Checked)
	{
		faceDetectionIsEnabled = true;
		faceDetectionWasActive = true;
	}
	else
	{
		faceDetectionIsEnabled = false;
	}
}


void CamThread::setCascadePath(QString haarClassifierCascade)
{
	if (haarClassifierCascade == "none")
	{
		faceDetectionIsEnabled = false;
		// disable checkBoxes in the GUI
		emit disableFaceDetection();
	}


	haarClassifierCascadeFilename = haarClassifierCascade;
}


bool CamThread::init()
{
	if (initDone == false)
	{
		// do only *one* init!
		initDone = true;
/*
		if ( 1 )
		{
			emit message(QString("<font color=\"#FF0000\">ERROR: could not initialize capturing from /dev/video%1.</font>").arg( 0 ));

			cameraIsOn = false;
			stopped = true;

			return false;
		}
	}

	cameraIsOn = true;
*/
	return true;
	}

//	return false;
}
