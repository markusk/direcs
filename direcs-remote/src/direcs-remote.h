#ifndef DIRECSREMOTE_H
#define DIRECSREMOTE_H

//-------------------------------------------------------------------
#include "gui.h"

#include <QtGui>
#include <QtNetwork>
//-------------------------------------------------------------------

// forward declarations because of circular includes!
class Gui;
class QUdpSocket;


class DirecsRemote : public QObject
{
    Q_OBJECT


	public:
		DirecsRemote();
		~DirecsRemote();
		void testPort(int function);
	
	
	public slots:
		void sendNetworkCommand(QString command);
	
	
	private slots:
		/**
		This slot is automatically called by the framework, in the case of pending datagrams.
		*/
		void processPendingDatagrams();


	private:
		Gui *gui1;
		QUdpSocket *udpSocket;
		QUdpSocket *udpSocketReceiver;
};
#endif
