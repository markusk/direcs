/*************************************************************************
 *   Copyright (C) 2009 by Markus Knapp                                  *
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
#include <QImage>
#include <QtGui> // for QMessage
#include <QTime>
#include <QString>
#include <QtDebug> // for a more convenient use of qDebug
//-------------------------------------------------------------------
#ifdef Q_OS_LINUX // currently supported only under linux (no MAC OS and Windoze at the moment)
	#include <cv.h>
	#include <highgui.h>
#endif

#include <stdio.h>
#include <ctype.h>
//-------------------------------------------------------------------

typedef struct
{
	int x;
	int y;
	int radius;
	int rank;
} KOORD_T;


/*!
\brief This class gets a live picture from a connected camera.
This class uses the Open Source Computer Vision Library for grabbing the pictures and also for face detection.
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
		Sets the camera device.
		@param device is an integer value! 0 for the first vide device (e.g. /dev/video0). -2 if an error occured.
		 */
		void setCameraDevice(int device);

		/**
		Returns the image height of the camera. Retrieved in the constructor!
		*/
		int imageHeight();

		/**
		Returns the image width of the camera. Retrieved in the constructor!
		*/
		int imageWidth();

		/**
		Returns the image pixel depth of the camera. Retrieved in the constructor!
		*/
		int imagePixelDepth();
		
		void stop();
		virtual void run();

		
	public slots:
		/**
		Enables or disables the face detection. When activated, a circle for each face is drawn on the camera live image.
		@param state has to be Qt::Checked to enable the detection. All other states disable.
		*/
		void enableFaceDetection(int state);
		
		/**
		Draws a red object in the camera image, when the camera hits the end switches (when panning and tilting).
		@param position can be TOP, BOTTOM, LEFT, RIGHT
		@param state can be true or false (for ON and OFF)disableFaceDetection
		@sa Gui::showContactAlarm()
		 */
		void drawContactAlarm(char position, bool state);
		
		/**
		*/
		void test();


	
	signals:
#ifdef Q_OS_LINUX // currently supported only under linux (no MAC OS and Windoze at the moment)
		/**
		@param *imgPtr is a pointer to the camera image
		@sa Gui::setCamImage()
		*/
		void camDataComplete(IplImage* imgPtr);
		//void camDataComplete(QImage* image);
#endif

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


	private:
#ifdef Q_OS_LINUX // currently supported only under linux (no MAC OS and Windoze at the moment)
		QImage * IplImageToQImage(const IplImage * iplImage); //! Converts an OpenCV iplImage into a nice QImage :-)
#endif

		bool initDone;
		bool cameraIsOn;
		bool faceDetectionIsEnabled;
		bool faceDetectionWasActive;
		QVector <KOORD_T> detectedFaces; /** the coordinates of the last n detected faces */
		int cameraDevice;
		QString haarClassifierCascadeFilename;
#ifdef Q_OS_LINUX // currently supported only under linux (no MAC OS and Windoze at the moment)
		CvScalar hsv2rgb( float hue );
		IplImage *imgPtr;
		CvCapture *capture;
		CvMemStorage *storage;
		CvHaarClassifierCascade *cascade;
		IplImage *gray;
		IplImage *small_img;
#endif
		/* ----------
		QImage *qimage; // for IplImageToQImage()
		IplImage* tchannel0;
		IplImage* tchannel1;
		IplImage* tchannel2;
		IplImage* tchannel3;
		int bytesPerLine;
		IplImage *imgPtrDest;
		----------- */
		int width;
		int height;
		int pixeldepth;
		bool contactAlarmTop;
		bool contactAlarmBottom;
		bool contactAlarmLeft;
		bool contactAlarmRight;
		volatile bool stopped;
		
		// Every thread sleeps some time, for having a bit more time for the other threads!
		// Time in milliseconds
		//static const unsigned long THREADSLEEPTIME = 100; // Default: 100 ms
		
		static const double scale = 1.7; // 1.3 is okay for 640*480 images, 1.8 for 640*480.
		
		// The position for the contact alarm in the camera image
		static const char LEFT   = 0;
		static const char RIGHT  = 1;
		static const char TOP    = 2;
		static const char BOTTOM = 3;
		
		static const int FACEARRAYSIZE = 24;
};

#endif
