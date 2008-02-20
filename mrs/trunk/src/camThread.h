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
This class uses the Open Source Computer Vision Library.
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


	signals:
		void camDataComplete(IplImage* frame);


	private:
		bool cameraIsOn;
		CvScalar hsv2rgb( float hue );
		IplImage *frame;
		IplImage *imgPtr;
		CvCapture *capture;
		CvMemStorage *storage;
		CvHaarClassifierCascade *cascade;
		//mutable QMutex mutex; // make this class thread-safe
		volatile bool stopped;


		// Every thread sleeps some time, for having a bit more time for the other threads!
		// Time in milliseconds
		// static const unsigned long THREADSLEEPTIME = 100; // Default: 100 ms
};

#endif
