#ifndef CIRCUIT_H
#define CIRCUIT_H

//-------------------------------------------------------------------
#include "interface.h"
//-------------------------------------------------------------------

/**
\brief Delivers a initialisation for the robot's circuits

This class delivers a initialisation for the robot's circuits and checks, if the robot is ON or OFF.
*/
class Circuit
{
	public:
		Circuit(InterfaceAvr *i);
		~Circuit();
		
		/**
		Initialises the robot's circuits. It also checks, if the robot is ON or OFF.
		@return The state of the robot (true when ON, false when OFF).
		*/
		bool initCircuit();


	private:
		InterfaceAvr *interface1;
		static const unsigned char INIT = 250;
		static const unsigned char INITANSWER = 64;
};

#endif
