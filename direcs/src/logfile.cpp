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

#include "logfile.h"

Logfile::Logfile()
{
	//------------------------------------------------------------------
	// set the name of the programms ini-file
	//------------------------------------------------------------------
	mainIniFilename = "not_set";
}


Logfile::~Logfile()
{
}


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
	

	if (mainIniFilename == "not_set")
	{
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


QString Logfile::getInifileName()
{
	return mainIniFilename;
}


void Logfile::writeSetting(QString group, QString name, int value)
{
	//-------------------------------------
	// store the programm settings		
	//-------------------------------------
	settings->beginGroup(group);
	
	// save setting
	settings->setValue(name, value);
	
	settings->endGroup();
}


int Logfile::readSetting(QString group, QString name)
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


QString Logfile::readString(QString group, QString name)
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


void Logfile::sync(void)
{
	settings->sync();
}


void Logfile::setInifileName(QString filename)
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
