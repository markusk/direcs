#include "usb_win.h"

//*********************************************************************
// All methods return 0 if successfull and a value not equal on error.
//*********************************************************************

Usb::Usb()
{
	/***
	* constructor
	***/


	// first init of the handle variable
	hgDriver = INVALID_HANDLE_VALUE;
	
	
	// general init  < < < <  still needed?!?!?
	//port0 = 0;
	//port1 = 0;
}


Usb::~Usb()
{
	/***
	* destructor
	***/
}


bool Usb::closeDriver()
{
	/***
	* close USB-Driver
	***/
	
	bool ret = CloseHandle(hgDriver);
	hgDriver = INVALID_HANDLE_VALUE;
	
	return !ret;
}


bool Usb::openDriver()
{
	/***
 	* open USB-Driver
 	***/


	// driver already opened?
	if(hgDriver != INVALID_HANDLE_VALUE)
	{
		// open again for error handling
		closeDriver();
	}	

	// open device driver with the correct filename
	hgDriver = CreateFileA("\\\\.\\CompuLABusb_0", GENERIC_WRITE || GENERIC_READ, FILE_SHARE_WRITE || FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, 0);

	return hgDriver == INVALID_HANDLE_VALUE;
}


bool Usb::writePort(unsigned char Port, unsigned char Value)
{
	/***
	* set port
	*
	* Port:  port to set (0,1)
	* Value: output value
	***/
	
	DWORD command, inDummy, count;
	
	// calculate instruction for driver:
	// 21: set port
	command = (Value<<16) + Port * 256 + 21;
	inDummy = 0;
	
	// output
	return !DeviceIoControl(hgDriver, COMPULAB_IOCTL, &command, sizeof(command), &inDummy, sizeof(inDummy), &count, NULL);
}


bool Usb::readPort(unsigned char Port, unsigned char *Value)
{
	/***
	* read port
	*
	* Port:  port to read (0,1)
	* Value: read value
	***/
	
	DWORD command, count;
	bool ok;
	unsigned char inBytes[2];
	
	// calculate instruction for driver:
	// 21: read port
	command = 20 + Port*256;
	ok = DeviceIoControl(hgDriver, COMPULAB_IOCTL, &command, sizeof(command), &inBytes, sizeof(inBytes), &count, NULL);
	
	if (ok)
	{
		*Value=(unsigned char)inBytes[1];
	}	
	
 	return !ok;
}
