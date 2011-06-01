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

#ifndef CIRCUIT_H
#define CIRCUIT_H

//-------------------------------------------------------------------
#include "interfaceAvr.h"
#include <QMutex>
#include <QTime>
//-------------------------------------------------------------------

/**
\author Markus Knapp
\brief Delivers a initialisation for the robot's circuits

This class delivers a initialisation for the robot's circuits and checks, if the robot is ON or OFF.
*/
class Circuit : public QObject
{
	Q_OBJECT

	public:
		Circuit(InterfaceAvr *i, QMutex *m);
		~Circuit();

		/**
		@return The state of the robot (true when connected (and aswers), false when not).
		*/
		bool isConnected();

		/**
		@return The state of the compass module which is connected to the robots circuit (Atmel board).
		*/
		bool compassConnected();


	public slots:
		/**
		Initialises the robot's circuits. It also checks, if the robot is ON or OFF.
		If the robot is ON and answers, it emits the Signal @sa robotState()
		*/
		void initCircuit();

		/**
		Puts the robot's circuits to sleep. This also disables the robot's watchdog!
		@return true, when everything was fine
		*/
		bool sleep();

		/**
		Initialises the robot's 3D compass. Actually it checks, if the module is connected or not.
		@return true, when connected.
		*/
		bool initCompass();

		/**
		This slots takes the robot (circuit) state, to know if the robot is ON or OFF.
		When the class knows this, unnecessary communication with the interface can be avoided.

		@param state can be ON or OFF
		 */
		void setRobotState(bool state);

		/**
		This slot takes the received Atmel command string and stores it in a class member.
		@param state
		@param atmelAnswer
		*/
		void takeAnswer(bool state, QString atmelAnswer);


	signals:
		/**
		This signal emits the robot (circuit) state to all connected slots, to tell them if the robot is ON or OFF
		@param state can be true or false
		*/
		void robotState(bool state);

		/**
		This signal emits the 3D compass module state; it checks if the module is connected to the Atmel board.
		Not in use, at the moment...
		@param state can be true or false
		*/
		void compassState(bool state);

		/**
		Sends a string to the GUI log.
		@param text is the message to be emitted
		*/
		void message(QString text);


	private:
		mutable QMutex *mutex; // make this class thread-safe
		InterfaceAvr *interface1;
		QString atmelCommand; /// this is the command for the Atmel
		QString expectedAtmelAnswer; /// this stores the answer which the Atmel should Answer from the last command he got.
		bool answerReceived;  /// this indicates, that a complete Atmel command was received - e.g. *sl#
		bool answerTimeout; /// this is set to true, when we have a timout while waiting for an Atmel answer

		QTime duration; /// for measuring between sending an command to Atmel and the time it needs till the Atmel answers
		static const int ATMELTIMEOUT = 250; /// timeout in ms

		static const unsigned char INIT = 64;
		static const unsigned char INITANSWER = 64;
		bool circuitState; // stores the robot state within this class
		bool firstInitDone;
		bool compassCircuitState; // stores the state of the compass module within this class

		static const bool ON  = true;
		static const bool OFF = false;
};

#endif
