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

#ifndef GSMTHREAD_H
#define GSMTHREAD_H

//-------------------------------------------------------------------
#include "interfaceAvr.h"
//-------------------------------------------------------------------
#include <QThread>
#include <QMutex>
#include <QCoreApplication>
//-------------------------------------------------------------------

/**
\author Markus Knapp
\brief Responsible for communicating with the GSM module.

The GSMThread class is a thread, responsible for getting all data from and to the GSM module.
Currently it checks if new SMS are received and emits a signal. This thread can also delete SMS on the GSM module.


The following commands are currently implemented (same on Arduino side):

gsmi	=	init GSM module
gsmp	=	unlock GSM module with PIN
gsms	=	(get) GSM status

smsc	=	count available SMS
smsl	=	read Last SMS
smsr	=	read SMS #
smss	=	send SMS
smsd	=	delete SMS #
*/
class GSMThread : public QThread
{
	Q_OBJECT

	public:
		GSMThread(InterfaceAvr *i, QMutex *m);
		~GSMThread();

		/**
		Stops the thread.
		*/
		void stop();

		/**
		Starts the thread.
		*/
		virtual void run();

		/**
		 * @return the no of available SMS's
		 */
		int getSMSavailable();

		/**
		 * @brief Reads the last SMS
		 * @param returns the text of the SMS (body)
		 * @return true on success
		 */
		bool readLastSMS(QString &text);

		/**
		 * @return the GSM status
		 *
		 * 0 = Not registered
		 * 1 = Registered (home)
		 * 2 = Not registered (searching)
		 * 3 = Denied
		 * 4 = Unknown
		 * 5 = Registered roaming
		 */
		int getGSMStatus();

		/**
		@return The heartbeat value which should look like high=5 or low=0 Volt.
		*/
		int getHeartbeatValue();


	public slots:
		/**
		 * @brief Initialises the GSM module
		 * @return true on success
		 */
		bool init();

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
		This slots takes the GSM module state, to know if its down or not.
		When the class knows this it is unnecessary to communicate with the GSM module.
		@param state can be true or false
		*/
		void setGSMState(bool state);


	signals:
		/**
		Sends a string over the network.
		*/
		void sendNetworkString(QString text);

		/**
		Emits the current GSM status (i.e. to the GUI GSM LED)
		@sa Gui::setLEDGSM()
		*/
		void GSMStatus(unsigned char status);

		/**
		This signal is emitted every \todo seconds, when a specific value from the microcontroller was received.
		@sa Gui::setLEDHeartbeat()
		@sa Logfile::writeHeartbeat()
		*/
		void heartbeat(unsigned char state);

		/**
		This signal is emitted when SMS are available (>0).
		@param the number of available SMS
		@param text contains the text of the SMS
		*/
		void SMSavailable(int noSMS, QString text);

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
		@param errorlevel needs to be defined. Temporariliy we use -3 in case of error in this thread. -2 is for the sensor thread.
		*/
		void systemerror(int errorlevel);


	private:
		/**
		 * @brief Checks how many SMS are available.
		 * @return the amount of available SMS, or -1 in case of error
		 */
		int countSMS();

		/**
		 * @brief Unlocks the SIM PIN on the GSM module
		 * @return true on success
		 */
		bool unlockPIN();

		/**
		 * @brief Gets the GSM status and stores it locally.
		 * @return true on success
		 *
		 * 0 = Not registered
		 * 1 = Registered (home)
		 * 2 = Not registered (searching)
		 * 3 = Denied
		 * 4 = Unknown
		 * 5 = Registered roaming
		 */
		bool getStatus();


		QString className;	/// this will contain the name of _this_ class at runtime for debug messages
		mutable QMutex *mutex; // make this class thread-safe
		InterfaceAvr *interface1;
		volatile bool stopped;
		bool simulationMode;
		bool robotState; // stores the robot state within this class
		bool GSMState; // stores the GSM module state within this class
		int networkState; // stores the state of the GSM network

		// Every thread sleeps some time, for having a bit more time fo the other threads!
		// Time in milliseconds
		static const unsigned long THREADSLEEPTIME = 1000; // Default: 100 ms?!?

		int availableSMS; /// contains the no of available SMS
		int lastAmountSMS;
		bool firstSMSCount;

		static const int MAXIMUMPLOTHEARTBEAT = 5; /// theoratically 5 Volt for heartbeat "high" @sa Gui::initPlot

		/*
		Since a heartbeat can only high or low. we store only 1 value in this array. But we need an array for the plotThread.
		*/
		//int heartbeatValue[1];

		/**
		This is for the @sa heartbeat signal (the GUI LED colors)
		*/
		static const unsigned char LEDOFF	= 0;
		static const unsigned char RED		= 1;
		static const unsigned char GREEN	= 2;
		static const unsigned char BLUE	    = 3;
		static const unsigned char WHITE	= 4;
		static const unsigned char YELLOW	= 5;

		static const bool ON  = true;
		static const bool OFF = false;

		static const int GSM_Not_Registered = 0;
		static const int GSM_Registered_Home = 1;
		static const int GSM_Not_Registered_Searching = 2;
		static const int GSM_Denied = 3;
		static const int GSM_Unknown = 4;
		static const int GSM_Registered_Roaming = 5;
};

#endif
