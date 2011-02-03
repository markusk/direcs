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

//	image = cvCreateImageHeader(cvSize(640,480), 8, 3);
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

	IplImage *image = cvCreateImageHeader(cvSize(640,480), 8, 3);

	//  start "threading"...
	while (!stopped)
	{
//		if (cameraIsOn == true)
//		{

		freenect_sync_get_video((void**)&data, &timestamp, 0, FREENECT_VIDEO_RGB);
		cvSetData(image, data, 640*3);
		cvCvtColor(image, image, CV_RGB2BGR);
		cvShowImage("RGB", image);

		/*
		Mat mImage;  > header!

		QImage tmp( (uchar*)color.data, color.cols, color.rows, color.step, QImage::Format_RGB888  );
		ui.imageFrame->setPixmap( QPixmap::fromImage( tmp ) );
		*/

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
