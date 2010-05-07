/*************************************************************************
 *   Copyright (C) 2010 by Markus Knapp                                  *
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

#ifndef NETWORKTHREAD_H
#define NETWORKTHREAD_H

//-------------------------------------------------------------------
#include <QThread>
#include <QtNetwork>
//-------------------------------------------------------------------

class QUdpSocket;

/**
\brief Handles the network access (e.g. WLAN) to the robot.

It opens an udpSocket and listens on a port. When data are received, the signal \e dataReceived is emitted.
*/
class NetworkThread : public QThread
{
    Q_OBJECT

	public:
		NetworkThread();
		~NetworkThread();
		
		/**
		Stops the thread.
		*/
		void stop();
		
		/**
		Starts the thread.
		*/
		virtual void run();
		
		/**
		Sets the network port
		*/
		void setPort(unsigned int port);
	
	
	public slots:
		/**
		Sends a string over the network
		*/
		void sendNetworkCommand(QString text);


	signals:
		/**
		This signal emits when data were received over the network.
		*/
		void dataReceived(QString text);


	private slots:
		/**
		This slot is automatically called by the framework, in the case of pending datagrams.
		*/
		void processPendingDatagrams();


	private:
		QUdpSocket *udpSocket;
		volatile bool stopped;
		unsigned int networkPort;
		
		// Every thread sleeps some time, for having a bit more time fo the other threads!
		// Time in milliseconds
		static const unsigned long THREADSLEEPTIME = 1000; // Default: 25 ms  (old: 5 ms)
};

#endif
