/***************************************************************************
 *   Copyright (C) 2008 by Markus Knapp                                    *
 *   mrs@direcs.de                                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include "mrs-arm.h"

int main(int argc, char *argv[])
{
	QCoreApplication app(argc, argv);
 
	// create mrs-arm class object
	Mrsarm *s = new Mrsarm();
	Q_UNUSED (s);

	return app.exec();
}


Mrsarm::Mrsarm()
{
	serialPort = "/dev/ttyUSB0";


	if (!openSerialPort())
	{
		qDebug("\nError opening serial port!\n\n");
		return;
	}
	
	speak("All systems activated.");
	
	QCoreApplication::exit(0);
}


Mrsarm::~Mrsarm()
{
	closeSerialPort();
	
	delete port;
	port = NULL;
}


void Mrsarm::speak(QString textToSpeak, char volume, char pitch, char speed)
{
	char c = 0;


	qDebug("Sending commands...");

	// 'speak' command
	c = 0x80;
	if (port->write(&c, 1) == -1)
	{
		// ERROR!
		qDebug("\nError writing to serial port!\n");
		return;
	}
	receiveSerialMsg();

	
	// full volume (0 to 7)
	c = volume;
	port->write(&c, 1);
	receiveSerialMsg();

	// speed pitch (0 to 7 (7 is the lowest))
	c = pitch;
	port->write(&c, 1);
	receiveSerialMsg();

	// speed speed (0 to 7)
	c = speed;
	port->write(&c, 1);
	receiveSerialMsg();
	qDebug("Sent");
	

	qDebug("Sending ASCII text to speak...");
	for (int n=0; n < textToSpeak.length() ;n++)
	{
		// get one character from the string to say
		const char *  t = textToSpeak.mid(n,1).toLatin1();
		c = *t;
		port->write(&c, 1);
		receiveSerialMsg();
	}
	qDebug("Sent.");
	
	
	// end of transmit. start mrs-arming!
	c = 0x00;
	port->write(&c, 1);
	//qDebug("%d byte transmitted: %d", i, c);
	receiveSerialMsg();
	qDebug("Spoken.");
}


bool Mrsarm::openSerialPort()
{
	// modify the serial port settings
	port = new QextSerialPort(serialPort);
	port->setBaudRate(BAUD38400);
	port->setFlowControl(FLOW_OFF);
	port->setParity(PAR_NONE);
	port->setDataBits(DATA_8);
	port->setStopBits(STOP_2);
	qDebug("isOpen : %d", port->isOpen());
	
	// Open port
	qDebug("Opening port...");
	
	return port->open(QIODevice::ReadWrite);
	qDebug("is open: %d\n", port->isOpen());
}


void Mrsarm::closeSerialPort()
{
	if (port->isOpen() != 0)
	{
		qDebug("Closing port...");
		port->close();
		qDebug("Closed\n");
	}
		
	qDebug("is open: %d\n", port->isOpen());
}


void Mrsarm::receiveSerialMsg()
{
	char buff[1024];
	int numBytes;
 
	numBytes = port->bytesAvailable();
	if(numBytes > 0) 
	{
		if(numBytes > 1024)
			numBytes = 1024;
	
		int i = port->read(buff, numBytes);
		buff[i] = '\0';
		//QString msg = buff;
	
		//qDebug("bytes available: %d\n", numBytes);
		//qDebug("received: %d\n", i);
	}
}
