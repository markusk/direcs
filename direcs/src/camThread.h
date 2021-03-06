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
#include <opencv/cv.h>
#include <opencv/highgui.h>

#ifdef Q_OS_MAC
#include <libfreenect/libfreenect_sync.h>
#else
#include <libfreenect_sync.h>
#endif


#include <iostream>
#include <vector>
#include <cmath>

using namespace cv;
using namespace std;

//-------------------------------------------------------------------


/**
\brief This class gets a live picture from a Kinect camera.
This class uses the QThread techniques and then some OpenCV stuff on the live picture; we will see...
*/
class CamThread : public QThread
{
	Q_OBJECT


	public:
		CamThread();

		~CamThread();

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

		/**
		 * @brief Disable the camera thread. I.E. when no camera usage is forced by a global ini-file.
		 */
		void disable();

		void stop();
		virtual void run();


	public slots:
		/**
		Enables or disables the face detection. When activated, a circle for each face is drawn on the camera live image.
		@param state has to be Qt::Checked to enable the detection. All other states disable.
		*/
		void enableFaceDetection(int state);

		/**
		  Set threshold for OpenCV algorithm.
		  @param threshold
		  */
		void setThreshold(int threshold);

		/**
		Sets the angle of the kinect camera (moves the motor!)
		@param angle is the angle
		@return false on error.
		*/
		bool setAngle(int angle);

		/**
		 * @brief Turns the camera LED on or off or let it flash.
		 * @param state can be LED_OFF, LED_GREEN, LED_RED, LED_YELLOW, LED_BLINK_GREEN or LED_BLINK_RED_YELLOW.
		 * @return false on error
		 */
		bool turnLED(freenect_led_options state);


	signals:
		/**
		@param *image is a pointer to the camera image
		@sa Gui::setCamImage()
		*/
		void camImageComplete(QImage* image);

		/**
		@param *image is a pointer to the camera image
		@sa Gui::setCamImageDepth()
		*/
		void camImageDepthComplete(QImage* image);

		/**
		@param *image is a pointer to the camera image
		@sa Gui::setCamImageOpenCV()
		*/
		void camImageOpenCVComplete(QImage* image);

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
		freenect_context *f_ctx;
		freenect_device *f_dev;

		char *data; // the 'raw' data from the Kinect image
		char *dataDepth;
		unsigned int timestamp;

		QImage qimage; // for sending a QImage to the GUI (Signal)
		QImage qimageDepth; // for sending a QImage to the GUI (Signal)
		QImage qimageOpenCV; // for sending a QImage to the GUI (Signal)
		Mat gray; // for OpencV

		int mThreshold;

		std::vector<uint16_t> m_gamma;

		//mutable QMutex m_rgb_mutex;
		//mutable QMutex m_depth_mutex;


		bool initDone;
		bool cameraIsOn;
		bool faceDetectionIsEnabled;
		bool faceDetectionWasActive;
		QString haarClassifierCascadeFilename;
		volatile bool stopped;

		// Kinect angles (motor movement room)
		static const int KINECTANGLEMIN = -30;
		static const int KINECTANGLEMID =   0;
		static const int KINECTANGLEMAX =  30;

		// Every thread sleeps some time, for having a bit more time for the other threads!
		// Time in milliseconds
		static const unsigned long THREADSLEEPTIME = 50; // Default: 100 ms
};

#endif
