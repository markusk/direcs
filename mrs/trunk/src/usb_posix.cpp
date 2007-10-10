#include "usb_posix.h"

Usb::Usb()
{
	driverDir = "/sys/bus/usb/drivers/cypress_cy7c63/";
}


Usb::~Usb()
{
}


bool Usb::openDriver()
{
	QString filename;
	QDir directory(driverDir);
	QStringList filter;
	QStringList directoryList;
	

	// show only directories
	directory.setFilter(QDir::Dirs | QDir::NoDotAndDotDot);
	// show only "devices"
	filter << "*-*:*.*";
	directory.setNameFilters(filter);
	// sort by name
	directory.setSorting(QDir::Name | QDir::DirsFirst);

	// copy directory names in a string list
	directoryList = directory.entryList();

	qDebug() << "Using driver directory" << driverDir;
	// check if there is at least one device (directory)
	if (directoryList.isEmpty())
	{
		QMessageBox::critical(0, "Sorry", "No cypress USB device found!\nAlready connected?");
		return false;
	}

	
	// Debugging...
	/*
	qDebug("dirs: %d", directoryList.size());
	for (int i = 0; i < directoryList.size(); ++i)
	{
		qDebug(QString(directoryList.at(i)).toAscii());
	}
	*/
	
	
	// check if there is more than one device (directory)
	if (directoryList.size() > 1)
	{
		QMessageBox::critical(0, "Sorry", "More than one cypress USB device found!\nThis is not implemented yet...");
		return false;
	}

	// append first found directory name of device to the path
	driverDir += directoryList.at(0);
	qDebug() << "Using device" << driverDir;


	return true;
}


bool Usb::closeDriver()
{
	return true;
}


bool Usb::readPort(unsigned char port, unsigned char *retValue)
{
	FILE *file;
	QString filename;
	int value = 0;
	
	
	switch (port)
	{
		case 0:
			filename = driverDir + "/port0";
			break;
		case 1:
			filename = driverDir + "/port1";
			break;
		default:
			qDebug("Sorry, port number has to be 0 or 1, not %d!", port);
			return false;
			break;
	}
	
	
	//----------------------------
	// open device file / port
	//----------------------------
	file = fopen(filename.toAscii(),"rb");
	
	if (file == NULL)
	{
		qDebug("Error opening file (port%d).", port);
		qDebug() << "Errormsg:" << strerror(errno);
		return false;
	}
	
	
	//-------------------
	// read value
	//-------------------
	if (fscanf(file, "%d", &value) != EOF)
	{
		//qDebug("Value read port%d: %d", port, value);
		fclose(file);
						
		// "return" the value
		*retValue = value;
		return true;
	}

	// error!
	fclose(file);
	qDebug("Error reading file (port%d).", port);
	qDebug() << "Errormsg:" << strerror(errno);
	return false;
}


bool Usb::writePort(unsigned char port, unsigned char value)
{
	FILE *file;
	QString filename;
	int writeValue = value;
	
	
	switch (port)
	{
		case 0:
			filename = driverDir + "/port0";
			break;
		case 1:
			filename = driverDir + "/port1";
			break;
		default:
			qDebug("Sorry, port number has to be 0 or 1, not %d!", port);
			return false;
			break;
	}
	
	
	//----------------------------
	// open device file / port
	//----------------------------
	file = fopen(filename.toAscii(),"wb");
	
	if (file == NULL)
	{
		qDebug("Error opening file (port%d).", port);
		qDebug() << "Errormsg:" << strerror(errno);
		return false;
	}
	
	
	//--------------
	// write value
	//--------------
	//rewind(port0);
	if (fprintf(file,"%d\n",writeValue) < 0)
	{
		fclose(file);
		qDebug("Error reading file (port0).");
		qDebug() << "Errormsg:" << strerror(errno);
		return false;
	}

	// okay
	//qDebug("Value written to port%d: %d", port, writeValue);
	fclose(file);
	return true;
}
