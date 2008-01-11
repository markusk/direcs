#include "interfaceAvr.h"

InterfaceAvr::InterfaceAvr()
{
	// creating the serial port object
	serialPort = new QextSerialPort();
}


InterfaceAvr::~InterfaceAvr()
{
	delete serialPort;
}


bool InterfaceAvr::openComPort(QString comPort)
{
	//-------------------------------------------------------
	// open a serial port ("COM1" for example on Windows)
	// using qextserialport
	//-------------------------------------------------------
	serialPort->setPortName(comPort);
	serialPort->setBaudRate(BAUD9600); // < < < < different to camera comunication (BAUD115200) in cam.cpp
	serialPort->setDataBits(DATA_8);
	serialPort->setStopBits(STOP_1);
	serialPort->setParity(PAR_NONE);
	serialPort->setFlowControl(FLOW_OFF);
	
	if (serialPort->open() == false)
	{
		return false;
	}
	
	// Flushes all pending I/O to the serial port.
	serialPort->flush();
	
	return true;
}


void InterfaceAvr::closeComPort()
{
	serialPort->close();
}


bool InterfaceAvr::sendChar(char character)
{
	static int receiveErrorCounter = 0;
	
	//------------------------------------------
	// send one byte to serial port 
	//------------------------------------------
	if (serialPort->putChar(character) == false)
	{	
		receiveErrorCounter++;
		//qDebug("%d. ERROR sendChar to serial port (sendChar, InterfaceAvr)", receiveErrorCounter);
	
		//
		// MASSIVE COMMUNICATION ERROR!
		//
		if (receiveErrorCounter >= 4)
		{
			receiveErrorCounter = 0;
			emit tooMuchErrors();
		}
		
		return false;
	}
	
	
	return true;
}


bool InterfaceAvr::receiveChar(char *character)
{
	return serialPort->getChar(character);
}


bool InterfaceAvr::receiveInt(int *value)
{
	static int receiveErrorCounter = 0;
	char character = 0;
	int intValue = 0;
	int helpValue = 0;
	
	
	//-----------------
	// receive MS-Byte
	//-----------------
	if (receiveChar(&character) == false)
	{
		receiveErrorCounter++;
		//qDebug("%d. ERROR receiving from serial port (receiveInt, InterfaceAvr)", receiveErrorCounter);
	
		//
		// MASSIVE COMMUNICATION ERROR!
		//
		if (receiveErrorCounter >= 4)
		{
			receiveErrorCounter = 0;
			emit tooMuchErrors();
		}
		
		value = 0;
		return false;
	}
	
	// reset error counter
	receiveErrorCounter = 0;

	
	// bit shifting
	intValue = (character << 8);

	
	//-----------------
	// receive LS-Byte
	//-----------------
	if (receiveChar(&character) == false)
	{
		//qDebug("ERROR receiving from serial port (receiveInt, InterfaceAvr)");
		value = 0;
		return false;
	}

	// convert from signed char to unsigned int
	if (character < 0)
	{
		helpValue = ( 127 & character ) + 128;
	}
	else
	{
		helpValue = character;
	}
	
	
	// this is the 16 Bit result
	intValue += helpValue;
	
	// "return" the value
	*value = intValue;
	
	return true;
}
