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
		ui->textEdit->insertHtml(QString("<b><u>Port %1</u></b><br>").arg(n));

		showPorts(portInfo);

		// n plus 1
		n++;
	}

	ui->textEdit->insertPlainText("\n");

	// if a USB device is added or removed, call the Slot onPortAddedOrRemoved
	enumerator = new QextSerialEnumerator(this);
	enumerator->setUpNotifications();

	connect(enumerator, SIGNAL(deviceDiscovered(QextPortInfo)), SLOT(onPortAdded(QextPortInfo)));
	connect(enumerator, SIGNAL(deviceRemoved(QextPortInfo)), SLOT(onPortRemoved(QextPortInfo)));

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


void MainWindow::onPortAdded(QextPortInfo newPortInfo)
{
	// scroll to end
	ui->textEdit->ensureCursorVisible();
	ui->textEdit->insertHtml("<br><b><i>Serial port added!</i></b><br>");

	// show ports
	showPorts(newPortInfo);
}


void MainWindow::onPortRemoved(QextPortInfo newPortInfo)
{
	// scroll to end
	ui->textEdit->ensureCursorVisible();
	ui->textEdit->insertHtml("<br><b><i>Serial port removed!</i></b><br>");

	// show ports
	showPorts(newPortInfo);
}


void MainWindow::showPorts(QextPortInfo portInfos)
{
	ui->textEdit->insertHtml(QString("<b>Port name:</b> %1<br>").arg(portInfos.portName));
	ui->textEdit->insertHtml(QString("<b>Physical name:</b> %1<br>").arg(portInfos.physName));
	ui->textEdit->insertHtml(QString("<b>Friendly name:</b> %1<br>").arg(portInfos.friendName));
	ui->textEdit->insertHtml(QString("<b>Enumerator name:</b> %1<br>").arg(portInfos.enumName));
	ui->textEdit->insertHtml(QString("<b>Vendor ID:</b> %1<br>").arg(portInfos.vendorID));
	ui->textEdit->insertHtml(QString("<b>Product ID:</b> %1<br>").arg(portInfos.productID));
	ui->textEdit->insertHtml("<br>");

	// scroll to end
	ui->textEdit->ensureCursorVisible();
}
