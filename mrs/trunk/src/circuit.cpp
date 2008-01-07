#include "circuit.h"

Circuit::Circuit(InterfaceAvr *i)
{
	// copy the pointer from the original object
	interface1 = i;
}


Circuit::~Circuit()
{
}


bool Circuit::initCircuit()
{
	//-------------------------------------------------------
	// Basic init for all the bits on the robot circuit
	//-------------------------------------------------------
	if (interface1->sendChar(INIT) == false)
	{
		return false;
	}
	else
	{
		// check if the robot answers with "@"
		char answer = 0;
		interface1->receiveChar(&answer);
		
		if (answer == INITANSWER)
		{
			// everthing's fine :-)
			return true;
		}
	}
	
	return false;
}
