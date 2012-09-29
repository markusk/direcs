/*************************************************************************
 *   Copyright (C) 2011 by Markus Knapp                                  *
 *   www.direcs.de                                                       *
 *                                                                       *
 *   This file is part of direcs.                                        *
 *                                                                       *
 *   direcs is free software: you can redistribute it and/or modify it   *
 *   under the terms of the GNU General Public License as published      *
 *   by the Free Software Foundation, version 3 of the License.          *
 *                                                                       *
 *   direcs is distributed in the hope that it will be useful,           *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of      *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the        *
 *   GNU General Public License for more details.                        *
 *                                                                       *
 *   You should have received a copy of the GNU General Public License   *
 *   along with direcs. If not, see <http://www.gnu.org/licenses/>.      *
 *                                                                       *
 *************************************************************************/

#include "gui.h"


Gui::Gui(QMainWindow *parent) : QMainWindow(parent)
{
	// define some nice green and red colors
	labelFillColorGreen = QColor(64, 255, 64);
	labelFillColorRed   = QColor(255, 64, 64);
	labelFillColorBlue  = QColor(64, 64, 255);

	//-------------------------------------------------------
	// startup the GUI
	//-------------------------------------------------------
	ui.setupUi(this);

	//--------------------------------
	// do the rest of my init stuff
	//--------------------------------
	init();
}


void Gui::init()
{
	laserXPos = 0; // correct value is set in the initLaserView()!!
	laserYPos = 0; // correct value is set in the initLaserView()!!


}


Gui::~Gui()
{
}


void Gui::closeEvent(QCloseEvent *event)
{
	// no compiler warning "unused"
	Q_UNUSED(event);


	//qDebug("closeEvent");
	emit shutdown();
}


void Gui::on_actionExit_activated()
{
	close();
}



void Gui::appendLog(QString text, bool CR, bool sayIt, bool addTimestamp)
{
	if (addTimestamp)
	{
		// get the current date and time for a timestimp in the log
		now = QDateTime::currentDateTime();

		// prepend timestamp
		text = QString("%1:%2:%3 %4").arg(now.toString("hh")).arg(now.toString("mm")).arg(now.toString("ss")).arg(text);
	}

	// insert the text in the GUI
	ui.textEditLog->insertHtml(text);

	if (CR == TRUE) // default!
		ui.textEditLog->insertHtml("<br>");

	// Ensures that the cursor is visible by scrolling the text edit if necessary.
	ui.textEditLog->ensureCursorVisible();

	Q_UNUSED(sayIt);
}



void Gui::appendAnswerLog(QString text, bool CR, bool sayIt, bool addTimestamp)
{
//	if (addTimestamp)
//	{
//		// get the current date and time for a timestimp in the log
//		now = QDateTime::currentDateTime();

//		// prepend timestamp
//		text = QString("%1:%2:%3 %4").arg(now.toString("hh")).arg(now.toString("mm")).arg(now.toString("ss")).arg(text);
//	}

//	// insert the text in the GUI
//	ui.textEditAnswerLog->insertHtml(text);

//	if (CR == TRUE) // default!
//		ui.textEditAnswerLog->insertHtml("<br>");

//	// Ensures that the cursor is visible by scrolling the text edit if necessary.
//	ui.textEditAnswerLog->ensureCursorVisible();

	Q_UNUSED(text);
	Q_UNUSED(sayIt);
	Q_UNUSED(CR);
	Q_UNUSED(addTimestamp);
}


void Gui::on_actionSimulate_activated()
{
	if (ui.actionSimulate->isChecked())
	{
		ui.actionSimulate->setIcon(QIcon(QPixmap(":/images/images/utilities-system-monitor-active.png")));
		ui.actionSimulate->setStatusTip("Disable simulation");
		ui.actionSimulate->setToolTip("Disable simulation");
		emit simulate(true);
	}
	else
	{
		ui.actionSimulate->setIcon(QIcon(QPixmap(":/images/images/utilities-system-monitor.png")));
		ui.actionSimulate->setStatusTip("Simulate");
		ui.actionSimulate->setToolTip("Simulate");
		emit simulate(false);
	}
}


void Gui::on_actionTest_activated()
{
	appendLog("Test!");
	emit test();
}


void Gui::setLEDGreen(bool state)
{
	if (state)
	{
		ui.lblLEDGreen->setPixmap(QPixmap(":/images/images/led_green.gif"));
	}
	else
	{
		ui.lblLEDGreen->setPixmap(QPixmap(":/images/images/led_gray.gif"));
		return;
	}
}


void Gui::setLEDRed(bool state)
{
	if (state)
	{
		ui.lblLEDRed->setPixmap(QPixmap(":/images/images/led_red.gif"));
	}
	else
	{
		ui.lblLEDRed->setPixmap(QPixmap(":/images/images/led_gray.gif"));
		return;
	}
}


void Gui::setFlashlight(bool state)
{
	if (state)
	{
		ui.lblRelais->setPixmap(QPixmap(":/images/images/relais_on.png"));
		ui.lblFlashlight->setPixmap(QPixmap(":/images/images/strobo_on.png"));
	}
	else
	{
		ui.lblRelais->setPixmap(QPixmap(":/images/images/relais_off.png"));
		ui.lblFlashlight->setPixmap(QPixmap(":/images/images/strobo_off.png"));
		return;
	}
}


QString Gui::removeHtml(QString text)
{
	//------------------------------
	// remove HTML tags from string
	//------------------------------
	int start= -1;
	do
	{
		// search for the first HTML "<"
		start = text.indexOf("<");
		text.remove(start, text.indexOf(">") - start + 1);
	} while (text.contains(">"));
	// till the last HTML ">" is found

	return text;
}


void Gui::initLaserStuff()
{
	// called after all the laser settings have been read in the direcs mail class!
	createLaserScannerObjects();
//	createLaserDistanceObjects();
//	createLaserWidthObjects();

	//----------------------------------------------------------------------------------
	// get the mouse position, when the robot position is to be changed
	//----------------------------------------------------------------------------------
	connect(scene, SIGNAL( robotPositionChanged(QGraphicsSceneMouseEvent *) ), this, SLOT( setRobotPosition(QGraphicsSceneMouseEvent *) ));
	connect(scene, SIGNAL( wheelZoom(QGraphicsSceneWheelEvent *) ), this, SLOT( zoomLaserView(QGraphicsSceneWheelEvent *) ));
}


void Gui::initLaserView()
{
	int i = 0;
	qreal angle = 0;
	// for getting nice x and y position
	qreal x = 0;
	qreal y = 0;


	//--------------
	// window init
	//--------------
	/// \todo which values for which resolutions!?
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
	y = laserYPos; // INITIALLASERYPOSFRONT has no effect here, only in on_sliderZoom_valueChanged !!


	//--------------
	// FRONT laser
	//--------------
	// rotate every laser line by 'resolution' degree (e.g. 0.1 or 0.5 or 1.0) [initLaserView]
	// Be sure to rotate that the middle of the laserAngle is always at a vertical 90 degree in the scene!
	// (Rotate laser line counterclockwise by 180 degrees and further by the half of the laserAngle)
	i = 0;
	angle = ( 180 - (laserscannerAngleFront/2) );

	while (i<laserLineListFront->size())
	{
// org:		laserLineListFront->at(i)->rotate(angle);
		laserLineListFront->at(i)->rotate(180);        //  rotate by 180 deg to show them bottom up
// test		laserLineListFront->at(i)->setTransform(QTransform().translate(x, y).rotate(45).translate(-x,-y));

		i++;
// org:		angle += laserscannerResolutionFront;
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


void Gui::createLaserScannerObjects()
{
	// the start position for the pos. calculation
	laserXPos = 0;
	laserYPos = 0;
	//laserFrontYPos = INITIALLASERYPOSFRONT;
	//laserRearYPos  = INITIALLASERYPOSREAR;

	// set some nice colors for some widgets
	colorLaserObstacle =  Qt::red;
	colorLaserFreeWay = Qt::darkRed;
	colorLaserPreferredDrivingDirection = Qt::darkGreen;
	colorLaserCenterDrivingDirection = Qt::yellow;
	colorLaserIgnoreArea =  Qt::darkYellow;
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

	// enable OpenGL rendering with antialiasing (and direct hardware rendering (if supportet by the hardware))
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

	// put one layer up (layer 2).
	pixmapBot1->setZValue(2);


	//=====================================================
	// create the laser line lists
	//=====================================================
	laserLineListFront = new QList <QGraphicsLineItem*>();
	laserLineListRear = new QList <QGraphicsLineItem*>();


	//-------------------------------------
	// create the FRONT laser line list
	//-------------------------------------
	// create all laser lines(e.g.180 or 270, one for each degree or half-degree)
	// the rotation will be done in the initLaserView() !
	for (int i=0; i<(laserscannerAngleFront/laserscannerResolutionFront); i++)
	{
		QGraphicsLineItem *line = new QGraphicsLineItem();

		// the length (and position) of the laser line in pixel
		line->setLine(0,0,0,0);

		// set the laser line color
		//line->setPen(QPen(colorLaserFreeWay, 3));
		line->setPen(QPen(colorLaserFreeWay));

		// put one layer up (layer 3).
		line->setZValue(3);

		// add line to QList
		laserLineListFront->append(line);

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

	// put one layer up (layer 4).
	pixmapBot2->setZValue(4);
//	pixmapBot1->setVisible(false); // just for testing
//	pixmapBot2->setVisible(false); // just for testing
}


void Gui::on_sliderZoom_valueChanged(int value)
{
	qreal x = 0;
	qreal y = 0;
	qreal newSize = 0;
	QString text;


	// show the zoom value in a label /// @todo put his in a signal slot in qt desginer
	// ui.labelLaserTop->setText(tr("%1").arg(value));


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
	x = laserXPos -  ( pixmapBot1->pixmap().width()  / 2 / startScale * lastZoom);
	y = laserYPos -  ( pixmapBot1->pixmap().height() / 2 / startScale * lastZoom);

	// horizontal center
	pixmapBot1->setPos(x, y);
	pixmapBot2->setPos(x, y);


	//------------------------------------------------------
	// change the x and y position of the FRONT laser lines [on_sliderZoom_valueChanged]
	//------------------------------------------------------
// org:	x = laserXPos;
	x = laserXPos - (laserLineListFront->size()/2);
// org:	y = laserYPos - (INITIALLASERYPOSFRONT / startScale * lastZoom);

// org:	for (int i=0; i<laserLineListFront->size(); i++)
	for (int i=0, m=0; i<laserLineListFront->size(); i++, m++)
	{
		// use current y position, which is the length to change y position (distance from robot to obstacle)
		y = laserYPos - (laserLineListFront->at(i)->y() / startScale * lastZoom);

		// horizontal center:
		// x = middle of the bot pixmap in the view
		// y = set manually
// org:		laserLineListFront->at(i)->setPos(x, y);
		laserLineListFront->at(i)->setPos(x+m, y);
	}


	//------------------------------------------------
	// change the y position of the REAR laser lines
	//------------------------------------------------
	x = laserXPos;
	y = laserYPos + (INITIALLASERYPOSREAR / startScale * lastZoom);

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
		y = laserYPos - (newSize / 2) + (INITIALLASERYPOSFRONT / startScale * lastZoom);

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
		y = laserYPos - (newSize / 2) - (INITIALLASERYPOSREAR / startScale * lastZoom);

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
