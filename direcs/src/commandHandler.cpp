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
	className = this->staticMetaObject.className();

	stopped = false;
	simulationMode = false;

	// copy the pointer from the original object
	interface1 = i;
	mutex = m;

	interfaceState = ON; // Wer're thinking positive. The robot is ON untill whe know nothing other. :-)

	commandSentSuccessfull = false;

	commandInProgress = false;

	// start with command (process) ID no. 0
	currentID = 0;

	// send answers from interfaceAvr to this class
	connect(interface1, SIGNAL(commandCompleted(QString, QString)), this, SLOT(takeCommandAnswer(QString, QString)));

	// timer which ckecks if we have a general timout when waiting for a command answer
//	timeoutTimer = new QTimer();
//	connect(timeoutTimer, SIGNAL(timeout()), this, SLOT(generalTimeout()));
}


CommandHandler::~CommandHandler()
{
	// stop all activities first
	stop();

//	delete timeoutTimer;
}


void CommandHandler::stop()
{
	// stop general timeout timer since we got an answer
//	timeoutTimer->stop();

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

		if ( (interfaceState == ON) && (simulationMode == false) )
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

			// add name of the calling class
			tempAnswer.caller = tempCommand.caller;

			// generate timestamp ("now")
			tempAnswer.timestamp = QDateTime::currentDateTime();

			// add to answer list
			answerList.append(tempAnswer);

			/*
			// debug msg
			if (currentID > 0)
			{
				emit message( QString("command ID=%1 string=%2 caller=%3 time=%4").arg( tempAnswer.ID ).arg( tempAnswer.string ).arg(tempAnswer.caller).arg( tempAnswer.timestamp.toString("hh:mm:ss.zzz") ));
			}
			*/

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
				// let this run loop wait before sending the next command to the Atmel
				commandInProgress = true;

				// start own time measuring. This will be used, if we get an answer from the Atmel
				duration.currentDateTime();
/*
				// start additional seperate timer. If we NEVER get an answer, this slot will be called
				// a singleShot timer did not work here, so we use the class memer...
				if (timeoutTimer->isActive() == false)
					timeoutTimer->start(ATMELTIMEOUT);
*/
				emit message("Sent.");
			}
			else
			{
				emit message(QString("ERROR sending command (%1::run").arg(this->staticMetaObject.className()) ); /// @todo use this nice call in every class for showing which method caused an error!
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
///		emit sensorDataComplete();   @todo this is old unused code from sensorThread. emit something else here?
	}
	stopped = false;
}


void CommandHandler::takeCommand(QString commandString, QString caller)
{
	command tempCommand;
	answer tempAnswer;


	/// @todo check for robotState here, too?

	// if we recieved already commands (and expect answers, so the answer list is not empty)
	if (answerList.isEmpty() == false)
	{
		//------------------
		// general timeout?
		//------------------
		// okay, first le'ts check how much time was gone, since we received the first command
		// if it is too long, than we have a general timeout and will stop all actions!

		// get oldest command element
		// this contains also the start timestamp of this command
		tempAnswer = answerList.first();

		// hey kids, what time is it?
		QDateTime now = QDateTime::currentDateTime();

		if (tempAnswer.timestamp.msecsTo(now) > ATMELTIMEOUT)
		{
			emit message(QString("General timeout error (%1ms > %2ms)").arg( tempAnswer.timestamp.msecsTo(now) ).arg( ATMELTIMEOUT ));

			// timeout
			// let this class know, that we had an error
			setRobotState(OFF);;
			commandSentSuccessfull = false;

			emit heartbeat(RED);
			emit message(QString("<font color=\"#FF0000\">ERROR when waiting executing command. Stopping %1!</font>").arg(className));
			// stop this thread
			stop();

			//-----------------------
			// inform other modules!
			//-----------------------
			emit robotState(false);

			return;
		}
	}


	// lock mutex
	commandListMutex.lock();

	// fill data structure
	tempCommand.string = commandString;
	tempCommand.ID = currentID;
	tempCommand.caller = caller;

	// add command and ID to command lists
	commandList.append(tempCommand);

	// debug msg
	// emit message( QString("command %1, Id=%2 from %3 appended").arg(tempCommand.string).arg(tempCommand.ID).arg(tempCommand.caller) );

	// create next command ID
	currentID++; /// @todo double cross check: do have we have more than x commands in the queued in the line? > emit systemerror than.

	// unlock mutex
	commandListMutex.unlock();
}


void CommandHandler::takeCommandAnswer(QString atmelAnswer, QString correspondingCommand)
{
	answer tempAnswer;

	 QDateTime::currentDateTime();


	// stop general timeout timer since we got an answer
//	timeoutTimer->stop();

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

			// remove element from answer list
			answerList.removeAt(i);

			commandSentSuccessfull = true; // this lets the seperate timeout slot just return

			// debug msg
			// emit message(QString("Expected answer received in %1").arg(className));

			//----------
			// timeout?
			//----------
			QDateTime now = QDateTime::currentDateTime();

			if (tempAnswer.timestamp.msecsTo(now) > ATMELTIMEOUT)
			{
				emit message(QString("Timeout (%1ms > %2ms)").arg( tempAnswer.timestamp.msecsTo(now) ).arg( ATMELTIMEOUT ));

				// timeout
				// let this class know, that we had an error
				setRobotState(OFF);;
				commandSentSuccessfull = false;

				emit heartbeat(RED);
				emit message(QString("<font color=\"#FF0000\">ERROR executing command. Stopping %1!</font>").arg(className));
				// stop this thread
				stop();

		/// @todo is systemerror emission still needed?!?  s.a. other error situations within this class!
				// inform other modules
		//	    emit systemerror(-2);

				return;
			}


			// ------------
			// emit Signal
			// ------------
			// answer with the complete string which the Atmel received and with the name of the calling class which asked for executing this command
			emit commandComplete(atmelAnswer, tempAnswer.caller);

			return;
		}
	}


	//------------------
	// answer not found
	//------------------
	emit message(QString("<font color=\"#FF0000\">ERROR! Answer %1 not expected. Stopping commandHandler!</font>").arg(atmelAnswer));

	// let this class know, that we had an error
	setRobotState(OFF);;
	commandSentSuccessfull = false;

	emit heartbeat(RED);
	// stop this thread
	stop();
	return;



/*
	int value = 0; // for conversion to int



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


		varMutex.lock();
		atmelCommand = "none"; // reset current command
		emit message("+++ whats goig on here=?"); /// @todo call stop() here ?!? when does this case occur?
		return;
	}

 */
}

/*
void CommandHandler::generalTimeout()
{
	// first check if we had already an answer from the Atmel
	if (commandSentSuccessfull == true)
	{
		// we are happy
		return;
	}

	emit message(QString("<font color=\"#FF0000\">ERROR: General timeout (> %1ms). Stopping %2!</font>").arg(ATMELTIMEOUT).arg(className));

	// let this class know, that we had an error
	setRobotState(OFF);;

	emit heartbeat(RED);

	// stop this thread
	stop();
}
*/

void CommandHandler::setSimulationMode(bool state)
{
	simulationMode = state;

	// fill array with some nice values
	if (simulationMode == true)
	{
		/// @todo implement simulation mode for commandHandler
		emit message("ERROR: simulation mode noe yet implemented in CommandHandler class!!");
	}
	else
	{
		if (interfaceState==OFF)
			emit heartbeat(RED);
	}
}


void CommandHandler::setRobotState(bool state)
{
	// store the state within this class
	interfaceState = state;

	emit message(QString("Robot state set to %1 in %2").arg(state).arg(className));
}
