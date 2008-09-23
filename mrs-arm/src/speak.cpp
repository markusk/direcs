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
//#include <QApplication>
#include <QCoreApplication>
#include <qextserialport.h>
#include "speak.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
 
	// create MrsRemote class object
	Speak *s = new Speak();

	return app.exec();
}


void Speak::Speak()
{
	unsigned char text[] = {'H','e','l','l','o',' ','M','a','r','k','u','s','@'};
	unsigned char c = 250; // INIT
	int i = 0;


	//modify the port settings on your own
	port = new QextSerialPort("/dev/ttyUSB0");
	port->setBaudRate(BAUD38400);   
	port->setFlowControl(FLOW_OFF);
	port->setParity(PAR_NONE);    
	port->setDataBits(DATA_8);   
	port->setStopBits(STOP_2);    
	qDebug("isOpen : %d", port->isOpen());
	
	// Open port
	qDebug("Opening port...");
	port->open(QIODevice::ReadWrite);
	qDebug("is open: %d\n", port->isOpen());


	printf("Sending commands...");

	c = 0x80; // 'speak' command
	i = port->write(&c, 1);
	qDebug("%d byte transmitted: %d", i, c);
	
	// now waitung for ACK!!
	receiveMsg();
	while ( read_port(dev_fd, &c, 1) < 1 )
	{
	}

	c = 0x00; // full volume (0 to 7)
	write_port(dev_fd, &c, 1);
	while ( read_port(dev_fd, &c, 1) < 1 )
	{
	}

	c = 0x07; // speed pitch (0 to 7 (7 is the lowest))
	write_port(dev_fd, &c, 1);
	while ( read_port(dev_fd, &c, 1) < 1 )
	{
	}

	c = 0x02; // speed speed (0 to 7)
	write_port(dev_fd, &c, 1);
	while ( read_port(dev_fd, &c, 1) < 1 )
	{
	}

	
	
	if (port->isOpen() > 0)
	{
		qDebug("Closing port...");
		port->close();
	}
	qDebug("is open: %d", port->isOpen());
}


void Speak::~Speak()
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
		if(numBytes > 1024) numBytes = 1024;
	
		int i = port->read(buff, numBytes);
		buff[i] = '\0';
		QString msg = buff;
	
		received_msg->append(msg);
		received_msg->ensureCursorVisible();
		qDebug("bytes available: %d\n", numBytes);
		qDebug("received: %d\n", i);
	}
}


