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

#ifndef CAMTHREAD_H
#define CAMTHREAD_H

//-------------------------------------------------------------------
#include <QtGlobal> // for Q_OS_* Makro!
#include <QThread>
#include <QImage>
#include <QtDebug> // for a more convenient use of qDebug
#include <QFile>
#include <QMutex>
//-------------------------------------------------------------------
#include <libfreenect.hpp>
#include <opencv/cv.h>
#include <opencv/cxcore.h>
#include <opencv/highgui.h>

#include <iostream>
#include <vector>
#include <cmath>

using namespace cv;
using namespace std;

//-------------------------------------------------------------------


/*!
\brief This class gets a live picture from a Kinect camera.
This class uses the freenect library and the OpenCV library for grabbing pictures and also for face detection.
*/
class CamThread : public QThread, public Freenect::FreenectDevice
{
	Q_OBJECT


	public:
		CamThread(freenect_context *_ctx, int _index) : Freenect::FreenectDevice(_ctx, _index),
			m_buffer_depth(FREENECT_DEPTH_11BIT_SIZE),
			m_buffer_rgb(FREENECT_VIDEO_RGB_SIZE),
			m_gamma(2048),
			m_new_rgb_frame(false),
			m_new_depth_frame(false),
			depthMat(cv::Size(640,480),CV_16UC1), rgbMat(cv::Size(640,480), CV_8UC3,Scalar(0)), ownMat(cv::Size(640,480), CV_8UC3,cv::Scalar(0))
		{
			stopped = false;
			initDone = false;;
			cameraIsOn = false;
			faceDetectionIsEnabled = false;
			faceDetectionWasActive = false;
			haarClassifierCascadeFilename = "none";

			Mat depthMat(cv::Size(640,480), CV_16UC1);
			Mat depthf(cv::Size(640,480), CV_8UC1);
			Mat rgbMat(cv::Size(640,480), CV_8UC3, cv::Scalar(0));
			Mat ownMat(cv::Size(640,480), CV_8UC3, cv::Scalar(0));

			for (unsigned int i = 0 ; i < 2048 ; i++)
			{
				float v = i/2048.0;
				v = std::pow(v, 3)* 6;
				m_gamma[i] = v*6*256;
			}


			Freenect::Freenect freenect;
		//	MyFreenectDevice& device = freenect.createDevice<MyFreenectDevice>(0);

		//	namedWindow("rgb",CV_WINDOW_AUTOSIZE);
		//	namedWindow("depth",CV_WINDOW_AUTOSIZE);

		//	device.startVideo();
		//	device.startDepth();
		}

		~CamThread();


		// Do not call directly even in child
		void VideoCallback(void* _rgb, uint32_t timestamp);

		// Do not call directly even in child
		void DepthCallback(void* _depth, uint32_t timestamp);

		bool getVideo(Mat& output);

		bool getDepth(Mat& output);

		/**
		@return true on success
		*/
		bool init();

		/**
		*/
		bool isConnected(void);

		/**
		Sets the path and filename to the haar classifier cascade.
		@param haarClassifierCascade is the whole filename
		 */
		void setCascadePath(QString haarClassifierCascade);

		void stop();
		virtual void run();


	public slots:
		/**
		Enables or disables the face detection. When activated, a circle for each face is drawn on the camera live image.
		@param state has to be Qt::Checked to enable the detection. All other states disable.
		*/
		void enableFaceDetection(int state);


	signals:
		/**
		@param *imgPtr is a pointer to the camera image
		@sa Gui::setCamImage()
		*/
		void camDataComplete(IplImage* imgPtr);
		//void camDataComplete(QImage* image);

		/**
		Disables checkBoxes in the GUI
		*/
		void disableFaceDetection();

		/**
		Disables camera controls in the GUI
		 */
		void disableCamera();

		/**
		This signal is emmited when a face was detected in the camera image

		@param faces is the total number of faces detected
		@param faceX is the X coordinate to the middle of a detected face (0, if none)
		@param faceY is the Y coordinate to the middle of a detected face (0, if none)
		@param faceRadius is the radius (0, if none)
		@param lastFaceX is the X coordinate of the latest face detected
		@param lastFaceY is the Y coordinate of the latest face detected
		@sa Direcs::faceTracking()
		*/
		void faceDetected(int faces, int faceX, int faceY, int faceRadius, int lastFaceX, int lastFaceY);

		/**
		Emits a info or error message to a slot.
		This slot can be used to display a text on a splash screen, log file, to print it to a console...
		@param text is the message to be emitted
		*/
		void message(QString text);


	private:
		std::vector<uint8_t> m_buffer_depth;
		std::vector<uint8_t> m_buffer_rgb;
		std::vector<uint16_t> m_gamma;
		mutable QMutex m_rgb_mutex;
		mutable QMutex m_depth_mutex;
		Mat depthMat;
		Mat rgbMat;
		Mat ownMat;
		bool m_new_rgb_frame;
		bool m_new_depth_frame;

		bool initDone;
		bool cameraIsOn;
		bool faceDetectionIsEnabled;
		bool faceDetectionWasActive;
		QString haarClassifierCascadeFilename;
		volatile bool stopped;

		// Every thread sleeps some time, for having a bit more time for the other threads!
		// Time in milliseconds
		static const unsigned long THREADSLEEPTIME = 100; // Default: 100 ms
};

#endif
