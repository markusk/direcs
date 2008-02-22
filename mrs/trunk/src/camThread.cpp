#include "camThread.h"


CamThread::CamThread() : QThread()
{
	//QMutexLocker locker(&mutex); // make this class thread-safe
	stopped = false;
	cameraIsOn = false;
	frame = new IplImage();
	imgPtr = new IplImage();
	// for face detection
	storage = new CvMemStorage();
	cascade = new CvHaarClassifierCascade();
	/*
    colors[0][0] = 
        {{0,0,255}},
        {{0,128,255}},
        {{0,255,255}},
        {{0,255,0}},
        {{255,128,0}},
        {{255,255,0}},
        {{255,0,0}},
        {{255,0,255}}
    };
    */
	//----------------------------------------
	// face detection (start)
	//----------------------------------------

	
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
	delete cascade;
	delete storage;
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
			

			//----------------------------------------
			// face detection (start)
			//----------------------------------------
		    double scale = 1.3;
		    IplImage* gray = cvCreateImage( cvSize(imgPtr->width, imgPtr->height), 8, 1 );
		    IplImage* small_img = cvCreateImage( cvSize( cvRound (imgPtr->width/scale), cvRound (imgPtr->height/scale)), 8, 1 );
		    int i;

		    cvCvtColor( imgPtr, gray, CV_BGR2GRAY );
		    cvResize( gray, small_img, CV_INTER_LINEAR );
		    cvEqualizeHist( small_img, small_img );
		    cvClearMemStorage( storage );

		    if( cascade )
		    {
		        //double t = (double)cvGetTickCount();
		        CvSeq* faces = cvHaarDetectObjects( small_img, cascade, storage, 1.1, 2, 0/*CV_HAAR_DO_CANNY_PRUNING*/, cvSize(30, 30) );
		        //t = (double)cvGetTickCount() - t;
		        
		        //printf( "detection time = %gms\n", t/((double)cvGetTickFrequency()*1000.) );
		        
		        // draw a circle for each face
		        for( i = 0; i < (faces ? faces->total : 0); i++ )
		        {
		            CvRect* r = (CvRect*)cvGetSeqElem( faces, i );
		            CvPoint center;
		            int radius;
		            center.x = cvRound((r->x + r->width*0.5)*scale);
		            center.y = cvRound((r->y + r->height*0.5)*scale);
		            radius = cvRound((r->width + r->height)*0.25*scale);
		            
		            // void cvCircle( CvArr* img, CvPoint center, int radius, double color, int thickness=1 );
		            cvCircle( imgPtr, center, radius, CV_RGB(255, 0, 0), 1, 8, 0 );
		        }
		    }

		    cvReleaseImage( &gray );
		    cvReleaseImage( &small_img );
			//----------------------------------------
			// face detection (end)
			//----------------------------------------
			
		
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
		// release capture
		cvReleaseCapture(&capture);
	}
	
	stopped = false;
}


bool CamThread::isConnected(void)
{
	return cameraIsOn;
}
