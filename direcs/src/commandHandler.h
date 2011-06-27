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

#ifndef COMMANDHANDLER_H
#define COMMANDHANDLER_H

//-------------------------------------------------------------------
#include "interfaceAvr.h"
//-------------------------------------------------------------------
#include <QThread>
#include <QMutex>
#include <QTime>
#include <QTimer>
#include <math.h>
//-------------------------------------------------------------------

/**
\author Markus Knapp
\brief brief brief

text text text
*/
class CommandHandler : public QThread
{
	Q_OBJECT

	public:
		CommandHandler(InterfaceAvr *i, QMutex *m);
		~CommandHandler();

		/**
		Stops the thread.
		*/
		void stop();

		/**
		Starts the thread.
		*/
		virtual void run();


	public slots:
		/**
		This slot enables or disables the simulation mode.
		In the simulation mode all sensor values are set to a fixed value. No real sensor values are read from the robot, when set to true.
		@param state can be true or false.
		@sa Gui::simulate()
		*/
		void setSimulationMode(bool state);

		/**
		This slots takes the robot (circuit) state, to know if the robot is ON or OFF.
		When the class knows this, unnecessary communication with the interface can be avoided.
		@param state can be ON or OFF
		*/
		void setRobotState(bool state);

		/**
		This slot takes the a command string and adds it to a list to be executed by the Atmel controller.
		@param command is the command
		*/
		void takeCommand(QString command);

		/**
		This slot takes the received answers from the Atmel after sending a command string to it.
		@param atmelAnswer contains the answer
		@param correspondingCommand contains the command which was used in the Slot @sa InterfaceAvr::commandCompleted()
		*/
		void takeCommandAnswer(QString atmelAnswer, QString correspondingCommand);


	private slots:
		/**
		This Slot is called if we never get an answer from the Atmel
		*/
		void timeout();


	signals:
		/**
		Sends a string over the network.
		*/
		void sendNetworkString(QString text);

		/**
		This signal is emitted every \todo seconds, when a specific value from the microcontroller was received.
		@sa Gui::setLEDHeartbeat()
		@sa Logfile::writeHeartbeat()
		*/
		void heartbeat(unsigned char state);

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
		mutable QMutex *mutex;				// make this class thread-safe
		InterfaceAvr *interface1;			/// a pointer to the Atmel communication class (serial port stuff)
		volatile bool stopped;
		volatile bool commandInProgress;	/// set to true, when currently executing an command on and with the serial port
		bool simulationMode;
		bool robotState;					// stores the robot state within this class
		bool commandExecutedSuccessfull;	/// set to true, if command executed successfull. In this case a later timeout slot will check this first!

		QList <QString> commands;			/// This list contains the commands for the Atmel. It is a kind of queue.


		QString atmelCommand; /// this is the command for the Atmel
		mutable QMutex varMutex; // this is for the var atmelCommand

		QTime duration; /// for measuring between sending an command to Atmel and the time it needs till the Atmel answers
		static const int ATMELTIMEOUT = 500; /// timeout in ms

		// Every thread sleeps some time, for having a bit more time fo the other threads!
		// Time in milliseconds
		static const unsigned long THREADSLEEPTIME = 750; // Default: 100 ms

		static const int MAXIMUMPLOTHEARTBEAT = 5; /// theoratically 5 Volt for heartbeat "high" @sa Gui::initPlot

		/**
		Since a heartbeat can only high or low. we store only 1 value in this array. But we need an array for the plotThread.
		*/
		int heartbeatValue[1];

		static const bool ON  = true;   /** For motor or robot "ON" */
		static const bool OFF = false;  /** For motor or robot "OFF" */

		/**
		This is for the @sa heartbeat signal (the GUI LED colors)
		*/
		static const unsigned char LEDOFF	= 0;
		static const unsigned char RED		= 1;
		static const unsigned char GREEN	= 2;
};

#endif
