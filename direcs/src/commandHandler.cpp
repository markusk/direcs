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

	commandSentSuccessfull = false;

	commandInProgress = false;

	// start with command (process) ID no. 0
	currentID = 0;

	// send answers from interfaceAvr to this class
	connect(interface1, SIGNAL(commandCompleted(QString, QString)), this, SLOT(takeCommandAnswer(QString, QString)));
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
	command tempCommand;
	answer  tempAnswer;
	QString commandToBeSent;
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
			while (commandList.isEmpty())
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
			tempCommand = commandList.first();

			// store command in this class
			tempAnswer.string = tempCommand.string;

			// set expected answer string from command
			tempAnswer.string = tempCommand.string;

			// store command in this class
			commandToBeSent = tempCommand.string;

			// get command ID and use this ID in answer list
			tempAnswer.ID = tempCommand.ID;

			// generate timestamp ("now")
			tempAnswer.timestamp = QDateTime::currentDateTime();

			// add to answer list
			answerList.append(tempAnswer);


			// debug msg
			if (currentID > 0)
			{
				emit message( QString("command ID=%1 string=%2 time=%3").arg( tempAnswer.ID ).arg( tempAnswer.string ).arg( tempAnswer.timestamp.toString("hh:mm:ss.zzz") ));
			}

			// remove from "to do" list
			commandList.removeFirst();

			// unlock mutex
			commandListMutex.unlock();


			//-----------------------
			// send command to Atmel
			//-----------------------
			emit message(QString("SENDING COMMAND %1").arg(commandToBeSent));

			// this command is not executed yet
			commandSentSuccessfull = false;

			// Lock the mutex.
			mutex->lock();

			if (interface1->sendString(commandToBeSent) == true)
			{
				// start own time measuring. This will be used, if we get an answer from the Atmel
				duration.start();

				// start additional seperate timer. If we NEVER get an answer, this slot will be called
				QTimer::singleShot(ATMELTIMEOUT, this, SLOT(timeout()) );

				emit message("Sent.");
			}
			else
			{
				emit message("ERROR sending command (commandHandler::run")	;
			}

			// Unlock the mutex.
			mutex->unlock();



		} // robot is on  and  simulation mode is off

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


void CommandHandler::takeCommand(QString commandString)
{
	command tempCommand;


	// lock mutex
	commandListMutex.lock();

	// fill data structure
	tempCommand.string = commandString;
	tempCommand.ID = currentID;

	// add command and ID to command lists
	commandList.append(tempCommand);

	// debug msg
	// emit message( QString("command ID %1, %2 appended").arg(tempCommand.string).arg(tempCommand.ID) );

	// create next command ID
	currentID++;

	// unlock mutex
	commandListMutex.unlock();
}


void CommandHandler::takeCommandAnswer(QString atmelAnswer, QString correspondingCommand)
{
	answer tempAnswer;


	// we have an answer, so we can tell the run-loop, that it can continue
	commandInProgress = false;


	// see if executed answer is in list of expected answers
	for (int i=0; i<answerList.size(); i++)
	{
		// at can be faster than []
		tempAnswer = answerList.at(i);

		// see if the Atmel answer starts with the starter and the sent command (i.e. *s7... )
		if (atmelAnswer.startsWith( QString("%1%2").arg(starter).arg(tempAnswer.string) ))
		{
			//--------------
			// answer found
			//--------------
			emit message("Expected answer received");

			//----------
			// timeout?
			//----------
			QDateTime now = QDateTime::currentDateTime();

			if (tempAnswer.timestamp.msecsTo(now) > ATMELTIMEOUT)
			{
				emit message(QString("Timeout (%1ms > %2ms)").arg( tempAnswer.timestamp.msecsTo(now) ).arg( ATMELTIMEOUT ));

				// timeout
				// let this class know, that we had an error
				robotState = false;
				commandSentSuccessfull = false;

				emit heartbeat(RED);
				emit message("<font color=\"#FF0000\">ERROR executing command. Stopping commandHandler!</font>");
				// stop this thread
				stop();

		/// @todo is systemerror emission still needed?!?  s.a. other error situations within this class!
				// inform other modules
		//	    emit systemerror(-2);

				return;
			}


			return;
		}
	}


	//------------------
	// answer not found
	//------------------
	emit message(QString("<font color=\"#FF0000\">ERROR! Answer %1 not expected. Stopping commandHandler!</font>").arg(atmelAnswer));

	// let this class know, that we had an error
	robotState = false;
	commandSentSuccessfull = false;

	emit heartbeat(RED);
	// stop this thread
	stop();
	return;



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
	// first check if we had already an answer from the Atmel
	if (commandSentSuccessfull == true)
	{
		// reset state
	//	commandExecutedSuccessfull = false;

		// we are happy
		return;
	}

	emit message(QString("Timeout (> %2ms)").arg(ATMELTIMEOUT));

	// let this class know, that we had an error
	robotState = OFF;

	emit heartbeat(RED);

	emit message("<font color=\"#FF0000\">ERROR reading sensor. Stopping sensorThread!</font>");
	// stop this thread
	stop();

	return;
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
