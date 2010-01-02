/*************************************************************************
 *   Copyright (C) 2009 by Markus Knapp                                  *
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


Gui::Gui(SettingsDialog *s, JoystickDialog *j, AboutDialog *a, QMainWindow *parent) : QMainWindow(parent)
{
	// copy the pointer from the original SensorThread object
	settingsDialog = s;
	joystickDialog = j;
	aboutDialog = a;

	robotIsOn = false;
	consoleMode = false;
	laserXPos = 0; // correct value is set in the initLaserView()!!
	laserYPos = 0; // correct value is set in the initLaserView()!!
	laserscannerAngleFront = 0;
	laserscannerAngleRear = 0;
	mLargestFreeAreaStartFront = 0;
	mLargestFreeAreaEndFront = 0;
	mLargestFreeAreaStartRear = 0;
	mLargestFreeAreaEndRear = 0;

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
	// remote control is enabled by default. @sa Direcs::init()
	ui.actionRemote->setChecked(true);

	// set battery power labels to green
// 	ui.groupBoxBattery1->setPalette(QPalette(labelFillColorGreen));
// 	ui.groupBoxBattery2->setPalette(QPalette(labelFillColorGreen));
	
// 	ui.lblVoltage1->setPalette(QPalette(labelFillColorGreen));
// 	ui.lblVoltage2->setPalette(QPalette(labelFillColorGreen));

	//----------------------------------------------------------------------------
	// Voltage stuff (progressBars)
	//----------------------------------------------------------------------------
// 	ui.progressBarVoltage1->setRange(0, MAXIMUMVOLTAGE1);
// 	ui.progressBarVoltage2->setRange(0, MAXIMUMVOLTAGE2);
	
	// set maximum in cm AND raise the widget (make it topmost)!
	/*
	ui.progressBarSensor1->setMaximum(SENSORPROGRESSBARMAXIR);
	ui.progressBarSensor2->setMaximum(SENSORPROGRESSBARMAXIR);
	ui.progressBarSensor3->setMaximum(SENSORPROGRESSBARMAXIR);
	ui.progressBarSensor4->setMaximum(SENSORPROGRESSBARMAXIR);
	ui.progressBarSensor5->setMaximum(SENSORPROGRESSBARMAXIR);
	ui.progressBarSensor6->setMaximum(SENSORPROGRESSBARMAXIR);
	ui.progressBarSensor7->setMaximum(SENSORPROGRESSBARMAXIR);
	ui.progressBarSensor8->setMaximum(SENSORPROGRESSBARMAXIR);
	ui.progressBarSensor16->setMaximum(SENSORPROGRESSBARMAXUS);
	*/
/*
infrared Sensors temporarily removed from robot!!
	ui.progressBarSensor1->raise();
	ui.progressBarSensor2->raise();
	ui.progressBarSensor3->raise();
	ui.progressBarSensor4->raise();
	ui.progressBarSensor5->raise();
	ui.progressBarSensor6->raise();
	ui.progressBarSensor7->raise();
	ui.progressBarSensor8->raise();
*/

	/*
	// "fill" the battery power labels (autofill is true)
	ui.lblBatteryPower1->setPalette(QPalette(QColor(255, 255, 255)));
	ui.lblBatteryPower2->setPalette(QPalette(QColor(255, 255, 255)));
	ui.lblBatteryPower3->setPalette(QPalette(QColor(255, 255, 255)));
	ui.lblBatteryPower4->setPalette(QPalette(QColor(255, 255, 255)));
	*/

	
	//----------------------------------------------------------------------------------
	// Whenever the state of the face detect check box changes, set the detection mode
	//----------------------------------------------------------------------------------
	connect(ui.checkBoxFaceDetection, SIGNAL( stateChanged(int) ), SIGNAL( enableFaceDetection(int) ));

	//----------------------------------------------------------------------------------
	// Whenever the state of the face tracking check box changes, set the tracking mode
	//----------------------------------------------------------------------------------
	connect(ui.checkBoxFaceTracking, SIGNAL( stateChanged(int) ), SIGNAL( enableFaceTracking(int) ));

	//----------------------------------------------------------------------------
	// Plot stuff
	//----------------------------------------------------------------------------
	initPlots();

	//----------------------------------------------------------------------------
	// Compass stuff
	//----------------------------------------------------------------------------
// 	initCompass(); // this is for the 2D compass. Not in use at the moment.

	//----------------------------------------------------------------------------
	// Laser Scanner graphics Stuff (scene, view, lines, OpenGL etc.)
	//----------------------------------------------------------------------------
	// -> the laser lines and pixmap init sutff is now done AFTER the laser settings are read in the direcs main class!


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
	
	delete widthLeftCircle;
	delete widthRightCircle;
	delete widthLine;

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
	ui.actionDrive->setEnabled(state);
	ui.actionReset->setEnabled(state);
	ui.btnResetMovement1->setEnabled(state);
	ui.btnResetMovement2->setEnabled(state);
	
	if (!robotIsOn)
	{
		// TODO: test if this really is needed. Is the heartbeaet in the sensorthread sufficient?
		// set the state LED to red
		setLEDHeartbeat(RED);
	}
	
	/*
	TODO: still needed? state already checked in motorControl at lower level. :-)
	ui.btnPower1->setEnabled(state);
	ui.btnPower2->setEnabled(state);
	ui.btnPower3->setEnabled(state);
	ui.btnPower4->setEnabled(state);
	*/
}


void Gui::appendLog(QString text, bool CR, bool sayIt)
{
	// insert the text in the GUI
	ui.textEditLog->insertHtml(text);

	if (CR == TRUE) // default!
		ui.textEditLog->insertHtml("<br>");

	// Ensures that the cursor is visible by scrolling the text edit if necessary.
	ui.textEditLog->ensureCursorVisible();


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
	// insert the text
	ui.textEditNetworkLog->insertHtml(text);

	if (CR == TRUE)
	{
		// insert a line break
		ui.textEditNetworkLog->insertHtml("<br>");
	}

	// Ensures that the cursor is visible by scrolling the text edit if necessary.
	ui.textEditNetworkLog->ensureCursorVisible();


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


void Gui::on_actionDrive_activated()
{
	static bool toggle = false;


	if (toggle == false)
	{
		appendLog("Start driving...");
		toggle = true;

		// change text of "drive button"
		ui.actionDrive->setText("Stop &driving");
		ui.actionDrive->setToolTip("Stop driving");
		ui.actionDrive->setStatusTip("Stop driving");

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
		ui.actionDrive->setText("&Drive");
		ui.actionDrive->setToolTip("Start driving");
		ui.actionDrive->setStatusTip("Start driving");

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

	// reset progressBars
	/*
	ui.progressBarSensor1->setValue(0);
	ui.progressBarSensor2->setValue(0);
	ui.progressBarSensor3->setValue(0);
	ui.progressBarSensor4->setValue(0);
	ui.progressBarSensor5->setValue(0);
	ui.progressBarSensor6->setValue(0);
	ui.progressBarSensor7->setValue(0);
	ui.progressBarSensor8->setValue(0);
	ui.progressBarSensor16->setValue(0);
	*/

	// set labels back in default position
	ui.btnPower1->setPalette(QPalette(QColor(255, 255, 255))); // TODO: use which color here?
	ui.btnPower2->setPalette(QPalette(QColor(255, 255, 255)));
	ui.btnPower3->setPalette(QPalette(QColor(255, 255, 255)));
	ui.btnPower4->setPalette(QPalette(QColor(255, 255, 255)));

	ui.btnDirection1->setText("FORWARD");
	ui.btnDirection1->setPalette(QPalette(QColor(255, 255, 255)));
	ui.btnDirection2->setText("FORWARD");
	ui.btnDirection2->setPalette(QPalette(QColor(255, 255, 255)));
	ui.btnDirection3->setText("FORWARD");
	ui.btnDirection3->setPalette(QPalette(QColor(255, 255, 255)));
	ui.btnDirection4->setText("FORWARD");
	ui.btnDirection4->setPalette(QPalette(QColor(255, 255, 255)));
}


void Gui::on_btnResetMovement1_clicked()
{
	// reset counter
	emit resetDrivenDistance(MOTORSENSOR1);

 	// reset labels
	ui.labelDrivenDistance1->setText("0 cm");
 	ui.labelRevolutions1->setText("0");
}


void Gui::on_btnResetMovement2_clicked()
{
	// reset counter
	emit resetDrivenDistance(MOTORSENSOR2);

	// reset labels
	ui.labelDrivenDistance2->setText("0 cm");
	ui.labelRevolutions2->setText("0");
}


void Gui::on_actionTest_activated()
{
	emit test();
}


void Gui::on_actionAll_activated()
{
	ui.dockNavigation->show();
	ui.dockCamera->show();
	ui.dockOdometrie->show();
	ui.dockVoltage->show();
	ui.dockCurrent->show();
	ui.dockLog->show();
// 	settingsDialog->show();
// 	joystickDialog->show();
}


void Gui::on_actionCamera_activated()
{
	if (ui.dockCamera->isVisible())
	{
		ui.dockCamera->hide();
	}
	else
	{
		ui.dockCamera->show();
	}
}


void Gui::on_actionOdometrie_activated()
{
	if (ui.dockOdometrie->isVisible())
	{
		ui.dockOdometrie->hide();
	}
	else
	{
		ui.dockOdometrie->show();
	}
}


void Gui::on_actionVoltage_activated()
{
	if (ui.dockVoltage->isVisible())
	{
		ui.dockVoltage->hide();
	}
	else
	{
		ui.dockVoltage->show();
	}
}


void Gui::on_actionCurrent_activated()
{
	if (ui.dockCurrent->isVisible())
	{
		ui.dockCurrent->hide();
	}
	else
	{
		ui.dockCurrent->show();
	}
}


void Gui::on_actionSettings_activated()
{
	if (settingsDialog->isVisible())
	{
		settingsDialog->hide();
	}
	else
	{
		settingsDialog->show();
	}
}


void Gui::on_actionLog_activated()
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


void Gui::on_actionJoystick_activated()
{
	if (joystickDialog->isVisible())
	{
		joystickDialog->hide();
	}
	else
	{
		joystickDialog->show();
	}
}


void Gui::on_actionAbout_activated()
{
	if (aboutDialog->isVisible())
	{
		aboutDialog->hide();
	}
	else
	{
		aboutDialog->show();
	}
}


/*
void Gui::showDistance(int sensor, int distance)
{
	if ((sensor < SENSOR1) || (sensor > SENSOR16))
	{
		qDebug("ERROR gui, showDistance: wrong IR sensor number");
		return;
	}


	switch (sensor)
	{
		case SENSOR1:
			ui.labelSensorDistance1->setText(QString("%1").arg(distance));
			return;
			break;
		case SENSOR2:
			ui.labelSensorDistance2->setText(QString("%1").arg(distance));
			return;
			break;
		case SENSOR3:
			ui.labelSensorDistance3->setText(QString("%1").arg(distance));
			return;
			break;
		case SENSOR4:
			ui.labelSensorDistance4->setText(QString("%1").arg(distance));
			return;
			break;
		case SENSOR5:
			ui.labelSensorDistance5->setText(QString("%1").arg(distance));
			return;
			break;
		case SENSOR6:
			ui.labelSensorDistance6->setText(QString("%1").arg(distance));
			return;
			break;
		case SENSOR7:
			ui.labelSensorDistance7->setText(QString("%1").arg(distance));
			return;
			break;
		case SENSOR8:
			ui.labelSensorDistance8->setText(QString("%1").arg(distance));
			return;
			break;
		case SENSOR16:
			// super sonic sensor
			ui.labelSensorDistance16->setText(QString("%1 cm").arg(distance));
			return;
			break;
	}
}
*/


void Gui::showDistanceGraphical(int sensor, int distance)
{
	Q_UNUSED(sensor);
	Q_UNUSED(distance);
	
/*
	if ((sensor < SENSOR1) || (sensor > SENSOR16))
	{
		qDebug("ERROR gui, showDistanceGraphical: wrong IR sensor number");
		return;
	}


	switch (sensor)
	{
		case SENSOR1:
			ui.progressBarSensor1->setValue(distance);
			return;
			break;
		case SENSOR2:
			ui.progressBarSensor2->setValue(distance);
			return;
			break;
		case SENSOR3:
			ui.progressBarSensor3->setValue(distance);
			return;
			break;
		case SENSOR4:
			ui.progressBarSensor4->setValue(distance);
			return;
			break;
		case SENSOR5:
			ui.progressBarSensor5->setValue(distance);
			return;
			break;
		case SENSOR6:
			ui.progressBarSensor6->setValue(distance);
			return;
			break;
		case SENSOR7:
			ui.progressBarSensor7->setValue(distance);
			return;
			break;
		case SENSOR8:
			ui.progressBarSensor8->setValue(distance);
			return;
			break;
		case SENSOR16:
			ui.progressBarSensor16->setValue(distance);
			return;
			break;
	}
*/
}


void Gui::showDrivenDistance(int sensor, int distance)
{
	// show distance value in centimeters in a text label
	// erwartet den value als distance (z.B. in cm)!
	if ((sensor < MOTORSENSOR1) || (sensor > MOTORSENSOR2))
	{
		qDebug("ERROR gui, showDrivenDistance: wrong motor sensor number");
		return;
	}


	switch (sensor)
	{
		case MOTORSENSOR1:
			ui.labelDrivenDistance1->setText(QString("%1 cm").arg(distance));
			return;
			break;
		case MOTORSENSOR2:
			ui.labelDrivenDistance2->setText(QString("%1 cm").arg(distance));
			return;
			break;
	}
}


void Gui::showVoltage(int sensor, float voltage)
{
	if ((sensor < VOLTAGESENSOR1) || (sensor > VOLTAGESENSOR2))
	{
		qDebug("ERROR gui, showVoltage: wrong voltage sensor number");
		return;
	}

// qDebug("voltage %d=%f / MINIMUMVOLTAGE1=%f / MINIMUMVOLTAGE2=%f", sensor, voltage, MINIMUMVOLTAGE1, MINIMUMVOLTAGE2);

	switch (sensor)
	{
		case VOLTAGESENSOR1:
			ui.lblVoltage1->setText(QString("%1").setNum(voltage, 'f', 2).append(" Volt"));
// 			ui.progressBarVoltage1->setValue(voltage);
			
			// change color of the label depending on the voltage
			if (voltage < MINIMUMVOLTAGE1)
			{
// 				ui.groupBoxBattery1->setPalette(QPalette(labelFillColorRed));
// 				ui.lblVoltage1->setPalette(QPalette(labelFillColorRed));
				// change icon
				if (voltage > 0.0)
				{
					ui.lblBattery1->setPixmap(QPixmap(":/images/images/battery-060.png"));
				}
				else
				{
					ui.lblBattery1->setPixmap(QPixmap(":/images/images/battery-missing.png"));
				}
			}
			else
			{
// 				ui.groupBoxBattery1->setPalette(QPalette(labelFillColorGreen));
// 				ui.lblVoltage1->setPalette(QPalette(labelFillColorGreen));
				// change icon
				ui.lblBattery1->setPixmap(QPixmap(":/images/images/battery-100.png"));
			}
			
			return;
			break;
		case VOLTAGESENSOR2:
			ui.lblVoltage2->setText(QString("%1").setNum(voltage, 'f', 2).append(" Volt"));
//			ui.progressBarVoltage2->setValue(voltage);
				
			// change color of the label depending on the voltage
			if (voltage < MINIMUMVOLTAGE2)
			{
// 				ui.groupBoxBattery2->setPalette(QPalette(labelFillColorRed));
// 				ui.lblVoltage2->setPalette(QPalette(labelFillColorRed));
				// change icon
				if (voltage > 0.0)
				{
					ui.lblBattery2->setPixmap(QPixmap(":/images/images/battery-060.png"));
				}
				else
				{
					ui.lblBattery2->setPixmap(QPixmap(":/images/images/battery-missing.png"));
				}
			}
			else
			{
// 				ui.groupBoxBattery2->setPalette(QPalette(labelFillColorGreen));
// 				ui.lblVoltage2->setPalette(QPalette(labelFillColorGreen));
				// change icon
				ui.lblBattery2->setPixmap(QPixmap(":/images/images/battery-100.png"));
			}
			
			return;
			break;
	}
}


void Gui::showAlarm(short int sensor, bool state)
{
	Q_UNUSED(sensor);
	Q_UNUSED(state);
	
/*	
	if ((sensor < SENSOR1) || (sensor > SENSOR16))
	{
		qDebug("error in showAlarm! sensorValue=%d", sensor);
		return;
	}


	switch (sensor)
	{
		case SENSOR1:
			if (state == ON)
			{
				// change sensor-pixmap color to red     [blue = 0, 70, 213]
				//ui.labelSensorPic1->setPalette(QPalette(labelFillColorRed));
				//ui.labelSensorAlarm1->setEnabled(true);
				//ui.frameSensorAlarm1->setPalette(QPalette(labelFillColorRed));
				// red progressBar background
				ui.progressBarSensor1->setPalette(QPalette(labelFillColorRed));
			}
			else
			{
				// change sensor-pixmap color to white
				//ui.labelSensorPic1->setPalette(QPalette(QColor(255,255,255)));
				//ui.labelSensorAlarm1->setEnabled(false);
				//ui.frameSensorAlarm1->setPalette(QPalette(QColor(255,255,255)));
				// progressBar background white
				ui.progressBarSensor1->setPalette(QPalette(QColor(255, 255, 255)));
			}
			return;
			break;
		case SENSOR2:
			if (state == ON)
			{
				// change sensor-pixmap color to red     [blue = 0, 70, 213]
				//ui.labelSensorPic2->setPalette(QPalette(labelFillColorRed));
				//ui.labelSensorAlarm2->setEnabled(true);
				//ui.frameSensorAlarm2->setPalette(QPalette(labelFillColorRed));
				// red progressBar background
				ui.progressBarSensor2->setPalette(QPalette(labelFillColorRed));
			}
			else
			{
				// change sensor-pixmap color to white
				//ui.labelSensorPic2->setPalette(QPalette(QColor(255,255,255)));
				//ui.labelSensorAlarm2->setEnabled(false);
				//ui.frameSensorAlarm2->setPalette(QPalette(QColor(255,255,255)));
				// progressBar background white
				ui.progressBarSensor2->setPalette(QPalette(QColor(255, 255, 255)));
			}
			return;
			break;
		case SENSOR3:
			if (state == ON)
			{
				// change sensor-pixmap color to red     [blue = 0, 70, 213]
				//ui.labelSensorPic3->setPalette(QPalette(labelFillColorRed));
				//ui.labelSensorAlarm3->setEnabled(true);
				//ui.frameSensorAlarm3->setPalette(QPalette(labelFillColorRed));
				// red progressBar background
				ui.progressBarSensor3->setPalette(QPalette(labelFillColorRed));
			}
			else
			{
				// change sensor-pixmap color to white
				//ui.labelSensorPic3->setPalette(QPalette(QColor(255,255,255)));
				//ui.labelSensorAlarm3->setEnabled(false);
				//ui.frameSensorAlarm3->setPalette(QPalette(QColor(255,255,255)));
				// progressBar background white
				ui.progressBarSensor3->setPalette(QPalette(QColor(255, 255, 255)));
			}
			return;
			break;
		case SENSOR4:
			if (state == ON)
			{
				// change sensor-pixmap color to red     [blue = 0, 70, 213]
				//ui.labelSensorPic4->setPalette(QPalette(labelFillColorRed));
				//ui.labelSensorAlarm4->setEnabled(true);
				//ui.frameSensorAlarm4->setPalette(QPalette(labelFillColorRed));
				// red progressBar background
				ui.progressBarSensor4->setPalette(QPalette(labelFillColorRed));
			}
			else
			{
				// change sensor-pixmap color to white
				//ui.labelSensorPic4->setPalette(QPalette(QColor(255,255,255)));
				//ui.labelSensorAlarm4->setEnabled(false);
				//ui.frameSensorAlarm4->setPalette(QPalette(QColor(255,255,255)));
				// progressBar background white
				ui.progressBarSensor4->setPalette(QPalette(QColor(255, 255, 255)));
			}
			return;
			break;
		case SENSOR5:
			if (state == ON)
			{
				// change sensor-pixmap color to red     [blue = 0, 70, 213]
				//ui.labelSensorPic5->setPalette(QPalette(labelFillColorRed));
				//ui.labelSensorAlarm5->setEnabled(true);
				//ui.frameSensorAlarm5->setPalette(QPalette(labelFillColorRed));
				// red progressBar background
				ui.progressBarSensor5->setPalette(QPalette(labelFillColorRed));
			}
			else
			{
				// change sensor-pixmap color to white
				//ui.labelSensorPic5->setPalette(QPalette(QColor(255,255,255)));
				//ui.labelSensorAlarm5->setEnabled(false);
				//ui.frameSensorAlarm5->setPalette(QPalette(QColor(255,255,255)));
				// progressBar background white
				ui.progressBarSensor5->setPalette(QPalette(QColor(255, 255, 255)));
			}
			return;
			break;
		case SENSOR6:
			if (state == ON)
			{
				// change sensor-pixmap color to red     [blue = 0, 70, 213]
				//ui.labelSensorPic6->setPalette(QPalette(labelFillColorRed));
				//ui.labelSensorAlarm6->setEnabled(true);
				//ui.frameSensorAlarm6->setPalette(QPalette(labelFillColorRed));
				// red progressBar background
				ui.progressBarSensor6->setPalette(QPalette(labelFillColorRed));
			}
			else
			{
				// change sensor-pixmap color to white
				//ui.labelSensorPic6->setPalette(QPalette(QColor(255,255,255)));
				//ui.labelSensorAlarm6->setEnabled(false);
				//ui.frameSensorAlarm6->setPalette(QPalette(QColor(255,255,255)));
				// progressBar background white
				ui.progressBarSensor6->setPalette(QPalette(QColor(255, 255, 255)));
			}
			return;
			break;
		case SENSOR7:
			if (state == ON)
			{
				// change sensor-pixmap color to red     [blue = 0, 70, 213]
				//ui.labelSensorPic7->setPalette(QPalette(labelFillColorRed));
				//ui.labelSensorAlarm7->setEnabled(true);
				//ui.frameSensorAlarm7->setPalette(QPalette(labelFillColorRed));
				// red progressBar background
				ui.progressBarSensor7->setPalette(QPalette(labelFillColorRed));
			}
			else
			{
				// change sensor-pixmap color to white
				//ui.labelSensorPic7->setPalette(QPalette(QColor(255,255,255)));
				//ui.labelSensorAlarm7->setEnabled(false);
				//ui.frameSensorAlarm7->setPalette(QPalette(QColor(255,255,255)));
				// progressBar background white
				ui.progressBarSensor7->setPalette(QPalette(QColor(255, 255, 255)));
			}
			return;
			break;
		case SENSOR8:
			if (state == ON)
			{
				// change sensor-pixmap color to red     [blue = 0, 70, 213]
				//ui.labelSensorPic8->setPalette(QPalette(labelFillColorRed));
				//ui.labelSensorAlarm8->setEnabled(true);
				//ui.frameSensorAlarm8->setPalette(QPalette(labelFillColorRed));
				// red progressBot background
				ui.progressBarSensor8->setPalette(QPalette(labelFillColorRed));
			}
			else
			{
				// change sensor-pixmap color to white
				//ui.labelSensorPic8->setPalette(QPalette(QColor(255,255,255)));
				//ui.labelSensorAlarm8->setEnabled(false);
				//ui.frameSensorAlarm8->setPalette(QPalette(QColor(255,255,255)));
				// progressBar background white
				ui.progressBarSensor8->setPalette(QPalette(QColor(255, 255, 255)));
			}
			return;
			break;
		case SENSOR16:
			if (state == ON)
			{
				// change sensor-pixmap color to red     [blue = 0, 70, 213]
				//ui.labelSensorPic16->setPalette(QPalette(labelFillColorRed));
				//ui.labelSensorAlarm16->setEnabled(true);
				//ui.frameSensorAlarm16->setPalette(QPalette(labelFillColorRed));
				ui.progressBarSensor16->setPalette(QPalette(labelFillColorRed));
			}
			else
			{
				// change sensor-pixmap color to white
				//ui.labelSensorPic16->setPalette(QPalette(QColor(255,255,255)));
				//ui.labelSensorAlarm16->setEnabled(false);
				//ui.frameSensorAlarm16->setPalette(QPalette(QColor(255,255,255)));
				ui.progressBarSensor16->setPalette(QPalette(QColor(255, 255, 255)));
			}
			return;
			break;
	}
*/
}


void Gui::showMotorStatus(unsigned char motor, bool power, unsigned char direction)
{
	switch (motor)
	{
		//-------------
		case MOTOR1:
		//-------------
			switch (power)
			{
				case ON:
					ui.btnPower1->setPalette(QPalette(labelFillColorGreen));
					ui.btnDirection1->setPalette(QPalette(labelFillColorGreen));
					break;
				case OFF:
					ui.btnPower1->setPalette(QPalette(QColor(255, 255, 255))); // TODO: which color instead of white
					ui.btnDirection1->setPalette(QPalette(QColor(255, 255, 255)));
					break;
			}

			switch (direction)
			{
				case CLOCKWISE:
					ui.btnDirection1->setText("BACKWARD");
					ui.btnDirection1->setPalette(QPalette(labelFillColorGreen));
					break;
				case COUNTERCLOCKWISE:
					ui.btnDirection1->setText("FORWARD");
					ui.btnDirection1->setPalette(QPalette(labelFillColorGreen));
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
					ui.btnPower2->setPalette(QPalette(labelFillColorGreen));
					ui.btnDirection2->setPalette(QPalette(labelFillColorGreen));
					break;
				case OFF:
					ui.btnPower2->setPalette(QPalette(QColor(255, 255, 255))); // TODO: which color instead of white
					ui.btnDirection2->setPalette(QPalette(QColor(255, 255, 255)));
					break;
			}

			switch (direction)
			{
				case CLOCKWISE:
					ui.btnDirection2->setText("BACKWARD");
					ui.btnDirection2->setPalette(QPalette(labelFillColorGreen));
					break;
				case COUNTERCLOCKWISE:
					ui.btnDirection2->setText("FORWARD");
					ui.btnDirection2->setPalette(QPalette(labelFillColorGreen));
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
					ui.btnPower3->setPalette(QPalette(labelFillColorGreen));
					ui.btnDirection3->setPalette(QPalette(labelFillColorGreen));
					break;
				case OFF:
					ui.btnPower3->setPalette(QPalette(QColor(255, 255, 255))); // TODO: which color instead of white
					ui.btnDirection3->setPalette(QPalette(QColor(255, 255, 255)));
					break;
			}

			switch (direction)
			{
				case CLOCKWISE:
					ui.btnDirection3->setText("BACKWARD");
					ui.btnDirection3->setPalette(QPalette(labelFillColorGreen));
					break;
				case COUNTERCLOCKWISE:
					ui.btnDirection3->setText("FORWARD");
					ui.btnDirection3->setPalette(QPalette(labelFillColorGreen));
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
					ui.btnPower4->setPalette(QPalette(labelFillColorGreen));
					ui.btnDirection4->setPalette(QPalette(labelFillColorGreen));
					break;
				case OFF:
					ui.btnPower4->setPalette(QPalette(QColor(255, 255, 255))); // TODO: which color instead of white
					ui.btnDirection4->setPalette(QPalette(QColor(255, 255, 255)));
					break;
			}

			switch (direction)
			{
				case CLOCKWISE:
					ui.btnDirection4->setText("BACKWARD");
					ui.btnDirection4->setPalette(QPalette(labelFillColorGreen));
					break;
				case COUNTERCLOCKWISE:
					ui.btnDirection4->setText("FORWARD");
					ui.btnDirection4->setPalette(QPalette(labelFillColorGreen));
					break;
				case SAME:
					break;
			}
			break;
	} // switch
}


void Gui::showLaserFrontAngles(int largestFreeAreaStart, int largestFreeAreaEnd, int centerOfFreeWay, float width)
{
	// TODO: do all this stuff for the rear scanner?

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
//	ui.lblLaserFrontFreeEnd->setText(QString("%1 deg").arg(largestFreeAreaEnd));
	ui.lblLaserFrontFreeCenter->setText(QString("%1 deg").arg(centerOfFreeWay));
	
	
	// show width in cm with one decimal place (Nachkommastelle)
	if (width != -1)
	{
		ui.lblLaserFrontFreeWidth->setText(QString("%1").setNum(width, 'f', 1).append(" cm"));
	}
	else
	{
		ui.lblLaserFrontFreeWidth->setText("oo");
	}
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

void Gui::showFaceTrackDirection(QString direction)
{
	if (direction == "UP")
	{
		ui.lblFaceTrackTo->setPixmap(QPixmap(":/images/images/up.png"));
		return;
	}

	if (direction == "UPLEFT")
	{
		ui.lblFaceTrackTo->setPixmap(QPixmap(":/images/images/upleft.png"));
		return;
	}

	if (direction == "UPRIGHT")
	{
		ui.lblFaceTrackTo->setPixmap(QPixmap(":/images/images/upright.png"));
		return;
	}

	if (direction == "DOWN")
	{
		ui.lblFaceTrackTo->setPixmap(QPixmap(":/images/images/down.png"));
		return;
	}

	if (direction == "DOWNLEFT")
	{
		ui.lblFaceTrackTo->setPixmap(QPixmap(":/images/images/downleft.png"));
		return;
	}

	if (direction == "DOWNRIGHT")
	{
		ui.lblFaceTrackTo->setPixmap(QPixmap(":/images/images/downright.png"));
		return;
	}

	if (direction == "LEFT")
	{
		ui.lblFaceTrackTo->setPixmap(QPixmap(":/images/images/left.png"));
		return;
	}

	if (direction == "RIGHT")
	{
		ui.lblFaceTrackTo->setPixmap(QPixmap(":/images/images/right.png"));
		return;
	}

	if (direction == "NONE")
	{
		ui.lblFaceTrackTo->setPixmap(QPixmap(":/images/images/middle.png"));
		return;
	}
}


void Gui::on_btnSavePicture_clicked()
{
	saveCamImage();
	appendLog("Picture saved");
}


void Gui::showFaceTrackData(int faces, int faceX, int faceY, int faceRadius, int lastFaceX, int lastFaceY)
{
	// show coordinates of the first detected face
	ui.lblFaces->setNum(faces);

	ui.lblFaceX->setNum(faceX);
	ui.lblFaceY->setNum(faceY);
	ui.lblFaceRadius->setNum(faceRadius);

	ui.lblLastFaceX->setNum(lastFaceX);
	ui.lblLastFaceY->setNum(lastFaceY);
}


void Gui::disableFaceDetection()
{
	appendLog("Face detection disabled!");
	ui.checkBoxFaceDetection->setEnabled(false);
	ui.checkBoxFaceTracking->setEnabled(false);
}


void Gui::disableCamera()
{
	appendLog("Camera disabled!");
	ui.dockCamera->setEnabled(false);
	ui.dockCamera->hide();
}


void Gui::setCamImageData(int width, int height, int pixeldepth)
{
	// tell the OpenGLContext the image data
	ui.frameCamera->setImageData(width, height, pixeldepth);
	appendLog("Camera image width, height and pixel depth set.");
}


//void Gui::setCamImage(QImage* image)
void Gui::setCamImage(IplImage* frame)
{
	// set image from OpenGL context to Qt frame!
	ui.frameCamera->setImage((unsigned char*)frame->imageData);

	// try it with qimage instead of iplImage...
	//ui.lblCamera->setPixmap(pixmap.fromImage(*(image)));

/*
	// save pic, when ckecked in GUI
	if ( ui.checkBoxAutoSave->isChecked() )
	{
		saveCamImage();
	}
*/
}


void Gui::saveCamImage(void)
{
	//---------------------------
	// grab cam pic from Qt-GUI
	//---------------------------
	static QDateTime timestamp;


	cameraPicToSave = QPixmap::grabWindow(QWidget::winId(), ui.dockCamera->x()+ui.frameCamera->x()+9, ui.dockCamera->y()+ui.frameCamera->y()+95, ui.frameCamera->width(), ui.frameCamera->height());
	//cameraPicToSave = QPixmap::grabWidget(ui.frameCamera); //doesnt work
	//cameraPicToSave = QPixmap::grabWidget(ui.dockCamera); //doesnt work

	//--------------------------------------------------------------------------------------
	// save image to disk, but not within the same seond (same timestamp like last picture)
	//--------------------------------------------------------------------------------------
	if (QDateTime::currentDateTime() != timestamp)
	{
		// get the actual date and time
		timestamp = QDateTime::currentDateTime();
		QString filename = timestamp.toString("yyyy-MM-dd_hh-mm-ss");
		filename += ".png";

		// save
		cameraPicToSave.save(filename.toAscii(), "PNG");
		//appendLog(tr("Picture \"%1\" saved.").arg(filename));
	}
}


void Gui::setPlotData1(double *xval, double *yval, int size)
{
	//---------------
	// curveCurrent1
	//---------------

	// set curve with data
	curveCurrent1.setData(xval, yval, size);

	// attach data to curve
	curveCurrent1.attach(ui.qwtPlotCurrent1);

	// after changing the values, replot the curve
	ui.qwtPlotCurrent1->replot();
}


void Gui::setPlotData2(double *xval, double *yval, int size)
{
	//---------------
	// curveCurrent2
	//---------------

	// set curve with data
	curveCurrent2.setData(xval, yval, size);

	// attach data to curve
	curveCurrent2.attach(ui.qwtPlotCurrent1); // add curve to qwtPlot 1 !!

	// after changing the values, replot the curve
	ui.qwtPlotCurrent1->replot(); // replot qwtPlot 1 !!
}


void Gui::setPlotData3(double *xval, double *yval, int size)
{
	//---------------
	// curveCurrent3
	//---------------

	// set curve with data
	curveCurrent3.setData(xval, yval, size);

	// attach data to curve
	curveCurrent3.attach(ui.qwtPlotCurrent2); // curve 3 to qwtPlot 2

	// after changing the values, replot the curve
	ui.qwtPlotCurrent2->replot();
}


void Gui::setPlotData4(double *xval, double *yval, int size)
{
	//---------------
	// curveCurrent4
	//---------------

	// set curve with data
	curveCurrent4.setData(xval, yval, size);

	// attach data to curve
	curveCurrent4.attach(ui.qwtPlotCurrent2); // curve 4 to qwtPlot 2

	// after changing the values, replot the curve
	ui.qwtPlotCurrent2->replot();  // replot qwtPlot 2 !!
}


void Gui::setPlotData5(double *xval, double *yval, int size)
{
	//---------------
	// curveVoltage1
	//---------------

	// set curve with data
	curveVoltage1.setData(xval, yval, size);

	// attach data to curve
	curveVoltage1.attach(ui.qwtPlotVoltage1);

	// after changing the values, replot the curve
	ui.qwtPlotVoltage1->replot();
}


void Gui::setPlotData6(double *xval, double *yval, int size)
{
	//---------------
	// curveVoltage2
	//---------------

	// set curve with data
	curveVoltage2.setData(xval, yval, size);

	// attach data to curve
	curveVoltage2.attach(ui.qwtPlotVoltage2);

	// after changing the values, replot the curve
	ui.qwtPlotVoltage2->replot();
}


void Gui::on_btnPower1_toggled(bool checked)
{
	if (checked)
	{
		// motor on
		ui.btnPower1->setPalette(QPalette(labelFillColorGreen));
		
		if (ui.btnDirection1->isChecked())
		{
			emit drive(MOTOR1BW);
		}
		else
		{
			emit drive(MOTOR1FW);
		}
	}
	else
	{
		// motor off
		ui.btnPower1->setPalette(QPalette(QColor(255, 255, 255))); // TODO: use default gray instead of white!
		emit drive(MOTOR1OFF);
	}
}


void Gui::on_btnPower2_toggled(bool checked)
{
	if (checked)
	{
		// motor on
		ui.btnPower2->setPalette(QPalette(labelFillColorGreen));
		
		if (ui.btnDirection2->isChecked())
		{
			emit drive(MOTOR2BW);
		}
		else
		{
			emit drive(MOTOR2FW);
		}
	}
	else
	{
		// motor off
		ui.btnPower2->setPalette(QPalette(QColor(255, 255, 255))); // TODO: use default gray instead of white!
		emit drive(MOTOR2OFF);
	}
}


void Gui::on_btnPower3_toggled(bool checked)
{
	if (checked)
	{
		// motor on
		ui.btnPower3->setPalette(QPalette(labelFillColorGreen));
		
		if (ui.btnDirection3->isChecked())
		{
			emit drive(MOTOR3BW);
		}
		else
		{
			emit drive(MOTOR3FW);
		}
	}
	else
	{
		// motor off
		ui.btnPower3->setPalette(QPalette(QColor(255, 255, 255))); // TODO: use default gray instead of white!
		emit drive(MOTOR3OFF);
	}
}


void Gui::on_btnPower4_toggled(bool checked)
{
	if (checked)
	{
		// motor on
		ui.btnPower4->setPalette(QPalette(labelFillColorGreen));
		
		if (ui.btnDirection4->isChecked())
		{
			emit drive(MOTOR4BW);
		}
		else
		{
			emit drive(MOTOR4FW);
		}
	}
	else
	{
		// motor off
		ui.btnPower4->setPalette(QPalette(QColor(255, 255, 255))); // TODO: use default gray instead of white!
		emit drive(MOTOR4OFF);
	}
}


void Gui::on_btnDirection1_toggled(bool checked)
{
	if (checked)
	{
		// backward
		ui.btnDirection1->setPalette(QPalette(labelFillColorGreen));
		
		if (ui.btnPower1->isChecked())
		{
			emit drive(MOTOR1BW);
		}
	}
	else
	{
		// forward
		ui.btnDirection1->setPalette(QPalette(QColor(255, 255, 255))); // TODO: which color instead of white
		
		if (ui.btnPower1->isChecked())
		{
			emit drive(MOTOR1FW);
		}
	}
}


void Gui::on_btnDirection2_toggled(bool checked)
{
	if (checked)
	{
		// backward
		ui.btnDirection2->setPalette(QPalette(labelFillColorGreen));
		
		if (ui.btnPower2->isChecked())
		{
			emit drive(MOTOR1BW);
		}
	}
	else
	{
		// forward
		ui.btnDirection2->setPalette(QPalette(QColor(255, 255, 255))); // TODO: which color instead of white
		
		if (ui.btnPower2->isChecked())
		{
			emit drive(MOTOR2FW);
		}
	}
}


void Gui::on_btnDirection3_toggled(bool checked)
{
	if (checked)
	{
		// backward
		ui.btnDirection3->setPalette(QPalette(labelFillColorGreen));
		
		if (ui.btnPower3->isChecked())
		{
			emit drive(MOTOR3BW);
		}
	}
	else
	{
		// forward
		ui.btnDirection3->setPalette(QPalette(QColor(255, 255, 255))); // TODO: which color instead of white
		
		if (ui.btnPower3->isChecked())
		{
			emit drive(MOTOR3FW);
		}
	}
}


void Gui::on_btnDirection4_toggled(bool checked)
{
	if (checked)
	{
		// backward
		ui.btnDirection4->setPalette(QPalette(labelFillColorGreen));
		
		if (ui.btnPower4->isChecked())
		{
			emit drive(MOTOR4BW);
		}
	}
	else
	{
		// forward
		ui.btnDirection4->setPalette(QPalette(QColor(255, 255, 255))); // TODO: which color instead of white
		
		if (ui.btnPower4->isChecked())
		{
			emit drive(MOTOR4FW);
		}
	}
}


void Gui::on_actionRemote_activated()
{
	// if remote control is enabled, disable some controls in the GUI
	if (ui.actionRemote->isChecked())
	{
		ui.actionDrive->setEnabled(false);
		emit enableRemoteControlListening(true);
	}
	else
	{
		if (robotIsOn)
		{
			ui.actionDrive->setEnabled(true);
		}
		emit enableRemoteControlListening(false);
	}
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
	x = laserXPos -  ( pixmapBot1->pixmap().width()  / 2 / startScale * lastZoom);
	y = laserYPos -  ( pixmapBot1->pixmap().height() / 2 / startScale * lastZoom);

	// horizontal center
	pixmapBot1->setPos(x, y);
	pixmapBot2->setPos(x, y);

	
	//------------------------------------------------------
	// change the x and y position of the FRONT laser lines
	//------------------------------------------------------
	x = laserXPos;
	y = laserYPos - (INITIALLASERYPOSFRONT / startScale * lastZoom);

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


void Gui::on_checkBoxMirror_stateChanged(int state)
{
	// QtGL class!!
	ui.frameCamera->enableMirrorMode(state);
}


void Gui::on_checkBoxFaceDetection_stateChanged(int state)
{
	// en/disable face *tracking* checkbox
	if (state == Qt::Checked)
	{
		ui.checkBoxFaceTracking->setEnabled(true);
	}
	else
	{
		ui.checkBoxFaceTracking->setEnabled(false);
	}
}


void Gui::on_checkBoxFaceTracking_stateChanged(int state)
{
	//
	// This slot is only for changing the label!
	// Not for enabling face detection!
	// This is done via signal slot!
	//

	// change face tracking direction icon to middle
	if (state == Qt::Unchecked)
	{
		ui.lblFaceTrackTo->setPixmap(QPixmap(":/images/images/middle.png"));
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
	y = laserYPos; // INITIALLASERYPOSFRONT has no effect here, only in on_sliderZoom_valueChanged !!

	
	//--------------
	// REAR laser
	//--------------
	// rotate every laser line by one degree
	// Be sure to rotate that the middle of the laserAngle is always at a vertical 90 degree in the scene!
	// (Rotate laser line counterclockwise by the half of the laserAngle)
	for (int i=0, angle = -(laserscannerAngleRear/2); i<laserLineListRear->size(); i++, angle++)
	{
		laserLineListRear->at(i)->rotate(angle);
	}


	y = laserYPos;// INITIALLASERYPOSREAR has no effect here, only in on_sliderZoom_valueChanged !!


	//--------------
	// FRONT laser
	//--------------
	// rotate every laser line by one degree
	// Be sure to rotate that the middle of the laserAngle is always at a vertical 90 degree in the scene!
	// (Rotate laser line counterclockwise by 180 degrees and further by the half of the laserAngle)
	for (int i=0, angle = -(180 + (laserscannerAngleFront/2)); i<laserLineListFront->size(); i++, angle++)
	{
		laserLineListFront->at(i)->rotate(angle);
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
	qreal x = 0.0;
	qreal y = 0.0;
	int r = 0;
	qreal xKart;
	qreal yKart;
	int angle = 0;
	static qreal widthLinePosX1 = 0.0;
	static qreal widthLinePosX2 = 0.0;
	static qreal widthLinePosY1 = 0.0;
	static qreal widthLinePosY2 = 0.0;
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
		laserLineListFront->at(i)->setToolTip( QString("%1 m  /  %2 deg  /  Flag=%3  /  %4 Pixel").arg(laserScannerValues[i]).arg(i).arg(laserScannerFlags[i]).arg(laserLineLength) );

		// convert from polar to Cartesian coordinates
		// minus the current "middle position" of the robot within the gui frame
		// WARNING: "cos" functions use radians!! so we convert the degrees to radions here!
		xKart = laserLineLength * cos( (double) i / 360.0 * 2.0 * M_PI  );
		yKart = laserLineLength * sin( (double) i / 360.0 * 2.0 * M_PI  );

		x = laserLineListFront->at(i)->scenePos().x();
		y = laserLineListFront->at(i)->scenePos().y();


		// draw the first (left) width circle
		if (i == mLargestFreeAreaStartFront)
		{
			angle = i;
			r = laserLineListFront->at(angle)->line().length();

			// convert from polar to kartesic coordinates
			// sin and cos are swapped here because of a different x, y and angle orientation than in a normal kartesic coordination system!
			xKart = r * sin( angle * M_PI / 180 );
			yKart = r * cos( angle * M_PI / 180 );

			// make x negative because of a different x, y and angle orientation than in a normal kartesic coordination system!
			// (difference between real world and computer)
			xKart = xKart * -1;

			// add the "start coordinates" (the laser line origin)
			xKart += x - (widthCirclesWidth/2);
			yKart += y - (widthCirclesWidth/2);

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
			// sin and cos are swapped here because of a different x, y and angle orientation than in a normal kartesic coordination system!
			xKart = r * sin( angle * M_PI / 180 );
			yKart = r * cos( angle * M_PI / 180 );

			// make x negative because of a different x, y and angle orientation than in a normal kartesic coordination system!
			// (difference between real world and computer)
			xKart = xKart * -1;

			// add the "start coordinates" (the laser line origin)
			xKart += x - (widthCirclesWidth/2);
			yKart += y - (widthCirclesWidth/2);

			//------------------------------------------------------------------
			// store this positions for the width line
			//------------------------------------------------------------------
			// corrected by the circle radius, to get the middle of the circle
			widthLinePosX1 = xKart +  (widthCirclesWidth/2);
			widthLinePosY1 = yKart +  (widthCirclesWidth/2);

			// set the circle position!
			widthLeftCircle->setPos(xKart, yKart);
		}
		else
		{
			// draw the next (right) width circle
			if (i == mLargestFreeAreaEndFront)
			{
				angle = i;
				r = laserLineListFront->at(angle)->line().length();

				// convert from polar to kartesic coordinates
				// sin and cos are swapped here because of a different x, y and angle orientation than in a normal kartesic coordination system!
				xKart = r * sin( angle * M_PI / 180 );
				yKart = r * cos( angle * M_PI / 180 );

				// make x negative because of a different x, y and angle orientation than in a normal kartesic coordination system!
				// (difference between real world and computer)
				xKart = xKart * -1;

				// add the "start coordinates" (the laser line origin)
				xKart += x - (widthCirclesWidth/2);
				yKart += y - (widthCirclesWidth/2);


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
				// sin and cos are swapped here because of a different x, y and angle orientation than in a normal kartesic coordination system!
				xKart = r * sin( angle * M_PI / 180 );
				yKart = r * cos( angle * M_PI / 180 );

				// make x negative because of a different x, y and angle orientation than in a normal kartesic coordination system!
				// (difference between real world and computer)
				xKart = xKart * -1;

				// add the "start coordinates" (the laser line origin)
				xKart += x - (widthCirclesWidth/2);
				yKart += y - (widthCirclesWidth/2);

				//------------------------------------------------------------------
				// store this positions for the width line
				//------------------------------------------------------------------
				// corrected by the circle radius, to get the middle of the circle
				widthLinePosX2 = xKart +  (widthCirclesWidth/2);
				widthLinePosY2 = yKart +  (widthCirclesWidth/2);

				// draw it
				widthLine->setLine(widthLinePosX1, widthLinePosY1, widthLinePosX2, widthLinePosY2);

				// set the circle position!
				widthRightCircle->setPos( widthLinePosX2 - (widthCirclesWidth/2), widthLinePosY2 - (widthCirclesWidth/2) );
			}
		}
	}

	// make the width objects visible
	widthLeftCircle->setVisible(true);
	widthRightCircle->setVisible(true);
	widthLine->setVisible(true);
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
	// create all laser lines(e.g.180 or 270, one for each degree)
	// the rotation will be done in the initLaserView() !
	for (int i=0; i<laserscannerAngleFront; i++)
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
	for (int i=0; i<laserscannerAngleRear; i++)
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
//	pixmapBot2->setVisible(false);
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
    widthCirclesWidth = 20;  // TODO: which radius?
		
	widthLeftCircle = new QGraphicsEllipseItem();
	widthRightCircle = new QGraphicsEllipseItem();
	widthLine = new QGraphicsLineItem();

	// set the start angle of the circle
	widthLeftCircle->setStartAngle(0*16);
	widthRightCircle->setStartAngle(0*16);
	// set the span angle of the circle
	widthLeftCircle->setSpanAngle(360*16);
	widthRightCircle->setSpanAngle(360*16);

	// set the color
	widthLeftCircle->setPen(QPen(Qt::blue)); // TODO: define circle the color!
	widthRightCircle->setPen(QPen(Qt::blue)); // TODO: define circle the color!
	widthLine->setPen(QPen(Qt::blue)); // TODO: define circle the color!

	// setting to the lowest layer level
	widthLeftCircle->setZValue(1);
	widthRightCircle->setZValue(1);
	widthLine->setZValue(1);

	// draw a circle to see the coordinates for the 'drive-tru width'
	// change the width and height
	widthLeftCircle->setRect(0, 0, widthCirclesWidth, widthCirclesWidth);
	widthRightCircle->setRect(0, 0, widthCirclesWidth, widthCirclesWidth);

	// make them unvisible untill we have a change in thre laser line length (other method of this class)
	widthLeftCircle->setVisible(false);
	widthRightCircle->setVisible(false);
	widthLine->setVisible(false);

	// add the item to the scene
	scene->addItem(widthLeftCircle);
	scene->addItem(widthRightCircle);
	scene->addItem(widthLine);
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


void Gui::initPlots()
{
	QwtText plotTitle;
	// get the current application font
	QFont applicationFont = QApplication::font();
	int fontSize = 0;
	QColor col;
	
	
	//--------------------------------------
	// plot curve "MOTOR CURRENT" 1 + 2
	//--------------------------------------
	// set this font for the plot widget
	ui.qwtPlotCurrent1->setAxisFont(QwtPlot::xBottom, applicationFont);
	ui.qwtPlotCurrent1->setAxisFont(QwtPlot::yLeft, applicationFont);
	ui.qwtPlotCurrent1->setAxisFont(QwtPlot::axisCnt, applicationFont);
	
	// set title and font of plot widget
	fontSize = applicationFont.pointSize();
	plotTitle = QString("Motor 1 + 2");
	// increase title by 2 points
	applicationFont.setPointSize(++fontSize);
	applicationFont.setBold(true);
	plotTitle.setFont(applicationFont);
	ui.qwtPlotCurrent1->setTitle(plotTitle);
	
	// Set axis scale (instead of using autoscale, which is default)
	// time (60 sec)
	ui.qwtPlotCurrent1->setAxisScale(QwtPlot::xBottom, 0, 60.0, 10);

	curveCurrent1.setRenderHint(QwtPlotItem::RenderAntialiased);
	curveCurrent1.setPen(QPen(labelFillColorBlue));
// 	curveCurrent1.setBrush(labelFillColorBlue); // this fills the area under the line


	//--------------------------------------
	// plot curve "MOTOR CURRENT" 3 + 4
	//--------------------------------------
	// get the current application font
	applicationFont = QApplication::font();
	
	// set this font for the plot widget
	ui.qwtPlotCurrent2->setAxisFont(QwtPlot::xBottom, applicationFont);
	ui.qwtPlotCurrent2->setAxisFont(QwtPlot::yLeft, applicationFont);
	ui.qwtPlotCurrent2->setAxisFont(QwtPlot::axisCnt, applicationFont);
	
	// set title and font of plot widget
	fontSize = applicationFont.pointSize();
	plotTitle = QString("Motor 3 + 4");
	// increase title by 2 points
	applicationFont.setPointSize(++fontSize);
	applicationFont.setBold(true);
	plotTitle.setFont(applicationFont);
	ui.qwtPlotCurrent2->setTitle(plotTitle);

	// Set axis scale (instead of using autoscale, which is default)
	// time (60 sec)
	ui.qwtPlotCurrent2->setAxisScale(QwtPlot::xBottom, 0, 60.0, 10);

	curveCurrent2.setRenderHint(QwtPlotItem::RenderAntialiased);
	curveCurrent2.setPen(QPen(labelFillColorRed));
// 	curveCurrent2.setBrush(labelFillColorRed); // this fills the area under the line

	// plot curve "MOTOR CURRENT" 3
	curveCurrent3.setRenderHint(QwtPlotItem::RenderAntialiased);
	curveCurrent3.setPen(QPen(labelFillColorBlue));
// 	curveCurrent3.setBrush(labelFillColorBlue); // this fills the area under the line

	// plot curve "MOTOR CURRENT" 4
	curveCurrent4.setRenderHint(QwtPlotItem::RenderAntialiased);
	curveCurrent4.setPen(QPen(labelFillColorRed));
// 	curveCurrent4.setBrush(labelFillColorRed); // this fills the area under the line
	
	
	//--------------------------------------
	// plot curve "VOLTAGE 1"
	//--------------------------------------
	// get the current application font
	applicationFont = QApplication::font();
	// set this font for the plot widget
	ui.qwtPlotVoltage1->setAxisFont(QwtPlot::xBottom, applicationFont);
	ui.qwtPlotVoltage1->setAxisFont(QwtPlot::yLeft, applicationFont);
	ui.qwtPlotVoltage1->setAxisFont(QwtPlot::axisCnt, applicationFont);
	
	// set title and font of plot widget
	fontSize = applicationFont.pointSize();
	plotTitle = QString("12 V Battery");
	// increase title by 2 points
	applicationFont.setPointSize(++fontSize);
	applicationFont.setBold(true);
	plotTitle.setFont(applicationFont);
	ui.qwtPlotVoltage1->setTitle(plotTitle);
	
	// Set axis scale (instead of using autoscale, which is default)
	// maximum voltage value at which the axis should and, and the steps between each divider
	ui.qwtPlotVoltage1->setAxisScale(QwtPlot::yLeft, 0, MAXIMUMPLOTVOLTAGE1, 2.0); // TODO: put a nice step value here, to ini or to config mene
	
	// Set axis scale (instead of using autoscale, which is default)
	// time (60 sec)
	ui.qwtPlotVoltage1->setAxisScale(QwtPlot::xBottom, 0, 60.0, 10);

	curveVoltage1.setRenderHint(QwtPlotItem::RenderAntialiased);
 	curveVoltage1.setPen(QPen(labelFillColorBlue));
// 	curveVoltage1.setBrush(labelFillColorBlue); // this fills the area under the line
	
	
	//--------------------------------------
	// plot curve "MOTOR VOLTAGE" 3 + 4
	//--------------------------------------
	// get the current application font
	applicationFont = QApplication::font();
	// set this font for the plot widget
	ui.qwtPlotVoltage2->setAxisFont(QwtPlot::xBottom, applicationFont);
	ui.qwtPlotVoltage2->setAxisFont(QwtPlot::yLeft, applicationFont);
	ui.qwtPlotVoltage2->setAxisFont(QwtPlot::axisCnt, applicationFont);
	
	// set title and font of plot widget
	fontSize = applicationFont.pointSize();
	plotTitle = QString("24 V Battery");
	// increase title by 2 points
	applicationFont.setPointSize(++fontSize);
	applicationFont.setBold(true);
	plotTitle.setFont(applicationFont);
	ui.qwtPlotVoltage2->setTitle(plotTitle);
	
	// Set axis scale (instead of using autoscale, which is default)
	// maximum voltage value at which the axis should and, and the steps between each divider
	ui.qwtPlotVoltage2->setAxisScale(QwtPlot::yLeft, 0, MAXIMUMPLOTVOLTAGE2, 4.0); // TODO: put a nice step value here, to ini or to config mene
	
	// Set axis scale (instead of using autoscale, which is default)
	// time (60 sec)
	ui.qwtPlotVoltage2->setAxisScale(QwtPlot::xBottom, 0, 60.0, 10);

	curveVoltage2.setRenderHint(QwtPlotItem::RenderAntialiased);
	curveVoltage2.setPen(QPen(labelFillColorBlue));
// 	curveVoltage2.setBrush(labelFillColorBlue); // this fills the area under the line
}


/*
void Gui::initCompass()
{
	int c;

	QPalette colorGroup;
	for ( c = 0; c < QPalette::NColorRoles; c++ )
	{
		colorGroup.setColor((QPalette::ColorRole)c, QColor());
	}

#if QT_VERSION < 0x040000
	colorGroup.setColor(QPalette::Base, backgroundColor().light(120));
#else
	colorGroup.setColor(QPalette::Base, palette().color(backgroundRole()).light(120));
#endif
	colorGroup.setColor(QPalette::Foreground, colorGroup.color(QPalette::Base));


	 // A 2D compass showing another needle. No. 4
	ui.qwtCompass->setScaleOptions(QwtDial::ScaleTicks | QwtDial::ScaleLabel);
	ui.qwtCompass->setScaleTicks(0, 0, 3);

	ui.qwtCompass->setNeedle(new QwtCompassMagnetNeedle( QwtCompassMagnetNeedle::TriangleStyle, Qt::white, Qt::red));
	ui.qwtCompass->setValue(0.0);


	// get palette
	QPalette newPalette = ui.qwtCompass->palette();

	for ( c = 0; c < QPalette::NColorRoles; c++ )
	{
		if ( colorGroup.color((QPalette::ColorRole)c).isValid() )
		{
			for ( int cg = 0; cg < QPalette::NColorGroups; cg++ )
			{
				newPalette.setColor( (QPalette::ColorGroup)cg, (QPalette::ColorRole)c, colorGroup.color((QPalette::ColorRole)c));
			}
		}
	}

	for ( int i = 0; i < QPalette::NColorGroups; i++ )
	{
		QPalette::ColorGroup cg = (QPalette::ColorGroup)i;

		const QColor light = newPalette.color(cg, QPalette::Base).light(170);
		const QColor dark = newPalette.color(cg, QPalette::Base).dark(170);
		const QColor mid = ui.qwtCompass->frameShadow() == QwtDial::Raised ? newPalette.color(cg, QPalette::Base).dark(110) : newPalette.color(cg, QPalette::Base).light(110);
		newPalette.setColor(cg, QPalette::Dark, dark);
		newPalette.setColor(cg, QPalette::Mid, mid);
		newPalette.setColor(cg, QPalette::Light, light);
	}

	// set new palette
	ui.qwtCompass->setPalette(newPalette);
}
*/


void Gui::showCompassData(float x, float y, float z, float heading)
{
	// also formats the string to 1 digits precision!
	ui.lblCompassX->setText( QString("%1").setNum(x, 'f', 1).append(" deg") );
	ui.lblCompassY->setText( QString("%1").setNum(y, 'f', 1).append(" deg") );
	ui.lblCompassZ->setText( QString("%1").setNum(z, 'f', 1).append(" deg") );
	ui.lblCompassHeading->setText( QString("%1").setNum(heading, 'f', 1).append(" deg") );
	
// 	ui.qwtCompass->setValue(x);
	
	// set the 3D OpenGL compass!
	ui.frameCompass->setAllRotations(x, y, z); // TODO: and what to do with the 'heading'?
}


void Gui::setLEDHeartbeat(unsigned char state)
{
	switch (state)
	{
		case RED:
			ui.lblLEDHeartbeat->setPixmap(QPixmap(":/images/images/led_red.gif"));
			break;
		case GREEN:
			ui.lblLEDHeartbeat->setPixmap(QPixmap(":/images/images/led_green.gif"));
			break;
		case LEDOFF:
			ui.lblLEDHeartbeat->setPixmap(QPixmap(":/images/images/led_gray.gif"));
			break;
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
