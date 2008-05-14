#ifndef PLOTTHREAD_H
#define PLOTTHREAD_H

//-------------------------------------------------------------------
#include <qwt_plot.h>
#include <qwt_plot_marker.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_layout.h>
#include <qwt_legend.h>
#include <qwt_data.h>
#include <qwt_text.h>
//-------------------------------------------------------------------
#include "sensorThread.h"
//-------------------------------------------------------------------
#include <QThread>
//-------------------------------------------------------------------

/**
This class gets power values from the motor sensors and emits them to the GUI.
*/
class PlotThread : public QThread
{
    Q_OBJECT

	public:
		PlotThread(SensorThread *s);
		~PlotThread();
		void stop();
		virtual void run();


	signals:
		void plotDataComplete1(double *xval1, double *yval1, int size);
		void plotDataComplete2(double *xval2, double *yval2, int size);
		void plotDataComplete3(double *xval3, double *yval3, int size);
		void plotDataComplete4(double *xval4, double *yval4, int size);


	private:
		SensorThread *sensThread;
		volatile bool stopped;
		
		// Every thread sleeps some time, for having a bit more time fo the other threads!
		// Time in milliseconds
		static const unsigned long THREADSLEEPTIME = 1000; // Default: 1000 ms
		
		//====================================
		// array holding the plot values
		static const int SIZE = 60;
		double xval1[SIZE];
		double yval1[SIZE];
		//====================================
		double xval2[SIZE];
		double yval2[SIZE];
		//====================================
		double xval3[SIZE];
		double yval3[SIZE];
		//====================================
		double xval4[SIZE];
		double yval4[SIZE];
		//====================================
		
		static const short int MOTORSENSOR1 = 0;
		static const short int MOTORSENSOR2 = 1;
		static const short int MOTORSENSOR3 = 2;
		static const short int MOTORSENSOR4 = 3;
};

#endif
