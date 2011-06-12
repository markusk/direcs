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

#ifndef SIMULATIONTHREAD_H
#define SIMULATIONTHREAD_H

//-------------------------------------------------------------------
#include "interfaceAvr.h"
//-------------------------------------------------------------------
#include <QThread>
#include <QMutex>
#include <QDebug>
//-------------------------------------------------------------------



/**
\author Markus Knapp
\brief Responsible for simulating the Atmel on the serial port.

This thread answers like the atmel on the serial line.
*/
class SimulationThread : public QThread
{
	Q_OBJECT

	public:
		SimulationThread(InterfaceAvr *i, QMutex *m);
		~SimulationThread();

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
		@param status can be true or false.
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
		This slots takes a string received from the Atel. @sa InterfaceAvr::commandCompleted()
		@param string contains the received string
		*/
		void commandReceived(QString string);


	signals:
		/**
		This signal is emitted every TODO: seconds, when a specific value from the microcontroller was received.
		@sa Gui::setLEDHeartbeat()
		@sa Logfile::writeHeartbeat()
		*/
		void heartbeat(bool alive);

		/**
		Emits a info or error message to a slot.
		This slot can be used to display a text on a splash screen, log file, to print it to a console...
		*/
		void message(QString text, bool CR=true, bool sayIt=false, bool addTimestamp=true);

		/**
		Emits a info or error message to a slot.
		This slot can be used to display a text on a splash screen, log file, to print it to a console...
		*/
		void answer(QString text, bool CR=true, bool sayIt=false, bool addTimestamp=true);

		/**
		Simulated Atmel method
		*/
		void greenLED(bool state);

		/**
		Simulated Atmel method
		*/
		void redLED(bool state);

		/**
		Simulated Atmel method
		*/
		void relais(bool state);


	private:
		/**
		Simulated Atmel method
		*/
		void sendUInt(uint16_t value);

		/**
		Simulated Atmel method
		*/
		uint16_t readADC(unsigned char channel);

		/**
		This method sends the 'answer' Signal/string to the Atmel via InterfaceAvr.
		To reuse IntervaceAvr in direcs.cpp., too, we remove the starter * and terminator # in this method, since this will be sent automatically by @sa InterfaceAvr::sendString()
		*/
		void sendToAtmel(QString string);

		mutable QMutex *mutex; // make this class thread-safe
		InterfaceAvr *interface1;
		volatile bool stopped;
		bool simulationMode;
		bool robotState; // stores the robot state within this class
		bool commandComplete; /// this is set to true by the slot @sa commandReceived

		/**
		Simulated Atmel values
		*/
		uint16_t leftDistanceCounter;
		uint16_t rightDistanceCounter;

		QString commandString; ///  this will contain the command string from the Atmel when received via signal from InterfaceAVr


		// Every thread sleeps some time, for having a bit more time fo the other threads!
		// Time in milliseconds
		static const unsigned long THREADSLEEPTIME = 100; // Default: 25 ms

		static const bool ON  = true;   /** For motor or robot "ON" */
		static const bool OFF = false;  /** For motor or robot "OFF" */

		static const char starter    = 42; /// This starts the serial string for the Atmel controller.     42  =  *
		static const char terminator = 35; /// This terminates the serial string for the Atmel controller. 35  =  #

		static const float CONVERSIONFACTORVOLTAGESENSOR1 = 57.000; // 12 V, command *s8#
		static const float CONVERSIONFACTORVOLTAGESENSOR2 = 36.125; // 24 V, command *s7#


/**
Simulated Atmel method defines
*/
// these numbers represent the Ports of the A/D converter C  !!!
// for the infrared sensors and the motor sensors
#define SENSOR1					0  // ADC0
#define SENSOR2					1  // ADC1
#define SENSOR3					2  // ADC2
#define SENSOR4					3  // ADC3
#define SENSOR5					4  // ADC4
#define SENSOR6					5  // ADC5
#define SENSOR7					6  // ADC6
#define SENSOR8					7  // ADC7

#define SENSORMOTOR1			38 // ADC14
#define SENSORMOTOR2			39 // ADC15
};

#endif
