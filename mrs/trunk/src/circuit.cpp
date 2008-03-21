#include "circuit.h"

Circuit::Circuit(InterfaceAvr *i, QMutex *m)
{
	// copy the pointer from the original object
	interface1 = i;
	mutex = m;

	robotIsOn = false;
	firstInitDone = false;
}


Circuit::~Circuit()
{
}


void Circuit::initCircuit()
{
	//-------------------------------------------------------
	// Basic init for all the bits on the robot circuit
	//-------------------------------------------------------
	if (interface1->sendChar(INIT) == true)
	{
		// check if the robot answers with "@"
		char answer = 0;
		interface1->receiveChar(&answer);
		
		if (answer == INITANSWER)
		{
			// everthing's fine :-)
			firstInitDone = true;
			robotIsOn = true;
			return;
		}
	}
	
	qDebug("Robot is OFF.");
	firstInitDone = false;
	robotIsOn = false;
}


bool Circuit::isConnected()
{
	// if not tried to init hardware, do this!
	if (firstInitDone == false)
	{
		initCircuit();
		firstInitDone = true;
	}

	return robotIsOn;
}
