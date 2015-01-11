#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	init_port(); //initiation of the port

//	connect(ui->dial, SIGNAL(sliderMoved(int)), this, SLOT(transmitCmd(int)));

	// On connecte le mouvement du bouton a l'envoi de la commande
//    connect(ui->slider, SIGNAL(sliderMoved(int)), this, SLOT(transmitCmd(int)));

	// On connecte le mouvement du slider a l'envoi de la commande

	QTimer::singleShot(3000, this, SLOT(arduinoInit()));
}


MainWindow::~MainWindow()
{
	delete ui;
}


void MainWindow::arduinoInit()
{
	transmitCmd('*');
	transmitCmd('r');
	transmitCmd('e');
	transmitCmd('#');
}


void MainWindow::init_port(void)
{
	QString dev_port = "/dev/tty.usbmodem1451";
	port = new QextSerialPort(dev_port); //on ouvre le port
	// ls /dev/ | grep USB   << Permet de donner la liste des périphériques USB

	port->open(QIODevice::ReadWrite | QIODevice::Unbuffered);

	if(!port->isOpen())
	{
		QMessageBox::warning(this, "Impossible d'ouvrir le port", dev_port);
	}

	// On parametre la liaison :
	port->setBaudRate(BAUD9600);
	port->setFlowControl(FLOW_OFF);
	port->setParity(PAR_NONE);
	port->setDataBits(DATA_8);
	port->setStopBits(STOP_1);
}


void MainWindow::transmitCmd(int value)
{
	QByteArray byte; //byte a envoyer
	qint64 bw = 0; //bytes really writen

	byte.clear(); // on efface le contenu de byte
	byte.append(value); // on ajoute "value" a byte


	if (port != NULL)
	{ // on vérifie que le port existe toujours
		bw = port->write(byte); // on écrit les byte
		//(bw récupere le nombre de byte écris)
		qDebug() << bw << "byte(s) written | Value sent:" << value << "(dec)";
		port->flush(); // on attend la fin de la transmission
	}

}
