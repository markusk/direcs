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
	laserscannerResolutionFront = 0.0;
	laserscannerResolutionRear = 0.0;
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
				// closing com port etc. is done in the laser class destructor
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
					// TODO: add support for 2 lasers...
					laserS300->readRequestTelegram(&laserScannerValuesFront[0]);
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


float LaserThread::getValue(short int laserScanner, int angle)
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
			return laserScannerValuesFront[angle];
			break;
		case LASER2:
			return laserScannerValuesRear[angle];
			break;
	}
	
	return 0;
}


int LaserThread::getFlag(short int laserScanner, int angle)
{
	if ((angle < 0) || ((laserScanner==LASER1) &&  (angle > laserscannerAngleFront)) || ((laserScanner==LASER2) &&  (angle > laserscannerAngleRear)) )
	{
		qDebug("laser angle out of alowed range (LaserThread::getLaserScannerFlag)");
		return 0;
	}

	
	switch(laserScanner)
	{
		case LASER1:
			return laserScannerFlagsFront[angle];
			break;
		case LASER2:
			return laserScannerFlagsRear[angle];
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
		for (int i=0; i<360; i++)
		{
			// the distances
			// laserScannerValuesFront[i] = 2.30; //(i+1) / 100;
			laserScannerValuesFront[i] = simulationValuesFront[i];
			// the flags
			laserScannerFlagsFront[i] = OBSTACLE;
		}

		for (int i=0; i<360; i++)
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
		for (int i=0; i<360; i++)
		{
			// the distances
			laserScannerValuesFront[i] = 0;
			// the flags
			laserScannerFlagsFront[i] = OBSTACLE;
		}

		for (int i=0; i<360; i++)
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
			if (laserscannerTypeFront == S300)
			{
				laserS300->setDevicePort(serialPort);
			}
			else
			{
				qDebug("laser type not supported (LaserThreadd::setSerialPort");
			}
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
			// TODO: support two S300 lasers
			qDebug("Only one laser S300 currently supported! (LaserThreadd::setSerialPort");
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


void LaserThread::setType(short int laserScanner, QString laserType)
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

			// let the splash screen from the direcs class show laser init messages
			// (connect the signal from the laser class to the signal from this class)
			connect(laserS300, SIGNAL(emitMessage(QString)), this, SIGNAL(message(QString)));

			return;
		}
}


void LaserThread::setAngle(short int laserScanner, int angle)
{
		switch (laserScanner)
		{
			case LASER1:
				// store value in the class member
				laserscannerAngleFront = angle;

				// initialisation of the laser values list (QVector). Create one object with length 0.0 for every laser line
				laserScannerValuesFront.fill(0.0, (laserscannerAngleFront/laserscannerResolutionFront)); // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<< FIXME: where to do this?

				// add a 0 flag
				laserScannerFlagsFront.fill(0.0, (laserscannerAngleFront/laserscannerResolutionFront)); // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<< FIXME: where to do this?

				// initialisation of the laser simulation values list (QVector)
				simulationValuesFront.fill(0.0, (laserscannerAngleFront/laserscannerResolutionFront)); // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<< FIXME: where to do this?
				break;
			case LASER2:
				// store value in the class member
				laserscannerAngleRear = angle;

				// initialisation of the laser values list (QVector). Create one object with length 0.0 for every laser line
				laserScannerValuesRear.fill(0.0, (laserscannerAngleRear/laserscannerResolutionRear)); // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<< FIXME: where to do this?

				// add a 0 flag
				laserScannerFlagsRear.fill(0.0, (laserscannerAngleRear/laserscannerResolutionRear)); // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<< FIXME: where to do this?

				// initialisation of the laser simulation values list (QVector)
				simulationValuesRear.fill(0.0, (laserscannerAngleRear/laserscannerResolutionRear)); // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<< FIXME: where to do this?
				break;
			default:
				qDebug("laser number not yet supported  (LaserThread::setLaserscannerAngle");
				break;
		}
}


void LaserThread::setResolution(short int laserScanner, float resolution)
{
	switch (laserScanner)
	{
		case LASER1:
			// store value in the class member
			laserscannerResolutionFront = resolution;

			// initialisation of the laser values list (QVector). Create one object with length 0.0 for every laser line
			laserScannerValuesFront.fill(0.0, (laserscannerAngleFront/laserscannerResolutionFront)); // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<< FIXME: where to do this?

			// add a 0 flag
			laserScannerFlagsFront.fill(0.0, (laserscannerAngleFront/laserscannerResolutionFront)); // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<< FIXME: where to do this?

			// initialisation of the laser simulation values list (QVector)
			simulationValuesFront.fill(0.0, (laserscannerAngleFront/laserscannerResolutionFront)); // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<< FIXME: where to do this?
			break;
		case LASER2:
			// store value in the class member
			laserscannerResolutionRear = resolution;

			// initialisation of the laser values list (QVector). Create one object with length 0.0 for every laser line
			laserScannerValuesRear.fill(0.0, (laserscannerAngleRear/laserscannerResolutionRear)); // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<< FIXME: where to do this?

			// add a 0 flag
			laserScannerFlagsRear.fill(0.0, (laserscannerAngleRear/laserscannerResolutionRear)); // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<< FIXME: where to do this?

			// initialisation of the laser simulation values list (QVector)
			simulationValuesRear.fill(0.0, (laserscannerAngleRear/laserscannerResolutionRear)); // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<< FIXME: where to do this?
			break;
		default:
			qDebug("laser number not yet supported  (LaserThread::setLaserscannerResolution");
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


float LaserThread::getResolution(short int laserScanner)
{
		switch (laserScanner)
		{
		case LASER1:
			return laserscannerResolutionFront;
			break;
		case LASER2:
			return laserscannerResolutionRear;
			break;
		}

		qDebug("laser number not yet supported  (LaserThreadd::getResolution");
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
				// TODO: Support two S300 scanners
				if (laserS300->openComPort() == true)
				{
					if (laserS300->setup() == 0)
					{
						laserScannerFrontIsConnected = true;
						return true;
					}
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
				qDebug("Support for a second S300 as rear laser scanner not implemented yet");
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
	simulationValuesFront[0]	= 2.07;		simulationValuesRear[0]		= 0.25;
	simulationValuesFront[1]	= 2.07;		simulationValuesRear[1]		= 0.25;
	simulationValuesFront[2]	= 2.07;		simulationValuesRear[2]		= 0.25;
	simulationValuesFront[3]	= 2.07;		simulationValuesRear[3]		= 0.25;
	simulationValuesFront[4]	= 2.07;		simulationValuesRear[4]		= 0.25;
	simulationValuesFront[5]	= 2.07;		simulationValuesRear[5]		= 0.25;
	simulationValuesFront[6]	= 2.07;		simulationValuesRear[6]		= 0.25;
	simulationValuesFront[7]	= 2.07;		simulationValuesRear[7]		= 0.25;
	simulationValuesFront[8]	= 2.07;		simulationValuesRear[8]		= 0.25;
	simulationValuesFront[9]	= 2.07;		simulationValuesRear[9]		= 0.25;
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
	simulationValuesFront[358]	= 6.40;		simulationValuesRear[358]	= 6.40;
	simulationValuesFront[359]	= 6.40;		simulationValuesRear[359]	= 6.40;
	simulationValuesFront[360] = 4.00;		simulationValuesRear[360] = 2.00;
	simulationValuesFront[361] = 4.00;		simulationValuesRear[361] = 2.00;
	simulationValuesFront[362] = 4.00;		simulationValuesRear[362] = 2.00;
	simulationValuesFront[363] = 4.00;		simulationValuesRear[363] = 2.00;
	simulationValuesFront[364] = 4.00;		simulationValuesRear[364] = 2.00;
	simulationValuesFront[365] = 4.00;		simulationValuesRear[365] = 2.00;
	simulationValuesFront[366] = 4.00;		simulationValuesRear[366] = 2.00;
	simulationValuesFront[367] = 4.00;		simulationValuesRear[367] = 2.00;
	simulationValuesFront[368] = 4.00;		simulationValuesRear[368] = 2.00;
	simulationValuesFront[369] = 4.00;		simulationValuesRear[369] = 2.00;
	simulationValuesFront[370] = 4.00;		simulationValuesRear[370] = 2.00;
	simulationValuesFront[371] = 4.00;		simulationValuesRear[371] = 2.00;
	simulationValuesFront[372] = 4.00;		simulationValuesRear[372] = 2.00;
	simulationValuesFront[373] = 4.00;		simulationValuesRear[373] = 2.00;
	simulationValuesFront[374] = 4.00;		simulationValuesRear[374] = 2.00;
	simulationValuesFront[375] = 4.00;		simulationValuesRear[375] = 2.00;
	simulationValuesFront[376] = 4.00;		simulationValuesRear[376] = 2.00;
	simulationValuesFront[377] = 4.00;		simulationValuesRear[377] = 2.00;
	simulationValuesFront[378] = 4.00;		simulationValuesRear[378] = 2.00;
	simulationValuesFront[379] = 4.00;		simulationValuesRear[379] = 2.00;
	simulationValuesFront[380] = 4.00;		simulationValuesRear[380] = 2.00;
	simulationValuesFront[381] = 4.00;		simulationValuesRear[381] = 2.00;
	simulationValuesFront[382] = 3.00;		simulationValuesRear[382] = 4.00;
	simulationValuesFront[383] = 3.00;		simulationValuesRear[383] = 4.00;
	simulationValuesFront[384] = 3.00;		simulationValuesRear[384] = 4.00;
	simulationValuesFront[385] = 3.00;		simulationValuesRear[385] = 4.00;
	simulationValuesFront[386] = 3.00;		simulationValuesRear[386] = 4.00;
	simulationValuesFront[387] = 3.00;		simulationValuesRear[387] = 4.00;
	simulationValuesFront[388] = 3.00;		simulationValuesRear[388] = 4.00;
	simulationValuesFront[389] = 3.00;		simulationValuesRear[389] = 4.00;
	simulationValuesFront[390] = 3.00;		simulationValuesRear[390] = 4.00;
	simulationValuesFront[391] = 3.00;		simulationValuesRear[391] = 4.00;
	simulationValuesFront[392] = 3.00;		simulationValuesRear[392] = 4.00;
	simulationValuesFront[393] = 3.00;		simulationValuesRear[393] = 4.00;
	simulationValuesFront[394] = 3.00;		simulationValuesRear[394] = 4.00;
	simulationValuesFront[395] = 3.00;		simulationValuesRear[395] = 4.00;
	simulationValuesFront[396] = 3.00;		simulationValuesRear[396] = 4.00;
	simulationValuesFront[397] = 3.00;		simulationValuesRear[397] = 4.00;
	simulationValuesFront[398] = 3.00;		simulationValuesRear[398] = 4.00;
	simulationValuesFront[399] = 3.00;		simulationValuesRear[399] = 4.00;
	simulationValuesFront[400] = 3.00;		simulationValuesRear[400] = 4.00;
	simulationValuesFront[401] = 3.00;		simulationValuesRear[401] = 4.00;
	simulationValuesFront[402] = 3.00;		simulationValuesRear[402] = 4.00;
	simulationValuesFront[403] = 3.00;		simulationValuesRear[403] = 4.00;
	simulationValuesFront[404] = 3.00;		simulationValuesRear[404] = 4.00;
	simulationValuesFront[405] = 3.00;		simulationValuesRear[405] = 4.00;
	simulationValuesFront[406] = 3.00;		simulationValuesRear[406] = 4.00;
	simulationValuesFront[407] = 3.00;		simulationValuesRear[407] = 4.00;
	simulationValuesFront[408] = 3.00;		simulationValuesRear[408] = 4.00;
	simulationValuesFront[409] = 3.00;		simulationValuesRear[409] = 4.00;
	simulationValuesFront[410] = 3.00;		simulationValuesRear[410] = 4.00;
	simulationValuesFront[411] = 3.00;		simulationValuesRear[411] = 4.00;
	simulationValuesFront[412] = 3.00;		simulationValuesRear[412] = 4.00;
	simulationValuesFront[413] = 3.00;		simulationValuesRear[413] = 4.00;
	simulationValuesFront[414] = 3.00;		simulationValuesRear[414] = 4.00;
	simulationValuesFront[415] = 3.00;		simulationValuesRear[415] = 4.00;
	simulationValuesFront[416] = 3.00;		simulationValuesRear[416] = 4.00;
	simulationValuesFront[417] = 3.00;		simulationValuesRear[417] = 4.00;
	simulationValuesFront[418] = 3.00;		simulationValuesRear[418] = 4.00;
	simulationValuesFront[419] = 3.00;		simulationValuesRear[419] = 4.00;
	simulationValuesFront[420] = 3.00;		simulationValuesRear[420] = 4.00;
	simulationValuesFront[421] = 3.00;		simulationValuesRear[421] = 4.00;
	simulationValuesFront[422] = 3.00;		simulationValuesRear[422] = 4.00;
	simulationValuesFront[423] = 3.00;		simulationValuesRear[423] = 4.00;
	simulationValuesFront[424] = 3.00;		simulationValuesRear[424] = 4.00;
	simulationValuesFront[425] = 3.00;		simulationValuesRear[425] = 4.00;
	simulationValuesFront[426] = 3.00;		simulationValuesRear[426] = 4.00;
	simulationValuesFront[427] = 3.00;		simulationValuesRear[427] = 4.00;
	simulationValuesFront[428] = 3.00;		simulationValuesRear[428] = 4.00;
	simulationValuesFront[429] = 3.00;		simulationValuesRear[429] = 4.00;
	simulationValuesFront[430] = 3.00;		simulationValuesRear[430] = 4.00;
	simulationValuesFront[431] = 3.00;		simulationValuesRear[431] = 4.00;
	simulationValuesFront[432] = 3.00;		simulationValuesRear[432] = 4.00;
	simulationValuesFront[433] = 2.00;		simulationValuesRear[433] = 5.00;
	simulationValuesFront[434] = 2.00;		simulationValuesRear[434] = 5.00;
	simulationValuesFront[435] = 2.00;		simulationValuesRear[435] = 5.00;
	simulationValuesFront[436] = 2.00;		simulationValuesRear[436] = 5.00;
	simulationValuesFront[437] = 2.00;		simulationValuesRear[437] = 5.00;
	simulationValuesFront[438] = 2.00;		simulationValuesRear[438] = 5.00;
	simulationValuesFront[439] = 2.00;		simulationValuesRear[439] = 5.00;
	simulationValuesFront[440] = 2.00;		simulationValuesRear[440] = 5.00;
	simulationValuesFront[441] = 2.00;		simulationValuesRear[441] = 5.00;
	simulationValuesFront[442] = 2.00;		simulationValuesRear[442] = 5.00;
	simulationValuesFront[443] = 2.00;		simulationValuesRear[443] = 5.00;
	simulationValuesFront[444] = 2.00;		simulationValuesRear[444] = 5.00;
	simulationValuesFront[445] = 2.00;		simulationValuesRear[445] = 5.00;
	simulationValuesFront[446] = 2.00;		simulationValuesRear[446] = 5.00;
	simulationValuesFront[447] = 2.00;		simulationValuesRear[447] = 5.00;
	simulationValuesFront[448] = 2.00;		simulationValuesRear[448] = 5.00;
	simulationValuesFront[449] = 2.00;		simulationValuesRear[449] = 5.00;
	simulationValuesFront[450] = 2.00;		simulationValuesRear[450] = 5.00;
	simulationValuesFront[451] = 2.00;		simulationValuesRear[451] = 5.00;
	simulationValuesFront[452] = 2.00;		simulationValuesRear[452] = 5.00;
	simulationValuesFront[453] = 2.00;		simulationValuesRear[453] = 5.00;
	simulationValuesFront[454] = 2.00;		simulationValuesRear[454] = 5.00;
	simulationValuesFront[455] = 2.00;		simulationValuesRear[455] = 5.00;
	simulationValuesFront[456] = 2.00;		simulationValuesRear[456] = 5.00;
	simulationValuesFront[457] = 2.00;		simulationValuesRear[457] = 5.00;
	simulationValuesFront[458] = 2.00;		simulationValuesRear[458] = 5.00;
	simulationValuesFront[459] = 2.00;		simulationValuesRear[459] = 5.00;
	simulationValuesFront[460] = 2.00;		simulationValuesRear[460] = 5.00;
	simulationValuesFront[461] = 2.00;		simulationValuesRear[461] = 5.00;
	simulationValuesFront[462] = 2.00;		simulationValuesRear[462] = 5.00;
	simulationValuesFront[463] = 2.00;		simulationValuesRear[463] = 5.00;
	simulationValuesFront[464] = 2.00;		simulationValuesRear[464] = 5.00;
	simulationValuesFront[465] = 2.00;		simulationValuesRear[465] = 5.00;
	simulationValuesFront[466] = 2.00;		simulationValuesRear[466] = 5.00;
	simulationValuesFront[467] = 2.00;		simulationValuesRear[467] = 5.00;
	simulationValuesFront[468] = 2.00;		simulationValuesRear[468] = 5.00;
	simulationValuesFront[469] = 2.00;		simulationValuesRear[469] = 5.00;
	simulationValuesFront[470] = 2.00;		simulationValuesRear[470] = 5.00;
	simulationValuesFront[471] = 2.00;		simulationValuesRear[471] = 5.00;
	simulationValuesFront[472] = 2.00;		simulationValuesRear[472] = 5.00;
	simulationValuesFront[473] = 2.00;		simulationValuesRear[473] = 5.00;
	simulationValuesFront[474] = 2.00;		simulationValuesRear[474] = 5.00;
	simulationValuesFront[475] = 2.00;		simulationValuesRear[475] = 5.00;
	simulationValuesFront[476] = 2.00;		simulationValuesRear[476] = 5.00;
	simulationValuesFront[477] = 2.00;		simulationValuesRear[477] = 5.00;
	simulationValuesFront[478] = 2.00;		simulationValuesRear[478] = 5.00;
	simulationValuesFront[479] = 2.00;		simulationValuesRear[479] = 5.00;
	simulationValuesFront[480] = 2.00;		simulationValuesRear[480] = 5.00;
	simulationValuesFront[481] = 2.00;		simulationValuesRear[481] = 5.00;
	simulationValuesFront[482] = 2.00;		simulationValuesRear[482] = 5.00;
	simulationValuesFront[483] = 2.00;		simulationValuesRear[483] = 5.00;
	simulationValuesFront[484] = 2.00;		simulationValuesRear[484] = 5.00;
	simulationValuesFront[485] = 2.00;		simulationValuesRear[485] = 5.00;
	simulationValuesFront[486] = 2.00;		simulationValuesRear[486] = 5.00;
	simulationValuesFront[487] = 2.00;		simulationValuesRear[487] = 5.00;
	simulationValuesFront[488] = 2.00;		simulationValuesRear[488] = 5.00;
	simulationValuesFront[489] = 2.00;		simulationValuesRear[489] = 5.00;
	simulationValuesFront[490] = 2.00;		simulationValuesRear[490] = 5.00;
	simulationValuesFront[491] = 2.00;		simulationValuesRear[491] = 5.00;
	simulationValuesFront[492] = 2.00;		simulationValuesRear[492] = 5.00;
	simulationValuesFront[493] = 2.00;		simulationValuesRear[493] = 5.00;
	simulationValuesFront[494] = 2.00;		simulationValuesRear[494] = 5.00;
	simulationValuesFront[495] = 2.00;		simulationValuesRear[495] = 5.00;
	simulationValuesFront[496] = 2.00;		simulationValuesRear[496] = 5.00;
	simulationValuesFront[497] = 2.00;		simulationValuesRear[497] = 5.00;
	simulationValuesFront[498] = 2.00;		simulationValuesRear[498] = 5.00;
	simulationValuesFront[499] = 2.00;		simulationValuesRear[499] = 5.00;
	simulationValuesFront[500] = 2.00;		simulationValuesRear[500] = 5.00;
	simulationValuesFront[501] = 2.00;		simulationValuesRear[501] = 5.00;
	simulationValuesFront[502] = 2.00;		simulationValuesRear[502] = 5.00;
	simulationValuesFront[503] = 2.00;		simulationValuesRear[503] = 5.00;
	simulationValuesFront[504] = 2.00;		simulationValuesRear[504] = 5.00;
	simulationValuesFront[505] = 2.00;		simulationValuesRear[505] = 5.00;
	simulationValuesFront[506] = 2.00;		simulationValuesRear[506] = 5.00;
	simulationValuesFront[507] = 2.00;		simulationValuesRear[507] = 5.00;
	simulationValuesFront[508] = 2.00;		simulationValuesRear[508] = 5.00;
	simulationValuesFront[509] = 2.00;		simulationValuesRear[509] = 5.00;
	simulationValuesFront[510] = 2.00;		simulationValuesRear[510] = 5.00;
	simulationValuesFront[511] = 2.00;		simulationValuesRear[511] = 5.00;
	simulationValuesFront[512] = 2.00;		simulationValuesRear[512] = 5.00;
	simulationValuesFront[513] = 2.00;		simulationValuesRear[513] = 5.00;
	simulationValuesFront[514] = 2.00;		simulationValuesRear[514] = 5.00;
	simulationValuesFront[515] = 2.00;		simulationValuesRear[515] = 5.00;
	simulationValuesFront[516] = 2.00;		simulationValuesRear[516] = 5.00;
	simulationValuesFront[517] = 2.00;		simulationValuesRear[517] = 5.00;
	simulationValuesFront[518] = 2.00;		simulationValuesRear[518] = 5.00;
	simulationValuesFront[519] = 2.00;		simulationValuesRear[519] = 5.00;
	simulationValuesFront[520] = 2.00;		simulationValuesRear[520] = 5.00;
	simulationValuesFront[521] = 2.00;		simulationValuesRear[521] = 5.00;
	simulationValuesFront[522] = 2.00;		simulationValuesRear[522] = 5.00;
	simulationValuesFront[523] = 2.00;		simulationValuesRear[523] = 5.00;
	simulationValuesFront[524] = 2.00;		simulationValuesRear[524] = 5.00;
	simulationValuesFront[525] = 2.00;		simulationValuesRear[525] = 5.00;
	simulationValuesFront[526] = 2.00;		simulationValuesRear[526] = 5.00;
	simulationValuesFront[527] = 2.00;		simulationValuesRear[527] = 5.00;
	simulationValuesFront[528] = 2.00;		simulationValuesRear[528] = 5.00;
	simulationValuesFront[529] = 2.00;		simulationValuesRear[529] = 5.00;
	simulationValuesFront[530] = 2.00;		simulationValuesRear[530] = 5.00;
	simulationValuesFront[531] = 2.00;		simulationValuesRear[531] = 5.00;
	simulationValuesFront[532] = 2.00;		simulationValuesRear[532] = 5.00;
	simulationValuesFront[533] = 2.00;		simulationValuesRear[533] = 5.00;
	simulationValuesFront[534] = 2.00;		simulationValuesRear[534] = 5.00;
	simulationValuesFront[535] = 2.00;		simulationValuesRear[535] = 5.00;
	simulationValuesFront[536] = 2.00;		simulationValuesRear[536] = 5.00;
	simulationValuesFront[537] = 2.00;		simulationValuesRear[537] = 5.00;
	simulationValuesFront[538] = 2.00;		simulationValuesRear[538] = 5.00;
	simulationValuesFront[539] = 2.00;		simulationValuesRear[539] = 5.00;
	simulationValuesFront[540] = 2.00;		simulationValuesRear[540] = 5.00;
	simulationValuesFront[541] = 2.00;		simulationValuesRear[541] = 5.00;
	simulationValuesFront[542] = 2.00;		simulationValuesRear[542] = 5.00;
	simulationValuesFront[543] = 2.00;		simulationValuesRear[543] = 5.00;
	simulationValuesFront[544] = 2.00;		simulationValuesRear[544] = 5.00;
	simulationValuesFront[545] = 2.00;		simulationValuesRear[545] = 5.00;
	simulationValuesFront[546] = 2.00;		simulationValuesRear[546] = 5.00;
	simulationValuesFront[547] = 2.00;		simulationValuesRear[547] = 5.00;
	simulationValuesFront[548] = 2.00;		simulationValuesRear[548] = 5.00;
	simulationValuesFront[549] = 2.00;		simulationValuesRear[549] = 5.00;
	simulationValuesFront[550] = 2.00;		simulationValuesRear[550] = 5.00;
	simulationValuesFront[551] = 2.00;		simulationValuesRear[551] = 5.00;
	simulationValuesFront[552] = 2.00;		simulationValuesRear[552] = 5.00;
	simulationValuesFront[553] = 2.00;		simulationValuesRear[553] = 5.00;
	simulationValuesFront[554] = 2.00;		simulationValuesRear[554] = 5.00;
	simulationValuesFront[555] = 2.00;		simulationValuesRear[555] = 5.00;
	simulationValuesFront[556] = 2.00;		simulationValuesRear[556] = 5.00;
	simulationValuesFront[557] = 2.00;		simulationValuesRear[557] = 5.00;
	simulationValuesFront[558] = 2.00;		simulationValuesRear[558] = 5.00;
	simulationValuesFront[559] = 2.00;		simulationValuesRear[559] = 5.00;
	simulationValuesFront[560] = 2.00;		simulationValuesRear[560] = 5.00;
	simulationValuesFront[561] = 2.00;		simulationValuesRear[561] = 5.00;
	simulationValuesFront[562] = 2.00;		simulationValuesRear[562] = 5.00;
	simulationValuesFront[563] = 2.00;		simulationValuesRear[563] = 5.00;
	simulationValuesFront[564] = 2.00;		simulationValuesRear[564] = 5.00;
	simulationValuesFront[565] = 2.00;		simulationValuesRear[565] = 5.00;
	simulationValuesFront[566] = 2.00;		simulationValuesRear[566] = 5.00;
	simulationValuesFront[567] = 2.00;		simulationValuesRear[567] = 5.00;
	simulationValuesFront[568] = 2.00;		simulationValuesRear[568] = 5.00;
	simulationValuesFront[569] = 2.00;		simulationValuesRear[569] = 5.00;
	simulationValuesFront[570] = 2.00;		simulationValuesRear[570] = 5.00;
	simulationValuesFront[571] = 2.00;		simulationValuesRear[571] = 5.00;
	simulationValuesFront[572] = 2.00;		simulationValuesRear[572] = 5.00;
	simulationValuesFront[573] = 2.00;		simulationValuesRear[573] = 5.00;
	simulationValuesFront[574] = 2.00;		simulationValuesRear[574] = 5.00;
	simulationValuesFront[575] = 2.00;		simulationValuesRear[575] = 5.00;
	simulationValuesFront[576] = 2.00;		simulationValuesRear[576] = 5.00;
	simulationValuesFront[577] = 2.00;		simulationValuesRear[577] = 5.00;
	simulationValuesFront[578] = 2.00;		simulationValuesRear[578] = 5.00;
	simulationValuesFront[579] = 2.00;		simulationValuesRear[579] = 5.00;
	simulationValuesFront[580] = 2.00;		simulationValuesRear[580] = 5.00;
	simulationValuesFront[581] = 2.00;		simulationValuesRear[581] = 5.00;
	simulationValuesFront[582] = 1.00;		simulationValuesRear[582] = 6.00;
	simulationValuesFront[583] = 1.00;		simulationValuesRear[583] = 6.00;
	simulationValuesFront[584] = 1.00;		simulationValuesRear[584] = 6.00;
	simulationValuesFront[585] = 1.00;		simulationValuesRear[585] = 6.00;
	simulationValuesFront[586] = 1.00;		simulationValuesRear[586] = 6.00;
	simulationValuesFront[587] = 1.00;		simulationValuesRear[587] = 6.00;
	simulationValuesFront[588] = 1.00;		simulationValuesRear[588] = 6.00;
	simulationValuesFront[589] = 1.00;		simulationValuesRear[589] = 6.00;
	simulationValuesFront[590] = 1.00;		simulationValuesRear[590] = 6.00;
	simulationValuesFront[591] = 1.00;		simulationValuesRear[591] = 6.00;
	simulationValuesFront[592] = 1.00;		simulationValuesRear[592] = 6.00;
	simulationValuesFront[593] = 1.00;		simulationValuesRear[593] = 6.00;
	simulationValuesFront[594] = 1.00;		simulationValuesRear[594] = 6.00;
	simulationValuesFront[595] = 1.00;		simulationValuesRear[595] = 6.00;
	simulationValuesFront[596] = 1.00;		simulationValuesRear[596] = 6.00;
	simulationValuesFront[597] = 1.00;		simulationValuesRear[597] = 6.00;
	simulationValuesFront[598] = 1.00;		simulationValuesRear[598] = 6.00;
	simulationValuesFront[599] = 1.00;		simulationValuesRear[599] = 6.00;
	simulationValuesFront[600] = 1.00;		simulationValuesRear[600] = 6.00;
	simulationValuesFront[601] = 1.00;		simulationValuesRear[601] = 6.00;
	simulationValuesFront[602] = 1.00;		simulationValuesRear[602] = 6.00;
	simulationValuesFront[603] = 1.00;		simulationValuesRear[603] = 6.00;
	simulationValuesFront[604] = 1.00;		simulationValuesRear[604] = 6.00;
	simulationValuesFront[605] = 1.00;		simulationValuesRear[605] = 6.00;
	simulationValuesFront[606] = 1.00;		simulationValuesRear[606] = 6.00;
	simulationValuesFront[607] = 1.00;		simulationValuesRear[607] = 6.00;
	simulationValuesFront[608] = 1.00;		simulationValuesRear[608] = 6.00;
	simulationValuesFront[609] = 1.00;		simulationValuesRear[609] = 6.00;
	simulationValuesFront[610] = 1.00;		simulationValuesRear[610] = 6.00;
	simulationValuesFront[611] = 1.00;		simulationValuesRear[611] = 6.00;
	simulationValuesFront[612] = 1.00;		simulationValuesRear[612] = 6.00;
	simulationValuesFront[613] = 1.00;		simulationValuesRear[613] = 6.00;
	simulationValuesFront[614] = 1.00;		simulationValuesRear[614] = 6.00;
	simulationValuesFront[615] = 1.00;		simulationValuesRear[615] = 6.00;
	simulationValuesFront[616] = 1.00;		simulationValuesRear[616] = 6.00;
	simulationValuesFront[617] = 1.00;		simulationValuesRear[617] = 6.00;
	simulationValuesFront[618] = 1.00;		simulationValuesRear[618] = 6.00;
	simulationValuesFront[619] = 1.00;		simulationValuesRear[619] = 6.00;
	simulationValuesFront[620] = 1.00;		simulationValuesRear[620] = 6.00;
	simulationValuesFront[621] = 1.00;		simulationValuesRear[621] = 6.00;
	simulationValuesFront[622] = 1.00;		simulationValuesRear[622] = 6.00;
	simulationValuesFront[623] = 1.00;		simulationValuesRear[623] = 6.00;
	simulationValuesFront[624] = 1.00;		simulationValuesRear[624] = 6.00;
	simulationValuesFront[625] = 1.00;		simulationValuesRear[625] = 6.00;
	simulationValuesFront[626] = 1.00;		simulationValuesRear[626] = 6.00;
	simulationValuesFront[627] = 1.00;		simulationValuesRear[627] = 6.00;
	simulationValuesFront[628] = 1.00;		simulationValuesRear[628] = 6.00;
	simulationValuesFront[629] = 1.00;		simulationValuesRear[629] = 6.00;
	simulationValuesFront[630] = 1.00;		simulationValuesRear[630] = 6.00;
	simulationValuesFront[631] = 1.00;		simulationValuesRear[631] = 6.00;
	simulationValuesFront[632] = 1.00;		simulationValuesRear[632] = 6.00;
	simulationValuesFront[633] = 1.00;		simulationValuesRear[633] = 6.00;
	simulationValuesFront[634] = 1.00;		simulationValuesRear[634] = 6.00;
	simulationValuesFront[635] = 1.00;		simulationValuesRear[635] = 6.00;
	simulationValuesFront[636] = 1.00;		simulationValuesRear[636] = 6.00;
	simulationValuesFront[637] = 1.00;		simulationValuesRear[637] = 6.00;
	simulationValuesFront[638] = 1.00;		simulationValuesRear[638] = 6.00;
	simulationValuesFront[639] = 1.00;		simulationValuesRear[639] = 6.00;
	simulationValuesFront[640] = 1.00;		simulationValuesRear[640] = 6.00;
	simulationValuesFront[641] = 1.00;		simulationValuesRear[641] = 6.00;
	simulationValuesFront[642] = 1.00;		simulationValuesRear[642] = 6.00;
	simulationValuesFront[643] = 1.00;		simulationValuesRear[643] = 6.00;
	simulationValuesFront[644] = 1.00;		simulationValuesRear[644] = 6.00;
	simulationValuesFront[645] = 1.00;		simulationValuesRear[645] = 6.00;
	simulationValuesFront[646] = 1.00;		simulationValuesRear[646] = 6.00;
	simulationValuesFront[647] = 1.00;		simulationValuesRear[647] = 6.00;
	simulationValuesFront[648] = 1.00;		simulationValuesRear[648] = 6.00;
	simulationValuesFront[649] = 1.00;		simulationValuesRear[649] = 6.00;
	simulationValuesFront[650] = 1.00;		simulationValuesRear[650] = 6.00;
	simulationValuesFront[651] = 1.00;		simulationValuesRear[651] = 6.00;
	simulationValuesFront[652] = 1.00;		simulationValuesRear[652] = 6.00;
	simulationValuesFront[653] = 1.00;		simulationValuesRear[653] = 6.00;
	simulationValuesFront[654] = 1.00;		simulationValuesRear[654] = 6.00;
	simulationValuesFront[655] = 1.00;		simulationValuesRear[655] = 6.00;
	simulationValuesFront[656] = 1.00;		simulationValuesRear[656] = 6.00;
	simulationValuesFront[657] = 1.00;		simulationValuesRear[657] = 6.00;
	simulationValuesFront[658] = 1.00;		simulationValuesRear[658] = 6.00;
	simulationValuesFront[659] = 1.00;		simulationValuesRear[659] = 6.00;
	simulationValuesFront[660] = 1.00;		simulationValuesRear[660] = 6.00;
	simulationValuesFront[661] = 1.00;		simulationValuesRear[661] = 6.00;
	simulationValuesFront[662] = 1.00;		simulationValuesRear[662] = 6.00;
	simulationValuesFront[663] = 1.00;		simulationValuesRear[663] = 6.00;
	simulationValuesFront[664] = 1.00;		simulationValuesRear[664] = 6.00;
	simulationValuesFront[665] = 1.00;		simulationValuesRear[665] = 6.00;
	simulationValuesFront[666] = 1.00;		simulationValuesRear[666] = 6.00;
	simulationValuesFront[667] = 1.00;		simulationValuesRear[667] = 6.00;
	simulationValuesFront[668] = 1.00;		simulationValuesRear[668] = 6.00;
	simulationValuesFront[669] = 1.00;		simulationValuesRear[669] = 6.00;
	simulationValuesFront[670] = 1.00;		simulationValuesRear[670] = 6.00;
	simulationValuesFront[671] = 1.00;		simulationValuesRear[671] = 6.00;
	simulationValuesFront[672] = 1.00;		simulationValuesRear[672] = 6.00;
	simulationValuesFront[673] = 1.00;		simulationValuesRear[673] = 6.00;
	simulationValuesFront[674] = 1.00;		simulationValuesRear[674] = 6.00;
	simulationValuesFront[675] = 1.00;		simulationValuesRear[675] = 6.00;
	simulationValuesFront[676] = 1.00;		simulationValuesRear[676] = 6.00;
	simulationValuesFront[677] = 1.00;		simulationValuesRear[677] = 6.00;
	simulationValuesFront[678] = 1.00;		simulationValuesRear[678] = 6.00;
	simulationValuesFront[679] = 1.00;		simulationValuesRear[679] = 6.00;
	simulationValuesFront[680] = 1.00;		simulationValuesRear[680] = 6.00;
	simulationValuesFront[681] = 1.00;		simulationValuesRear[681] = 6.00;
	simulationValuesFront[682] = 1.00;		simulationValuesRear[682] = 6.00;
	simulationValuesFront[683] = 1.00;		simulationValuesRear[683] = 6.00;
	simulationValuesFront[684] = 1.00;		simulationValuesRear[684] = 6.00;
	simulationValuesFront[685] = 1.00;		simulationValuesRear[685] = 6.00;
	simulationValuesFront[686] = 1.00;		simulationValuesRear[686] = 6.00;
	simulationValuesFront[687] = 1.00;		simulationValuesRear[687] = 6.00;
	simulationValuesFront[688] = 1.00;		simulationValuesRear[688] = 6.00;
	simulationValuesFront[689] = 1.00;		simulationValuesRear[689] = 6.00;
	simulationValuesFront[690] = 1.00;		simulationValuesRear[690] = 6.00;
	simulationValuesFront[691] = 1.00;		simulationValuesRear[691] = 6.00;
	simulationValuesFront[692] = 1.00;		simulationValuesRear[692] = 6.00;
	simulationValuesFront[693] = 1.00;		simulationValuesRear[693] = 6.00;
	simulationValuesFront[694] = 1.00;		simulationValuesRear[694] = 6.00;
	simulationValuesFront[695] = 1.00;		simulationValuesRear[695] = 6.00;
	simulationValuesFront[696] = 1.00;		simulationValuesRear[696] = 6.00;
	simulationValuesFront[697] = 1.00;		simulationValuesRear[697] = 6.00;
	simulationValuesFront[698] = 1.00;		simulationValuesRear[698] = 6.00;
	simulationValuesFront[699] = 1.00;		simulationValuesRear[699] = 6.00;
	simulationValuesFront[700] = 1.00;		simulationValuesRear[700] = 6.00;
	simulationValuesFront[701] = 1.00;		simulationValuesRear[701] = 6.00;
	simulationValuesFront[702] = 1.00;		simulationValuesRear[702] = 6.00;
	simulationValuesFront[703] = 1.00;		simulationValuesRear[703] = 6.00;
	simulationValuesFront[704] = 1.00;		simulationValuesRear[704] = 6.00;
	simulationValuesFront[705] = 1.00;		simulationValuesRear[705] = 6.00;
	simulationValuesFront[706] = 1.00;		simulationValuesRear[706] = 6.00;
	simulationValuesFront[707] = 1.00;		simulationValuesRear[707] = 6.00;
	simulationValuesFront[708] = 1.00;		simulationValuesRear[708] = 6.00;
	simulationValuesFront[709] = 1.00;		simulationValuesRear[709] = 6.00;
	simulationValuesFront[710] = 1.00;		simulationValuesRear[710] = 6.00;
	simulationValuesFront[711] = 1.00;		simulationValuesRear[711] = 6.00;
	simulationValuesFront[712] = 1.00;		simulationValuesRear[712] = 6.00;
	simulationValuesFront[713] = 1.00;		simulationValuesRear[713] = 6.00;
	simulationValuesFront[714] = 1.00;		simulationValuesRear[714] = 6.00;
	simulationValuesFront[715] = 1.00;		simulationValuesRear[715] = 6.00;
	simulationValuesFront[716] = 1.00;		simulationValuesRear[716] = 6.00;
	simulationValuesFront[717] = 1.00;		simulationValuesRear[717] = 6.00;
	simulationValuesFront[718] = 1.00;		simulationValuesRear[718] = 6.00;
	simulationValuesFront[719] = 1.00;		simulationValuesRear[719] = 6.00;
}                                                       
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                
