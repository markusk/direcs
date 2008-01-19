#ifndef CIRCUIT_H
#define CIRCUIT_H

//-------------------------------------------------------------------
#include "interfaceAvr.h"
//-------------------------------------------------------------------

/**
\brief Delivers a initialisation for the robot's circuits

This class delivers a initialisation for the robot's circuits and checks, if the robot is ON or OFF.
*/
class Circuit : public QObject
{
	Q_OBJECT

	public:
		Circuit(InterfaceAvr *i);
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


	private:
		InterfaceAvr *interface1;
		static const unsigned char INIT = 250;
		static const unsigned char INITANSWER = 64;
		bool robotIsOn;
		bool firstInitDone;
};

#endif
