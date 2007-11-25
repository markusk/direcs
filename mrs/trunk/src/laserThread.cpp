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
	//QMutexLocker locker(&mutex); // make this class thread-safe
	numReadings = 0;
	stopped = false;
	simulationMode = false;


	// initialisation
	for (int i=0; i<LASERSCANNERARRAYSIZE; i++)
	{
		// add a 0 m laser distance
		laserScannerValues.append(0);
		
		// add a 0 flag
		laserScannerFlags.append(0);
	}
}


LaserThread::~LaserThread()
{
	stopped = false;
	
	// shutdown laser
	// the parameter '0' is unused!!
	carmen_laser_shutdown(0);
	
	//QMutexLocker locker(&mutex); // make this class thread-safe
}


void LaserThread::stop()
{
	stopped = true;
	numReadings = 0;
}


void LaserThread::run()
{
	static bool initialized = false;
	int laserValue = 0;
//	static carmen_laser_laser_message msg; // from laser_ipc.c, publish_laser_message


	// initialze the laser scanner
	if ((initialized == false) && (simulationMode==false))
	{
		initialized = true;
		//----------------------------------------------
		// start the (former CARMEN) laser module here!
		//----------------------------------------------
		// 0. server_initialize			-> NO
		// 1. carmen_ipc_initialize		-> NO
		// 2. carmen_param_check_version	-> NO
		// 3. carmen_laser_start		-> YES
		if ( carmen_laser_start() != 0 )
		{
			qDebug("No access to serial port of the laser scanner... Stopping LaserThread!");
			
			// stopping laser Thread!
			stopped = true;
		}
		// 4. carmen_laser_run			-> YES (scroll down!)
		
		// 5. carmen_ipc_sleep			-> NO
		//
		// AUS EIGENER laser.cpp
		// 6. carmen_ipc_initialize(argc, argv);-> NO
		// 7. laser_num  = 0;
		// 8. carmen_laser_subscribe_frontlaser_message (&laser, (carmen_handler_t)laser_handler, CARMEN_SUBSCRIBE_LATEST);
		// 9. start_time = carmen_get_time();
		// 10.carmen_graphics_update_ipc_callbacks((GdkInputFunction)updateIPC);
		// 11. gtk_main();
	}
	
	
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
			// CARMEN laser module
			laserValue = carmen_laser_run();
			
			// TODO: get value FROM laserValue

			// check if all 180 beams were read (in the laser module)
			// TODO: case for LASER1,...n
			numReadings = getLaserNumReadings(LASER1);
			
			// numReadings can't be over the number of elements in the QList 'laserScannerValues'!!
			// TODO: 180 elements vs. const vs. 
			if (numReadings>=180)
				numReadings = 179;
			
			// if YES
			if (numReadings > 0)
			{
				// /get the data from 0° to 180° (left to right)
				for (int i=0; i<numReadings; i++)
				{
					// get value from laser
					// store the value in an array in this thread
					// TODO: case for LASER1,...n
					laserScannerValues[i] = getLaserDistance(LASER1, i);
				}
				
				
				//====================================================================
				//  e m i t  Signal
				//====================================================================
				emit laserDataComplete(&laserScannerValues[0], &laserScannerFlags[0]);
			}
		}
		else
		{
			//---------------------------------
			// S I M U L A T I O N   M O D E
			//---------------------------------
			numReadings = 180;
			
				emit laserDataComplete(&laserScannerValues[0], &laserScannerFlags[0]);
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
	
	
	//return laserScannerValue[angle][DISTANCE];
	return laserScannerValues.at(angle);
}


float LaserThread::getLaserScannerFlag(int angle)
{
	if ((angle < 0) || (angle > LASERSCANNERARRAYSIZE-1))
	{
	  return 0;
	}
	
	
	//return laserScannerValue[angle][FLAG];
	return laserScannerFlags.at(angle);
}


void LaserThread::setLaserScannerFlag(int angle, int flag)
{
	if ((angle < 0) || (angle > LASERSCANNERARRAYSIZE-1))
	{
	  return;
	}
	
	
	laserScannerFlags[angle] = flag;
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
		laserScannerValues[0]	= 0.25;
		laserScannerValues[1]	= 0.25;
		laserScannerValues[2]	= 0.25;
		laserScannerValues[3]	= 0.25;
		laserScannerValues[4]	= 0.25;
		laserScannerValues[5]	= 0.25;
		laserScannerValues[6]	= 0.25;
		laserScannerValues[7]	= 0.25;
		laserScannerValues[8]	= 0.25;
		laserScannerValues[9]	= 0.25;
		laserScannerValues[10]	= 1.5;
		laserScannerValues[11]	= 1.5;
		laserScannerValues[12]	= 1.5;
		laserScannerValues[13]	= 1.5;
		laserScannerValues[14]	= 1.5;
		laserScannerValues[15]	= 1.5;
		laserScannerValues[16]	= 1.5;
		laserScannerValues[17]	= 1.5;
		laserScannerValues[18]	= 1.5;
		laserScannerValues[19]	= 1.5;
		laserScannerValues[20]	= 2.0;
		laserScannerValues[21]	= 2.0;
		laserScannerValues[22]	= 2.0;
		laserScannerValues[23]	= 2.0;
		laserScannerValues[24]	= 2.0;
		laserScannerValues[25]	= 2.0;
		laserScannerValues[26]	= 2.0;
		laserScannerValues[27]	= 2.0;
		laserScannerValues[28]	= 2.0;
		laserScannerValues[29]	= 2.0;
		laserScannerValues[30]	= 2.5;
		laserScannerValues[31]	= 2.5;
		laserScannerValues[32]	= 2.5;
		laserScannerValues[33]	= 2.5;
		laserScannerValues[34]	= 2.5;
		laserScannerValues[35]	= 2.5;
		laserScannerValues[36]	= 2.5;
		laserScannerValues[37]	= 2.5;
		laserScannerValues[38]	= 2.5;
		laserScannerValues[39]	= 2.5;
		laserScannerValues[40]	= 3.25;
		laserScannerValues[41]	= 3.25;
		laserScannerValues[42]	= 3.25;
		laserScannerValues[43]	= 3.25;
		laserScannerValues[44]	= 3.25;
		laserScannerValues[45]	= 3.25;
		laserScannerValues[46]	= 3.25;
		laserScannerValues[47]	= 3.25;
		laserScannerValues[48]	= 3.25;
		laserScannerValues[49]	= 3.25;
		laserScannerValues[50]	= 3.5;
		laserScannerValues[51]	= 3.5;
		laserScannerValues[52]	= 3.5;
		laserScannerValues[53]	= 3.5;
		laserScannerValues[54]	= 3.5;
		laserScannerValues[55]	= 3.5;
		laserScannerValues[56]	= 3.5;
		laserScannerValues[57]	= 3.5;
		laserScannerValues[58]	= 3.5;
		laserScannerValues[59]	= 3.5;
		laserScannerValues[60]	= 4.0;
		laserScannerValues[61]	= 4.0;
		laserScannerValues[62]	= 4.0;
		laserScannerValues[63]	= 4.0;
		laserScannerValues[64]	= 4.0;
		laserScannerValues[65]	= 4.0;
		laserScannerValues[66]	= 4.0;
		laserScannerValues[67]	= 4.0;
		laserScannerValues[68]	= 4.0;
		laserScannerValues[69]	= 4.0;
		laserScannerValues[70]	= 4.5;
		laserScannerValues[71]	= 4.5;
		laserScannerValues[72]	= 4.5;
		laserScannerValues[73]	= 4.5;
		laserScannerValues[74]	= 4.5;
		laserScannerValues[75]	= 4.5;
		laserScannerValues[76]	= 4.5;
		laserScannerValues[77]	= 4.5;
		laserScannerValues[78]	= 4.5;
		laserScannerValues[79]	= 4.5;
		laserScannerValues[80]	= 5.0;
		laserScannerValues[81]	= 5.0;
		laserScannerValues[82]	= 5.0;
		laserScannerValues[83]	= 5.0;
		laserScannerValues[84]	= 5.0;
		laserScannerValues[85]	= 5.0;
		laserScannerValues[86]	= 9.0;
		laserScannerValues[87]	= 9.0;
		laserScannerValues[88]	= 9.0;
		laserScannerValues[89]	= 9.0;
		laserScannerValues[90]	= 2.5;
		laserScannerValues[91]	= 2.5;
		laserScannerValues[92]	= 2.5;
		laserScannerValues[93]	= 2.5;
		laserScannerValues[94]	= 2.5;
		laserScannerValues[95]	= 2.5;
		laserScannerValues[96]	= 2.5;
		laserScannerValues[97]	= 2.5;
		laserScannerValues[98]	= 2.5;
		laserScannerValues[99]	= 2.5;
		laserScannerValues[100]	= 2.0;
		laserScannerValues[101]	= 2.0;
		laserScannerValues[102]	= 2.0;
		laserScannerValues[103]	= 2.0;
		laserScannerValues[104]	= 2.0;
		laserScannerValues[105]	= 2.0;
		laserScannerValues[106]	= 3.0;
		laserScannerValues[107]	= 3.0;
		laserScannerValues[108]	= 3.0;
		laserScannerValues[109]	= 3.0;
		laserScannerValues[110]	= 3.0;
		laserScannerValues[111]	= 3.0;
		laserScannerValues[112]	= 3.0;
		laserScannerValues[113]	= 3.0;
		laserScannerValues[114]	= 3.0;
		laserScannerValues[115]	= 3.0;
		laserScannerValues[116]	= 3.0;
		laserScannerValues[117]	= 3.0;
		laserScannerValues[118]	= 3.0;
		laserScannerValues[119]	= 3.0;
		laserScannerValues[120]	= 2.0;
		laserScannerValues[121]	= 2.0;
		laserScannerValues[122]	= 2.0;
		laserScannerValues[123]	= 2.0;
		laserScannerValues[124]	= 2.0;
		laserScannerValues[125]	= 2.0;
		laserScannerValues[126]	= 2.0;
		laserScannerValues[127]	= 2.0;
		laserScannerValues[128]	= 2.0;
		laserScannerValues[129]	= 2.0;
		laserScannerValues[130]	= 2.0;
		laserScannerValues[131]	= 2.0;
		laserScannerValues[132]	= 2.0;
		laserScannerValues[133]	= 2.0;
		laserScannerValues[134]	= 2.0;
		laserScannerValues[135]	= 2.0;
		laserScannerValues[136]	= 2.0;
		laserScannerValues[137]	= 2.0;
		laserScannerValues[138]	= 2.0;
		laserScannerValues[139]	= 2.0;
		laserScannerValues[140]	= 6.0;
		laserScannerValues[141]	= 6.0;
		laserScannerValues[142]	= 6.0;
		laserScannerValues[143]	= 6.0;
		laserScannerValues[144]	= 6.0;
		laserScannerValues[145]	= 6.0;
		laserScannerValues[146]	= 6.0;
		laserScannerValues[147]	= 6.0;
		laserScannerValues[148]	= 6.0;
		laserScannerValues[149]	= 6.0;
		laserScannerValues[150]	= 9.0;
		laserScannerValues[151]	= 9.0;
		laserScannerValues[152]	= 9.0;
		laserScannerValues[153]	= 9.0;
		laserScannerValues[154]	= 9.0;
		laserScannerValues[155]	= 9.0;
		laserScannerValues[156]	= 9.0;
		laserScannerValues[157]	= 9.0;
		laserScannerValues[158]	= 9.0;
		laserScannerValues[159]	= 9.0;
		laserScannerValues[160]	= 4.0;
		laserScannerValues[161]	= 4.0;
		laserScannerValues[162]	= 4.0;
		laserScannerValues[163]	= 4.0;
		laserScannerValues[164]	= 4.0;
		laserScannerValues[165]	= 4.0;
		laserScannerValues[166]	= 4.0;
		laserScannerValues[167]	= 4.0;
		laserScannerValues[168]	= 4.0;
		laserScannerValues[169]	= 4.0;
		laserScannerValues[170]	= 0.25;
		laserScannerValues[171]	= 0.25;
		laserScannerValues[172]	= 0.25;
		laserScannerValues[173]	= 0.25;
		laserScannerValues[174]	= 0.25;
		laserScannerValues[175]	= 0.25;
		laserScannerValues[176]	= 0.25;
		laserScannerValues[177]	= 0.25;
		laserScannerValues[178]	= 0.25;
		laserScannerValues[179]	= 0.25;
	}
	else
	{
		// initialisation
		for (int i=0; i<LASERSCANNERARRAYSIZE; i++)
		{
			// the distances
			laserScannerValues[i] = 0;
			// the flags
			laserScannerFlags[i] = 0;
		}
		
		// for refreshing the gui (deleting simulated laser lines)
		emit laserDataComplete(&laserScannerValues[0], &laserScannerFlags[0]);
	}
}
