#include "circuit.h"

Circuit::Circuit(InterfaceAvr *i)
{
	// copy the pointer from the original object
	interface1 = i;
	robotIsOn = false;
}


Circuit::~Circuit()
{
}


void Circuit::initCircuit()
{
	//-------------------------------------------------------
	// Basic init for all the bits on the robot circuit
	//-------------------------------------------------------
	if (interface1->sendChar(INIT) == false)
	{
		robotIsOn = false;
		//return false;
	}
	else
	{
		// check if the robot answers with "@"
		char answer = 0;
		interface1->receiveChar(&answer);
		
		if (answer == INITANSWER)
		{
			// everthing's fine :-)
			robotIsOn = true;
			//return true;
		}
	}
	
	robotIsOn = false;
	//return false;
}


bool Circuit::isConnected()
{
	return robotIsOn;
}
