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

#ifndef CIRCUIT_H
#define CIRCUIT_H

//-------------------------------------------------------------------
#include "interfaceAvr.h"
#include <QMutex>
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
		@return true, when the initialization was fine, so the robot is ON.
		*/
		bool initCircuit();

		/**
		 * @brief initArduino
		 * @return
		 */
		bool initArduino();

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
		QString className;	/// this will contain the name of _this_ class at runtime for debug messages

		mutable QMutex *mutex; // make this class thread-safe
		InterfaceAvr *interface1;

		QString atmelCommand;
		QString atmelAnswer;
		QString expectedAtmelAnswer;

		QString commandInitCircuit;		///	*re#
		QString commandInitCompass;		///	*cc#
		QString commandSleep;			///	*sl#

		static const unsigned char INIT = 64;
		static const unsigned char INITANSWER = 64;
		bool circuitState; // stores the robot state within this class
		bool firstInitDone;
		bool compassCircuitState; // stores the state of the compass module within this class

		static const bool ON  = true;
		static const bool OFF = false;

		static const int atmelTimout = 500; // time in ms for waiting for an answer

		/**
		example answer string without value from Atmel: *re#
		example answer string with value from Atmel:    *s7=42#
		*/
		static const char starter    = 42; /// This starts the serial string for the Atmel controller. 42 = *
		static const char terminator = 35; /// This terminates the serial string for the Atmel controller. 35 = #
		static const char divider    = 61; /// This divides the serial string for the Atmel controller. 61 = =
};

#endif
