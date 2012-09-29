/*************************************************************************
 *   Copyright (C) Markus Knapp                                          *
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
#include <QGraphicsScene> // for OpenGL (Laser lines)
//-------------------------------------------------------------------

#include "ui_guitest.h"
#include "laserScene.h"


/**
\author Markus Knapp
\brief The class for all GUI actions
*/
class Gui : public QMainWindow
{
	Q_OBJECT

	public:
		Gui(bool useSmallGUI, QMainWindow *parent = 0);
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
		void showMotorStatus(int motor, bool power, int direction);

		/**
		Initialise the laser view (find the middle of the now fresh shown control etc.)
		*/
		void initLaserView();

		/**
		This creates all the pixmaps, laser objects etc.
		Has to be called AFTER the laserType and angles are known (read from an ini-file) or so.
		*/
		void initLaserStuff();


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
		Enables/Disables some controls in the GUI if the robot, depending on the robots state.
		@param state can be ON or OFF
		@sa CameraThread::robotState()
		 */
		void setRobotControls(bool state);

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

		/**
		Zoom into the graphicsView/scene, if the mouse wheel was used.
		*/
		void zoomLaserView(QGraphicsSceneWheelEvent* wheelEvent);

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
		Turns the network master/slave GUI LED red, green or off.
		@param state can be RED, GREEN or OFF.
		*/
		void setLEDMasterSlave(unsigned char state);

		/**
		Turns the lase scanner GUI LED red, green or off.
		@param state can be RED, GREEN or OFF.
		*/
		void setLEDLaser(unsigned char state);

		/**
		Changes the text of the netowrk master/slave label
		@param text is the etxt to be shown (normally 'Master' or 'Slave)
		*/
		void setLabelMasterSlave(QString text);

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
		void drive(const int command);

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
		Saves the current laser data to a file. This signal is sent to the @sa LaserThread::writeLaserData via @sa Direcs
		*/
		void writeLaserData();

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

		/**
		  Sets the threshold on the OpenCV iamge stuff. This Signal is sent to the @sa CamThread
		 */
		void setThreshold(int value);

		/**
		Emits the demo mode signal
		@param state can be true or false.
		*/
		void demo(bool state);

		/**
		Emits a mediaplayer skip (track) signal
		*/
		void mediaSkip();


	protected slots:
		/**
		Action if a menu item or a toolbar button is clicked
		*/
		void on_actionExit_activated();
		void on_actionSettings_activated();
		void on_actionJoystick_activated();
#ifndef USEROBOTGUI
		void on_actionAll_activated();
		void on_actionCamera_activated();
		void on_actionVoltage_activated();
		//		void on_actionCurrent_activated();
		void on_actionState_activated();
		void on_actionLog_activated();
#endif
		void on_actionAbout_activated();
		void on_actionDrive_activated();
		void on_actionReset_activated();
		void on_actionTest_activated();
		void on_actionRemote_activated();
		void on_actionSimulate_activated();
		void on_actionSaveLaserData_activated();
		void on_actionDemo_activated();
		void on_actionMediaSkip_activated();


	private slots:
		void on_actionResetDrivenDistance_activated();

		void on_sliderZoom_valueChanged(int);


	private:
		void init();

		/**
		Creates all laser lines, pixmaps, scene, view for the laser scanner view.
		*/
		void createLaserScannerObjects();

		/**
		Creates the semi circles showing the distances around the robot in the laser scanner view.
		*/
		void createLaserDistanceObjects();

		/**
		Creates the graphical objects to show the drive through width for the robot in the laser scanner view.
		*/
		void createLaserWidthObjects();

		/**
		Removes HTML code from a given string.
		@param text is the string to be cleaned
		@return the cleaned string
		*/
		QString removeHtml(QString text); /// @todo fix me to leave the last char in the string. @sa Logfile

		bool robotIsOn; //! Stores the robots (circuits) state.ON or OFF
		bool useLargeGUI; /// if true, we will use the large GUI. If false, we will use the small one for fitting on a small onboard TFT. By default this is decided by the local hostname of the system. @sa Direcs
		Ui::mainWindow ui; /// This is the large default GUI, when not running on the robot

		QColor labelFillColorRed;
		QColor labelFillColorGreen;
		QColor labelFillColorBlue;
		QColor gridColor;
		QColor colorLaserObstacle;
		QColor colorLaserFreeWay;
		QColor colorLaserPreferredDrivingDirection;
		QColor colorLaserCenterDrivingDirection;
		QColor colorLaserIgnoreArea;
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

		QPixmap pixmap; // for IplImageToQImage()

		bool consoleMode; /// is enabled if the argument 'console' was passed by command-line. Sends all GUI messages to the command line.
		QDateTime now; /// this is for the timestamp in the logs in the gui
		QString string; // this is a free to use string

		static const int SENSORPROGRESSBARMAXIR = 50; /** max value in cm for ir sensor */
		static const int SENSORPROGRESSBARMAXUS = 400; /** max value in cm for us sensor */

		static const bool ON  = true;   /** For motor "ON" */
		static const bool OFF = false;  /** For motor "OFF" */

		/// Some driving directions *and* motor directions for the robot. @sa Direcs::drive() [Slot]
		static const int FORWARD    = 10; /// Motor direction (formerly "clockwise")
		static const int BACKWARD   = 20; /// Motor direction (formerly "counterclockwise"
		static const int LEFT		= 30;
		static const int RIGHT		= 40;
		static const int TURNLEFT	= 50;
		static const int TURNRIGHT	= 60;
		static const int START		= 70;
		static const int STOP		= 80;
		static const int WAIT		= 90;
		// static const int DIAGONAL_FORWARD_LEFT
		// static const int DIAGONAL_FORWARD_RIGHT
		// static const int DIAGONAL_BACKWARD_LEFT
		// static const int DIAGONAL_BACKWARD_RIGHT

		static const int SAME      = 200; /// Motor direction/power "same like before"
		static const int MOTOR1    = 210; /// Motor 1 front left
		static const int MOTOR2    = 220; /// Motor 2 front right
		static const int MOTOR3    = 230; /// Motor 3 back left
		static const int MOTOR4    = 240; /// Motor 4 back right
		static const int ALLMOTORS = 250; /// used for letting the robot getting only one command for all motors (like 'forward all'). This is to reduce commands on the serial line.

		static const int MOTOR1FW	= 300;
		static const int MOTOR1BW	= 310;
		static const int MOTOR1OFF	= 320;
		static const int MOTOR2FW	= 330;
		static const int MOTOR2BW	= 340;
		static const int MOTOR2OFF	= 350;
		static const int MOTOR3FW	= 360;
		static const int MOTOR3BW	= 370;
		static const int MOTOR3OFF	= 380;
		static const int MOTOR4FW	= 390;
		static const int MOTOR4BW	= 400;
		static const int MOTOR4OFF	= 410;

		static const int READ_AXIS_X = 500;
		static const int READ_AXIS_Y = 510;
		static const int READ_AXIS_Z = 520;

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

		static const float MINIMUMVOLTAGE1 = 13.20; /// \todo put min. voltage 1 in the ini-file (LiPo: 3,3 Volt x 4S = 13,2 Volt)
		static const float MINIMUMVOLTAGE2 = 24.00; /// \todo put min. voltage 2 in the ini-file (LiPo: 3.3 Volt x 6S = 19,8 Volt)

		static const float MAXIMUMVOLTAGE1 = 18.00; /// for progress bars \todo put max. voltage 1 in the ini-file (LiPo: 4,2 Volt x 4S = 16,8 Volt)
		static const float MAXIMUMVOLTAGE2 = 26.00; /// for progress bars \todo put max. voltage 2 in the ini-file (LiPo: 4,2 Volt x 6S = 25,2 Volt)

		static const float MAXIMUMPLOTVOLTAGE1 = 18.00; /// for plot widgets \todo put to ini-file
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

		/// the tags for the laser lines @sa ObstacleCheckThread
		static const int FREEWAY = 0;
		static const int OBSTACLE = 1;
		static const int LARGESTFREEWAY = 2;
		static const int CENTEROFLARGESTFREEWAY = 3;
		static const int IGNORETHIS = 4;

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
