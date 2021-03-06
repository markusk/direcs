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

#include "logfile.h"

Logfile::Logfile()
{
	logFilename = "not_set";
}


Logfile::~Logfile()
{
}

/*
QString Logfile::checkPath()
{
	//
	// check the current path from which the program started
	//

	// get the current path and store it
	programPath = QDir::currentPath();

	return programPath;
}


bool Logfile::checkFiles()
{
	QString filename;


	if (filename == "not_set")
	{
		return false;
	}


	// path + filename for ini-file
	filename = programPath;
	filename.append("/");
	filename.append(iniFilename);

	// check if ini-file exists
	if (QFile::exists(filename) == false)
	{
		return false;
	}


	return true;
}
*/

void Logfile::appendLog(QString text, bool CR)
{
	Q_UNUSED(CR); // temporarily not in use


	if (logFilename == "not_set")
	{
		qDebug("logFilename not set!");
		return;
	}


	// remove HTML
	if (text.contains("<") && text.contains(">"))
	{
		int start= -1;

		while (text.contains(">"))
		{
			// search for the first HTML "<"
			start = text.indexOf("<");

			// when found
			if (start != 1)
			{
				text.remove(start, (text.indexOf(">") - start + 1) );
			}
			else
				break; // leave while loop, when we only found an '>' without leading '<'
		};
	}


	now = QDateTime::currentDateTime(); // get the current date and time
	QTextStream out(&file);
	out << now.toString("yyyy") << "-" << now.toString("MM") << "-" << now.toString("dd") << "  " << now.toString("hh") << ":" << now.toString("mm") << ":" << now.toString("ss") << "." << now.toString("zzz") << "  " << text << "\n";
}


void Logfile::setFilename(QString filename)
{
	// set the filename
	logFilename = filename;
	file.setFileName(filename);

	if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append))
	{
		qDebug("Error opening logfile!");
		return;
	}
}


void Logfile::writeHeartbeat(unsigned char state)
{
	Q_UNUSED(state);

	appendLog("-heartbeat-");
}
