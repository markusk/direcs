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
		simulatedValuesFront.append(0);
		simulatedValuesRear.append(0);
	}

	setSimulationValues();
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
		setSimulationValues();

		// copy sim values into the distances values
		for (int i=0; i<laserScannerValuesFront.size()-1; i++)
		{
			// laserScannerValuesFront[i] = 2.30; //(i+1) / 100;
			laserScannerValuesFront[i] = simulatedValuesFront.at(i);
		}

		for (int i=0; i<laserScannerValuesRear.size()-1; i++)
		{
			// laserScannerValuesRear[i] = 2.30; //(i+1) / 100;
			laserScannerValuesRear[i] = simulatedValuesRear.at(i);
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
	simulatedValuesFront[0]	= 2.07;		simulatedValuesRear[0]	= 0.25;
	simulatedValuesFront[1]	= 2.07;		simulatedValuesRear[1]	= 0.25;
	simulatedValuesFront[2]	= 2.07;		simulatedValuesRear[2]	= 0.25;
	simulatedValuesFront[3]	= 2.07;		simulatedValuesRear[3]	= 0.25;
	simulatedValuesFront[4]	= 2.07;		simulatedValuesRear[4]	= 0.25;
	simulatedValuesFront[5]	= 2.07;		simulatedValuesRear[5]	= 0.25;
	simulatedValuesFront[6]	= 2.07;		simulatedValuesRear[6]	= 0.25;
	simulatedValuesFront[7]	= 2.07;		simulatedValuesRear[7]	= 0.25;
	simulatedValuesFront[8]	= 2.07;		simulatedValuesRear[8]	= 0.25;
	simulatedValuesFront[9]	= 2.07;		simulatedValuesRear[9]	= 0.25;
	simulatedValuesFront[10]	= 1.5;		simulatedValuesRear[10]	= 0.25;
	simulatedValuesFront[11]	= 1.5;		simulatedValuesRear[11]	= 0.25;
	simulatedValuesFront[12]	= 1.5;		simulatedValuesRear[12]	= 1.5;
	simulatedValuesFront[13]	= 1.5;		simulatedValuesRear[13]	= 1.5;
	simulatedValuesFront[14]	= 1.5;		simulatedValuesRear[14]	= 1.5;
	simulatedValuesFront[15]	= 1.5;		simulatedValuesRear[15]	= 1.5;
	simulatedValuesFront[16]	= 1.5;		simulatedValuesRear[16]	= 1.5;
	simulatedValuesFront[17]	= 1.5;		simulatedValuesRear[17]	= 1.5;
	simulatedValuesFront[18]	= 1.5;		simulatedValuesRear[18]	= 1.5;
	simulatedValuesFront[19]	= 1.5;		simulatedValuesRear[19]	= 1.5;
	simulatedValuesFront[20]	= 2.0;		simulatedValuesRear[20]	= 2.0;
	simulatedValuesFront[21]	= 2.0;		simulatedValuesRear[21]	= 2.0;
	simulatedValuesFront[22]	= 2.0;		simulatedValuesRear[22]	= 2.0;
	simulatedValuesFront[23]	= 2.0;		simulatedValuesRear[23]	= 2.0;
	simulatedValuesFront[24]	= 2.0;		simulatedValuesRear[24]	= 2.0;
	simulatedValuesFront[25]	= 2.0;		simulatedValuesRear[25]	= 2.0;
	simulatedValuesFront[26]	= 2.0;		simulatedValuesRear[26]	= 2.0;
	simulatedValuesFront[27]	= 2.0;		simulatedValuesRear[27]	= 2.0;
	simulatedValuesFront[28]	= 2.0;		simulatedValuesRear[28]	= 2.0;
	simulatedValuesFront[29]	= 2.0;		simulatedValuesRear[29]	= 2.0;
	simulatedValuesFront[30]	= 2.5;		simulatedValuesRear[30]	= 2.5;
	simulatedValuesFront[31]	= 2.5;		simulatedValuesRear[31]	= 2.5;
	simulatedValuesFront[32]	= 2.5;		simulatedValuesRear[32]	= 2.5;
	simulatedValuesFront[33]	= 2.5;		simulatedValuesRear[33]	= 2.5;
	simulatedValuesFront[34]	= 2.5;		simulatedValuesRear[34]	= 2.5;
	simulatedValuesFront[35]	= 2.5;		simulatedValuesRear[35]	= 2.5;
	simulatedValuesFront[36]	= 2.5;		simulatedValuesRear[36]	= 2.5;
	simulatedValuesFront[37]	= 2.5;		simulatedValuesRear[37]	= 2.5;
	simulatedValuesFront[38]	= 2.5;		simulatedValuesRear[38]	= 2.5;
	simulatedValuesFront[39]	= 2.5;		simulatedValuesRear[39]	= 2.5;
	simulatedValuesFront[40]	= 3.25;		simulatedValuesRear[40]	= 3.25;
	simulatedValuesFront[41]	= 3.25;		simulatedValuesRear[41]	= 3.25;
	simulatedValuesFront[42]	= 3.25;		simulatedValuesRear[42]	= 3.25;
	simulatedValuesFront[43]	= 3.25;		simulatedValuesRear[43]	= 3.25;
	simulatedValuesFront[44]	= 3.25;		simulatedValuesRear[44]	= 3.25;
	simulatedValuesFront[45]	= 3.25;		simulatedValuesRear[45]	= 3.25;
	simulatedValuesFront[46]	= 3.25;		simulatedValuesRear[46]	= 3.25;
	simulatedValuesFront[47]	= 3.25;		simulatedValuesRear[47]	= 3.25;
	simulatedValuesFront[48]	= 3.25;		simulatedValuesRear[48]	= 3.25;
	simulatedValuesFront[49]	= 3.25;		simulatedValuesRear[49]	= 3.25;
	simulatedValuesFront[50]	= 3.5;		simulatedValuesRear[50]	= 3.5;
	simulatedValuesFront[51]	= 3.5;		simulatedValuesRear[51]	= 3.5;
	simulatedValuesFront[52]	= 3.5;		simulatedValuesRear[52]	= 3.5;
	simulatedValuesFront[53]	= 3.5;		simulatedValuesRear[53]	= 3.5;
	simulatedValuesFront[54]	= 3.5;		simulatedValuesRear[54]	= 3.5;
	simulatedValuesFront[55]	= 3.5;		simulatedValuesRear[55]	= 3.5;
	simulatedValuesFront[56]	= 3.5;		simulatedValuesRear[56]	= 3.5;
	simulatedValuesFront[57]	= 3.5;		simulatedValuesRear[57]	= 3.5;
	simulatedValuesFront[58]	= 3.5;		simulatedValuesRear[58]	= 3.5;
	simulatedValuesFront[59]	= 3.5;		simulatedValuesRear[59]	= 3.5;
	simulatedValuesFront[60]	= 4.0;		simulatedValuesRear[60]	= 4.0;
	simulatedValuesFront[61]	= 4.0;		simulatedValuesRear[61]	= 4.0;
	simulatedValuesFront[62]	= 4.0;		simulatedValuesRear[62]	= 4.0;
	simulatedValuesFront[63]	= 4.0;		simulatedValuesRear[63]	= 4.0;
	simulatedValuesFront[64]	= 4.0;		simulatedValuesRear[64]	= 4.0;
	simulatedValuesFront[65]	= 4.0;		simulatedValuesRear[65]	= 4.0;
	simulatedValuesFront[66]	= 4.0;		simulatedValuesRear[66]	= 4.0;
	simulatedValuesFront[67]	= 4.0;		simulatedValuesRear[67]	= 4.0;
	simulatedValuesFront[68]	= 4.0;		simulatedValuesRear[68]	= 4.0;
	simulatedValuesFront[69]	= 4.0;		simulatedValuesRear[69]	= 4.0;
	simulatedValuesFront[70]	= 4.5;		simulatedValuesRear[70]	= 4.5;
	simulatedValuesFront[71]	= 4.5;		simulatedValuesRear[71]	= 4.5;
	simulatedValuesFront[72]	= 4.5;		simulatedValuesRear[72]	= 4.5;
	simulatedValuesFront[73]	= 4.5;		simulatedValuesRear[73]	= 4.5;
	simulatedValuesFront[74]	= 4.5;		simulatedValuesRear[74]	= 4.5;
	simulatedValuesFront[75]	= 4.5;		simulatedValuesRear[75]	= 4.5;
	simulatedValuesFront[76]	= 4.5;		simulatedValuesRear[76]	= 4.5;
	simulatedValuesFront[77]	= 4.5;		simulatedValuesRear[77]	= 4.5;
	simulatedValuesFront[78]	= 4.5;		simulatedValuesRear[78]	= 4.5;
	simulatedValuesFront[79]	= 4.5;		simulatedValuesRear[79]	= 4.5;
	simulatedValuesFront[80]	= 5.0;		simulatedValuesRear[80]	= 4.5;
	simulatedValuesFront[81]	= 5.0;		simulatedValuesRear[81]	= 5.0;
	simulatedValuesFront[82]	= 5.0;		simulatedValuesRear[82]	= 5.0;
	simulatedValuesFront[83]	= 5.0;		simulatedValuesRear[83]	= 5.0;
	simulatedValuesFront[84]	= 5.0;		simulatedValuesRear[84]	= 5.0;
	simulatedValuesFront[85]	= 5.0;		simulatedValuesRear[85]	= 5.0;
	simulatedValuesFront[86]	= 9.0;		simulatedValuesRear[86]	= 9.0;
	simulatedValuesFront[87]	= 9.0;		simulatedValuesRear[87]	= 9.0;
	simulatedValuesFront[88]	= 16.2;		simulatedValuesRear[88]	= 16.2;
	simulatedValuesFront[89]	= 16.2;		simulatedValuesRear[89]	= 16.2;
	simulatedValuesFront[90]	= 9.0;		simulatedValuesRear[90]	= 9.0;
	simulatedValuesFront[91]	= 2.5;		simulatedValuesRear[91]	= 2.5;
	simulatedValuesFront[92]	= 2.5;		simulatedValuesRear[92]	= 2.5;
	simulatedValuesFront[93]	= 2.5;		simulatedValuesRear[93]	= 2.5;
	simulatedValuesFront[94]	= 2.5;		simulatedValuesRear[94]	= 2.5;
	simulatedValuesFront[95]	= 2.5;		simulatedValuesRear[95]	= 2.5;
	simulatedValuesFront[96]	= 2.5;		simulatedValuesRear[96]	= 2.5;
	simulatedValuesFront[97]	= 2.5;		simulatedValuesRear[97]	= 2.5;
	simulatedValuesFront[98]	= 2.5;		simulatedValuesRear[98]	= 2.5;
	simulatedValuesFront[99]	= 2.5;		simulatedValuesRear[99]	= 2.5;
	simulatedValuesFront[100]	= 2.0;		simulatedValuesRear[100]	= 2.0;
	simulatedValuesFront[101]	= 2.0;		simulatedValuesRear[101]	= 2.0;
	simulatedValuesFront[102]	= 2.0;		simulatedValuesRear[102]	= 2.0;
	simulatedValuesFront[103]	= 2.0;		simulatedValuesRear[103]	= 2.0;
	simulatedValuesFront[104]	= 2.0;		simulatedValuesRear[104]	= 2.0;
	simulatedValuesFront[105]	= 2.0;		simulatedValuesRear[105]	= 2.0;
	simulatedValuesFront[106]	= 3.0;		simulatedValuesRear[106]	= 3.0;
	simulatedValuesFront[107]	= 3.0;		simulatedValuesRear[107]	= 3.0;
	simulatedValuesFront[108]	= 3.0;		simulatedValuesRear[108]	= 3.0;
	simulatedValuesFront[109]	= 3.0;		simulatedValuesRear[109]	= 3.0;
	simulatedValuesFront[110]	= 3.0;		simulatedValuesRear[110]	= 3.0;
	simulatedValuesFront[111]	= 3.0;		simulatedValuesRear[111]	= 3.0;
	simulatedValuesFront[112]	= 3.0;		simulatedValuesRear[112]	= 3.0;
	simulatedValuesFront[113]	= 3.0;		simulatedValuesRear[113]	= 3.0;
	simulatedValuesFront[114]	= 3.0;		simulatedValuesRear[114]	= 3.0;
	simulatedValuesFront[115]	= 3.0;		simulatedValuesRear[115]	= 3.0;
	simulatedValuesFront[116]	= 3.0;		simulatedValuesRear[116]	= 3.0;
	simulatedValuesFront[117]	= 3.0;		simulatedValuesRear[117]	= 3.0;
	simulatedValuesFront[118]	= 3.0;		simulatedValuesRear[118]	= 3.0;
	simulatedValuesFront[119]	= 3.0;		simulatedValuesRear[119]	= 3.0;
	simulatedValuesFront[120]	= 2.0;		simulatedValuesRear[120]	= 2.0;
	simulatedValuesFront[121]	= 2.0;		simulatedValuesRear[121]	= 2.0;
	simulatedValuesFront[122]	= 2.0;		simulatedValuesRear[122]	= 2.0;
	simulatedValuesFront[123]	= 2.0;		simulatedValuesRear[123]	= 2.0;
	simulatedValuesFront[124]	= 2.0;		simulatedValuesRear[124]	= 2.0;
	simulatedValuesFront[125]	= 2.0;		simulatedValuesRear[125]	= 2.0;
	simulatedValuesFront[126]	= 2.0;		simulatedValuesRear[126]	= 2.0;
	simulatedValuesFront[127]	= 2.0;		simulatedValuesRear[127]	= 2.0;
	simulatedValuesFront[128]	= 2.0;		simulatedValuesRear[128]	= 2.0;
	simulatedValuesFront[129]	= 2.0;		simulatedValuesRear[129]	= 2.0;
	simulatedValuesFront[130]	= 2.0;		simulatedValuesRear[130]	= 2.0;
	simulatedValuesFront[131]	= 2.0;		simulatedValuesRear[131]	= 2.0;
	simulatedValuesFront[132]	= 2.0;		simulatedValuesRear[132]	= 2.0;
	simulatedValuesFront[133]	= 2.0;		simulatedValuesRear[133]	= 2.0;
	simulatedValuesFront[134]	= 2.0;		simulatedValuesRear[134]	= 2.0;
	simulatedValuesFront[135]	= 2.0;		simulatedValuesRear[135]	= 2.0;
	simulatedValuesFront[136]	= 2.0;		simulatedValuesRear[136]	= 2.0;
	simulatedValuesFront[137]	= 2.0;		simulatedValuesRear[137]	= 2.0;
	simulatedValuesFront[138]	= 2.0;		simulatedValuesRear[138]	= 2.0;
	simulatedValuesFront[139]	= 2.0;		simulatedValuesRear[139]	= 2.0;
	simulatedValuesFront[140]	= 6.0;		simulatedValuesRear[140]	= 6.0;
	simulatedValuesFront[141]	= 6.0;		simulatedValuesRear[141]	= 6.0;
	simulatedValuesFront[142]	= 6.0;		simulatedValuesRear[142]	= 6.0;
	simulatedValuesFront[143]	= 6.0;		simulatedValuesRear[143]	= 6.0;
	simulatedValuesFront[144]	= 6.0;		simulatedValuesRear[144]	= 6.0;
	simulatedValuesFront[145]	= 6.0;		simulatedValuesRear[145]	= 6.0;
	simulatedValuesFront[146]	= 6.0;		simulatedValuesRear[146]	= 6.0;
	simulatedValuesFront[147]	= 6.0;		simulatedValuesRear[147]	= 6.0;
	simulatedValuesFront[148]	= 6.0;		simulatedValuesRear[148]	= 6.0;
	simulatedValuesFront[149]	= 6.0;		simulatedValuesRear[149]	= 6.0;
	simulatedValuesFront[150]	= 6.5;		simulatedValuesRear[150]	= 6.5;
	simulatedValuesFront[151]	= 6.5;		simulatedValuesRear[151]	= 6.5;
	simulatedValuesFront[152]	= 6.5;		simulatedValuesRear[152]	= 6.5;
	simulatedValuesFront[153]	= 6.5;		simulatedValuesRear[153]	= 6.5;
	simulatedValuesFront[154]	= 6.5;		simulatedValuesRear[154]	= 6.5;
	simulatedValuesFront[155]	= 6.5;		simulatedValuesRear[155]	= 6.5;
	simulatedValuesFront[156]	= 6.5;		simulatedValuesRear[156]	= 6.5;
	simulatedValuesFront[157]	= 6.5;		simulatedValuesRear[157]	= 6.5;
	simulatedValuesFront[158]	= 6.5;		simulatedValuesRear[158]	= 6.5;
	simulatedValuesFront[159]	= 10.0;		simulatedValuesRear[159]	= 10.0;
	simulatedValuesFront[160]	= 4.0;		simulatedValuesRear[160]	= 4.0;
	simulatedValuesFront[161]	= 4.0;		simulatedValuesRear[161]	= 4.0;
	simulatedValuesFront[162]	= 4.0;		simulatedValuesRear[162]	= 4.0;
	simulatedValuesFront[163]	= 4.0;		simulatedValuesRear[163]	= 4.0;
	simulatedValuesFront[164]	= 4.0;		simulatedValuesRear[164]	= 4.0;
	simulatedValuesFront[165]	= 4.0;		simulatedValuesRear[165]	= 4.0;
	simulatedValuesFront[166]	= 4.0;		simulatedValuesRear[166]	= 4.0;
	simulatedValuesFront[167]	= 4.0;		simulatedValuesRear[167]	= 4.0;
	simulatedValuesFront[168]	= 4.0;		simulatedValuesRear[168]	= 4.0;
	simulatedValuesFront[169]	= 4.0;		simulatedValuesRear[169]	= 4.0;
	simulatedValuesFront[170]	= 3.18;		simulatedValuesRear[170]	= 3.18;
	simulatedValuesFront[171]	= 3.18;		simulatedValuesRear[171]	= 3.18;
	simulatedValuesFront[172]	= 3.18;		simulatedValuesRear[172]	= 3.18;
	simulatedValuesFront[173]	= 3.18;		simulatedValuesRear[173]	= 3.18;
	simulatedValuesFront[174]	= 3.18;		simulatedValuesRear[174]	= 3.18;
	simulatedValuesFront[175]	= 3.18;		simulatedValuesRear[175]	= 3.18;
	simulatedValuesFront[176]	= 3.18;		simulatedValuesRear[176]	= 3.18;
	simulatedValuesFront[177]	= 3.18;		simulatedValuesRear[177]	= 3.18;
	simulatedValuesFront[178]	= 3.18;		simulatedValuesRear[178]	= 3.18;
	simulatedValuesFront[179]	= 3.18;		simulatedValuesRear[179]	= 3.18;
	simulatedValuesFront[180]	= 2.25;		simulatedValuesRear[180]	= 3.25;
	simulatedValuesFront[181]	= 2.25;		simulatedValuesRear[181]	= 2.25;
	simulatedValuesFront[182]	= 2.25;		simulatedValuesRear[182]	= 2.25;
	simulatedValuesFront[183]	= 2.25;		simulatedValuesRear[183]	= 2.25;
	simulatedValuesFront[184]	= 2.25;		simulatedValuesRear[184]	= 2.25;
	simulatedValuesFront[185]	= 2.25;		simulatedValuesRear[185]	= 2.25;
	simulatedValuesFront[186]	= 2.25;		simulatedValuesRear[186]	= 2.25;
	simulatedValuesFront[187]	= 2.25;		simulatedValuesRear[187]	= 2.25;
	simulatedValuesFront[188]	= 2.25;		simulatedValuesRear[188]	= 2.25;
	simulatedValuesFront[189]	= 2.25;		simulatedValuesRear[189]	= 2.25;
	simulatedValuesFront[190]	= 2.25;		simulatedValuesRear[190]	= 2.25;
	simulatedValuesFront[191]	= 2.25;		simulatedValuesRear[191]	= 2.25;
	simulatedValuesFront[192]	= 2.25;		simulatedValuesRear[192]	= 2.25;
	simulatedValuesFront[193]	= 2.25;		simulatedValuesRear[193]	= 2.25;
	simulatedValuesFront[194]	= 2.25;		simulatedValuesRear[194]	= 2.25;
	simulatedValuesFront[195]	= 2.25;		simulatedValuesRear[195]	= 2.25;
	simulatedValuesFront[196]	= 2.25;		simulatedValuesRear[196]	= 2.25;
	simulatedValuesFront[197]	= 2.25;		simulatedValuesRear[197]	= 2.25;
	simulatedValuesFront[198]	= 2.25;		simulatedValuesRear[198]	= 2.25;
	simulatedValuesFront[199]	= 2.25;		simulatedValuesRear[199]	= 2.25;
	simulatedValuesFront[200]	= 2.25;		simulatedValuesRear[200]	= 2.25;
	simulatedValuesFront[201]	= 1.50;		simulatedValuesRear[201]	= 1.50;
	simulatedValuesFront[202]	= 1.50;		simulatedValuesRear[202]	= 1.50;
	simulatedValuesFront[203]	= 1.50;		simulatedValuesRear[203]	= 1.50;
	simulatedValuesFront[204]	= 1.50;		simulatedValuesRear[204]	= 1.50;
	simulatedValuesFront[205]	= 1.50;		simulatedValuesRear[205]	= 1.50;
	simulatedValuesFront[206]	= 1.50;		simulatedValuesRear[206]	= 1.50;
	simulatedValuesFront[207]	= 1.50;		simulatedValuesRear[207]	= 1.50;
	simulatedValuesFront[208]	= 1.50;		simulatedValuesRear[208]	= 1.50;
	simulatedValuesFront[209]	= 1.50;		simulatedValuesRear[209]	= 1.50;
	simulatedValuesFront[210]	= 1.50;		simulatedValuesRear[210]	= 1.50;
	simulatedValuesFront[211]	= 1.50;		simulatedValuesRear[211]	= 1.50;
	simulatedValuesFront[212]	= 1.50;		simulatedValuesRear[212]	= 1.50;
	simulatedValuesFront[213]	= 1.50;		simulatedValuesRear[213]	= 1.50;
	simulatedValuesFront[214]	= 1.50;		simulatedValuesRear[214]	= 1.50;
	simulatedValuesFront[215]	= 1.50;		simulatedValuesRear[215]	= 1.50;
	simulatedValuesFront[216]	= 1.50;		simulatedValuesRear[216]	= 1.50;
	simulatedValuesFront[217]	= 1.50;		simulatedValuesRear[217]	= 1.50;
	simulatedValuesFront[218]	= 1.50;		simulatedValuesRear[218]	= 1.50;
	simulatedValuesFront[219]	= 1.50;		simulatedValuesRear[219]	= 1.50;
	simulatedValuesFront[220]	= 1.50;		simulatedValuesRear[220]	= 1.50;
	simulatedValuesFront[221]	= 2.50;		simulatedValuesRear[221]	= 2.50;
	simulatedValuesFront[222]	= 2.50;		simulatedValuesRear[222]	= 2.50;
	simulatedValuesFront[223]	= 2.50;		simulatedValuesRear[223]	= 2.50;
	simulatedValuesFront[224]	= 2.50;		simulatedValuesRear[224]	= 2.50;
	simulatedValuesFront[225]	= 2.50;		simulatedValuesRear[225]	= 2.50;
	simulatedValuesFront[226]	= 2.50;		simulatedValuesRear[226]	= 2.50;
	simulatedValuesFront[227]	= 2.50;		simulatedValuesRear[227]	= 2.50;
	simulatedValuesFront[228]	= 2.50;		simulatedValuesRear[228]	= 2.50;
	simulatedValuesFront[229]	= 2.50;		simulatedValuesRear[229]	= 2.50;
	simulatedValuesFront[230]	= 2.50;		simulatedValuesRear[230]	= 2.50;
	simulatedValuesFront[231]	= 2.50;		simulatedValuesRear[231]	= 2.50;
	simulatedValuesFront[232]	= 2.50;		simulatedValuesRear[232]	= 2.50;
	simulatedValuesFront[233]	= 2.50;		simulatedValuesRear[233]	= 2.50;
	simulatedValuesFront[234]	= 2.50;		simulatedValuesRear[234]	= 2.50;
	simulatedValuesFront[235]	= 2.50;		simulatedValuesRear[235]	= 2.50;
	simulatedValuesFront[236]	= 2.50;		simulatedValuesRear[236]	= 2.50;
	simulatedValuesFront[237]	= 2.50;		simulatedValuesRear[237]	= 2.50;
	simulatedValuesFront[238]	= 2.50;		simulatedValuesRear[238]	= 2.50;
	simulatedValuesFront[239]	= 2.50;		simulatedValuesRear[239]	= 2.50;
	simulatedValuesFront[240]	= 2.50;		simulatedValuesRear[240]	= 2.50;
	simulatedValuesFront[241]	= 6.00;		simulatedValuesRear[241]	= 6.00;
	simulatedValuesFront[242]	= 6.00;		simulatedValuesRear[242]	= 6.00;
	simulatedValuesFront[243]	= 6.00;		simulatedValuesRear[243]	= 6.00;
	simulatedValuesFront[244]	= 6.00;		simulatedValuesRear[244]	= 6.00;
	simulatedValuesFront[245]	= 6.00;		simulatedValuesRear[245]	= 6.00;
	simulatedValuesFront[246]	= 6.00;		simulatedValuesRear[246]	= 6.00;
	simulatedValuesFront[247]	= 6.00;		simulatedValuesRear[247]	= 6.00;
	simulatedValuesFront[248]	= 6.00;		simulatedValuesRear[248]	= 6.00;
	simulatedValuesFront[249]	= 6.00;		simulatedValuesRear[249]	= 6.00;
	simulatedValuesFront[250]	= 6.00;		simulatedValuesRear[250]	= 6.00;
	simulatedValuesFront[251]	= 6.00;		simulatedValuesRear[251]	= 6.00;
	simulatedValuesFront[252]	= 6.00;		simulatedValuesRear[252]	= 6.00;
	simulatedValuesFront[253]	= 6.00;		simulatedValuesRear[253]	= 6.00;
	simulatedValuesFront[254]	= 6.00;		simulatedValuesRear[254]	= 6.00;
	simulatedValuesFront[255]	= 6.00;		simulatedValuesRear[255]	= 6.00;
	simulatedValuesFront[256]	= 6.00;		simulatedValuesRear[256]	= 6.00;
	simulatedValuesFront[257]	= 6.00;		simulatedValuesRear[257]	= 6.00;
	simulatedValuesFront[258]	= 6.00;		simulatedValuesRear[258]	= 6.00;
	simulatedValuesFront[259]	= 6.00;		simulatedValuesRear[259]	= 6.00;
	simulatedValuesFront[260]	= 6.00;		simulatedValuesRear[260]	= 6.00;
	simulatedValuesFront[261]	= 4.00;		simulatedValuesRear[261]	= 4.00;
	simulatedValuesFront[262]	= 4.00;		simulatedValuesRear[262]	= 4.00;
	simulatedValuesFront[263]	= 4.00;		simulatedValuesRear[263]	= 4.00;
	simulatedValuesFront[264]	= 4.00;		simulatedValuesRear[264]	= 4.00;
	simulatedValuesFront[265]	= 4.00;		simulatedValuesRear[265]	= 4.00;
	simulatedValuesFront[266]	= 4.00;		simulatedValuesRear[266]	= 4.00;
	simulatedValuesFront[267]	= 4.00;		simulatedValuesRear[267]	= 4.00;
	simulatedValuesFront[268]	= 4.00;		simulatedValuesRear[268]	= 4.00;
	simulatedValuesFront[269]	= 4.00;		simulatedValuesRear[269]	= 4.00;
	simulatedValuesFront[270]	= 4.00;		simulatedValuesRear[270]	= 4.00;
	simulatedValuesFront[271]	= 4.00;		simulatedValuesRear[271]	= 4.00;
	simulatedValuesFront[272]	= 4.00;		simulatedValuesRear[272]	= 4.00;
	simulatedValuesFront[273]	= 4.00;		simulatedValuesRear[273]	= 4.00;
	simulatedValuesFront[274]	= 4.00;		simulatedValuesRear[274]	= 4.00;
	simulatedValuesFront[275]	= 4.00;		simulatedValuesRear[275]	= 4.00;
	simulatedValuesFront[276]	= 4.00;		simulatedValuesRear[276]	= 4.00;
	simulatedValuesFront[277]	= 4.00;		simulatedValuesRear[277]	= 4.00;
	simulatedValuesFront[278]	= 4.00;		simulatedValuesRear[278]	= 4.00;
	simulatedValuesFront[279]	= 4.00;		simulatedValuesRear[279]	= 4.00;
	simulatedValuesFront[280]	= 4.00;		simulatedValuesRear[280]	= 4.00;
	simulatedValuesFront[281]	= 1.20;		simulatedValuesRear[281]	= 1.20;
	simulatedValuesFront[282]	= 1.20;		simulatedValuesRear[282]	= 1.20;
	simulatedValuesFront[283]	= 1.20;		simulatedValuesRear[283]	= 1.20;
	simulatedValuesFront[284]	= 1.20;		simulatedValuesRear[284]	= 1.20;
	simulatedValuesFront[285]	= 1.20;		simulatedValuesRear[285]	= 1.20;
	simulatedValuesFront[286]	= 1.20;		simulatedValuesRear[286]	= 1.20;
	simulatedValuesFront[287]	= 1.20;		simulatedValuesRear[287]	= 1.20;
	simulatedValuesFront[288]	= 1.20;		simulatedValuesRear[288]	= 1.20;
	simulatedValuesFront[289]	= 1.20;		simulatedValuesRear[289]	= 1.20;
	simulatedValuesFront[290]	= 1.20;		simulatedValuesRear[290]	= 1.20;
	simulatedValuesFront[291]	= 1.20;		simulatedValuesRear[291]	= 1.20;
	simulatedValuesFront[292]	= 1.20;		simulatedValuesRear[292]	= 1.20;
	simulatedValuesFront[293]	= 1.20;		simulatedValuesRear[293]	= 1.20;
	simulatedValuesFront[294]	= 1.20;		simulatedValuesRear[294]	= 1.20;
	simulatedValuesFront[295]	= 1.20;		simulatedValuesRear[295]	= 1.20;
	simulatedValuesFront[296]	= 1.20;		simulatedValuesRear[296]	= 1.20;
	simulatedValuesFront[297]	= 1.20;		simulatedValuesRear[297]	= 1.20;
	simulatedValuesFront[298]	= 1.20;		simulatedValuesRear[298]	= 1.20;
	simulatedValuesFront[299]	= 1.20;		simulatedValuesRear[299]	= 1.20;
	simulatedValuesFront[300]	= 1.20;		simulatedValuesRear[300]	= 1.20;
	simulatedValuesFront[301]	= 3.00;		simulatedValuesRear[301]	= 3.00;
	simulatedValuesFront[302]	= 3.00;		simulatedValuesRear[302]	= 3.00;
	simulatedValuesFront[303]	= 3.00;		simulatedValuesRear[303]	= 3.00;
	simulatedValuesFront[304]	= 3.00;		simulatedValuesRear[304]	= 3.00;
	simulatedValuesFront[305]	= 3.00;		simulatedValuesRear[305]	= 3.00;
	simulatedValuesFront[306]	= 3.00;		simulatedValuesRear[306]	= 3.00;
	simulatedValuesFront[307]	= 3.00;		simulatedValuesRear[307]	= 3.00;
	simulatedValuesFront[308]	= 3.00;		simulatedValuesRear[308]	= 3.00;
	simulatedValuesFront[309]	= 3.00;		simulatedValuesRear[309]	= 3.00;
	simulatedValuesFront[310]	= 3.00;		simulatedValuesRear[310]	= 3.00;
	simulatedValuesFront[311]	= 3.00;		simulatedValuesRear[311]	= 3.00;
	simulatedValuesFront[312]	= 3.00;		simulatedValuesRear[312]	= 3.00;
	simulatedValuesFront[313]	= 3.00;		simulatedValuesRear[313]	= 3.00;
	simulatedValuesFront[314]	= 3.00;		simulatedValuesRear[314]	= 3.00;
	simulatedValuesFront[315]	= 3.00;		simulatedValuesRear[315]	= 3.00;
	simulatedValuesFront[316]	= 3.00;		simulatedValuesRear[316]	= 3.00;
	simulatedValuesFront[317]	= 3.00;		simulatedValuesRear[317]	= 3.00;
	simulatedValuesFront[318]	= 3.00;		simulatedValuesRear[318]	= 3.00;
	simulatedValuesFront[319]	= 0.10;		simulatedValuesRear[319]	= 0.10;
	simulatedValuesFront[320]	= 0.10;		simulatedValuesRear[320]	= 0.10;
	simulatedValuesFront[321]	= 0.10;		simulatedValuesRear[321]	= 0.10;
	simulatedValuesFront[322]	= 0.10;		simulatedValuesRear[322]	= 0.10;
	simulatedValuesFront[323]	= 0.10;		simulatedValuesRear[323]	= 0.10;
	simulatedValuesFront[324]	= 0.10;		simulatedValuesRear[324]	= 0.10;
	simulatedValuesFront[325]	= 0.10;		simulatedValuesRear[325]	= 0.10;
	simulatedValuesFront[326]	= 0.10;		simulatedValuesRear[326]	= 0.10;
	simulatedValuesFront[327]	= 0.10;		simulatedValuesRear[327]	= 0.10;
	simulatedValuesFront[328]	= 0.10;		simulatedValuesRear[328]	= 0.10;
	simulatedValuesFront[329]	= 0.10;		simulatedValuesRear[329]	= 0.10;
	simulatedValuesFront[330]	= 0.10;		simulatedValuesRear[330]	= 0.10;
	simulatedValuesFront[331]	= 0.10;		simulatedValuesRear[331]	= 0.10;
	simulatedValuesFront[332]	= 0.10;		simulatedValuesRear[332]	= 0.10;
	simulatedValuesFront[333]	= 0.10;		simulatedValuesRear[333]	= 0.10;
	simulatedValuesFront[334]	= 0.10;		simulatedValuesRear[334]	= 0.10;
	simulatedValuesFront[335]	= 0.10;		simulatedValuesRear[335]	= 0.10;
	simulatedValuesFront[336]	= 0.10;		simulatedValuesRear[336]	= 0.10;
	simulatedValuesFront[337]	= 0.10;		simulatedValuesRear[337]	= 0.10;
	simulatedValuesFront[338]	= 0.10;		simulatedValuesRear[338]	= 0.10;
	simulatedValuesFront[339]	= 0.40;		simulatedValuesRear[339]	= 0.40;
	simulatedValuesFront[340]	= 0.40;		simulatedValuesRear[340]	= 0.40;
	simulatedValuesFront[341]	= 0.40;		simulatedValuesRear[341]	= 0.40;
	simulatedValuesFront[342]	= 0.40;		simulatedValuesRear[342]	= 0.40;
	simulatedValuesFront[343]	= 0.40;		simulatedValuesRear[343]	= 0.40;
	simulatedValuesFront[344]	= 0.40;		simulatedValuesRear[344]	= 0.40;
	simulatedValuesFront[345]	= 0.40;		simulatedValuesRear[345]	= 0.40;
	simulatedValuesFront[346]	= 0.40;		simulatedValuesRear[346]	= 0.40;
	simulatedValuesFront[347]	= 0.40;		simulatedValuesRear[347]	= 0.40;
	simulatedValuesFront[348]	= 0.40;		simulatedValuesRear[348]	= 0.40;
	simulatedValuesFront[349]	= 0.40;		simulatedValuesRear[349]	= 0.40;
	simulatedValuesFront[350]	= 0.40;		simulatedValuesRear[350]	= 0.40;
	simulatedValuesFront[351]	= 0.40;		simulatedValuesRear[351]	= 0.40;
	simulatedValuesFront[352]	= 0.40;		simulatedValuesRear[352]	= 0.40;
	simulatedValuesFront[353]	= 0.40;		simulatedValuesRear[353]	= 0.40;
	simulatedValuesFront[354]	= 0.40;		simulatedValuesRear[354]	= 0.40;
	simulatedValuesFront[355]	= 0.40;		simulatedValuesRear[355]	= 0.40;
	simulatedValuesFront[356]	= 0.40;		simulatedValuesRear[356]	= 0.40;
	simulatedValuesFront[359]	= 0.40;		simulatedValuesRear[359]	= 0.40;
}                                                       
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                
