#include "circuit.h"

Circuit::Circuit(InterfaceAvr *i)
{
	//------------------------------------------------------------------
	// 
	//------------------------------------------------------------------
	interface1 = i;
}


Circuit::~Circuit()
{
	delete interface1;
}


bool Circuit::initCircuit()
{
	//-------------------------------------------------------
	// Basic init for all the bits on the robot circuit
	//-------------------------------------------------------

	// set the direction
	if (interface1->sendChar(INIT) == false)
	{
		qDebug("ERROR sending to serial port (initCircuit)");
		return false;
	}
	else
	{
		// check if the robot answers with "@"
		char answer = 0;
		interface1->receiveChar(&answer);
		
		if (answer == 64)
		{
			// everthing's fine :-)
			//qDebug("Robot is ON.");
			return true;
		}
	}
	
	// error!
	//qDebug("Robot did not answer. Is it Off?");
	return false;
}


/*
bool Circuit::findRobotHardware()
{
	unsigned char port0 = 0; // USB port 0
	QTime t;
	unsigned char NE555Bit = 0; // the high/low-bit from NE555


	//----------------------------------------------------
	// check and wait for raising edge of NE555 signal
	// test duration: 500 ms
	//----------------------------------------------------
	
	// start time measuring
	t.start();
	do
	{
		//-----------------
		// Read Port 0
		//-----------------
		usbPort->readPort(0, &port0);
		// read the bit
		NE555Bit = ((port0 & NE555) / NE555);
	} while((NE555Bit == 0) && (t.elapsed() <= 2000));
	
	
	//------------------------------------------
	// rising did not appear -> robot is OFF
	//------------------------------------------
	if (NE555Bit == 0)
	{
		// robot is OFF
		return false;
	}
	else
	{
		// robot is ON  :-)
		return true;
	}
}
*/
