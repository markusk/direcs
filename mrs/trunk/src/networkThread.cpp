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
