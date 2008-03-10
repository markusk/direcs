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

	
	//------------------------------------------
	// try to capture from the first camera (0)
	//------------------------------------------
	capture = cvCaptureFromCAM(0);
	
	if (!capture)
	{
		qDebug("INFO: could not initialize capturing from /dev/video0. No camera connected?");
		cameraIsOn = false;
		width=0;
		height=0;
		stopped = true;
		
	}
	else
	{
		cameraIsOn = true;

		//--------------------------------------------
		// retrieve image (camera) size and store it!
		//--------------------------------------------
		imgPtr = cvRetrieveFrame(capture);

		// get width and height
		width = imgPtr->width;
		height = imgPtr->height;


		//--------------------------------------------
		// 
		//--------------------------------------------
		//TODO: check path (use variables from mrs)
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
	CvPoint rectStart;
	CvPoint rectEnd;
	int radius;
	CvFont font;
	//CvFont fontSmall;
	int faceX = 0;
	int faceY = 0;
	int faceRadius = 0;
	QString text;


	// cvInitFont( CvFont* font, int font_face, double hscale, double vscale, double shear=0, int thickness=1, int line_type=8 );
	cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX, 1.0, 1.0, 0, 2 /* thickness */);
	//cvInitFont(&fontSmall, CV_FONT_HERSHEY_PLAIN, 0.2, 0.2, 0, 1 /* thickness */);

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

			// put image size information to top left
			//text = QString("%1x%2").arg(width).arg(height);
			//cvPutText( imgPtr, text.toAscii(), cvPoint(20, 20), &font, CV_RGB(64, 64, 255) );
			
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
					
					// when *no* faces detected
					if (faces->total == 0)
					{
						// for emit signal
						faceX = 0;
						faceY = 0;
						faceRadius = 0;
					}
					else
					{
						// draw a rectangle for each face
						for( i = 0; i < (faces ? faces->total : 0); i++ )
						{
							rectangle = (CvRect*)cvGetSeqElem( faces, i );
							center.x = qRound((rectangle->x + rectangle->width*0.5)*scale);
							center.y = qRound((rectangle->y + rectangle->height*0.5)*scale);
							radius = qRound((rectangle->width + rectangle->height)*0.25*scale);
							
							rectStart.x = rectangle->x*scale;
							rectStart.y = rectangle->y*scale;
							rectEnd.x = rectStart.x + rectangle->width*scale;
							rectEnd.y = rectStart.y + rectangle->height*scale;
				
							// draw rectangles(s)
							if (i==0)
							{
								// for emit signal
								faceX = center.x;
								faceY = center.y;
								faceRadius = radius;
								// first face in white
								cvRectangle(imgPtr, rectStart, rectEnd, CV_RGB(255, 255, 255));
							}
							else
							{
								// other faces darker color
								cvRectangle(imgPtr, rectStart, rectEnd, CV_RGB(128, 128, 128));
							}
						}
					}
				}
			}
			else
			{
				// face detection disabled
				faceX = 0;
				faceY = 0;
				faceRadius = 0;
			}
			//----------------------------------------
			// face detection (end)
			//----------------------------------------
			
			
			//----------------------------------------
			// draw end switch alarm into image
			//----------------------------------------
			if (contactAlarmLeft)
			{
				// TODO: Use GetTextSize!
				//Retrieves width and height of text string
				//void cvGetTextSize( const char* text_string, const CvFont* font, CvSize* text_size, int* baseline );
				cvPutText( imgPtr, "[Stop]", cvPoint(5, (imgPtr->height/2)-5), &font, CV_RGB(255, 64, 64) );
			}
			
			if (contactAlarmRight)
			{
				cvPutText( imgPtr, "[Stop]", cvPoint(imgPtr->width-100, (imgPtr->height/2)-5), &font, CV_RGB(255, 64, 64) );
			}
			
			if (contactAlarmTop)
			{
				cvPutText( imgPtr, "[Stop]", cvPoint((imgPtr->width/2)-50, 30), &font, CV_RGB(255, 64, 64) );
			}
			
			if (contactAlarmBottom)
			{
				cvPutText( imgPtr, "[Stop]", cvPoint((imgPtr->width/2)-50, imgPtr->height-10), &font, CV_RGB(255, 64, 64) );
			}
			
			
			//====================================================================
			//  e m i t  Signal (e.g. send image and face0 coordinates to GUI)
			//====================================================================
			emit camDataComplete(imgPtr, faceX, faceY, faceRadius);
		
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


int CamThread::imageWidth()
{
	return width;
}


int CamThread::imageHeight()
{
	return height;
}
