#ifndef GUI_H
#define GUI_H

//-------------------------------------------------------------------
#include "ui_gui.h"
#include "mrs.h"
#include "motor.h"
#include "servo.h"
#include "sensorThread.h"
#include "plotThread.h"
#include "obstacleCheckThread.h"
#include "camThread.h"
#include "networkThread.h"
#include "laserThread.h"
#include "circuit.h"
#include "interface.h"
//-------------------------------------------------------------------
#include "cv.h"
#include "highgui.h"
#include "QtGLContext.h"
#include <stdio.h>
#include <ctype.h>
//-------------------------------------------------------------------
#include <qwt_plot_layout.h>
#include <qwt_plot_curve.h>
#include <qwt_scale_draw.h>
#include <qwt_scale_widget.h>
#include <qwt_legend.h>
#include <qwt_legend_item.h>
//-------------------------------------------------------------------
#include <QPainter> // for drawing lines (old method)
#include <QtGui>
#include <QtOpenGL>
#include <QGraphicsScene> // for OpenGL (Laser lines)
//-------------------------------------------------------------------


// forward declarations because of circular includes!
class Mrs;
class Motor;
class Servo;
class SensorThread;
class PlotThread;
class ObstacleCheckThread;
class CamThread;
class NetworkThread;
class Circuit;
class Interface;
class Laser;


/**
The class for all GUI actions
*/
class Gui : public QMainWindow
{
	Q_OBJECT

	public:
		Gui(Mrs *m, SensorThread *s, PlotThread *p, ObstacleCheckThread *o, Circuit *c, CamThread *ca, Motor *mot, Servo *serv, NetworkThread *net, LaserThread *l, QMainWindow *parent = 0);
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
		void showAlarm(short int sensor, unsigned char state);
		
		/**
		Shows the status and direction of a motor (ON, OFF, RIGHT, LEFT)
		@param motor is the motor number.
		@param power can be ON or OFF.
		@param direction can be FORWARD or BACKWARD.
		*/
		void showMotorStatus(unsigned char motor, unsigned char power, unsigned char direction);
		
		/**
		Appends text to the main log in the main window.
		@param text is the text to be displayed.
		@param CR adds a carriage return (CR) to the text, if true (default). This parameter is optional!
		@param sayIt If true, the text is also spoken (default=false). This parameter is optional!
		*/
		void appendLog(QString text, bool CR=true, bool sayIt=false);
		
		/**
		@return The slider value of a motor speed.
		@param motor is the motor number.
		*/
		int getSliderMotorSpeed(int motor);
		
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
		int getSliderBotSlotValue();
		
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
		Sets the slider value of an obstacle distance for the infrared and ultrasonic sensors (when a alarm should be released).
		@param value is distance in cm.
		*/
		void setSliderObstacleValue(int value);
		
		/**
		Sets the slider value of an obstacle distance for the laser scanner (when a alarm should be released).
		@param value is distance in cm.
		*/
		void setSliderObstacleLaserScannerValue(int value);
		
		//void setSliderPositionServo1(int value);
		//void setSliderPositionServo2(int value);
		
		/**
		Enables or disables the camera control in the GUI, whether a camera was found or not.
		@param state can be true or false.
		*/
		void enableCamControls(bool state);
		
		/**
		Enables or disables the laser scanner control in the GUI, whether the laser scanner is active or not.
		@param state can be true or false.
		*/
		void enableLaserScannerControls(bool state);
		
		/**
		Checks or unckecks the SaveSettings checkbox, depending on the value read from the ini-file.
		*/
		void setCheckBoxSaveSettings(Qt::CheckState state);
		
		//void setBtnSavePic(bool activate);


	
	public slots:
		/**
		Shows the new picture from the cam (live). This slot is called from the camera thread.
		@sa CamThread()
		*/
		void setCamImage(IplImage* frame);
		
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
		@param CR=true adds a carriage return (CR) to the text, if true (default). This parameter is optional!
		@param sayIt=true If true, the text is also spoken (default=false). This parameter is optional!
		@sa appendLog()
		*/
		void appendNetworkLog(QString text, bool CR=true, bool sayIt=false);
		
		/**
		Refreshes the view of the lines from the laser scanner
		*/
		void refreshLaserView();
	
	
	protected:
		void closeEvent();


	signals:
		/**
		Enables or disables the listening for the robot remote control. This signal is sent from the remote control button.
		@param status can be true or false.
		@sa NetworkThread()
		*/
		// TODO: change name to 'state'
		void enableRemoteControlListening(bool status);
	
		/**
		Enables or disables the robots simulation mode.
		This signal is sent from the simulation button and received from all threads which deliver real signals from the bot.
		Once the signal is received, the slots switches to simulation mode.
		@param status can be true or false.
		@sa Mrs::setSimulationMode()
		@sa SensorThread::setSimulationMode()
		@sa LaserThread::setSimulationMode()
		@sa ObstackeCheckThread::setSimulationMode()
		*/
		// TODO: change name to 'state'
		void simulate(bool status);
	
		/*
		Emits a speak signal. This signal is sent to the speakThread.
		*/
		// FixMe: SIOD ERROR: the currently assigned stack limit has been exceded 
		//void speak(QString text);

		
	private slots:
		//void on_sliderMotorSpeed_valueChanged(int);
		void on_sliderMotor1Speed_sliderReleased();
		void on_spinBoxMotor1Speed_valueChanged(int);
		void on_sliderMotor2Speed_sliderReleased();
		void on_spinBoxMotor2Speed_valueChanged(int);
		void on_sliderObstacle_valueChanged(int);
		void on_spinBoxObstacle_valueChanged(int);
		void on_sliderObstacleLaserScanner_valueChanged(int);
		void on_sliderPositionServo1_valueChanged(int);
		void on_btnResetMovement1_clicked();
		void on_btnResetMovement2_clicked();
		void on_btnDrive_clicked();
		void on_btnReset_clicked();
		void on_btnClearLog_clicked();
		void on_btnClearNetworkLog_clicked();
		void on_btnExit_clicked();
		//void on_btnConnectToLIRC_clicked();
		//void on_btnExecuteScript_clicked();
		void on_btnTest_clicked();
		//void on_ckeckBoxSaveSettings_stateChanged(int state);
		//void on_btnSendCommand_clicked();
		//void on_btnInitCam_clicked();
		void on_btnSavePicture_clicked();
		void on_btnAutoSavePicture_clicked();
		//void on_btnGetPicture_clicked();
		void on_btnClearPicture_clicked();
		void on_btnEnableRemote_clicked();
		void on_btnSimulate_clicked();
		void on_sliderLaserScale_valueChanged(int);
		void on_checkBoxAngleView_stateChanged(int);


	private:
		/**
		Draws some help lines / distances / dimensons in the laser scanner view.
		*/
		void drawLaserDistances(bool flatView);
		
		/**
		Saves the current picture to disk (one time shot).
		*/
		void saveCamImage(void);
		
		Ui::Gui ui;
		Mrs *mrs1;
		SensorThread *sensThread;
		PlotThread *plotThread;
		ObstacleCheckThread *obstCheckThread;
		Circuit *circuit1;
		CamThread *cam1;
		NetworkThread *netThread;
		LaserThread *lsrThread;
		Motor *motors;
		Servo *servos;
		QwtPlotCurve curve1;
		QwtPlotCurve curve2;
		QPixmap cameraPicToSave;
		QColor colorLaserObstacle;
		QColor colorLaserFreeWay;
		QColor colorLaserPreferredDrivingDirection;
		QColor colorHelpLine;
		QColor colorHelpLineText;
		QColor colorGraphicsSceneBackground;
		QPen laserLinePen;
		int lastScale;
		
		// TODO: doxygen tags
		QGraphicsScene *scene; /** The QGraphicsScene for showing the laser lines in the GUI */
		QList <QGraphicsLineItem*> *laserLineList;  /** A pointer to a QList of pointers to the laser lines (QGraphicsLineItems) */
		QList <QGraphicsLineItem*> *laserDistanceLineList;  /** A pointer to a QList of pointers to the shown distances from the laser lines (kind of coordinate system) */

		
		static const unsigned char ON  = 1;  /** For motor "ON" */
		static const unsigned char OFF = 0;  /** For motor "OFF" */
		
		static const unsigned char START = 5;
		static const unsigned char STOP  = 6;
		
		static const unsigned char CLOCKWISE        = 0;  /** Motor direction "CLOCKWISE" */
		static const unsigned char COUNTERCLOCKWISE = 1;  /** Motor direction "COUNTERCLOCKWISE" */
		static const unsigned char SAME             = 3;  /** Motor direction/power "same like before" */
		static const unsigned char MOTOR1           = 10; /** Motor 1 */
		static const unsigned char MOTOR2           = 20; /** Motor 2 */
		
		static const unsigned char SERVO1           = 10; // Servo 1
		//static const unsigned char SERVO2           = 20; // Servo 2
		
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
	
		/** factor for fitting 6 meters (measured from the laser scanner) into a frame with a height of 270 pixels */
		static const int FITTOFRAMEFACTOR=45;
		
		// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		/**
		Defines the size of the laserLines[] array
		This es equal to the number of degrees.
		See also laserThread.h */
		//static const unsigned char LASERSCANNERARRAYSIZE = 181;
};
#endif
