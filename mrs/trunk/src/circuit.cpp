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
		
		// everthing's fine :-)
		if (answer == INITANSWER)
		{
			// Unlock the mutex
			mutex->unlock();
			firstInitDone = true;
			robotIsOn = true;
			emit robotState(true);
			return;
		}
	}
	
	// Unlock the mutex.
	mutex->unlock();
	
	firstInitDone = true;
	robotIsOn = false;
	qDebug("Robot is OFF.");
	emit robotState(false);
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
