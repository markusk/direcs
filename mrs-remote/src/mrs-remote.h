#ifndef MRSREMOTE_H
#define MRSREMOTE_H

//-------------------------------------------------------------------
#include "gui.h"

#include <QtGui>
#include <QtNetwork>
//-------------------------------------------------------------------

// forward declarations because of circular includes!
class Gui;
class QUdpSocket;


class MrsRemote : public QObject
{
    Q_OBJECT


	public:
		MrsRemote();
		~MrsRemote();
		void testPort(int function);
		
	public slots:
		void sendNetworkCommand(QString command);


	private:
		Gui *gui1;
		QUdpSocket *udpSocket;
};

#endif
