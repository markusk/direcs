#include "gui.h"


Gui::Gui(MrsRemote *m, QMainWindow *parent) : QMainWindow(parent)
{
	mrsremote1 = m;
	
	robotDrives = false;

	//-------------------------------------------------------
	// startup the GUI
	//-------------------------------------------------------
	ui.setupUi(this);
	
	// get the motor speed from the GUI
	speedMotor1 = ui.spinBoxSpeed1->value();
	speedMotor2 = ui.spinBoxSpeed2->value();
	
	view = new QWebView(ui.widgetWeb);
	//view->load(QUrl("http://mrs:8000/"));
	view->load(QUrl("http://mrs/"));
	view->show();
	
	/*
	media = new MediaObject(ui.widgetWeb);
	vwidget = new VideoWidget(ui.widgetWeb);
	Phonon::createPath(media, vwidget)
	media->enqueue("/home/markus/media/mp3/Classic/klaus badelt - pirates of the caribbean - 15 - he's a pirate.mp3");
	*/
}


Gui::~Gui()
{
	emit commandIssued("shutdown");
	delete view;
	//delete media;
	//delete vwidget;
	
	delete mrsremote1;
}


int Gui::getPort()
{
	return ui.lineEditPortValue->text().toInt();
}


void Gui::on_btnForward_clicked()
{
	robotDrives = true;
	ui.btnStartStop->setIcon(QIcon(":/images/images/stop.png"));
	emit commandIssued("forward");
}


void Gui::on_btnBackward_clicked()
{
	robotDrives = true;
	ui.btnStartStop->setIcon(QIcon(":/images/images/stop.png"));
	emit commandIssued("backward");
}


void Gui::on_btnLeft_clicked()
{
	robotDrives = true;
	ui.btnStartStop->setIcon(QIcon(":/images/images/stop.png"));
	emit commandIssued("left");
}


void Gui::on_btnRight_clicked()
{
	robotDrives = true;
	ui.btnStartStop->setIcon(QIcon(":/images/images/stop.png"));
	emit commandIssued("right");
}


void Gui::on_btnStartStop_clicked()
{
	if (robotDrives == false)
	{
		// start driving
		robotDrives = true;
		ui.btnStartStop->setIcon(QIcon(":/images/images/stop.png"));
		emit commandIssued("start");
	}
	else
	{
		// stop driving
		emit commandIssued("stop");
		ui.btnStartStop->setIcon(QIcon(":/images/images/underFootOne.png"));
		robotDrives = false;
	}
}


void Gui::on_spinBoxSpeed1_valueChanged(int value)
{
	if (value > speedMotor1)
	{
		//
		// increase speed
		//
		
		// store the new speed
		speedMotor1 = value;
		emit commandIssued("increasespeedmotor1");
	}
	else
	{
		//
		// reduce speed
		//
		
		// store the new speed
		speedMotor1 = value;
		emit commandIssued("reducespeedmotor1");
	}
}


void Gui::on_spinBoxSpeed2_valueChanged(int value)
{
	if (value > speedMotor2)
	{
		//
		// increase speed
		//
		
		// store the new speed
		speedMotor2 = value;
		emit commandIssued("increasespeedmotor2");
	}
	else
	{
		//
		// reduce speed
		//
		
		// store the new speed
		speedMotor2 = value;
		emit commandIssued("reducespeedmotor2");
	}
}


void Gui::on_btnExit_clicked()
{
	close();
}


void Gui::closeEvent()
{
	mrsremote1->~MrsRemote();
}


void Gui::appendLog(QString text, bool sayIt)
{
	// insert the text
	ui.textEdit1->insertHtml(text);

	// insert a line break
	ui.textEdit1->insertHtml("<br>");
	
	// Ensures that the cursor is visible by scrolling the text edit if necessary.
	ui.textEdit1->ensureCursorVisible(); 


	// say the message via MS speech.lib
	if (sayIt == true)
	{
	}
}


void Gui::on_btnClear_clicked()
{
	ui.textEdit1->clear();
}


void Gui::on_lblLink_linkActivated(QString link)
{
	appendLog("Link funktioniert");
	QMessageBox::information(0, "test", "Link clicked! :-)", QMessageBox::Ok, QMessageBox::NoButton);
}


void Gui::on_lblLink_linkHovered(QString link)
{
	appendLog("Linkfunktioniert");
	QMessageBox::information(0, "test", "Link hovered! :-)", QMessageBox::Ok, QMessageBox::NoButton);
}


void Gui::on_btnTest_clicked()
{
	emit commandIssued("Hello world! :-)");
}
