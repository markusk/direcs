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
#include <QtGui>
#include <QWebView>
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
		int getPort();
		void appendLog(QString text, bool sayIt=false);
	
	
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


	private:
		void initPlots();
		
		Ui::Gui ui;
		DirecsRemote *direcsremote1;
		QWebView *view;
		//MediaObject *media;
		//VideoWidget *vwidget;
		bool robotDrives;
		int speedMotor1;
		int speedMotor2;
		QwtPlotCurve curve1;
		QwtPlotCurve curve2;
		QwtPlotCurve curve3;
		QwtPlotCurve curve4;
		
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
};
#endif
