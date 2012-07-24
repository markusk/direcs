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

#ifndef NETWORKTHREAD_H
#define NETWORKTHREAD_H

//-------------------------------------------------------------------
#include <QThread>
#include <QtNetwork>
//-------------------------------------------------------------------

class QUdpSocket;

/**
\author Markus Knapp
\brief Handles the network access to the robot (e.g. via WLAN).

It opens an UDP socket and listens on a port. When data are received, the signal \e dataReceived is emitted.
UDP  used, since the sending of the data does not need to be very reliable.
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
		Sets the network ports
		@param portListen is the network port where the class receives data.
		@param portSend is the network port where the class sends data.
		@return true on success, false on error
		*/
		bool init(unsigned int portListen, unsigned int portSend);

		/**
		Swaps the sender and listener port. This is at the beginning, when we have two instances of this program in one network
		*/
		void swapPorts();


	public slots:
		/**
		Sends a string over the network
		*/
		void sendNetworkCommand(QString text);

		/**
		Enables sending the 'master' signal over network every second
		*/
		void setNetworkMaster();

		/**
		Setting the network 'slave' state.
		*/
		void setNetworkSlave();


	signals:
		/**
		This signal emits when data were received over the network.
		*/
		void dataReceived(QString text);

		/**
		This signal is emitted every MSLEEP seconds to show that we are sending a master signal. @sa Gui::setLEDMasterSlave()
		*/
		void heartbeat(unsigned char state);


	private slots:
		/**
		This slot is automatically called by the framework, in the case of pending datagrams.
		*/
		void processPendingDatagrams();


	private:
		QUdpSocket *udpSocket;
		volatile bool stopped;
		unsigned int networkPortListen;
		unsigned int networkPortSend;
		bool iAmTheMaster; /// Indicates if this program is the master and sends data from the robot over the network.
		bool iAmTheSlave; /// Indicates if this program is the slave and receives data from the robot over the network.

		// Every thread sleeps some time, for having a bit more time fo the other threads!
		// Time in milliseconds
		static const unsigned long THREADSLEEPTIME = 1000; // Default: 25 ms  (old: 5 ms)

		/**
		This is for the @sa heartbeat signal (the GUI LED colors)
		*/
		static const unsigned char LEDOFF	= 0;
		static const unsigned char RED		= 1;
		static const unsigned char GREEN	= 2;
};

#endif
