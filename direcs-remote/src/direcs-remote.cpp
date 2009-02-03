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
	plotThread = new PlotThread();
	
	udpSocketReceiver = new QUdpSocket(this);
	udpSocketReceiver->bind( gui->getPort() + 1 );

	// do something with the network received data, when complete
	connect(udpSocketReceiver, SIGNAL(readyRead()), this, SLOT(processPendingDatagrams()));
	
	// show received motor current values in gui
	connect(this, SIGNAL(showMotorCurrent(int, int)), gui, SLOT(showMotorCurrent(int, int)));
	// emit motor current values to plot thread
	connect(this, SIGNAL(plotValueReceived(int, int)), plotThread, SLOT(setPlotValue(int, int)));
	
	//----------------------------------------------------------------------------
	// connect plotThread signal to "setPlotData"
	// (Whenever the plot thread has new data, the data are show in the GUI)
	//----------------------------------------------------------------------------
	connect(plotThread, SIGNAL( plotDataComplete1(double *, double *, int) ), gui, SLOT( setPlotData1(double *, double *, int) ));
	connect(plotThread, SIGNAL( plotDataComplete2(double *, double *, int) ), gui, SLOT( setPlotData2(double *, double *, int) ));
	connect(plotThread, SIGNAL( plotDataComplete3(double *, double *, int) ), gui, SLOT( setPlotData3(double *, double *, int) ));
	connect(plotThread, SIGNAL( plotDataComplete4(double *, double *, int) ), gui, SLOT( setPlotData4(double *, double *, int) ));
	
	//----------------------------------------------------------------------------
	// connect networkThread signal to "dataReceived"
	// (Whenever data were received, the data are shown in the GUI)
	//----------------------------------------------------------------------------
	connect(gui, SIGNAL( commandIssued(QString) ), this, SLOT( sendNetworkCommand(QString) ));
	
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
	
	//-----------------------------------------------------------
	// start the plot thread ("clock" for plotting the curves)
	//-----------------------------------------------------------
	if (plotThread->isRunning() == false)
	{
		//gui->appendLog("Starting plot thread...", false);
		plotThread->start();
		//gui->appendLog("Plot thread started.");
	}
}


DirecsRemote::~DirecsRemote()
{
	//--------------------------------
	// quit the plotThread
	//--------------------------------
	if (plotThread->isRunning() == true)
	{
		// my own stop routine :-)
		plotThread->stop();

		// slowing thread down
		plotThread->setPriority(QThread::IdlePriority);
		plotThread->quit();

		//-------------------------------------------
		// start measuring time for timeout ckecking
		//-------------------------------------------
		QTime t;
		t.start();
		do
		{
		} while ((plotThread->isFinished() == false) && (t.elapsed() <= 2000));

		if (plotThread->isFinished() == true)
		{
		}
		else
		{
			qDebug("Terminating Plot thread because it doesn't answer...");
			plotThread->terminate();
			plotThread->wait(1000);
			qDebug("Plot thread terminated.");
		}
	}

	//--------------------------------------------------
	// clean up in reverse order (except from the gui)
	//--------------------------------------------------
	delete plotThread;
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
	int value = 0;
	bool okay = false;
	
	// m = motor sensor value
	// *0m42# means 'motorsensor1 with 42 mA'
	if ( text.contains("m") )
	{
		int start = text.indexOf("m") + 1;
		// string always end with a '#'
		int count = text.indexOf("#") - start;
		QString text2 = text.mid( start, count );
		
		// convert value to int
		value = text2.toInt(&okay);
		
		// check which motor sensor was in string ' *0m... '
		if ( text.startsWith( QString("*%1").arg(MOTORSENSOR1) ) )
		{
			emit ( showMotorCurrent(MOTORSENSOR1, value) );
			emit ( plotValueReceived(MOTORSENSOR1, value) );
		}
			
		if ( text.startsWith( QString("*%1").arg(MOTORSENSOR2) ) )
		{
			emit ( showMotorCurrent(MOTORSENSOR2, value) );
			emit ( plotValueReceived(MOTORSENSOR2, value) );
		}
			
		if ( text.startsWith( QString("*%1").arg(MOTORSENSOR3) ) )
		{
			emit ( showMotorCurrent(MOTORSENSOR3, value) );
			emit ( plotValueReceived(MOTORSENSOR3, value) );
		}
			
		if ( text.startsWith( QString("*%1").arg(MOTORSENSOR4) ) )
		{
			emit ( showMotorCurrent(MOTORSENSOR4, value) );
			emit ( plotValueReceived(MOTORSENSOR4, value) );
		}
	}
}
