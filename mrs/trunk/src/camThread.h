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
		
		void stop();
		virtual void run();

		
	public slots:
		/**
		Enables or disables the face detection. When activated, a circle for each face is drawn on the camera live image.
		@param state has to be Qt::Checked to enable the detection. All other states disable.
		*/ 
		void enableFaceDetection(int state);

	
	signals:
		void camDataComplete(IplImage* frame);


	private:
		bool cameraIsOn;
		bool faceDetectionIsEnabled;
		bool faceDetectionWasActive;
		CvScalar hsv2rgb( float hue );
		IplImage *frame;
		IplImage *imgPtr;
		CvCapture *capture;
		CvMemStorage *storage;
		CvHaarClassifierCascade *cascade;
	    IplImage *gray;
	    IplImage *small_img;
		//mutable QMutex mutex; // make this class thread-safe
		volatile bool stopped;

		// Every thread sleeps some time, for having a bit more time for the other threads!
		// Time in milliseconds
		//static const unsigned long THREADSLEEPTIME = 100; // Default: 100 ms
	    static const double scale = 1.7; // 1.3 is okay for 640*480 images, 1.8 for 640*480. 
	    static const int circleThickness=1;
	    static const int lineThickness=2;
};

#endif
