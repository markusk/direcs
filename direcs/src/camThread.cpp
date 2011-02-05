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

	m_gamma.resize(2048);

	/// build the gamma table used for the depth to rgb conversion
	/// taken from the demo programs
	for (int i=0; i<2048; ++i)
	{
		float v = i/2048.0;
		v = std::pow(v, 3)* 6;
		m_gamma[i] = v*6*256;
	}
}


CamThread::~CamThread()
{
	stopped = true;

	// Stops the runloop (if running), cleans up all buffers, unsets callbacks, and flushes queues from the Kinect
//	freenect_sync_stop();
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

		// create an empty image with OpenCV
		Mat depthMat(Size(640, 480), CV_16UC1);
//		Mat depthF  (Size(640, 480), CV_8UC1);

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


			//----------------------
			// do OpenCV stuff here
			//----------------------
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
			int g_thresh = 100;
			vector<vector<Point> > lines;
			vector<Vec4i> hierarchy;

			cvtColor( rgbMat, gray, CV_BGR2GRAY );
			threshold( gray, gray, g_thresh, 255, CV_THRESH_BINARY );

			findContours( gray, lines, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
			drawContours(gray, lines, -1, Scalar(255, 0, 0), 2);

			// convert to QImage
			cvtColor( gray, rgbMat, CV_GRAY2RGB );
			qimageOpenCV = QImage( (uchar*) rgbMat.data, rgbMat.cols, rgbMat.rows, rgbMat.step, QImage::Format_RGB888 );

			// send OpenCV processed image to GUI
			emit camImageOpenCVComplete(&qimageOpenCV);


			//-------------------
			// get depth picture
			//-------------------
			freenect_sync_get_depth((void**)&dataDepth, &timestamp, 0, FREENECT_DEPTH_10BIT);

			// convert image
			depthMat.data = (uchar*) dataDepth;


// + + +


			// *_depth

			// cast the void pointer to the unint16_t the data is actually in
//				uint16_t* depth = static_cast<uint16_t*>(_depth);

			// now loop and fill data buffers
			for (unsigned int i = 0; i < FREENECT_FRAME_PIX; ++i)
			{
				// first our two raw buffers the first will lose precision and may well
				// be removed in the next iterations
// not needed!	m_bufferDepthRaw[i] = depth[i];
				m_bufferDepthRaw16[i] = depth[i];

				// now get the index into the gamma table
				int pval = m_gamma[depth[i]];

				// get the lower bit
				int lb = pval & 0xff;

				// shift right by 8 and determine which colour value to fill the
				// array with based on the position
				switch (pval>>8)
				{
				case 0:
					m_bufferDepth[3*i+0] = 255;
					m_bufferDepth[3*i+1] = 255-lb;
					m_bufferDepth[3*i+2] = 255-lb;
					break;
				case 1:
					m_bufferDepth[3*i+0] = 255;
					m_bufferDepth[3*i+1] = lb;
					m_bufferDepth[3*i+2] = 0;
					break;
				case 2:
					m_bufferDepth[3*i+0] = 255-lb;
					m_bufferDepth[3*i+1] = 255;
					m_bufferDepth[3*i+2] = 0;
					break;
				case 3:
					m_bufferDepth[3*i+0] = 0;
					m_bufferDepth[3*i+1] = 255;
					m_bufferDepth[3*i+2] = lb;
					break;
				case 4:
					m_bufferDepth[3*i+0] = 0;
					m_bufferDepth[3*i+1] = 255-lb;
					m_bufferDepth[3*i+2] = 255;
					break;
				case 5:
					m_bufferDepth[3*i+0] = 0;
					m_bufferDepth[3*i+1] = 0;
					m_bufferDepth[3*i+2] = 255-lb;
					break;
				default:
					m_bufferDepth[3*i+0] = 0;
					m_bufferDepth[3*i+1] = 0;
					m_bufferDepth[3*i+2] = 0;
					break;
				}
			}


// + + +


//			depthMat.convertTo(depthF, CV_8UC1, 255.0/2048.0);

			// convert to QImage
			cvtColor( depthMat, rgbMat2, CV_GRAY2RGB );
//			qimage = QImage( (uchar*) depthMat.data, depthMat.cols, depthMat.rows, depthMat.step, QImage::Format_Indexed8 );
			qimageDepth = QImage( (uchar*) rgbMat2.data, rgbMat2.cols, rgbMat2.rows, rgbMat2.step, QImage::Format_RGB888 );

			// send DEPTH image to GUI
			emit camImageDepthComplete(&qimageDepth);

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
