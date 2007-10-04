#ifndef NETWORKTHREAD_H
#define NETWORKTHREAD_H

//-------------------------------------------------------------------
#include <QThread>
#include <QtNetwork>
//-------------------------------------------------------------------

class QUdpSocket;


class NetworkThread : public QThread
{
    Q_OBJECT

	public:
		NetworkThread();
		~NetworkThread();
		void stop();
		virtual void run();
		void sendNetworkCommand();


	signals:
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
