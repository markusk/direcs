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
	// Lock the mutex. If another thread has locked the mutex then this call will block until that thread has unlocked it.
	mutex->lock();
	
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
			// Unlocks the mutex. Attempting to unlock a mutex in a different thread to the one that locked it results in an error.
			mutex->unlock();
			// everthing's fine :-)
			firstInitDone = true;
			robotIsOn = true;
			return;
		}
	}
	
	// Unlocks the mutex. Attempting to unlock a mutex in a different thread to the one that locked it results in an error.
	mutex->unlock();
	
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
