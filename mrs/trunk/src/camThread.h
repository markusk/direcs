#ifndef CAMTHREAD_H
#define CAMTHREAD_H

//-------------------------------------------------------------------
#include <QImage>
#include <QtGui> // for QMessage
#include <QTime>
#include <QString>
#include <QtDebug> // for a more convenient use of qDebug
//-------------------------------------------------------------------
#include "cv.h"
#include "highgui.h"
#include <stdio.h>
#include <ctype.h>
//-------------------------------------------------------------------

typedef struct
{
	int x;
	int y;
	int radius;
	int rank;
} KOORD_T;


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
		
		/**
		*/
		void init();

		/**
		*/
		bool isConnected(void);
		
		/**
		Sets the path and filename to the haar classifier cascade.
		@param haarClassifierCascade is the whole filename
		 */
		void setCascadePath(QString haarClassifierCascade);
		
		/**
		Sets the camera device.
		@param device is an integer value! 0 for the first vide device (e.g. /dev/video0). -2 if an error occured.
		 */
		void setCameraDevice(int device);

		/**
		Returns the image height of the camera. Retrieved in the constructor!
		*/
		int imageHeight();

		/**
		Returns the image width of the camera. Retrieved in the constructor!
		*/
		int imageWidth();

		/**
		Returns the image pixel depth of the camera. Retrieved in the constructor!
		*/
		int imagePixelDepth();
		
		void stop();
		virtual void run();

		
	public slots:
		/**
		Enables or disables the face detection. When activated, a circle for each face is drawn on the camera live image.
		@param state has to be Qt::Checked to enable the detection. All other states disable.
		*/
		void enableFaceDetection(int state);
		
		/**
		Draws a red object in the camera image, when the camera hits the end switches (when panning and tilting).
		@param position can be TOP, BOTTOM, LEFT, RIGHT
		@param state can be true or false (for ON and OFF)disableFaceDetection
		@sa Gui::showContactAlarm()
		 */
		void drawContactAlarm(char position, bool state);
		
		/**
		*/
		void test();


	
	signals:
		/**
		@param *imgPtr is a pointer to the camera image
		@sa Gui::setCamImage()
		*/
		void camDataComplete(IplImage* imgPtr);
		
		/**
		Disables checkBoxes in the GUI
		*/
		void disableFaceDetection();
		
		/**
		Disables camera controls in the GUI
		 */
		void disableCamera();


		/**
		TODO: text text text
		@param faceX is the X coordinate to the middle of a detected face (0, if none)
		@param faceY is the Y coordinate to the middle of a detected face (0, if none)
		@param faceRadius is the radius (0, if none)
		@sa Mrs::faceTracking()
		*/
		void faceDetected(int faces, int faceX, int faceY, int faceRadius, int lastFaceX, int lastFaceY);


	private:
		bool initDone;
		bool cameraIsOn;
		bool faceDetectionIsEnabled;
		bool faceDetectionWasActive;
		QVector <KOORD_T> detectedFaces; /** the coordinates of the last n detected faces */
		int cameraDevice;
		QString haarClassifierCascadeFilename;
		CvScalar hsv2rgb( float hue );
		IplImage *imgPtr;
		int width;
		int height;
		int pixeldepth;
		CvCapture *capture;
		CvMemStorage *storage;
		CvHaarClassifierCascade *cascade;
		IplImage *gray;
		IplImage *small_img;
		bool contactAlarmTop;
		bool contactAlarmBottom;
		bool contactAlarmLeft;
		bool contactAlarmRight;
		volatile bool stopped;
		
		// Every thread sleeps some time, for having a bit more time for the other threads!
		// Time in milliseconds
		//static const unsigned long THREADSLEEPTIME = 100; // Default: 100 ms
		
		static const double scale = 1.7; // 1.3 is okay for 640*480 images, 1.8 for 640*480.
		
		// The position for the contact alarm in the camera image
		static const char LEFT   = 0;
		static const char RIGHT  = 1;
		static const char TOP    = 2;
		static const char BOTTOM = 3;
		
		static const int FACEARRAYSIZE = 24;
};

#endif
