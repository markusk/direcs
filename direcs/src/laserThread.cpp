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

#include "laserThread.h"


LaserThread::LaserThread()
{
	stopped = false;
	laserScannerFrontIsConnected = false;
	laserScannerRearIsConnected = false;
	simulationMode = false;
	numReadingsFront = 0;
	numReadingsRear = 0;

	// initialisation
	for (int i=0; i<LASERSCANNERARRAYSIZE; i++) // FIXME: 180 degrees bug!!
	{
		// add a 0 m laser distance
		laserScannerValuesFront.append(0);
		laserScannerValuesRear.append(0);

		// add a 0 flag
		laserScannerFlagsFront.append(0);
		laserScannerFlagsRear.append(0);
	}

	// call init() here?
}


LaserThread::~LaserThread()
{
	// laserScannerFrontIsConnected is set in the isConnected method!
	/*
	  TODO !!!
	if (laserScannerFrontIsConnected || laserScannerRearIsConnected)
	{
		if (laserScannerType == PLS)
		{
			// shutdown laser (parameter '0' is not in use)
			// This shuts down ALL lasers (managed internaly in laser.cpp)!
			laser->direcs_laser_shutdown(0);
			delete laser;
		}
		else
		{
			if (laserScannerType == S300)
			{
				delete laserS300;
			}
		}
	}
	*/
}


void LaserThread::stop()
{
	stopped = true;
	numReadingsFront = 0;
	numReadingsRear = 0;
}


void LaserThread::run()
{
	int laserValue = 0;

	
	// check if all 180 beams were read
	numReadingsFront = 0;
	numReadingsRear = 0;
	
	//
	//  start "threading"...
	//
	while (!stopped)
	{
		// let the thread sleep some time
		// for having more time for the other threads
		msleep(THREADSLEEPTIME);
		
		if (
			((simulationMode==false) && (laserScannerFrontIsConnected == true)) ||
			((simulationMode==false) && (laserScannerRearIsConnected == true))
		   )
		{
			if ( (laserscannerTypeFront==PLS) || (laserscannerTypeRear==PLS)) // this is stupid, but the direcs_laser_run gets always ALL laser scanner values (wasn't my idea)
			{
				// asks ALL lasers (managed internaly in laser_main.cpp) for data.
				// this calls code for PLS laser scanners!
				laserValue = laser->direcs_laser_run();

				switch (laserValue)
				{
				case LASER1:
					getAndStoreLaserValuesFront();
					emit laserDataCompleteFront(&laserScannerValuesFront[0], &laserScannerFlagsFront[0]);
					break;
				case LASER2:
					getAndStoreLaserValuesRear();
					emit laserDataCompleteRear(&laserScannerValuesRear[0], &laserScannerFlagsRear[0]);
					break;
				case (LASER1+LASER2):
					getAndStoreLaserValuesFront();
					emit laserDataCompleteFront(&laserScannerValuesFront[0], &laserScannerFlagsFront[0]);
					getAndStoreLaserValuesRear();
					emit laserDataCompleteRear(&laserScannerValuesRear[0], &laserScannerFlagsRear[0]);
					break;
				}
			}
			else
			{
				if ( (laserscannerTypeFront==S300) || (laserscannerTypeRear==S300))
				{
					// TODO: add S300 stuff
				}
			}
		}
		else
		{
			//---------------------------------
			// S I M U L A T I O N   M O D E
			//---------------------------------
			numReadingsFront = 180; // FIXME: check if we have a 270 deg. laser!
			numReadingsRear = 180;  // FIXME: check if we have a 270 deg. laser!
			//====================================================================
			//  e m i t  Signal
			//====================================================================
			emit laserDataCompleteFront(&laserScannerValuesFront[0], &laserScannerFlagsFront[0]);
			emit laserDataCompleteRear(&laserScannerValuesRear[0], &laserScannerFlagsRear[0]);
		}
	}
	stopped = false;
}


void LaserThread::getAndStoreLaserValuesFront()
{
	if (laserscannerTypeFront == PLS)
	{
		// check if all 180 beams were read (in the laser module)
		numReadingsFront = laser->getLaserNumReadings(LASER1);
		
		// numReadings can't be over the number of elements in the QList 'laserScannerValues'!!
		if (numReadingsFront > LASERSCANNERARRAYSIZE)
			numReadingsFront = LASERSCANNERARRAYSIZE;
		
		// if YES
		if (numReadingsFront > 0)
		{
			if (mountingLaserscannerFront == "normal")
			{
				// /get the data from 0° to 180° (left to right)
				for (int angle=0; angle<numReadingsFront; angle++)
				{
					// get value from laser
					// store the value in an array in this thread
					laserScannerValuesFront[angle] = laser->getLaserDistance(LASER1, angle);
			
					// send value over the network
					// *0l23a42# means LASER1 has at angle 23 a length of 42 cm
					emit sendNetworkString( QString("*%1l%2a%3#").arg(LASER1).arg(angle).arg( laserScannerValuesFront[angle] ) );
				}
			}
			else
			{
				// flip the data, due to a flipped mounting of the hardware!
				//
				// get the data from 0° to 180° (left to right)
				// 'flip' will be increased every step - 1, so the data are stored from 180° to 0°
				for (int angle=0, flip=numReadingsFront-1; angle<numReadingsFront; angle++, flip--)
				{
					// get value from laser
					// store the value in an array in this thread
					laserScannerValuesFront[flip] = laser->getLaserDistance(LASER1, angle);
			
					// send value over the network
					// *0l23a42# means LASER1 has at angle 23 a length of 42 cm
					emit sendNetworkString( QString("*%1l%2a%3#").arg(LASER1).arg(angle).arg( laserScannerValuesFront[angle] ) );
				}
			}
		}
	}
	else
	{
		if (laserscannerTypeFront == S300)
		{
			// TODO: add S300 stuff
		}
	}
}


void LaserThread::getAndStoreLaserValuesRear()
{
	if (laserscannerTypeRear == PLS)
	{
		// check if all 180 beams were read (in the laser module)
		numReadingsRear = laser->getLaserNumReadings(LASER2);

		// numReadings can't be over the number of elements in the QList 'laserScannerValues'!!
		if (numReadingsRear > LASERSCANNERARRAYSIZE)
			numReadingsRear = LASERSCANNERARRAYSIZE;

		// if YES
		if (numReadingsRear > 0)
		{
			if (mountingLaserscannerFront == "normal")
			{
				// /get the data from 0° to 180° (left to right)
				for (int angle=0; angle<numReadingsRear; angle++)
				{
					// get value from laser
					// store the value in an array in this thread
					laserScannerValuesRear[angle] = laser->getLaserDistance(LASER2, angle);

					// send value over the network
					// *1l23a42# means LASER2 has at angle 23 a length of 42 cm
					emit sendNetworkString( QString("*%1l%2a%3#").arg(LASER2).arg(angle).arg( laserScannerValuesRear[angle] ) );
				}
			}
			else
			{
				// flip the data, due to a flipped mounting of the hardware!
				//
				// get the data from 0° to 180° (left to right)
				// 'flip' will be increased every step - 1, so the data are stored from 180° to 0°
				for (int angle=0, flip=numReadingsRear-1; angle<numReadingsRear; angle++, flip--)
				{
					// get value from laser
					// store the value in an array in this thread
					laserScannerValuesRear[flip] = laser->getLaserDistance(LASER2, angle);

					// send value over the network
					// *1l23a42# means LASER2 has at angle 23 a length of 42 cm
					emit sendNetworkString( QString("*%1l%2a%3#").arg(LASER2).arg(angle).arg( laserScannerValuesRear[angle] ) );
				}
			}
		}
	}
	else
	{
		if (laserscannerTypeRear == S300)
		{
			// TODO: add S300 stuff
		}
	}
}


float LaserThread::getLaserScannerValue(short int laserScanner, int angle)
{
	//
	// These values were NOT converted at any point in this method!
	//
	
	if ((angle < 0) || (angle > LASERSCANNERARRAYSIZE-1)) // FIXME: LASERSCANNERARRAYSIZE vs. 180 / 270 degrees!
	{
	  return 0;
	}
	
	
	switch(laserScanner)
	{
		case LASER1:
			return laserScannerValuesFront.at(angle);
			break;
		case LASER2:
			return laserScannerValuesRear.at(angle);
			break;
	}
	
	return 0;
}


float LaserThread::getLaserScannerFlag(short int laserScanner, int angle)
{
	if ((angle < 0) || (angle > LASERSCANNERARRAYSIZE-1))
	{
	  return 0;
	}
	
	
	switch(laserScanner)
	{
		case LASER1:
			return laserScannerFlagsFront.at(angle);
			break;
		case LASER2:
			return laserScannerFlagsRear.at(angle);
			break;
	}
	
	return 0;
}


void LaserThread::setLaserScannerFlag(short int laserScanner, int angle, int flag)
{
	if ((angle < 0) || (angle > LASERSCANNERARRAYSIZE-1))
	{
	  return;
	}
	
	
	switch(laserScanner)
	{
		case LASER1:
			laserScannerFlagsFront[angle] = flag;
			break;
		case LASER2:
			laserScannerFlagsRear[angle] = flag;
			break;
	}
	
	return;
}


void LaserThread::setSimulationMode(bool status)
{
	simulationMode = status;
	
	
	if (simulationMode == true)
	{
		numReadingsFront = LASERSCANNERARRAYSIZE;
		numReadingsRear = LASERSCANNERARRAYSIZE;
/*		
		// fill with values from 1 to 180 cm
		for (float i=0; i<LASERSCANNERARRAYSIZE; i++)
		{
			laserScannerValuesFront[i] = 0.18;//(i+1) / 100;
			laserScannerValuesRear[i] = 0.18;//(i+1) / 100;
		}
*/
		// fill the array with some nice values
		laserScannerValuesFront[0]	= 0.25;		laserScannerValuesRear[0]	= 0.25;
		laserScannerValuesFront[1]	= 0.25;		laserScannerValuesRear[1]	= 0.25;
		laserScannerValuesFront[2]	= 0.25;		laserScannerValuesRear[2]	= 0.25;
		laserScannerValuesFront[3]	= 0.25;		laserScannerValuesRear[3]	= 0.25;
		laserScannerValuesFront[4]	= 0.25;		laserScannerValuesRear[4]	= 0.25;
		laserScannerValuesFront[5]	= 0.25;		laserScannerValuesRear[5]	= 0.25;
		laserScannerValuesFront[6]	= 0.25;		laserScannerValuesRear[6]	= 0.25;
		laserScannerValuesFront[7]	= 0.25;		laserScannerValuesRear[7]	= 0.25;
		laserScannerValuesFront[8]	= 0.25;		laserScannerValuesRear[8]	= 0.25;
		laserScannerValuesFront[9]	= 0.25;		laserScannerValuesRear[9]	= 0.25;
		laserScannerValuesFront[10]	= 1.5;		laserScannerValuesRear[10]	= 0.25;
		laserScannerValuesFront[11]	= 1.5;		laserScannerValuesRear[11]	= 0.25;
		laserScannerValuesFront[12]	= 1.5;		laserScannerValuesRear[12]	= 1.5;
		laserScannerValuesFront[13]	= 1.5;		laserScannerValuesRear[13]	= 1.5;
		laserScannerValuesFront[14]	= 1.5;		laserScannerValuesRear[14]	= 1.5;
		laserScannerValuesFront[15]	= 1.5;		laserScannerValuesRear[15]	= 1.5;
		laserScannerValuesFront[16]	= 1.5;		laserScannerValuesRear[16]	= 1.5;
		laserScannerValuesFront[17]	= 1.5;		laserScannerValuesRear[17]	= 1.5;
		laserScannerValuesFront[18]	= 1.5;		laserScannerValuesRear[18]	= 1.5;
		laserScannerValuesFront[19]	= 1.5;		laserScannerValuesRear[19]	= 1.5;
		laserScannerValuesFront[20]	= 2.0;		laserScannerValuesRear[20]	= 2.0;
		laserScannerValuesFront[21]	= 2.0;		laserScannerValuesRear[21]	= 2.0;
		laserScannerValuesFront[22]	= 2.0;		laserScannerValuesRear[22]	= 2.0;
		laserScannerValuesFront[23]	= 2.0;		laserScannerValuesRear[23]	= 2.0;
		laserScannerValuesFront[24]	= 2.0;		laserScannerValuesRear[24]	= 2.0;
		laserScannerValuesFront[25]	= 2.0;		laserScannerValuesRear[25]	= 2.0;
		laserScannerValuesFront[26]	= 2.0;		laserScannerValuesRear[26]	= 2.0;
		laserScannerValuesFront[27]	= 2.0;		laserScannerValuesRear[27]	= 2.0;
		laserScannerValuesFront[28]	= 2.0;		laserScannerValuesRear[28]	= 2.0;
		laserScannerValuesFront[29]	= 2.0;		laserScannerValuesRear[29]	= 2.0;
		laserScannerValuesFront[30]	= 2.5;		laserScannerValuesRear[30]	= 2.5;
		laserScannerValuesFront[31]	= 2.5;		laserScannerValuesRear[31]	= 2.5;
		laserScannerValuesFront[32]	= 2.5;		laserScannerValuesRear[32]	= 2.5;
		laserScannerValuesFront[33]	= 2.5;		laserScannerValuesRear[33]	= 2.5;
		laserScannerValuesFront[34]	= 2.5;		laserScannerValuesRear[34]	= 2.5;
		laserScannerValuesFront[35]	= 2.5;		laserScannerValuesRear[35]	= 2.5;
		laserScannerValuesFront[36]	= 2.5;		laserScannerValuesRear[36]	= 2.5;
		laserScannerValuesFront[37]	= 2.5;		laserScannerValuesRear[37]	= 2.5;
		laserScannerValuesFront[38]	= 2.5;		laserScannerValuesRear[38]	= 2.5;
		laserScannerValuesFront[39]	= 2.5;		laserScannerValuesRear[39]	= 2.5;
		laserScannerValuesFront[40]	= 3.25;		laserScannerValuesRear[40]	= 3.25;
		laserScannerValuesFront[41]	= 3.25;		laserScannerValuesRear[41]	= 3.25;
		laserScannerValuesFront[42]	= 3.25;		laserScannerValuesRear[42]	= 3.25;
		laserScannerValuesFront[43]	= 3.25;		laserScannerValuesRear[43]	= 3.25;
		laserScannerValuesFront[44]	= 3.25;		laserScannerValuesRear[44]	= 3.25;
		laserScannerValuesFront[45]	= 3.25;		laserScannerValuesRear[45]	= 3.25;
		laserScannerValuesFront[46]	= 3.25;		laserScannerValuesRear[46]	= 3.25;
		laserScannerValuesFront[47]	= 3.25;		laserScannerValuesRear[47]	= 3.25;
		laserScannerValuesFront[48]	= 3.25;		laserScannerValuesRear[48]	= 3.25;
		laserScannerValuesFront[49]	= 3.25;		laserScannerValuesRear[49]	= 3.25;
		laserScannerValuesFront[50]	= 3.5;		laserScannerValuesRear[50]	= 3.5;
		laserScannerValuesFront[51]	= 3.5;		laserScannerValuesRear[51]	= 3.5;
		laserScannerValuesFront[52]	= 3.5;		laserScannerValuesRear[52]	= 3.5;
		laserScannerValuesFront[53]	= 3.5;		laserScannerValuesRear[53]	= 3.5;
		laserScannerValuesFront[54]	= 3.5;		laserScannerValuesRear[54]	= 3.5;
		laserScannerValuesFront[55]	= 3.5;		laserScannerValuesRear[55]	= 3.5;
		laserScannerValuesFront[56]	= 3.5;		laserScannerValuesRear[56]	= 3.5;
		laserScannerValuesFront[57]	= 3.5;		laserScannerValuesRear[57]	= 3.5;
		laserScannerValuesFront[58]	= 3.5;		laserScannerValuesRear[58]	= 3.5;
		laserScannerValuesFront[59]	= 3.5;		laserScannerValuesRear[59]	= 3.5;
		laserScannerValuesFront[60]	= 4.0;		laserScannerValuesRear[60]	= 4.0;
		laserScannerValuesFront[61]	= 4.0;		laserScannerValuesRear[61]	= 4.0;
		laserScannerValuesFront[62]	= 4.0;		laserScannerValuesRear[62]	= 4.0;
		laserScannerValuesFront[63]	= 4.0;		laserScannerValuesRear[63]	= 4.0;
		laserScannerValuesFront[64]	= 4.0;		laserScannerValuesRear[64]	= 4.0;
		laserScannerValuesFront[65]	= 4.0;		laserScannerValuesRear[65]	= 4.0;
		laserScannerValuesFront[66]	= 4.0;		laserScannerValuesRear[66]	= 4.0;
		laserScannerValuesFront[67]	= 4.0;		laserScannerValuesRear[67]	= 4.0;
		laserScannerValuesFront[68]	= 4.0;		laserScannerValuesRear[68]	= 4.0;
		laserScannerValuesFront[69]	= 4.0;		laserScannerValuesRear[69]	= 4.0;
		laserScannerValuesFront[70]	= 4.5;		laserScannerValuesRear[70]	= 4.5;
		laserScannerValuesFront[71]	= 4.5;		laserScannerValuesRear[71]	= 4.5;
		laserScannerValuesFront[72]	= 4.5;		laserScannerValuesRear[72]	= 4.5;
		laserScannerValuesFront[73]	= 4.5;		laserScannerValuesRear[73]	= 4.5;
		laserScannerValuesFront[74]	= 4.5;		laserScannerValuesRear[74]	= 4.5;
		laserScannerValuesFront[75]	= 4.5;		laserScannerValuesRear[75]	= 4.5;
		laserScannerValuesFront[76]	= 4.5;		laserScannerValuesRear[76]	= 4.5;
		laserScannerValuesFront[77]	= 4.5;		laserScannerValuesRear[77]	= 4.5;
		laserScannerValuesFront[78]	= 4.5;		laserScannerValuesRear[78]	= 4.5;
		laserScannerValuesFront[79]	= 4.5;		laserScannerValuesRear[79]	= 4.5;
		laserScannerValuesFront[80]	= 5.0;		laserScannerValuesRear[80]	= 4.5;
		laserScannerValuesFront[81]	= 5.0;		laserScannerValuesRear[81]	= 5.0;
		laserScannerValuesFront[82]	= 5.0;		laserScannerValuesRear[82]	= 5.0;
		laserScannerValuesFront[83]	= 5.0;		laserScannerValuesRear[83]	= 5.0;
		laserScannerValuesFront[84]	= 5.0;		laserScannerValuesRear[84]	= 5.0;
		laserScannerValuesFront[85]	= 5.0;		laserScannerValuesRear[85]	= 5.0;
		laserScannerValuesFront[86]	= 9.0;		laserScannerValuesRear[86]	= 9.0;
		laserScannerValuesFront[87]	= 9.0;		laserScannerValuesRear[87]	= 9.0;
		laserScannerValuesFront[88]	= 16.2;		laserScannerValuesRear[88]	= 16.2;
		laserScannerValuesFront[89]	= 16.2;		laserScannerValuesRear[89]	= 16.2;
		laserScannerValuesFront[90]	= 9.0;		laserScannerValuesRear[90]	= 9.0;
		laserScannerValuesFront[91]	= 2.5;		laserScannerValuesRear[91]	= 2.5;
		laserScannerValuesFront[92]	= 2.5;		laserScannerValuesRear[92]	= 2.5;
		laserScannerValuesFront[93]	= 2.5;		laserScannerValuesRear[93]	= 2.5;
		laserScannerValuesFront[94]	= 2.5;		laserScannerValuesRear[94]	= 2.5;
		laserScannerValuesFront[95]	= 2.5;		laserScannerValuesRear[95]	= 2.5;
		laserScannerValuesFront[96]	= 2.5;		laserScannerValuesRear[96]	= 2.5;
		laserScannerValuesFront[97]	= 2.5;		laserScannerValuesRear[97]	= 2.5;
		laserScannerValuesFront[98]	= 2.5;		laserScannerValuesRear[98]	= 2.5;
		laserScannerValuesFront[99]	= 2.5;		laserScannerValuesRear[99]	= 2.5;
		laserScannerValuesFront[100]	= 2.0;		laserScannerValuesRear[100]	= 2.0;
		laserScannerValuesFront[101]	= 2.0;		laserScannerValuesRear[101]	= 2.0;
		laserScannerValuesFront[102]	= 2.0;		laserScannerValuesRear[102]	= 2.0;
		laserScannerValuesFront[103]	= 2.0;		laserScannerValuesRear[103]	= 2.0;
		laserScannerValuesFront[104]	= 2.0;		laserScannerValuesRear[104]	= 2.0;
		laserScannerValuesFront[105]	= 2.0;		laserScannerValuesRear[105]	= 2.0;
		laserScannerValuesFront[106]	= 3.0;		laserScannerValuesRear[106]	= 3.0;
		laserScannerValuesFront[107]	= 3.0;		laserScannerValuesRear[107]	= 3.0;
		laserScannerValuesFront[108]	= 3.0;		laserScannerValuesRear[108]	= 3.0;
		laserScannerValuesFront[109]	= 3.0;		laserScannerValuesRear[109]	= 3.0;
		laserScannerValuesFront[110]	= 3.0;		laserScannerValuesRear[110]	= 3.0;
		laserScannerValuesFront[111]	= 3.0;		laserScannerValuesRear[111]	= 3.0;
		laserScannerValuesFront[112]	= 3.0;		laserScannerValuesRear[112]	= 3.0;
		laserScannerValuesFront[113]	= 3.0;		laserScannerValuesRear[113]	= 3.0;
		laserScannerValuesFront[114]	= 3.0;		laserScannerValuesRear[114]	= 3.0;
		laserScannerValuesFront[115]	= 3.0;		laserScannerValuesRear[115]	= 3.0;
		laserScannerValuesFront[116]	= 3.0;		laserScannerValuesRear[116]	= 3.0;
		laserScannerValuesFront[117]	= 3.0;		laserScannerValuesRear[117]	= 3.0;
		laserScannerValuesFront[118]	= 3.0;		laserScannerValuesRear[118]	= 3.0;
		laserScannerValuesFront[119]	= 3.0;		laserScannerValuesRear[119]	= 3.0;
		laserScannerValuesFront[120]	= 2.0;		laserScannerValuesRear[120]	= 2.0;
		laserScannerValuesFront[121]	= 2.0;		laserScannerValuesRear[121]	= 2.0;
		laserScannerValuesFront[122]	= 2.0;		laserScannerValuesRear[122]	= 2.0;
		laserScannerValuesFront[123]	= 2.0;		laserScannerValuesRear[123]	= 2.0;
		laserScannerValuesFront[124]	= 2.0;		laserScannerValuesRear[124]	= 2.0;
		laserScannerValuesFront[125]	= 2.0;		laserScannerValuesRear[125]	= 2.0;
		laserScannerValuesFront[126]	= 2.0;		laserScannerValuesRear[126]	= 2.0;
		laserScannerValuesFront[127]	= 2.0;		laserScannerValuesRear[127]	= 2.0;
		laserScannerValuesFront[128]	= 2.0;		laserScannerValuesRear[128]	= 2.0;
		laserScannerValuesFront[129]	= 2.0;		laserScannerValuesRear[129]	= 2.0;
		laserScannerValuesFront[130]	= 2.0;		laserScannerValuesRear[130]	= 2.0;
		laserScannerValuesFront[131]	= 2.0;		laserScannerValuesRear[131]	= 2.0;
		laserScannerValuesFront[132]	= 2.0;		laserScannerValuesRear[132]	= 2.0;
		laserScannerValuesFront[133]	= 2.0;		laserScannerValuesRear[133]	= 2.0;
		laserScannerValuesFront[134]	= 2.0;		laserScannerValuesRear[134]	= 2.0;
		laserScannerValuesFront[135]	= 2.0;		laserScannerValuesRear[135]	= 2.0;
		laserScannerValuesFront[136]	= 2.0;		laserScannerValuesRear[136]	= 2.0;
		laserScannerValuesFront[137]	= 2.0;		laserScannerValuesRear[137]	= 2.0;
		laserScannerValuesFront[138]	= 2.0;		laserScannerValuesRear[138]	= 2.0;
		laserScannerValuesFront[139]	= 2.0;		laserScannerValuesRear[139]	= 2.0;
		laserScannerValuesFront[140]	= 6.0;		laserScannerValuesRear[140]	= 6.0;
		laserScannerValuesFront[141]	= 6.0;		laserScannerValuesRear[141]	= 6.0;
		laserScannerValuesFront[142]	= 6.0;		laserScannerValuesRear[142]	= 6.0;
		laserScannerValuesFront[143]	= 6.0;		laserScannerValuesRear[143]	= 6.0;
		laserScannerValuesFront[144]	= 6.0;		laserScannerValuesRear[144]	= 6.0;
		laserScannerValuesFront[145]	= 6.0;		laserScannerValuesRear[145]	= 6.0;
		laserScannerValuesFront[146]	= 6.0;		laserScannerValuesRear[146]	= 6.0;
		laserScannerValuesFront[147]	= 6.0;		laserScannerValuesRear[147]	= 6.0;
		laserScannerValuesFront[148]	= 6.0;		laserScannerValuesRear[148]	= 6.0;
		laserScannerValuesFront[149]	= 6.0;		laserScannerValuesRear[149]	= 6.0;
		laserScannerValuesFront[150]	= 6.5;		laserScannerValuesRear[150]	= 6.5;
		laserScannerValuesFront[151]	= 6.5;		laserScannerValuesRear[151]	= 6.5;
		laserScannerValuesFront[152]	= 6.5;		laserScannerValuesRear[152]	= 6.5;
		laserScannerValuesFront[153]	= 6.5;		laserScannerValuesRear[153]	= 6.5;
		laserScannerValuesFront[154]	= 6.5;		laserScannerValuesRear[154]	= 6.5;
		laserScannerValuesFront[155]	= 6.5;		laserScannerValuesRear[155]	= 6.5;
		laserScannerValuesFront[156]	= 6.5;		laserScannerValuesRear[156]	= 6.5;
		laserScannerValuesFront[157]	= 6.5;		laserScannerValuesRear[157]	= 6.5;
		laserScannerValuesFront[158]	= 6.5;		laserScannerValuesRear[158]	= 6.5;
		laserScannerValuesFront[159]	= 10.0;		laserScannerValuesRear[159]	= 10.0;
		laserScannerValuesFront[160]	= 4.0;		laserScannerValuesRear[160]	= 4.0;
		laserScannerValuesFront[161]	= 4.0;		laserScannerValuesRear[161]	= 4.0;
		laserScannerValuesFront[162]	= 4.0;		laserScannerValuesRear[162]	= 4.0;
		laserScannerValuesFront[163]	= 4.0;		laserScannerValuesRear[163]	= 4.0;
		laserScannerValuesFront[164]	= 4.0;		laserScannerValuesRear[164]	= 4.0;
		laserScannerValuesFront[165]	= 4.0;		laserScannerValuesRear[165]	= 4.0;
		laserScannerValuesFront[166]	= 4.0;		laserScannerValuesRear[166]	= 4.0;
		laserScannerValuesFront[167]	= 4.0;		laserScannerValuesRear[167]	= 4.0;
		laserScannerValuesFront[168]	= 4.0;		laserScannerValuesRear[168]	= 4.0;
		laserScannerValuesFront[169]	= 4.0;		laserScannerValuesRear[169]	= 4.0;
		laserScannerValuesFront[170]	= 0.18;		laserScannerValuesRear[170]	= 0.18;
		laserScannerValuesFront[171]	= 0.18;		laserScannerValuesRear[171]	= 0.18;
		laserScannerValuesFront[172]	= 0.18;		laserScannerValuesRear[172]	= 0.18;
		laserScannerValuesFront[173]	= 0.18;		laserScannerValuesRear[173]	= 0.18;
		laserScannerValuesFront[174]	= 0.18;		laserScannerValuesRear[174]	= 0.18;
		laserScannerValuesFront[175]	= 0.18;		laserScannerValuesRear[175]	= 0.18;
		laserScannerValuesFront[176]	= 0.18;		laserScannerValuesRear[176]	= 0.18;
		laserScannerValuesFront[177]	= 0.18;		laserScannerValuesRear[177]	= 0.18;
		laserScannerValuesFront[178]	= 0.18;		laserScannerValuesRear[178]	= 0.18;
		laserScannerValuesFront[179]	= 0.18;		laserScannerValuesRear[179]	= 0.18; // FIXME: and what, if we have 270 degrees?

		// for refreshing the gui (deleting simulated laser lines)
		emit laserDataCompleteFront(&laserScannerValuesFront[0], &laserScannerFlagsFront[0]);
		emit laserDataCompleteRear(&laserScannerValuesRear[0], &laserScannerFlagsRear[0]);
	}
	else
	{
		// initialisation
		for (int i=0; i<LASERSCANNERARRAYSIZE; i++)
		{
			// the distances
			laserScannerValuesFront[i] = 0;
			laserScannerValuesRear[i] = 0;
			// the flags
			laserScannerFlagsFront[i] = 0;
			laserScannerFlagsRear[i] = 0;
		}
		
		// for refreshing the gui (deleting simulated laser lines)
		emit laserDataCompleteFront(&laserScannerValuesFront[0], &laserScannerFlagsFront[0]);
		emit laserDataCompleteRear(&laserScannerValuesRear[0], &laserScannerFlagsRear[0]);
	}
}


void LaserThread::setSerialPort(short int laserScanner, QString serialPort)
{
	switch (laserScanner)
	{
	case LASER1:
		if (laserscannerTypeFront == PLS)
		{
			// for laser.cpp:
			laser->setDevicePort(laserScanner, serialPort);
		}
		else
		{
			// TODO: add S300 stuff
		}
		break;
	case LASER2:
		if (laserscannerTypeRear == PLS)
		{
			// for laser.cpp:
			laser->setDevicePort(laserScanner, serialPort);
		}
		else
		{
			// TODO: add S300 stuff
		}
		break;
	default:
		qDebug("laser number not yet supported (LaserThreadd::setSerialPort");
		break;
	}
}


void LaserThread::setMounting(short int laserScanner, QString mounting)
{
		switch (laserScanner)
		{
			case LASER1:
				mountingLaserscannerFront = mounting;
				break;
			case LASER2:
				mountingLaserscannerRear = mounting;
				break;
			default:
				qDebug("laser number not yet supported (LaserThreadd::setMounting");
				break;
		}
}


void LaserThread::setLaserscannerType(short int laserScanner, QString laserType)
{
		switch (laserScanner)
		{
			case LASER1:
			if (laserType=="LMS")
			{
				laserscannerTypeFront = LMS;
			}
			else
			{
				if (laserType=="PLS")
				{
					laserscannerTypeFront = PLS;
				}
				else
				{
					if (laserType=="S300")
					{
						laserscannerTypeFront = S300;
					}
					else
					{
						qDebug("laser type not yet supported  (LaserThreadd::setLaserscannerType, LASER1");
					}
				}
			}
			break;
		case LASER2:
			if (laserType=="LMS")
			{
				laserscannerTypeFront = LMS;
			}
			else
			{
				if (laserType=="PLS")
				{
					laserscannerTypeFront = PLS;
				}
				else
				{
					if (laserType=="S300")
					{
						laserscannerTypeFront = S300;
					}
					else
					{
						qDebug("laser type not yet supported (LaserThreadd::setLaserscannerType, LASER1");
					}
				}
			}
			break;
		default:
			qDebug("laser number not yet supported  (LaserThreadd::setLaserscannerType");
			break;
		}
}


void LaserThread::setLaserscannerAngle(short int laserScanner, int angle)
{
		switch (laserScanner)
		{
			case LASER1:
			// FIXME: do something with the angle!!
			break;
		case LASER2:
			// FIXME: do something with the angle!!
			break;
		default:
			qDebug("laser number not yet supported  (LaserThreadd::setLaserscannerAngle");
			break;
		}
}


bool LaserThread::isConnected(short int laserScanner)
{
	switch (laserScanner)
	{
	case LASER1:
		if (laserscannerTypeFront == PLS)
		{

			// this is the 'old' laser stuff
			laser = new Laser();

			// let the splash screen from the direcs class show laser init messages
			// (connect the signal from the laser class to the signal from this class)
			connect(laser, SIGNAL(message(QString)), this, SIGNAL(message(QString)));

			// try to start the laser module LASER1
			if (laser->direcs_laser_start(laserScanner) == 0)
			{
				laserScannerFrontIsConnected = true;
				return true;
			}
			else
			{
				laserScannerFrontIsConnected = false;
				return false;
			}
		}
		else
		{
			if (laserscannerTypeFront == S300)
			{
				laserS300 = new SickS300();

				// S300 stuff
				// connect to serial port etc.
				if (laserS300->setup() == 0) // FIXME: what if we have 2 scanners?
				{
					laserScannerFrontIsConnected = true;
					return true;
				}

				laserScannerFrontIsConnected = false;
				return false;
			}
			else
			{
				qDebug("ERROR: Unsupported laser typ in laserThread.cpp chosen!");
				laserScannerFrontIsConnected = false;
				return false;
			}
		}
		break;
	case LASER2:
		if (laserscannerTypeFront == PLS)
		{
			// try to start the laser module LASER2
			if (laser->direcs_laser_start(laserScanner) == 0)
			{
				laserScannerRearIsConnected = true;
				return true;
			}
			else
			{
				laserScannerRearIsConnected = false;
				return false;
			}
		}
		else
		{
			if (laserscannerTypeFront == S300)
			{
				// TODO: S300 stuff
				qDebug("Support for S300 as rear laser scanner not implemented yet");
				laserScannerRearIsConnected = false;
				return false;
			}
			else
			{
				qDebug("ERROR: Unsupported laser typ in laserThread.cpp chosen!");
				laserScannerRearIsConnected = false;
				return false;
			}
		}
		break;
	default:
		qDebug("laser number not yet supported (LaserThreadd::setMounting");
		break;
	}


	stopped = true; // don't run the thread!
	return false;
}
