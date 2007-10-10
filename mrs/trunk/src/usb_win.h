#ifndef USB_H
#define USB_H

#include <windows.h>

// Control-Code which is used from the USB-Driver
#define COMPULAB_IOCTL 4


class Usb 
{
	public:
		Usb();
		~Usb();

		bool closeDriver();
		bool openDriver();
		bool writePort(unsigned char, unsigned char);
		bool readPort(unsigned char, unsigned char *);
		unsigned char port0;
		unsigned char port1;


	private:
		// handle for the device driver
		HANDLE hgDriver;
};

#endif
