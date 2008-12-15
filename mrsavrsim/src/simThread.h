/*************************************************************************
 *   Copyright (C) 2008 by Markus Knapp                                  *
 *   mrs @ direcs.de                                                     *
 *                                                                       *
 *   This file is part of mrs.                                           *
 *                                                                       *
 *   mrs is free software: you can redistribute it and/or modify it      *
 *   under the terms of the GNU General Public License as published      *
 *   by the Free Software Foundation, version 3 of the License.          *
 *                                                                       *
 *   mrs is distributed in the hope that it will be useful,              *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of      *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the        *
 *   GNU General Public License for more details.                        *
 *                                                                       *
 *   You should have received a copy of the GNU General Public License   *
 *   along with mrs. If not, see <http://www.gnu.org/licenses/>.         *
 *                                                                       *
 *************************************************************************/

#ifndef SIMTHREAD_H
#define SIMTHREAD_H

//-------------------------------------------------------------------
#include <QThread>
#include <QMutex>
#include "interfaceAvr.h"
//-------------------------------------------------------------------

/**
\brief Responsible for simulating the robot.

The SimThread class is a thread, responsible for ....
*/
class SimThread : public QThread
{
    Q_OBJECT

	public:
		SimThread(InterfaceAvr *i, QMutex *m);
		~SimThread();
		
		/**
		Stops the thread.
		*/
		void stop();
		
		/**
		Starts the thread.
		*/
		virtual void run();


	signals:
		/**
		This signal is emitted when all sensors were read.
		@sa Mrs::showSensorData()
		*/
		void sensorDataComplete();


	private:
		InterfaceAvr *interface1;
		mutable QMutex *mutex; // make this class thread-safe
		volatile bool stopped;
		bool simulationMode;
		
		// Every thread sleeps some time, for having a bit more time fo the other threads!
		// Time in milliseconds
		static const unsigned long THREADSLEEPTIME = 25; // Default: 25 ms  (old: 5 ms)
};

#endif
