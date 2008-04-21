#include "gui.h"


Gui::Gui(QMainWindow *parent) : QMainWindow(parent)
{
	robotIsOn = false;
	
	//-------------------------------------------------------
	// startup the GUI
	//-------------------------------------------------------
	ui.setupUi(this);

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
	*/
	ui.progressBarSensor16->setMaximum(SENSORPROGRESSBARMAXUS);
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
	
	// change the value of a spinBox when the value of the corresponding slider changes
	connect(ui.sliderMotor1Speed, SIGNAL(valueChanged(int)), ui.spinBoxMotor1Speed, SLOT(setValue(int)));
	connect(ui.sliderMotor2Speed, SIGNAL(valueChanged(int)), ui.spinBoxMotor2Speed, SLOT(setValue(int)));
	// and vice versa
	connect(ui.spinBoxMotor1Speed, SIGNAL(valueChanged(int)), ui.sliderMotor1Speed, SLOT(setValue(int)));
	connect(ui.spinBoxMotor2Speed, SIGNAL(valueChanged(int)), ui.sliderMotor2Speed, SLOT(setValue(int)));
	
	// change the value of a spinBox when the value of the corresponding slider changes
	connect(ui.sliderMaximumSpeed, SIGNAL(valueChanged(int)), ui.spinBoxMaximumSpeed, SLOT(setValue(int)));
	connect(ui.sliderMinimumSpeed, SIGNAL(valueChanged(int)), ui.spinBoxMinimumSpeed, SLOT(setValue(int)));
	// and vice versa
	connect(ui.spinBoxMaximumSpeed, SIGNAL(valueChanged(int)), ui.sliderMaximumSpeed, SLOT(setValue(int)));
	connect(ui.spinBoxMinimumSpeed, SIGNAL(valueChanged(int)), ui.sliderMinimumSpeed, SLOT(setValue(int)));
	
	// emit the signal from Gui to obstacleCheckThread via Mrs!
	connect(ui.sliderRobotSlot, SIGNAL(valueChanged(int)), SIGNAL(setRobotSlot(int)));
	// change the value of a spinBox when the value of the corresponding slider changes
	connect(ui.sliderRobotSlot, SIGNAL(valueChanged(int)), ui.spinBoxRobotSlot, SLOT(setValue(int)));
	// and vice versa
	connect(ui.spinBoxRobotSlot, SIGNAL(valueChanged(int)), ui.sliderRobotSlot, SLOT(setValue(int)));
	
	// emit the signal from Gui to obstacleCheckThread via Mrs!
	connect(ui.sliderStraightForwardDeviation, SIGNAL(valueChanged(int)), SIGNAL(setStraightForwardDeviation(int)));
	// change the value of a spinBox when the value of the corresponding slider changes
	connect(ui.sliderStraightForwardDeviation, SIGNAL(valueChanged(int)), ui.spinBoxStraightForwardDeviation, SLOT(setValue(int)));
	// and vice versa
	connect(ui.spinBoxStraightForwardDeviation, SIGNAL(valueChanged(int)), ui.sliderStraightForwardDeviation, SLOT(setValue(int)));
	
	// emit the signal from Gui to obstacleCheckThread via Mrs!
	connect(ui.sliderObstacleLaserScanner, SIGNAL(valueChanged(int)), SIGNAL(setMinObstacleDistanceLaser(int)));
	// change the value of a spinBox when the value of the corresponding slider changes
	connect(ui.sliderObstacleLaserScanner, SIGNAL(valueChanged(int)), ui.spinBoxObstacleLaserScanner, SLOT(setValue(int)));
	// and vice versa
	connect(ui.spinBoxObstacleLaserScanner, SIGNAL(valueChanged(int)), ui.sliderObstacleLaserScanner, SLOT(setValue(int)));
	
	// emit the signal from Gui to obstacleCheckThread via Mrs!
	connect(ui.sliderObstacle, SIGNAL(valueChanged(int)), SIGNAL(setMinObstacleDistance(int)));
	// change the value of a spinBox when the value of the corresponding slider changes
	connect(ui.sliderObstacle, SIGNAL(valueChanged(int)), ui.spinBoxObstacle, SLOT(setValue(int)));
	// and vice versa
	connect(ui.spinBoxObstacle, SIGNAL(valueChanged(int)), ui.sliderObstacle, SLOT(setValue(int)));

	
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
	initializePlots();
	
	//----------------------------------------------------------------------------
	// Laser Scanner graphics Stuff (scene, view, lines, OpenGL etc.)
	//----------------------------------------------------------------------------
	createLaserScannerObjects();
	
	createLaserDistanceObjects();
	
	//--------------
	// window init
	//--------------
	// TODO: which values for which resolutions!?
	ui.splitLaserView->setSizes(QList<int>() << 900 << 100);
	// make dockLaserView wider
	//ui.dockLaserView->resize(ui.graphicsViewLaser->width(), ui.dockLaserView->height());
	//ui.centralWidget->resize(ui.graphicsViewLaser->width(), ui.dockLaserView->height());
	//ui.centralWidget->resize(400, ui.dockLaserView->height());
	ui.dockSettings->hide();
	ui.dockNetworkLog->hide();
}


Gui::~Gui()
{
	delete scannerRearSplash;
	delete scannerFrontSplash;
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
	while (!laserLineListRear->isEmpty())
	{
		delete laserLineListRear->takeFirst();
	}
	delete laserLineListRear;
	
	delete scene;
	//delete cameraScene;
}


void Gui::closeEvent(QCloseEvent *event)
{
	// no compiler warning "unused"
	Q_UNUSED(event);
	
	
	//qDebug("closeEvent");
	emit shutdown();
}


void Gui::on_btnExit_clicked()
{
	close();
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
	ui.btnDrive->setEnabled(state);
	ui.btnReset->setEnabled(state);
	ui.btnResetMovement1->setEnabled(state);
	ui.btnResetMovement2->setEnabled(state);
}


/*
void Gui::appLog(QString text)
{
	// just a quick and dirty wrapper static method for old CARMEN standard C code!
	Gui::appendLog(text);
}
*/


void Gui::appendLog(QString text, bool CR, bool sayIt)
{
	// insert the text
	ui.textEditLog->insertHtml(text);

	if (CR == TRUE)
	{
		// insert a line break
		ui.textEditLog->insertHtml("<br>");
	}
	
	// Ensures that the cursor is visible by scrolling the text edit if necessary.
	ui.textEditLog->ensureCursorVisible();


	// say the message via festival
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


	// say the message via festival
	if (sayIt == true)
	{
		emit speak(text);
	}
}


void Gui::on_btnDrive_clicked()
{
	static bool toggle = false;
	
	
	if (toggle == false)
	{
		appendLog("Start driving...");
		toggle = true;

		// change text of "drive button"
		ui.btnDrive->setText(" Stop &driving");

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
		ui.btnDrive->setText(" &Drive");

		//****************
		// stop driving
		//****************
		emit drive(STOP);
	}
}


void Gui::on_sliderMotor1Speed_sliderReleased()
{
	// no auto connect in constructor, because this slot has no "value" parameter!
	emit setMotorSpeed(1, ui.sliderMotor1Speed->value());
	
	int value = ui.sliderMotor1Speed->value();
	
	//laserDistanceLineListFront->at(0)->setStartAngle(ui.sliderMotor1Speed->value() * 16);
	laserDistanceLineListFront->at(0)->setPos(value, value);
	appendLog(QString("x,y=%1").arg(value));
}


void Gui::on_sliderMotor2Speed_sliderReleased()
{
	// no auto connect in constructor, because this slot has no "value" parameter!
	emit setMotorSpeed(2, ui.sliderMotor2Speed->value());
	
	//laserDistanceLineListFront->at(0)->setSpanAngle(ui.sliderMotor2Speed->value() * 16);
	//appendLog(QString("spanAngle=%1").arg(ui.sliderMotor2Speed->value()));
}


void Gui::on_btnReset_clicked()
{
	// TODO: check, if robot is OFF !!!!
	
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
	*/
	ui.progressBarSensor16->setValue(0);
	//ui.btnExecuteScript->setText("Execute s&cript");

	// set labels back in default position
	ui.lblPower1->setText("OFF");
	ui.lblPower1->setEnabled(false);
	ui.lblPower1->setPalette(QPalette(QColor(255, 255, 255)));
	ui.lblPower2->setText("OFF");
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

	// reactivate sliders & Co.
	ui.sliderMotor1Speed->setEnabled(true);
	ui.spinBoxMotor1Speed->setEnabled(true);
	
	ui.sliderMotor2Speed->setEnabled(true);
	ui.spinBoxMotor2Speed->setEnabled(true);
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


void Gui::on_btnTest_clicked()
{
	emit test();
}


void Gui::on_actionAll_activated()
{
	ui.dockLaserView->show();
	ui.dockCamera->show();
	ui.dockMotors->show();
	ui.dockSettings->show();
	ui.dockLog->show();
	ui.dockNetworkLog->show();
	ui.dockJoystick->show();
}


void Gui::on_actionLaser_View_activated()
{
	if (ui.dockLaserView->isVisible())
	{
		ui.dockLaserView->hide();
	}
	else
	{
		ui.dockLaserView->show();
	}
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


void Gui::on_actionMotors_activated()
{
	if (ui.dockMotors->isVisible())
	{
		ui.dockMotors->hide();
	}
	else
	{
		ui.dockMotors->show();
	}
}


void Gui::on_actionSettings_activated()
{
	if (ui.dockSettings->isVisible())
	{
		ui.dockSettings->hide();
	}
	else
	{
		ui.dockSettings->show();
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


void Gui::on_actionNetwork_Log_activated()
{
	if (ui.dockNetworkLog->isVisible())
	{
		ui.dockNetworkLog->hide();
	}
	else
	{
		ui.dockNetworkLog->show();
	}
}


void Gui::on_actionJoystick_activated()
{
	if (ui.dockJoystick->isVisible())
	{
		ui.dockJoystick->hide();
	}
	else
	{
		ui.dockJoystick->show();
	}
}


/*
void Gui::showDistance(int sensor, int distance)
{
	if ((sensor < SENSOR1) || (sensor > SENSOR16))
	{
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
	if ((sensor < SENSOR1) || (sensor > SENSOR16))
	{
	  return;
	}


	switch (sensor)
	{
		/*
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
		*/
		case SENSOR16:
			ui.progressBarSensor16->setValue(distance);
			return;
			break;
	}
}


void Gui::showDrivenDistance(int sensor, int distance)
{
	// show distance value in centimeters in a text label
	// erwartet den value als distance (z.B. in cm)!
	if ((sensor < MOTORSENSOR1) || (sensor > MOTORSENSOR2))
	{
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


void Gui::showAlarm(short int sensor, bool state)
{
	if ((sensor < SENSOR1) || (sensor > SENSOR16))
	{
		qDebug("error in showAlarm! sensorValue=%d", sensor);
		return;
	}


	switch (sensor)
	{
		/*
		case SENSOR1:
			if (state == ON)
			{
				// change sensor-pixmap color to red     [blue = 0, 70, 213]
				//ui.labelSensorPic1->setPalette(QPalette(QColor(255, 64, 64)));
				//ui.labelSensorAlarm1->setEnabled(true);
				//ui.frameSensorAlarm1->setPalette(QPalette(QColor(255, 64, 64)));
				// red progressBar background
				ui.progressBarSensor1->setPalette(QPalette(QColor(255, 64, 64)));
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
				//ui.labelSensorPic2->setPalette(QPalette(QColor(255, 64, 64)));
				//ui.labelSensorAlarm2->setEnabled(true);
				//ui.frameSensorAlarm2->setPalette(QPalette(QColor(255, 64, 64)));
				// red progressBar background
				ui.progressBarSensor2->setPalette(QPalette(QColor(255, 64, 64)));
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
				//ui.labelSensorPic3->setPalette(QPalette(QColor(255, 64, 64)));
				//ui.labelSensorAlarm3->setEnabled(true);
				//ui.frameSensorAlarm3->setPalette(QPalette(QColor(255, 64, 64)));
				// red progressBar background
				ui.progressBarSensor3->setPalette(QPalette(QColor(255, 64, 64)));
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
				//ui.labelSensorPic4->setPalette(QPalette(QColor(255, 64, 64)));
				//ui.labelSensorAlarm4->setEnabled(true);
				//ui.frameSensorAlarm4->setPalette(QPalette(QColor(255, 64, 64)));
				// red progressBar background
				ui.progressBarSensor4->setPalette(QPalette(QColor(255, 64, 64)));
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
				//ui.labelSensorPic5->setPalette(QPalette(QColor(255, 64, 64)));
				//ui.labelSensorAlarm5->setEnabled(true);
				//ui.frameSensorAlarm5->setPalette(QPalette(QColor(255, 64, 64)));
				// red progressBar background
				ui.progressBarSensor5->setPalette(QPalette(QColor(255, 64, 64)));
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
				//ui.labelSensorPic6->setPalette(QPalette(QColor(255, 64, 64)));
				//ui.labelSensorAlarm6->setEnabled(true);
				//ui.frameSensorAlarm6->setPalette(QPalette(QColor(255, 64, 64)));
				// red progressBar background
				ui.progressBarSensor6->setPalette(QPalette(QColor(255, 64, 64)));
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
				//ui.labelSensorPic7->setPalette(QPalette(QColor(255, 64, 64)));
				//ui.labelSensorAlarm7->setEnabled(true);
				//ui.frameSensorAlarm7->setPalette(QPalette(QColor(255, 64, 64)));
				// red progressBar background
				ui.progressBarSensor7->setPalette(QPalette(QColor(255, 64, 64)));
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
				//ui.labelSensorPic8->setPalette(QPalette(QColor(255, 64, 64)));
				//ui.labelSensorAlarm8->setEnabled(true);
				//ui.frameSensorAlarm8->setPalette(QPalette(QColor(255, 64, 64)));
				// red progressBot background
				ui.progressBarSensor8->setPalette(QPalette(QColor(255, 64, 64)));
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
		*/
		case SENSOR16:
			if (state == ON)
			{
				// change sensor-pixmap color to red     [blue = 0, 70, 213]
				//ui.labelSensorPic16->setPalette(QPalette(QColor(255, 64, 64)));
				//ui.labelSensorAlarm16->setEnabled(true);
				//ui.frameSensorAlarm16->setPalette(QPalette(QColor(255, 64, 64)));
				ui.progressBarSensor16->setPalette(QPalette(QColor(255, 64, 64)));
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
}


void Gui::showMotorStatus(unsigned char motor, bool power, unsigned char direction)
{
	// show the motor status with the radio buttons
	switch (motor)
	{
		//-------------
		case MOTOR1:
		//-------------
			switch (power)
			{
				case ON:
					//ui.lblPower1->setText("ON");
					ui.lblPower1->setEnabled(true);
					ui.lblPower1->setPalette(QPalette(QColor(64, 255, 64)));
					
					ui.lblDirection1->setEnabled(true);
					ui.lblDirection1->setPalette(QPalette(QColor(64, 255, 64)));
					break;
				case OFF:
					//ui.lblPower1->setText("OFF");
					ui.lblPower1->setEnabled(false);
					ui.lblPower1->setPalette(QPalette(QColor(255, 255, 255)));
					
					ui.lblDirection1->setEnabled(false);
					ui.lblDirection1->setPalette(QPalette(QColor(255, 255, 255)));
					break;
				case SAME:
					break;
			}

			switch (direction)
			{
				case CLOCKWISE:
					ui.lblDirection1->setText("FORWARD");
					ui.lblDirection1->setEnabled(true);
					ui.lblDirection1->setPalette(QPalette(QColor(64, 255, 64)));
					return;
					break;
				case COUNTERCLOCKWISE:
					ui.lblDirection1->setText("BACKWARD");
					ui.lblDirection1->setEnabled(true);
					ui.lblDirection1->setPalette(QPalette(QColor(64, 255, 64)));
					return;
					break;
				case SAME:
					return;
					break;
			}
			return;
			break;
			
		//-------------
		case MOTOR2:
		//-------------
			switch (power)
			{
				case ON:
					//ui.lblPower2->setText("ON");
					ui.lblPower2->setEnabled(true);
					ui.lblPower2->setPalette(QPalette(QColor(64, 255, 64)));
					
					ui.lblDirection1->setEnabled(true);
					ui.lblDirection1->setPalette(QPalette(QColor(64, 255, 64)));
					break;
				case OFF:
					//ui.lblPower2->setText("OFF");
					ui.lblPower2->setEnabled(false);
					ui.lblPower2->setPalette(QPalette(QColor(255, 255, 255)));
					
					ui.lblDirection2->setEnabled(false);
					ui.lblDirection2->setPalette(QPalette(QColor(255, 255, 255)));
					break;
				case SAME:
					break;
			}

			switch (direction)
			{
				case CLOCKWISE:
					ui.lblDirection2->setText("FORWARD");
					ui.lblDirection2->setEnabled(true);
					ui.lblDirection2->setPalette(QPalette(QColor(64, 255, 64)));
					return;
					break;
				case COUNTERCLOCKWISE:
					ui.lblDirection2->setText("BACKWARD");
					ui.lblDirection2->setEnabled(true);
					ui.lblDirection2->setPalette(QPalette(QColor(64, 255, 64)));
					return;
					break;
				case SAME:
					return;
					break;
			}
			return;
			break;
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
		ui.lblPreferredDirection->setPixmap(QPixmap(":/images/images/forward.png"));
		return;
	}
	
	if (direction == "LEFT")
	{
		// other direction image, because different view in the gui!
		ui.lblPreferredDirection->setPixmap(QPixmap(":/images/images/right.png"));
		return;
	}
	
	if (direction == "RIGHT")
	{
		// other direction image, because different view in the gui!
		ui.lblPreferredDirection->setPixmap(QPixmap(":/images/images/left.png"));
		return;
	}
}

void Gui::showFaceTrackDirection(QString direction)
{
	if (direction == "UP")
	{
		ui.lblFaceTrackTo->setPixmap(QPixmap(":/images/images/backward.png"));
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
		ui.lblFaceTrackTo->setPixmap(QPixmap(":/images/images/forward.png"));
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


int Gui::getSliderMotorSpeed(int motor)
{
	switch (motor)
	{
		case 1:
			return ui.sliderMotor1Speed->value();
			break;
		case 2:
			return ui.sliderMotor2Speed->value();
			break;
	}
	
	
	return -1;
}


int Gui::getSliderMinimumSpeed(void)
{
	return ui.sliderMinimumSpeed->value();
}


int Gui::getSliderMaximumSpeed(void)
{
	return ui.sliderMaximumSpeed->value();
}


int Gui::getSliderObstacleValue()
{
	return ui.sliderObstacle->value();
}


int Gui::getSliderObstacleLaserScannerValue()
{
	return ui.sliderObstacleLaserScanner->value();
}


int Gui::getSliderRobotSlotValue()
{
	return ui.sliderRobotSlot->value();
}


int Gui::getSliderStraightForwardDeviationValue()
{
	return ui.sliderStraightForwardDeviation->value();
}


Qt::CheckState Gui::getCheckBoxSaveSettings()
{
	return ui.checkBoxSaveSettings->checkState();
}


void Gui::setSliderMotorSpeed(int motor, int value)
{
	switch (motor)
	{
		case 1:
			ui.sliderMotor1Speed->setValue(value);
			ui.spinBoxMotor1Speed->setValue(value);
			break;
		case 2:
			ui.sliderMotor2Speed->setValue(value);
			ui.spinBoxMotor2Speed->setValue(value);
			break;
	}
}


void Gui::setSliderMinimumSpeed(int speed)
{
	ui.sliderMinimumSpeed->setValue(speed);
}


void Gui::setSliderMaximumSpeed(int speed)
{
	ui.sliderMaximumSpeed->setValue(speed);
}


void Gui::setSliderObstacleValue(int value)
{
	ui.sliderObstacle->setValue(value);
}


void Gui::setSliderObstacleLaserScannerValue(int value)
{
	ui.sliderObstacleLaserScanner->setValue(value);
}


void Gui::setSliderRobotSlot(int angle)
{
	ui.sliderRobotSlot->setValue(angle);
}


void Gui::setSliderStraightForwardDeviation(int angle)
{
	ui.sliderStraightForwardDeviation->setValue(angle);
}


void Gui::setCheckBoxSaveSettings(Qt::CheckState state)
{
	ui.checkBoxSaveSettings->setCheckState(state);
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


void Gui::setCamImage(IplImage* frame)
{
	// set image from OpenGL context to Qt frame!
	//ui.frameCamera->setImage((unsigned char*)frame->imageData, frame->width, frame->height, (frame->nChannels * frame->depth) );
	ui.frameCamera->setImage((unsigned char*)frame->imageData);
	
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
	
	
	// TODO: Seltsame Bildmaße! :-?
	cameraPicToSave = QPixmap::grabWindow(QWidget::winId(), ui.dockCamera->x()+19, ui.dockCamera->y()+19, 474, 353);
	//cameraPicToSave = QPixmap::grabWindow(QWidget::winId(), ui.frame->x(), ui.frame->y(), 200, 100);
	// cameraPicToSave = QPixmap::grabWidget(ui.frameCamera); doesnt work
	
	//---------------------------------------------------------------------
	// save image to disk, but not within the same seond (same timestamp)
	//---------------------------------------------------------------------
	// FIXME: every second is too intensive!!!
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
	curve2.attach(ui.qwtPlotCurrent2);
	
	// after changing the values, replot the curve
	ui.qwtPlotCurrent2->replot();
}


void Gui::on_btnEnableRemote_clicked()
{
	// if remote control is enabled, disable some controls in the GUI
	if (ui.btnEnableRemote->isChecked())
	{
		ui.btnDrive->setEnabled(false);
		emit enableRemoteControlListening(true);
	}
	else
	{
		if (robotIsOn)
		{
			ui.btnDrive->setEnabled(true);
		}
		emit enableRemoteControlListening(false);
	}
}


void Gui::on_btnSimulate_clicked()
{
	if (ui.btnSimulate->isChecked())
	{
		// nice red button background
		ui.btnSimulate->setPalette(QPalette(QColor(255, 64, 64)));
		emit simulate(true);
	}
	else
	{
		// set back to default color!!
		ui.btnSimulate->setPalette(QApplication::palette());
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
	laserXPos = (ui.graphicsViewLaser->width() / 2) - ( pixmapBot1->pixmap().width() / 2 / startScale * lastZoom) ;
	
	// horizontal center
	pixmapBot1->setPos(laserXPos, laserFrontYPos);
	pixmapBot2->setPos(laserXPos, laserFrontYPos);
	//appendLog(QString("<b>sliderZoomValueChanged...bot y pos()=%1</b>").arg(pixmapBot1->y()));
	
	
	//-----------------------------------------------------
	// change the x and y position of the laser lines
	//-----------------------------------------------------
	x = calculateLaserXpos();
	y = calculateLaserFrontYpos();
	
	for (int i=0; i<laserLineListFront->size(); i++)
	{
		// horizontal center:
		// x = middle of the bot pixmap in the view
		// y = set manually
		laserLineListFront->at(i)->setPos(x, y);
	}
	
	
	//------------------------------------------------
	// change the y position of the laser lines
	//------------------------------------------------
	y = calculateLaserRearYpos();
	
	for (int i=0; i<laserLineListRear->size(); i++)
	{
		// horizontal center:
		// x = middle of the bot pixmap in the view
		// y = set manually
		laserLineListRear->at(i)->setPos(x, y);
	}
	
	
	//-------------------------------------------------------------
	// change the y position of the laser distance lines and text
	//-------------------------------------------------------------
	for (int i=0; i<laserDistanceLineListFront->size(); i++)
	{
		// (LASERDISTANCEFIRSTCIRCLE/STARTZOOMLEVEL) is the 'factor'
		// value is the zoomSlider value
		newSize = (LASERDISTANCEFIRSTCIRCLE / STARTZOOMLEVEL * value) + (i * LASERDISTANCEDISTANCE);
		
		// recalculate the new position (put the middle of the circle in the middle of the graphics view)
		x = calculateLaserXpos() - (newSize / 2);
		y = calculateLaserFrontYpos() - (newSize / 2);
		
		// change the width and height
		laserDistanceLineListFront->at(i)->setRect(0, 0, newSize, newSize);
		// set the circle position!
		laserDistanceLineListFront->at(i)->setPos(x, y);
		
		
		// recalculate the new text position
		x = calculateLaserXpos();
		y = calculateLaserFrontYpos() + (newSize/2);
		
		// set the text position!
		laserDistanceTextFront->at(i)->setPos(x, y);
		
		// set the text
		text = QString("%1").arg(newSize/(FITTOFRAMEFACTOR*value*2));
		// only show 2 digits after the ','
		laserDistanceTextFront->at(i)->setText(text.left(text.indexOf(".") + 3) + "m");
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
	//QLineF line;
	

	//==================================================
	// move the laser lines to their x and y positions
	//==================================================
	x = calculateLaserXpos();
	y = calculateLaserFrontYpos();
	
	//--------------
	// FRONT laser
	//--------------
	for (int i=0, angle=90; i<laserLineListFront->size(); i++, angle--)
	{
		// reset transform or rotation
		laserLineListFront->at(i)->resetTransform();
		
		// set position of each line
		laserLineListFront->at(i)->rotate(angle);
		
		// horizontal center:
		// x = middle of the bot pixmap in the view
		// y = set manually
		//line = laserLineListFront->at(i)->line();
		laserLineListFront->at(i)->setPos((x - laserLineListFront->at(i)->line().length()), y);
	}
	
	x = calculateLaserXpos();
	y = calculateLaserRearYpos();

	//--------------
	// REAR laser
	//--------------
	for (int i=0, angle=-90; i<laserLineListRear->size(); i++, angle--)
	{
		// reset transform or rotation
		laserLineListRear->at(i)->resetTransform();
		
		// set position of each line
		laserLineListRear->at(i)->rotate(angle);
		
		// horizontal center:
		// x = middle of the bot pixmap in the view
		// y = set manually
		//line = laserLineListRear->at(i)->line();
		laserLineListRear->at(i)->setPos((x - laserLineListRear->at(i)->line().length()), y);
	}
	
	
	//==================================================
	//
	//==================================================
	on_sliderZoom_valueChanged(lastZoom);
	
	
	//====================================================
	// move the distance lines to their x and y positions
	// has to be *after* sliderZoom_valueChanged for correct 'distance' positions!
	//====================================================
	// in the middle of the front of bot, minus a half of the innerst circle (no. 0)
	x = calculateLaserXpos() - (laserDistanceLineListFront->at(0)->rect().width() / 2);
	y = calculateLaserFrontYpos() - (laserDistanceLineListFront->at(0)->rect().height() / 2);
	
	
	//-----------------
	// FRONT distances
	//-----------------
	for (int i=0; i<laserDistanceLineListFront->size(); i++)
	{
		laserDistanceLineListFront->at(i)->setPos( (x - (i*LASERDISTANCEDISTANCE/2)), y - (i*LASERDISTANCEDISTANCE/2) );
	}
	
	
	// zoom into the laser lines by default factor
	ui.sliderZoom->setValue(STARTZOOMLEVEL);
}


void Gui::refreshLaserViewFront(float *laserScannerValues, int *laserScannerFlags)
{
	/*
	// FIXME: doesnt work!
	static bool showLaserSplashFront = true;
	
	
	if (showLaserSplashFront == true)
	{
		// first line is longer than 0 m ! (a values was measured)
		if (laserLineListFront->at(0)->line().length() > 0)
		{
			// laser splash screen OFF
			showLaserSplashFront = false;
	*/
			laserSplash(false, LASER1);
	/*
		}
	}
	*/
	
	
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
	/*
	// FIXME: doesnt work!
	static bool showLaserSplashRear = true;
	
	if (showLaserSplashRear == true)
	{
		// first line is longer than 0 m ! (a values was measured)
		if (laserLineListRear->at(0)->line().length() > 0)
		{
			// laser splash screen OFF
			showLaserSplashRear = false;
	*/
			laserSplash(false, LASER2);
	/*
		}
	}
	*/
	
	
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


void Gui::createLaserScannerObjects()
{
	// the start position for the pos. calculation
	laserXPos = 0;
	laserFrontYPos = 0;
	laserRearYPos = 0;
	
	// set some nice colors for some widgets
	colorLaserObstacle =  QColor(255, 50, 50); // light red
	colorLaserFreeWay = Qt::darkRed;
	colorLaserPreferredDrivingDirection = QColor(7, 68, 30); // green
	colorLaserCenterDrivingDirection = Qt::green;
	colorGraphicsSceneBackground = Qt::black;
	
	
	// the scene for the laser scanner lines
	scene = new QGraphicsScene();
	
	// set some colors
	scene->setBackgroundBrush(colorGraphicsSceneBackground);
	
	// turn off moving of scene, when objects extend the scene
	// (set scene rect to size of GUI element)
	scene->setSceneRect(0, 0, ui.graphicsViewLaser->width(), ui.graphicsViewLaser->height());
	
	// set scene to the GUI
	ui.graphicsViewLaser->setScene(scene);
	
	// enable OpenGL rendering with antialiasing (and direct hardware rendering (if supportet from the hardware))
	ui.graphicsViewLaser->setViewport(new QGLWidget(QGLFormat(QGL::DoubleBuffer | QGL::DirectRendering)));

	//=======================================================
	// add "searching laser scanner" pic "splash"
	//=======================================================
	// add items to the scene
	scannerFrontSplash = new QGraphicsPixmapItem(QPixmap(":/images/images/sickfront.png"));
	// add the pixmap
	scene->addItem(scannerFrontSplash);
	// center it in the down 1/3
	scannerFrontSplash->setPos(((ui.graphicsViewLaser->width() / 2) - 74), ((ui.graphicsViewLaser->height()*0.66) - 74));
	// FIXME:  see refreshLaserView (both). Turning off lasersplash doesn't work
	scannerFrontSplash->setVisible(false);
	
	// add items to the scene
	scannerRearSplash = new QGraphicsPixmapItem(QPixmap(":/images/images/sickrear.png"));
	// add the pixmap
	scene->addItem(scannerRearSplash);
	// center it in the upper 1/3
	scannerRearSplash->setPos(((ui.graphicsViewLaser->width() / 2) - 74), ((ui.graphicsViewLaser->height()*0.33) - 74));
	// FIXME:  see refreshLaserView (both). Turning off lasersplash doesn't work
	scannerRearSplash->setVisible(false);
	

	
	//=======================================================
	// add robot picture1
	//=======================================================
	// add items to the scene
	pixmapBot1 = new QGraphicsPixmapItem(QPixmap(":/images/images/bot_from_above.png"));
	
	// init the scale for the laser line / distances drawing stuff
	lastZoom = ui.sliderZoom->value();
	
	//--------------------------------------------------------------
	// calculate a nice y position in the view
	//--------------------------------------------------------------
	startScale = 10;
	
	laserXPos = calculateLaserXpos();
	laserFrontYPos = calculateLaserFrontYpos();
	laserRearYPos  = calculateLaserRearYpos();
	
	// change scale of the robot pic to 1/10 to fit in the window and to fit on the size of the laser lines
	pixmapBot1->scale( (1.0 / startScale), (1.0 / startScale));
	
	// horizontal center
	pixmapBot1->setPos(laserXPos, laserFrontYPos);
	
	// add the pixmap (invisible)
	pixmapBot1->setVisible(false);
	scene->addItem(pixmapBot1);
	
	// put one layer up (layer 2). All others share the same (unset) layer under the pixmap.
	pixmapBot1->setZValue(1);
	
	
	//=======================================================
	// create the laser line lists
	//=======================================================
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
		line->setVisible(false);
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
		line->setVisible(false);
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
	pixmapBot2->setPos(laserXPos, laserFrontYPos);
	
	// add the pixmap (invisible)
	pixmapBot2->setVisible(false);
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
	// create the laser line distances with text
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
}


qreal Gui::calculateLaserXpos()
{
	// value for normal drawing (from center to border of control)
	return ui.graphicsViewLaser->width() / 2;
}


qreal Gui::calculateLaserFrontYpos()
{
	// value for normal drawing (from center to border of control)
	return (pixmapBot1->pixmap().height()/startScale*0.66*lastZoom) + laserFrontYPos;
}


qreal Gui::calculateLaserRearYpos()
{
	// value for normal drawing (from center to border of control)
	return (pixmapBot1->pixmap().height()/startScale*0.33*lastZoom) + laserRearYPos;
}


void Gui::laserSplash(bool status, short int laserScanner)
{
	switch (laserScanner)
	{
		case LASER1:
			if (status == true)
			{
				//
				// laser splash ON
				//
				scannerFrontSplash->setVisible(true);
				pixmapBot1->setVisible(false);
				pixmapBot2->setVisible(false);
				for (int i=laserLineListFront->size()-1; i>=0; i--)
				{
					// lines invisible
					laserLineListFront->at(i)->setVisible(false);
				}
			}
			else
			{
				//
				// laser splash OFF
				//
				scannerFrontSplash->setVisible(false);
				pixmapBot1->setVisible(true);
				pixmapBot2->setVisible(true);
				for (int i=laserLineListFront->size()-1; i>=0; i--)
				{
					// lines visible
					laserLineListFront->at(i)->setVisible(true);
				}
			}
			break;
		case LASER2:
			if (status == true)
			{
				//
				// laser splash ON
				//
				scannerRearSplash->setVisible(true);
				pixmapBot1->setVisible(false);
				pixmapBot2->setVisible(false);
				for (int i=laserLineListRear->size()-1; i>=0; i--)
				{
					// lines invisible
					laserLineListRear->at(i)->setVisible(false);
				}
			}
			else
			{
				//
				// laser splash OFF
				//
				scannerRearSplash->setVisible(false);
				pixmapBot1->setVisible(true);
				pixmapBot2->setVisible(true);
				for (int i=laserLineListRear->size()-1; i>=0; i--)
				{
					// lines visible
					laserLineListRear->at(i)->setVisible(true);
				}
			}
			break;
	}
}


void Gui::initializePlots()
{
	//--------------------------------------
	// plot curve "MOTOR CURRENT" 1
	//--------------------------------------
	//ui.qwtPlotCurrent1->setTitle("Motor 1");

	// Set axis titles
	//ui.qwtPlotCurrent1->setAxisTitle(QwtPlot::xBottom, "Time/s");
	//ui.qwtPlotCurrent1->setAxisTitle(QwtPlot::yLeft, "Current/mA");

	// Set axis scale (instead of using autoscale, which is default)
	// time
	ui.qwtPlotCurrent1->setAxisScale(QwtPlot::xBottom, 0, 60.0, 10);
	// Ampere (1000 mA, Step 200)
	//ui.qwtPlotCurrent1->setAxisScale(QwtPlot::yLeft,   0, 4000.0, 400);
	
	QColor col = QColor(255, 64, 64);
	curve1.setRenderHint(QwtPlotItem::RenderAntialiased);
	curve1.setPen(QPen(col));
	curve1.setBrush(col);
	
	
	//--------------------------------------
	// plot curve "MOTOR CURRENT" 2
	//--------------------------------------
	//ui.qwtPlotCurrent2->setTitle("Motor 2");

	// Set axis titles
	//ui.qwtPlotCurrent2->setAxisTitle(QwtPlot::xBottom, "Time/s");
	//ui.qwtPlotCurrent2->setAxisTitle(QwtPlot::yLeft, "Current/mA");

	// Set axis scale (instead of using autoscale, which is default)
	// time
	ui.qwtPlotCurrent2->setAxisScale(QwtPlot::xBottom, 0, 60.0, 10);
	// Ampere (1000 mA, Step 200)
	//ui.qwtPlotCurrent2->setAxisScale(QwtPlot::yLeft,   0, 4000.0, 400);
	
	col = QColor(64, 64, 255);
	curve2.setRenderHint(QwtPlotItem::RenderAntialiased);
	curve2.setPen(QPen(col));
	curve2.setBrush(col);
}


void Gui::showJoystickAxes(int axisNumber, int axisValue)
{
	// TODO: put axis numbers to ini-file
	switch (axisNumber)
	{
		case JOYSTICKAXISX:
			// X axis
			ui.sliderJoystickX->setValue(axisValue);
			break;
		case JOYSTICKAXISY:
			// Y axis
			ui.sliderJoystickY->setValue(axisValue);
			break;
		case JOYSTICKAXIS2Y:
			// Y axis "buttons"
			// up
			if (axisValue < 0)
			{
				ui.radioBtnJoy5Up->setChecked(true);
			}
			
			// down
			if (axisValue > 0)
			{
				ui.radioBtnJoy5Down->setChecked(true);
			}

			if (axisValue == 0)
			{
				ui.radioBtnJoy5Up->setChecked(false);
				ui.radioBtnJoy5Down->setChecked(false);
			}
			break;
		case JOYSTICKAXIS2X:
			// X axis "buttons"
			// left
			if (axisValue < 0)
			{
				ui.radioBtnJoy4Left->setChecked(true);
			}
			
			// right
			if (axisValue > 0)
			{
				ui.radioBtnJoy4Right->setChecked(true);
			}

			if (axisValue == 0)
			{
				ui.radioBtnJoy4Left->setChecked(false);
				ui.radioBtnJoy4Right->setChecked(false);
			}
			break;
	}
}


void Gui::showJoystickButtons(int buttonNumber, bool buttonState)
{
	static bool toggle0 = false;
	/*
	static bool toggle1 = false;
	static bool toggle2 = false;
	static bool toggle3 = false;
	static bool toggle4 = false;
	static bool toggle5 = false;
	*/
	static bool toggle10 = false;
	static bool toggle11 = false;
	
	
	// TODO: put button numbers to ini-file
	switch (buttonNumber)
	{
		case 0:
			if (buttonState==true)
			{
				if (toggle0 == false)
				{
					toggle0=true;
				}
				else
				{
					toggle0=false;
				}
			}
			ui.radioBtnJoy0->setChecked(toggle0);
			break;
		case 1:
			ui.radioBtnJoy1->setChecked(buttonState);
			break;
		case 2:
			ui.radioBtnJoy2->setChecked(buttonState);
			break;
		case 3:
			ui.radioBtnJoy3->setChecked(buttonState);
			break;
		case 4:
			ui.radioBtnJoy4->setChecked(buttonState);
			break;
		case 5:
			ui.radioBtnJoy5->setChecked(buttonState);
			break;
		case 10:
			if (buttonState==true)
			{
				if (toggle10 == false)
				{
					toggle10=true;
				}
				else
				{
					toggle10=false;
				}
			}
			ui.radioBtnJoy10->setChecked(toggle10);
			break;
		case 11:
			if (buttonState==true)
			{
				if (toggle11 == false)
				{
					toggle11=true;
				}
				else
				{
					toggle11=false;
				}
			}
			ui.radioBtnJoy11->setChecked(toggle10);
			break;
	}
}
