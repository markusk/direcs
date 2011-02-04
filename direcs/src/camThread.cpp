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

	// Stops the runloop (if running), cleans up all buffers, unsets callbacks, and flushes queues from the Kinect
	freenect_sync_stop();
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

	// create an empty image with OpenCV
	Mat depthMat(Size(640, 480), CV_16UC1);
	Mat depthF  (Size(640, 480), CV_8UC1);

	// show empty image with OpenCV
	// namedWindow("video");

	//  start "threading"...
	while (!stopped)
	{
//		if (cameraIsOn == true)
//		{

		//-----------------
		// get RGB picture
		//-----------------
		freenect_sync_get_video((void**)&data, &timestamp, 0, FREENECT_VIDEO_RGB);

		// convert image
		rgbMat.data = (uchar*) data;
		// cvtColor(rgbMat, rgbMat, CV_RGB2BGR); // only when shown in OpenCV window (there we need BGR)!

		// refresh image with OpenCV
		// imshow("video", rgbMat);

		// convert to QImage
		qimage = QImage( (uchar*) rgbMat.data, rgbMat.cols, rgbMat.rows, rgbMat.step, QImage::Format_RGB888 );

		// send RGB image to GUI
		emit camDataComplete (&qimage);


		//----------------------
		// do OpenCV stuff here
		//----------------------
		cv::cvtColor(rgbMat, gray, CV_RGB2GRAY);
		GaussianBlur(gray, gray, Size(5, 5), 2, 2);
		cv::Canny(gray, gray, 20, 60, 3);

		vector<Vec4i> lines;
		HoughLinesP(gray, lines, 1, CV_PI/180, 80, 30, 10);

		for (size_t i = 0; i < lines.size(); i++)
		{
			line(rgbMat, Point(lines[i][0], lines[i][1]), Point(lines[i][2], lines[i][3]), Scalar(0,0,255), 2, 8);
		}

		// convert to QImage
		qimage = QImage( (uchar*) rgbMat.data, rgbMat.cols, rgbMat.rows, rgbMat.step, QImage::Format_RGB888 );
*/

/* geht nicht
		cv::cvtColor( rgbMat, gray, CV_RGB2GRAY );
//		GaussianBlur( gray, gray, Size(9, 9), 2, 2 );
		GaussianBlur( gray, gray, Size(5, 5), 2, 2 );

		cv::Canny( gray, gray, 20, 60, 3 );

		// convert to QImage
		qimage = QImage( (uchar*) gray.data, gray.cols, gray.rows, gray.step, QImage::Format_Indexed8 );
*/

// - - - - beispiel 8.2

//		IplImage*    g_image    = NULL; // das ist das bild
		IplImage*    g_gray    = NULL;
		int        g_thresh  = 100;
		CvMemStorage*  g_storage  = NULL;


		if( g_storage==NULL )
		{
			g_gray = cvCreateImage( cvGetSize(g_image), 8, 1 );
			g_storage = cvCreateMemStorage(0);
		} else
		{
			cvClearMemStorage( g_storage );
		}

		CvSeq* contours = 0;
		cvCvtColor( g_image, g_gray, CV_BGR2GRAY );
		cvThreshold( g_gray, g_gray, g_thresh, 255, CV_THRESH_BINARY );
		cvFindContours( g_gray, g_storage, &contours );
		cvZero( g_gray );

		if( contours )
		{
			cvDrawContours(g_gray, contours, cvScalarAll(255), cvScalarAll(255), 100);
		}

		cvShowImage( "Contours", g_gray );


// - - - - - - - - - - -

		// send OpenCV processed image to GUI
		emit camOpenCVComplete(&qimage);


/*
		//-------------------
		// get depth picture
		//-------------------
		freenect_sync_get_depth((void**)&data, &timestamp, 0, FREENECT_DEPTH_10BIT);

		// convert image
		depthF.data = (uchar*) data;

//		depthMat.convertTo(depthF, CV_8UC1, 255.0/2048.0);

		// convert to QImage
//		qimage = QImage( (uchar*) depthMat.data, depthMat.cols, depthMat.rows, depthMat.step, QImage::Format_Indexed8 );
		qimage = QImage( (uchar*) depthF.data, depthF.cols, depthF.rows, depthF.step, QImage::Format_Indexed8 );

		// send DEPTH image to GUI
		emit camDepthComplete(&qimage);
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
