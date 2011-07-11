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
		In the simulation mode... [to  be defined for this class]
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
		@param commandString is the command
		@param caller is the name of the class which called this slot
		*/
		void takeCommand(QString commandString, QString caller);

		/**
		This slot takes the received answers from the Atmel after sending a command string was sent to it.
		@param atmelAnswer contains the answer
		*/
		void takeCommandAnswer(QString atmelAnswer);


	signals:
		/**
		This signal is emitted, when a valid answer was completely received by the Atmel in time (all conditions necessary to be matched!).
		@param answer is the answer string from the Atmel (i.e. *re# or *s7=867#)
		@param caller is the name of the calling class (which called the Slot @sa takeCommand()
		*/
		void commandComplete(QString answer, QString caller);

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

		/**
		This signal emits the robots state to all connected slots, to tell them when we have problem with the communication and so with the Atmel.
		*/
		void robotState(bool state);


	private:
		QString className;					/// this will contain the name of this class at runtime.
		mutable QMutex *mutex;				// make this class thread-safe
		InterfaceAvr *interface1;			/// a pointer to the Atmel communication class (serial port stuff)
		volatile bool stopped;
		volatile bool commandInProgress;	/// set to true, when currently executing an command on and with the serial port
		bool simulationMode;
		bool interfaceState;				/// stores the interface / robot state within this class
		bool commandSentSuccessfull;		/// set to true, if command executed successfull. In this case a later timeout slot will check this first!
		unsigned char currentID;			/// this is a unique ID which is increased by one at every new received command (kind of process ID)

		struct command
		{
			QString string;			/// this is the command
			unsigned char ID;		/// this is the unique ID
			QString caller;			/// this is the name of the calling class (which called the Slot @sa takeCommand()
		};

		struct answer
		{
			QString string;			/// this is the expected answer
			unsigned char ID;		/// this is the unique ID
			QString caller;			/// this is the name of the calling class (which called the Slot @sa takeCommand()
			QDateTime timestamp;	/// this is the timestamps when the command execution *started*. With this we can check how long a command took, when we get the answer
		};

		QList <command> commandList;
		QList <answer>  answerList;

		mutable QMutex commandListMutex; // this is for the var atmelCommand

		/**
		example answer string without value from Atmel: *re#
		example answer string with value    from Atmel: *s7=42#
		*/
		static const char starter    = 42; /// This starts the serial string for the Atmel controller.     42  =  *
		static const char terminator = 35; /// This terminates the serial string for the Atmel controller. 35  =  #
		static const char divider    = 61; /// This divides the serial string for the Atmel controller.    61  =  =

		// Every thread sleeps some time, for having a bit more time fo the other threads!
		// Time in milliseconds
		static const unsigned long THREADSLEEPTIME = 250; // Default: 100 ms

		static const int ATMELTIMEOUT = 500; /// timeout in ms

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
