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
#include <QTime>


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
		Emits a signal to check network state of this program. @sa Direcs::setNetworkState
		*/
		void checkNetworkState();

		/**
		 * @brief checkArduinoState
		 */
		void checkArduinoState();

		/**
		Sends a signal over the network. E.g. if we are 'master' or 'slave. @sa Direcs::sendNetworkMessage
		*/
		void networkMessage();

	public slots:
		/**
		This slots takes the robot (circuit) state, to know if the robot is ON or OFF.
		When the class knows this, unnecessary communication with the interface can be avoided.

		@param state can be ON or OFF
		 */
		void setRobotState(bool state);


	private:
		QTime startTime; /// this is for getting the current time
		QTime now; /// this is for getting the current time
		volatile bool stopped;

		bool robotState; // stores the robot state within this class
		bool networkStateSet; // event occured?
		bool arduinoStateSet; // event occured?
		static const int timeToNetworkCheck = 3; // time in seconds
		static const int timeToArduinoCheck = 3; // time in seconds

		// Every thread sleeps some time, for having a bit more time fo the other threads!
		// Time in milliseconds
		static const unsigned long THREADSLEEPTIME = 1000; // 1000 ms

};

#endif
