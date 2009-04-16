/*************************************************************************
 *   Copyright (C) 2009 by Markus Knapp                                  *
 *   www.direcs.de                                                       *
 *                                                                       *
 *   This file is part of direcs.                                        *
 *                                                                       *
 *   direcs is free software: you can redistribute it and/or modify it   *
 *   under the terms of the GNU General Public License as published      *
 *   by the Free Software Foundation, version 3 of the License.          *
 *                                                                       *
 *   direcs is distributed in the hope that it will be useful,           *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of      *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the        *
 *   GNU General Public License for more details.                        *
 *                                                                       *
 *   You should have received a copy of the GNU General Public License   *
 *   along with direcs. If not, see <http://www.gnu.org/licenses/>.      *
 *                                                                       *
 *************************************************************************/

#ifndef GUI_H
#define GUI_H


#ifdef _TTY_POSIX_ // only include in Linux environments, because OpenCV is not available for Windows (and does not make sense for ARM)
	#include "cv.h" // for type IplImage (camThread)
#endif

//-------------------------------------------------------------------
#include <QtGui>
#include <QtOpenGL>
#include <QGraphicsScene> // for OpenGL (Laser lines)
//-------------------------------------------------------------------
#include "joystickDialog.h"
#include "settingsDialog.h"
#include "aboutDialog.h"
#include "laserScene.h"
#include "ui_mainWindow.h"
//-------------------------------------------------------------------
#include <qwt_plot_layout.h>
#include <qwt_plot_curve.h>
#include <qwt_scale_draw.h>
#include <qwt_scale_widget.h>
#include <qwt_legend.h>
#include <qwt_legend_item.h>
//-------------------------------------------------------------------
#include <qwt_compass.h>
#include <qwt_dial_needle.h>
//-------------------------------------------------------------------
#include "QtGLContext.h"



/**
\brief The class for all GUI actions
*/
class Gui : public QMainWindow
{
	Q_OBJECT

	public:
		Gui(SettingsDialog *s, JoystickDialog *j, AboutDialog *a, QMainWindow *parent = 0);
		~Gui();

		/*
		Shows a sensor distance in centimeters (cm) in a text label.
		@param sensor is the sensor number.
		@param distance is the distance in cm.
		void showDistance(int sensor, int distance);
		*/

		/**
		Shows a sensor distance in a progress bar.
		@param sensor is the sensor number.
		@param distance is the distance in cm.
		@sa SensorThread::convertToSensorValue(), SensorThread::convertToDistance()
		*/
		void showDistanceGraphical(int sensor, int distance);

		/**
		Shows the driven distance in a text label.
		@param sensor is the sensor number.
		@param distance is the distance in cm.
		*/
		void showDrivenDistance(int sensor, int distance);

		/**
		Shows a sensor alarm (turns progressBars red, text in bold etc.)
		@param sensor is the sensor number.
		@param state can be ON or OFF.
		*/
		void showAlarm(short int sensor, bool state);

		/**
		Shows the status and direction of a motor (ON, OFF, RIGHT, LEFT)
		@param motor is the motor number.
		@param power can be ON or OFF.
		@param direction can be FORWARD or BACKWARD.
		*/
		void showMotorStatus(unsigned char motor, bool power, unsigned char direction);

#ifdef _TTY_POSIX_ // only include in Linux environments, because OpenCV is not available for Windows (and does not make sense for ARM)
		/**
		tell the OpenGLContext the image data
		*/
		void setCamImageData(int width, int height, int pixeldepth);
#endif

		/**
		Initialise the laser view (find the middle of the now fresh shown control etc.)
		*/
		void initLaserView();


	public slots:
		/**
		Appends text to the main log in the main window.
		@param text is the text to be displayed.
		@param CR adds a carriage return (CR) to the text, if true (default). This parameter is optional!
		@param sayIt If true, the text is also spoken (default=false). This parameter is optional!
		 */
		void appendLog(QString text, bool CR=true, bool sayIt=false);
		/*
		// just a quick and dirty wrapper static method for old CARMEN standard C code!
		static void appLog(QString text);
		*/

#ifdef _TTY_POSIX_ // only include in Linux environments, because OpenCV is not available for Windows (and does not make sense for ARM)
		/**
		Shows the new picture from the cam (live).
		@param frame
		@sa CamThread::camDataComplete()
		*/
		void setCamImage(IplImage* frame);
		//void setCamImage(QImage* image);
#endif

		/**
		Show some face track data in the GUI.
		@param faces is the number of detected faces
		@param faceX is the X coordinate to the middle of a detected face (0, if none)
		@param faceY is the Y coordinate to the middle of a detected face (0, if none)
		@param faceRadius is the radius (0, if none)
		*/
		void showFaceTrackData(int faces, int faceX, int faceY, int faceRadius, int lastFaceX, int lastFaceY);

		/**
		Disables camera controls in the GUI.
		@sa CameraThread::setCameraDevice()
		 */
		void disableCamera();

		/**
		Disables face detection checkBoxes in the GUI.
		@sa CameraThread::setCascadePath()
		 */
		void disableFaceDetection();

		/**
		Enables/Disables some controls in the GUI if the robot, depending on the robots state.
		@param state can be ON or OFF
		@sa CameraThread::robotState()
		 */
		void setRobotControls(bool state);

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
		Append text to the network log in the main window
		@param text is the text to be displayed.
		@param CR adds a carriage return (CR) to the text, if true (default). This parameter is optional!
		@param sayIt If true, the text is also spoken (default=false). This parameter is optional!
		@sa appendLog()
		*/
		void appendNetworkLog(QString text, bool CR=true, bool sayIt=false);

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

		/**
		Shows the face tracking direction in a lable.
		@param direction can be UP, DOWN, LEFT, RIGHT or NONE
		 */
		void showFaceTrackDirection(QString direction);

		/**
		Shows the compass axes values in labels. Also sets the compass needle to the value of the x axis.
		@param x is the x axis value
		@param y is the y axis value
		@param z is the z axis value
		*/
		void showCompassData(float x, float y, float z);


	protected:
		void closeEvent(QCloseEvent *event);


	signals:
		/**
		Exits the whole programm.
		 */
		void shutdown();

		/**
		Initializes the robots basic circuit.
		 */
		void initCircuit();

		/**
		Initializes the servos.
		 */
		void initServos();

		/**
		Instructs the robot to drive FORWARD, BACKWARD, LEFT and RIGHT. The commands START, STOP or WAIT are also possible.
		@param command
		 */
		void drive(const unsigned char command);

		/**
		Resets the driven distance to 0.
		@param sensor is the sensor number.
		*/
		void resetDrivenDistance(int sensor);

		/**
		Enables or disables the listening for the robot remote control. This signal is sent from the remote control button.
		@param state can be true or false.
		@sa NetworkThread()
		*/
		void enableRemoteControlListening(bool state);

		/**
		Enables or disables the robots simulation mode.
		This signal is sent from the simulation button and received from all threads which deliver real signals from the bot.
		Once the signal is received, the slots switches to simulation mode.
		@param state can be true or false.
		@sa Direcs::setSimulationMode()
		@sa SensorThread::setSimulationMode()
		@sa LaserThread::setSimulationMode()
		@sa ObstackeCheckThread::setSimulationMode()
		*/
		void simulate(bool state);

		/**
		Enables or disables the face detection. When activated, a circle for each face is drawn on the camera live image.
		@param state has to be Qt::Checked to enable the detection. All other states disable.
		*/
		void enableFaceDetection(int state);

		/**
		Enables or disables the face tracking. When activated, a face will be tracked by the camera.
		@param state has to be Qt::Checked to enable the detection. All other states disable.
		*/
		void enableFaceTracking(int state);

		/**
		Emits a speak signal. This signal is sent to the speakThread.
		*/
		void speak(QString text);

		/**
		Emits a signal to call the test method in the Direcs class..
		 */
		void test();


	protected slots:
		/**
		Action if a menu item or a toolbar button is clicked
		*/
		void on_actionExit_activated();
		void on_actionCamera_activated();
		void on_actionOdometrie_activated();
		void on_actionPower_activated();
		void on_actionSettings_activated();
		void on_actionLog_activated();
		void on_actionJoystick_activated();
		void on_actionAll_activated();
		void on_actionAbout_activated();
		void on_actionDrive_activated();
		void on_actionReset_activated();
		void on_actionTest_activated();
		void on_actionRemote_activated();
		void on_actionSimulate_activated();


	private slots:
		void on_btnResetMovement1_clicked();
		void on_btnResetMovement2_clicked();
		//void on_ckeckBoxSaveSettings_stateChanged(int state);
		void on_btnSavePicture_clicked();
		void on_sliderZoom_valueChanged(int);
		void on_checkBoxMirror_stateChanged(int);
		void on_checkBoxFaceDetection_stateChanged(int);
		void on_checkBoxFaceTracking_stateChanged(int);
		void on_radioBtnMotor1FW_clicked();
		void on_radioBtnMotor1BW_clicked();
		void on_radioBtnMotor1OFF_clicked();
		void on_radioBtnMotor2FW_clicked();
		void on_radioBtnMotor2BW_clicked();
		void on_radioBtnMotor2OFF_clicked();
		void on_radioBtnMotor3FW_clicked();
		void on_radioBtnMotor3BW_clicked();
		void on_radioBtnMotor3OFF_clicked();
		void on_radioBtnMotor4FW_clicked();
		void on_radioBtnMotor4BW_clicked();
		void on_radioBtnMotor4OFF_clicked();

		/**
		Saves the current picture to disk (one time shot).
		*/
		void saveCamImage(void);


	private:
		void initPlots();
		void initCompass();

		/**
		Creates all objects, lines, scene, view etc.
		*/
		void createLaserScannerObjects();
		void createLaserDistanceObjects();

		bool robotIsOn; //! Stores the robots (circuits) state.ON or OFF
		Ui::mainWindow ui;
		SettingsDialog *settingsDialog; // just a pointer to the object created in the direcs class!
		JoystickDialog *joystickDialog; // just a pointer to the object created in the direcs class!
		AboutDialog *aboutDialog;       // just a pointer to the object created in the direcs class!
		QwtPlotCurve curve1;
		QwtPlotCurve curve2;
		QwtPlotCurve curve3;
		QwtPlotCurve curve4;
		QwtCompassMagnetNeedle needle;
		QPixmap cameraPicToSave;
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
		QPixmap pixmap; // for IplImageToQImage()


		static const int SENSORPROGRESSBARMAXIR = 50; /** max value in cm for ir sensor */
		static const int SENSORPROGRESSBARMAXUS = 400; /** max value in cm for us sensor */
		
		static const float AMPERESMAXPLOTCURVE1 = 3000.0; /// The maximum axis Y value in Amperes (A) for plot curve number 1
		static const float AMPERESMAXPLOTCURVE2 = 3000.0; /// The maximum axis Y value in Amperes (A) for plot curve number 2
		static const float AMPERESSTEPPLOTCURVE1 = 500.0; /// The step value for the y axis for plot curve number 1
		static const float AMPERESSTEPPLOTCURVE2 = 500.0; /// The step value for the y axis for plot curve number 1

		static const bool ON  = true;   /** For motor "ON" */
		static const bool OFF = false;  /** For motor "OFF" */

		/// @sa Direcs::drive() [Slot]
		static const unsigned char START = 7; //! For button signal "start driving"
		static const unsigned char STOP  = 8; //! For button signal "stop driving"
		static const unsigned char MOTOR1FW		= 10; /// for the test widget
		static const unsigned char MOTOR1BW		= 11; /// for the test widget
		static const unsigned char MOTOR1OFF	= 12; /// for the test widget
		static const unsigned char MOTOR2FW		= 13; /// for the test widget
		static const unsigned char MOTOR2BW		= 14; /// for the test widget
		static const unsigned char MOTOR2OFF	= 15; /// for the test widget
		static const unsigned char MOTOR3FW		= 16; /// for the test widget
		static const unsigned char MOTOR3BW		= 17; /// for the test widget
		static const unsigned char MOTOR3OFF	= 18; /// for the test widget
		static const unsigned char MOTOR4FW		= 19; /// for the test widget
		static const unsigned char MOTOR4BW		= 20; /// for the test widget
		static const unsigned char MOTOR4OFF	= 21; /// for the test widget

		static const unsigned char CLOCKWISE        = 0;  /// Motor direction "CLOCKWISE"
		static const unsigned char COUNTERCLOCKWISE = 1;  /// Motor direction "COUNTERCLOCKWISE
		static const unsigned char SAME             = 3;  /// Motor direction/power "same like before"
		static const unsigned char MOTOR1           = 10; /// Motor 1 front left
		static const unsigned char MOTOR2           = 20; /// Motor 2 front right
		static const unsigned char MOTOR3           = 30; /// Motor 3 back left
		static const unsigned char MOTOR4           = 40; /// Motor 4 back right

		/**
		Give the sensors some names

		DONT CHANGE THIS NUMBERS!
		THEY ARE ALSO USED TO ADRESS THE ARRAY "iRSensorValue[]" !!
		*/
		static const short int SENSOR1 = 1;
		static const short int SENSOR2 = 2;
		static const short int SENSOR3 = 4;
		static const short int SENSOR4 = 8;
		static const short int SENSOR5 = 16;
		static const short int SENSOR6 = 32;
		static const short int SENSOR7 = 64;
		static const short int SENSOR8 = 128;

		/** ultrasonic sensor */
		static const short int SENSOR16 = 256;
		/** Value if no sensor has a value to react */
		static const short int NONE = 0;

		static const short int MOTORSENSOR1 = 0;
		static const short int MOTORSENSOR2 = 1;

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
