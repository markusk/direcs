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


/*
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
*/


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
		} // camera is ON
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
