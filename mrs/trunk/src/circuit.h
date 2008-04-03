#ifndef CIRCUIT_H
#define CIRCUIT_H

//-------------------------------------------------------------------
#include "interfaceAvr.h"
#include <QMutex>
//-------------------------------------------------------------------

/**
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
		
		
	public slots:
		/**
		Initialises the robot's circuits. It also checks, if the robot is ON or OFF.
		 */
		void initCircuit();

	signals:
		/**
		This signal emits the robot (circuit) state to all connected slots, to tell them if the robot is ON or OFF
		@param state can be ON or OFF
		*/
		void robotState(bool state);


	private:
		mutable QMutex *mutex; // make this class thread-safe
		InterfaceAvr *interface1;
		static const unsigned char INIT = 250;
		static const unsigned char INITANSWER = 64;
		bool robotIsOn;
		bool firstInitDone;
		
		static const bool ON  = true;
		static const bool OFF = false;
};

#endif
