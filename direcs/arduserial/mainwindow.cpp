#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
	// show MainWindow (GUI)
	ui->setupUi(this);

	// display message in GUI
	ui->plainTextEdit->insertPlainText("Looking for serial ports...\n\n");

	// get a list of available serial ports.
	// this is not used in the code and only for demontration.
	QList <QextPortInfo> ports = QextSerialEnumerator::getPorts();
	foreach (QextPortInfo portInfo, ports)
	{
		int n = 1;

		// display found ports in GUI
		ui->plainTextEdit->insertPlainText(QString("Port name:\n").append(portInfo.portName).append("\n"));
		ui->plainTextEdit->insertPlainText(QString("Physical name:\n").append(portInfo.physName).append("\n"));
		ui->plainTextEdit->insertPlainText(QString("Friendly name:\n").append(portInfo.friendName).append("\n"));
		ui->plainTextEdit->insertPlainText(QString("Enumerator name:\n").append(portInfo.enumName).append("\n"));
		ui->plainTextEdit->insertPlainText(QString("Vendor ID:\n").append(portInfo.vendorID).append("\n"));
		ui->plainTextEdit->insertPlainText(QString("Product ID:\n").append(portInfo.productID).append("\n"));
		ui->plainTextEdit->insertPlainText("\n");

		/*
		qDebug("\n");
		qDebug() << "port name:" << portInfo.portName;   /// Port name (QString)
		qDebug() << "physical name:" << portInfo.physName;   /// Physical name (QString)
		qDebug() << "friendly name:" << portInfo.friendName; /// Friendly name (QString)
		qDebug() << "enumerator name:" << portInfo.enumName;   /// Enumerator name (QString)
		qDebug() << "vendor ID:" << portInfo.vendorID;       /// Vendor ID (int)
		qDebug() << "product ID:" << portInfo.productID;      /// Product ID (int)
		qDebug("\n");
		*/
	}

	ui->plainTextEdit->insertPlainText("\n");

	// the settings for the serial port
	// Be aware, that the Arduino has to use the same speed (9600 Baud)
	PortSettings settings = {BAUD9600, DATA_8, PAR_NONE, STOP_1, FLOW_OFF, 10}; // 10 = timeout in ms

	// the name of the serial port
	// on Windows, this would be i.e. COM5
	serialPortName = "/dev/tty.usbmodem1451";

	// create the serial port object.
	// we get the serial data on the port "event driven".
	port = new QextSerialPort(serialPortName, settings, QextSerialPort::EventDriven);

	// initialise the serial port
	// continue only on success (true)
	if (initSerialPort() == true)
	{
		//	connect(ui->dial, SIGNAL(sliderMoved(int)), this, SLOT(transmitCmd(int)));

			// On connecte le mouvement du bouton a l'envoi de la commande
		//    connect(ui->slider, SIGNAL(sliderMoved(int)), this, SLOT(transmitCmd(int)));

		// Special timer, needed for Arduino!
		//
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


bool MainWindow::initSerialPort(void)
{
	// open the serial port
	port->open(QIODevice::ReadWrite | QIODevice::Unbuffered);

	// error opening port
	if ( port->isOpen() == false)
	{
		// show error message
		ui->plainTextEdit->insertPlainText(QString("ERROR OPENING SERIAL PORT ").append(serialPortName).append("\n\n"));

//		QMessageBox::warning(this, "Unable to open the serial port.", serialPortName);

		return false;
	}

	// success
	return true;
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
