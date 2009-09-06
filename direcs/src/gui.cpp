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

	// define some nice green and red colors
	labelFillColorGreen = QColor(64, 255, 64);
	labelFillColorRed   = QColor(255, 64, 64);
	labelFillColorBlue  = QColor(64, 64, 255);

	//-------------------------------------------------------
	// startup the GUI
	//-------------------------------------------------------
	ui.setupUi(this);

	// set battery power labels to green
	ui.groupBoxBattery1->setPalette(QPalette(labelFillColorGreen));
	ui.groupBoxBattery2->setPalette(QPalette(labelFillColorGreen));
	
	ui.lblVoltage1->setPalette(QPalette(labelFillColorGreen));
	ui.lblVoltage2->setPalette(QPalette(labelFillColorGreen));

	//----------------------------------------------------------------------------
	// Voltage stuff (progressBars)
	//----------------------------------------------------------------------------
	ui.progressBarVoltage1->setRange(0, MAXIMUMVOLTAGE1);
	ui.progressBarVoltage2->setRange(0, MAXIMUMVOLTAGE2);
	
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
	initCompass();
	initCompassView(); // crate the graphics scene object, the compass axes etc.

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
	
	// delete compass stuff
	//delete compassWidget;
	/*
	delete zAxis;
	delete yAxis;
	delete xAxis;
	delete compassScene;
	*/
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
	//ui.btnExecuteScript->setText("Execute s&cript");

	// set labels back in default position
	ui.lblPower1->setEnabled(false);
	ui.lblPower1->setPalette(QPalette(QColor(255, 255, 255)));
	ui.lblPower2->setEnabled(false);
	ui.lblPower2->setPalette(QPalette(QColor(255, 255, 255)));

	ui.lblDirection1->setText("FORWARD");
	ui.lblDirection1->setEnabled(false);
	ui.lblDirection1->setPalette(QPalette(QColor(255, 255, 255)));
	ui.lblDirection2->setText("FORWARD");
	ui.lblDirection2->setEnabled(false);
	ui.lblDirection2->setPalette(QPalette(QColor(255, 255, 255)));

	// reactivate buttons
	//ui.btnExecuteScript->setEnabled(true);
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
	ui.dockCamera->show();
	ui.dockOdometrie->show();
	ui.dockEnergy->show();
	ui.dockLog->show();
	settingsDialog->show();
	joystickDialog->show();
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


void Gui::on_actionPower_activated()
{
	if (ui.dockEnergy->isVisible())
	{
		ui.dockEnergy->hide();
	}
	else
	{
		ui.dockEnergy->show();
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


	switch (sensor)
	{
		case VOLTAGESENSOR1:
			ui.lblVoltage1->setText(QString("%1").setNum(voltage, 'f', 2).append(" Volt"));
			ui.progressBarVoltage1->setValue(voltage);
				
			// change color of the label depending on the voltage
			if (voltage < MINIMUMVOLTAGE1)
			{
				ui.groupBoxBattery1->setPalette(QPalette(labelFillColorRed));
				ui.lblVoltage1->setPalette(QPalette(labelFillColorRed));
			}
			else
			{
				ui.groupBoxBattery1->setPalette(QPalette(labelFillColorGreen));
				ui.lblVoltage1->setPalette(QPalette(labelFillColorGreen));
			}
			
			return;
			break;
		case VOLTAGESENSOR2:
			ui.lblVoltage2->setText(QString("%1").setNum(voltage, 'f', 2).append(" Volt"));
			ui.progressBarVoltage2->setValue(voltage);
				
			// change color of the label depending on the voltage
			if (voltage < MINIMUMVOLTAGE2)
			{
				ui.groupBoxBattery2->setPalette(QPalette(labelFillColorRed));
				ui.lblVoltage2->setPalette(QPalette(labelFillColorRed));
			}
			else
			{
				ui.groupBoxBattery2->setPalette(QPalette(labelFillColorGreen));
				ui.lblVoltage2->setPalette(QPalette(labelFillColorGreen));
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
					ui.lblPower1->setEnabled(true);
					ui.lblPower1->setPalette(QPalette(labelFillColorGreen));
					ui.lblDirection1->setEnabled(true);
					ui.lblDirection1->setPalette(QPalette(labelFillColorGreen));
					break;
				case OFF:
					ui.lblPower1->setEnabled(false);
					ui.lblPower1->setPalette(QPalette(QColor(255, 255, 255)));
					ui.lblDirection1->setEnabled(false);
					ui.lblDirection1->setPalette(QPalette(QColor(255, 255, 255)));
					break;
			}

			switch (direction)
			{
				case CLOCKWISE:
					ui.lblDirection1->setText("BACKWARD");
					ui.lblDirection1->setEnabled(true);
					ui.lblDirection1->setPalette(QPalette(labelFillColorGreen));
					break;
				case COUNTERCLOCKWISE:
					ui.lblDirection1->setText("FORWARD");
					ui.lblDirection1->setEnabled(true);
					ui.lblDirection1->setPalette(QPalette(labelFillColorGreen));
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
					ui.lblPower2->setEnabled(true);
					ui.lblPower2->setPalette(QPalette(labelFillColorGreen));
					ui.lblDirection2->setEnabled(true);
					ui.lblDirection2->setPalette(QPalette(labelFillColorGreen));
					break;
				case OFF:
					ui.lblPower2->setEnabled(false);
					ui.lblPower2->setPalette(QPalette(QColor(255, 255, 255)));
					ui.lblDirection2->setEnabled(false);
					ui.lblDirection2->setPalette(QPalette(QColor(255, 255, 255)));
					break;
			}

			switch (direction)
			{
				case CLOCKWISE:
					ui.lblDirection2->setText("BACKWARD");
					ui.lblDirection2->setEnabled(true);
					ui.lblDirection2->setPalette(QPalette(labelFillColorGreen));
					break;
				case COUNTERCLOCKWISE:
					ui.lblDirection2->setText("FORWARD");
					ui.lblDirection2->setEnabled(true);
					ui.lblDirection2->setPalette(QPalette(labelFillColorGreen));
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
					ui.lblPower3->setEnabled(true);
					ui.lblPower3->setPalette(QPalette(labelFillColorGreen));
					ui.lblDirection3->setEnabled(true);
					ui.lblDirection3->setPalette(QPalette(labelFillColorGreen));
					break;
				case OFF:
					ui.lblPower3->setEnabled(false);
					ui.lblPower3->setPalette(QPalette(QColor(255, 255, 255)));
					ui.lblDirection3->setEnabled(false);
					ui.lblDirection3->setPalette(QPalette(QColor(255, 255, 255)));
					break;
			}

			switch (direction)
			{
				case CLOCKWISE:
					ui.lblDirection3->setText("BACKWARD");
					ui.lblDirection3->setEnabled(true);
					ui.lblDirection3->setPalette(QPalette(labelFillColorGreen));
					break;
				case COUNTERCLOCKWISE:
					ui.lblDirection3->setText("FORWARD");
					ui.lblDirection3->setEnabled(true);
					ui.lblDirection3->setPalette(QPalette(labelFillColorGreen));
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
					ui.lblPower4->setEnabled(true);
					ui.lblPower4->setPalette(QPalette(labelFillColorGreen));
					ui.lblDirection4->setEnabled(true);
					ui.lblDirection4->setPalette(QPalette(labelFillColorGreen));
					break;
				case OFF:
					ui.lblPower4->setEnabled(false);
					ui.lblPower4->setPalette(QPalette(QColor(255, 255, 255)));
					ui.lblDirection4->setEnabled(false);
					ui.lblDirection4->setPalette(QPalette(QColor(255, 255, 255)));
					break;
			}

			switch (direction)
			{
				case CLOCKWISE:
					ui.lblDirection4->setText("BACKWARD");
					ui.lblDirection4->setEnabled(true);
					ui.lblDirection4->setPalette(QPalette(labelFillColorGreen));
					break;
				case COUNTERCLOCKWISE:
					ui.lblDirection4->setText("FORWARD");
					ui.lblDirection4->setEnabled(true);
					ui.lblDirection4->setPalette(QPalette(labelFillColorGreen));
					break;
				case SAME:
					break;
			}
			break;
	} // switch
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
	//ui.dockCamera->hide();
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


void Gui::on_radioBtnMotor1FW_clicked()
{
	emit drive(MOTOR1FW);
}


void Gui::on_radioBtnMotor1BW_clicked()
{
	emit drive(MOTOR1BW);
}


void Gui::on_radioBtnMotor1OFF_clicked()
{
	emit drive(MOTOR1OFF);
}


void Gui::on_radioBtnMotor2FW_clicked()
{
	emit drive(MOTOR2FW);
}


void Gui::on_radioBtnMotor2BW_clicked()
{
	emit drive(MOTOR2BW);
}


void Gui::on_radioBtnMotor2OFF_clicked()
{
	emit drive(MOTOR2OFF);
}


void Gui::on_radioBtnMotor3FW_clicked()
{
	emit drive(MOTOR3FW);
}


void Gui::on_radioBtnMotor3BW_clicked()
{
	emit drive(MOTOR3BW);
}


void Gui::on_radioBtnMotor3OFF_clicked()
{
	emit drive(MOTOR3OFF);
}


void Gui::on_radioBtnMotor4FW_clicked()
{
	emit drive(MOTOR4FW);
}


void Gui::on_radioBtnMotor4BW_clicked()
{
	emit drive(MOTOR4BW);
}


void Gui::on_radioBtnMotor4OFF_clicked()
{
	emit drive(MOTOR4OFF);
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
		// nice red button background
		// TODO: how to change a color of a button?  ui.actionSimulate->setPalette(QPalette(labelFillColorRed));
		emit simulate(true);
	}
	else
	{
		// set back to default color!!
		// TODO: how to change a color of a button?  ui.actionSimulate->setPalette(QApplication::palette());
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
	for (int i=0, angle=90; i<laserLineListRear->size(); i++, angle--)
	{
		// reset transform or rotation
		laserLineListRear->at(i)->resetTransform();

		// rotate every line by one degree
		laserLineListRear->at(i)->rotate(angle);

		// set position of each line
		laserLineListRear->at(i)->setPos((x - laserLineListRear->at(i)->line().length()), y);
	}


	y = laserYPos;// INITIALLASERYPOSREAR has no effect here, only in on_sliderZoom_valueChanged !!

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
		// Org: laserLineListFront->at(i)->setToolTip( QString("%1 m  / %2 deg / Flag=%3 / %4 Pixel / x=%5, y=%6").arg(laserScannerValues[i]).arg(i).arg(laserScannerFlags[i]).arg(laserLineLength) );
		
		// convert from polar to Cartesian coordinates
		// minus the current "middle position" of the robot within the gui frame
		// FIXME: fix this conversion to the correct values!
		int xKart = qRound( laserLineLength * cos(i) ) - laserXPos;
		int yKart = qRound( laserLineLength * sin(i) ) - laserYPos;
		laserLineListFront->at(i)->setToolTip( QString("%1 m  / %2 deg / Flag=%3 / %4 Pixel / x=%5, y=%6").arg(laserScannerValues[i]).arg(i).arg(laserScannerFlags[i]).arg(laserLineLength).arg(xKart).arg(yKart) );
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
	// TODO: check if always 180 lines!
	// create 180 laser lines (0 to 179)
	for (int i=-90; i<90; i++)
	{
		QGraphicsLineItem *line = new QGraphicsLineItem();

		// the length (and position) of the laser line in pixel
		line->setLine(0,0,0,0);

		// set the laser line color
		//line->setPen(QPen(colorLaserFreeWay, 3));
		line->setPen(QPen(colorLaserFreeWay));

		// set position of each line
		line->rotate(i);

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
	// TODO: check if always 180 lines!
	// create 180 laser lines (0 to 179)
	for (int i=90; i>-90; i--)
	{
		QGraphicsLineItem *line = new QGraphicsLineItem();

		// set the laser line color
		// line->setPen(QPen(colorLaserFreeWay, 3));
		line->setPen(QPen(colorLaserFreeWay));

		// the length (and position) of the laser line in pixel
		line->setLine(0,0,0,0);

		// set position of each line
		line->rotate(i);

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
// 	ui.qwtPlotCurrent1->setAxisScale(QwtPlot::yLeft,   0, AMPERESMAXPLOTCURVE1, AMPERESSTEPPLOTCURVE1); // use free dynamic axis scale now!

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
//	ui.qwtPlotCurrent2->setAxisScale(QwtPlot::yLeft,   0, AMPERESMAXPLOTCURVE2, AMPERESSTEPPLOTCURVE2); // use free dynamic axis scale now!

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


	// no 4
	/*
	 A compass showing another needle
	 */
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


void Gui::initCompassView()
{
	// the 3D OpenGL compass
//	compassWidget = new CompassWidget;
	// add widget to the GUI
//	ui.frameCompass->initializeGL();
	
/*
	// the graphicsScene for the compass
	compassScene = new QGraphicsScene();

	// set some colors
	compassScene->setBackgroundBrush(Qt::black);

	// turn off moving of scene, when objects extend the scene									-> DISABLED to enable dragging the robot in the laserView!!
	// (set scene rect to size of GUI element)													-> DISABLED to enable dragging the robot in the laserView!!
	//scene->setSceneRect(0, 0, ui.graphicsViewLaser->width(), ui.graphicsViewLaser->height());	-> DISABLED to enable dragging the robot in the laserView!!

	// set scene to the GUI
	ui.graphicsViewCompass->setScene(compassScene);

	// enable OpenGL rendering with antialiasing (and direct hardware rendering (if supportet by the hardware))
	ui.graphicsViewCompass->setViewport(new QGLWidget(QGLFormat(QGL::DoubleBuffer | QGL::DirectRendering)));
	
	QGraphicsLineItem *xAxis = new QGraphicsLineItem();
	QGraphicsLineItem *yAxis = new QGraphicsLineItem();
	QGraphicsLineItem *zAxis = new QGraphicsLineItem();

	// set the laser line color
	xAxis->setPen(QPen(Qt::red));
	yAxis->setPen(QPen(Qt::green));
	zAxis->setPen(QPen(Qt::blue));
	
	xAxis->setLine( 0,  0,   0, -40); // TODO: correct values
	yAxis->setLine( 0,  0,  40,  0);
	zAxis->setLine( 0,  0,  20, -20);
	
	// add items to scene
	compassScene->addItem(xAxis);
	compassScene->addItem(yAxis);
	compassScene->addItem(zAxis);
*/
}


void Gui::showCompassData(float x, float y, float z)
{
	// also formats the string to 3 digits precision!
	ui.lblCompassX->setText( QString("%1").setNum(x, 'f', 3).append(" deg") );
	ui.lblCompassY->setText( QString("%1").setNum(y, 'f', 3).append(" deg") );
	ui.lblCompassZ->setText( QString("%1").setNum(z, 'f', 3).append(" deg") );
	
	ui.qwtCompass->setValue(x);
	
	// set the 3D OpenGL compass!
	ui.frameCompass->setAllRotations(x, y, z);
}


void Gui::setLEDHeartbeat(bool state)
{
	if (state)
	{
		ui.lblLEDHeartbeat->setPixmap(QPixmap(":/images/images/led_green.gif"));
	}
	else
	{
		ui.lblLEDHeartbeat->setPixmap(QPixmap(":/images/images/led_gray.gif"));
		return;
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
