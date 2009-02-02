#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "direcs-remote.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
 
	// create DirecsRemote class object
	DirecsRemote *m = new DirecsRemote();

	return app.exec();
}


DirecsRemote::DirecsRemote()
{
	//------------------------------------------------------------------
	// create the objects
	//------------------------------------------------------------------
	gui = new Gui(this);
	udpSocket = new QUdpSocket(this);
	
	udpSocketReceiver = new QUdpSocket(this);
	udpSocketReceiver->bind( gui->getPort() + 1 );

	// do something with the network received data, when complete
	connect(udpSocketReceiver, SIGNAL(readyRead()), this, SLOT(processPendingDatagrams()));
	
	// show received
	connect(this, SIGNAL(showMotorCurrent(int, int)), gui, SLOT(showMotorCurrent(int, int)));
	
	//------------------------------------------------------------------
	// for getting the screen resolution
	//------------------------------------------------------------------
	QDesktopWidget *desktop = QApplication::desktop();

	//------------------------------------------------------------------
	// place gui window at a nice position on the screen
	//------------------------------------------------------------------
	if (desktop->width() > 1024)
	{
		// move mainWindow to the center of the screen
		gui->move( (desktop->width() - gui->width())/2, (desktop->height() - gui->height())/2 );

		// show the main window
		gui->show();
	}
	else
	{
		// resolution too smal for this window. Maximizing...
		// show the main window
		gui->showMaximized();
	}
	
	
	//----------------------------------------------------------------------------
	// connect networkThread signal to "dataReceived"
	// (Whenever data were received, the data are shown in the GUI)
	//----------------------------------------------------------------------------
	connect(gui, SIGNAL( commandIssued(QString) ), this, SLOT( sendNetworkCommand(QString) ));
}


DirecsRemote::~DirecsRemote()
{
	//QMessageBox::information(0,"test","look");

	//--------------------------------------------------
	// clean up in reverse order (except from the gui)
	//--------------------------------------------------
	delete udpSocketReceiver;
	delete udpSocket;
	delete gui;
}


void DirecsRemote::processPendingDatagrams()
{
	while (udpSocketReceiver->hasPendingDatagrams())
	{
		QByteArray datagram;
		datagram.resize(udpSocketReceiver->pendingDatagramSize());
		udpSocketReceiver->readDatagram(datagram.data(), datagram.size());
		
		//====================================================================
		//  e m i t  Signal
		//====================================================================
		// emit dataReceived(tr("%1").arg(datagram.data()));
		
		// show received datagram in the gui log
		QString text = datagram.data();
		
		// sensor data received
		if (text.startsWith("*") )
		{
			parseNetworkString(text);
		}
		else
		{
			// other text received
			gui->appendLog( text );
		}
	}
}


void DirecsRemote::testPort(int function)
{
}


void DirecsRemote::sendNetworkCommand(QString command)
{
	gui->appendLog(command);
	
	QByteArray datagram = command.toAscii();
	udpSocket->writeDatagram(datagram.data(), datagram.size(), QHostAddress::Broadcast, gui->getPort());
}


void DirecsRemote::parseNetworkString(QString text)
{
	int value = 99;
	
	if ( text.contains("m") )
	{
		QString text2 = text.mid( text.indexOf("m")+1, text.indexOf("#")-1 );
		gui->appendLog( text2 );
		
		emit ( showMotorCurrent(MOTORSENSOR1, value) );
	}
}
