#ifndef NETWORKTHREAD_H
#define NETWORKTHREAD_H

//-------------------------------------------------------------------
#include <QThread>
#include <QtNetwork>
//-------------------------------------------------------------------

class QUdpSocket;

/**
\brief Handles the network access (e.g. WLAN) to the robot.

It opens an udpSocket and listens on a port. When data are received, a signal \e dataReceived is emitted.
*/
// TODO: Change this to a non-thread! Is the a possibility to "disconnect" oder just no "emit"?
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
		
		//void sendNetworkCommand();


	signals:
		/**
		*/
		void dataReceived(QString text);


	private slots:
		void processPendingDatagrams();


	private:
		//mutable QMutex mutex; // make this class thread-safe
		QUdpSocket *udpSocket;
		volatile bool stopped;
		
		// Every thread sleeps some time, for having a bit more time fo the other threads!
		// Time in milliseconds
		static const unsigned long THREADSLEEPTIME = 1000; // Default: 25 ms  (old: 5 ms)

		static const int PORT = 45454;
};

#endif
