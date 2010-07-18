#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "direcs-remote.h"

int main(int argc, char *argv[])
{
	Q_UNUSED(argc);
	Q_UNUSED(argv);

	// Initialize the resource file
	//Q_INIT_RESOURCE(direcs-remote);

	QApplication app(argc, argv);

	// create DirecsRemote class object
	DirecsRemote *m = new DirecsRemote();

	return app.exec();
}


DirecsRemote::DirecsRemote()
{
	QLocale::setDefault(QLocale::German);

	//------------------------------------------------------------------
	// create the objects
	//------------------------------------------------------------------
	gui = new Gui(this);
	udpSocket = new QUdpSocket(this);
	plotThread = new PlotThread();
	laserThread = new LaserThread();
	network = new Network();

	udpSocketReceiver = new QUdpSocket(this);
	udpSocketReceiver->bind( gui->getPort() + 1 );

	// do something with the network received data, when complete
	connect(udpSocketReceiver, SIGNAL(readyRead()), this, SLOT(processPendingDatagrams()));

	// show received motor current values in gui
	connect(this, SIGNAL(showMotorCurrent(int, int)), gui, SLOT(showMotorCurrent(int, int)));

	// show received voltages in gui
	connect(this, SIGNAL(showVoltage(int, float)), gui, SLOT(showVoltage(int, float)));

	// emit motor current values to plot thread
	connect(this, SIGNAL(plotValueReceived(int, int)), plotThread, SLOT(setPlotValue(int, int)));

	// emit laser scanner values to laser thread
	connect(this, SIGNAL(laserValueReceived(int, int, float)), laserThread, SLOT(setLaserValue(int, int, float)));

	//----------------------------------------------------------------------------
	// connect laserThread signal to "dataReceived"
	// (Whenever data were received, the data are shown in the GUI)
	//----------------------------------------------------------------------------
	connect(laserThread, SIGNAL( laserDataCompleteFront(float *, int *) ), gui, SLOT( refreshLaserViewFront(float *, int *) ));
	connect(laserThread, SIGNAL( laserDataCompleteRear(float *, int *) ), gui, SLOT( refreshLaserViewRear(float *, int *) ));

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

	//----------------------------------------------------------------------------
	// connect signal dataComplete from the network class to the gui slot
	// (Whenever the image is complete, the image is shown in the GUI)
	//----------------------------------------------------------------------------
	connect(network, SIGNAL( dataComplete(QImage*) ), gui, SLOT( setCamImage(QImage*) ));

	//----------------------------------------------------------------------------
	// connect signal cameraUrlChanged from the gui to the network class slot
	// (Whenever the image url is changed, the try to load the image from there
	//----------------------------------------------------------------------------
	connect(gui, SIGNAL( cameraUrlChanged(QString) ), network, SLOT ( setUrl(QString) ) );

	//-----------------------------------------------------------
	// start the plot thread ("clock" for plotting the curves)
	//-----------------------------------------------------------
	if (plotThread->isRunning() == false)
	{
		//gui->appendLog("Starting plot thread...", false);
		plotThread->start();
		//gui->appendLog("Plot thread started.");
	}

	// TODO: nice exit point and error message
	if (!QGLFormat::hasOpenGL())
	{
		qDebug("This system has no OpenGL support!");
		// TODO: exit?!?
	}

	//-----------------------------------------------------------
	// one time init for the laser view
	//-----------------------------------------------------------
	gui->initLaserView();

	//--------------------------------------------------------------
	// start the laser thread ("clock" for taking the laser values
	//--------------------------------------------------------------
	if (laserThread->isRunning() == false)
	{
		//gui->appendLog("Starting laser thread...", false);
		laserThread->start();
		//gui->appendLog("Laser thread started.");
	}

	/*
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
	*/
		// show the main window
		gui->showMaximized();
	/*
	}
	*/
}


DirecsRemote::~DirecsRemote()
{
	//--------------------------------
	// quit the laserThread
	//--------------------------------
	if (laserThread->isRunning() == true)
	{
		// my own stop routine :-)
		laserThread->stop();

		// slowing thread down
		laserThread->setPriority(QThread::IdlePriority);
		laserThread->quit();

		//-------------------------------------------
		// start measuring time for timeout ckecking
		//-------------------------------------------
		QTime t;
		t.start();
		do
		{
		} while ((laserThread->isFinished() == false) && (t.elapsed() <= 2000));

		if (laserThread->isFinished() == true)
		{
		}
		else
		{
			qDebug("Terminating laser thread because it doesn't answer...");
			laserThread->terminate();
			laserThread->wait(1000);
			qDebug("Laser thread terminated.");
		}
	}

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
	delete network;
	delete laserThread;
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
	int iValue = 0;
	float fValue = 0.0;
	int start = 0;
	int count = 0;
	bool okay = false;
	QString text2;
	/*
	// show the first 10 network strings (DEBUG)
	static int n = 0;

	if (n<10)
	{
		n++;
		qDebug() << "string=" << text;
	}
	*/


	// m = motor sensor value
	// *0m42# means 'motorsensor1 with 42 mA'
	if ( text.contains("m") )
	{
		start = text.indexOf("m") + 1;
		// string always end with a '#'
		count = text.indexOf("#") - start;
		text2 = text.mid( start, count );

		// convert value to int
		iValue = text2.toInt(&okay);

		// check which motor sensor was in string ' *0m... '
		if ( text.startsWith( QString("*%1").arg(MOTORSENSOR1) ) )
		{
// 			emit ( showMotorCurrent(MOTORSENSOR1, value) );
			emit ( plotValueReceived(MOTORSENSOR1, iValue) );
			return;
		}

		if ( text.startsWith( QString("*%1").arg(MOTORSENSOR2) ) )
		{
// 			emit ( showMotorCurrent(MOTORSENSOR2, value) );
			emit ( plotValueReceived(MOTORSENSOR2, iValue) );
			return;
		}

		if ( text.startsWith( QString("*%1").arg(MOTORSENSOR3) ) )
		{
// 			emit ( showMotorCurrent(MOTORSENSOR3, value) );
			emit ( plotValueReceived(MOTORSENSOR3, iValue) );
			return;
		}

		if ( text.startsWith( QString("*%1").arg(MOTORSENSOR4) ) )
		{
// 			emit ( showMotorCurrent(MOTORSENSOR4, value) );
			emit ( plotValueReceived(MOTORSENSOR4, iValue) );
			return;
		}

		return;
	}


	// v = voltage
	// *0v42# means 'voltage 0 with 42 Volt'
	if ( text.contains("v") )
	{
		start = text.indexOf("v") + 1;
		// string always end with a '#'
		count = text.indexOf("#") - start;
		text2 = text.mid( start, count );

		// convert value to int
		iValue = text2.toInt(&okay);

		// check which volatage was in string ' *0v... '
		if ( text.startsWith( QString("*%1").arg(0) ) )
		{
			emit ( showVoltage(0, convertToVolt(0, iValue)) );
//			emit ( plotValueReceived(0, iValue) );
			return;
		}

		// check which volatage was in string ' *0v... '
		if ( text.startsWith( QString("*%1").arg(1) ) )
		{
			emit ( showVoltage(1, convertToVolt(1, iValue)) );
//			emit ( plotValueReceived(1, iValue) );
			return;
		}

		return;
	}


	// l = laser scanner value
	// *0l23a42# means LASER1 has at angle 23 a length of 42 cm
	if ( text.contains("l") )
	{
		// get the ANGLE
		start = text.indexOf("l") + 1;
		// angle value always end with a 'a'
		count = text.indexOf("a") - start;
		text2 = text.mid( start, count );
		// convert value to int
		iValue = text2.toInt(&okay);

		// get the CENTIMETERS
		start = text.indexOf("a") + 1;
		// angle value always end with a 'a'
		count = text.indexOf("#") - start;
		text2 = text.mid( start, count );
		// convert value to ***float**
		fValue = text2.toFloat(&okay);

		// check which laser scanner was in string ' *0l... '
		if ( text.startsWith( QString("*%1").arg(LASER1) ) )
		{
			emit ( laserValueReceived(LASER1, iValue, fValue) );
			return;
		}

		if ( text.startsWith( QString("*%1").arg(LASER2) ) )
		{
			emit ( laserValueReceived(LASER2, iValue, fValue) );
			return;
		}

		return;
	}
}


float DirecsRemote::convertToVolt(int voltage, int value)
{
	switch (voltage)
	{
	case 0:
		// convert the measured value to Volt (V)
		return ( value / CONVERSIONFACTORVOLTAGESENSOR1 );
		break;
	case 1:
		// convert the measured value to Volt (V)
		return ( value / CONVERSIONFACTORVOLTAGESENSOR2 );
		break;
	}

	// wrong voltage number selected
	return -1;
}
