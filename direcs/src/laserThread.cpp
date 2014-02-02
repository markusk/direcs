/*************************************************************************
 *   Copyright (C) Markus Knapp                                          *
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
	measureMode = false;
	numReadingsFront = 0;
	numReadingsRear  = 0;
	laserscannerAngleFront = -1;
	laserscannerAngleRear  = -1;
	laserscannerResolutionFront = -1.0;
	laserscannerResolutionRear  = -1.0;

	// for the simulation values
	inifile1 = new Inifile();
	inifile1->setFilename("direcs.sim");

	// for the simulation values
	// file name will be set during call of saveLaserData
	inifileLaserdata = new Inifile();
}


LaserThread::~LaserThread()
{
	// laserScannerFrontIsConnected is set in the isConnected method!
	if (laserScannerFrontIsConnected || laserScannerRearIsConnected)
	{
		if ((laserscannerTypeFront == S300) || ((laserscannerTypeRear == S300)) )
		{
			// closing com port etc. is done in the laser class destructor
			delete laserS300;
		}

		if ((laserscannerTypeFront == HOKUYO_URG) || ((laserscannerTypeRear == HOKUYO_URG)) )
		{
			// closing com port etc. is done in the laser class destructor
			delete laserHokuyoURGsimple;
		}
	}

	delete inifileLaserdata;
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
	static int errorCounter = 0;


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
			if ( (laserscannerTypeFront==S300) || (laserscannerTypeRear==S300))
			{
				/// \todo add support for 2 lasers...
				if (laserS300->readRequestTelegram() != -1)
				{
					getAndStoreLaserValuesFront();
					emit laserDataCompleteFront(laserScannerValuesFront, laserScannerFlagsFront);

					// resetting error counter one by one (for each good read, we are moving to "0".
					if (errorCounter > 0)
					{
						errorCounter--;
					}
				}
				else
				{
					if (errorCounter >= MAXERRORS)
					{
						// emitting a signal to other modules which don't get the return value but need to know that we have a sensor error here.
						emit systemerror(-1);

						stopped = true;
						emit message(QString("%1 ERRORs reading S300 data. Laser thread stopped!").arg(MAXERRORS));
						errorCounter = 0;
					}
					else
					{
						errorCounter++;
						emit message(QString("%1. error reading S300 laser.").arg(errorCounter));
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
	if (laserscannerTypeFront == S300)
	{
		float currentDistance = 0.0;


		if (mountingLaserscannerFront == "normal")
		{
			// /get the data from 0 to 270 degrees (left to right)
			// since we have a resolution at 0.5 degrees, this is an index for the array with 540 values!
			for (int angleIndex=0; angleIndex<(laserscannerAngleFront/laserscannerResolutionFront); angleIndex++)
			{
				//  get distance
				currentDistance = laserS300->getDistance(angleIndex);

				// are we in a the special measure mode? (e.g. walking around the robot for saving a special distance)
				if (measureMode)
				{
					// only store value from laser if the new value (current distance) is less than the last stored value
					if (currentDistance < laserScannerValuesFront[angleIndex])
					{
						// store the value in an array in this thread
						laserScannerValuesFront[angleIndex] = currentDistance;
					}
				}
				else
				{
					// store the value in an array in this thread
					laserScannerValuesFront[angleIndex] = currentDistance;
				}

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
			// 'flip' will be increased every step - 1, so the data are stored from 270 deg to 0 deg
			for (int angleIndex=0, flip=(laserscannerAngleFront/laserscannerResolutionFront)-1; angleIndex<(laserscannerAngleFront/laserscannerResolutionFront); angleIndex++, flip--)
			{
				//  get distance
				currentDistance = laserS300->getDistance(angleIndex);

				// are we in a the special measure mode? (e.g. walking around the robot for saving a special distance)
				if (measureMode)
				{
					// only store value from laser if the new value (current distance) is less than the last stored value
					if (currentDistance < laserScannerValuesFront[angleIndex])
					{
						// store the value in an array in this thread
						laserScannerValuesFront[flip] = currentDistance;
					}
				}
				else
				{
					// store the value in an array in this thread
					laserScannerValuesFront[flip] = currentDistance;
				}

				// send value over the network
				// *0l23a42# means LASER1 has at angle 23 a length of 42 cm
				emit sendNetworkString( QString("*%1l%2a%3#").arg(LASER1).arg(angleIndex).arg( (int) (laserScannerValuesFront[angleIndex]*100) ) );
			}
		}
	}
}


void LaserThread::getAndStoreLaserValuesRear()
{
	if (laserscannerTypeRear == S300)
	{
		/// \todo add S300 stuff
	}
}


float LaserThread::getValue(short int laserScanner, int angle)
{
	switch(laserScanner)
	{
		case LASER1:
			if ( (angle < 0) || ((angle >= (laserscannerAngleFront/laserscannerResolutionFront) ) ) )
			{
				emit message(QString("ERROR: Laser1 angle %1 out of allowed range 0-%2 (LaserThread::getValue).").arg(angle).arg(laserscannerAngleFront/laserscannerResolutionFront));
				return 0;
			}
			return laserScannerValuesFront[angle];
			break;
		case LASER2:
			if ( (angle < 0) || ((angle >= (laserscannerAngleRear/laserscannerResolutionRear) ) ) )
			{
				emit message(QString("ERROR: Laser1 angle %1 out of allowed range 0-%2 (LaserThread::getValue).").arg(angle).arg(laserscannerAngleRear/laserscannerResolutionRear));
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
//				emit message("Laser1 angle %d out of allowed range 0-%.1f (LaserThread::getFlag).", angle, (laserscannerAngleFront/laserscannerResolutionFront));
				return 0;
			}
			return laserScannerFlagsFront[angle];
			break;
		case LASER2:
			if ( (angle < 0) || ((angle >= (laserscannerAngleRear/laserscannerResolutionRear) ) ) )
			{
//				emit message("Laser2 angle %d out of allowed range 0-%.1f (LaserThread::getFlag).", angle, (laserscannerAngleRear/laserscannerResolutionRear));
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
//				emit message("Laser1 angle %d out of allowed range 0-%.1f (LaserThread::setFlag).", angle, (laserscannerAngleFront/laserscannerResolutionFront));
				break;
			}
			laserScannerFlagsFront[angle] = flag;
			break;
		case LASER2:
			if ( (angle < 0) || ((angle >= (laserscannerAngleRear/laserscannerResolutionRear) ) ) )
			{
//				emit message("Laser2 angle %d out of allowed range 0-%.1f (LaserThread::setFlag).", angle, (laserscannerAngleRear/laserscannerResolutionRear));
				break;
			}
			laserScannerFlagsRear[angle] = flag;
			break;
	}

	return;
}


void LaserThread::setSimulationMode(bool state)
{
	// check if all necessary values have been initialised
	if (laserScannerFrontIsConnected)
	{
		if (laserscannerAngleFront == -1)
		{
			emit message("ERROR: Front laser angle not set before calling LaserThread::setSimulationMode()!");
			return;
		}
		if (laserscannerResolutionFront == -1)
		{
			emit message("ERROR: Front laser resolution not set before calling LaserThread::setSimulationMode()!");
			return;
		}
	}

	if (laserScannerRearIsConnected)
	{
		if (laserscannerAngleRear == -1)
		{
			emit message("ERROR: Rear laser angle not set before calling LaserThread::setSimulationMode()!");
			return;
		}
		if (laserscannerResolutionRear == -1)
		{
			emit message("ERROR: Rear laser resolution not set before calling LaserThread::setSimulationMode()!");
			return;
		}
	}

	// store the sim state
	simulationMode = state;


	if (simulationMode == true)
	{
		// sim value init
		if (readSimulationValues() == false)
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


void LaserThread::setMeasureMode(bool state)
{
	// store that we are in measure state now (or not)
	measureMode = state;
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

		if (laserscannerTypeFront == S300)
		{
			laserS300->setDevicePort(serialPort);
			return;
		}

		if (laserscannerTypeFront == HOKUYO_URG)
		{
			laserHokuyoURGsimple->setDevicePort(serialPort);
			return;
		}

		emit message(QString("ERROR: Laser1 type %1 not supported (LaserThread::setSerialPort").arg(laserscannerTypeFront));
		return;
		break;
	case LASER2:
		if (laserscannerTypeFront == NONE)
		{
			return;
		}

		if (laserscannerTypeFront == S300)
		{
			/// \todo support two S300 lasers
			emit message("ERROR: Laser2 for S300 not yet supported (LaserThread::setSerialPort)");
			return;
		}

		if (laserscannerTypeFront == HOKUYO_URG)
		{
			/// \todo support two HOKUYO lasers
			emit message("ERROR: Laser2 for HOKUYO not yet supported (LaserThread::setSerialPort)");
			return;
		}


		emit message(QString("ERROR: Laser1 type %1 not supported (LaserThread::setSerialPort").arg(laserscannerTypeRear));
		return;
		break;
	default:
		emit message(QString("ERROR: Laser1 number %1 not supported (LaserThread::setSerialPort").arg(laserScanner));
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
				emit message("ERROR: Laser number not yet supported (LaserThread::setMounting");
				break;
		}
}


void LaserThread::setType(short int laserScanner, QString laserType)
{
		switch (laserScanner)
		{
			case LASER1:
				if (laserType=="S300")
				{
					laserscannerTypeFront = S300;
				}
				else
				{
					if (laserType=="HokuyoURG")
					{
						laserscannerTypeFront = HOKUYO_URG;
					}
					else
					{
						if (laserType=="none")
						{
							laserscannerTypeFront = NONE;
						}
						else
						{
							emit message("ERROR: Laser type not yet supported  (LaserThread::setLaserscannerType, LASER1");
						}
					}
				}
				break;
		case LASER2:
			if (laserType=="S300")
			{
				laserscannerTypeRear = S300;
			}
			else
			{
				if (laserType=="HokuyoURG")
				{
					laserscannerTypeFront = HOKUYO_URG;
				}
				else
				{
					if (laserType=="none")
					{
						laserscannerTypeRear = NONE;
					}
					else
					{
						emit message("ERROR: Laser type not yet supported  (LaserThread::setLaserscannerType, LASER2");
					}
				}
			}
			break;
		default:
			emit message("ERROR: Laser number not yet supported  (LaserThread::setLaserscannerType");
			break;
		}


		// create the laser object
		// S300 stuff
		if  (laserType=="S300")
		{
			laserS300 = new SickS300();

			// let the splash screen and the GUI / log file from the direcs class show laser init messages
			// (connect the signal from the laser class to the signal from this class)
			connect(laserS300, SIGNAL(message(QString)), this, SIGNAL(message(QString)));

			return;
		}


		// create the laser object
		// Hokuyo URG simple stuff
		if  (laserType=="HokuyoURG")
		{
			laserHokuyoURGsimple = new HokuyoURGsimple();

			// let the splash screen and the GUI / log file from the direcs class show laser init messages
			// (connect the signal from the laser class to the signal from this class)
			connect(laserHokuyoURGsimple, SIGNAL(message(QString)), this, SIGNAL(message(QString)));

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
				emit message("ERROR: Laser number not yet supported  (LaserThread::setLaserscannerAngle");
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
			emit message("ERROR: Laser number not yet supported  (LaserThread::setLaserscannerResolution");
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

		emit message("ERROR: Laser number not yet supported  (LaserThread::getAngle");
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

		emit message("ERROR: Laser number not yet supported  (LaserThread::getResolution");
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
			emit message("ERROR: Front laser angle not set before calling LaserThread::isConnecteed()!");
			return false;
		}
		if (laserscannerResolutionFront == -1)
		{
			emit message("ERROR: Front laser resolution not set before calling LaserThread::isConnecteed()!");
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
			if (laserscannerTypeFront == HOKUYO_URG)
			{

// TEST TEST TEST
laserHokuyoURGsimple->findPort();

				/// \todo Support two Hokuyo scanners
				if (laserHokuyoURGsimple->openComPort() == true)
				{
					if (laserHokuyoURGsimple->readRequestTelegram() == 0)
					{
						qDebug("URG OKAY");
						return true;
					}
				}

				qDebug("URG not OKAY");
				laserScannerFrontIsConnected = false;
				return false;
			}

			if (laserscannerTypeFront != NONE)
			{
				emit message("ERROR: Unsupported laser type! (LaserThread::isConnected)");
			}
			laserScannerFrontIsConnected = false;
			return false;
		}
		break;
	case LASER2:
		// check if all necessary values have been initialised
		if (laserscannerAngleRear == -1)
		{
			emit message("ERROR: Rear laser angle not set before calling LaserThread::isConnecteed()!");
			return false;
		}
		if (laserscannerResolutionRear == -1)
		{
			emit message("ERROR: Rear laser resolution not set before calling LaserThread::isConnecteed()!");
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

		if (laserscannerTypeRear == S300)
		{
			/// \todo S300 stuff
			emit message("INFO: Support for a second S300 as rear laser scanner not implemented yet");
			laserScannerRearIsConnected = false;
			return false;
		}
		else
		{
			if (laserscannerTypeRear == HOKUYO_URG)
			{
				/// \todo Hokuyo stuff
				emit message("INFO: Support for a second Hokuyo as rear laser scanner not implemented yet");
				laserScannerRearIsConnected = false;
				return false;
			}

			if (laserscannerTypeRear != NONE)
			{
				emit message("ERROR: Unsupported laser type! (LaserThread::isConnected)");
			}
			laserScannerRearIsConnected = false;
			return false;
		}
		break;
	default:
		emit message("ERROR: Laser number not yet supported (LaserThread::setMounting");
		break;
	}


	stopped = true; // don't run the thread!
	return false;
}


bool LaserThread::readSimulationValues()
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
			emit message(QString("ERROR: Simulation value \"%1\" not found in sim file at [Frontlaser]! LaserThread::setSimulationValues").arg(i));
			emit message("Stopping to read!");
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
			emit message(QString("ERROR: Simulation value \"%1\" not found in sim file at [Rearlaser]! LaserThread::setSimulationValues").arg(i));
			emit message("Stopping to read!");
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


void LaserThread::saveLaserData()
{
	QDateTime now; // this is for the timestamp in the log
	QString filename;
	QString stringName;
	QString stringValue;


	// get current date and time
	now = QDateTime::currentDateTime(); // get the current date and time

	// set filenam
	filename = QString("direcs__%1-%2-%3__%4-%5-%6.sim").arg(now.toString("yyyy")).arg(now.toString("MM")).arg(now.toString("dd")).arg(now.toString("hh")).arg(now.toString("mm")).arg(now.toString("ss"));
	inifileLaserdata->setFilename(filename);

	/// @todo Stop laser thread here for saving the values or is a mutex lock for the QList okay or are QLists able to handle this?

	// write the FRONT laser sim values to file
	for (int i=0; i<(laserscannerAngleFront/laserscannerResolutionFront); i++)
	{
		// convert i to string
		stringName = QString("%1").arg(i);

		// convert laser float values to string
		// we are using a string instead of float here, since Qt write float in a quite non human readable format like 'settingt=@Variant(\0\0\0\x87@\xa0\0\0)'.
		stringValue = QString("%1").arg( laserScannerValuesFront.at(i) );

		// write value to ini file
		inifileLaserdata->writeSetting("Frontlaser", stringName, stringValue);
	}

	// write the REAR laser sim values to file
	for (int i=0; i<(laserscannerAngleRear/laserscannerResolutionRear); i++)
	{
		// convert i to string
		stringName = QString("%1").arg(i);

		// convert laser float values to string
		// we are using a string instead of float here, since Qt write float in a quite non human readable format like 'settingt=@Variant(\0\0\0\x87@\xa0\0\0)'.
		stringValue = QString("%1").arg( laserScannerValuesRear.at(i) );

		// write value to ini file
		inifileLaserdata->writeSetting("Rearlaser", stringName, stringValue);
	}


	emit message(QString("Laser data written to <b>%1</b>").arg(filename));
}
