#include "camThread.h"


CamThread::CamThread() : QThread()
{
	//QMutexLocker locker(&mutex); // make this class thread-safe
	stopped = false;
	cameraIsOn = false;
	frame = new IplImage();
	imgPtr = new IplImage();
	
	
	// try to capture from the first camera (0
	capture = cvCaptureFromCAM(0);
	
	if (!capture)
	{
		qDebug("Could not initialize capturing! :-(");
		cameraIsOn = false;
		stopped = true;
	}
	else
	{
		cameraIsOn = true;
	}
}


CamThread::~CamThread()
{
	delete frame;
}


void CamThread::stop()
{
	stopped = true;
}


void CamThread::run()
{
	//
	//  start "threading"...
	//
	while (!stopped)
	{
		if (cameraIsOn == true)
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
		
			//====================================================================
			//  e m i t  Signal
			//====================================================================
			emit camDataComplete(imgPtr);
		
			// let the thread sleep some time
			//msleep(THREADSLEEPTIME);
		}
	}
	
	
	if (cameraIsOn == true)
	{
		// release capture
		cvReleaseCapture(&capture);
	}
	
	stopped = false;
}


bool CamThread::isConnected(void)
{
	return cameraIsOn;
}
