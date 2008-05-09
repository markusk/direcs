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
		
		yval1[i] = 0;
		yval2[i] = 0;
		yval3[i] = 0;
		yval4[i] = 0;
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
		//yval3[SIZE-1] = sensThread->getMAmpere(MOTORSENSOR3);
		yval3[SIZE-1] = 0; // TODO: implement 3rd sensor!

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
		//yval4[SIZE-1] = sensThread->getMAmpere(MOTORSENSOR3);
		yval4[SIZE-1] = 0; // TODO: implement 4th sensor!

		//--------------
		//  e m i t
		//--------------
		emit plotDataComplete4(xval4, yval4, SIZE);
	}
	stopped = false;
}
