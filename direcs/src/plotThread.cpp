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

#include "plotThread.h"

PlotThread::PlotThread(SensorThread *s)
{
	stopped = false;
	
	// copy the pointer from the original object
	sensThread = s;
	
	//----------------------------------------
	// initialize array with values (x and y)
	//----------------------------------------
	// x values = seconds
	for (int i=0; i<SIZE; i++)
	{
		xval1[i] = i;
		xval2[i] = i;
		xval3[i] = i;
		xval4[i] = i;
		xval5[i] = i;
		xval6[i] = i;
		
		yval1[i] = 0;
		yval2[i] = 0;
		yval3[i] = 0;
		yval4[i] = 0;
		yval5[i] = 0;
		yval6[i] = 0;
	}
}


PlotThread::~PlotThread()
{
}


void PlotThread::stop()
{
	stopped = true;
}


void PlotThread::run()
{
	//
	//  start "threading"...
	//
	while (!stopped)
	{
		// let the thread sleep some time
		// for having more time for the other threads
		msleep(THREADSLEEPTIME);
		
		
		//------------------------------------------------------
		// get value from sensor motor 1
		//------------------------------------------------------
		// shift the values
		// (and delete the first one)
		for (int i=1; i<SIZE; i++)
		{
			yval1[i-1] = yval1[i];
		}
		
		// get a motor sensor value
		// the last value (second 60) is the latest (the read value)!
		yval1[SIZE-1] = sensThread->getMAmpere(MOTORSENSOR1);
		
		//--------------
		//  e m i t
		//--------------
		emit plotDataComplete1(xval1, yval1, SIZE);


		//------------------------------------------------------
		// get value from sensor motor 2
		//------------------------------------------------------
		// shift the values
		// (and delete the first one)
		for (int i=1; i<SIZE; i++)
		{
			yval2[i-1] = yval2[i];
		}
		
		// get a motor sensor value
		// the last value (second 60) is the latest (the read value)!
		yval2[SIZE-1] = sensThread->getMAmpere(MOTORSENSOR2);

		//--------------
		//  e m i t
		//--------------
		emit plotDataComplete2(xval2, yval2, SIZE);


		//------------------------------------------------------
		// get value from sensor motor 3
		//------------------------------------------------------
		// shift the values
		// (and delete the first one)
		for (int i=1; i<SIZE; i++)
		{
			yval3[i-1] = yval3[i];
		}
		
		// get a motor sensor value
		// the last value (second 60) is the latest (the read value)!
		yval3[SIZE-1] = sensThread->getMAmpere(MOTORSENSOR3);

		//--------------
		//  e m i t
		//--------------
		emit plotDataComplete3(xval3, yval3, SIZE);


		//------------------------------------------------------
		// get value from sensor motor 4
		//------------------------------------------------------
		// shift the values
		// (and delete the first one)
		for (int i=1; i<SIZE; i++)
		{
			yval4[i-1] = yval4[i];
		}
		
		// get a motor sensor value
		// the last value (second 60) is the latest (the read value)!
		yval4[SIZE-1] = sensThread->getMAmpere(MOTORSENSOR4);

		//--------------
		//  e m i t
		//--------------
		emit plotDataComplete4(xval4, yval4, SIZE);


		//------------------------------------------------------
		// get value from voltage sensor 1
		//------------------------------------------------------
		// shift the values
		// (and delete the first one)
		for (int i=1; i<SIZE; i++)
		{
			yval5[i-1] = yval5[i];
		}
		
		// get the sensor value
		// the last value (second 60) is the latest (the read value)!
		yval5[SIZE-1] = sensThread->getVoltage(VOLTAGESENSOR1);

		//--------------
		//  e m i t
		//--------------
		emit plotDataComplete5(xval5, yval5, SIZE);


		//------------------------------------------------------
		// get value from voltage sensor 2
		//------------------------------------------------------
		// shift the values
		// (and delete the first one)
		for (int i=1; i<SIZE; i++)
		{
			yval6[i-1] = yval6[i];
		}
		
		// get the sensor value
		// the last value (second 60) is the latest (the read value)!
		yval6[SIZE-1] = sensThread->getVoltage(VOLTAGESENSOR2);

		//--------------
		//  e m i t
		//--------------
		emit plotDataComplete6(xval6, yval6, SIZE);
	}
	stopped = false;
}
