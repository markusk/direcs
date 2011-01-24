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

#include "inifile.h"

Inifile::Inifile()
{
	//------------------------------------------------------------------
	// timer stuff
	//------------------------------------------------------------------
	//commandTimer = new QTimer();
	//connect(commandTimer, SIGNAL(timeout()), SLOT(commandClock()));

	//------------------------------------------------------------------
	// set the name of the programms ini-file
	//------------------------------------------------------------------
	mainIniFilename = "not_set";

	// the setings object is created in the setInifileName() method !


	// Inizialize the (script) command counter
	//commandCounter = 0;

	// Inizialize the last command for the robot
	//lastCommand = "stop";

	// Name of the section to search for in the ini-file (script exection)
	//iniSection = "Sequence1";
}


Inifile::~Inifile()
{
}


QString Inifile::checkPath()
{
	//
	// check the current path from which the program started
	//

	// get the current path and store it
	programPath = QDir::currentPath();

	return programPath;
}


bool Inifile::checkFiles()
{
	QString filename;


	if (mainIniFilename == "not_set")
	{
		qDebug("Path for mainIniFilename not set in Inifile::CheckFiles().");
		return false;
	}


	// path + filename for ini-file
	filename = programPath;
	filename.append("/");
	filename.append(mainIniFilename);

	// check if ini-file exists
	if (QFile::exists(filename) == false)
	{
		return false;
	}
	else
	{
		return true;
	}
/*
	// This variable holds the version of the Windows operating system on which the application is run (Windows only).
	WinVersion QSysInfo::WindowsVersion;
*/

}


QString Inifile::getInifileName()
{
	return mainIniFilename;
}


void Inifile::writeSetting(QString group, QString name, int value)
{
	//-------------------------------------
	// store the programm settings
	//-------------------------------------
	settings->beginGroup(group);

	// save setting
	settings->setValue(name, value);

	settings->endGroup();
}


int Inifile::readSetting(QString group, QString name)
{
	// check if ini-file is writable
	if (settings->isWritable() == false)
	{
		return -2;
	}

	// string for group+value in inifile
	QString iniSection = group + "/" + name;

	// read value from ini-file
	// return value or -1 on error.
	return settings->value(iniSection, -1).toInt();
}


QString Inifile::readString(QString group, QString name)
{
	// check if ini-file is writable
	if (settings->isWritable() == false)
	{
		return "error2";
	}

	// string for group+name in inifile
	QString iniSection = group + "/" + name;

	if (settings->contains(iniSection) == false)
	{
		return "error1";
	}

	// read value from ini-file
	// return String
	return settings->value(iniSection, name).toString();
}


float Inifile::readFloat(QString group, QString name)
{

	// string for group+value in inifile
	QString iniSection = group + "/" + name;

	// read value from ini-file
	// return the value or -1 on error.
	return settings->value(iniSection, -1).toDouble();
}


/*
void Inifile::startStopCommandClock(int command)
{
	//
	// start or stop the command clock
	//

	if (command == START)
	{
		// If timer is not running, start it!
		if  ( !commandTimer->isActive() )
		{
			// timeout every 1000 milli seconds
			commandTimer->start(1000);
		}
		//ui.textEdit1->append("Command timer started...");
	}
	else
	{
		// If timer is running, stop it!
		if  ( commandTimer->isActive() )
		{
			// stop timer
			commandTimer->stop();
		}
		//ui.textEdit1->append("Command timer stopped.");
	}
}
*/


/*
void Inifile::commandClock()
{
	//------------------------------------------------------------
	// Timer for processing the commands from the ini-file
	//------------------------------------------------------------
	//
	//
	//   A T T E N T I O N:
	//
	// For the "duration" of the commands in the ini-file, an int-type is used.
	// An int requires 32 bit and last from -2.147.483.648 to +2.147.483.648
	//
	// The measure unit for this duration is "seconds".
	//
	// 2.147.483.648 seconds are 35.791.394 minutes
	//                       are 596.523 hours
	//                       are 2.4855 days
	//                       are 68 years.
	//
	// this should be enough for one command.
	// I'm not sure, if the battery lasts for this time... ;-)
	//
	// These seconds are the duration for the command - how long one command
	// should be executed.
	// e.g. "forward 10" means "drive 10 seconds forward".
	//
	//----------------------------------------------------------------------------
	QString iniKey;
	QString iniString;
	static int scriptDuration = 0;


	// commandCounter + 1
	commandCounter++;

	// String for the "Key" to read in the ini-file
	iniKey = QString("command%1").arg(commandCounter);

	// read "Sequence1/Command1"
	iniString = settings->value(iniSection + "/" + iniKey, "error").toString();


	// on error, show message
	if (iniString == "error")
	{
		// show message
		ui.textEdit1->append("<font color=\"#FF0000\">Error reading ini-file!!</font>");
		// stop timer (and so stop reading ini-file)
		startStopCommandClock(STOP);

		scriptDuration = 0;

		return;
	}
	else
	{
		//----------------------------
		// if reading was okay
		//----------------------------
		if (iniString != "error")
		{
			int commandDuration = 0;
			QString command;
			QString str;

			// show read result
			//ui.textEdit1->append(iniString);

			// Find the instruction in the string (get from the first character to the first found space char)
			//
			// copy iniString to command
			command = iniString;
			// copy string from first char till the last space
			command.truncate(iniString.lastIndexOf(" "));

			// show command
			//str = QString("command: '%1'").arg(command);
			//ui.textEdit1->append(str);

			/// Find the "command" of the instruction \todo.
			//
			// copy the rest of the iniString from last blank till the end
			str = iniString.mid(iniString.lastIndexOf(" ")+1);

			// Attention: This duration-string is converted to int!!
			commandDuration = str.toInt();

			// show commandDuration
			//str = QString("commandDuration: %1").arg(commandDuration);
			//ui.textEdit1->append(str);


			//-------------------------------------------------
			// "do" the commands
			//-------------------------------------------------

			// start driving
			if (command == "start")
			{
				// Set timer to the duration of the command
				// That means, that the next command is read, when the next timer event occurs.
				commandTimer->setInterval(commandDuration * 1000);

				// start MotorTimer
				startStopMotorClock(START);

				ui.textEdit1->append("Motor clock started.");

				return;
			}

			//
			// stop driving
			//
			if (command == "stop")
			{
				// turn motors OFF
				motorControl(MOTOR1, OFF, SAME);
				motorControl(MOTOR2, OFF, SAME);
				ui.textEdit1->append("Stopped.");

				// Set timer to the duration of the command
				// That means, that the next command is read, when the next timer event occurs.
				commandTimer->setInterval(commandDuration * 1000);

				// when normal script operation (no change maneuver),
				// do the following
				if (iniSection.startsWith("Sequence") == true)
				{
					//----------------
					// stop MotorTimer
					//----------------
					startStopMotorClock(STOP);

					// reset command counter, because the last command is always "stop"
					commandCounter = 0;

					// show script duration
					str = QString("Script duration: %1 seconds").arg(scriptDuration);
					ui.textEdit1->append(str);

					// reset commandDuration
					scriptDuration = 0;

					// stop timer (and so stop reading ini-file)
					startStopCommandClock(STOP);

					return;
				}
				else
				{
					// when method is called from "check For Obstacle"
					// do nothing (don't stop driving and don't stop reading the script)
					return;
				}
			}


			// drive forward
			if (command == "forward")
			{
				motorControl(MOTOR1, ON, CLOCKWISE);
				motorControl(MOTOR2, ON, CLOCKWISE);

				// Set timer to the duration of the command (like "drive forward 10 secs").
				// That means, that the next command is read, when the next timer event occurs.
				// Meanwhile the robot "does" the command
				commandTimer->setInterval(commandDuration * 1000);

				// for measuring the script duration
				scriptDuration += commandDuration;

				// show command
				str = QString("Driving %1 for %2 seconds.").arg(command).arg(commandDuration);
				ui.textEdit1->append(str);

				return;
			}

			// drive backwords
			if (command == "backward")
			{
				motorControl(MOTOR1, ON, COUNTERCLOCKWISE);
				motorControl(MOTOR2, ON, COUNTERCLOCKWISE);

				// Set timer to the duration of the command (like "drive forward 10 secs").
				// That means, that the next command is read, when the next timer event occurs.
				// Meanwhile the robot "does" the command
				commandTimer->setInterval(commandDuration * 1000);

				// for measuring the script duration
				scriptDuration += commandDuration;

				// show command
				str = QString("Driving %1 for %2 seconds.").arg(command).arg(commandDuration);
				ui.textEdit1->append(str);

				return;
			}

			// turning right
			if (command == "right")
			{
				motorControl(MOTOR1, ON, COUNTERCLOCKWISE);
				motorControl(MOTOR2, ON, CLOCKWISE);

				commandTimer->setInterval(commandDuration * 1000);

				// for measuring the script duration
				scriptDuration += commandDuration;

				// show command
				str = QString("Turning %1 for %2 seconds.").arg(command).arg(commandDuration);
				ui.textEdit1->append(str);

				return;
			}

			// turning left
			if (command == "left")
			{
				motorControl(MOTOR1, ON, CLOCKWISE);
				motorControl(MOTOR2, ON, COUNTERCLOCKWISE);

				commandTimer->setInterval(commandDuration * 1000);

				// for measuring the script duration
				scriptDuration += commandDuration;

				// show command
				str = QString("Turning %1 for %2 seconds.").arg(command).arg(commandDuration);
				ui.textEdit1->append(str);

				return;
			}

			//
			// stop executing script!
			//
			if (command == "end")
			{
				// change ini-section back to default
				iniSection="Sequence1";

				// reset command counter, because the last command here is always "end"
				commandCounter = 0;

				//------------------------------------------
				// stop timer (and so stop reading ini-file)
				//------------------------------------------
				startStopCommandClock(STOP);

				// show script duration
				str = QString("Change maneuver ended succesfully after %1 seconds.").arg(scriptDuration);
				ui.textEdit1->append(str);

				// reset scriptDuration
				scriptDuration = 0;

				// re-start sensor timer  < < < < < < < ? ? ? ? < < < <
				// ( stopped in btnExecuteScipt_clicked() !!)
				startStopSensorClock(START);

				return;
			}

		} // reading from ini-file was okay

	} // ini-file OK
}
*/


void Inifile::sync(void)
{
	settings->sync();
}


void Inifile::setFilename(QString filename)
{
	if (mainIniFilename == "not_set")
	{
		// set the filename
		mainIniFilename = filename;

		//------------------------------------------------------------------
		// create the settings object. Use the ini-format
		//------------------------------------------------------------------
		settings = new QSettings(mainIniFilename, QSettings::IniFormat);

		// deactivate fallbacks (read only in the specified file)
		settings->setFallbacksEnabled(false);
	}
}
