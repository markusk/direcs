#include "heartbeat.h"


Heartbeat::Heartbeat(InterfaceAvr *i, QMutex *m) : QThread()
{
	// copy the pointer from the original object
	interface1 = i;
	mutex = m;

	stopped = false;
	initDone = false;
	robotIsOn = false;
}


Heartbeat::~Heartbeat()
{
	// FIXME: delete SIGSEV!
	//delete imgPtr;
}


void Heartbeat::stop()
{
	stopped = true;
}


void Heartbeat::run()
{
	if (initDone==false)
	{
		//init();
	}

	//
	//  start "threading"...
	//
	while (!stopped)
	{
		// Lock the mutex. If another thread has locked the mutex then this call will block until that thread has unlocked it.
		mutex->lock();
		
		//-------------------------------------------------------
		// Basic init for all the bits on the robot circuit
		//-------------------------------------------------------
		if (interface1->sendChar(111) == true)
		{
				// Unlock the mutex
				mutex->unlock();
		}
		
		// Unlock the mutex.
		mutex->unlock();
	
	} // while !stopped
	
	
	stopped = false;
}


bool Heartbeat::isConnected(void)
{
	return robotIsOn;
}


void Heartbeat::init()
{
	//if ( (cameraIsOn == true) && (initDone == false) )
	if (initDone == false)
	{
		// do only *one* init!
		initDone = true;

		//-----------------
		// try to connect
		//-----------------
		
		if (0)
		{
			qDebug("INFO: could not initialize blablabla");
			robotIsOn = false;
			stopped = true;
			return;
		}
		
		robotIsOn = true;
	}
}


void Heartbeat::setRobotState(bool state)
{
	robotIsOn = state;
}
