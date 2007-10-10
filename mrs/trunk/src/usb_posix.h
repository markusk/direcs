#ifndef USB_H
#define USB_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <QDir>
#include <QMessageBox>
#include <QtDebug>


class Usb 
{
	public:
		Usb();
		~Usb();

		bool openDriver();
		bool closeDriver();
		bool readPort(unsigned char port, unsigned char *retValue);
		bool writePort(unsigned char port, unsigned char value);

		
	private:
		QString driverDir;
};

#endif
