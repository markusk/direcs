#include "camThread.h"


CamThread::CamThread() : QThread()
{
	stopped = false;
	initDone = false;;
	cameraIsOn = false;
	faceDetectionIsEnabled = true;
	faceDetectionWasActive = false;
	haarClassifierCascadeFilename = "none";
	cameraDevice = -2; // -2, because of -1 is a default device!
	imgPtr = new IplImage();
	contactAlarmTop = false;
	contactAlarmBottom = false;
	contactAlarmLeft = false;
	contactAlarmRight = false;

	// size and init the vector
	detectedFaces.resize(FACEARRAYSIZE);
}


CamThread::~CamThread()
{
	// FIXME: delete SIGSEV!
	//delete imgPtr;
}


void CamThread::stop()
{
	stopped = true;
}


void CamThread::run()
{
	int i=0;
	CvSeq *faces;
	CvRect *faceRectangle;
	CvPoint faceCenter;
	CvPoint rectStart;
	CvPoint rectEnd;
	int faceX = 0;
	int faceY = 0;
	int faceRadius = 0;
	static int lastFaceX = 0;
	static int lastFaceY = 0;
	QTime timestamp;
	int counter = FACEARRAYSIZE - 1;
	CvFont font;
	//CvFont fontSmall;
	QString text;
	// test: show picture with OpenCv -> see also emit at the buttom of this method!
	//cvNamedWindow( "result", 1 );


	// cvInitFont( CvFont* font, int font_face, double hscale, double vscale, double shear=0, int thickness=1, int line_type=8 );
	cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX, 1.0, 1.0, 0, 2);
	//cvInitFont(&fontSmall, CV_FONT_HERSHEY_PLAIN, 0.2, 0.2, 0, 1);


	if (initDone==false)
	{
		// first load haar classifier cascade etc. ...
		init();
	}


	// create a vector with n elements an initialise it with zeroes
	//QVector <KOORD_T> detectedFaces(FACEARRAYSIZE);


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
			// face detection
			//----------------------------------------
	
			//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			if (counter > 0)
			{
				// get the actual date and time
				timestamp = QTime::currentTime();
			
				// store the acutal second
				detectedFaces[counter].x = timestamp.second();
				//qDebug("vector.x 0=%d", detectedFaces.at(0).x);
				
				counter--;
			}
			//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			
			
			// reset actual values for new detection
			faceX = 0;
			faceY = 0;
			faceRadius = 0;
			
			// classifier cascade loaded in the init method
			if ( (faceDetectionIsEnabled) && (cascade) )
			{
				// converts from BGR2 to to gray
				cvCvtColor( imgPtr, gray, CV_BGR2GRAY );
				// resize the gray image, to the size of the small_img (faster!)
				cvResize( gray, small_img, CV_INTER_LINEAR );
				cvEqualizeHist( small_img, small_img );
				
				// clears the storage
				cvClearMemStorage(storage);
	
				// detect objects in the gray image
				faces = cvHaarDetectObjects( small_img, cascade, storage, 1.1, 2, 0, cvSize(30, 30) );
				
				//-----------------
				// faces detected
				//-----------------
				if (faces->total >= 0)
				{
					
					
					// put some information to top left
					//text = QString("Faces: %1").arg(faces->total);
					//cvPutText( imgPtr, text.toAscii(), cvPoint(20, 25), &font, CV_RGB(64, 64, 255) );
					
					// draw a faceRectangle for each face
					for(i=0; i < faces->total; i++)
					{
						// the rect around a face
						faceRectangle = (CvRect*) cvGetSeqElem(faces, i);
						
						// center and radius of the face
						faceCenter.x = qRound((faceRectangle->x + faceRectangle->width*0.5)*scale);
						faceCenter.y = qRound((faceRectangle->y + faceRectangle->height*0.5)*scale);
						faceRadius = qRound((faceRectangle->width + faceRectangle->height)*0.25*scale);
						
/*
						// first check, if the actual detected face is the same face (it is in the same area)
						if 	(
							 	// TODO: set pixel range/area to a percent basis of img size and/or radius
								( (faceCenter.x > lastFaceX - 50) && ((faceCenter.x < lastFaceX + 50)) && (faceCenter.y > lastFaceY - 50) && (faceCenter.y < lastFaceY + 50)  ) ||
								( (faceCenter.x == lastFaceX) && (faceCenter.y == lastFaceY) ) ||
								( (lastFaceY==0) && (lastFaceX==0) ) // <- only first time
							)
*/								
								// the rect coordinates
								rectStart.x = faceRectangle->x*scale;
								rectStart.y = faceRectangle->y*scale;
								rectEnd.x = rectStart.x + faceRectangle->width*scale;
								rectEnd.y = rectStart.y + faceRectangle->height*scale;
								
								// draw rectangles(s)
								// for emit signal TODO: why different emit variables. why not faceCenter.x and y?!?
								faceX = faceCenter.x;
								faceY = faceCenter.y;
								// first face in red
								cvRectangle(imgPtr, rectStart, rectEnd, CV_RGB(255, 128, 128));
					} // for each detected face
				} // face detected
				else
				{
					//--------------------------
					// *no* faces detected
					//--------------------------
					faceX = 0;
					faceY = 0;
					faceRadius = 0;
					lastFaceX = 0;
					lastFaceY = 0;
				}
			
				// show data in the gui and analyse the face data
				emit faceDetected(faces->total, faceX, faceY, faceRadius, lastFaceX, lastFaceY);
				
			} // detection enabled
			
			
			//----------------------------------------
			// draw 'end contact' alarm into image
			//----------------------------------------
			if (contactAlarmLeft)
			{
				// TODO: Use GetTextSize!
				//Retrieves width and height of text string
				//void cvGetTextSize( const char* text_string, const CvFont* font, CvSize* text_size, int* baseline );
				cvPutText( imgPtr, "[Stop]", cvPoint(5, (height/2)-5), &font, CV_RGB(255, 64, 64) );
			}
			
			if (contactAlarmRight)
			{
				cvPutText( imgPtr, "[Stop]", cvPoint(width-100, (height/2)-5), &font, CV_RGB(255, 64, 64) );
			}
			
			if (contactAlarmTop)
			{
				cvPutText( imgPtr, "[Stop]", cvPoint((width/2)-50, 30), &font, CV_RGB(255, 64, 64) );
			}
			
			if (contactAlarmBottom)
			{
				cvPutText( imgPtr, "[Stop]", cvPoint((width/2)-50, height-10), &font, CV_RGB(255, 64, 64) );
			}
			
			// test: show picture with OpenCv
			//cvShowImage( "result", imgPtr );
			
			//====================================================================
			//  e m i t  Signal (e.g. send image and face0 coordinates to GUI)
			//====================================================================
			emit camDataComplete(imgPtr);
		
			// let the thread sleep some time
			//msleep(THREADSLEEPTIME);
			
		} // cameraIsOn
	} // while !stopped
	
	
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


void CamThread::setCascadePath(QString haarClassifierCascade)
{
	if (haarClassifierCascade == "none")
	{
		faceDetectionIsEnabled = false;
		// disable checkBoxes in the GUI
		emit disableFaceDetection();
	}
	
	
	haarClassifierCascadeFilename = haarClassifierCascade;
}


void CamThread::setCameraDevice(int device)
{
	cameraDevice = device;
	
	// on error
	if (cameraDevice == -2)
	{
		stopped = true;
		initDone = true;
		cameraIsOn = false;
	}
}


void CamThread::init()
{
	//if ( (cameraIsOn == true) && (initDone == false) )
	if (initDone == false)
	{
		// do only *one* init!
		initDone = true;

		//-----------------
		// try to capture
		//-----------------
		capture = cvCaptureFromCAM(cameraDevice);
		
		if (!capture)
		{
			qDebug("INFO: could not initialize capturing from /dev/video%d. No camera connected?", cameraDevice);
			width=0;
			height=0;
			pixeldepth = 0;
			cameraIsOn = false;
			stopped = true;
			// disable camera controls in the GUI
			emit disableCamera();
			return;
		}
		
		cameraIsOn = true;
	
		//--------------------------------------------------
		// retrieve an image ang get the size and store it!
		//--------------------------------------------------
		imgPtr = cvRetrieveFrame(capture);
	
		// get width and height & Co.
		width = imgPtr->width;
		height = imgPtr->height;
		pixeldepth = (imgPtr->nChannels * imgPtr->depth);
		
	
		//-----------------------------------------------------
		// Load trained cascade of haar classifers from file.
		// For fast object detection
		//-----------------------------------------------------
		cascade = (CvHaarClassifierCascade*)cvLoad(haarClassifierCascadeFilename.toAscii(), 0, 0, 0);
		
		if(!cascade)
		{
			qDebug() << "ERROR: Could not load classifier cascade for face detection!";
			faceDetectionIsEnabled = false;
			// disable checkBoxes in the GUI
			emit disableFaceDetection();
		}
		else
		{
			//-----------------------------
			// for later face detection:
			//-----------------------------
			// creates an empty memory storage
			storage = cvCreateMemStorage(0);
			
			// create a blank gray scale image with same size
			gray = cvCreateImage( cvSize(width, height), 8, 1 );
			
			// create a blank small image with the same size
			small_img = cvCreateImage( cvSize( cvRound(width/scale), cvRound(height/scale)), 8, 1 );
		}
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


int CamThread::imagePixelDepth()
{
	return pixeldepth;
}


void CamThread::test()
{
	qDebug("test okay. size=%d", detectedFaces.size());
	for (int i = 0; i < detectedFaces.size(); ++i)
	{
		qDebug("second%d = %d", i, detectedFaces.at(i).x);
	}
}
