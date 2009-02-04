#ifndef GUI_H
#define GUI_H

//-------------------------------------------------------------------
// some driving directions for the robot
//
#define	FORWARD				1
#define	BACKWARD			2
#define	LEFT				3
#define	RIGHT				4
#define START				5
#define STOP				6
//-------------------------------------------------------------------

//-------------------------------------------------------------------
#include "ui_gui.h"
#include "direcs-remote.h"
#include "laserScene.h"
#include <QtGui>
#include <QWebView>
#include <QtOpenGL>
#include <QGraphicsScene> // for OpenGL (Laser lines)
//-------------------------------------------------------------------
#include <qwt_plot_layout.h>
#include <qwt_plot_curve.h>
#include <qwt_scale_draw.h>
#include <qwt_scale_widget.h>
#include <qwt_legend.h>
#include <qwt_legend_item.h>
//-------------------------------------------------------------------
/*
#include <Phonon/Global>
#include <Phonon/MediaObject>
#include <Phonon/Path>
#include <Phonon/AudioOutput>
//-------------------------------------------------------------------
*/

// forward declarations because of circular includes!
class DirecsRemote;

class Gui : public QMainWindow
{
	Q_OBJECT

	public:
		Gui(DirecsRemote *m, QMainWindow *parent = 0);
		~Gui();
		
		/**
		Reads the network port for the UDPSocket
		*/
		int getPort();
		
		/**
		Puts text in to a log
		*/
		void appendLog(QString text, bool sayIt=false);

		/**
		Initialise the laser view (find the middle of the now fresh shown control etc.)
		HAS to be called AFTER showing the gui the first time!
		*/
		void initLaserView();
	
	
	public slots:
		void showMotorCurrent(int motor, int value);

		/**
		Shows the actual plot data (e.g. measured current from motor 1). This slot is called from the plot thread.
		@param xval points to an array with the values for the x axis (usually the time line).
		@param yval points to an array with the values for the y axis (usually the measured values).
		@param size is the size of the array.
		@sa PlotThread()
		*/
		void setPlotData1(double *xval, double *yval, int size);

		/**
		Shows the actual plot data (e.g. measured current from motor 2). This slot is called from the plot thread.
		@param xval points to an array with the values for the x axis (usually the time line).
		@param yval points to an array with the values for the y axis (usually the measured values).
		@param size is the size of the array.
		@sa PlotThread()
		*/
		void setPlotData2(double *xval, double *yval, int size);

		/**
		Shows the actual plot data (e.g. measured current from motor 3). This slot is called from the plot thread.
		@param xval points to an array with the values for the x axis (usually the time line).
		@param yval points to an array with the values for the y axis (usually the measured values).
		@param size is the size of the array.
		@sa PlotThread()
		*/
		void setPlotData3(double *xval, double *yval, int size);

		/**
		Shows the actual plot data (e.g. measured current from motor 4). This slot is called from the plot thread.
		@param xval points to an array with the values for the x axis (usually the time line).
		@param yval points to an array with the values for the y axis (usually the measured values).
		@param size is the size of the array.
		@sa PlotThread()
		*/
		void setPlotData4(double *xval, double *yval, int size);

		/**
		Refreshes the view of the lines from the front laser scanner.
		*/
		void refreshLaserViewFront(float *laserScannerValues, int *laserScannerFlags);
		
		/**
		Refreshes the view of the lines from the rear laser scanner.
		*/
		void refreshLaserViewRear(float *laserScannerValues, int *laserScannerFlags);
		
		/**
		Change the robot position in the graphicsView/scene, if the robot is moved via mouse
		*/
		void setRobotPosition(QGraphicsSceneMouseEvent* mouseEvent);
		
		/**
		Zoom into the graphicsView/scene, if the mouse wheel was used.
		*/
		void zoomLaserView(QGraphicsSceneWheelEvent* wheelEvent);

		/**
		Shows the angles of the free area where to drive in lables.
		@param largestFreeAreaStart
		@param largestFreeAreaEnd
		@param centerOfFreeWay
		*/
		void showLaserFrontAngles(int largestFreeAreaStart, int largestFreeAreaEnd, int centerOfFreeWay);

		/**
		Shows the preferred driving direction in a lable.
		@param direction can be FORWARD, BACKWARD, LEFT or RIGHT
		*/
		void showPreferredDirection(QString direction);
	
	
	protected:
		void closeEvent();
	
	
	signals:
		void commandIssued(QString command);
	
	
	private slots:
		void on_btnClear_clicked();
		void on_btnExit_clicked();
		void on_btnTest_clicked();
		void on_lblLink_linkActivated(QString link);
		void on_lblLink_linkHovered(QString link);
		void on_btnForward_clicked();
		void on_btnBackward_clicked();
		void on_btnLeft_clicked();
		void on_btnRight_clicked();
		void on_btnStartStop_clicked();
		void on_btnShutdown_clicked();
		void on_spinBoxSpeed1_valueChanged(int value);
		void on_spinBoxSpeed2_valueChanged(int value);
		void on_btnGo_clicked();
		void on_sliderZoom_valueChanged(int);


	private:
		void initPlots();

		/**
		Creates all objects, lines, scene, view etc.
		*/
		void createLaserScannerObjects();
		void createLaserDistanceObjects();
		
		Ui::Gui ui;
		DirecsRemote *direcsremote1;
		QWebView *webView;
		//MediaObject *media;
		//VideoWidget *vwidget;
		bool robotDrives;
		int speedMotor1;
		int speedMotor2;
		QwtPlotCurve curve1;
		QwtPlotCurve curve2;
		QwtPlotCurve curve3;
		QwtPlotCurve curve4;
		QColor labelFillColorRed;
		QColor labelFillColorGreen;
		QColor labelFillColorBlue;
		QColor colorLaserObstacle;
		QColor colorLaserFreeWay;
		QColor colorLaserPreferredDrivingDirection;
		QColor colorLaserCenterDrivingDirection;
		QColor colorHelpLine;
		QColor colorHelpLineText;
		QColor colorGraphicsSceneBackground;
		QPen laserLinePen;
		int lastZoom;
		qreal startScale;
		qreal laserXPos;
		qreal laserYPos;
		QGraphicsScene *scene;								/** The QGraphicsScene for showing the laser lines in the GUI */
		//QGraphicsScene *cameraScene;						/** The QGraphicsScene for drawing "over" the camera image */
		QList <QGraphicsLineItem*> *laserLineListFront;		/** A pointer to a QList of pointers to the front laser lines (QGraphicsLineItems) */
		QList <QGraphicsLineItem*> *laserLineListRear;		/** A pointer to a QList of pointers to the rear laser lines (QGraphicsLineItems) */
		QList <QGraphicsEllipseItem*> *laserDistanceLineListFront;	/** A pointer to a QList of pointers to the shown distances from the front laser lines (kind of coordinate system) */
		QList <QGraphicsSimpleTextItem*> *laserDistanceTextFront;	/** A pointer to a QList of pointers to the shown distances from the front laser lines (text) */
		QList <QGraphicsEllipseItem*> *laserDistanceLineListRear;	/** A pointer to a QList of pointers to the shown distances from the front laser lines (kind of coordinate system) */
		QList <QGraphicsSimpleTextItem*> *laserDistanceTextRear;	/** A pointer to a QList of pointers to the shown distances from the front laser lines (text) */
		QGraphicsPixmapItem *pixmapBot1;
		QGraphicsPixmapItem *pixmapBot2;
		QGraphicsPixmapItem *scannerFrontSplash;
		QGraphicsPixmapItem *scannerRearSplash;
		QPixmap pixmap; // for IplImageToQImage()
		
		// Give the infrared sensors some names
		//
		// DONT CHANGE THIS NUMBERS!
		// THEY ARE ALSO USED TO ADRESS THE ARRAY "iRSensorValue[]" !!
		//
		static const short int SENSOR1 = 1;
		static const short int SENSOR2 = 2;
		static const short int SENSOR3 = 4;
		static const short int SENSOR4 = 8;
		static const short int SENSOR5 = 16;
		static const short int SENSOR6 = 32;
		static const short int SENSOR7 = 64;
		static const short int SENSOR8 = 128;
		
		// ultrasonic sensor
		static const short int SENSOR16 = 256;
		
		static const short int MOTORSENSOR1 = 0;
		static const short int MOTORSENSOR2 = 1;
		static const short int MOTORSENSOR3 = 2;
		static const short int MOTORSENSOR4 = 3;
		
		/**
		 Give the contacts for the pan tilt cam end positions some nice names
		 */
		static const unsigned char CONTACT1 = 0;
		static const unsigned char CONTACT2 = 1;
		static const unsigned char CONTACT3 = 2;
		static const unsigned char CONTACT4 = 3;
		
		static const float AMPERESMAXPLOTCURVE1 = 3000.0; /// The maximum axis Y value in Amperes (A) for plot curve number 1
		static const float AMPERESMAXPLOTCURVE2 = 3000.0; /// The maximum axis Y value in Amperes (A) for plot curve number 2
		static const float AMPERESSTEPPLOTCURVE1 = 500.0; /// The step value for the y axis for plot curve number 1
		static const float AMPERESSTEPPLOTCURVE2 = 500.0; /// The step value for the y axis for plot curve number 1

		/**
		Factor for fitting 6 meters (measured from the laser scanner) into a frame with a height of 270 pixels at a default zoom level of 5 !!
		Example:

		laservalue = 0.18 cm
		fit factor = 45
		zoom level = 5

		0.18 x 45 x 5 = 40.5 Pixel -> 41 Pixel is the length of the laser line in the GUI
		*/
		static const int FITTOFRAMEFACTOR = 45;
		static const int STARTZOOMLEVEL = 3;

		//! the initial Y position of the laser lines and the robot image in the GUI
		static const int INITIALLASERYPOSFRONT = 100;
		static const int INITIALLASERYPOSREAR  = 100;

		//! Number of distance semi circles
		static const int LASERDISTANCECIRCLES = 27;
		//! the size (diameter) of the first (innerst) circle in pixels
		static const int LASERDISTANCEFIRSTCIRCLE = 82;
		//! Distances between the lines to show the laser distances in pixels
		static const int LASERDISTANCEDISTANCE = 60;

		// the tags for the laser lines
		static const int FREEWAY = 0;
		static const int OBSTACLE = 1;
		static const int LARGESTFREEWAY = 2;
		static const int CENTEROFLARGESTFREEWAY = 3;

		// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		/**
		Defines the size of the laserLines[] array
		This es equal to the number of degrees.
		See also laserThread.h */
		//static const unsigned char LASERSCANNERARRAYSIZE = 181;

		/**
		For the laserThread
		*/
		static const short int LASER1 = 1;
		static const short int LASER2 = 2;
		static const short int LASER3 = 4;
		static const short int LASER4 = 8;
		static const short int LASER5 = 16;
};
#endif
