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
#include <QCoreApplication>
#include <qextserialport.h>
#include "speak.h"

int main(int argc, char *argv[])
{
	QCoreApplication app(argc, argv);
 
	// create Speak class object
	Speak *s = new Speak();
	Q_UNUSED (s);

	return app.exec();
}


Speak::Speak()
{
	serialPort = "/dev/ttyUSB0";
	textToSpeak = "Hello Markus";
	char c = 0;
	int i = 0;


	// modify the port settings
	// these settings are set, when the port is opened
	port = new QextSerialPort(serialPort);
	port->setBaudRate(BAUD38400);
	port->setFlowControl(FLOW_OFF);
	port->setParity(PAR_NONE);
	port->setDataBits(DATA_8);
	port->setStopBits(STOP_2);
	qDebug("isOpen : %d", port->isOpen());
	
	// Open port
	qDebug("Opening port...");
	if (port->open(QIODevice::ReadWrite) != true)
	{
		qDebug("\nError opening serial port!\n\n");
		QCoreApplication::exit(-1);
	}
	qDebug("is open: %d\n", port->isOpen());


	qDebug("Sending commands...");

	// 'speak' command
	c = 0x80;
	i = port->write(&c, 1);
	receiveMsg();

	// full volume (0 to 7)
	c = 0x00;
	i = port->write(&c, 1);
	receiveMsg();

	// speed pitch (0 to 7 (7 is the lowest))
	c = 0x07;
	i = port->write(&c, 1);
	receiveMsg();

	// speed speed (0 to 7)
	c = 0x02;
	i = port->write(&c, 1);
	receiveMsg();
	qDebug("Sent\n");
	
	//
	// sending string here ! ! !
	//
	qDebug("Sending text...");
	for (int n=0; n < textToSpeak.length() ;n++)
	{
		// get one character from the string to say
		const char *  t = textToSpeak.mid(n,1).toLatin1();
		c = *t;
		i = port->write(&c, 1);
		receiveMsg();
	}
	qDebug("Sent.\n");
	
	
	// end of transmit. start speaking!
	c = 0x00;
	i = port->write(&c, 1);
	//qDebug("%d byte transmitted: %d", i, c);
	receiveMsg();
	
	
	qDebug("Closing port...");
	port->close();
	qDebug("Closed\n");
	
	qDebug("is open: %d\n", port->isOpen());

	// clean exit
	QCoreApplication::exit(0);
}


Speak::~Speak()
{
	delete port;
	port = NULL;
}


void Speak::receiveMsg()
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
