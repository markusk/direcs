/*************************************************************************
 *   Copyright (C) 2011 by Markus Knapp                                  *
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

//-------------------------------------------------------------------
#include <QtGlobal> // for Q_OS_* Makro!
#include <QtGui>
#include <QtOpenGL>

#ifdef ACTIVELASERVIEW
	#include <QGraphicsScene> // for OpenGL (Laser lines)
#endif

//-------------------------------------------------------------------
#include "joystickDialog.h"
#include "settingsDialog.h"
#include "aboutDialog.h"

#ifdef ACTIVELASERVIEW
	#include "laserScene.h"
#endif

#include "compassWidget.h"


#include "ui_direcs.h"

#ifndef BUILDFORROBOT
	#include <qwt_plot_layout.h>
	#include <qwt_plot_curve.h>
	#include <qwt_plot_grid.h>
	#include <qwt_scale_draw.h>
	#include <qwt_scale_widget.h>
	#include <qwt_legend.h>
	#include <qwt_legend_item.h>
	// #include <qwt_compass.h>
	#include <qwt_dial_needle.h>
#endif


/**
\author Markus Knapp
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
		Shows the voltage from the power supply in a text label.
		@param sensor is the sensor number.
		@param voltage is the voltage in V.
		*/
		void showVoltage(int sensor, float voltage);

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

#ifdef ACTIVELASERVIEW
		/**
		Initialise the laser view (find the middle of the now fresh shown control etc.)
		*/
		void initLaserView();

		/**
		This creates all the pixmaps, laser objects etc.
		Has to be called AFTER the laserType and angles are known (read from an ini-file) or so.
		*/
		void initLaserStuff();
#endif


	public slots:
		/**
		Appends text to the main log in the main window.
		@param text is the text to be displayed.
		@param CR adds a carriage return (CR) to the text, if true (default). This parameter is optional.
		@param sayIt If true, the text is also spoken (default=false). This parameter is optional.
		@param addTimestamp If true, the a timestamp is added in front of the text. This parameter is optional.
		 */
		void appendLog(QString text, bool CR=true, bool sayIt=false, bool addTimestamp=true);

		/**
		Append text to the network log in the main window
		@param text is the text to be displayed.
		@param CR adds a carriage return (CR) to the text, if true (default). This parameter is optional!
		@param sayIt If true, the text is also spoken (default=false). This parameter is optional!
		@sa appendLog()
		*/
		void appendNetworkLog(QString text, bool CR=true, bool sayIt=false);

		/**
		Append text to the serial log in the main window
		@param text is the text to be displayed.
		@param CR adds a carriage return (CR) to the text, if true (default). This parameter is optional!
		*/
		void appendSerialLog(QString text, bool CR=true);

		/**
		Shows the new RGB picture from the Kienct camera.
		@param frame
		@sa CamThread::camDataComplete()
		*/
		void setCamImage(QImage* image);

		/**
		Shows the new depth picture from the Kienct camera.
		@param frame
		@sa CamThread::camDepthComplete()
		*/
		void setCamImageDepth(QImage* image);

		/**
		Shows the OpenCV processed picture from the Kienct camera.
		@param frame
		@sa CamThread::camOpenCVComplete()
		*/
		void setCamImageOpenCV(QImage* image);

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
		Disables the compass window in the GUI.
		 */
		void disableCompass();

		/**
		Enables/Disables some controls in the GUI if the robot, depending on the robots state.
		@param state can be ON or OFF
		@sa CameraThread::robotState()
		 */
		void setRobotControls(bool state);

/*
#ifndef BUILDFORROBOT
		/ **
		Shows the actual plot data (e.g. measured current from motor 1). This slot is called from the plot thread.
		@param xval points to an array with the values for the x axis (usually the time line).
		@param yval points to an array with the values for the y axis (usually the measured values).
		@param size is the size of the array.
		@sa PlotThread()
		* /
		void setPlotData1(double *xval, double *yval, int size);
#endif

#ifndef BUILDFORROBOT
		/ **
		Shows the actual plot data (e.g. measured current from motor 2). This slot is called from the plot thread.
		@param xval points to an array with the values for the x axis (usually the time line).
		@param yval points to an array with the values for the y axis (usually the measured values).
		@param size is the size of the array.
		@sa PlotThread()
		* /
		void setPlotData2(double *xval, double *yval, int size);
#endif

#ifndef BUILDFORROBOT
		/ **
		Shows the actual plot data (e.g. measured current from motor 3). This slot is called from the plot thread.
		@param xval points to an array with the values for the x axis (usually the time line).
		@param yval points to an array with the values for the y axis (usually the measured values).
		@param size is the size of the array.
		@sa PlotThread()
		* /
		void setPlotData3(double *xval, double *yval, int size);
#endif

#ifndef BUILDFORROBOT
		/ **
		Shows the actual plot data (e.g. measured current from motor 4). This slot is called from the plot thread.
		@param xval points to an array with the values for the x axis (usually the time line).
		@param yval points to an array with the values for the y axis (usually the measured values).
		@param size is the size of the array.
		@sa PlotThread()
		* /
		void setPlotData4(double *xval, double *yval, int size);
#endif
*/

#ifndef BUILDFORROBOT
		/**
		Shows the actual plot data (e.g. measured voltage from sensor 1). This slot is called from the plot thread.
		@param xval points to an array with the values for the x axis (usually the time line).
		@param yval points to an array with the values for the y axis (usually the measured values).
		@param size is the size of the array.
		@sa PlotThread()
		*/
		void setPlotData5(double *xval, double *yval, int size);
#endif

#ifndef BUILDFORROBOT
		/**
		Shows the actual plot data (e.g. measured voltage from sensor 2). This slot is called from the plot thread.
		@param xval points to an array with the values for the x axis (usually the time line).
		@param yval points to an array with the values for the y axis (usually the measured values).
		@param size is the size of the array.
		@sa PlotThread()
		*/
		void setPlotData6(double *xval, double *yval, int size);
#endif

#ifndef BUILDFORROBOT
		/**
		Shows the actual plot data (heartbeat from the robot). This slot is called from the plot thread.
		@param xval points to an array with the values for the x axis (usually the time line).
		@param yval points to an array with the values for the y axis (usually the measured values).
		@param size is the size of the array.
		@sa PlotThread()
		*/
		void setPlotDataHeartbeat(double *xval, double *yval, int size);
#endif

#ifdef ACTIVELASERVIEW
		/**
		Refreshes the view of the lines from the front laser scanner.
		*/
		void refreshLaserViewFront(QList <float> laserScannerValues, QList <int> laserScannerFlags);

		/**
		Refreshes the view of the lines from the rear laser scanner.
		*/
		void refreshLaserViewRear(QList <float> laserScannerValues, QList <int> laserScannerFlags);

		/**
		Change the robot position in the graphicsView/scene, if the robot is moved via mouse
		*/
		void setRobotPosition(QGraphicsSceneMouseEvent* mouseEvent);
#endif

		/**
		Gets the angle of a laser from the @sa laserThread for drawing the correct laser scanner angles (e.g. 180 or 270 degrees)
		@param laserScanner can be LASER1 or LASER2
		@param angle in degrees
		*/
		void setLaserscannerAngle(short int laserscanner, int angle);

		/**
		Gets the resolution of a laser from the @sa laserThread for drawing the correct laser scanner angles (e.g. 0.5 or 1.0 degrees)
		@param laserScanner can be LASER1 or LASER2
		@param angle in degrees
		*/
		void setLaserscannerResolution(short int laserscanner, float resolution);

#ifdef ACTIVELASERVIEW
		/**
		Zoom into the graphicsView/scene, if the mouse wheel was used.
		*/
		void zoomLaserView(QGraphicsSceneWheelEvent* wheelEvent);
#endif

		/**
		Shows the angles of the free area where to drive in lables.
		@param largestFreeAreaStart
		@param largestFreeAreaEnd
		@param centerOfFreeWay
		@param width of the estimated drive-trough area
		*/
		void showLaserFrontAngles(int largestFreeAreaStart, int largestFreeAreaEnd, int centerOfFreeWay, float width);

		/**
		Shows the preferred driving direction in a lable.
		@param direction can be FORWARD, BACKWARD, LEFT or RIGHT
		@sa Direcs::logicalUnit
		*/
		void showPreferredDirection(QString direction);

		/**
		Shows the face tracking direction in a lable.
		@param direction can be UP, DOWN, LEFT, RIGHT or NONE
		 */
		void showFaceTrackDirection(QString direction);

		/**
		Shows the compass axes values in labels. Also sets the compass needle to the value of the x axis and sets the 3D OpenGL compass!
		@param x is the x axis value
		@param y is the y axis value
		@param z is the z axis value
		@param heading is the heading of the compass
		*/
		void showCompassData(float x, float y, float z, float heading);

		/**
		Turns the heartbeat/state GUI LED red, green or off.
		@param state can be RED, GREEN or OFF.
		*/
		void setLEDHeartbeat(unsigned char state);

		/**
		Turns the GUI compass LED red, green or off.
		@param state can be RED, GREEN or OFF.
		*/
		void setLEDCompass(unsigned char state);

		/**
		Turns the joystick GUI LED red, green or off.
		@param state can be RED, GREEN or OFF.
		*/
		void setLEDJoystick(unsigned char state);

		/**
		Turns the camera GUI LED red, green or off.
		@param state can be RED, GREEN or OFF.
		*/
		void setLEDCamera(unsigned char state);

		/**
		Turns the network GUI LED red, green or off.
		@param state can be RED, GREEN or OFF.
		*/
		void setLEDNetwork(unsigned char state);

		/**
		Turns the lase scanner GUI LED red, green or off.
		@param state can be RED, GREEN or OFF.
		*/
		void setLEDLaser(unsigned char state);

		/**
		Turns the CONSOLE mode on or off.
		If enabled, all log messages are passed to the console.
		@param state can be true or false.
		*/
		void setConsoleMode(bool state);

		/**
		This slot catches all signals from the signal @sa systemerror
		*/
		void systemerrorcatcher(int errorlevel);

		/**
		Shows the angle of the Kinect
		@param angle is the angle in degrees
		*/
		void showKinectAngle(double angle);


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

		/**
		 */
		void setKinectVideoMode(int mode);

		/**
		 */
		void setKinectAngle(double i);

		/**
		 */
		void resetKinectAngle();


	protected slots:
		/**
		Action if a menu item or a toolbar button is clicked
		*/
		void on_actionExit_activated();
		void on_actionCamera_activated();
		void on_actionVoltage_activated();
//		void on_actionCurrent_activated();
		void on_actionState_activated();
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
		void on_actionResetDrivenDistance_activated();
/*
		void on_btnResetMovement1_clicked();
		void on_btnResetMovement2_clicked();
		void on_btnResetMovement3_clicked();
		void on_btnResetMovement4_clicked();
*/
		//void on_ckeckBoxSaveSettings_stateChanged(int state);
		void on_btnSavePicture_clicked();

#ifdef ACTIVELASERVIEW
		void on_sliderZoom_valueChanged(int);
#endif

		void on_checkBoxMirror_stateChanged(int);
		void on_checkBoxFaceDetection_stateChanged(int);
		void on_checkBoxFaceTracking_stateChanged(int);
/*
		void on_btnPower1_toggled(bool);
		void on_btnPower2_toggled(bool);
		void on_btnPower3_toggled(bool);
		void on_btnPower4_toggled(bool);
		void on_btnDirection1_toggled(bool);
		void on_btnDirection2_toggled(bool);
		void on_btnDirection3_toggled(bool);
		void on_btnDirection4_toggled(bool);
*/
//		void on_btnKinectVideoRGB_clicked(bool checked);
//		void on_btnKinectVideoYUVRGB_clicked(bool checked);
//		void on_btnKinectVideoIR8Bit_clicked(bool checked);
		void on_spinBoxKinectAngle_valueChanged(int i);
		void on_btnKinectResetAngle_clicked();

		/**
		Saves the current picture to disk (one time shot).
		*/
		void saveCamImage(void);

		void processCam();


	private:
		void init();
#ifndef BUILDFORROBOT
		void initPlots();
#endif
		void initCompassView();

#ifdef ACTIVELASERVIEW
		/**
		Creates all objects, lines, scene, view etc.
		*/
		void createLaserScannerObjects();
		void createLaserDistanceObjects();
		void createLaserWidthObjects();
#endif

		/**
		Removes HTML code from a given string.
		@param text is the string to be cleaned
		@return the cleaned string
		*/
		QString removeHtml(QString text);

		bool robotIsOn; //! Stores the robots (circuits) state.ON or OFF
		Ui::mainWindow ui;
		SettingsDialog *settingsDialog; // just a pointer to the object created in the direcs class!
		JoystickDialog *joystickDialog; // just a pointer to the object created in the direcs class!
		AboutDialog *aboutDialog;       // just a pointer to the object created in the direcs class!
#ifndef BUILDFORROBOT
		QwtPlot qwtPlotVoltage1; // this plot object will be added to the GUI layout if we don't build on the robot directly!
		QwtPlot qwtPlotVoltage2; // this plot object will be added to the GUI layout if we don't build on the robot directly!
		QwtPlot qwtPlotCurrent1; // this plot object will be added to the GUI layout if we don't build on the robot directly!
		QwtPlot qwtPlotCurrent2; // this plot object will be added to the GUI layout if we don't build on the robot directly!
		QwtPlot qwtPlotHeartbeat; // this plot object will be added to the GUI layout if we don't build on the robot directly!
		QwtPlotCurve curveVoltage1;
		QwtPlotCurve curveVoltage2;
		QwtPlotCurve curveCurrent1;
		QwtPlotCurve curveCurrent2;
		QwtPlotCurve curveCurrent3;
		QwtPlotCurve curveCurrent4;
		QwtPlotCurve curveHeartbeat;
		QwtPlotGrid gridVoltage1;
		QwtPlotGrid gridVoltage2;
		QwtPlotGrid gridCurrent1;
		QwtPlotGrid gridCurrent2;
		QwtPlotGrid gridHeartbeat;
// 		QwtCompassMagnetNeedle needle;
#endif
		QPixmap cameraPicToSave;

		QColor labelFillColorRed;
		QColor labelFillColorGreen;
		QColor labelFillColorBlue;
		QColor gridColor;
		QColor colorLaserObstacle;
		QColor colorLaserFreeWay;
		QColor colorLaserPreferredDrivingDirection;
		QColor colorLaserCenterDrivingDirection;
		QColor colorHelpLine;
		QColor colorHelpLineText;
		QColor colorGraphicsSceneBackground;
		int laserscannerAngleFront; /// this values holds the range of the laserscanner viewing angle. e.g. 180 oder 270 degrees.  just for reading the value here. @sa laserThread
		int laserscannerAngleRear; /// this values holds the range of the laserscanner viewing angle. e.g. 180 oder 270 degrees.  just for reading the value here. @sa laserThread
		float laserscannerResolutionFront; /// this values holds the resolution of the laserscanner. @sa laserThread
		float laserscannerResolutionRear; /// this values holds the resolution of the laserscanner. @sa laserThread
		int mLargestFreeAreaStartFront; /// this stores the free area angle from the front laser. Got it from @sa showLaserFrontAngles()
		int mLargestFreeAreaEndFront;   /// this stores the free area angle from the front laser. Got it from @sa showLaserFrontAngles()
		int mLargestFreeAreaStartRear;  /// this stores the free area angle from the rear  laser. Got it from @sa showLaserFrontAngles()
		int mLargestFreeAreaEndRear;    /// this stores the free area angle from the rear  laser. Got it from @sa showLaserFrontAngles()

#ifdef ACTIVELASERVIEW
		QPen laserLinePen;
		int lastZoom;
		qreal startScale;
		qreal laserXPos;
		qreal laserYPos;
		QGraphicsScene *scene;								/** The QGraphicsScene for showing the laser lines in the GUI */
		QList <QGraphicsLineItem*> *laserLineListFront;		/** A pointer to a QList of pointers to the front laser lines (QGraphicsLineItems) */
		QList <QGraphicsLineItem*> *laserLineListRear;		/** A pointer to a QList of pointers to the rear laser lines (QGraphicsLineItems) */
		QList <QGraphicsEllipseItem*> *laserDistanceLineListFront;	/** A pointer to a QList of pointers to the shown distances from the front laser lines (kind of coordinate system) */
		QList <QGraphicsSimpleTextItem*> *laserDistanceTextFront;	/** A pointer to a QList of pointers to the shown distances from the front laser lines (text) */
		QList <QGraphicsEllipseItem*> *laserDistanceLineListRear;	/** A pointer to a QList of pointers to the shown distances from the front laser lines (kind of coordinate system) */
		QList <QGraphicsSimpleTextItem*> *laserDistanceTextRear;	/** A pointer to a QList of pointers to the shown distances from the front laser lines (text) */
		QGraphicsPixmapItem *pixmapBot1;
		QGraphicsPixmapItem *pixmapBot2;
		QGraphicsEllipseItem *widthLeftCircleFront;  /// shows the width for the robots drive-trough for the front laser
		QGraphicsEllipseItem *widthRightCircleFront; /// shows the width for the robots drive-trough for the front laser
		QGraphicsLineItem *widthLineFront;           /// shows the width for the robots drive-trough for the front laser
		QGraphicsSimpleTextItem *widthTextFront;     /// shows the width for the robots drive-trough for the front laser
		QGraphicsEllipseItem *widthLeftCircleRear;   /// shows the width for the robots drive-trough for the rear laser
		QGraphicsEllipseItem *widthRightCircleRear;  /// shows the width for the robots drive-trough for the rear laser
		QGraphicsLineItem *widthLineRear;            /// shows the width for the robots drive-trough for the rear laser
		QGraphicsSimpleTextItem *widthTextRear;      /// shows the width for the robots drive-trough for the rear laser
		qreal widthCirclesWidth; /// the width of the robots drive-tru circles
#endif

		QPixmap pixmap; // for IplImageToQImage()

		CompassWidget *compassWidget;							/// The 3D OpenGL compass widget
		bool consoleMode; /// is enabled if the argument 'console' was passed by command-line. Sends all GUI messages to the command line.
		QDateTime now; /// this is for the timestamp in the logs in the gui
/*
		cv::Mat frame;
*/

		static const int SENSORPROGRESSBARMAXIR = 50; /** max value in cm for ir sensor */
		static const int SENSORPROGRESSBARMAXUS = 400; /** max value in cm for us sensor */

//		static const float AMPERESMAXPLOTCURVE1 = 3000.0; /// The maximum axis Y value in Amperes (A) for plot curve number 1
//		static const float AMPERESMAXPLOTCURVE2 = 3000.0; /// The maximum axis Y value in Amperes (A) for plot curve number 2
//		static const float AMPERESSTEPPLOTCURVE1 = 500.0; /// The step value for the y axis for plot curve number 1
//		static const float AMPERESSTEPPLOTCURVE2 = 500.0; /// The step value for the y axis for plot curve number 1

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
		static const short int MOTORSENSOR3 = 2;
		static const short int MOTORSENSOR4 = 3;

		static const short int VOLTAGESENSOR1 = 0;
		static const short int VOLTAGESENSOR2 = 1;

		static const float MINIMUMVOLTAGE1 = 10.80; /// \todo put min. voltage 1 in the ini-file
		static const float MINIMUMVOLTAGE2 = 21.60; // \todo put min. voltage 2 in the ini-file

		static const float MAXIMUMVOLTAGE1 = 12.00; /// for progress bars \todo put max. voltage 1 in the ini-file
		static const float MAXIMUMVOLTAGE2 = 24.00; /// for progress bars \todo put max. voltage 2 in the ini-file

		static const float MAXIMUMPLOTVOLTAGE1 = 13.00; /// for plot widgets \todo put to ini-file
		static const float MAXIMUMPLOTVOLTAGE2 = 26.00; /// for plot widgets \todo put to ini-file

		static const float MAXIMUMPLOTHEARTBEAT = 5.00; /// theoratically 5 Volt for heartbeat "high" @sa SensorThread::run

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
		//! This is the difference in pixel measured drom the absolute center within the robot pixmap!
		static const int INITIALLASERYPOSFRONT = 60.0; // FIXME: when values are different, the distance line do not fit on the lsaer lines in on_sliderZoom_valueChanged
		static const int INITIALLASERYPOSREAR  = 60.0; // FIXME: when values are different, the distance line do not fit on the lsaer lines in on_sliderZoom_valueChanged

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

		/**
		The GUI LED colors
		*/
		static const unsigned char LEDOFF	= 0;
		static const unsigned char RED		= 1;
		static const unsigned char GREEN	= 2;
};
#endif
