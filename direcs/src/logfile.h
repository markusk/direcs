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

#ifndef LOGFILE_H
#define LOGFILE_H

//-------------------------------------------------------------------
#include <QObject>
#include <QSettings>
#include <QDir>
#include <QFile>
//-------------------------------------------------------------------

/**
\brief Handles the access to the global log-file.

This class handles the access to the global log-file where all actions are stored if enabled in the ini-file.
*/
class Logfile : public QObject
{
    Q_OBJECT

	public:
		Logfile();
		~Logfile();
		
		/**
		Checks the current path from which the program started.
		@return The path as a string.
		*/
		QString checkPath();
		
		/**
		Checks if the needed ini-file exists in the current path.
		@return True, when the file was found.
		*/
		bool checkFiles();
		
		/**
		Stores a settings in an ini-file. The settings are stored in the typical ini-file-format.\n
		Example ini-file:\n
		\n
		[Config]\n
		motor1Speed=23
		@param group is the group area in the file (e.g. \e Config).
		@param name is the name of the parameter (e.g. \e motor1speed).
		@param value is the value of the parameter (e.g. \e 23).
		*/
		void writeSetting(QString group, QString name, int value);
		
		/**
		Reads an integer setting from an ini-file. The settings are read in the typical ini-file-format.\n
		Example ini-file:\n
		\n
		[Config]\n
		motor1Speed=23
		@param group is the group area in the file (e.g. \e Config).
		@param name is the object to read (e.g. \e motor1speed).
		@return The setting as an integer value.
		*/
		int readSetting(QString group, QString name);
		
		/**
		Reads a string from an ini-file. The settings are read in the typical ini-file-format.\n
		Example ini-file:\n
		\n
		[Config]\n
		serialPortMicrocontroller=/dev/ttyUSB1
		@param group is the group area in the file (e.g. \e Config).
		@param name is the object to read (e.g. \e serialPortMicrocontroller).
		@return The setting as a string.
		*/
		QString readString(QString group, QString name);
		
		/**
		Forces the ini.file to be written \e immediately. For example at program end.
		*/
		void sync(void);
		
		/**
		@return The current ini-file.
		*/
		QString getInifileName();


	public slots:
		/**
		set the name of the ini-file where all the settings are stored.
		@param filename
		*/
		void setInifileName(QString filename);


	private:
		QSettings* settings;
		QString programPath;
		QString mainIniFilename;
};

#endif
