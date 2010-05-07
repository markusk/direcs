/*************************************************************************
 *   Copyright (C) 2010 by Markus Knapp                                  *
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
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QDateTime>
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
		Sets the name of the log file.
		@param filename
		*/
		void setFilename(QString filename);
	
		
	public slots:
		
		/**
		Appends text to the log file.
		@param text is the text to be written.
		@param CR adds a carriage return (CR) to the text, if true (default). This parameter is optional!
		 */
		void appendLog(QString text, bool CR=true);
		
		/**
		Write the received heartbeat signal from the hardware to the logfile.
		TODO: Place somewhere else!
		@param state is unused!
		@sa SensorThread::heartbeat()
		 */
		void writeHeartbeat(unsigned char state);


	private:
		QString logFilename;
		QFile file;
		QString programPath;
};

#endif
