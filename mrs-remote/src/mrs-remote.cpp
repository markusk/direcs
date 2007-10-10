#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "mrs-remote.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
 
	// create MrsRemote class object
	MrsRemote *m = new MrsRemote();

	return app.exec();
}


MrsRemote::MrsRemote()
{
	//------------------------------------------------------------------
	// create the objects
	//------------------------------------------------------------------
	gui1 = new Gui(this);
	udpSocket = new QUdpSocket(this);
	
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
		gui1->move( (desktop->width() - gui1->width())/2, (desktop->height() - gui1->height())/2 );

		// show the main window
		gui1->show();
	}
	else
	{
		// resolution too smal for this window. Maximizing...
		// show the main window
		gui1->showMaximized();
	}
	
	
	//----------------------------------------------------------------------------
	// connect networkThread signal to "dataReceived"
	// (Whenever data were received, the data are shown in the GUI)
	//----------------------------------------------------------------------------
	connect(gui1, SIGNAL( commandIssued(QString) ), this, SLOT( sendNetworkCommand(QString) ));
}


MrsRemote::~MrsRemote()
{
	QMessageBox::information(0,"test","look");

	//--------------------------------------------------
	// clean up in reverse order (except from the gui)
	//--------------------------------------------------
	delete udpSocket;
	delete gui1;
}


void MrsRemote::testPort(int function)
{
}


void MrsRemote::sendNetworkCommand(QString command)
{
	gui1->appendLog(command);
	
	QByteArray datagram = command.toAscii();
	udpSocket->writeDatagram(datagram.data(), datagram.size(), QHostAddress::Broadcast, gui1->getPort());
}
