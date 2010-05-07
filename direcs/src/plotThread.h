/*************************************************************************
 *   Copyright (C) 2010 by Markus Knapp                                  *
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
\brief Gets values from sensors and plots them in (emits them to) the GUI.

This class gets current values from the motor sensors and emits them to the GUI, which plots them using qwtplot.
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
		void plotDataComplete5(double *xval4, double *yval4, int size);
		void plotDataComplete6(double *xval4, double *yval4, int size);


	private:
		SensorThread *sensThread;
		volatile bool stopped;
		
		// Every thread sleeps some time, for having a bit more time fo the other threads!
		// Time in milliseconds
		static const unsigned long THREADSLEEPTIME = 500; // Default: 1000 ms
		
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
		double xval5[SIZE];
		double yval5[SIZE];
		//====================================
		double xval6[SIZE];
		double yval6[SIZE];
		//====================================
		
		static const short int MOTORSENSOR1 = 0;
		static const short int MOTORSENSOR2 = 1;
		static const short int MOTORSENSOR3 = 2;
		static const short int MOTORSENSOR4 = 3;
		
		static const short int VOLTAGESENSOR1 = 0;
		static const short int VOLTAGESENSOR2 = 1;
};

#endif
