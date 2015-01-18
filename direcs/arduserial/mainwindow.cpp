#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
	n = 1;

	// show MainWindow (GUI)
	ui->setupUi(this);

	// display message in GUI
	ui->textEdit->insertPlainText("Looking for serial ports...\n\n");

	// get a list of available serial ports.
	// this is not used in the code and only for demontration.
	QList <QextPortInfo> ports = QextSerialEnumerator::getPorts();

	foreach (QextPortInfo portInfo, ports)
	{
		// display found ports in GUI
		ui->textEdit->insertHtml(QString("<b>%1. port name:</b> %2<br>").arg(n).arg(portInfo.portName));
		ui->textEdit->insertHtml(QString("<b>%1. physical name:</b> %2<br>").arg(n).arg(portInfo.physName));
		ui->textEdit->insertHtml(QString("<b>%1. friendly name:</b> %2<br>").arg(n).arg(portInfo.friendName));
		ui->textEdit->insertHtml(QString("<b>%1. enumerator name:</b> %2<br>").arg(n).arg(portInfo.enumName));
		ui->textEdit->insertHtml(QString("<b>%1. vendor ID:</b> %2<br>").arg(n).arg(portInfo.vendorID));
		ui->textEdit->insertHtml(QString("<b>%1. product ID:</b> %2<br>").arg(n).arg(portInfo.productID));
		ui->textEdit->insertHtml("<br>");

		// n plus 1
		n++;
	}

	ui->textEdit->insertPlainText("\n");

	// if a USB device is added or removed, call the Slot onPortAddedOrRemoved
	enumerator = new QextSerialEnumerator(this);
	enumerator->setUpNotifications();

	connect(enumerator, SIGNAL(deviceDiscovered(QextPortInfo)), SLOT(onPortAddedOrRemoved(QextPortInfo)));
	connect(enumerator, SIGNAL(deviceRemoved(QextPortInfo)), SLOT(onPortAddedOrRemoved(QextPortInfo)));

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
		// Special timer, needed for Arduino!
		//
		// Reason:
		// When the serial (USB) port is opened, the Arduino is not ready for serial communication immediately.
		// Therefore we start a timer. After 3000 ms (3 seconds), it will call the function arduinoInit().
		// This can then be used for a first command to the Arduino, like "Hey Arduino, Qt-Software now startet!".
		QTimer::singleShot(3000, this, SLOT(arduinoInit()));

		// if data are received on the serial port, call onReadyRead
		connect(port, SIGNAL(readyRead()), SLOT(onReadyRead()));
	}
}


MainWindow::~MainWindow()
{
	delete ui;
	delete port;
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
		ui->textEdit->insertHtml(QString("<b>Error opening serial port <i>%1</i>.</b><br>").arg(serialPortName));

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


void MainWindow::onPortAddedOrRemoved(QextPortInfo newPortInfo)
{
	// scroll to end
	ui->textEdit->ensureCursorVisible();

	// display found port in GUI
	ui->textEdit->insertHtml("<br><b><i>Serial port added!</i></b><br>");
	ui->textEdit->insertHtml(QString("<b>port name:</b> %1<br>").arg(newPortInfo.portName));
	ui->textEdit->insertHtml(QString("<b>physical name:</b> %1<br>").arg(newPortInfo.physName));
	ui->textEdit->insertHtml(QString("<b>friendly name:</b> %1<br>").arg(newPortInfo.friendName));
	ui->textEdit->insertHtml(QString("<b>enumerator name:</b> %1<br>").arg(newPortInfo.enumName));
	ui->textEdit->insertHtml(QString("<b>vendor ID:</b> %1<br>").arg(newPortInfo.vendorID));
	ui->textEdit->insertHtml(QString("<b>product ID:</b> %1<br>").arg(newPortInfo.productID));
	ui->textEdit->insertHtml("<br>");

	// scroll to end
	ui->textEdit->ensureCursorVisible();
}
