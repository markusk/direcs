#ifndef CAMTHREAD_H
#define CAMTHREAD_H

//-------------------------------------------------------------------
#include <QImage>
#include <QtGui> // for QMessage
#include <QTime>
#include <QString>
//-------------------------------------------------------------------
#include "cv.h"
#include "highgui.h"
#include <stdio.h>
#include <ctype.h>
//-------------------------------------------------------------------

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
		bool isConnected(void);

		/*
		Returns the image height of the camera. Retrieved in the constructor!
		*/
		int imageHeight();

		/*
		Returns the image width of the camera. Retrieved in the constructor!
		*/
		int imageWidth();
		
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
		@param state can be true or false (for ON and OFF)
		@sa Gui::showContactAlarm()
		 */
		void drawContactAlarm(char position, bool state);

	
	signals:
		/**
		@param *frame is a pointer to the cam image
		@param faceX is the X coordinate to the middle of a detected face (0, if none)
		@param faceY is the Y coordinate to the middle of a detected face (0, if none)
		@param faceRadius is the radius (0, if none)
		@sa Mrs::faceTracking()
		*/
		void camDataComplete(IplImage* frame, int faceX, int faceY, int faceRadius, int lastFaceX, int lastFaceY);


	private:
		bool cameraIsOn;
		bool faceDetectionIsEnabled;
		bool faceDetectionWasActive;
		CvScalar hsv2rgb( float hue );
		IplImage *frame;
		IplImage *imgPtr;
		int width;
		int height;
		CvCapture *capture;
		CvMemStorage *storage;
		CvHaarClassifierCascade *cascade;
		IplImage *gray;
		IplImage *small_img;
		bool contactAlarmTop;
		bool contactAlarmBottom;
		bool contactAlarmLeft;
		bool contactAlarmRight;
		//mutable QMutex mutex; // make this class thread-safe
		volatile bool stopped;

		// Every thread sleeps some time, for having a bit more time for the other threads!
		// Time in milliseconds
		//static const unsigned long THREADSLEEPTIME = 100; // Default: 100 ms
		static const double scale = 1.7; // 1.3 is okay for 640*480 images, 1.8 for 640*480.
		static const int circleThickness=1;
		static const int lineThickness=16;
		
		// The position for the contact alarm in the camera image
		static const char LEFT   = 0;
		static const char RIGHT  = 1;
		static const char TOP    = 2;
		static const char BOTTOM = 3;
};

#endif
