#include "camThread.h"


CamThread::CamThread() : QThread()
{
	//QMutexLocker locker(&mutex); // make this class thread-safe
	stopped = false;
	
	cameraIsOn = false;
	
	//capture = new CvCapture();
	
	// okay: IplImage* frame = 0;
	frame = new IplImage();
	
	// neu:
	imgPtr = new IplImage();
}


CamThread::~CamThread()
{
	delete frame;
}


bool CamThread::getStatus()
{
	// TODO: do something with the cam and check the answer
	cameraIsOn = true;
	return cameraIsOn;
}


void CamThread::setStatus(bool status)
{
	// set the status
	cameraIsOn = status;
}


void CamThread::stop()
{
	stopped = true;
}


void CamThread::run()
{
	//QDateTime timestamp;
	
	//--------------------
	// capture from dev 0
	//--------------------
	CvCapture* capture = cvCaptureFromCAM(0);

	if (!capture)
	{
		qDebug("Could not initialize capturing! :-(");
		stopped = true;
		return;
	}
	
	
	//
	//  start "threading"...
	//
	while (!stopped)
	{


		//----------------
		// get frame
		//----------------
		if (!cvGrabFrame(capture))
		{
			qDebug("Error, getting frame! :-(");
			stopped = true;
			return;
		}
	
		imgPtr = cvRetrieveFrame(capture);
	
	
		/*
		//---------------------------------------------------------------------
		// save image to disk, but not within the same seond (same timestamp)
		//---------------------------------------------------------------------
		if (QDateTime::currentDateTime() != timestamp)
		{
			// get the actual date and time
			timestamp = QDateTime::currentDateTime();
			QString filename = timestamp.toString("yyyy-MM-dd_hh-mm-ss");
			filename += ".png";
			//cvSaveImage(filename.toAscii(), frame);
			cvSaveImage(filename.toAscii(), imgPtr);
		}
		*/


		//====================================================================
		//  e m i t  Signal
		//====================================================================
		emit camDataComplete(imgPtr);
	
	
		// let the thread sleep some time
		msleep(THREADSLEEPTIME);
	
	}
	
	
	// release capture
	cvReleaseCapture(&capture);
	
	stopped = false;
}
