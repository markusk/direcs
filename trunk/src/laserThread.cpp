//
// C++ Implementation: laserThread
//
// Description: 
//
//
// Author: Markus Knapp <webmaster@direcs.de>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "laserThread.h"


LaserThread::LaserThread()
{
	numReadings = 0;
	stopped = false;
	simulationMode = false;
	
	//QMutexLocker locker(&mutex); // make this class thread-safe
	
	
	// initialisation
	for (int i=0; i<LASERSCANNERARRAYSIZE; i++)
	{
		// the distances
		laserScannerValue[i][DISTANCE] = 0;
		// the flags
		laserScannerValue[i][FLAG] = 0;
	}
}


LaserThread::~LaserThread()
{
	stopped = false;
	
	//QMutexLocker locker(&mutex); // make this class thread-safe
}


void LaserThread::stop()
{
	stopped = true;
	numReadings = 0;
}


/*!
This method reads the values from the Laser Scanner and emits a signal to show them in the GUI.
*/
void LaserThread::run()
{
	// check if all 180 beams were read
	numReadings = 0;
	
	//
	//  start "threading"...
	//
	while (!stopped)
	{
		// let the thread sleep some time
		// for having more time for the other threads
		msleep(THREADSLEEPTIME);
		
		
		if (simulationMode==false)
		{
			// check if all 180 beams were read (in the laser module)
			numReadings = getLaserNumReadings();
			//qDebug("numReadings=%d",numReadings);
			
			// if YES
			if (numReadings > 0)
			{
				// /get the data from 0° to 180° (left to right)
				for (int i=0; i<=numReadings; i++)
				{
					// get value from laser
					// store the value in an array in this thread
					laserScannerValue[i][DISTANCE] = getLaserDistance(i);
				}
				
				
				//====================================================================
				//  e m i t  Signal
				//====================================================================
				emit laserDataComplete();
			}
		}
		else
		{
			//---------------------------------
			// S I M U L A T I O N   M O D E
			//---------------------------------
			numReadings = 180;
			
			emit laserDataComplete();
		}
	}
	stopped = false;
}


float LaserThread::getLaserScannerValue(int angle)
{
	//
	// These values were NOT converted at any point in this method!
	//
	
	if ((angle < 0) || (angle > LASERSCANNERARRAYSIZE-1))
	{
	  return 0;
	}
	
	
	return laserScannerValue[angle][DISTANCE];
}


float LaserThread::getLaserScannerFlag(int angle)
{
	if ((angle < 0) || (angle > LASERSCANNERARRAYSIZE-1))
	{
	  return 0;
	}
	
	
	return laserScannerValue[angle][FLAG];
}


void LaserThread::setLaserScannerFlag(int angle, int flag)
{
	if ((angle < 0) || (angle > LASERSCANNERARRAYSIZE-1))
	{
	  return;
	}
	
	
	laserScannerValue[angle][FLAG] = flag;
}


int LaserThread::getNumReadings()
{
	// check if all 180 beams were read (in the laser module)
	// return getLaserNumReadings(); <- changed 04.08.2007 !!
	return numReadings;
}


void LaserThread::setSimulationMode(bool status)
{
	simulationMode = status;
	
	if (simulationMode == true)
	{
		numReadings = 180;
		
		// fill the array with some nice values
		//for (int i=0; i<LASERSCANNERARRAYSIZE; i++)
		laserScannerValue[0][DISTANCE]		= 1.0;
		laserScannerValue[1][DISTANCE]		= 1.0;
		laserScannerValue[2][DISTANCE]		= 1.0;
		laserScannerValue[3][DISTANCE]		= 1.0;
		laserScannerValue[4][DISTANCE]		= 1.0;
		laserScannerValue[5][DISTANCE]		= 1.0;
		laserScannerValue[6][DISTANCE]		= 1.0;
		laserScannerValue[7][DISTANCE]		= 1.0;
		laserScannerValue[8][DISTANCE]		= 1.0;
		laserScannerValue[9][DISTANCE]		= 1.0;
		laserScannerValue[10][DISTANCE]		= 1.5;
		laserScannerValue[11][DISTANCE]		= 1.5;
		laserScannerValue[12][DISTANCE]		= 1.5;
		laserScannerValue[13][DISTANCE]		= 1.5;
		laserScannerValue[14][DISTANCE]		= 1.5;
		laserScannerValue[15][DISTANCE]		= 1.5;
		laserScannerValue[16][DISTANCE]		= 1.5;
		laserScannerValue[17][DISTANCE]		= 1.5;
		laserScannerValue[18][DISTANCE]		= 1.5;
		laserScannerValue[19][DISTANCE]		= 1.5;
		laserScannerValue[20][DISTANCE]		= 2.0;
		laserScannerValue[21][DISTANCE]		= 2.0;
		laserScannerValue[22][DISTANCE]		= 2.0;
		laserScannerValue[23][DISTANCE]		= 2.0;
		laserScannerValue[24][DISTANCE]		= 2.0;
		laserScannerValue[25][DISTANCE]		= 2.0;
		laserScannerValue[26][DISTANCE]		= 2.0;
		laserScannerValue[27][DISTANCE]		= 2.0;
		laserScannerValue[28][DISTANCE]		= 2.0;
		laserScannerValue[29][DISTANCE]		= 2.0;
		laserScannerValue[30][DISTANCE]		= 2.5;
		laserScannerValue[31][DISTANCE]		= 2.5;
		laserScannerValue[32][DISTANCE]		= 2.5;
		laserScannerValue[33][DISTANCE]		= 2.5;
		laserScannerValue[34][DISTANCE]		= 2.5;
		laserScannerValue[35][DISTANCE]		= 2.5;
		laserScannerValue[36][DISTANCE]		= 2.5;
		laserScannerValue[37][DISTANCE]		= 2.5;
		laserScannerValue[38][DISTANCE]		= 2.5;
		laserScannerValue[39][DISTANCE]		= 2.5;
		laserScannerValue[40][DISTANCE]		= 3.25;
		laserScannerValue[41][DISTANCE]		= 3.25;
		laserScannerValue[42][DISTANCE]		= 3.25;
		laserScannerValue[43][DISTANCE]		= 3.25;
		laserScannerValue[44][DISTANCE]		= 3.25;
		laserScannerValue[45][DISTANCE]		= 3.25;
		laserScannerValue[46][DISTANCE]		= 3.25;
		laserScannerValue[47][DISTANCE]		= 3.25;
		laserScannerValue[48][DISTANCE]		= 3.25;
		laserScannerValue[49][DISTANCE]		= 3.25;
		laserScannerValue[50][DISTANCE]		= 3.5;
		laserScannerValue[51][DISTANCE]		= 3.5;
		laserScannerValue[52][DISTANCE]		= 3.5;
		laserScannerValue[53][DISTANCE]		= 3.5;
		laserScannerValue[54][DISTANCE]		= 3.5;
		laserScannerValue[55][DISTANCE]		= 3.5;
		laserScannerValue[56][DISTANCE]		= 3.5;
		laserScannerValue[57][DISTANCE]		= 3.5;
		laserScannerValue[58][DISTANCE]		= 3.5;
		laserScannerValue[59][DISTANCE]		= 3.5;
		laserScannerValue[60][DISTANCE]		= 4.0;
		laserScannerValue[61][DISTANCE]		= 4.0;
		laserScannerValue[62][DISTANCE]		= 4.0;
		laserScannerValue[63][DISTANCE]		= 4.0;
		laserScannerValue[64][DISTANCE]		= 4.0;
		laserScannerValue[65][DISTANCE]		= 4.0;
		laserScannerValue[66][DISTANCE]		= 4.0;
		laserScannerValue[67][DISTANCE]		= 4.0;
		laserScannerValue[68][DISTANCE]		= 4.0;
		laserScannerValue[69][DISTANCE]		= 4.0;
		laserScannerValue[70][DISTANCE]		= 4.5;
		laserScannerValue[71][DISTANCE]		= 4.5;
		laserScannerValue[72][DISTANCE]		= 4.5;
		laserScannerValue[73][DISTANCE]		= 4.5;
		laserScannerValue[74][DISTANCE]		= 4.5;
		laserScannerValue[75][DISTANCE]		= 4.5;
		laserScannerValue[76][DISTANCE]		= 4.5;
		laserScannerValue[77][DISTANCE]		= 4.5;
		laserScannerValue[78][DISTANCE]		= 4.5;
		laserScannerValue[79][DISTANCE]		= 4.5;
		laserScannerValue[80][DISTANCE]		= 5.0;
		laserScannerValue[81][DISTANCE]		= 5.0;
		laserScannerValue[82][DISTANCE]		= 5.0;
		laserScannerValue[83][DISTANCE]		= 5.0;
		laserScannerValue[84][DISTANCE]		= 5.0;
		laserScannerValue[85][DISTANCE]		= 5.0;
		laserScannerValue[86][DISTANCE]		= 2.5;
		laserScannerValue[87][DISTANCE]		= 2.5;
		laserScannerValue[88][DISTANCE]		= 2.5;
		laserScannerValue[89][DISTANCE]		= 2.5;
		laserScannerValue[90][DISTANCE]		= 2.5;
		laserScannerValue[91][DISTANCE]		= 2.5;
		laserScannerValue[92][DISTANCE]		= 2.5;
		laserScannerValue[93][DISTANCE]		= 2.5;
		laserScannerValue[94][DISTANCE]		= 2.5;
		laserScannerValue[95][DISTANCE]		= 2.5;
		laserScannerValue[96][DISTANCE]		= 2.5;
		laserScannerValue[97][DISTANCE]		= 2.5;
		laserScannerValue[98][DISTANCE]		= 2.5;
		laserScannerValue[99][DISTANCE]		= 2.5;
		laserScannerValue[100][DISTANCE]	= 2.0;
		laserScannerValue[101][DISTANCE]	= 2.0;
		laserScannerValue[102][DISTANCE]	= 2.0;
		laserScannerValue[103][DISTANCE]	= 2.0;
		laserScannerValue[104][DISTANCE]	= 2.0;
		laserScannerValue[105][DISTANCE]	= 2.0;
		laserScannerValue[106][DISTANCE]	= 3.0;
		laserScannerValue[107][DISTANCE]	= 3.0;
		laserScannerValue[108][DISTANCE]	= 3.0;
		laserScannerValue[109][DISTANCE]	= 3.0;
		laserScannerValue[110][DISTANCE]	= 3.0;
		laserScannerValue[111][DISTANCE]	= 3.0;
		laserScannerValue[112][DISTANCE]	= 3.0;
		laserScannerValue[113][DISTANCE]	= 3.0;
		laserScannerValue[114][DISTANCE]	= 3.0;
		laserScannerValue[115][DISTANCE]	= 3.0;
		laserScannerValue[116][DISTANCE]	= 3.0;
		laserScannerValue[117][DISTANCE]	= 3.0;
		laserScannerValue[118][DISTANCE]	= 3.0;
		laserScannerValue[119][DISTANCE]	= 3.0;
		laserScannerValue[120][DISTANCE]	= 2.0;
		laserScannerValue[121][DISTANCE]	= 2.0;
		laserScannerValue[122][DISTANCE]	= 2.0;
		laserScannerValue[123][DISTANCE]	= 2.0;
		laserScannerValue[124][DISTANCE]	= 2.0;
		laserScannerValue[125][DISTANCE]	= 2.0;
		laserScannerValue[126][DISTANCE]	= 2.0;
		laserScannerValue[127][DISTANCE]	= 2.0;
		laserScannerValue[128][DISTANCE]	= 2.0;
		laserScannerValue[129][DISTANCE]	= 2.0;
		laserScannerValue[130][DISTANCE]	= 2.0;
		laserScannerValue[131][DISTANCE]	= 2.0;
		laserScannerValue[132][DISTANCE]	= 2.0;
		laserScannerValue[133][DISTANCE]	= 2.0;
		laserScannerValue[134][DISTANCE]	= 2.0;
		laserScannerValue[135][DISTANCE]	= 2.0;
		laserScannerValue[136][DISTANCE]	= 2.0;
		laserScannerValue[137][DISTANCE]	= 2.0;
		laserScannerValue[138][DISTANCE]	= 2.0;
		laserScannerValue[139][DISTANCE]	= 2.0;
		laserScannerValue[140][DISTANCE]	= 6.0;
		laserScannerValue[141][DISTANCE]	= 6.0;
		laserScannerValue[142][DISTANCE]	= 6.0;
		laserScannerValue[143][DISTANCE]	= 6.0;
		laserScannerValue[144][DISTANCE]	= 6.0;
		laserScannerValue[145][DISTANCE]	= 6.0;
		laserScannerValue[146][DISTANCE]	= 6.0;
		laserScannerValue[147][DISTANCE]	= 6.0;
		laserScannerValue[148][DISTANCE]	= 6.0;
		laserScannerValue[149][DISTANCE]	= 6.0;
		laserScannerValue[150][DISTANCE]	= 9.0;
		laserScannerValue[151][DISTANCE]	= 9.0;
		laserScannerValue[152][DISTANCE]	= 9.0;
		laserScannerValue[153][DISTANCE]	= 9.0;
		laserScannerValue[154][DISTANCE]	= 9.0;
		laserScannerValue[155][DISTANCE]	= 9.0;
		laserScannerValue[156][DISTANCE]	= 9.0;
		laserScannerValue[157][DISTANCE]	= 9.0;
		laserScannerValue[158][DISTANCE]	= 9.0;
		laserScannerValue[159][DISTANCE]	= 9.0;
		laserScannerValue[160][DISTANCE]	= 4.0;
		laserScannerValue[161][DISTANCE]	= 4.0;
		laserScannerValue[162][DISTANCE]	= 4.0;
		laserScannerValue[163][DISTANCE]	= 4.0;
		laserScannerValue[164][DISTANCE]	= 4.0;
		laserScannerValue[165][DISTANCE]	= 4.0;
		laserScannerValue[166][DISTANCE]	= 4.0;
		laserScannerValue[167][DISTANCE]	= 4.0;
		laserScannerValue[168][DISTANCE]	= 4.0;
		laserScannerValue[169][DISTANCE]	= 4.0;
		laserScannerValue[170][DISTANCE]	= 2.0;
		laserScannerValue[171][DISTANCE]	= 2.0;
		laserScannerValue[172][DISTANCE]	= 2.0;
		laserScannerValue[173][DISTANCE]	= 2.0;
		laserScannerValue[174][DISTANCE]	= 2.0;
		laserScannerValue[175][DISTANCE]	= 2.0;
		laserScannerValue[176][DISTANCE]	= 2.0;
		laserScannerValue[177][DISTANCE]	= 2.0;
		laserScannerValue[178][DISTANCE]	= 2.0;
		laserScannerValue[179][DISTANCE]	= 2.0;

		/*
		for (int i=0; i<LASERSCANNERARRAYSIZE; i++)
		{
			// the distances
			laserScannerValue[i][DISTANCE] = 4.0;
			// the flags
			laserScannerValue[i][FLAG] = 0;
		}
		*/
		
	}
	else
	{
		// initialisation
		for (int i=0; i<LASERSCANNERARRAYSIZE; i++)
		{
			// the distances
			laserScannerValue[i][DISTANCE] = 0;
			// the flags
			laserScannerValue[i][FLAG] = 0;
		}
		
		// for refreshing the gui (deleting simulated laser lines)
		emit laserDataComplete();
	}
}
