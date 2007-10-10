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


class CamThread : public QThread
{
    Q_OBJECT

	public:
		CamThread();
		~CamThread();
		QImage getFrame(bool hiResMode, bool stretchMode);
		bool getStatus();
		void setStatus(bool status);
		
		void stop();
		virtual void run();


	signals:
		void camDataComplete(IplImage* frame);


	private:
		//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		CvScalar hsv2rgb( float hue );
		//CvCapture* capture;
		IplImage* frame;
		IplImage* imgPtr;
		//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		bool cameraIsOn;
		
		//mutable QMutex mutex; // make this class thread-safe
		volatile bool stopped;
		
		// Every thread sleeps some time, for having a bit more time for the other threads!
		// Time in milliseconds
		static const unsigned long THREADSLEEPTIME = 100; // Default: 7.000 ms
		
};

#endif
