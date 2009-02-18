#include "gui.h"


Gui::Gui(DirecsRemote *m, QMainWindow *parent) : QMainWindow(parent)
{
	direcsremote1 = m;
	
	robotDrives = false;
	laserXPos = 0; // correct value is set in the initLaserView()
	laserYPos = 0; // correct value is set in the initLaserView()
	
	// define some nice green and red colors
	labelFillColorGreen = QColor(64, 255, 64);
	labelFillColorRed   = QColor(255, 64, 64);
	labelFillColorBlue  = QColor(64, 64, 255);

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
	
	webView = new QWebView(ui.widgetWeb);
	// load the URL
	webView->load(QUrl(ui.lineEditWebAddress->text()));
	webView->show();
	
	/*
	media = new MediaObject(ui.widgetWeb);
	vwidget = new VideoWidget(ui.widgetWeb);
	Phonon::createPath(media, vwidget)
	media->enqueue("/home/markus/media/mp3/test.mp3");
	*/
	
	// When the address field was leaved or enter pressed, call the "go" button slot! :-)
	connect(ui.lineEditWebAddress, SIGNAL( editingFinished() ), this, SLOT( on_btnGo_clicked() ) );
	
	
	//----------------------------------------------------------------------------
	// Plot stuff
	//----------------------------------------------------------------------------
	initPlots();

	//----------------------------------------------------------------------------
	// Laser Scanner graphics Stuff (scene, view, lines, OpenGL etc.)
	//----------------------------------------------------------------------------
	createLaserScannerObjects();
	createLaserDistanceObjects();

	//----------------------------------------------------------------------------------
	// get the mouse position, when the robot position is to be changed
	//----------------------------------------------------------------------------------
	connect(scene, SIGNAL( robotPositionChanged(QGraphicsSceneMouseEvent *) ), this, SLOT( setRobotPosition(QGraphicsSceneMouseEvent *) ));
	connect(scene, SIGNAL( wheelZoom(QGraphicsSceneWheelEvent *) ), this, SLOT( zoomLaserView(QGraphicsSceneWheelEvent *) ));
}


Gui::~Gui()
{
	emit commandIssued("shutdown");
	delete webView;
	//delete media;
	//delete vwidget;
	delete pixmapBot2;
	delete pixmapBot1;


	// empty QList
	while (!laserDistanceTextFront->isEmpty())
	{
		delete laserDistanceTextFront->takeFirst();
	}
	delete laserDistanceTextFront;

	// empty QList
	while (!laserDistanceLineListFront->isEmpty())
	{
		delete laserDistanceLineListFront->takeFirst();
	}
	delete laserDistanceLineListFront;

	// empty QList
	while (!laserLineListFront->isEmpty())
	{
		delete laserLineListFront->takeFirst();
	}
	delete laserLineListFront;


	// empty QList
	while (!laserDistanceTextRear->isEmpty())
	{
		delete laserDistanceTextRear->takeFirst();
	}
	delete laserDistanceTextRear;

	// empty QList
	while (!laserDistanceLineListRear->isEmpty())
	{
		delete laserDistanceLineListRear->takeFirst();
	}
	delete laserDistanceLineListRear;

	// empty QList
	while (!laserLineListRear->isEmpty())
	{
		delete laserLineListRear->takeFirst();
	}
	delete laserLineListRear;

	delete scene;
	
	delete direcsremote1;
}


void Gui::on_btnGo_clicked()
{
	// load the URL
	// using 'setUrl'instead of 'load' here! setUrl clears the view and loads the URL.
	webView->setUrl(QUrl(ui.lineEditWebAddress->text()));
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

/*
void Gui::on_btnExit_clicked()
{
	close();
}
*/

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

/*
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
*/

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
// 			ui.lblMotor1mA->setText(QString("%1 mA").arg(value));
			break;
		case MOTORSENSOR2:
// 			ui.lblMotor1mA->setText(QString("%1 mA").arg(value));
			break;
		case MOTORSENSOR3:
// 			ui.lblMotor1mA->setText(QString("%1 mA").arg(value));
			break;
		case MOTORSENSOR4:
// 			ui.lblMotor1mA->setText(QString("%1 mA").arg(value));
			break;
	}
}


void Gui::on_sliderZoom_valueChanged(int value)
{
	qreal x = 0;
	qreal y = 0;
	qreal newSize = 0;
	QString text;


	// show the zoom value in a label
	ui.labelLaserTop->setText(tr("%1").arg(value));


	// rescale the robot pixmap to the last scale
	// (the laser lines are rescaled automatically by in the slot)
	pixmapBot1->scale( (1 / (qreal)lastZoom), (1 / (qreal)lastZoom) );
	pixmapBot2->scale( (1 / (qreal)lastZoom), (1 / (qreal)lastZoom) );

	// scale to the actual slider value
	pixmapBot1->scale(value, value);
	pixmapBot2->scale(value, value);

	//--------------------------
	// store the current scale
	// ('lastZoom' is also used in calculateLaserFrontYpos!!)
	//--------------------------
	lastZoom = value;

	//------------------------------
	// set the position of the bot
	//------------------------------
	// recalculate the middle position of the bot pixmap!
	x = laserXPos -  ( pixmapBot1->pixmap().width() / 2 / startScale * lastZoom);
	y = laserYPos - ( pixmapBot1->pixmap().height() / 2 / startScale * lastZoom);
	//laserXPos = (ui.graphicsViewLaser->width() / 2) - ( pixmapBot1->pixmap().width() / 2 / startScale * lastZoom);

	// horizontal center
	pixmapBot1->setPos(x, y);
	pixmapBot2->setPos(x, y);
	//appendLog(QString("<b>sliderZoomValueChanged...bot y pos()=%1</b>").arg(pixmapBot1->y()));


	//------------------------------------------------------
	// change the x and y position of the FRONT laser lines
	//------------------------------------------------------
	x = laserXPos;
	y = laserYPos;

	for (int i=0; i<laserLineListFront->size(); i++)
	{
		// horizontal center:
		// x = middle of the bot pixmap in the view
		// y = set manually
		laserLineListFront->at(i)->setPos(x, y);
	}


	//------------------------------------------------
	// change the y position of the REAR laser lines
	//------------------------------------------------
	x = laserXPos;
	y = laserYPos;

	for (int i=0; i<laserLineListRear->size(); i++)
	{
		// horizontal center:
		// x = middle of the bot pixmap in the view
		// y = set manually
		laserLineListRear->at(i)->setPos(x, y);
	}


	//------------------------------------------------------------------
	// change the y position of the FRONT laser distance lines and text
	//------------------------------------------------------------------
	for (int i=0; i<laserDistanceLineListFront->size(); i++)
	{
		// (LASERDISTANCEFIRSTCIRCLE/STARTZOOMLEVEL) is the 'factor'
		// value is the zoomSlider value
		newSize = (LASERDISTANCEFIRSTCIRCLE / STARTZOOMLEVEL * value) + (i * LASERDISTANCEDISTANCE);

		// recalculate the new position (put the middle of the circle in the middle of the graphics view)
		x = laserXPos - (newSize / 2);
		y = laserYPos - (newSize / 2);

		// change the width and height
		laserDistanceLineListFront->at(i)->setRect(0, 0, newSize, newSize);
		// set the circle position!
		laserDistanceLineListFront->at(i)->setPos(x, y);


		// recalculate the new text position
		x = laserXPos;
		y = laserYPos + (newSize/2);

		// set the text position!
		laserDistanceTextFront->at(i)->setPos(x, y);

		// set the text
		text = QString("%1").arg(newSize/(FITTOFRAMEFACTOR*value*2));
		// only show 2 digits after the ','
		laserDistanceTextFront->at(i)->setText(text.left(text.indexOf(".") + 3) + "m");
	}


	//------------------------------------------------------------------
	// change the y position of the REAR laser distance lines and text
	//------------------------------------------------------------------
	for (int i=0; i<laserDistanceLineListRear->size(); i++)
	{
		// (LASERDISTANCEFIRSTCIRCLE/STARTZOOMLEVEL) is the 'factor'
		// value is the zoomSlider value
		newSize = (LASERDISTANCEFIRSTCIRCLE / STARTZOOMLEVEL * value) + (i * LASERDISTANCEDISTANCE);

		// recalculate the new position (put the middle of the circle in the middle of the graphics view)
		x = laserXPos - (newSize / 2);
		y = laserYPos - (newSize / 2);

		// change the width and height
		laserDistanceLineListRear->at(i)->setRect(0, 0, newSize, newSize);
		// set the circle position!
		laserDistanceLineListRear->at(i)->setPos(x, y);


		// recalculate the new text position
		x = laserXPos;
		y = laserYPos - (newSize/2);

		// set the text position!
		laserDistanceTextRear->at(i)->setPos(x, y);

		// set the text
		text = QString("%1").arg(newSize/(FITTOFRAMEFACTOR*value*2));
		// only show 2 digits after the ','
		laserDistanceTextRear->at(i)->setText(text.left(text.indexOf(".") + 3) + "m");
	}
}


void Gui::initLaserView()
{
	// for getting nice x and y position
	qreal x = 0;
	qreal y = 0;


	//--------------
	// window init
	//--------------
	// TODO: which values for which resolutions!?
	//ui.splitLaserView->setSizes(QList<int>() << 1000 << 100);
	// make dockLaserView wider
	//ui.dockLaserView->resize(ui.graphicsViewLaser->width(), ui.dockLaserView->height());
	//ui.centralWidget->resize(ui.graphicsViewLaser->width(), ui.dockLaserView->height());
	//ui.centralWidget->resize(400, ui.dockLaserView->height());


	//==================================================
	// move the laser lines to their x and y positions
	//==================================================

	// init the reference spot pos at startup!
	// (horicontal and vertical middle of the view)
	laserXPos = (ui.graphicsViewLaser->width()  / 2);
	laserYPos = (ui.graphicsViewLaser->height() / 2);

	// init laser y pos at startup!
	x = laserXPos;
	y = laserYPos + INITIALLASERYPOSFRONT;

	//--------------
	// REAR laser
	//--------------
	for (int i=0, angle=90; i<laserLineListRear->size(); i++, angle--)
	{
		// reset transform or rotation
		laserLineListRear->at(i)->resetTransform();

		// rotate every line by one degree
		laserLineListRear->at(i)->rotate(angle);

		// set position of each line
		laserLineListRear->at(i)->setPos((x - laserLineListRear->at(i)->line().length()), y);
	}


	y = laserYPos + INITIALLASERYPOSREAR;

	//--------------
	// FRONT laser
	//--------------
	for (int i=0, angle=-90; i<laserLineListFront->size(); i++, angle--)
	{
		// reset transform or rotation
		laserLineListFront->at(i)->resetTransform();

		// rotate every line by one degree
		laserLineListFront->at(i)->rotate(angle);

		// set position of each line
		laserLineListFront->at(i)->setPos((x - laserLineListFront->at(i)->line().length()), y);
	}


	//==========================================================
	// refresh the view with the actual zoom (after gui came up)
	//==========================================================
	on_sliderZoom_valueChanged(lastZoom);


	//=============================================================================
	// move the FRONT distance lines to their x and y positions
	// has to be *after* sliderZoom_valueChanged for correct 'distance' positions!
	//=============================================================================
	// in the middle of the front of bot, minus a half of the innerst circle (no. 0)
	x = laserXPos - (laserDistanceLineListFront->at(0)->rect().width() / 2);
	y = laserYPos - (laserDistanceLineListFront->at(0)->rect().height() / 2);

	//-----------------
	// FRONT distances
	//-----------------
	for (int i=0; i<laserDistanceLineListFront->size(); i++)
	{
		laserDistanceLineListFront->at(i)->setPos( (x - (i*LASERDISTANCEDISTANCE/2)), y - (i*LASERDISTANCEDISTANCE/2) );
	}


	//=============================================================================
	// move the REAR distance lines to their x and y positions
	// has to be *after* sliderZoom_valueChanged for correct 'distance' positions!
	//=============================================================================
	// in the middle of the front of bot, minus a half of the innerst circle (no. 0)
	x = laserXPos - (laserDistanceLineListRear->at(0)->rect().width() / 2);
	y = laserYPos - (laserDistanceLineListRear->at(0)->rect().height() / 2);

	//-----------------
	// REAR distances
	//-----------------
	for (int i=0; i<laserDistanceLineListRear->size(); i++)
	{
		laserDistanceLineListRear->at(i)->setPos( (x - (i*LASERDISTANCEDISTANCE/2)), y - (i*LASERDISTANCEDISTANCE/2) );
	}


	// zoom into the laser lines by default factor
	ui.sliderZoom->setValue(STARTZOOMLEVEL);
}


void Gui::refreshLaserViewFront(float *laserScannerValues, int *laserScannerFlags)
{
	int laserLineLength = 0;
	int zoomView = ui.sliderZoom->value(); // get a scale to fit the beams into the window

	//----------------------------------------------------------------------------------------
	// Change the laser lines (color and length)
	//----------------------------------------------------------------------------------------
	// First: set/change the color of each line, depending on the flag set in the laserThread
	//----------------------------------------------------------------------------------------
	for (int i=0; i<laserLineListFront->size(); i++)
	{
		// check if there was an obstacle
		if (laserScannerFlags[i] == OBSTACLE)
		{
			// obstacle detected!
			//orgBrush.setColor(colorLaserObstacle);
			laserLineListFront->at(i)->setPen(QPen(colorLaserObstacle));
		}
		else
		{
			if (laserScannerFlags[i] == LARGESTFREEWAY)
			{
				// l a r g e s t   f r e e   w a y
				laserLineListFront->at(i)->setPen(QPen(colorLaserPreferredDrivingDirection));
			}
			else
			{
				if (laserScannerFlags[i] == CENTEROFLARGESTFREEWAY)
				{
					//  center of free way (only one line)
					laserLineListFront->at(i)->setPen(QPen(colorLaserCenterDrivingDirection));
				}
				else
				{
					//   n o   o b s t a c l e
					laserLineListFront->at(i)->setPen(QPen(colorLaserFreeWay));
				}
			}
		}
	}


	//---------------------------------------------------------------------------
	// Second: change the *length* of each line!
	//---------------------------------------------------------------------------
	// /get the data from 180° to 0° (right to left!!)
	for (int i=0; i<laserLineListFront->size(); i++)
	{
		// get value from laser and
		// draw the lines at every 1°
		laserLineLength = qRound(laserScannerValues[i]*FITTOFRAMEFACTOR*zoomView); // length in Pixel!!!

		laserLineListFront->at(i)->setLine(0, 0, 0, laserLineLength);

		// set tool tip of the line to the distance
		laserLineListFront->at(i)->setToolTip( QString("%1 m  / %2 deg / Flag=%3 / %4 Pixel").arg(laserScannerValues[i]).arg(i).arg(laserScannerFlags[i]).arg(laserLineLength) );
	}
}


void Gui::refreshLaserViewRear(float *laserScannerValues, int *laserScannerFlags)
{
	int laserLineLength = 0;
	int zoomView = ui.sliderZoom->value(); // get a scale to fit the beams into the window

	//----------------------------------------------------------------------------------------
	// Change the laser lines (color and length)
	//----------------------------------------------------------------------------------------
	// First: set/change the color of each line, depending on the flag set in the laserThread
	//----------------------------------------------------------------------------------------
	for (int i=0; i<laserLineListRear->size(); i++)
	{
		// check if there was an obstacle
		if (laserScannerFlags[i] == OBSTACLE)
		{
			// obstacle detected!
			//orgBrush.setColor(colorLaserObstacle);
			laserLineListRear->at(i)->setPen(QPen(colorLaserObstacle));
		}
		else
		{
			if (laserScannerFlags[i] == LARGESTFREEWAY)
			{
				// l a r g e s t   f r e e   w a y
				laserLineListRear->at(i)->setPen(QPen(colorLaserPreferredDrivingDirection));
			}
			else
			{
				if (laserScannerFlags[i] == CENTEROFLARGESTFREEWAY)
				{
					//  center of free way (only one line)
					laserLineListRear->at(i)->setPen(QPen(colorLaserCenterDrivingDirection));
				}
				else
				{
					//   n o   o b s t a c l e
					laserLineListRear->at(i)->setPen(QPen(colorLaserFreeWay));
				}
			}
		}
	}


	//---------------------------------------------------------------------------
	// Second: change the *length* of each line!
	//---------------------------------------------------------------------------
	// /get the data from 180° to 0° (right to left!!)
	for (int i=0; i<laserLineListRear->size(); i++)
	{
		// get value from laser and
		// draw the lines at every 1°
		laserLineLength = qRound(laserScannerValues[i]*FITTOFRAMEFACTOR*zoomView); // length in Pixel!!!


		laserLineListRear->at(i)->setLine(0, 0, 0, laserLineLength);

		// set tool tip of the line to the distance
		//laserLinList->at(i)->setToolTip(QString("%1 m / %2 Pixel").arg(laserScannerValues[i]).arg(laserLineLength));
		//laserLineListRear->at(i)->setToolTip(QString("%1 m (%2 deg)").arg(laserScannerValues[i]).arg(i));
		laserLineListRear->at(i)->setToolTip( QString("%1 m  / %2 deg / Flag=%3").arg(laserScannerValues[i]).arg(i).arg(laserScannerFlags[i]) );
//		laserLineListRear->at(i)->setToolTip(QString("x=%1 y=%2 (%3 deg)").arg(pos.x()).arg(pos.y()).arg(i+1));
	}
}


void Gui::setRobotPosition(QGraphicsSceneMouseEvent* mouseEvent)
{
	//qreal diff = laserFrontYPos - laserRearYPos;

	// new y pos
	laserYPos = mouseEvent->scenePos().y();
	//laserRearYPos = laserFrontYPos - diff;

	// new x pos
	laserXPos = mouseEvent->scenePos().x();

	// refresh laserView
	on_sliderZoom_valueChanged(ui.sliderZoom->value());
}


void Gui::zoomLaserView(QGraphicsSceneWheelEvent* wheelEvent)
{
	int zoomValue = ui.sliderZoom->value();


	if (wheelEvent->delta() > 0)
	{
		zoomValue++;
	}
	else
	{
		zoomValue--;
	}

	// refresh laserView (set zoom slider)
	ui.sliderZoom->setValue(zoomValue);
}


void Gui::createLaserScannerObjects()
{
	// the start position for the pos. calculation
	laserXPos = 0;
	laserYPos = 0;
	//laserFrontYPos = INITIALLASERYPOSFRONT;
	//laserRearYPos  = INITIALLASERYPOSREAR;

	// set some nice colors for some widgets
	colorLaserObstacle =  QColor(255, 50, 50); // light red
	colorLaserFreeWay = Qt::darkRed;
	colorLaserPreferredDrivingDirection = QColor(7, 68, 30); // green
	colorLaserCenterDrivingDirection = Qt::green;
	colorGraphicsSceneBackground = Qt::black;


	// the graphicsScene for the laser scanner
	scene = new LaserScene();

	// set some colors
	scene->setBackgroundBrush(colorGraphicsSceneBackground);

	// turn off moving of scene, when objects extend the scene									-> DISABLED to enable dragging the robot in the laserView!!
	// (set scene rect to size of GUI element)													-> DISABLED to enable dragging the robot in the laserView!!
	//scene->setSceneRect(0, 0, ui.graphicsViewLaser->width(), ui.graphicsViewLaser->height());	-> DISABLED to enable dragging the robot in the laserView!!

	// set scene to the GUI
	ui.graphicsViewLaser->setScene(scene);

	// enable OpenGL rendering with antialiasing (and direct hardware rendering (if supportet from the hardware))
	ui.graphicsViewLaser->setViewport(new QGLWidget(QGLFormat(QGL::DoubleBuffer | QGL::DirectRendering)));


	//=======================================================
	// add robot picture1
	//=======================================================
	// add items to the scene
	pixmapBot1 = new QGraphicsPixmapItem(QPixmap(":/images/images/bot_from_above.png"));

	// init the scale for the laser line / distances drawing stuff
	lastZoom = ui.sliderZoom->value();

	//--------------------------------------------------------------
	// set the start scale
	//--------------------------------------------------------------
	startScale = 10;

	// change scale of the robot pic to 1/10 to fit in the window and to fit on the size of the laser lines
	pixmapBot1->scale( (1.0 / startScale), (1.0 / startScale));

	// horizontal center
	pixmapBot1->setPos(laserXPos, laserYPos);

	// add the pixmap
	scene->addItem(pixmapBot1);

	// put one layer up (layer 2). All others share the same (unset) layer under the pixmap.
	pixmapBot1->setZValue(1);


	//=====================================================
	// create the laser line lists
	//=====================================================
	laserLineListFront = new QList <QGraphicsLineItem*>();
	laserLineListRear = new QList <QGraphicsLineItem*>();


	//-------------------------------------
	// create the FRONT laser line list
	//-------------------------------------
	// TODO: check if always 180 lines!
	// create 180 laser lines (0 to 179)
	for (int i=-90; i<90; i++)
	{
		QGraphicsLineItem *line = new QGraphicsLineItem();

		// the length (and position) of the laser line in pixel
		line->setLine(0,0,0,0);

		// FIXME doest not work: line->setPen(QPen(colorLaserFreeWay, 3));
		line->setPen(QPen(colorLaserFreeWay));

		// set position of each line
		line->rotate(i);

		// put one layer up (layer 2). All others share the same (unset) layer under the pixmap.
		line->setZValue(2);

		// add line to QList
		laserLineListFront->append(line);

		// add line to scene
		scene->addItem(line);
	}


	//-------------------------------------
	// create the REAR laser line list
	//-------------------------------------
	// TODO: check if always 180 lines!
	// create 180 laser lines (0 to 179)
	for (int i=90; i>-90; i--)
	{
		QGraphicsLineItem *line = new QGraphicsLineItem();

		// FIXME doest not work: line->setPen(QPen(colorLaserFreeWay, 3));
		line->setPen(QPen(colorLaserFreeWay));

		// the length (and position) of the laser line in pixel
		line->setLine(0,0,0,0);

		// set position of each line
		line->rotate(i);

		// put one layer up (layer 2). All others share the same (unset) layer under the pixmap.
		line->setZValue(2);

		// add line to QList
		laserLineListRear->append(line);

		// add line to scene
		scene->addItem(line);
	}


	//=======================================================
	// add robot picture2
	//=======================================================
	// add items to the scene
	pixmapBot2 = new QGraphicsPixmapItem(QPixmap(":/images/images/bot_from_above_TOP2.png"));

	// change scale of the robot pic to 1/10 to fit in the window and to fit on the size of the laser lines
	pixmapBot2->scale( (1.0 / startScale), (1.0 / startScale));

	// horizontal center
	pixmapBot2->setPos(laserXPos, laserYPos);

	// add the pixmap
	scene->addItem(pixmapBot2);

	// put one layer up (layer 2). All others share the same (unset) layer under the pixmap.
	pixmapBot2->setZValue(3);
}


void Gui::createLaserDistanceObjects()
{
	// set colors
	colorHelpLine = Qt::gray;
	colorHelpLineText = Qt::gray;


	//--------------------------------------------------------------
	// create the FRONT laser line distances with text
	//--------------------------------------------------------------
	laserDistanceLineListFront = new QList <QGraphicsEllipseItem*>();
	laserDistanceTextFront = new QList <QGraphicsSimpleTextItem*>();

	// create and add twelve semi circles and text
	for (int i=0; i<LASERDISTANCECIRCLES; i++)
	{
		// create semi circles
		// position doesn't matter, because of moving circles in setLaserDistancePosition()! So we just take 0,0 here.
		QGraphicsEllipseItem *semiCircle = new QGraphicsEllipseItem(0, 0, LASERDISTANCEFIRSTCIRCLE + (i*LASERDISTANCEDISTANCE), LASERDISTANCEFIRSTCIRCLE + (i*LASERDISTANCEDISTANCE));

		// create an (empty) text
		QGraphicsSimpleTextItem *text = new QGraphicsSimpleTextItem();

		// set the start angle of the circle
		semiCircle->setStartAngle(180*16);
		// set the span angle of the circle
		semiCircle->setSpanAngle(180*16);

		// set semiCircle color
		semiCircle->setPen(QPen(colorHelpLine));

		// set text color
		text->setBrush(QBrush(colorHelpLine));

		// setting to the highest layer level
		semiCircle->setZValue(4);
		text->setZValue(4);

		// add semiCircle to QList
		laserDistanceLineListFront->append(semiCircle);
		laserDistanceTextFront->append(text);

		// add semiCircle to scene
		scene->addItem(semiCircle);

		// add text to scene
		scene->addItem(text);
	}


	//--------------------------------------------------------------
	// create the REAR laser line distances with text
	//--------------------------------------------------------------
	laserDistanceLineListRear = new QList <QGraphicsEllipseItem*>();
	laserDistanceTextRear = new QList <QGraphicsSimpleTextItem*>();

	// create and add twelve semi circles and text
	for (int i=0; i<LASERDISTANCECIRCLES; i++)
	{
		// create semi circles
		// position doesn't matter, because of moving circles in setLaserDistancePosition()! So we just take 0,0 here.
		QGraphicsEllipseItem *semiCircle = new QGraphicsEllipseItem(0, 0, LASERDISTANCEFIRSTCIRCLE + (i*LASERDISTANCEDISTANCE), LASERDISTANCEFIRSTCIRCLE + (i*LASERDISTANCEDISTANCE));

		// create an (empty) text
		QGraphicsSimpleTextItem *text = new QGraphicsSimpleTextItem();

		// set the start angle of the circle
		semiCircle->setStartAngle(0);
		// set the span angle of the circle
		semiCircle->setSpanAngle(180*16);

		// set semiCircle color
		semiCircle->setPen(QPen(colorHelpLine));

		// set text color
		text->setBrush(QBrush(colorHelpLine));

		// setting to the highest layer level
		semiCircle->setZValue(4);
		text->setZValue(4);

		// add semiCircle to QList
		laserDistanceLineListRear->append(semiCircle);
		laserDistanceTextRear->append(text);

		// add semiCircle to scene
		scene->addItem(semiCircle);

		// add text to scene
		scene->addItem(text);
	}
}


void Gui::showLaserFrontAngles(int largestFreeAreaStart, int largestFreeAreaEnd, int centerOfFreeWay)
{
	ui.lblLaserFrontFreeStart->setText(QString("%1 deg").arg(largestFreeAreaStart));
	ui.lblLaserFrontFreeEnd->setText(QString("%1 deg").arg(largestFreeAreaEnd));
	ui.lblLaserFrontFreeCenter->setText(QString("%1 deg").arg(centerOfFreeWay));
}


void Gui::showPreferredDirection(QString direction)
{
	if (direction == "FORWARD")
	{
		ui.lblPreferredDirection->setPixmap(QPixmap(":/images/images/up.png"));
		return;
	}

	if (direction == "LEFT")
	{
		ui.lblPreferredDirection->setPixmap(QPixmap(":/images/images/left.png"));
		return;
	}

	if (direction == "RIGHT")
	{
		ui.lblPreferredDirection->setPixmap(QPixmap(":/images/images/right.png"));
		return;
	}
}


void Gui::setCamImage(QImage* image)
{
	ui.lblCamera->setPixmap(pixmap.fromImage(*(image)));
}
