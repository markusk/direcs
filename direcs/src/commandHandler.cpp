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

#include "commandHandler.h"

CommandHandler::CommandHandler(InterfaceAvr *i, QMutex *m)
{
	stopped = false;
	simulationMode = false;

	// copy the pointer from the original object
	interface1 = i;
	mutex = m;



	robotState = ON; // Wer're thinking positive. The robot is ON untill whe know nothing other. :-)

	commandExecutedSuccessfull = false;

	commandInProgress = false;

	commandInQueue = false;

	// start with command (process) ID no. 0
	currentID = 0;

	// send answers from interfaceAvr to this class
//	connect(interface1, SIGNAL(commandCompleted(QString, QString)), this, SLOT(takeCommandAnswer(QString, QString)));
}


CommandHandler::~CommandHandler()
{
	// stop all activities first
	stop();
}


void CommandHandler::stop()
{
	stopped = true;

	// send "off" or "shutdown" heartbeat signal over network to remote app
	emit heartbeat(RED);

	// send heartbeat over the network
	// *0h2# means 'heartbeat no. 0 is DEAD'
	emit sendNetworkString("*0h2#");
}


void CommandHandler::run()
{
	bool heartbeatToggle = false;


	//  start "threading"...
	while (!stopped)
	{
		// let the thread sleep some time for having more time for the other threads
		msleep(THREADSLEEPTIME);

		if ( (robotState == ON) && (simulationMode == false) )
		{
			// wait for finished command execution (which is currently in progress)
			while (commandInProgress)
			{
				// see if we nead to break out
				if (stopped)
				{
					return;
				}

				// let the thread sleep some time
				msleep(50);
			}


			// wait for an command in the list to be executed
			while (!commandInQueue)
			{
				// see if we nead to break out
				if (stopped)
				{
					return;
				}

				// let the thread sleep some time
				msleep(50);
			}


			// lock mutex
			commandListMutex.lock();

			// get next command from list (oldest first)
			QString commandToBeExecuted = commandStrings.first();

			// add command ID to list
			answerIDs.append(currentID);

			// add expected answer to list
			answerStrings.append(commandToBeExecuted);

			// add timestamp to list
			answerTimestamps.append(QDateTime::currentDateTime());

			// debug msg
			if (currentID > 0)
			{
// err				emit message( QString("command ID=%1 string=%2 time=%3 time-dif=%4ms").arg(commandIDs.first()).arg(commandStrings.first()).arg(answerTimestamps.first().toString("hh:mm:ss.zzz")).arg(answerTimestamps.first().msecsTo( QDateTime::currentDateTime() )) );
				emit message( QString("command ID=%1 string=%2 time=%3").arg(commandIDs.first()).arg(commandStrings.first()).arg(answerTimestamps.first().toString("hh:mm:ss.zzz")) );
			}

			// remove from "to do" list
			commandStrings.removeFirst();

			// remove from "to do" list
			answerTimestamps.removeFirst();

			// remove from "to do" list
			commandIDs.removeFirst();

			// any other comands in list?
			if (commandStrings.isEmpty())
			{
				commandInQueue = false;
			}

			// unlock mutex
			commandListMutex.unlock();


			//------------
			/// @ todo   s e n d   c o m m a n d   h e r e
			//------------
			emit message("TEST COMMAND");

		} // state=ON & simulation=false

		if (simulationMode)
		{
			// now we're sleeping additional 500ms because we're only simulating.
			msleep(500);

			// send an (simulated) optical heartbeat signal to the GUI
			if (!heartbeatToggle)
			{
				emit heartbeat(GREEN);
			}
			else
			{
				emit heartbeat(LEDOFF);
			}
			heartbeatToggle = !heartbeatToggle;
		}

		//  e m i t  Signal
//		emit sensorDataComplete();
	}
	stopped = false;
}


void CommandHandler::takeCommand(QString command)
{
	// lock mutex
	commandListMutex.lock();

	// add command and expected answers to lists
	commandStrings.append(command);

	// add command ID to list
	commandIDs.append(currentID);

	// create next command ID
	currentID++;

	commandInQueue = true;

	// unlock mutex
	commandListMutex.unlock();
}


void CommandHandler::takeCommandAnswer(QString atmelAnswer, QString correspondingCommand)
{
	commandInProgress = false;

	// see if executed answer is in list of expected answers
	for (int i=0; i<answerStrings.size(); i++)
	{
		// at can be faster than []
		if (answerStrings.at(i) == atmelAnswer)
		{
			return;
		}
	}


/*
	int value = 0; // for conversion to int


	if ((correspondingCommand != atmelCommand) || (stopped == true))
	{
//		emit message(QString("Answer %1 is not for me (SensorThread).").arg(atmelAnswer));
		// emit message(QString("correspondingCommand %1 != atmelCommand %2 (SensorThread).").arg(correspondingCommand).arg(atmelCommand));
		return;
	}

	//----------
	// timeout?
	//----------
	if (duration.elapsed() > ATMELTIMEOUT)
	{
		emit message(QString("Timeout (%1ms > %2ms)").arg(duration.elapsed()).arg(ATMELTIMEOUT));

		// timeout
		// let this class know, that we had an error
		robotState = false;
		commandExecutedSuccessfull = false;

		varMutex.lock();
		atmelCommand = "none"; // reset current command
		varMutex.unlock();

		emit heartbeat(RED);
		emit message("<font color=\"#FF0000\">ERROR reading sensor. Stopping sensorThread!</font>");
		// stop this thread
		stop();

/// @todo is systemerror emission still needed?!?  s.a. other error situations within this class!
		// inform other modules
//	    emit systemerror(-2);

		return;
	}

	//------------------
	// everthing's fine
	//------------------
	/// @todo check if we have numbers between the * and #
	if (atmelAnswer.startsWith("*") && atmelAnswer.endsWith("#")) /// This is different to @sa Circuit and @sa Motor. Since we get a value like *42, we only check the string.
	{
//		emit message(QString("Answer %1 was correct (SensorThread).").arg(atmelAnswer));

		// convert answer to int
		if (interface1->convertStringToInt(atmelAnswer, value) == false)
		{
			// error
			emit message("ERROR converting sensor value.");

			// do not return, continue here! In both cases, we store the value! In case of error, value is 0.
		}

		// check the last command
		if (atmelCommand == commandReadVoltageSensor1)
		{
			// store measured value
			voltageSensorValue[VOLTAGESENSOR1] = value;

//			emit message(QString("VOLTAGESENSOR1 = %1 = %2 Volt").arg(correspondingCommand).arg(convertToVolt(VOLTAGESENSOR1)));

			// send value over the network
			// *0v42# means voltagesensor1 with 42 V (the digits after the decimal points are ignored here!)
			emit sendNetworkString( QString("*%1v%2#").arg(VOLTAGESENSOR1).arg( (int) voltageSensorValue[VOLTAGESENSOR1]));

			varMutex.lock();
			atmelCommand = "none"; // reset current command
			varMutex.unlock();

			commandExecutedSuccessfull = true;

			/// @todo maybe set a seperate "go on with run thread" here?

			return;
		}

		if (atmelCommand == commandReadVoltageSensor2)
		{
			// store measured value
			voltageSensorValue[VOLTAGESENSOR2] = value;

//			emit message(QString("VOLTAGESENSOR2 = %1 = %2 Volt").arg(correspondingCommand).arg(convertToVolt(VOLTAGESENSOR2)));

			// send value over the network
			// *0v42# means voltagesensor1 with 42 V (the digits after the decimal points are ignored here!)
			emit sendNetworkString( QString("*%1v%2#").arg(VOLTAGESENSOR2).arg( (int) voltageSensorValue[VOLTAGESENSOR2]));

			varMutex.lock();
			atmelCommand = "none"; // reset current command
			varMutex.unlock();

			/// @todo maybe set a seperate "go on with run thread" here?

			commandExecutedSuccessfull = true;

			return;
		}

		varMutex.lock();
		atmelCommand = "none"; // reset current command
		emit message("+++ whats goig on here=?"); /// @todo call stop() here ?!? when does this case occur?
		return;
	}
	else
	{
		//--------------
		// wrong answer
		//--------------
		emit message(QString("ERROR: Answer was %1 intead of *nnn#.").arg(atmelAnswer)); /// This is different to @sa Circuit and @sa Motor. Since we get a value like *42, we only check the string.

		// let this class know, that we had an error
		robotState = false;
		commandExecutedSuccessfull = false;

		varMutex.lock();
		atmelCommand = "none"; // reset current command
		varMutex.unlock();

		emit heartbeat(RED);
		emit message("<font color=\"#FF0000\">ERROR reading sensor. Stopping sensorThread!</font>");
		// stop this thread
		stop();
		return;
	}
	*/
}


void CommandHandler::timeout()
{
	/*
	// first check if we had already an answer from the Atmel
	if (commandExecutedSuccessfull == true)
	{
		// reset state
	//	commandExecutedSuccessfull = false;

		// we are happy
		varMutex.lock();
		atmelCommand = "none"; // reset current command
		varMutex.unlock();
/// @todo maybe set a seperate "go on with run thread" here?
		return;
	}

	emit message(QString("Timeout (> %2ms)").arg(ATMELTIMEOUT));
	varMutex.lock();
	atmelCommand = "none"; // reset current command
	varMutex.unlock();

	// let this class know, that we had an error
	robotState = OFF;

	emit heartbeat(RED);

	emit message("<font color=\"#FF0000\">ERROR reading sensor. Stopping sensorThread!</font>");
	// stop this thread
	stop();

	return;
	*/
}


void CommandHandler::setSimulationMode(bool state)
{
	simulationMode = state;

	// fill array with some nice values
	if (simulationMode == true)
	{
//		// copy sim values into the distances values
//		for (int i=0; i<laserScannerValuesFront.count(); i++)
//		{
//			// the distances
//			// laserScannerValuesFront[i] = 2.30; //(i+1) / 100;
//			laserScannerValuesFront[i] = simulationValuesFront[i];
//			// the flags
//			laserScannerFlagsFront[i] = OBSTACLE;
//		}
	}
	else
	{
		if (robotState==OFF)
			emit heartbeat(RED);
	}
}


void CommandHandler::setRobotState(bool state)
{
	// store the state within this class
	robotState = state;
}
