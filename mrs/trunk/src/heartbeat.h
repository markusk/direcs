#ifndef HEARTBEAT_H
#define HEARTBEAT_H

//-------------------------------------------------------------------
#include "interfaceAvr.h"
#include <QThread>
#include <QMutex>
//-------------------------------------------------------------------

/*!
\brief This thread sends a heartbeat to the microcontroller.
The microccontroller waits for this heartsbeats. It stops/resets the robot, if no heartbeats are received.
*/
class Heartbeat : public QThread
{
    Q_OBJECT

	public:
		Heartbeat(InterfaceAvr *i, QMutex *m);
		~Heartbeat();
		
		/**
		*/
		void init();

		/**
		*/
		bool isConnected(void);
		
		void stop();
		virtual void run();

		
	public slots:
		/**
		This slot sets (stores) the robots (circuits) state within this class.
		@param state can be ON or OFF
		 */
		void setRobotState(bool state);


	signals:
		/**
		TODO: text text text
		alarm
		 */
		void alarm();


	private:
		bool initDone;
		volatile bool stopped;
		bool robotIsOn; //! Stores the robots (circuits) state.ON or OFF
		mutable QMutex *mutex; // make this class thread-safe
		InterfaceAvr *interface1;
		
		// Every thread sleeps some time, for having a bit more time for the other threads!
		// Time in milliseconds
		static const unsigned long THREADSLEEPTIME = 500; // Default: 500 ms
};

#endif
