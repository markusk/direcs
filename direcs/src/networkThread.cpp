/*************************************************************************
 *   Copyright (C) Markus Knapp                                          *
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
	networkPortListen = 0;
	networkPortSend = 0;
	iAmTheMaster = false;
	iAmTheSlave = false;

	udpSocket = new QUdpSocket(this);
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
	bool heartbeatToggle = false;


	//
	//  start "threading"...
	//
	emit dataReceived(tr("Listening on port %1...").arg(networkPortListen));

	while (!stopped)
	{

		// let the thread sleep some time (kind of timer, because we sleep for one second here)
		msleep(THREADSLEEPTIME);


		// send master signal when we are the network master
		if (iAmTheMaster)
		{
			sendNetworkCommand("*master#"); /// @todo add *master# string to ini-file

			// send an optical heartbeat signal to the GUI
			if (heartbeatToggle)
			{
				emit heartbeat(GREEN);
			}
			else
			{
				emit heartbeat(LEDOFF);
			}

			heartbeatToggle = !heartbeatToggle;
		}
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


void NetworkThread::sendNetworkCommand(QString text)
{
	if (networkPortSend > 0)
	{
		QByteArray datagram = text.toAscii();
		udpSocket->writeDatagram(datagram.data(), datagram.size(), QHostAddress::Broadcast, networkPortSend);
	}
	else
	{
		emit dataReceived(tr("ERROR sending network command: Network 'send' port not set! Port is still %1...").arg(networkPortSend));
	}
}


void NetworkThread::swapPorts()
{
	// first disconnect the signal!
	udpSocket->disconnect(SIGNAL(readyRead()));
	udpSocket->close();

	// swap ports
	unsigned int temp = networkPortListen;
	networkPortListen = networkPortSend;
	networkPortSend = temp;


	if (init(networkPortListen, networkPortSend) == false)
	{
		qDebug("ERROR swapping network ports.");
		return;
	}
}


bool NetworkThread::init(unsigned int portListen, unsigned int portSend)
{
	// store ports locally
	networkPortListen = portListen;
	networkPortSend   = portSend;

	// bind the socket to the given port for listening
	if (udpSocket->bind(networkPortListen) == true)
	{
		// do something with the network received data, when completely received
		connect(udpSocket, SIGNAL(readyRead()), this, SLOT(processPendingDatagrams()));
		return true;
	}

	// error
	return false;
}


void NetworkThread::setNetworkMaster()
{
	// we are master now
	// from now on we will send this over network, that we we are master
	iAmTheMaster = true;
	iAmTheSlave = false;
}


void NetworkThread::setNetworkSlave()
{
	// we are slave now
	// from now on we will receive data over the network
	iAmTheSlave = true;
	iAmTheMaster = false;
}
