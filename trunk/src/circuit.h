#ifndef CIRCUIT_H
#define CIRCUIT_H

//-------------------------------------------------------------------
#include "interface.h"
//-------------------------------------------------------------------

class Circuit
{
	public:
		Circuit(InterfaceAvr *i);
		~Circuit();
		bool initCircuit();
		//bool findRobotHardware();


	private:
		//char writeToDevice(unsigned char);  entfaellt ?
		InterfaceAvr *interface1;
		
		// The bit for the hardware check (is robot turned on?)
		//static const unsigned char NE555 = BIT7;

		static const unsigned char INIT = 250;

};

#endif
