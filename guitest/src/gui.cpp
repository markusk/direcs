/*************************************************************************
 *   Copyright (C) Markus Knapp                                          *
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
	robotIsOn = false;
	consoleMode = false;
	useLargeGUI = true;

	laserXPos = 0; // correct value is set in the initLaserView()!!
	laserYPos = 0; // correct value is set in the initLaserView()!!

	laserscannerAngleFront = 0;
	laserscannerAngleRear = 0;
	laserscannerResolutionFront = 0;
	laserscannerResolutionRear = 0;
	mLargestFreeAreaStartFront = 0;
	mLargestFreeAreaEndFront = 0;
	mLargestFreeAreaStartRear = 0;
	mLargestFreeAreaEndRear = 0;

	// define some nice green and red colors
	labelFillColorGreen = QColor(64, 255, 64);
	labelFillColorRed   = QColor(255, 64, 64);
	labelFillColorBlue  = QColor(64, 64, 255);
	gridColor = QColor(Qt::black);

	// store which GUI to use
	// the robot GUI is the small one
	useLargeGUI = !useSmallGUI;


	// do the rest of my init stuff
	init();
}


void Gui::init()
{
	//-----------------
	// startup the GUI
	//-----------------
	ui.setupUi(this);
qDebug("gui init okay");

	// remote control is enabled by default. @sa Direcs::init()
	ui.actionRemote->setChecked(true);



	//----------------------------------------------------------------------------
	// Laser Scanner graphics Stuff (scene, view, lines, OpenGL etc.)
	//----------------------------------------------------------------------------
	// -> the laser lines and pixmap init stuff is now done AFTER the laser settings are read in the direcs main class!


	//----------------------------------
	// change text of buttons
	//----------------------------------
	ui.actionDrive->setText("&Drive");
	ui.actionDrive->setToolTip("Start driving");
	ui.actionDrive->setStatusTip("Start driving");
}


Gui::~Gui()
{
	delete pixmapBot2;
	delete pixmapBot1;

	delete widthLeftCircleFront;
	delete widthRightCircleFront;
	delete widthLineFront;
	delete widthTextFront;

	delete widthLeftCircleRear;
	delete widthRightCircleRear;
	delete widthLineRear;
	delete widthTextRear;

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


void Gui::setRobotControls(bool state)
{
	// store the state
	robotIsOn = state;

	// set the controls
	if (useLargeGUI)
	{
		ui.actionDrive->setEnabled(state);
		ui.actionReset->setEnabled(state);
	}

	if (!robotIsOn)
	{
		/// \todo test if this really is needed. Is the heartbeaet in the sensorthread sufficient?
		// set the state LED to red
		setLEDHeartbeat(RED);
	}
}


void Gui::appendLog(QString text, bool CR, bool sayIt, bool addTimestamp)
{
	if (addTimestamp == true)
	{
		// get the current date and time for a timestimp in the log
		now = QDateTime::currentDateTime();

		// prepend timestamp
		text = QString("%1:%2:%3 %4").arg(now.toString("hh")).arg(now.toString("mm")).arg(now.toString("ss")).arg(text);
	}

	// insert the new text in the GUI
	if (useLargeGUI)
	{
		ui.textEditGlobalLog->insertHtml(text);
	}
	else
	{
		// uiSmall.textEditGlobalLog->insertHtml(text);
	}

	if (CR == true) // default!
	{
		if (useLargeGUI)
		{
			ui.textEditGlobalLog->insertHtml("<br>");
		}
		else
		{
			// uiSmall.textEditGlobalLog->insertHtml("<br>");
		}
	}

	// Ensures that the cursor is visible by scrolling the text edit if necessary.
	if (useLargeGUI)
	{
		ui.textEditGlobalLog->ensureCursorVisible();
	}
	else
	{
		// uiSmall.textEditGlobalLog->ensureCursorVisible();
	}


	if (consoleMode)
	{
		// remove HTML code
		text = removeHtml(text);
		// print text to console
		// qDebug() << text; is NOT used, because it adds quotation marks to all strings
		QByteArray textForConsole = text.toLatin1();
		qDebug("%s", textForConsole.data());
	}


	// say the message via Linux espeak
	if (sayIt == true)
	{
		emit speak(text);
	}
}


void Gui::appendNetworkLog(QString text, bool CR, bool sayIt)
{
	QString newText;


	// get the current date and time for a timestamp in the log
	now = QDateTime::currentDateTime();
	// with DATE: newText = QString("%1-%2-%3 %4:%5:%6 %7").arg(now.toString("yyyy")).arg(now.toString("MM")).arg(now.toString("dd")).arg(now.toString("hh")).arg(now.toString("mm")).arg(now.toString("ss")).arg(text);
	newText = QString("%1:%2:%3 %4").arg(now.toString("hh")).arg(now.toString("mm")).arg(now.toString("ss")).arg(text);

	// insert the text
	if (useLargeGUI)
	{
		ui.textEditNetworkLog->insertHtml(newText);
	}
	else
	{
		// uiSmall.textEditNetworkLog->insertHtml(newText);
	}

	if (CR == true) // default!
	{
		// insert a line break
		if (useLargeGUI)
		{
			ui.textEditNetworkLog->insertHtml("<br>");
		}
		else
		{
			// uiSmall.textEditNetworkLog->insertHtml("<br>");
		}
	}

	// Ensures that the cursor is visible by scrolling the text edit if necessary.
	if (useLargeGUI)
	{
		ui.textEditNetworkLog->ensureCursorVisible();
	}
	else
	{
		// uiSmall.textEditNetworkLog->ensureCursorVisible();
	}


	if (consoleMode)
	{
		// remove HTML code
		text = removeHtml(newText);
		// print text to console
		// qDebug() << text; is NOT used, because it adds quotation marks to all strings
		QByteArray textForConsole = text.toLatin1();
		qDebug("%s", textForConsole.data());
	}


	// say the message via Linux espeak
	if (sayIt == true)
	{
		emit speak(text);
	}
}


void Gui::appendSerialLog(QString text, bool CR)
{
	QString newText;


	// get the current date and time for a timestimp in the log
	now = QDateTime::currentDateTime();
	// with DATE: newText = QString("%1-%2-%3 %4:%5:%6 %7").arg(now.toString("yyyy")).arg(now.toString("MM")).arg(now.toString("dd")).arg(now.toString("hh")).arg(now.toString("mm")).arg(now.toString("ss")).arg(text);
	newText = QString("%1:%2:%3 %4").arg(now.toString("hh")).arg(now.toString("mm")).arg(now.toString("ss")).arg(text);

	// insert the text
	if (useLargeGUI)
	{
		ui.textEditSerialLog->insertHtml(newText);
	}
	else
	{
		// uiSmall.textEditSerialLog->insertHtml(newText);
	}

	if (CR == true) // default!
	{
		// insert a line break
		if (useLargeGUI)
		{
			ui.textEditSerialLog->insertHtml("<br>");
		}
		else
		{
			// uiSmall.textEditSerialLog->insertHtml("<br>");
		}
	}

	// Ensures that the cursor is visible by scrolling the text edit if necessary.
	if (useLargeGUI)
	{
		ui.textEditSerialLog->ensureCursorVisible();
	}
	else
	{
		// uiSmall.textEditSerialLog->ensureCursorVisible();
	}


	if (consoleMode)
	{
		// remove HTML code
		text = removeHtml(newText);
		// print text to console
		// qDebug() << text; is NOT used, because it adds quotation marks to all strings
		QByteArray textForConsole = text.toLatin1();
		qDebug("%s", textForConsole.data());
	}
}


void Gui::on_actionDrive_activated()
{
	static bool toggle = false;


	if (toggle == false)
	{
		appendLog("Start driving...");
		toggle = true;

		// change text of "drive button"
		if (useLargeGUI)
		{
			ui.actionDrive->setText("Stop &driving");
			ui.actionDrive->setToolTip("Stop driving");
			ui.actionDrive->setStatusTip("Stop driving");
		}

		//----------------
		// start driving
		//----------------
		emit drive(START);
	}
	else
	{
		toggle = false;
		appendLog("Stop driving...");

		// change text of "drive button"
		if (useLargeGUI)
		{
			ui.actionDrive->setText("&Drive");
			ui.actionDrive->setToolTip("Start driving");
			ui.actionDrive->setStatusTip("Start driving");
		}

		//****************
		// stop driving
		//****************
		emit drive(STOP);
	}
}


void Gui::on_actionReset_activated()
{
	// reset the circuit (motors off etc.)
	emit initCircuit();

	// reset the servos
	emit initServos();

	// stop driving
	emit drive(STOP);

	appendLog("Reseted.");
}


void Gui::on_actionResetDrivenDistance_activated()
{
	// reset counter
	emit resetDrivenDistance(MOTORSENSOR1);
	// reset labels
	if (useLargeGUI)
	{
		ui.labelDrivenDistance1->setText("0 cm");
		ui.labelRevolutions1->setText("0");
	}

	// reset counter
	emit resetDrivenDistance(MOTORSENSOR2);
	// reset labels
	if (useLargeGUI)
	{
		ui.labelDrivenDistance2->setText("0 cm");
		ui.labelRevolutions2->setText("0");
	}

	// reset counter
	emit resetDrivenDistance(MOTORSENSOR3);
	// reset labels
	if (useLargeGUI)
	{
		ui.labelDrivenDistance3->setText("0 cm");
		ui.labelRevolutions3->setText("0");
	}

	// reset counter
	emit resetDrivenDistance(MOTORSENSOR4);
	// reset labels
	if (useLargeGUI)
	{
		ui.labelDrivenDistance4->setText("0 cm");
		ui.labelRevolutions4->setText("0");
	}
}


void Gui::on_actionTest_activated()
{
	emit test();
}


#ifndef USEROBOTGUI
void Gui::on_actionAll_activated()
{
}
#endif


#ifndef USEROBOTGUI
void Gui::on_actionCamera_activated()
{
}
#endif


#ifndef USEROBOTGUI
void Gui::on_actionVoltage_activated()
{
}
#endif


#ifndef USEROBOTGUI
void Gui::on_actionState_activated()
{
	if (useLargeGUI)
	{
		if (ui.dockState->isVisible())
		{
			ui.dockState->hide();
		}
		else
		{
			ui.dockState->show();
		}
	}
}
#endif


void Gui::on_actionSettings_activated()
{
}


#ifndef USEROBOTGUI
void Gui::on_actionLog_activated()
{
	if (useLargeGUI)
	{
		if (ui.dockLog->isVisible())
		{
			ui.dockLog->hide();
		}
		else
		{
			ui.dockLog->show();
		}
	}
}
#endif


void Gui::on_actionJoystick_activated()
{
}


void Gui::on_actionAbout_activated()
{
}


void Gui::showDistanceGraphical(int sensor, int distance)
{
	Q_UNUSED(sensor);
	Q_UNUSED(distance);
}


void Gui::showDrivenDistance(int sensor, int distance)
{
	// show distance value in centimeters in a text label
	// erwartet den value als distance (z.B. in cm)!
	if ((sensor < MOTORSENSOR1) || (sensor > MOTORSENSOR4))
	{
		qDebug("ERROR gui, showDrivenDistance: wrong motor sensor number");
		return;
	}


	switch (sensor)
	{
		case MOTORSENSOR1:
			if (useLargeGUI)
			{
				ui.labelDrivenDistance1->setText(QString("%1 cm").arg(distance));
			}
			return;
			break;
		case MOTORSENSOR2:
			if (useLargeGUI)
			{
				ui.labelDrivenDistance2->setText(QString("%1 cm").arg(distance));
			}
			return;
			break;
		case MOTORSENSOR3:
			if (useLargeGUI)
			{
				ui.labelDrivenDistance3->setText(QString("%1 cm").arg(distance));
			}
			return;
			break;
		case MOTORSENSOR4:
			if (useLargeGUI)
			{
				ui.labelDrivenDistance4->setText(QString("%1 cm").arg(distance));
			}
			else
			return;
			break;
	}
}


void Gui::showVoltage(int sensor, float voltage)
{
	Q_UNUSED(sensor);
	Q_UNUSED(voltage);
}


void Gui::showMotorStatus(int motor, bool power, int direction)
{
	switch (motor)
	{
		//-------------
		case MOTOR1:
		//-------------
			switch (power)
			{
				case ON:
					break;
				case OFF:
					ui.lblLEDBackRight1->setPixmap(QPixmap(":/images/images/led_gray.gif"));
					break;
			}

			switch (direction)
			{
				case FORWARD:
					ui.lblLEDBackRight1->setPixmap(QPixmap(":/images/images/led_green.gif"));
					break;
				case BACKWARD:
					ui.lblLEDBackRight1->setPixmap(QPixmap(":/images/images/led_red.gif"));
					break;
				case SAME:
					break;
			}
			break;

		//-------------
		case MOTOR2:
		//-------------
			switch (power)
			{
				case ON:
					break;
				case OFF:
					ui.lblLEDBackLeft2->setPixmap(QPixmap(":/images/images/led_gray.gif"));
					break;
			}

			switch (direction)
			{
				case FORWARD:
					ui.lblLEDBackLeft2->setPixmap(QPixmap(":/images/images/led_green.gif"));
					break;
				case BACKWARD:
					ui.lblLEDBackLeft2->setPixmap(QPixmap(":/images/images/led_red.gif"));
					break;
				case SAME:
					break;
			}
			break;

		//-------------
		case MOTOR3:
		//-------------
			switch (power)
			{
				case ON:
					break;
				case OFF:
					ui.lblLEDFrontRight3->setPixmap(QPixmap(":/images/images/led_gray.gif"));
					break;
			}

			switch (direction)
			{
				case FORWARD:
					ui.lblLEDFrontRight3->setPixmap(QPixmap(":/images/images/led_green.gif"));
					break;
				case BACKWARD:
					ui.lblLEDFrontRight3->setPixmap(QPixmap(":/images/images/led_red.gif"));
					break;
				case SAME:
					break;
			}
			break;

		//-------------
		case MOTOR4:
		//-------------
			switch (power)
			{
				case ON:
					break;
				case OFF:
					ui.lblLEDFrontLeft4->setPixmap(QPixmap(":/images/images/led_gray.gif"));
					break;
			}

			switch (direction)
			{
				case FORWARD:
					ui.lblLEDFrontLeft4->setPixmap(QPixmap(":/images/images/led_green.gif"));
					break;
				case BACKWARD:
					ui.lblLEDFrontLeft4->setPixmap(QPixmap(":/images/images/led_red.gif"));
					break;
				case SAME:
					break;
			}
			break;
	} // switch
}


void Gui::showLaserFrontAngles(int largestFreeAreaStart, int largestFreeAreaEnd, int centerOfFreeWay, float width)
{
	int configuredPassageWidth = 58;  /// < < < < < < < < < <
	/// \todo do all this stuff for the rear scanner?

	// store the values in the class local for refreshLaserView()
	if (largestFreeAreaStart == -1)
	{
		mLargestFreeAreaStartFront = 0;
	}
	else
	{
		mLargestFreeAreaStartFront = largestFreeAreaStart;
	}

	if (largestFreeAreaEnd == -1)
	{
		mLargestFreeAreaEndFront = 0;
	}
	else
	{
		mLargestFreeAreaEndFront = largestFreeAreaEnd;
	}


	ui.lblLaserFrontFreeArea->setText(QString("%1 - %2 deg").arg(largestFreeAreaStart).arg(largestFreeAreaEnd));
	ui.lblLaserFrontFreeCenter->setText(QString("%1 deg").arg(centerOfFreeWay));


	// show width in cm with one decimal place (Nachkommastelle)
	if (width != -1)
	{
//		// CONVERT the WIDTH to METERS!!
//		width /= 100;
//		// show the text with 2 decimals (Nachkommastellen)
//		ui.lblLaserFrontFreeWidth->setText(QString("%1").setNum(width, 'f', 2).append(" m"));

		// show the text with 0 decimals (Nachkommastellen)
		if (useLargeGUI)
		{
			ui.lblLaserFrontFreeWidth->setText(QString("%1").setNum(width, 'f', 0).append(" cm"));
		}
		else
		{
			// uiSmall.lblLaserFrontFreeWidth->setText(QString("%1").setNum(width, 'f', 0).append(" cm"));
		}

		// show the currently configured robot slot with with a prepended '>' or '<'
		if ( width > configuredPassageWidth)
		{
			if (useLargeGUI)
			{
				ui.lblPassageWidth->setText( QString("> %1 cm").arg(configuredPassageWidth) );
			}
			else
			{
				// uiSmall.lblPassageWidth->setText( QString("> %1 cm").arg(configuredPassageWidth) );
			}
		}
		else
		{
			if (useLargeGUI)
			{
				ui.lblPassageWidth->setText( QString("< %1 cm").arg(configuredPassageWidth) );
			}
			else
			{
				// uiSmall.lblPassageWidth->setText( QString("< %1 cm").arg(configuredPassageWidth) );
			}
		}
	}
	else
	{
		if (useLargeGUI)
		{
			ui.lblLaserFrontFreeWidth->setText("oo");
		}
		else
		{
			// uiSmall.lblLaserFrontFreeWidth->setText("oo");
		}

		// show the currently configured robot slot with with a prepended '>' or '<'
		if (useLargeGUI)
		{
			ui.lblPassageWidth->setText( QString("> %1 cm").arg(configuredPassageWidth) );
		}
		else
		{
			// uiSmall.lblPassageWidth->setText( QString("> %1 cm").arg(configuredPassageWidth) );
		}
	}


	// show the same width in the scene
	widthTextFront->setText( ui.lblLaserFrontFreeWidth->text() );
}


void Gui::showPreferredDirection(QString direction)
{
	if (direction == "FORWARD")
	{
		if (useLargeGUI)
		{
			ui.lblPreferredDirection->setPixmap(QPixmap(":/images/images/up.png"));
		}
		else
		{
			// uiSmall.lblPreferredDirection->setPixmap(QPixmap(":/images/images/up.png"));
		}
		return;
	}

	if (direction == "LEFT")
	{
		if (useLargeGUI)
		{
			ui.lblPreferredDirection->setPixmap(QPixmap(":/images/images/left.png"));
		}
		else
		{
			// uiSmall.lblPreferredDirection->setPixmap(QPixmap(":/images/images/left.png"));
		}
		return;
	}

	if (direction == "RIGHT")
	{
		if (useLargeGUI)
		{
			ui.lblPreferredDirection->setPixmap(QPixmap(":/images/images/right.png"));
		}
		else
		{
			// uiSmall.lblPreferredDirection->setPixmap(QPixmap(":/images/images/right.png"));
		}
		return;
	}

	if (direction == "NONE")
	{
		if (useLargeGUI)
		{
			ui.lblPreferredDirection->setPixmap(QPixmap(":/images/images/middle.png"));
		}
		else
		{
			// uiSmall.lblPreferredDirection->setPixmap(QPixmap(":/images/images/middle.png"));
		}
		return;
	}

	appendLog(QString("ERROR: direction '%1' not supported in showPreferredDirection::Gui").arg(direction));
}


void Gui::on_actionRemote_activated()
{
}


void Gui::on_actionSimulate_activated()
{
	if (ui.actionSimulate->isChecked())
	{
		ui.actionSimulate->setStatusTip("Disable simulation");
		ui.actionSimulate->setToolTip("Disable simulation");
		emit simulate(true);
	}
	else
	{
		ui.actionSimulate->setStatusTip("Simulate");
		ui.actionSimulate->setToolTip("Simulate");
		emit simulate(false);
	}
}


void Gui::on_actionSaveLaserData_activated()
{
	emit writeLaserData();
}


void Gui::on_actionDemo_activated()
{
	if (ui.actionDemo->isChecked())
	{
		emit demo(true);
	}
	else
	{
		emit demo(false);
	}
}


void Gui::on_actionMediaSkip_activated()
{
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
	// REAR laser
	//--------------
	// rotate every laser line by 'resolution' degree (e.g. 0.1 or 0.5 or 1.0)
	// Be sure to rotate that the middle of the laserAngle is always at a vertical 90 degree in the scene!
	// (Rotate laser line counterclockwise by the half of the laserAngle)
	i = 0;
	angle = -(laserscannerAngleRear/2);

	while (i<laserLineListRear->size())
	{
		laserLineListRear->at(i)->rotate(angle);

		i++;
		angle += laserscannerResolutionRear;
	}


	y = laserYPos;// INITIALLASERYPOSREAR has no effect here, only in on_sliderZoom_valueChanged !!


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
	if (useLargeGUI)
	{
		ui.sliderZoom->setValue(STARTZOOMLEVEL);
	}
	else
	{
		// uiSmall.sliderZoom->setValue(STARTZOOMLEVEL);
	}
}


void Gui::refreshLaserViewFront(QList <float> laserScannerValues, QList <int> laserScannerFlags)
{
	qreal x = 0.0;
	qreal y = 0.0;
	qreal distance = 0.0;
	/*
	int r = 0;
	qreal xKart;
	qreal yKart;
	int angle = 0;
	static qreal widthLineFrontPosX1 = 0.0;
	static qreal widthLineFrontPosX2 = 0.0;
	static qreal widthLineFrontPosY1 = 0.0;
	static qreal widthLineFrontPosY2 = 0.0;
	*/
	int laserLineLength = 0;
	int zoomView = 0;
	zoomView = ui.sliderZoom->value(); // get a scale to fit the beams into the window


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
			laserLineListFront->at(i)->setPen(QPen(colorLaserObstacle));
		}
		else
		{
			if (laserScannerFlags[i] == LARGESTFREEWAY)
			{
				// l a r g e s t   f r e e   w a y
				// laserLineListFront->at(i)->setPen(QPen(colorLaserPreferredDrivingDirection, 4));
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
					if (laserScannerFlags[i] == IGNORETHIS)
					{
						// mark the ignored areas
						laserLineListFront->at(i)->setPen(QPen(colorLaserIgnoreArea));
					}
					else
					{
						//   n o   o b s t a c l e
						laserLineListFront->at(i)->setPen(QPen(colorLaserFreeWay));
					}
				}
			}
		}
	}


	//---------------------------------------------------------------------------
	// Second: change the *length* of each line!
	//---------------------------------------------------------------------------
	// /get the data from 180 to 0 (right to left!!)
	for (int i=0; i<laserLineListFront->size(); i++)
	{
		// get value from laser and
		// draw the lines at every 1
// org:		laserLineLength = qRound(laserScannerValues[i]*FITTOFRAMEFACTOR*zoomView); // length in Pixel!!!
		laserLineLength = 10; // length in Pixel!!!

		// set line length
		laserLineListFront->at(i)->setLine(0, 0, 0, laserLineLength);

		// use length to change y position (distance from robot to obstacle)
		distance = qRound(laserScannerValues[i]*FITTOFRAMEFACTOR*zoomView);

		// set new line y position. x is untouched [refreshLaserViewFront]
// org:		laserLineListFront->at(i)->setPos(laserLineListFront->at(i)->scenePos().x(), laserLineLength);
		laserLineListFront->at(i)->setPos(laserLineListFront->at(i)->scenePos().x(), distance);


		// set tool tip of the line to the distance
		int flag = laserScannerFlags[i];

		if (flag == FREEWAY)
		{
			string = "FREEWAY";
		} else if (flag == OBSTACLE)
		{
			string = "OBSTACLE";
		} else if (flag == LARGESTFREEWAY)
		{
			string = "LARGESTFREEWAY";
		} else if (flag == CENTEROFLARGESTFREEWAY)
		{
			string = "CENTEROFLARGESTFREEWAY";
		} else if (flag == IGNORETHIS)
		{
			string = "IGNORETHIS";
		}

		laserLineListFront->at(i)->setToolTip( QString("FRONT: %1 m, %2, %3 deg, %4 Pixel").arg(laserScannerValues[i]).arg(string).arg(i).arg(laserLineLength) );


		// get the current scene positions of the laser line
		x = laserLineListFront->at(i)->scenePos().x();
		y = laserLineListFront->at(i)->scenePos().y();
/*
		// draw the first (left) width circle / get the firt coordinate for the width line
		if (i == mLargestFreeAreaStartFront)
		{
			angle = 270 - qRound(laserscannerAngleFront / 2) + i; // we're not starting at 0 degrees; we have an offset! (s.a. initLaserView (rotate loop)
			r = laserLineListFront->at(i)->line().length(); // here we have use 'i' instead of 'angle'. i is the current 'original angle'!

			// check which line of the free area is longer and store the positions for the drive trough line!
			if (laserLineListFront->at(mLargestFreeAreaStartFront)->line().length() < laserLineListFront->at(mLargestFreeAreaEndFront)->line().length())
			{
				// lenth of both lines is the same or the other one is shorter
				r = laserLineListFront->at(mLargestFreeAreaStartFront)->line().length();
			}
			else
			{
				// lenth of both lines is the same or the other one is shorter
				r = laserLineListFront->at(mLargestFreeAreaEndFront)->line().length();
			}

			// convert from polar to kartesic coordinates
			// WARNING: "cos" functions use radians!! so we convert the degrees to radions here!
			// sin and cos are swapped here because of a different x, y and angle orientation than in a normal kartesic coordination system!
			xKart = r * cos( angle * M_PI / 180 );
			yKart = r * sin( angle * M_PI / 180 );

			if ( (i < 90) && (laserscannerAngleFront > 180) )
			{
				// here we are in the quadrant where x is negative and y is postive
				// so we have to change the leading sign
				xKart *= -1;
			}

			// add the "start coordinates" (the laser line origin)
			xKart += x - (widthCirclesWidth/2);
			yKart += y - (widthCirclesWidth/2);

			//------------------------------------------------------------------
			// store this positions for the width line
			//------------------------------------------------------------------
			// corrected by the circle radius, to get the middle of the circle
			widthLineFrontPosX1 = xKart +  (widthCirclesWidth/2);
			widthLineFrontPosY1 = yKart +  (widthCirclesWidth/2);

			// set the circle position!
			widthLeftCircleFront->setPos(xKart, yKart);

			// set the width text to this circle
			// place it a bit above the circle
			widthTextFront->setPos(xKart, yKart - widthCirclesWidth * 2 );
		}
		else
		{
			// in all other cases
			// draw the next (right) width circle
			angle = 270 - qRound(laserscannerAngleFront / 2) + i; // we're not starting at 0 degrees; we have an offset! (s.a. initLaserView (rotate loop)
			r = laserLineListFront->at(i)->line().length(); // here we have use 'i' instead of 'angle'. i is the current 'original angle'!

			//--------------------------------------------------------------------
			// draw the width line
			//--------------------------------------------------------------------
			// check which line of the free area is longer and store the positions for the drive trough line!
			if (laserLineListFront->at(mLargestFreeAreaStartFront)->line().length() < laserLineListFront->at(mLargestFreeAreaEndFront)->line().length())
			{
				// lenth of both lines is the same or the other one is shorter
				r = laserLineListFront->at(mLargestFreeAreaStartFront)->line().length();
			}
			else
			{
				// lenth of both lines is the same or the other one is shorter
				r = laserLineListFront->at(mLargestFreeAreaEndFront)->line().length();
			}

			// convert from polar to kartesic coordinates
			// WARNING: "cos" functions use radians!! so we convert the degrees to radions here!
			// sin and cos are swapped here because of a different x, y and angle orientation than in a normal kartesic coordination system!
			xKart = r * cos( angle * M_PI / 180 );
			yKart = r * sin( angle * M_PI / 180 );

			if ( (i < 90) && (laserscannerAngleFront > 180) )
			{
				// here we are in the quadrant where x is negative and y is postive
				// so we have to change the leading sign
				xKart *= -1;
			}

			// add the "start coordinates" (the laser line origin)
			xKart += x - (widthCirclesWidth/2);
			yKart += y - (widthCirclesWidth/2);

			//------------------------------------------------------------------
			// store this positions for the width line
			//------------------------------------------------------------------
			// corrected by the circle radius, to get the middle of the circle
			widthLineFrontPosX2 = xKart +  (widthCirclesWidth/2);
			widthLineFrontPosY2 = yKart +  (widthCirclesWidth/2);

			// draw it
			widthLineFront->setLine(widthLineFrontPosX1, widthLineFrontPosY1, widthLineFrontPosX2, widthLineFrontPosY2);

			// set the circle position!
			widthRightCircleFront->setPos( widthLineFrontPosX2 - (widthCirclesWidth/2), widthLineFrontPosY2 - (widthCirclesWidth/2) );
		}
*/
	}

	// make the width objects visible
	widthLeftCircleFront->setVisible(true);
	widthRightCircleFront->setVisible(true);
	widthLineFront->setVisible(true);
}


void Gui::refreshLaserViewRear(QList <float> laserScannerValues, QList <int> laserScannerFlags)
{
	qreal x = 0.0;
	qreal y = 0.0;
	int r = 0;
	qreal xKart;
	qreal yKart;
	int angle = 0;
	static qreal widthLineRearPosX1 = 0.0;
	static qreal widthLineRearPosX2 = 0.0;
	static qreal widthLineRearPosY1 = 0.0;
	static qreal widthLineRearPosY2 = 0.0;
	int laserLineLength = 0;
	int zoomView = 0;
	zoomView = ui.sliderZoom->value(); // get a scale to fit the beams into the window


	//----------------------------------------------------------------------------------------
	// Change the laser lines (color and length)
	//----------------------------------------------------------------------------------------
	// First: set/change the color of each line, depending on the flag set in the laserThread
	//----------------------------------------------------------------------------------------
	for (int i=0; i<laserLineListRear->size(); i++)
	{
		if (laserScannerFlags[i] == IGNORETHIS)
		{
			//  i g n o r e d   a r e a s
			laserLineListRear->at(i)->setPen(QPen(colorLaserIgnoreArea));
		}
		else
		{
			// check if there was an obstacle
			if (laserScannerFlags[i] == OBSTACLE)
			{
				// obstacle detected!
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
		laserLineListRear->at(i)->setToolTip( QString("REAR: %1 m / %2 deg / Flag=%3 / %4 Pixel").arg(laserScannerValues[i]).arg(i).arg(laserScannerFlags[i]).arg(laserLineLength) );

		// get the current scene positions of the laser line
		x = laserLineListRear->at(i)->scenePos().x();
		y = laserLineListRear->at(i)->scenePos().y();

		// draw the first (left) width circle / get the firt coordinate for the width line
		if (i == mLargestFreeAreaStartRear)
		{
			angle = i - qRound(laserscannerAngleRear/2); // we're not starting at 0 degrees; we have an offset! (s.a. initLaserView (rotate loop)
			r = laserLineListRear->at(i)->line().length(); // here we have use 'i' instead of 'angle'. i is the current 'original angle'!

			// check which line of the free area is longer and store the positions for the drive trough line!
			if (laserLineListRear->at(mLargestFreeAreaStartRear)->line().length() < laserLineListRear->at(mLargestFreeAreaEndRear)->line().length())
			{
				// lenth of both lines is the same or the other one is shorter
				r = laserLineListRear->at(mLargestFreeAreaStartRear)->line().length();
			}
			else
			{
				// lenth of both lines is the same or the other one is shorter
				r = laserLineListRear->at(mLargestFreeAreaEndRear)->line().length();
			}

			// convert from polar to kartesic coordinates
			// WARNING: "cos" functions use radians!! so we convert the degrees to radions here!
			// sin and cos are swapped here because of a different x, y and angle orientation than in a normal kartesic coordination system!
			xKart = r * sin( angle * M_PI / 180 );
			yKart = r * cos( angle * M_PI / 180 );

			// bring it to the correct quadrant in the coord. system
			xKart *= -1;

			// add the "start coordinates" (the laser line origin)
			xKart += x - (widthCirclesWidth/2);
			yKart += y - (widthCirclesWidth/2);

			//------------------------------------------------------------------
			// store this positions for the width line
			//------------------------------------------------------------------
			// corrected by the circle radius, to get the middle of the circle
			widthLineRearPosX1 = xKart +  (widthCirclesWidth/2);
			widthLineRearPosY1 = yKart +  (widthCirclesWidth/2);

			// set the circle position!
			widthLeftCircleRear->setPos(xKart, yKart);
		}
		else
		{
			// in all other cases
			// draw the next (right) width circle
			angle = i - qRound(laserscannerAngleRear/2); // we're not starting at 0 degrees; we have an offset! (s.a. initLaserView (rotate loop)
			r = laserLineListRear->at(i)->line().length(); // here we have use 'i' instead of 'angle'. i is the current 'original angle'!

			//--------------------------------------------------------------------
			// draw the width line
			//--------------------------------------------------------------------
			// check which line of the free area is longer and store the positions for the drive trough line!
			if (laserLineListRear->at(mLargestFreeAreaStartRear)->line().length() < laserLineListRear->at(mLargestFreeAreaEndRear)->line().length())
			{
				// lenth of both lines is the same or the other one is shorter
				r = laserLineListRear->at(mLargestFreeAreaStartRear)->line().length();
			}
			else
			{
				// lenth of both lines is the same or the other one is shorter
				r = laserLineListRear->at(mLargestFreeAreaEndRear)->line().length();
			}

			// convert from polar to kartesic coordinates
			// WARNING: "cos" functions use radians!! so we convert the degrees to radions here!
			// sin and cos are swapped here because of a different x, y and angle orientation than in a normal kartesic coordination system!
			xKart = r * sin( angle * M_PI / 180 );
			yKart = r * cos( angle * M_PI / 180 );

			// bring it to the correct quadrant in the coord. system
			xKart *= -1;

			// add the "start coordinates" (the laser line origin)
			xKart += x - (widthCirclesWidth/2);
			yKart += y - (widthCirclesWidth/2);

			//------------------------------------------------------------------
			// store this positions for the width line
			//------------------------------------------------------------------
			// corrected by the circle radius, to get the middle of the circle
			widthLineRearPosX2 = xKart +  (widthCirclesWidth/2);
			widthLineRearPosY2 = yKart +  (widthCirclesWidth/2);

			// draw it
			widthLineRear->setLine(widthLineRearPosX1, widthLineRearPosY1, widthLineRearPosX2, widthLineRearPosY2);

			// set the circle position!
			widthRightCircleRear->setPos( widthLineRearPosX2 - (widthCirclesWidth/2), widthLineRearPosY2 - (widthCirclesWidth/2) );
		}
	} // for i

	// make the width objects visible
	widthLeftCircleRear->setVisible(true);
	widthRightCircleRear->setVisible(true);
	widthLineRear->setVisible(true);
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


void Gui::setLaserscannerAngle(short int laserscanner, int angle)
{
		switch (laserscanner)
		{
		case LASER1:
			laserscannerAngleFront = angle;
			return;
			break;
		case LASER2:
			laserscannerAngleRear = angle;
			return;
			break;
		}

		qDebug("laser number not yet supported  (Gui::setLaserscannerAngle");
}


void Gui::setLaserscannerResolution(short int laserscanner, float resolution)
{
	switch (laserscanner)
	{
	case LASER1:
		laserscannerResolutionFront = resolution;
		return;
		break;
	case LASER2:
		laserscannerResolutionRear = resolution;
		return;
		break;
	}

	qDebug("laser number not yet supported  (Gui::setLaserscannerResolution");
}


void Gui::zoomLaserView(QGraphicsSceneWheelEvent* wheelEvent)
{
	int zoomValue = 0;


	zoomValue = ui.sliderZoom->value();


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
	if (useLargeGUI)
	{
		ui.graphicsViewLaser->setScene(scene);
	}
	else
	{
		// uiSmall.graphicsViewLaser->setScene(scene);
	}

	// enable OpenGL rendering with antialiasing (and direct hardware rendering (if supportet by the hardware))
	if (useLargeGUI)
	{
		ui.graphicsViewLaser->setViewport(new QGLWidget(QGLFormat(QGL::DoubleBuffer | QGL::DirectRendering)));
	}
	else
	{
		// uiSmall.graphicsViewLaser->setViewport(new QGLWidget(QGLFormat(QGL::DoubleBuffer | QGL::DirectRendering)));
	}

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

	//-------------------------------------
	// create the REAR laser line list
	//-------------------------------------
	// create all laser lines(e.g.180 or 270, one for each degree)
	// the rotation will be done in the initLaserView() !
	for (int i=0; i<(laserscannerAngleRear/laserscannerResolutionRear); i++)
	{
		QGraphicsLineItem *line = new QGraphicsLineItem();

		// set the laser line color
		// line->setPen(QPen(colorLaserFreeWay, 3));
		line->setPen(QPen(colorLaserFreeWay));

		// the length (and position) of the laser line in pixel
		line->setLine(0,0,0,0);

		// put one layer up (layer 3).
		line->setZValue(3);

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

	// put one layer up (layer 4).
	pixmapBot2->setZValue(4);
//	pixmapBot1->setVisible(false); // just for testing
//	pixmapBot2->setVisible(false); // just for testing
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

		// setting to the lowest layer level
		semiCircle->setZValue(1);
		text->setZValue(1);

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

		// setting to the lowest layer level!
		semiCircle->setZValue(1);
		text->setZValue(1);

		// add semiCircle to QList
		laserDistanceLineListRear->append(semiCircle);
		laserDistanceTextRear->append(text);

		// add semiCircle to scene
		scene->addItem(semiCircle);

		// add text to scene
		scene->addItem(text);
	}
}


void Gui::createLaserWidthObjects()
{
	widthCirclesWidth = 10;  /// \todo which radius?

	// create the objects
	widthLeftCircleFront = new QGraphicsEllipseItem();
	widthRightCircleFront = new QGraphicsEllipseItem();
	widthLineFront = new QGraphicsLineItem();
	widthTextFront = new QGraphicsSimpleTextItem();

	widthLeftCircleRear = new QGraphicsEllipseItem();
	widthRightCircleRear = new QGraphicsEllipseItem();
	widthLineRear = new QGraphicsLineItem();
	widthTextRear = new QGraphicsSimpleTextItem();

	// set the start angle of the front laser circle
	widthLeftCircleFront->setStartAngle(0*16);
	widthRightCircleFront->setStartAngle(0*16);
	// set the span angle of the circle
	widthLeftCircleFront->setSpanAngle(360*16);
	widthRightCircleFront->setSpanAngle(360*16);

	// set the start angle of the rear laser circle
	widthLeftCircleRear->setStartAngle(0*16);
	widthRightCircleRear->setStartAngle(0*16);
	// set the span angle of the circle
	widthLeftCircleRear->setSpanAngle(360*16);
	widthRightCircleRear->setSpanAngle(360*16);

	// set the color
	widthLeftCircleFront->setPen(QPen(Qt::blue)); /// \todo define the circle color
	widthRightCircleFront->setPen(QPen(Qt::blue)); /// \todo define the circle color
	widthLineFront->setPen(QPen(Qt::blue)); /// \todo define the circle color
	widthTextFront->setPen(QPen(Qt::blue)); /// \todo define the circle color

	widthLeftCircleRear->setPen(QPen(Qt::blue)); /// \todo define the circle color
	widthRightCircleRear->setPen(QPen(Qt::blue)); /// \todo define the circle color
	widthLineRear->setPen(QPen(Qt::blue)); /// \todo define the circle color
	widthTextRear->setPen(QPen(Qt::blue)); /// \todo define the circle color

	// setting to the lowest layer level
	widthLeftCircleFront->setZValue(1);
	widthRightCircleFront->setZValue(1);
	widthLineFront->setZValue(1);
	widthTextFront->setZValue(1);

	widthLeftCircleRear->setZValue(1);
	widthRightCircleRear->setZValue(1);
	widthLineRear->setZValue(1);
	widthTextRear->setZValue(1);

	// draw a circle to see the coordinates for the 'drive-tru width'
	// change the width and height
	widthLeftCircleFront->setRect(0, 0, widthCirclesWidth, widthCirclesWidth);
	widthRightCircleFront->setRect(0, 0, widthCirclesWidth, widthCirclesWidth);

	widthLeftCircleRear->setRect(0, 0, widthCirclesWidth, widthCirclesWidth);
	widthRightCircleRear->setRect(0, 0, widthCirclesWidth, widthCirclesWidth);

	// make them unvisible untill we have a change in thre laser line length (other method of this class)
	widthLeftCircleFront->setVisible(false);
	widthRightCircleFront->setVisible(false);
	widthLineFront->setVisible(false);

	widthLeftCircleRear->setVisible(false);
	widthRightCircleRear->setVisible(false);
	widthLineRear->setVisible(false);

	// add the item to the scene
	scene->addItem(widthLeftCircleFront);
	scene->addItem(widthRightCircleFront);
	scene->addItem(widthLineFront);
	scene->addItem(widthTextFront);

	scene->addItem(widthLeftCircleRear);
	scene->addItem(widthRightCircleRear);
	scene->addItem(widthLineRear);
	scene->addItem(widthTextRear);
}


void Gui::initLaserStuff()
{
	// called after all the laser settings have been read in the direcs mail class!
	createLaserScannerObjects();
	createLaserDistanceObjects();
	createLaserWidthObjects();

	//----------------------------------------------------------------------------------
	// get the mouse position, when the robot position is to be changed
	//----------------------------------------------------------------------------------
	connect(scene, SIGNAL( robotPositionChanged(QGraphicsSceneMouseEvent *) ), this, SLOT( setRobotPosition(QGraphicsSceneMouseEvent *) ));
	connect(scene, SIGNAL( wheelZoom(QGraphicsSceneWheelEvent *) ), this, SLOT( zoomLaserView(QGraphicsSceneWheelEvent *) ));
}


void Gui::setLEDHeartbeat(unsigned char state)
{
	switch (state)
	{
		case RED:
			if (useLargeGUI)
			{
				ui.lblLEDHeartbeat->setPixmap(QPixmap(":/images/images/led_red.gif"));
			}
			else
			{
				// uiSmall.lblLEDHeartbeat->setPixmap(QPixmap(":/images/images/led_red.gif"));
			}
			break;
		case GREEN:
			if (useLargeGUI)
			{
				ui.lblLEDHeartbeat->setPixmap(QPixmap(":/images/images/led_green.gif"));
			}
			else
			{
				// uiSmall.lblLEDHeartbeat->setPixmap(QPixmap(":/images/images/led_green.gif"));
			}
			break;
		case LEDOFF:
			if (useLargeGUI)
			{
				ui.lblLEDHeartbeat->setPixmap(QPixmap(":/images/images/led_gray.gif"));
			}
			else
			{
				// uiSmall.lblLEDHeartbeat->setPixmap(QPixmap(":/images/images/led_gray.gif"));
			}
			break;
	}
}


void Gui::setLEDCompass(unsigned char state)
{
	switch (state)
	{
		case RED:
			if (useLargeGUI)
			{
				ui.lblLEDCompass->setPixmap(QPixmap(":/images/images/led_red.gif"));
			}
			else
			{
				// uiSmall.lblLEDCompass->setPixmap(QPixmap(":/images/images/led_red.gif"));
			}
			break;
		case GREEN:
			if (useLargeGUI)
			{
				ui.lblLEDCompass->setPixmap(QPixmap(":/images/images/led_green.gif"));
			}
			else
			{
				// uiSmall.lblLEDCompass->setPixmap(QPixmap(":/images/images/led_green.gif"));
			}
			break;
		case LEDOFF:
			if (useLargeGUI)
			{
				ui.lblLEDCompass->setPixmap(QPixmap(":/images/images/led_gray.gif"));
			}
			else
			{
				// uiSmall.lblLEDCompass->setPixmap(QPixmap(":/images/images/led_gray.gif"));
			}
			break;
	}
}


void Gui::setLEDJoystick(unsigned char state)
{
	switch (state)
	{
		case RED:
			if (useLargeGUI)
			{
				ui.lblLEDJoystick->setPixmap(QPixmap(":/images/images/led_red.gif"));
			}
			else
			{
				// uiSmall.lblLEDJoystick->setPixmap(QPixmap(":/images/images/led_red.gif"));
			}
			break;
		case GREEN:
			if (useLargeGUI)
			{
				ui.lblLEDJoystick->setPixmap(QPixmap(":/images/images/led_green.gif"));
			}
			else
			{
				// uiSmall.lblLEDJoystick->setPixmap(QPixmap(":/images/images/led_green.gif"));
			}
			break;
		case LEDOFF:
			if (useLargeGUI)
			{
				ui.lblLEDJoystick->setPixmap(QPixmap(":/images/images/led_gray.gif"));
			}
			else
			{
				// uiSmall.lblLEDJoystick->setPixmap(QPixmap(":/images/images/led_gray.gif"));
			}
			break;
	}
}


void Gui::setLEDCamera(unsigned char state)
{
	switch (state)
	{
		case RED:
			if (useLargeGUI)
			{
				ui.lblLEDCamera->setPixmap(QPixmap(":/images/images/led_red.gif"));
			}
			else
			{
				// uiSmall.lblLEDCamera->setPixmap(QPixmap(":/images/images/led_red.gif"));
			}
			break;
		case GREEN:
			if (useLargeGUI)
			{
				ui.lblLEDCamera->setPixmap(QPixmap(":/images/images/led_green.gif"));
			}
			else
			{
				// uiSmall.lblLEDCamera->setPixmap(QPixmap(":/images/images/led_green.gif"));
			}
			break;
		case LEDOFF:
			if (useLargeGUI)
			{
				ui.lblLEDCamera->setPixmap(QPixmap(":/images/images/led_gray.gif"));
			}
			else
			{
				// uiSmall.lblLEDCamera->setPixmap(QPixmap(":/images/images/led_gray.gif"));
			}
			break;
	}
}


void Gui::setLEDNetwork(unsigned char state)
{
	switch (state)
	{
			case RED:
			if (useLargeGUI)
			{
				ui.lblLEDNetwork->setPixmap(QPixmap(":/images/images/led_red.gif"));
			}
			else
			{
				// uiSmall.lblLEDNetwork->setPixmap(QPixmap(":/images/images/led_red.gif"));
			}
			break;
		case GREEN:
			if (useLargeGUI)
			{
				ui.lblLEDNetwork->setPixmap(QPixmap(":/images/images/led_green.gif"));
			}
			else
			{
				// uiSmall.lblLEDNetwork->setPixmap(QPixmap(":/images/images/led_green.gif"));
			}
			break;
		case LEDOFF:
			if (useLargeGUI)
			{
				ui.lblLEDNetwork->setPixmap(QPixmap(":/images/images/led_gray.gif"));
			}
			else
			{
				// uiSmall.lblLEDNetwork->setPixmap(QPixmap(":/images/images/led_gray.gif"));
			}
			break;
	}
}


void Gui::setLEDMasterSlave(unsigned char state)
{
	switch (state)
	{
			case RED:
			if (useLargeGUI)
			{
				ui.lblLEDMasterSlave->setPixmap(QPixmap(":/images/images/led_red.gif"));
			}
			else
			{
				// uiSmall.lblLEDMasterSlave->setPixmap(QPixmap(":/images/images/led_red.gif"));
			}
			break;
		case GREEN:
			if (useLargeGUI)
			{
				ui.lblLEDMasterSlave->setPixmap(QPixmap(":/images/images/led_green.gif"));
			}
			else
			{
				// uiSmall.lblLEDMasterSlave->setPixmap(QPixmap(":/images/images/led_green.gif"));
			}
			break;
		case LEDOFF:
			if (useLargeGUI)
			{
				ui.lblLEDMasterSlave->setPixmap(QPixmap(":/images/images/led_gray.gif"));
			}
			else
			{
				// uiSmall.lblLEDMasterSlave->setPixmap(QPixmap(":/images/images/led_gray.gif"));
			}
			break;
	}
}


void Gui::setLEDLaser(unsigned char state)
{
	switch (state)
	{
		case RED:
			if (useLargeGUI)
			{
				ui.lblLEDLaser->setPixmap(QPixmap(":/images/images/led_red.gif"));
			}
			else
			{
				// uiSmall.lblLEDLaser->setPixmap(QPixmap(":/images/images/led_red.gif"));
			}
			break;
		case GREEN:
			if (useLargeGUI)
			{
				ui.lblLEDLaser->setPixmap(QPixmap(":/images/images/led_green.gif"));
			}
			else
			{
				// uiSmall.lblLEDLaser->setPixmap(QPixmap(":/images/images/led_green.gif"));
			}
			break;
		case LEDOFF:
			if (useLargeGUI)
			{
				ui.lblLEDLaser->setPixmap(QPixmap(":/images/images/led_gray.gif"));
			}
			else
			{
				// uiSmall.lblLEDLaser->setPixmap(QPixmap(":/images/images/led_gray.gif"));
			}
			break;
	}
}


void Gui::setLabelMasterSlave(QString text)
{
	if (useLargeGUI)
	{
		ui.lblMasterSlave->setText(text);
	}
	else
	{
		// uiSmall.lblMasterSlave->setText(text);
	}
}


void Gui::setConsoleMode(bool state)
{
	consoleMode = state;
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


void Gui::systemerrorcatcher(int errorlevel)
{
	if (errorlevel == -1)
	{
		// turn GUI LED laser to red
		setLEDLaser(RED);
	}
}
