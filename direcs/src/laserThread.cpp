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
	laserscannerAngleFront = 0;
	laserscannerAngleRear = 0;

	// initialisation of 360 laser simulation values (QList). Add a QList-value. One for each angle
	for (int i=0; i<360; i++)
	{
		// add a 0 m laser distance
		simulationValuesFront[i] = 0;
		simulationValuesRear[i]  = 0;
	}
}


LaserThread::~LaserThread()
{
	// laserScannerFrontIsConnected is set in the isConnected method!
	if (laserScannerFrontIsConnected || laserScannerRearIsConnected)
	{
			// This shuts down *all* PLS lasers (managed internaly in laser.cpp)!
		if ((laserscannerTypeFront == PLS) || ((laserscannerTypeRear == PLS)) )
		{
			// shutdown laser (parameter '0' is not in use)
			laser->direcs_laser_shutdown(0);
			delete laser;
		}
		else
		{
		if ((laserscannerTypeFront == S300) || ((laserscannerTypeRear == S300)) )
			{
				delete laserS300;
			}
		}
	}
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
			numReadingsFront = laserscannerAngleFront;
			numReadingsRear = laserscannerAngleRear;
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
		if (numReadingsFront > laserscannerAngleFront)
			numReadingsFront = laserscannerAngleFront;
		
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
		if (numReadingsRear > laserscannerAngleRear)
			numReadingsRear = laserscannerAngleRear;

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
	// These values were NOT converted at any point in this method!
	
	if ((angle < 0) || (laserScanner==LASER1 &&  (angle > laserscannerAngleFront)) || (laserScanner==LASER2 &&  (angle > laserscannerAngleRear)) )
	{
		qDebug("laser angle out of alowed range (LaserThread::getLaserScannerValue)");
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


float LaserThread::getFlag(short int laserScanner, int angle)
{
	if ((angle < 0) || ((laserScanner==LASER1) &&  (angle > laserscannerAngleFront)) || ((laserScanner==LASER2) &&  (angle > laserscannerAngleRear)) )
	{
		qDebug("laser angle out of alowed range (LaserThread::getLaserScannerFlag)");
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


void LaserThread::setFlag(short int laserScanner, int angle, int flag)
{
	if ((angle < 0) || (laserScanner==LASER1 &&  (angle > laserscannerAngleFront)) || (laserScanner==LASER2 &&  (angle > laserscannerAngleRear)) )
	{
		qDebug("laser angle out of alowed range (LaserThread::setLaserScannerFlag)");
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
		numReadingsFront = laserscannerAngleFront;
		numReadingsRear = laserscannerAngleRear;
		// sim value init
		// now, that we know, how many laserlines we have, this should be safe to call here.
		setSimulationValues();

		// copy sim values into the distances values
		for (int i=0; i<laserScannerValuesFront.size()-1; i++)
		{
			// the distances
			// laserScannerValuesFront[i] = 2.30; //(i+1) / 100;
			laserScannerValuesFront[i] = simulationValuesFront[i];
			// the flags
			laserScannerFlagsFront[i] = OBSTACLE;
		}

		for (int i=0; i<laserScannerValuesRear.size()-1; i++)
		{
			// the distances
			// laserScannerValuesRear[i] = 2.30; //(i+1) / 100;
			laserScannerValuesRear[i] = simulationValuesRear[i];
			// the flags
			laserScannerFlagsRear[i] = OBSTACLE;
		}

		// for refreshing the gui (deleting simulated laser lines)
		emit laserDataCompleteFront(&laserScannerValuesFront[0], &laserScannerFlagsFront[0]);
		emit laserDataCompleteRear(&laserScannerValuesRear[0], &laserScannerFlagsRear[0]);
	}
	else
	{
		// initialisation
		for (int i=0; i<laserScannerValuesFront.size()-1; i++)
		{
			// the distances
			laserScannerValuesFront[i] = 0;
			// the flags
			laserScannerFlagsFront[i] = OBSTACLE;
		}

		for (int i=0; i<laserScannerValuesRear.size()-1; i++)
		{
			// the distances
			laserScannerValuesRear[i] = 0;
			// the flags
			laserScannerFlagsRear[i] = OBSTACLE;
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
						if (laserType=="none")
						{
							laserscannerTypeFront = NONE;
						}
						else
						{
							qDebug("laser type not yet supported  (LaserThreadd::setLaserscannerType, LASER1");
						}
					}
				}
			}
			break;
		case LASER2:
			if (laserType=="LMS")
			{
				laserscannerTypeRear = LMS;
			}
			else
			{
				if (laserType=="PLS")
				{
					laserscannerTypeRear = PLS;
				}
				else
				{
					if (laserType=="S300")
					{
						laserscannerTypeRear = S300;
					}
					else
					{
						if (laserType=="none")
						{
							laserscannerTypeRear = NONE;
						}
						else
						{
							qDebug("laser type not yet supported  (LaserThreadd::setLaserscannerType, LASER2");
						}
					}
				}
			}
			break;
		default:
			qDebug("laser number not yet supported  (LaserThreadd::setLaserscannerType");
			break;
		}


		// create the laser objects
		if ((laserType=="PLS") || (laserType=="LMS"))
		{
			// this is the 'old' laser stuff for the LMS or PLS
			laser = new Laser();

			// let the splash screen from the direcs class show laser init messages
			// (connect the signal from the laser class to the signal from this class)
			connect(laser, SIGNAL(message(QString)), this, SIGNAL(message(QString)));

			return;
		}

		// S300 stuff
		if  (laserType=="S300")
		{
			laserS300 = new SickS300();

			return;
		}
}


void LaserThread::setLaserscannerAngle(short int laserScanner, int angle)
{
		switch (laserScanner)
		{
			case LASER1:
			// store value in the class member
			laserscannerAngleFront = angle;
			// initialisation of the laser values list (QList). Add a QList-value. One  for eache angle
			for (int i=0; i<angle; i++)
			{
				// add a 0 m laser distance
				laserScannerValuesFront.append(0);
				// add a 0 flag
				laserScannerFlagsFront.append(0);
			}
			//qDebug("list size laserScannerValuesFront, Laser%d: %d", laserScanner, laserScannerValuesFront.size());
			//qDebug("list size laserScannerFlagsFront, Laser%d: %d", laserScanner, laserScannerFlagsFront.size());
			break;
		case LASER2:
			// store value in the class member
			laserscannerAngleRear = angle;
			// initialisation of the laser values list (QList). Add a QList-value. One  for eache angle
			for (int i=0; i<angle; i++)
			{
				// add a 0 m laser distance
				laserScannerValuesRear.append(0);
				// add a 0 flag
				laserScannerFlagsRear.append(0);
			}
			//qDebug("list size laserScannerValuesRear, Laser%d: %d", laserScanner, laserScannerValuesRear.size());
			//qDebug("list size laserScannerFlagsRear, Laser%d: %d", laserScanner, laserScannerFlagsRear.size());
			break;
		default:
			qDebug("laser number not yet supported  (LaserThread::setLaserscannerAngle");
			break;
		}
}


int LaserThread::getAngle(short int laserScanner)
{
		switch (laserScanner)
		{
		case LASER1:
			return laserscannerAngleFront;
			break;
		case LASER2:
			return laserscannerAngleRear;
			break;
		}

		qDebug("laser number not yet supported  (LaserThreadd::getAngle");
		return 0;
}


bool LaserThread::isConnected(short int laserScanner)
{
	switch (laserScanner)
	{
	case LASER1:
		if (laserscannerTypeFront == PLS)
		{
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
				if (laserscannerTypeFront != NONE)
				{
					qDebug("ERROR: Unsupported laser type! (LaserThread::isConnected)");
				}
				laserScannerFrontIsConnected = false;
				return false;
			}
		}
		break;
	case LASER2:
		if (laserscannerTypeRear == PLS)
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
			if (laserscannerTypeRear == S300)
			{
				// TODO: S300 stuff
				qDebug("Support for S300 as rear laser scanner not implemented yet");
				laserScannerRearIsConnected = false;
				return false;
			}
			else
			{
				if (laserscannerTypeRear != NONE)
				{
					qDebug("ERROR: Unsupported laser type! (LaserThread::isConnected)");
				}
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


void LaserThread::setSimulationValues()
{
	// fill the simulation value array with some nice values
	simulationValuesFront[0]	= 2.07;		simulationValuesRear[0]	= 0.25;
	simulationValuesFront[1]	= 2.07;		simulationValuesRear[1]	= 0.25;
	simulationValuesFront[2]	= 2.07;		simulationValuesRear[2]	= 0.25;
	simulationValuesFront[3]	= 2.07;		simulationValuesRear[3]	= 0.25;
	simulationValuesFront[4]	= 2.07;		simulationValuesRear[4]	= 0.25;
	simulationValuesFront[5]	= 2.07;		simulationValuesRear[5]	= 0.25;
	simulationValuesFront[6]	= 2.07;		simulationValuesRear[6]	= 0.25;
	simulationValuesFront[7]	= 2.07;		simulationValuesRear[7]	= 0.25;
	simulationValuesFront[8]	= 2.07;		simulationValuesRear[8]	= 0.25;
	simulationValuesFront[9]	= 2.07;		simulationValuesRear[9]	= 0.25;
	simulationValuesFront[10]	= 1.5;		simulationValuesRear[10]	= 0.25;
	simulationValuesFront[11]	= 1.5;		simulationValuesRear[11]	= 0.25;
	simulationValuesFront[12]	= 1.5;		simulationValuesRear[12]	= 1.5;
	simulationValuesFront[13]	= 1.5;		simulationValuesRear[13]	= 1.5;
	simulationValuesFront[14]	= 1.5;		simulationValuesRear[14]	= 1.5;
	simulationValuesFront[15]	= 1.5;		simulationValuesRear[15]	= 1.5;
	simulationValuesFront[16]	= 1.5;		simulationValuesRear[16]	= 1.5;
	simulationValuesFront[17]	= 1.5;		simulationValuesRear[17]	= 1.5;
	simulationValuesFront[18]	= 1.5;		simulationValuesRear[18]	= 1.5;
	simulationValuesFront[19]	= 1.5;		simulationValuesRear[19]	= 1.5;
	simulationValuesFront[20]	= 2.0;		simulationValuesRear[20]	= 2.0;
	simulationValuesFront[21]	= 2.0;		simulationValuesRear[21]	= 2.0;
	simulationValuesFront[22]	= 2.0;		simulationValuesRear[22]	= 2.0;
	simulationValuesFront[23]	= 2.0;		simulationValuesRear[23]	= 2.0;
	simulationValuesFront[24]	= 2.0;		simulationValuesRear[24]	= 2.0;
	simulationValuesFront[25]	= 2.0;		simulationValuesRear[25]	= 2.0;
	simulationValuesFront[26]	= 2.0;		simulationValuesRear[26]	= 2.0;
	simulationValuesFront[27]	= 2.0;		simulationValuesRear[27]	= 2.0;
	simulationValuesFront[28]	= 2.0;		simulationValuesRear[28]	= 2.0;
	simulationValuesFront[29]	= 2.0;		simulationValuesRear[29]	= 2.0;
	simulationValuesFront[30]	= 2.5;		simulationValuesRear[30]	= 2.5;
	simulationValuesFront[31]	= 2.5;		simulationValuesRear[31]	= 2.5;
	simulationValuesFront[32]	= 2.5;		simulationValuesRear[32]	= 2.5;
	simulationValuesFront[33]	= 2.5;		simulationValuesRear[33]	= 2.5;
	simulationValuesFront[34]	= 2.5;		simulationValuesRear[34]	= 2.5;
	simulationValuesFront[35]	= 2.5;		simulationValuesRear[35]	= 2.5;
	simulationValuesFront[36]	= 2.5;		simulationValuesRear[36]	= 2.5;
	simulationValuesFront[37]	= 2.5;		simulationValuesRear[37]	= 2.5;
	simulationValuesFront[38]	= 2.5;		simulationValuesRear[38]	= 2.5;
	simulationValuesFront[39]	= 2.5;		simulationValuesRear[39]	= 2.5;
	simulationValuesFront[40]	= 3.25;		simulationValuesRear[40]	= 3.25;
	simulationValuesFront[41]	= 3.25;		simulationValuesRear[41]	= 3.25;
	simulationValuesFront[42]	= 3.25;		simulationValuesRear[42]	= 3.25;
	simulationValuesFront[43]	= 3.25;		simulationValuesRear[43]	= 3.25;
	simulationValuesFront[44]	= 3.25;		simulationValuesRear[44]	= 3.25;
	simulationValuesFront[45]	= 3.25;		simulationValuesRear[45]	= 3.25;
	simulationValuesFront[46]	= 3.25;		simulationValuesRear[46]	= 3.25;
	simulationValuesFront[47]	= 3.25;		simulationValuesRear[47]	= 3.25;
	simulationValuesFront[48]	= 3.25;		simulationValuesRear[48]	= 3.25;
	simulationValuesFront[49]	= 3.25;		simulationValuesRear[49]	= 3.25;
	simulationValuesFront[50]	= 3.5;		simulationValuesRear[50]	= 3.5;
	simulationValuesFront[51]	= 3.5;		simulationValuesRear[51]	= 3.5;
	simulationValuesFront[52]	= 3.5;		simulationValuesRear[52]	= 3.5;
	simulationValuesFront[53]	= 3.5;		simulationValuesRear[53]	= 3.5;
	simulationValuesFront[54]	= 3.5;		simulationValuesRear[54]	= 3.5;
	simulationValuesFront[55]	= 3.5;		simulationValuesRear[55]	= 3.5;
	simulationValuesFront[56]	= 3.5;		simulationValuesRear[56]	= 3.5;
	simulationValuesFront[57]	= 3.5;		simulationValuesRear[57]	= 3.5;
	simulationValuesFront[58]	= 3.5;		simulationValuesRear[58]	= 3.5;
	simulationValuesFront[59]	= 3.5;		simulationValuesRear[59]	= 3.5;
	simulationValuesFront[60]	= 4.0;		simulationValuesRear[60]	= 4.0;
	simulationValuesFront[61]	= 4.0;		simulationValuesRear[61]	= 4.0;
	simulationValuesFront[62]	= 4.0;		simulationValuesRear[62]	= 4.0;
	simulationValuesFront[63]	= 4.0;		simulationValuesRear[63]	= 4.0;
	simulationValuesFront[64]	= 4.0;		simulationValuesRear[64]	= 4.0;
	simulationValuesFront[65]	= 4.0;		simulationValuesRear[65]	= 4.0;
	simulationValuesFront[66]	= 4.0;		simulationValuesRear[66]	= 4.0;
	simulationValuesFront[67]	= 4.0;		simulationValuesRear[67]	= 4.0;
	simulationValuesFront[68]	= 4.0;		simulationValuesRear[68]	= 4.0;
	simulationValuesFront[69]	= 4.0;		simulationValuesRear[69]	= 4.0;
	simulationValuesFront[70]	= 4.5;		simulationValuesRear[70]	= 4.5;
	simulationValuesFront[71]	= 4.5;		simulationValuesRear[71]	= 4.5;
	simulationValuesFront[72]	= 4.5;		simulationValuesRear[72]	= 4.5;
	simulationValuesFront[73]	= 4.5;		simulationValuesRear[73]	= 4.5;
	simulationValuesFront[74]	= 4.5;		simulationValuesRear[74]	= 4.5;
	simulationValuesFront[75]	= 4.5;		simulationValuesRear[75]	= 4.5;
	simulationValuesFront[76]	= 4.5;		simulationValuesRear[76]	= 4.5;
	simulationValuesFront[77]	= 4.5;		simulationValuesRear[77]	= 4.5;
	simulationValuesFront[78]	= 4.5;		simulationValuesRear[78]	= 4.5;
	simulationValuesFront[79]	= 4.5;		simulationValuesRear[79]	= 4.5;
	simulationValuesFront[80]	= 5.0;		simulationValuesRear[80]	= 5.0;
	simulationValuesFront[81]	= 5.0;		simulationValuesRear[81]	= 5.0;
	simulationValuesFront[82]	= 5.0;		simulationValuesRear[82]	= 5.0;
	simulationValuesFront[83]	= 5.0;		simulationValuesRear[83]	= 5.0;
	simulationValuesFront[84]	= 5.0;		simulationValuesRear[84]	= 5.0;
	simulationValuesFront[85]	= 5.0;		simulationValuesRear[85]	= 5.0;
	simulationValuesFront[86]	= 9.0;		simulationValuesRear[86]	= 9.0;
	simulationValuesFront[87]	= 9.0;		simulationValuesRear[87]	= 9.0;
	simulationValuesFront[88]	= 16.2;		simulationValuesRear[88]	= 16.2;
	simulationValuesFront[89]	= 16.2;		simulationValuesRear[89]	= 16.2;
	simulationValuesFront[90]	= 9.0;		simulationValuesRear[90]	= 9.0;
	simulationValuesFront[91]	= 2.5;		simulationValuesRear[91]	= 2.5;
	simulationValuesFront[92]	= 2.5;		simulationValuesRear[92]	= 2.5;
	simulationValuesFront[93]	= 2.5;		simulationValuesRear[93]	= 2.5;
	simulationValuesFront[94]	= 2.5;		simulationValuesRear[94]	= 2.5;
	simulationValuesFront[95]	= 2.5;		simulationValuesRear[95]	= 2.5;
	simulationValuesFront[96]	= 2.5;		simulationValuesRear[96]	= 2.5;
	simulationValuesFront[97]	= 2.5;		simulationValuesRear[97]	= 2.5;
	simulationValuesFront[98]	= 2.5;		simulationValuesRear[98]	= 2.5;
	simulationValuesFront[99]	= 2.5;		simulationValuesRear[99]	= 2.5;
	simulationValuesFront[100]	= 2.0;		simulationValuesRear[100]	= 2.0;
	simulationValuesFront[101]	= 2.0;		simulationValuesRear[101]	= 2.0;
	simulationValuesFront[102]	= 2.0;		simulationValuesRear[102]	= 2.0;
	simulationValuesFront[103]	= 2.0;		simulationValuesRear[103]	= 2.0;
	simulationValuesFront[104]	= 2.0;		simulationValuesRear[104]	= 2.0;
	simulationValuesFront[105]	= 2.0;		simulationValuesRear[105]	= 2.0;
	simulationValuesFront[106]	= 3.0;		simulationValuesRear[106]	= 3.0;
	simulationValuesFront[107]	= 3.0;		simulationValuesRear[107]	= 3.0;
	simulationValuesFront[108]	= 3.0;		simulationValuesRear[108]	= 3.0;
	simulationValuesFront[109]	= 3.0;		simulationValuesRear[109]	= 3.0;
	simulationValuesFront[110]	= 3.0;		simulationValuesRear[110]	= 3.0;
	simulationValuesFront[111]	= 3.0;		simulationValuesRear[111]	= 3.0;
	simulationValuesFront[112]	= 3.0;		simulationValuesRear[112]	= 3.0;
	simulationValuesFront[113]	= 3.0;		simulationValuesRear[113]	= 3.0;
	simulationValuesFront[114]	= 3.0;		simulationValuesRear[114]	= 3.0;
	simulationValuesFront[115]	= 3.0;		simulationValuesRear[115]	= 3.0;
	simulationValuesFront[116]	= 3.0;		simulationValuesRear[116]	= 3.0;
	simulationValuesFront[117]	= 3.0;		simulationValuesRear[117]	= 3.0;
	simulationValuesFront[118]	= 3.0;		simulationValuesRear[118]	= 3.0;
	simulationValuesFront[119]	= 3.0;		simulationValuesRear[119]	= 3.0;
	simulationValuesFront[120]	= 2.0;		simulationValuesRear[120]	= 2.0;
	simulationValuesFront[121]	= 2.0;		simulationValuesRear[121]	= 2.0;
	simulationValuesFront[122]	= 2.0;		simulationValuesRear[122]	= 2.0;
	simulationValuesFront[123]	= 2.0;		simulationValuesRear[123]	= 2.0;
	simulationValuesFront[124]	= 2.0;		simulationValuesRear[124]	= 2.0;
	simulationValuesFront[125]	= 2.0;		simulationValuesRear[125]	= 2.0;
	simulationValuesFront[126]	= 2.0;		simulationValuesRear[126]	= 2.0;
	simulationValuesFront[127]	= 2.0;		simulationValuesRear[127]	= 2.0;
	simulationValuesFront[128]	= 2.0;		simulationValuesRear[128]	= 2.0;
	simulationValuesFront[129]	= 2.0;		simulationValuesRear[129]	= 2.0;
	simulationValuesFront[130]	= 2.0;		simulationValuesRear[130]	= 2.0;
	simulationValuesFront[131]	= 2.0;		simulationValuesRear[131]	= 2.0;
	simulationValuesFront[132]	= 2.0;		simulationValuesRear[132]	= 2.0;
	simulationValuesFront[133]	= 2.0;		simulationValuesRear[133]	= 2.0;
	simulationValuesFront[134]	= 2.0;		simulationValuesRear[134]	= 2.0;
	simulationValuesFront[135]	= 2.0;		simulationValuesRear[135]	= 2.0;
	simulationValuesFront[136]	= 2.0;		simulationValuesRear[136]	= 2.0;
	simulationValuesFront[137]	= 2.0;		simulationValuesRear[137]	= 2.0;
	simulationValuesFront[138]	= 2.0;		simulationValuesRear[138]	= 2.0;
	simulationValuesFront[139]	= 2.0;		simulationValuesRear[139]	= 2.0;
	simulationValuesFront[140]	= 6.0;		simulationValuesRear[140]	= 6.0;
	simulationValuesFront[141]	= 6.0;		simulationValuesRear[141]	= 6.0;
	simulationValuesFront[142]	= 6.0;		simulationValuesRear[142]	= 6.0;
	simulationValuesFront[143]	= 6.0;		simulationValuesRear[143]	= 6.0;
	simulationValuesFront[144]	= 6.0;		simulationValuesRear[144]	= 6.0;
	simulationValuesFront[145]	= 6.0;		simulationValuesRear[145]	= 6.0;
	simulationValuesFront[146]	= 6.0;		simulationValuesRear[146]	= 6.0;
	simulationValuesFront[147]	= 6.0;		simulationValuesRear[147]	= 6.0;
	simulationValuesFront[148]	= 6.0;		simulationValuesRear[148]	= 6.0;
	simulationValuesFront[149]	= 6.0;		simulationValuesRear[149]	= 6.0;
	simulationValuesFront[150]	= 6.5;		simulationValuesRear[150]	= 6.5;
	simulationValuesFront[151]	= 6.5;		simulationValuesRear[151]	= 6.5;
	simulationValuesFront[152]	= 6.5;		simulationValuesRear[152]	= 6.5;
	simulationValuesFront[153]	= 6.5;		simulationValuesRear[153]	= 6.5;
	simulationValuesFront[154]	= 6.5;		simulationValuesRear[154]	= 6.5;
	simulationValuesFront[155]	= 6.5;		simulationValuesRear[155]	= 6.5;
	simulationValuesFront[156]	= 6.5;		simulationValuesRear[156]	= 6.5;
	simulationValuesFront[157]	= 6.5;		simulationValuesRear[157]	= 6.5;
	simulationValuesFront[158]	= 6.5;		simulationValuesRear[158]	= 6.5;
	simulationValuesFront[159]	= 10.0;		simulationValuesRear[159]	= 10.0;
	simulationValuesFront[160]	= 4.0;		simulationValuesRear[160]	= 4.0;
	simulationValuesFront[161]	= 4.0;		simulationValuesRear[161]	= 4.0;
	simulationValuesFront[162]	= 4.0;		simulationValuesRear[162]	= 4.0;
	simulationValuesFront[163]	= 4.0;		simulationValuesRear[163]	= 4.0;
	simulationValuesFront[164]	= 4.0;		simulationValuesRear[164]	= 4.0;
	simulationValuesFront[165]	= 4.0;		simulationValuesRear[165]	= 4.0;
	simulationValuesFront[166]	= 4.0;		simulationValuesRear[166]	= 4.0;
	simulationValuesFront[167]	= 4.0;		simulationValuesRear[167]	= 4.0;
	simulationValuesFront[168]	= 4.0;		simulationValuesRear[168]	= 4.0;
	simulationValuesFront[169]	= 4.0;		simulationValuesRear[169]	= 4.0;
	simulationValuesFront[170]	= 3.18;		simulationValuesRear[170]	= 3.18;
	simulationValuesFront[171]	= 3.18;		simulationValuesRear[171]	= 3.18;
	simulationValuesFront[172]	= 3.18;		simulationValuesRear[172]	= 3.18;
	simulationValuesFront[173]	= 3.18;		simulationValuesRear[173]	= 3.18;
	simulationValuesFront[174]	= 3.18;		simulationValuesRear[174]	= 3.18;
	simulationValuesFront[175]	= 3.18;		simulationValuesRear[175]	= 3.18;
	simulationValuesFront[176]	= 3.18;		simulationValuesRear[176]	= 3.18;
	simulationValuesFront[177]	= 3.18;		simulationValuesRear[177]	= 3.18;
	simulationValuesFront[178]	= 3.18;		simulationValuesRear[178]	= 3.18;
	simulationValuesFront[179]	= 3.18;		simulationValuesRear[179]	= 3.18;
	simulationValuesFront[180]	= 2.25;		simulationValuesRear[180]	= 3.25;
	simulationValuesFront[181]	= 2.25;		simulationValuesRear[181]	= 2.25;
	simulationValuesFront[182]	= 2.25;		simulationValuesRear[182]	= 2.25;
	simulationValuesFront[183]	= 2.25;		simulationValuesRear[183]	= 2.25;
	simulationValuesFront[184]	= 2.25;		simulationValuesRear[184]	= 2.25;
	simulationValuesFront[185]	= 2.25;		simulationValuesRear[185]	= 2.25;
	simulationValuesFront[186]	= 2.25;		simulationValuesRear[186]	= 2.25;
	simulationValuesFront[187]	= 2.25;		simulationValuesRear[187]	= 2.25;
	simulationValuesFront[188]	= 2.25;		simulationValuesRear[188]	= 2.25;
	simulationValuesFront[189]	= 2.25;		simulationValuesRear[189]	= 2.25;
	simulationValuesFront[190]	= 2.25;		simulationValuesRear[190]	= 2.25;
	simulationValuesFront[191]	= 2.25;		simulationValuesRear[191]	= 2.25;
	simulationValuesFront[192]	= 2.25;		simulationValuesRear[192]	= 2.25;
	simulationValuesFront[193]	= 2.25;		simulationValuesRear[193]	= 2.25;
	simulationValuesFront[194]	= 2.25;		simulationValuesRear[194]	= 2.25;
	simulationValuesFront[195]	= 2.25;		simulationValuesRear[195]	= 2.25;
	simulationValuesFront[196]	= 2.25;		simulationValuesRear[196]	= 2.25;
	simulationValuesFront[197]	= 2.25;		simulationValuesRear[197]	= 2.25;
	simulationValuesFront[198]	= 2.25;		simulationValuesRear[198]	= 2.25;
	simulationValuesFront[199]	= 2.25;		simulationValuesRear[199]	= 2.25;
	simulationValuesFront[200]	= 2.25;		simulationValuesRear[200]	= 2.25;
	simulationValuesFront[201]	= 1.50;		simulationValuesRear[201]	= 1.50;
	simulationValuesFront[202]	= 1.50;		simulationValuesRear[202]	= 1.50;
	simulationValuesFront[203]	= 1.50;		simulationValuesRear[203]	= 1.50;
	simulationValuesFront[204]	= 1.50;		simulationValuesRear[204]	= 1.50;
	simulationValuesFront[205]	= 1.50;		simulationValuesRear[205]	= 1.50;
	simulationValuesFront[206]	= 1.50;		simulationValuesRear[206]	= 1.50;
	simulationValuesFront[207]	= 1.50;		simulationValuesRear[207]	= 1.50;
	simulationValuesFront[208]	= 1.50;		simulationValuesRear[208]	= 1.50;
	simulationValuesFront[209]	= 1.50;		simulationValuesRear[209]	= 1.50;
	simulationValuesFront[210]	= 1.50;		simulationValuesRear[210]	= 1.50;
	simulationValuesFront[211]	= 1.50;		simulationValuesRear[211]	= 1.50;
	simulationValuesFront[212]	= 1.50;		simulationValuesRear[212]	= 1.50;
	simulationValuesFront[213]	= 1.50;		simulationValuesRear[213]	= 1.50;
	simulationValuesFront[214]	= 1.50;		simulationValuesRear[214]	= 1.50;
	simulationValuesFront[215]	= 1.50;		simulationValuesRear[215]	= 1.50;
	simulationValuesFront[216]	= 1.50;		simulationValuesRear[216]	= 1.50;
	simulationValuesFront[217]	= 1.50;		simulationValuesRear[217]	= 1.50;
	simulationValuesFront[218]	= 1.50;		simulationValuesRear[218]	= 1.50;
	simulationValuesFront[219]	= 1.50;		simulationValuesRear[219]	= 1.50;
	simulationValuesFront[220]	= 1.50;		simulationValuesRear[220]	= 1.50;
	simulationValuesFront[221]	= 2.50;		simulationValuesRear[221]	= 2.50;
	simulationValuesFront[222]	= 2.50;		simulationValuesRear[222]	= 2.50;
	simulationValuesFront[223]	= 2.50;		simulationValuesRear[223]	= 2.50;
	simulationValuesFront[224]	= 2.50;		simulationValuesRear[224]	= 2.50;
	simulationValuesFront[225]	= 2.50;		simulationValuesRear[225]	= 2.50;
	simulationValuesFront[226]	= 2.50;		simulationValuesRear[226]	= 2.50;
	simulationValuesFront[227]	= 2.50;		simulationValuesRear[227]	= 2.50;
	simulationValuesFront[228]	= 2.50;		simulationValuesRear[228]	= 2.50;
	simulationValuesFront[229]	= 2.50;		simulationValuesRear[229]	= 2.50;
	simulationValuesFront[230]	= 2.50;		simulationValuesRear[230]	= 2.50;
	simulationValuesFront[231]	= 2.50;		simulationValuesRear[231]	= 2.50;
	simulationValuesFront[232]	= 2.50;		simulationValuesRear[232]	= 2.50;
	simulationValuesFront[233]	= 2.50;		simulationValuesRear[233]	= 2.50;
	simulationValuesFront[234]	= 2.50;		simulationValuesRear[234]	= 2.50;
	simulationValuesFront[235]	= 2.50;		simulationValuesRear[235]	= 2.50;
	simulationValuesFront[236]	= 2.50;		simulationValuesRear[236]	= 2.50;
	simulationValuesFront[237]	= 2.50;		simulationValuesRear[237]	= 2.50;
	simulationValuesFront[238]	= 2.50;		simulationValuesRear[238]	= 2.50;
	simulationValuesFront[239]	= 2.50;		simulationValuesRear[239]	= 2.50;
	simulationValuesFront[240]	= 2.50;		simulationValuesRear[240]	= 2.50;
	simulationValuesFront[241]	= 6.00;		simulationValuesRear[241]	= 6.00;
	simulationValuesFront[242]	= 6.00;		simulationValuesRear[242]	= 6.00;
	simulationValuesFront[243]	= 6.00;		simulationValuesRear[243]	= 6.00;
	simulationValuesFront[244]	= 6.00;		simulationValuesRear[244]	= 6.00;
	simulationValuesFront[245]	= 6.00;		simulationValuesRear[245]	= 6.00;
	simulationValuesFront[246]	= 6.00;		simulationValuesRear[246]	= 6.00;
	simulationValuesFront[247]	= 6.00;		simulationValuesRear[247]	= 6.00;
	simulationValuesFront[248]	= 6.00;		simulationValuesRear[248]	= 6.00;
	simulationValuesFront[249]	= 6.00;		simulationValuesRear[249]	= 6.00;
	simulationValuesFront[250]	= 6.00;		simulationValuesRear[250]	= 6.00;
	simulationValuesFront[251]	= 6.00;		simulationValuesRear[251]	= 6.00;
	simulationValuesFront[252]	= 6.00;		simulationValuesRear[252]	= 6.00;
	simulationValuesFront[253]	= 6.00;		simulationValuesRear[253]	= 6.00;
	simulationValuesFront[254]	= 6.00;		simulationValuesRear[254]	= 6.00;
	simulationValuesFront[255]	= 6.00;		simulationValuesRear[255]	= 6.00;
	simulationValuesFront[256]	= 6.00;		simulationValuesRear[256]	= 6.00;
	simulationValuesFront[257]	= 6.00;		simulationValuesRear[257]	= 6.00;
	simulationValuesFront[258]	= 6.00;		simulationValuesRear[258]	= 6.00;
	simulationValuesFront[259]	= 6.00;		simulationValuesRear[259]	= 6.00;
	simulationValuesFront[260]	= 6.00;		simulationValuesRear[260]	= 6.00;
	simulationValuesFront[261]	= 4.00;		simulationValuesRear[261]	= 4.00;
	simulationValuesFront[262]	= 4.00;		simulationValuesRear[262]	= 4.00;
	simulationValuesFront[263]	= 4.00;		simulationValuesRear[263]	= 4.00;
	simulationValuesFront[264]	= 4.00;		simulationValuesRear[264]	= 4.00;
	simulationValuesFront[265]	= 4.00;		simulationValuesRear[265]	= 4.00;
	simulationValuesFront[266]	= 4.00;		simulationValuesRear[266]	= 4.00;
	simulationValuesFront[267]	= 4.00;		simulationValuesRear[267]	= 4.00;
	simulationValuesFront[268]	= 4.00;		simulationValuesRear[268]	= 4.00;
	simulationValuesFront[269]	= 4.00;		simulationValuesRear[269]	= 4.00;
	simulationValuesFront[270]	= 4.00;		simulationValuesRear[270]	= 4.00;
	simulationValuesFront[271]	= 4.00;		simulationValuesRear[271]	= 4.00;
	simulationValuesFront[272]	= 4.00;		simulationValuesRear[272]	= 4.00;
	simulationValuesFront[273]	= 4.00;		simulationValuesRear[273]	= 4.00;
	simulationValuesFront[274]	= 4.00;		simulationValuesRear[274]	= 4.00;
	simulationValuesFront[275]	= 4.00;		simulationValuesRear[275]	= 4.00;
	simulationValuesFront[276]	= 4.00;		simulationValuesRear[276]	= 4.00;
	simulationValuesFront[277]	= 4.00;		simulationValuesRear[277]	= 4.00;
	simulationValuesFront[278]	= 4.00;		simulationValuesRear[278]	= 4.00;
	simulationValuesFront[279]	= 4.00;		simulationValuesRear[279]	= 4.00;
	simulationValuesFront[280]	= 4.00;		simulationValuesRear[280]	= 4.00;
	simulationValuesFront[281]	= 1.20;		simulationValuesRear[281]	= 1.20;
	simulationValuesFront[282]	= 1.20;		simulationValuesRear[282]	= 1.20;
	simulationValuesFront[283]	= 1.20;		simulationValuesRear[283]	= 1.20;
	simulationValuesFront[284]	= 1.20;		simulationValuesRear[284]	= 1.20;
	simulationValuesFront[285]	= 1.20;		simulationValuesRear[285]	= 1.20;
	simulationValuesFront[286]	= 1.20;		simulationValuesRear[286]	= 1.20;
	simulationValuesFront[287]	= 1.20;		simulationValuesRear[287]	= 1.20;
	simulationValuesFront[288]	= 1.20;		simulationValuesRear[288]	= 1.20;
	simulationValuesFront[289]	= 1.20;		simulationValuesRear[289]	= 1.20;
	simulationValuesFront[290]	= 1.20;		simulationValuesRear[290]	= 1.20;
	simulationValuesFront[291]	= 1.20;		simulationValuesRear[291]	= 1.20;
	simulationValuesFront[292]	= 1.20;		simulationValuesRear[292]	= 1.20;
	simulationValuesFront[293]	= 1.20;		simulationValuesRear[293]	= 1.20;
	simulationValuesFront[294]	= 1.20;		simulationValuesRear[294]	= 1.20;
	simulationValuesFront[295]	= 1.20;		simulationValuesRear[295]	= 1.20;
	simulationValuesFront[296]	= 1.20;		simulationValuesRear[296]	= 1.20;
	simulationValuesFront[297]	= 1.20;		simulationValuesRear[297]	= 1.20;
	simulationValuesFront[298]	= 1.20;		simulationValuesRear[298]	= 1.20;
	simulationValuesFront[299]	= 1.20;		simulationValuesRear[299]	= 1.20;
	simulationValuesFront[300]	= 1.20;		simulationValuesRear[300]	= 1.20;
	simulationValuesFront[301]	= 3.00;		simulationValuesRear[301]	= 3.00;
	simulationValuesFront[302]	= 3.00;		simulationValuesRear[302]	= 3.00;
	simulationValuesFront[303]	= 3.00;		simulationValuesRear[303]	= 3.00;
	simulationValuesFront[304]	= 3.00;		simulationValuesRear[304]	= 3.00;
	simulationValuesFront[305]	= 3.00;		simulationValuesRear[305]	= 3.00;
	simulationValuesFront[306]	= 3.00;		simulationValuesRear[306]	= 3.00;
	simulationValuesFront[307]	= 3.00;		simulationValuesRear[307]	= 3.00;
	simulationValuesFront[308]	= 3.00;		simulationValuesRear[308]	= 3.00;
	simulationValuesFront[309]	= 3.00;		simulationValuesRear[309]	= 3.00;
	simulationValuesFront[310]	= 3.00;		simulationValuesRear[310]	= 3.00;
	simulationValuesFront[311]	= 3.00;		simulationValuesRear[311]	= 3.00;
	simulationValuesFront[312]	= 3.00;		simulationValuesRear[312]	= 3.00;
	simulationValuesFront[313]	= 3.00;		simulationValuesRear[313]	= 3.00;
	simulationValuesFront[314]	= 3.00;		simulationValuesRear[314]	= 3.00;
	simulationValuesFront[315]	= 3.00;		simulationValuesRear[315]	= 3.00;
	simulationValuesFront[316]	= 3.00;		simulationValuesRear[316]	= 3.00;
	simulationValuesFront[317]	= 3.00;		simulationValuesRear[317]	= 3.00;
	simulationValuesFront[318]	= 3.00;		simulationValuesRear[318]	= 3.00;
	simulationValuesFront[319]	= 5.10;		simulationValuesRear[319]	= 5.10;
	simulationValuesFront[320]	= 5.10;		simulationValuesRear[320]	= 5.10;
	simulationValuesFront[321]	= 5.10;		simulationValuesRear[321]	= 5.10;
	simulationValuesFront[322]	= 5.10;		simulationValuesRear[322]	= 5.10;
	simulationValuesFront[323]	= 5.10;		simulationValuesRear[323]	= 5.10;
	simulationValuesFront[324]	= 5.10;		simulationValuesRear[324]	= 5.10;
	simulationValuesFront[325]	= 5.10;		simulationValuesRear[325]	= 5.10;
	simulationValuesFront[326]	= 5.10;		simulationValuesRear[326]	= 5.10;
	simulationValuesFront[327]	= 5.10;		simulationValuesRear[327]	= 5.10;
	simulationValuesFront[328]	= 5.10;		simulationValuesRear[328]	= 5.10;
	simulationValuesFront[329]	= 5.10;		simulationValuesRear[329]	= 5.10;
	simulationValuesFront[330]	= 5.10;		simulationValuesRear[330]	= 5.10;
	simulationValuesFront[331]	= 5.10;		simulationValuesRear[331]	= 5.10;
	simulationValuesFront[332]	= 5.10;		simulationValuesRear[332]	= 5.10;
	simulationValuesFront[333]	= 5.10;		simulationValuesRear[333]	= 5.10;
	simulationValuesFront[334]	= 5.10;		simulationValuesRear[334]	= 5.10;
	simulationValuesFront[335]	= 5.10;		simulationValuesRear[335]	= 5.10;
	simulationValuesFront[336]	= 5.10;		simulationValuesRear[336]	= 5.10;
	simulationValuesFront[337]	= 5.10;		simulationValuesRear[337]	= 5.10;
	simulationValuesFront[338]	= 5.10;		simulationValuesRear[338]	= 5.10;
	simulationValuesFront[339]	= 6.40;		simulationValuesRear[339]	= 6.40;
	simulationValuesFront[340]	= 6.40;		simulationValuesRear[340]	= 6.40;
	simulationValuesFront[341]	= 6.40;		simulationValuesRear[341]	= 6.40;
	simulationValuesFront[342]	= 6.40;		simulationValuesRear[342]	= 6.40;
	simulationValuesFront[343]	= 6.40;		simulationValuesRear[343]	= 6.40;
	simulationValuesFront[344]	= 6.40;		simulationValuesRear[344]	= 6.40;
	simulationValuesFront[345]	= 6.40;		simulationValuesRear[345]	= 6.40;
	simulationValuesFront[346]	= 6.40;		simulationValuesRear[346]	= 6.40;
	simulationValuesFront[347]	= 6.40;		simulationValuesRear[347]	= 6.40;
	simulationValuesFront[348]	= 6.40;		simulationValuesRear[348]	= 6.40;
	simulationValuesFront[349]	= 6.40;		simulationValuesRear[349]	= 6.40;
	simulationValuesFront[350]	= 6.40;		simulationValuesRear[350]	= 6.40;
	simulationValuesFront[351]	= 6.40;		simulationValuesRear[351]	= 6.40;
	simulationValuesFront[352]	= 6.40;		simulationValuesRear[352]	= 6.40;
	simulationValuesFront[353]	= 6.40;		simulationValuesRear[353]	= 6.40;
	simulationValuesFront[354]	= 6.40;		simulationValuesRear[354]	= 6.40;
	simulationValuesFront[355]	= 6.40;		simulationValuesRear[355]	= 6.40;
	simulationValuesFront[356]	= 6.40;		simulationValuesRear[356]	= 6.40;
	simulationValuesFront[357]	= 6.40;		simulationValuesRear[357]	= 6.40;
	simulationValuesFront[358]	= 6.40;		simulationValuesRear[35]	= 6.40;
	simulationValuesFront[359]	= 6.40;		simulationValuesRear[359]	= 6.40;
}                                                       
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                
