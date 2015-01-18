#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
	// show MainWindow (GUI)
	ui->setupUi(this);

	// display message in GUI
	ui->textEdit->insertHtml("Scanning for serial ports... ");

	// get a list of available serial ports.
	// this is not used in the code and only for demontration.
	QList <QextPortInfo> ports = QextSerialEnumerator::getPorts();

	ui->textEdit->insertHtml("Done.<br><br>");

	// for displaying the number of found ports
	n = 1;

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

	// try to open Arduino serial port
	initArduino();
}


MainWindow::~MainWindow()
{
	delete ui;
	delete port;
}


void MainWindow::timerSlot()
{
	// send values to Arduino (insert your own initialisation here!)
	sendValue('*');
	sendValue('r');
	sendValue('e');
	sendValue('#');

	ui->textEdit->insertHtml("<br><b>Waiting for Arduino answer...</b><br><br>");
}


void MainWindow::initArduino()
{
	// initialise the serial port
	// continue only on success (true)
	if (openSerialPort() == true)
	{
		// display message in GUI
		ui->textEdit->insertHtml("<b>Sending data to Arduino in some seconds (arduinoInit)...</b><br><br>");

		// Special timer, needed for Arduino!
		//
		// Reason:
		// When the serial (USB) port is opened, the Arduino is not ready for serial communication immediately.
		// Therefore we start a timer. After 3000 ms (3 seconds), it will call the function arduinoInit().
		// This can then be used for a first command to the Arduino, like "Hey Arduino, Qt-Software now startet!".
		QTimer::singleShot(3000, this, SLOT(timerSlot()));
	}
}


bool MainWindow::openSerialPort(void)
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
		ui->textEdit->insertHtml(QString("%1 byte(s) written. Written value: %2 (DEC) / %3 (HEX) / %4 (ASCII)<br>").arg(bw).arg(value).arg(value, 0, 16).arg(QChar(value)));

		// flush serial port
		port->flush();
	}
}


void MainWindow::onReadyRead()
{
	QByteArray receivedData; // the data received from the serial port
	qint64 ba = 0; // bytes available
	QString str; // a string to show it
	QChar ch = 0; // the char of the received data
	int dec = 0; // the int of the received data


	// how many bytes are available?
	ba = port->bytesAvailable();

	// position in the string (index!)
	n = 0;

	// if data available (should _always_ be the case, since this method is called automatically by an event)
	if (ba > 0)
	{
		// read data and convert them to a QString
		receivedData = port->readAll();

		// convert from QByteArray to QString
		str = QString::fromUtf8(receivedData.constData());

		// show received data as QString
		ui->textEdit->insertHtml(QString("%1 byte(s) received. ASCII: %2<br>").arg(ba).arg(str));

		// show each byte
		while (n < receivedData.length())
		{
			// show DEC of each char
			//
			// convert one byte to QChar
			ch = receivedData.at(n);

			// convert to int
			dec = (int) ch.toAscii();

			// show in GUI
			ui->textEdit->insertHtml(QString("Byte No.%1: %2 (DEC)<br>").arg(n).arg(dec));

			// counter +1
			n++;
		}
	}
}


void MainWindow::onPortAdded(QextPortInfo newPortInfo)
{
	// get part of string
	// (i.e. looking only for the "usbmodem1451" within "/dev/tty.usbmodem1451")
	QStringRef subString = serialPortName.rightRef(serialPortName.lastIndexOf("."));


	// scroll to end
	ui->textEdit->ensureCursorVisible();
	ui->textEdit->insertHtml("<br><b><i>Serial port added!</i></b><br>");

	// show ports, with physical name
	showPorts(newPortInfo, true);

	// Wanted serial port (Arduino) found!
	if (newPortInfo.portName.contains(subString))
	{
		// show success message!
		ui->textEdit->insertHtml("+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++<br>");
		ui->textEdit->insertHtml(QString("+++ Yeah, Arduino '%1' found! +++<br>").arg(subString.toString()));
		ui->textEdit->insertHtml("+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++<br><br>");

		// try to open Arduino serial port
		initArduino();
	}
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
