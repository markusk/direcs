/*************************************************************************
 *   Copyright (C) 2009 by Markus Knapp                                  *
 *   www.direcs.de                                                       *
 *                                                                       *
 *   This file is part of direcs.                                        *
 *                                                                       *
 *   direcs is free software: you can redistribute it and/or modify it   *
 *   under the terms of the GNU General Public License as published      *
 *   by the Free Software Foundation, version 3 of the License.          *
 *                                                                       *
 *   direcs is distributed in the hope that it will be useful,           *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of      *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the        *
 *   GNU General Public License for more details.                        *
 *                                                                       *
 *   You should have received a copy of the GNU General Public License   *
 *   along with direcs. If not, see <http://www.gnu.org/licenses/>.      *
 *                                                                       *
 *************************************************************************/

#include "networkThread.h"

NetworkThread::NetworkThread()
{
	stopped = false;
	
	udpSocket = new QUdpSocket(this);
	udpSocket->bind(PORT);

	connect(udpSocket, SIGNAL(readyRead()), this, SLOT(processPendingDatagrams()));
}


NetworkThread::~NetworkThread()
{
	delete udpSocket;
}


void NetworkThread::stop()
{
	stopped = true;
}


void NetworkThread::run()
{
	//------------------------------------------------------------
	// description
	//------------------------------------------------------------
	
	//
	//  start "threading"...
	//
	
	emit dataReceived(tr("Listening on port %1...").arg(PORT));
	
	while (!stopped)
	{
		
		// let the thread sleep some time
		// for having more time for the other threads
		msleep(THREADSLEEPTIME);
		
		//
		// do somethin...
		//
		
		//====================================================================
		//  e m i t  Signal
		//====================================================================
		//emit sensorDataComplete();
	}
	stopped = false;
}


void NetworkThread::processPendingDatagrams()
{
	while (udpSocket->hasPendingDatagrams())
	{
		QByteArray datagram;
		datagram.resize(udpSocket->pendingDatagramSize());
		udpSocket->readDatagram(datagram.data(), datagram.size());
		
		//====================================================================
		//  e m i t  Signal
		//====================================================================
		emit dataReceived(tr("%1").arg(datagram.data()));
	}
}

/*
void NetworkThread::sendNetworkCommand()
{
	QString command = "command";
	emit dataReceived(tr("%1").arg(command));
	
	QByteArray datagram = command.toAscii();
	udpSocket->writeDatagram(datagram.data(), datagram.size(), QHostAddress::Broadcast, PORT);
}
*/
