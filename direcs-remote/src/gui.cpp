#include "gui.h"


Gui::Gui(DirecsRemote *m, QMainWindow *parent) : QMainWindow(parent)
{
	direcsremote1 = m;
	
	robotDrives = false;

	//-------------------------------------------------------
	// startup the GUI
	//-------------------------------------------------------
	ui.setupUi(this);
	
	// get the motor speed from the GUI
	speedMotor1 = ui.spinBoxSpeed1->value();
	speedMotor2 = ui.spinBoxSpeed2->value();
	
	// set web address of the robot
	//ui.plainTextEditWebAddress->setPlainText("http://mrs:8000");
	ui.lineEditWebAddress->setText("http://nslu/");
	
	view = new QWebView(ui.widgetWeb);
	// load the URL
	view->load(QUrl(ui.lineEditWebAddress->text()));
	view->show();
	
	/*
	media = new MediaObject(ui.widgetWeb);
	vwidget = new VideoWidget(ui.widgetWeb);
	Phonon::createPath(media, vwidget)
	media->enqueue("/home/markus/media/mp3/Classic/klaus badelt - pirates of the caribbean - 15 - he's a pirate.mp3");
	*/
	
	// When the address field was leaved or enter pressed, call the "go" button slot! :-)
	connect(ui.lineEditWebAddress, SIGNAL( editingFinished() ), this, SLOT( on_btnGo_clicked() ) );
}


Gui::~Gui()
{
	emit commandIssued("shutdown");
	delete view;
	//delete media;
	//delete vwidget;
	
	delete direcsremote1;
}


void Gui::on_btnGo_clicked()
{
	// load the URL
	// using 'setUrl'instead of 'load' here! setUrl clears the view and loads the URL.
	view->setUrl(QUrl(ui.lineEditWebAddress->text()));
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


void Gui::on_btnShutdown_clicked()
{
	robotDrives = false;
	emit commandIssued("shutdown");
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
	direcsremote1->~DirecsRemote();
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


void Gui::showMotorCurrent(int motor, int value)
{
	switch (motor)
	{
		case MOTORSENSOR1:
			ui.lblMotor1mA->setText(QString("%1 mA").arg(value));
			break;
		case MOTORSENSOR2:
			ui.lblMotor1mA->setText(QString("%1 mA").arg(value));
			break;
		case MOTORSENSOR3:
			ui.lblMotor1mA->setText(QString("%1 mA").arg(value));
			break;
		case MOTORSENSOR4:
			ui.lblMotor1mA->setText(QString("%1 mA").arg(value));
			break;
	}
}
