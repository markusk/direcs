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

	threadSleepTime = 0;
	atmelTimeout = 0;

	stopped = false;
	simulationMode = false;

	// copy the pointer from the original object
	interface1 = i;
	interfaceAvrMutex = m;

	interfaceState = ON; // Wer're thinking positive. The robot is ON untill whe know nothing other. :-)

	commandSentSuccessfull = false;

	commandInProgress = false;

	// start with command (process) ID no. 0
	currentID = 0;

	// send answers from interfaceAvr to this class
	connect(interface1, SIGNAL(commandCompleted(QString)), this, SLOT(takeCommandAnswer(QString)));
}


CommandHandler::~CommandHandler()
{
	// stop all activities first
	stop();
}


void CommandHandler::setSleepTime(unsigned long time)
{
	// store time
	threadSleepTime = time;
}


void CommandHandler::setAtmelTimeout(unsigned long time)
{
	// store time
	atmelTimeout = time;
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
	static command tempCommand;
	static answer  tempAnswer;
	static QString commandToBeSent;
	static bool heartbeatToggle = false;


	// check if time set
	if (threadSleepTime == 0)
	{
		emit message(QString("ERROR: Thread sleep time not set in %1!").arg(className));
		stop();
		return;
	}


	//  start "threading"...
	while (!stopped)
	{
		// let the thread sleep some time for having more time for the other threads
		msleep(threadSleepTime);

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
				msleep(threadSleepTime); /// @todo put this sleep time in a var
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
				msleep(threadSleepTime); /// @todo put this sleep time in a var
			}


			// get next command from list (oldest first)
			commandListMutex.lock();
			tempCommand = commandList.first();
			commandListMutex.unlock();

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
			answerListMutex.lock();
			answerList.append(tempAnswer);
			answerListMutex.unlock();

			// debug msg
			// if (currentID > 0)
			// {
			// 	emit message( QString("command ID=%1 string=%2 caller=%3 time=%4").arg( tempAnswer.ID ).arg( tempAnswer.string ).arg(tempAnswer.caller).arg( tempAnswer.timestamp.toString("hh:mm:ss.zzz") ));
			// }

			// remove from "to do" list
			commandListMutex.lock();
			commandList.removeFirst();
			commandListMutex.unlock();


			//-----------------------
			// send command to Atmel
			//-----------------------
			emit message(QString("%1 sends %2 (ID %3)").arg(className).arg(commandToBeSent).arg(currentID));

			// this command is not executed yet
			commandSentSuccessfull = false;

			// Lock the mutex.
			interfaceAvrMutex->lock();

			if (interface1->sendString(commandToBeSent) == true)
			{
				// let this run loop wait before sending the next command to the Atmel
				commandInProgress = true;

				// debug msg
				//emit message("Sent.");
			}
			else
			{
				emit message(QString("ERROR sending command (%1::run").arg(this->staticMetaObject.className()) ); /// @todo use this nice call in every class for showing which method caused an error!
			}

			// Unlock the mutex.
			interfaceAvrMutex->unlock();

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

	}
	stopped = false;
}


void CommandHandler::takeCommand(QString commandString, QString caller)
{
	command tempCommand;
	answer tempAnswer;
	unsigned long duration;


	// check if time set
	if (atmelTimeout == 0)
	{
		emit message(QString("ERROR: Atmel timeout not set in %1!").arg(className));
		stop();
		return;
	}


	/// @todo should we empty the command and answer lists in case of error, too? Could or should matter the whole class!?

	// this check helps if we exit direcs
	if (interfaceState == ON)
	{
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
			answerListMutex.lock();
			tempAnswer = answerList.first();
			answerListMutex.unlock();

			// hey kids, what time is it?
			duration = tempAnswer.timestamp.msecsTo(QDateTime::currentDateTime());

			if (duration > atmelTimeout)
			{
				emit message(QString("General timeout error (%1ms > %2ms)").arg( duration ).arg( atmelTimeout ));

				// timeout
				// let this class know, that we had an error
				setRobotState(OFF);;
				commandSentSuccessfull = false;

				emit heartbeat(RED);
				emit message(QString("<font color=\"#FF0000\">ERROR when waiting for executing command. Stopping %1!</font>").arg(className));
				// stop this thread
				stop();

				//-----------------------
				// inform other modules!
				//-----------------------
				emit robotState(false);

				return;
			}
		}


		// fill data structure
		tempCommand.string = commandString;
		tempCommand.ID = currentID;
		tempCommand.caller = caller;

		// add command and ID to command lists
		commandListMutex.lock();
		commandList.append(tempCommand);
		commandListMutex.unlock();

		// debug msg
		// emit message( QString("Recveived command %1, ID=%2 from %3 appended").arg(tempCommand.string).arg(tempCommand.ID).arg(tempCommand.caller) );
		emit message( QString("Added ID %1. %2 commands in queue.").arg(currentID).arg(commandList.count()));

		// create next command ID
		currentID++; /// @todo double cross check: do have we have more than x commands in the queued in the line? > emit systemerror than.

	} // interfaeState is ON
}


void CommandHandler::takeCommandAnswer(QString atmelAnswer)
{
	answer tempAnswer;
	unsigned long duration;


	// check if time set
	if (atmelTimeout == 0)
	{
		emit message(QString("ERROR: Atmel timeout not set in %1!").arg(className));
		stop();
		return;
	}


	// we have an answer, so we can tell the run-loop, that it can continue
	commandInProgress = false;

	// see if executed answer is in list of expected answers
	for (int i=0; i<answerList.size(); i++)
	{
		// at can be faster than []
		answerListMutex.lock();
		tempAnswer = answerList.at(i);
		answerListMutex.unlock();

		// see if the Atmel answer starts with the starter and the sent command (i.e. *s7... )
		if (atmelAnswer.startsWith( QString("%1%2").arg(starter).arg(tempAnswer.string) ))
		{
			//--------------
			// answer found
			//--------------

			// debug msg
			// emit message(QString("Expected answer %1 received in %2").arg(atmelAnswer).arg(className));

			// remove element from answer list
			answerListMutex.lock();
			answerList.removeAt(i);
			answerListMutex.unlock();

			commandSentSuccessfull = true; // this lets the seperate timeout slot just return

			//----------
			// timeout?
			//----------
			duration = tempAnswer.timestamp.msecsTo(QDateTime::currentDateTime());

			if (duration > atmelTimeout)
			{
				emit message(QString("Timeout (%1ms > %2ms)").arg( duration ).arg( atmelTimeout ));

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

			// debug msg
			emit message(QString("Command duration: %1 ms").arg(duration));
			emit commandInformation(QString("Duration: %1 ms  In queue: %2").arg(duration).arg(commandList.count()));

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

	// let this class know, that we had an error
	setRobotState(OFF);;
	commandSentSuccessfull = false;

	emit heartbeat(RED);
	emit message(QString("<font color=\"#FF0000\">ERROR! Answer %1 not expected. Stopping Stopping %2!</font>").arg(className).arg(className));
	// stop this thread
	stop();

	//-----------------------
	// inform other modules!
	//-----------------------
	emit robotState(false);

	return;
}


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
