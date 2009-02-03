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
	
	//----------------------------------------------------------------------------
	// Plot stuff
	//----------------------------------------------------------------------------
	initPlots();
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


void Gui::setPlotData1(double *xval, double *yval, int size)
{
	//---------------
	// curve1
	//---------------

	// set curve with data
	curve1.setData(xval, yval, size);

	// attach data to curve
	curve1.attach(ui.qwtPlotCurrent1);

	// after changing the values, replot the curve
	ui.qwtPlotCurrent1->replot();
}


void Gui::setPlotData2(double *xval, double *yval, int size)
{
	//---------------
	// curve2
	//---------------

	// set curve with data
	curve2.setData(xval, yval, size);

	// attach data to curve
	curve2.attach(ui.qwtPlotCurrent1); // add curve to qwtPlot 1 !!

	// after changing the values, replot the curve
	ui.qwtPlotCurrent1->replot(); // replot qwtPlot 1 !!
}


void Gui::setPlotData3(double *xval, double *yval, int size)
{
	//---------------
	// curve3
	//---------------

	// set curve with data
	curve3.setData(xval, yval, size);

	// attach data to curve
	curve3.attach(ui.qwtPlotCurrent2); // curve 3 to qwtPlot 2

	// after changing the values, replot the curve
	ui.qwtPlotCurrent2->replot();
}


void Gui::setPlotData4(double *xval, double *yval, int size)
{
	//---------------
	// curve4
	//---------------

	// set curve with data
	curve4.setData(xval, yval, size);

	// attach data to curve
	curve4.attach(ui.qwtPlotCurrent2); // curve 4 to qwtPlot 2

	// after changing the values, replot the curve
	ui.qwtPlotCurrent2->replot();  // replot qwtPlot 2 !!
}


void Gui::initPlots()
{
	//--------------------------------------
	// plot curve "MOTOR CURRENT" 1 + 2
	//--------------------------------------
	ui.qwtPlotCurrent1->setTitle("Motor 1 + 2");

	// Set axis titles
	//ui.qwtPlotCurrent1->setAxisTitle(QwtPlot::xBottom, "Time/s");
	//ui.qwtPlotCurrent1->setAxisTitle(QwtPlot::yLeft, "Current/mA");

	// Set axis scale (instead of using autoscale, which is default)
	// time
	ui.qwtPlotCurrent1->setAxisScale(QwtPlot::xBottom, 0, 60.0, 10);
	// Ampere (5000 mA, Step 500)
	ui.qwtPlotCurrent1->setAxisScale(QwtPlot::yLeft,   0, AMPERESMAXPLOTCURVE1, AMPERESSTEPPLOTCURVE1);

	QColor col = labelFillColorBlue;
	curve1.setRenderHint(QwtPlotItem::RenderAntialiased);
	curve1.setPen(QPen(col));
	curve1.setBrush(col);


	//--------------------------------------
	// plot curve "MOTOR CURRENT" 3 + 4
	//--------------------------------------
	ui.qwtPlotCurrent2->setTitle("Motor 3 + 4");

	// Set axis titles
	//ui.qwtPlotCurrent2->setAxisTitle(QwtPlot::xBottom, "Time/s");
	//ui.qwtPlotCurrent2->setAxisTitle(QwtPlot::yLeft, "Current/mA");

	// Set axis scale (instead of using autoscale, which is default)
	// time
	ui.qwtPlotCurrent2->setAxisScale(QwtPlot::xBottom, 0, 60.0, 10);
	// Ampere (5000 mA, Step 500)
	ui.qwtPlotCurrent2->setAxisScale(QwtPlot::yLeft,   0, AMPERESMAXPLOTCURVE2, AMPERESSTEPPLOTCURVE2);

	col = labelFillColorRed;
	curve2.setRenderHint(QwtPlotItem::RenderAntialiased);
	curve2.setPen(QPen(col));
	curve2.setBrush(col);


	//--------------------------------------
	// plot curve "MOTOR CURRENT" 3
	//--------------------------------------
	/*
	ui.qwtPlotCurrent3->setTitle("Motor 3");

	// Set axis titles
	//ui.qwtPlotCurrent3->setAxisTitle(QwtPlot::xBottom, "Time/s");
	//ui.qwtPlotCurrent3->setAxisTitle(QwtPlot::yLeft, "Current/mA");

	// Set axis scale (instead of using autoscale, which is default)
	// time
	ui.qwtPlotCurrent3->setAxisScale(QwtPlot::xBottom, 0, 60.0, 10);
	// Ampere (1000 mA, Step 200)
	//ui.qwtPlotCurrent2->setAxisScale(QwtPlot::yLeft,   0, 4000.0, 400);
	*/
	col = labelFillColorBlue;
	curve3.setRenderHint(QwtPlotItem::RenderAntialiased);
	curve3.setPen(QPen(col));
	curve3.setBrush(col);


	//--------------------------------------
	// plot curve "MOTOR CURRENT" 4
	//--------------------------------------
	/*
	ui.qwtPlotCurrent4->setTitle("Motor 4");

	// Set axis titles
	//ui.qwtPlotCurrent4->setAxisTitle(QwtPlot::xBottom, "Time/s");
	//ui.qwtPlotCurrent4->setAxisTitle(QwtPlot::yLeft, "Current/mA");

	// Set axis scale (instead of using autoscale, which is default)
	// time
	ui.qwtPlotCurrent4->setAxisScale(QwtPlot::xBottom, 0, 60.0, 10);
	// Ampere (1000 mA, Step 200)
	//ui.qwtPlotCurrent2->setAxisScale(QwtPlot::yLeft,   0, 4000.0, 400);
	*/
	col = labelFillColorRed;
	curve4.setRenderHint(QwtPlotItem::RenderAntialiased);
	curve4.setPen(QPen(col));
	curve4.setBrush(col);
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
