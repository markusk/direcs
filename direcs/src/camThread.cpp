/*************************************************************************
 *   Copyright (C) Markus Knapp                                          *
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

	mThreshold = 100;
}


CamThread::~CamThread()
{
	stopped = true;

	// Stops the runloop (if running), cleans up all buffers, unsets callbacks, and flushes queues from the Kinect
//	freenect_sync_stop();
}


void CamThread::disable()
{
	stop();

	initDone = true;
	cameraIsOn = false; // double safe

	emit message("+++ camThread disabled! +++");
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

	if (cameraIsOn)
	{
		// create an empty image with OpenCV
		Mat rgbMat(Size(640, 480), CV_8UC3, Scalar(0));
		Mat rgbMat2(Size(640, 480), CV_8UC3, Scalar(0));
		Mat rgbMat3(Size(640, 480), CV_8UC3, Scalar(0));

		// create an empty image with OpenCV
		Mat depthMat(Size(640, 480), CV_16UC1);
		Mat depthF  (Size(640, 480), CV_8UC1);

		// show empty image with OpenCV
		// namedWindow("video");

		//  start "threading"...
		while (!stopped)
		{

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
			emit camImageComplete (&qimage);


			//-------------------
			// get depth picture
			//-------------------
			freenect_sync_get_depth((void**)&dataDepth, &timestamp, 0, FREENECT_DEPTH_10BIT);

			// convert image
			depthMat.data = (uchar*) dataDepth;

			// convert again
			depthMat.convertTo(depthF, CV_8UC1, 255.0/2048.0);
			cvtColor( depthF, rgbMat2, CV_GRAY2RGB );

			// convert to QImage
			qimageDepth = QImage( (uchar*) rgbMat2.data, rgbMat2.cols, rgbMat2.rows, rgbMat2.step, QImage::Format_RGB888 );

			// send DEPTH image to GUI
			emit camImageDepthComplete(&qimageDepth);


			//-----------------------------------------
			// do OpenCV stuff here on the depth image
			//-----------------------------------------
			/*
			// canny
			//
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

			// contours
			//
			vector<vector<Point> > lines;
			vector<Vec4i> hierarchy;

			cvtColor( rgbMat2, gray, CV_BGR2GRAY );
			threshold( gray, gray, mThreshold, 255, CV_THRESH_BINARY );

			findContours( gray, lines, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
			drawContours(gray, lines, -1, Scalar(255, 0, 0), 2);

			// convert to QImage
			cvtColor( gray, rgbMat3, CV_GRAY2RGB );
			qimageOpenCV = QImage( (uchar*) rgbMat3.data, rgbMat3.cols, rgbMat3.rows, rgbMat3.step, QImage::Format_RGB888 );

			// send OpenCV processed image to GUI
			emit camImageOpenCVComplete(&qimageOpenCV);


			// let the thread sleep some time
			msleep(THREADSLEEPTIME);

		} // while !stopped

	} // cameraIsOn

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
	double angle = 0;


	// do only *one* init!
	if (initDone == false)
	{
		initDone = true;


		// constrain the angle from -30 to +30
		if (angle > 30)
		{
			angle = 30;
		}
		else if (angle <-30)
		{
			angle=-30;
		}


		// try to set angle
		if (freenect_sync_set_tilt_degs(angle, 0))
		{
			emit message("<font color=\"#FF0000\">ERROR: could not initialize Kinect camera.</font>");

			cameraIsOn = false;
			stopped = true;

			return false;
		}

	}


	emit message("Kinect camera initialized.");
	cameraIsOn = true;
	stopped = false;
	return true;
}


void CamThread::setThreshold(int threshold)
{
	mThreshold = threshold;
}
