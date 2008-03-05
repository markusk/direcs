#include "camThread.h"


CamThread::CamThread() : QThread()
{
	//QMutexLocker locker(&mutex); // make this class thread-safe
	stopped = false;
	cameraIsOn = false;
	faceDetectionIsEnabled = false;
	faceDetectionWasActive = false;
	frame = new IplImage();
	imgPtr = new IplImage();
	contactAlarmTop = false;
	contactAlarmBottom = false;
	contactAlarmLeft = false;
	contactAlarmRight = false;

	
	// try to capture from the first camera (0)
	capture = cvCaptureFromCAM(0);
	
	if (!capture)
	{
		qDebug("INFO: could not initialize capturing from /dev/video0. No camera connected?");
		cameraIsOn = false;
		stopped = true;
	}
	else
	{
		cameraIsOn = true;
	}

	if (cameraIsOn)
	{
		cascade = (CvHaarClassifierCascade*)cvLoad("/home/markus/develop/sourceforge/mrs/trunk/data/haarcascades/haarcascade_frontalface_alt2.xml", 0, 0, 0);

		if(!cascade)
	    {
	        qDebug("ERROR: Could not load classifier cascade for face detection!");
	    }
		else
		{
			storage = cvCreateMemStorage(0);
		}
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
	int i=0;
	CvSeq *faces;
	CvRect *rectangle;
	CvPoint center;
	CvPoint lineStart;
	CvPoint lineEnd;
	int radius;
	//CvFont *font;

	// cvInitFont( CvFont* font, int font_face, double hscale, double vscale, double shear=0, int thickness=1, int line_type=8 );
	// SIGSEV running this line  cvInitFont(font, CV_FONT_HERSHEY_SIMPLEX, 1.0, 1.0, 0, 1 /* thickness */, 8);

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
		
			// retrieve image
			imgPtr = cvRetrieveFrame(capture);

			//----------------------------------------
			// face detection (start)
			//----------------------------------------
			if (faceDetectionIsEnabled)
			{
				gray = cvCreateImage( cvSize(imgPtr->width, imgPtr->height), 8, 1 );
				// convert into a smaller image, to make things faster 
				small_img = cvCreateImage( cvSize( cvRound (imgPtr->width/scale), cvRound (imgPtr->height/scale)), 8, 1 );
	
				cvCvtColor( imgPtr, gray, CV_BGR2GRAY );
				cvResize( gray, small_img, CV_INTER_LINEAR );
				cvEqualizeHist( small_img, small_img );
				cvClearMemStorage( storage );
	
				if( cascade )
				{
					// detect objects in the image
					faces = cvHaarDetectObjects( small_img, cascade, storage, 1.1, 2, 0, cvSize(30, 30) );
	
					// draw a circle for each face
					for( i = 0; i < (faces ? faces->total : 0); i++ )
					{
						rectangle = (CvRect*)cvGetSeqElem( faces, i );
						center.x = qRound((rectangle->x + rectangle->width*0.5)*scale);
						center.y = qRound((rectangle->y + rectangle->height*0.5)*scale);
						radius = qRound((rectangle->width + rectangle->height)*0.25*scale);
			
						// draw circle(s)
						// void cvCircle( CvArr* img, CvPoint center, int radius, CvScalar color, int thickness=1, int line_type=8, int shift=0 );
						if (i==0)
							cvCircle( imgPtr, center, radius, CV_RGB(255, 0, 0), circleThickness);
						else
							cvCircle( imgPtr, center, radius, CV_RGB(0, 0, 255), circleThickness);
					}
				}
			}
			//----------------------------------------
			// face detection (end)
			//----------------------------------------
			
			
			//----------------------------------------
			// draw end switch alarm into image
			//----------------------------------------
			//cvPutText( imgPtr, "Tilt!", lineStart, font, CV_RGB(255, 64, 64) );
			if (contactAlarmLeft)
			{
				lineStart.x = lineThickness;
				lineStart.y = 1;
				lineEnd.x = lineThickness;
				lineEnd.y = imgPtr->height;
				cvLine( imgPtr, lineStart, lineEnd, CV_RGB(255, 64, 64), lineThickness);
			}
			
			if (contactAlarmRight)
			{
				lineStart.x = imgPtr->width - lineThickness;
				lineStart.y = 1;
				lineEnd.x = imgPtr->width - lineThickness;
				lineEnd.y = imgPtr->width;
				cvLine( imgPtr, lineStart, lineEnd, CV_RGB(255, 64, 64), lineThickness);
			}
			
			if (contactAlarmTop)
			{
				lineStart.x = lineThickness;
				lineStart.y = lineThickness;
				lineEnd.x = imgPtr->width - lineThickness;
				lineEnd.y = lineThickness;
				cvLine( imgPtr, lineStart, lineEnd, CV_RGB(255, 64, 64), lineThickness);
			}
			
			if (contactAlarmBottom)
			{
				lineStart.x = lineThickness;
				lineStart.y = imgPtr->height - lineThickness;
				lineEnd.x = imgPtr->width - lineThickness;
				lineEnd.y = imgPtr->height - lineThickness;
				cvLine( imgPtr, lineStart, lineEnd, CV_RGB(255, 64, 64), lineThickness);
			}
			
			
			//====================================================================
			//  e m i t  Signal (e.g. send image to GUI)
			//====================================================================
			emit camDataComplete(imgPtr);
		
			// let the thread sleep some time
			//msleep(THREADSLEEPTIME);
		}
	}
	
	
	if (cameraIsOn == true)
	{
		if (faceDetectionWasActive == true)
		{
			cvReleaseImage(&gray);
		    cvReleaseImage(&small_img);
		}

	    // release capture
		cvReleaseCapture(&capture);
	}
	
	stopped = false;
}


bool CamThread::isConnected(void)
{
	return cameraIsOn;
}


void CamThread::enableFaceDetection(int state)
{
	if (state == Qt::Checked)
	{
		faceDetectionIsEnabled = true;
		faceDetectionWasActive = true;
	}
	else
	{
		faceDetectionIsEnabled = false;
	}
}


void CamThread::drawContactAlarm(char position, bool state)
{
	switch (position)
	{
		case TOP:
			contactAlarmTop = state;
			break;
		case BOTTOM:
			contactAlarmBottom = state;
			break;
		case LEFT:
			contactAlarmLeft = state;
			break;
		case RIGHT:
			contactAlarmRight = state;
			break;
	}
}
