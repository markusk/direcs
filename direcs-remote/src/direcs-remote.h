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


	private:
		Gui *gui1;
		QUdpSocket *udpSocket;
};

#endif
