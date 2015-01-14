#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
	// show MainWindow (GUI)
	ui->setupUi(this);

	// initialise the serial port
	initSerialPort();

//	connect(ui->dial, SIGNAL(sliderMoved(int)), this, SLOT(transmitCmd(int)));

	// On connecte le mouvement du bouton a l'envoi de la commande
//    connect(ui->slider, SIGNAL(sliderMoved(int)), this, SLOT(transmitCmd(int)));

	// Special timer, needed for Arduino.
	// Reason:
	// When the serial (USB) port is opened, the Arduino is not ready for serial communication immediately.
	// Therefore we start a timer. After 3000 ms (3 seconds), it will call the function arduinoInit().
	// This can then be used for a first command to the Arduino, like "Hey Arduino, Qt-Software now startet!".
	QTimer::singleShot(3000, this, SLOT(arduinoInit()));

	// If data are received on the serial port, call onReadyRead
	connect(port, SIGNAL(readyRead()), SLOT(onReadyRead()));

	// connect(enumerator, SIGNAL(deviceDiscovered(QextPortInfo)), SLOT(onPortAddedOrRemoved()));
	// connect(enumerator, SIGNAL(deviceRemoved(QextPortInfo)), SLOT(onPortAddedOrRemoved()));
}


MainWindow::~MainWindow()
{
	delete ui;
}


void MainWindow::arduinoInit()
{
	sendValue('*');
	sendValue('r');
	sendValue('e');
	sendValue('#');
}


void MainWindow::initSerialPort(void)
{
/*
	foreach (QextPortInfo info, QextSerialEnumerator::getPorts())
		ui->portBox->addItem(info.portName);
*/

	PortSettings settings = {BAUD9600, DATA_8, PAR_NONE, STOP_1, FLOW_OFF, 10}; // 10 = timeout
	QString serialPortName = "/dev/tty.usbmodem1451";

	// Create the serial port object.
	// We get the serial data on the port "event driven".
	port = new QextSerialPort(serialPortName, settings, QextSerialPort::EventDriven);

	//
	enumerator = new QextSerialEnumerator(this);
	enumerator->setUpNotifications();

	// open the serial port
	port->open(QIODevice::ReadWrite | QIODevice::Unbuffered);

	// port not opened
	if ( !port->isOpen() )
	{
		QMessageBox::warning(this, "Unable to open the serial port.", serialPortName);
	}

/*
	// On parametre la liaison :
	port->setBaudRate(BAUD9600);
	port->setFlowControl(FLOW_OFF);
	port->setParity(PAR_NONE);
	port->setDataBits(DATA_8);
	port->setStopBits(STOP_1);
*/
}


void MainWindow::sendValue(int value)
{
	QByteArray byte; // byte to sent to the port
	qint64 bw = 0;   // bytes really written


	byte.clear(); // clear buffer to be sent
	byte.append(value); // fill buffer with value to be sent

	if (port != NULL)
	{
		// write byte to serial port
		bw = port->write(byte);

		// show sent data
		qDebug() << bw << "byte(s) written | Value sent:" << value << "(dec)";

		// flush serial port
		port->flush();
	}
}


void MainWindow::onReadyRead()
{
	QString receivedData; // the data received from the serial port


	// if data available (should _always_ be the case, since this method is called automatically by an event)
	if (port->bytesAvailable())
	{
		// read data and convert them to a QString
		receivedData = (QString::fromLatin1(port->readAll()));

		// show received data
		qDebug() << "Data received:" << receivedData;
	}
}
