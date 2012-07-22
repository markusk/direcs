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

#ifndef TIMERTHREAD_H
#define TIMERTHREAD_H

#include <QThread>


/**
\author Markus Knapp
\brief tbd

tbd
*/
class TimerThread : public QThread
{
	Q_OBJECT

	public:
		TimerThread();
		~TimerThread();

		/**
		Stops the thread.
		*/
		void stop();

		/**
		Starts the thread.
		*/
		virtual void run();

		/**
		tbd
		@return true on success, false on error
		*/
		bool init();


	signals:
		/**
		Emits a string to the GUI log / console.
		@sa Gui::appendLog()
		@param text is the message to be emitted
		@param CR is true when a CR/LF should be added at the end of the line (default)
		@param sayIt is true when the text for the log also should be spoken (default=false)
		@param addTimestamp If true, the a timestamp is added in front of the text. This parameter is optional.
		*/
		void message(QString text, bool CR=true, bool sayIt=false, bool addTimestamp=true);

		/**
		Emits a emergency signal for letting other modules know that we have a massive sensor error. So in that case an emergency stop or so could be initiated.
		@param errorlevel needs to be defined. Temporariliy we use -2 in case of error in this thread.
		*/
		void systemerror(int errorlevel);


	private:
		QDateTime startTime; /// this is for getting the current time
		QDateTime now; /// this is for getting the current time
		volatile bool stopped;

		// Every thread sleeps some time, for having a bit more time fo the other threads!
		// Time in milliseconds
		static const unsigned long THREADSLEEPTIME = 3000; // 3000 ms
};

#endif
