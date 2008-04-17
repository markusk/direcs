#ifndef GUI_H
#define GUI_H

//-------------------------------------------------------------------
#include "ui_mainWindow.h"
#include "QtGLContext.h"
//-------------------------------------------------------------------
#include "cv.h" // for type IplImage (camThread)
//-------------------------------------------------------------------
#include <qwt_plot_layout.h>
#include <qwt_plot_curve.h>
#include <qwt_scale_draw.h>
#include <qwt_scale_widget.h>
#include <qwt_legend.h>
#include <qwt_legend_item.h>
//-------------------------------------------------------------------
#include <QtGui>
#include <QtOpenGL>
#include <QGraphicsScene> // for OpenGL (Laser lines)
//-------------------------------------------------------------------


/**
\brief The class for all GUI actions
*/
class Gui : public QMainWindow
{
	Q_OBJECT

	public:
		Gui(QMainWindow *parent = 0);
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
		
		/**
		@return The slider value of a motor speed.
		@param motor is the motor number.
		*/
		int getSliderMotorSpeed(int motor);
		
		/**
		@return The slider value of the minimum speed.
		*/
		int getSliderMinimumSpeed(void);
		
		/**
		@return The slider value of the maximum speed.
		*/
		int getSliderMaximumSpeed(void);
		
		/**
		@return The slider value of an obstacle distance for the infrared and ultrasonic sensors (when a alarm should be released).
		*/
		int getSliderObstacleValue();
		
		/**
		@return The slider value of an obstacle distance for the laser scanner (when a alarm should be released).
		*/
		int getSliderObstacleLaserScannerValue();
		
		/**
		@return The angle to which the robot has to fit between.
		*/
		int getSliderRobotSlotValue();
		
		/**
		@return The slider value of the deviation to drive forward (deviation to 90 degrees).
		*/
		int getSliderStraightForwardDeviationValue();
		
		/**
		@return The state of a Qt checkBox.
		*/
		Qt::CheckState getCheckBoxSaveSettings();
		//Qt::CheckState getCheckBoxAutoSave();
		
		/**
		Sets the slider MotorSpeed.
		@param motor is the motor number.
		@param value is the speed of the motor.
		*/
		void setSliderMotorSpeed(int motor, int value);
		
		/**
		Sets the slider minimum speed.
		@param speed is the minimum speed of the motors.
		*/
		void setSliderMinimumSpeed(int speed);
		
		/**
		Sets the slider maximum speed.
		@param speed is the maximum speed of the motors.
		*/
		void setSliderMaximumSpeed(int speed);
		
		/**
		Sets the slider value of an obstacle distance for the infrared and ultrasonic sensors (when a alarm should be released).
		@param value is distance in cm.
		*/
		void setSliderObstacleValue(int value);
		
		/**
		Sets the slider value of an obstacle distance for the laser scanner (when a alarm should be released).
		@param value is distance in cm.
		*/
		void setSliderObstacleLaserScannerValue(int value);
		
		/**
		Sets the slider value of the robot slot, where it has to fit through.
		@param angle is the minimum angle in degrees.
		*/
		void setSliderRobotSlot(int angle);
		
		/**
		Sets the slider value of the deviation to drive forward (deviation to 90 degrees).
		@param angle is the maximum angle in degrees.
		*/
		void setSliderStraightForwardDeviation(int angle);
		
		/**
		Checks or unckecks the SaveSettings checkbox, depending on the value read from the ini-file.
		*/
		void setCheckBoxSaveSettings(Qt::CheckState state);
		
		/**
		tell the OpenGLContext the image data
		*/
		void setCamImageData(int width, int height, int pixeldepth);

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
		
		/**
		Shows the new picture from the cam (live).
		@param frame
		@sa CamThread::camDataComplete()
		*/
		void setCamImage(IplImage* frame);
		
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
		Shows the joystick values when it moves.
		*/
		void showJoystickAxes(int axisNumber, int axisValue);
		
		/**
		Shows the joystick button states when pressed.
		*/
		void showJoystickButtons(int buttonNumber, bool buttonState);
		
		/**
		Shows the angles of the free area where to drive in lables.
		@param largestFreeAreaStart
		@param largestFreeAreaEnd
		@param centerOfFreeWay
		*/
		void showLaserFrontAngles(int largestFreeAreaStart, int largestFreeAreaEnd, int centerOfFreeWay);
		
		/**
		Shows a graphic in the laser scanner view while searching for a scanner.
		@param status can be true or false
		@param laserScanner is the number of the laser scanner and can be LASER1, LASER2...
		*/
		void laserSplash(bool status, short int laserScanner);
		
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
		Emits the speed of a motor.
		@param motor is the motor number.
		@param speed is the speed (0 - 255).
		*/
		void setMotorSpeed(int motor, int speed);
		
		/*!
		This slot sets the angle, where the robots has to fit thru
		*/
		void setRobotSlot(int angle);

		/*!
		This slot sets the deviation to 90 degrees when the robot drives forward.
		@param deviation is the angle in degrees.
		*/
		void setStraightForwardDeviation(int deviation);
		
		/*!
		This slot set the minimum distance, which the robot needs. Used by the infrared and ultra sonic sensors.
		@param distance in centimeters
		*/
		void setMinObstacleDistance(int distance);
		
		/*!
		This slot set the minimum distance, which the robot needs. Used by the Laser scanner.
		@param distance in centimeters
		 */
		void setMinObstacleDistanceLaser(int distance);

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
		@sa Mrs::setSimulationMode()
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
		Emits a signal to call the test method in the Mrs class..
		 */
		void test();


	protected slots:
		/**
		Action if a menu item is clicked
		*/
		void on_actionExit_activated();
		void on_actionLaser_View_activated();
		void on_actionCamera_activated();
		void on_actionMotors_activated();
		void on_actionSettings_activated();
		void on_actionLog_activated();
		void on_actionNetwork_Log_activated();
		void on_actionJoystick_activated();


	private slots:
		void on_sliderMotor1Speed_sliderReleased();
		void on_sliderMotor2Speed_sliderReleased();
		void on_btnResetMovement1_clicked();
		void on_btnResetMovement2_clicked();
		void on_btnDrive_clicked();
		void on_btnReset_clicked();
		void on_btnExit_clicked();
		void on_btnTest_clicked();
		//void on_ckeckBoxSaveSettings_stateChanged(int state);
		void on_btnSavePicture_clicked();
		void on_btnEnableRemote_clicked();
		void on_btnSimulate_clicked();
		void on_sliderZoom_valueChanged(int);
		void on_checkBoxMirror_stateChanged(int);
		void on_checkBoxFaceDetection_stateChanged(int);
		void on_checkBoxFaceTracking_stateChanged(int);
		
		/**
		Saves the current picture to disk (one time shot).
		*/
		void saveCamImage(void);


	private:
		void initializePlots();
		
		/**
		Creates all objects, lines, scene, view etc.
		*/
		void createLaserScannerObjects();
		void createLaserDistanceObjects();
		
		/**
		Moves the laser lines to their correct x and y positions
		*/
		void setLaserLinesPositions();
		
		/**
		Moves the laser line distances to their correct x and y positions
		 */
		void setLaserDistancesPositions();
		
		/**
		Draws some help lines / distances / dimensons in the laser scanner view.
		*/
		void drawLaserDistances();
		
		/**
		Set a X position for the laser lines "start point"
		*/
		qreal calculateLaserXpos();
		
		/**
		Set a Y position for the FRONT laser lines "start point"
		*/
		qreal calculateLaserFrontYpos();
		
		/**
		Set a Y position for the REAR laser lines "start point"
		*/
		qreal calculateLaserRearYpos();
		
		Ui::mainWindow ui;
		bool robotIsOn; //! Stores the robots (circuits) state.ON or OFF
		QwtPlotCurve curve1;
		QwtPlotCurve curve2;
		QPixmap cameraPicToSave;
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
		qreal laserFrontYPos;
		qreal laserRearYPos;
		QGraphicsScene *scene;								/** The QGraphicsScene for showing the laser lines in the GUI */
		//QGraphicsScene *cameraScene;						/** The QGraphicsScene for drawing "over" the camera image */
		QList <QGraphicsLineItem*> *laserLineListFront;		/** A pointer to a QList of pointers to the front laser lines (QGraphicsLineItems) */
		QList <QGraphicsLineItem*> *laserLineListRear;		/** A pointer to a QList of pointers to the rear laser lines (QGraphicsLineItems) */
		QList <QGraphicsEllipseItem*> *laserDistanceLineListFront;	/** A pointer to a QList of pointers to the shown distances from the front laser lines (kind of coordinate system) */
		QList <QGraphicsPathItem*> *laserDistanceLineListRear;	/** A pointer to a QList of pointers to the shown distances from the rear laser lines (kind of coordinate system) */
		QGraphicsPixmapItem *pixmapBot1;
		QGraphicsPixmapItem *pixmapBot2;
		QGraphicsPixmapItem *scannerFrontSplash;
		QGraphicsPixmapItem *scannerRearSplash;

		static const int SENSORPROGRESSBARMAXIR = 50; /** max value in cm for ir sensor */
		static const int SENSORPROGRESSBARMAXUS = 400; /** max value in cm for us sensor */
		
		static const bool ON  = true;   /** For motor "ON" */
		static const bool OFF = false;  /** For motor "OFF" */
		
		static const unsigned char START = 5;
		static const unsigned char STOP  = 6;
		
		static const unsigned char CLOCKWISE        = 0;  /** Motor direction "CLOCKWISE" */
		static const unsigned char COUNTERCLOCKWISE = 1;  /** Motor direction "COUNTERCLOCKWISE" */
		static const unsigned char SAME             = 3;  /** Motor direction/power "same like before" */
		static const unsigned char MOTOR1           = 10; /** Motor 1 */
		static const unsigned char MOTOR2           = 20; /** Motor 2 */
		
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

		// TODO: put axis numbers to ini-file
		//! Joystick axis numbers
		static const int JOYSTICKAXISY = 2;
		static const int JOYSTICKAXISX = 3;
		static const int JOYSTICKAXIS2X = 4;
		static const int JOYSTICKAXIS2Y = 5;
	
		/** factor for fitting 6 meters (measured from the laser scanner) into a frame with a height of 270 pixels */
		static const int FITTOFRAMEFACTOR=45;
		static const int LASERLINELENGTH=278;
		
		
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
		
		/*!
		For the laserThread
		*/
		static const short int LASER1 = 1;
		static const short int LASER2 = 2;
		static const short int LASER3 = 4;
		static const short int LASER4 = 8;
		static const short int LASER5 = 16;
		
		//! Number of distance semi circles
		static const int LASERDISTANCECIRCLES = 20;
		//! Distances between the lines to show the laser distances
		static const int LASERDISTANCEDISTANCE = 60;
};
#endif
