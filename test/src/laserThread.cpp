/*************************************************************************
 *   Copyright (C) 2011 by Markus Knapp                                  *
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
	numReadingsRear  = 0;
	laserscannerAngleFront = -1;
	laserscannerAngleRear  = -1;
	laserscannerResolutionFront = -1.0;
	laserscannerResolutionRear  = -1.0;

	// for the simulation values
	inifile1 = new Inifile();
	inifile1->setFilename("direcs.sim");
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

	delete inifile1;
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


	// check if all necessary values wer set / members are initialised
	if ( (laserScannerFrontIsConnected && laserScannerValuesFront.isEmpty()) && (laserScannerRearIsConnected && laserScannerValuesRear.isEmpty()))
	{
		// don't thread
		stopped = true;
	}

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
					emit laserDataCompleteFront(laserScannerValuesFront, laserScannerFlagsFront);
					break;
				case LASER2:
					getAndStoreLaserValuesRear();
					emit laserDataCompleteRear(laserScannerValuesRear, laserScannerFlagsRear);
					break;
				case (LASER1+LASER2):
					getAndStoreLaserValuesFront();
					emit laserDataCompleteFront(laserScannerValuesFront, laserScannerFlagsFront);
					getAndStoreLaserValuesRear();
					emit laserDataCompleteRear(laserScannerValuesRear, laserScannerFlagsRear);
					break;
				}
			}
			else
			{
				if ( (laserscannerTypeFront==S300) || (laserscannerTypeRear==S300))
				{
					/// \todo add support for 2 lasers...
					if (laserS300->readRequestTelegram() != -1)
					{
						getAndStoreLaserValuesFront();
						emit laserDataCompleteFront(laserScannerValuesFront, laserScannerFlagsFront);
					}
					else
					{
						stopped = true;
						emit message("ERROR reading S300 data. Laser thread stopped!");
					}
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
			emit laserDataCompleteFront(laserScannerValuesFront, laserScannerFlagsFront);
			emit laserDataCompleteRear(laserScannerValuesRear, laserScannerFlagsRear);
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
				// /get the data from 0 to 180 degrees (left to right)
				for (int angle=0; angle<numReadingsFront; angle++)
				{
					// get value from laser
					// store the value in an array in this thread
					laserScannerValuesFront[angle] = laser->getLaserDistance(LASER1, angle);

					// send value over the network
					// *0l23a42# means LASER1 has at angle 23 a length of 42 cm
					emit sendNetworkString( QString("*%1l%2a%3#").arg(LASER1).arg(angle).arg( (int) (laserScannerValuesFront[angle]*100) ) );
				}
			}
			else
			{
				// flip the data, due to a flipped mounting of the hardware!
				//
				// get the data from 0 to 180 degrees (left to right)
				// 'flip' will be increased every step - 1, so the data are stored from 180° to 0°
				for (int angle=0, flip=numReadingsFront-1; angle<numReadingsFront; angle++, flip--)
				{
					// get value from laser
					// store the value in an array in this thread
					laserScannerValuesFront[flip] = laser->getLaserDistance(LASER1, angle);

					// send value over the network
					// *0l23a42# means LASER1 has at angle 23 a length of 42 cm
					emit sendNetworkString( QString("*%1l%2a%3#").arg(LASER1).arg(angle).arg( (int) (laserScannerValuesFront[angle]*100) ) );
				}
			}
		}
	}
	else
	{
		if (laserscannerTypeFront == S300)
		{
			if (mountingLaserscannerFront == "normal")
			{
				// /get the data from 0 to 270 degrees (left to right)
				// since we have a resolution at 0.5 degrees, this is an index for the array with 540 values!
				for (int angleIndex=0; angleIndex<(laserscannerAngleFront/laserscannerResolutionFront); angleIndex++)
				{
					// get value from laser
					// store the value in an array in this thread
					laserScannerValuesFront[angleIndex] = laserS300->getDistance(angleIndex);

					// send value over the network
					// *0l23a42# means LASER1 has at angle 23 a length of 42 cm
					emit sendNetworkString( QString("*%1l%2a%3#").arg(LASER1).arg(angleIndex).arg( (int) (laserScannerValuesFront[angleIndex]*100) ) );
				}
			}
			else
			{
				// flip the data, due to a flipped mounting of the hardware!
				//
				// get the data from 0 to 270 degrees (left to right)
				// since we have a resolution at 0.5 degrees, this is an index for the array with 540 values!
				// 'flip' will be increased every step - 1, so the data are stored from 270� to 0�
				for (int angleIndex=0, flip=(laserscannerAngleFront/laserscannerResolutionFront)-1; angleIndex<(laserscannerAngleFront/laserscannerResolutionFront); angleIndex++, flip--)
				{
					// get value from laser
					// store the value in an array in this thread
					laserScannerValuesFront[flip] = laserS300->getDistance(angleIndex);

					// send value over the network
					// *0l23a42# means LASER1 has at angle 23 a length of 42 cm
					emit sendNetworkString( QString("*%1l%2a%3#").arg(LASER1).arg(angleIndex).arg( (int) (laserScannerValuesFront[angleIndex]*100) ) );
				}
			}
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
					emit sendNetworkString( QString("*%1l%2a%3#").arg(LASER2).arg(angle).arg( (int) (laserScannerValuesRear[angle]*100) ) );
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
					emit sendNetworkString( QString("*%1l%2a%3#").arg(LASER2).arg(angle).arg( (int) (laserScannerValuesRear[angle]*100) ) );
				}
			}
		}
	}
	else
	{
		if (laserscannerTypeRear == S300)
		{
			/// \todo add S300 stuff
		}
	}
}


float LaserThread::getValue(short int laserScanner, int angle)
{
	switch(laserScanner)
	{
		case LASER1:
			if ( (angle < 0) || ((angle >= (laserscannerAngleFront/laserscannerResolutionFront) ) ) )
			{
				qDebug("Laser1 angle %d out of allowed range 0-%.1f (LaserThread::getValue).", angle, (laserscannerAngleFront/laserscannerResolutionFront));
				return 0;
			}
			return laserScannerValuesFront[angle];
			break;
		case LASER2:
			if ( (angle < 0) || ((angle >= (laserscannerAngleRear/laserscannerResolutionRear) ) ) )
			{
				qDebug("Laser1 angle %d out of allowed range 0-%.1f (LaserThread::getValue).", angle, (laserscannerAngleRear/laserscannerResolutionRear));
				return 0;
			}
			return laserScannerValuesRear[angle];
			break;
	}

	return 0;
}


int LaserThread::getFlag(short int laserScanner, int angle)
{
	switch(laserScanner)
	{
		case LASER1:
			if ( (angle < 0) || ((angle >= (laserscannerAngleFront/laserscannerResolutionFront) ) ) )
			{
//				qDebug("Laser1 angle %d out of allowed range 0-%.1f (LaserThread::getFlag).", angle, (laserscannerAngleFront/laserscannerResolutionFront));
				return 0;
			}
			return laserScannerFlagsFront[angle];
			break;
		case LASER2:
			if ( (angle < 0) || ((angle >= (laserscannerAngleRear/laserscannerResolutionRear) ) ) )
			{
//				qDebug("Laser2 angle %d out of allowed range 0-%.1f (LaserThread::getFlag).", angle, (laserscannerAngleRear/laserscannerResolutionRear));
				return 0;
			}
			return laserScannerFlagsRear[angle];
			break;
	}

	return 0;
}


void LaserThread::setFlag(short int laserScanner, int angle, int flag)
{
	switch(laserScanner)
	{
		case LASER1:
			if ( (angle < 0) || ((angle >= (laserscannerAngleFront/laserscannerResolutionFront) ) ) )
			{
//				qDebug("Laser1 angle %d out of allowed range 0-%.1f (LaserThread::setFlag).", angle, (laserscannerAngleFront/laserscannerResolutionFront));
				break;
			}
			laserScannerFlagsFront[angle] = flag;
			break;
		case LASER2:
			if ( (angle < 0) || ((angle >= (laserscannerAngleRear/laserscannerResolutionRear) ) ) )
			{
//				qDebug("Laser2 angle %d out of allowed range 0-%.1f (LaserThread::setFlag).", angle, (laserscannerAngleRear/laserscannerResolutionRear));
				break;
			}
			laserScannerFlagsRear[angle] = flag;
			break;
	}

	return;
}


void LaserThread::setSimulationMode(bool status)
{
	// check if all necessary values have been initialised
	if (laserScannerFrontIsConnected)
	{
		if (laserscannerAngleFront == -1)
		{
			qDebug("Front laser angle not set before calling LaserThread::setSimulationMode()!");
			return;
		}
		if (laserscannerResolutionFront == -1)
		{
			qDebug("Front laser resolution not set before calling LaserThread::setSimulationMode()!");
			return;
		}
	}

	if (laserScannerRearIsConnected)
	{
		if (laserscannerAngleRear == -1)
		{
			qDebug("Rear laser angle not set before calling LaserThread::setSimulationMode()!");
			return;
		}
		if (laserscannerResolutionRear == -1)
		{
			qDebug("Rear laser resolution not set before calling LaserThread::setSimulationMode()!");
			return;
		}
	}

	// store the sim state
	simulationMode = status;


	if (simulationMode == true)
	{
		// sim value init
		if (setSimulationValues() == false)
		{
			simulationMode = false;
			return;
		}

		numReadingsFront = laserscannerAngleFront;
		numReadingsRear = laserscannerAngleRear;

		// copy sim values into the distances values
		for (int i=0; i<laserScannerValuesFront.count(); i++)
		{
			// the distances
			// laserScannerValuesFront[i] = 2.30; //(i+1) / 100;
			laserScannerValuesFront[i] = simulationValuesFront[i];
			// the flags
			laserScannerFlagsFront[i] = OBSTACLE;
		}

		for (int i=0; i<laserScannerValuesRear.count(); i++)
		{
			// the distances
			// laserScannerValuesRear[i] = 2.30; //(i+1) / 100;
			laserScannerValuesRear[i] = simulationValuesRear[i];
			// the flags
			laserScannerFlagsRear[i] = OBSTACLE;
		}

		// for refreshing the gui (deleting simulated laser lines)
		emit laserDataCompleteFront(laserScannerValuesFront, laserScannerFlagsFront);
		emit laserDataCompleteRear(laserScannerValuesRear, laserScannerFlagsRear);
	}
	else
	{
		// initialisation
		for (int i=0; i<laserScannerValuesFront.count(); i++)
		{
			// the distances
			laserScannerValuesFront[i] = 0;
			// the flags
			laserScannerFlagsFront[i] = OBSTACLE;
		}

		for (int i=0; i<laserScannerValuesRear.count(); i++)
		{
			// the distances
			laserScannerValuesRear[i] = 0;
			// the flags
			laserScannerFlagsRear[i] = OBSTACLE;
		}

		// for refreshing the gui (deleting simulated laser lines)
		emit laserDataCompleteFront(laserScannerValuesFront, laserScannerFlagsFront);
		emit laserDataCompleteRear(laserScannerValuesRear, laserScannerFlagsRear);
	}
}


void LaserThread::setSerialPort(short int laserScanner, QString serialPort)
{
	switch (laserScanner)
	{
	case LASER1:
		if (laserscannerTypeFront == NONE)
		{
			return;
		}

		if (laserscannerTypeFront == PLS)
		{
			// for laser.cpp:
			laser->setDevicePort(laserScanner, serialPort);
			return;
		}

		if (laserscannerTypeFront == S300)
		{
			laserS300->setDevicePort(serialPort);
			return;
		}

		qDebug("Laser1 type %d not supported (LaserThreadd::setSerialPort", laserscannerTypeFront);
		return;
		break;
	case LASER2:
		if (laserscannerTypeFront == NONE)
		{
			return;
		}

		if (laserscannerTypeRear == PLS)
		{
			// for laser.cpp:
			laser->setDevicePort(laserScanner, serialPort);
			return;
		}

		if (laserscannerTypeFront == S300)
		{
			/// \todo support two S300 lasers
			qDebug("Laser2 for S300 not yet supported (LaserThreadd::setSerialPort");
			return;
		}

		qDebug("Laser2 type %d not supported (LaserThreadd::setSerialPort", laserscannerTypeRear);
		return;
		break;
	default:
		qDebug("Laser number %d not yet supported (LaserThreadd::setSerialPort", laserScanner);
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

			// let the splash screen and the GUI / log file from the direcs class show laser init messages
			// (connect the signal from the laser class to the signal from this class)
			connect(laserS300, SIGNAL(message(QString)), this, SIGNAL(message(QString)));

			// forward system error signals from the S300 by this class signal
			connect(laserS300, SIGNAL(systemerror(int)), this, SIGNAL(systemerror(int)));

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
				break;
			case LASER2:
				// store value in the class member
				laserscannerAngleRear = angle;
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
			break;
		case LASER2:
			// store value in the class member
			laserscannerResolutionRear = resolution;
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
		// check if all necessary values have been initialised
		if (laserscannerAngleFront == -1)
		{
			qDebug("Front laser angle not set before calling LaserThread::isConnecteed()!");
			return false;
		}
		if (laserscannerResolutionFront == -1)
		{
			qDebug("Front laser resolution not set before calling LaserThread::isConnecteed()!");
			return false;
		}

		// initialisation of the laser values list (QList). Create one object with length 0.0 for every laser line
		for (int i=0; i<(laserscannerAngleFront/laserscannerResolutionFront); i++)
		{
			laserScannerValuesFront.append(0.0);
		}

		// add a 0 flag for each line
		for (int i=0; i<(laserscannerAngleFront/laserscannerResolutionFront); i++)
		{
			laserScannerFlagsFront.append(0);
		}

		// initialisation of the laser simulation values list (QList)
		for (int i=0; i<(laserscannerAngleFront/laserscannerResolutionFront); i++)
		{
			simulationValuesFront.append(0.0);
		}

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
				/// \todo Support two S300 scanners
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
		// check if all necessary values have been initialised
		if (laserscannerAngleRear == -1)
		{
			qDebug("Rear laser angle not set before calling LaserThread::isConnecteed()!");
			return false;
		}
		if (laserscannerResolutionRear == -1)
		{
			qDebug("Rear laser resolution not set before calling LaserThread::isConnecteed()!");
			return false;
		}

		// initialisation of the laser values list (QList). Create one object with length 0.0 for every laser line
		for (int i=0; i<(laserscannerAngleRear/laserscannerResolutionRear); i++)
		{
			laserScannerValuesRear.append(0.0);
		}

		// add a 0 flag for each line
		for (int i=0; i<(laserscannerAngleRear/laserscannerResolutionRear); i++)
		{
			laserScannerFlagsRear.append(0);
		}

		// initialisation of the laser simulation values list (QList)
		for (int i=0; i<(laserscannerAngleRear/laserscannerResolutionRear); i++)
		{
			simulationValuesRear.append(0.0);
		}

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
				/// \todo S300 stuff
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


bool LaserThread::setSimulationValues()
{
	float floatValue = -1.0;


	// read the FRONT laser sim values from file
	for (int i=0; i<(laserscannerAngleFront/laserscannerResolutionFront); i++)
	{
		// read value from ini file
		floatValue = inifile1->readFloat("Frontlaser", QString("%1").arg(i) );

		if (floatValue == -1)
		{
			simulationValuesFront[i] = 0.0;
			qDebug("Simulation value \"%d\" not found in sim file at [Frontlaser]! LaserThread::setSimulationValues", i);
			qDebug("Stopping to read!");
			return false;
		}
		else
		{
			// store the value
			simulationValuesFront[i] = floatValue;
		}
	}

	// read the REAR laser sim values from file
	for (int i=0; i<(laserscannerAngleRear/laserscannerResolutionRear); i++)
	{
		// read value from ini file
		floatValue = inifile1->readFloat("Rearlaser", QString("%1").arg(i) );

		if (floatValue == -1)
		{
			simulationValuesRear[i] = 0.0;
			qDebug("Simulation value \"%d\" not found in sim file at [Rearlaser]! LaserThread::setSimulationValues", i);
			qDebug("Stopping to read!");
			return false;
		}
		else
		{
			// store the value
			simulationValuesRear[i] = floatValue;
		}
	}

	return true;
}
