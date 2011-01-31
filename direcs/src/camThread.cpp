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
	initDone = false;;

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
		Mat depthMat(Size(640,480),CV_16UC1);
		Mat depthf(Size(640,480),CV_8UC1);
		Mat rgbMat(Size(640,480),CV_8UC3,Scalar(0));
		Mat ownMat(Size(640,480),CV_8UC3,Scalar(0));

		Freenect::Freenect freenect;
//		DirecsKinect& device = freenect.createDevice<DirecsKinect>(0);

		namedWindow("rgb",CV_WINDOW_AUTOSIZE);
		namedWindow("depth",CV_WINDOW_AUTOSIZE);

//		device.startVideo();
//		device.startDepth();
		initDone = true;
	}


	//  start "threading"...
	while (!stopped)
	{
//		if (cameraIsOn == true)
//		{
//		device.getVideo(rgbMat);
//		device.getDepth(depthMat);
//		cv::imshow("rgb", rgbMat);
//		depthMat.convertTo(depthf, CV_8UC1, 255.0/2048.0);
//		cv::imshow("depth",depthf);

		// let the thread sleep some time
		msleep(THREADSLEEPTIME);
//		} // cameraIsOn
	} // while !stopped

	if (initDone)
	{
//			cvDestroyWindow("rgb");
//			cvDestroyWindow("depth");

//			device.stopVideo();
//			device.stopDepth();
	}

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
/*
	if (initDone == false)
	{
		// do only *one* init!
		initDone = true;

		if ( 1 )
		{
			emit message(QString("<font color=\"#FF0000\">ERROR: could not initialize capturing from /dev/video%1.</font>").arg( 0 ));

			cameraIsOn = false;
			stopped = true;

			return false;
		}
	}

		cameraIsOn = true;

	return true;
*/

	return false;
}
