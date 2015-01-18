#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
	n = 1;

	// show MainWindow (GUI)
	ui->setupUi(this);

	// display message in GUI
	ui->textEdit->insertHtml("Scanning for serial ports... ");

	// get a list of available serial ports.
	// this is not used in the code and only for demontration.
	QList <QextPortInfo> ports = QextSerialEnumerator::getPorts();

	ui->textEdit->insertHtml("Done.<br><br>");

	foreach (QextPortInfo portInfo, ports)
	{
		// display found ports in GUI
		ui->textEdit->insertHtml(QString("<b><u>Port %1</u></b><br>").arg(n));

		showPorts(portInfo);

		// n plus 1
		n++;
	}

	ui->textEdit->insertHtml("<br><br>");

	// if a USB device is added or removed, call the Slot onPortAddedOrRemoved
	enumerator = new QextSerialEnumerator(this);
	enumerator->setUpNotifications();

	connect(enumerator, SIGNAL(deviceDiscovered(QextPortInfo)), SLOT(onPortAdded(QextPortInfo)));
	connect(enumerator, SIGNAL(deviceRemoved(QextPortInfo)), SLOT(onPortRemoved(QextPortInfo)));

	//--------------------------------------------------------------------------------------------------
	// the settings for the serial port
	// Be aware, that the Arduino has to use the same speed (9600 Baud)
	PortSettings settings = {BAUD9600, DATA_8, PAR_NONE, STOP_1, FLOW_OFF, 10}; // 10 = timeout in ms
	//--------------------------------------------------------------------------------------------------

	//--------------------------------------------------------------------------------------------------
	// the name of the serial port
	// on Windows, this would be i.e. COM5
	serialPortName = "/dev/tty.usbmodem1451";
	//--------------------------------------------------------------------------------------------------

	// create the serial port object.
	// we get the serial data on the port "event driven".
	port = new QextSerialPort(serialPortName, settings, QextSerialPort::EventDriven);

	// if data are received on the serial port, call onReadyRead
	connect(port, SIGNAL(readyRead()), SLOT(onReadyRead()));

	// initialise the serial port
	// continue only on success (true)
	if (initSerialPort() == true)
	{
		// display message in GUI
		ui->textEdit->insertHtml("<b>Sending data to Arduino in some seconds (arduinoInit)...</b><br><br>");

		// Special timer, needed for Arduino!
		//
		// Reason:
		// When the serial (USB) port is opened, the Arduino is not ready for serial communication immediately.
		// Therefore we start a timer. After 3000 ms (3 seconds), it will call the function arduinoInit().
		// This can then be used for a first command to the Arduino, like "Hey Arduino, Qt-Software now startet!".
		QTimer::singleShot(3000, this, SLOT(arduinoInit()));
	}
}


MainWindow::~MainWindow()
{
	delete ui;
	delete port;
}


void MainWindow::arduinoInit()
{
	// send values to Arduino (insert your own initialisation here!)
	sendValue('*');
	sendValue('r');
	sendValue('e');
	sendValue('#');

	ui->textEdit->insertHtml("<br><b>Waiting for Arduino answer...</b><br><br>");
}


bool MainWindow::initSerialPort(void)
{
	// open the serial port
	port->open(QIODevice::ReadWrite | QIODevice::Unbuffered);

	// error opening port
	if (port->isOpen() == false)
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
		ui->textEdit->insertHtml(QString("%1 byte(s) written. Written value: %2 (DEC), %3 (HEX), %4 (ASCII)<br>").arg(bw).arg(value).arg(value, 0, 16).arg(QChar(value)));

		// flush serial port
		port->flush();
	}
}


void MainWindow::onReadyRead()
{
	QString receivedData; // the data received from the serial port
//	int receivedInt = 0;
	qint64 ba = 0; // bytes available


	// how many bytes are available?
	ba = port->bytesAvailable();

	// if data available (should _always_ be the case, since this method is called automatically by an event)
	if (ba > 0)
	{
		// read data and convert them to a QString
//		receivedInt = (int) port->readAll();
		receivedData = (QString::fromLatin1( port->readAll() ));

		// show received data
		ui->textEdit->insertHtml(QString("%1 byte(s) received. Received data: %2 (ASCII)<br>").arg(ba).arg(receivedData));
//		ui->textEdit->insertHtml(QString("%1 byte(s) received. Written value: %2 (DEC), %3 (HEX), %4 (ASCII)<br>").arg(ba).arg(value).arg(value, 0, 16).arg(receivedData));
	}
}


void MainWindow::onPortAdded(QextPortInfo newPortInfo)
{
	// scroll to end
	ui->textEdit->ensureCursorVisible();
	ui->textEdit->insertHtml("<br><b><i>Serial port added!</i></b><br>");

	// show ports, with physical name
	showPorts(newPortInfo, true);
}


void MainWindow::onPortRemoved(QextPortInfo newPortInfo)
{
	// scroll to end
	ui->textEdit->ensureCursorVisible();
	ui->textEdit->insertHtml("<br><b><i>Serial port removed!</i></b><br>");

	// show ports
	showPorts(newPortInfo);
}


void MainWindow::showPorts(QextPortInfo portInfos, bool added)
{
	ui->textEdit->insertHtml(QString("<b>Port name:</b> %1<br>").arg(portInfos.portName));

	if (added)
	{
		ui->textEdit->insertHtml(QString("<b>Physical name:</b> %1<br>").arg(portInfos.physName));
	}

	ui->textEdit->insertHtml(QString("<b>Friendly name:</b> %1<br>").arg(portInfos.friendName));
	ui->textEdit->insertHtml(QString("<b>Enumerator name:</b> %1<br>").arg(portInfos.enumName));
	ui->textEdit->insertHtml(QString("<b>Vendor ID:</b> %1<br>").arg(portInfos.vendorID));
	ui->textEdit->insertHtml(QString("<b>Product ID:</b> %1<br>").arg(portInfos.productID));
	ui->textEdit->insertHtml("<br>");

	// scroll to end
	ui->textEdit->ensureCursorVisible();
}
