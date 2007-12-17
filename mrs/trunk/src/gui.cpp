#include "gui.h"


Gui::Gui(Mrs *m, SensorThread *s, PlotThread *p, ObstacleCheckThread *o, Circuit *c, CamThread *ca, Motor *mot, Servo *serv, NetworkThread *net, LaserThread *l, QMainWindow *parent) : QMainWindow(parent)
{
	// copy the pointers from the original objects
	mrs1 = m;
	sensThread = s;
	plotThread = p;
	obstCheckThread = o;
	circuit1 = c;
	cam1 = ca;
	motors = mot;
	servos = serv;
	netThread = net;
	laserThread = l;
	
	
	//-------------------------------------------------------
	// startup the GUI
	//-------------------------------------------------------
	ui.setupUi(this);

	// TODO: make a const of this init values! see also sensorThread
	// value in cm
	// set maximum in cm AND raise the widget (make it topmost)!
	ui.progressBarSensor1->setMaximum(50);
	ui.progressBarSensor1->raise();
	ui.progressBarSensor2->setMaximum(50);
	ui.progressBarSensor2->raise();
	ui.progressBarSensor3->setMaximum(50);
	ui.progressBarSensor3->raise();
	ui.progressBarSensor4->setMaximum(50);
	ui.progressBarSensor4->raise();
	ui.progressBarSensor5->setMaximum(50);
	ui.progressBarSensor5->raise();
	ui.progressBarSensor6->setMaximum(50);
	ui.progressBarSensor6->raise();
	ui.progressBarSensor7->setMaximum(50);
	ui.progressBarSensor7->raise();
	ui.progressBarSensor8->setMaximum(50);
	ui.progressBarSensor8->raise();
	ui.progressBarSensor16->setMaximum(400); // max. 400 cm ultra sonic sensor !!
	//ui.progressBarSensor16->raise();
	
	
	// change the value of the spinBoxServo1 when the value of the sliderPositionServo1 changes
	connect(ui.sliderPositionServo1, SIGNAL(valueChanged(int)), ui.spinBoxPositionServo1, SLOT(setValue(int)));
	//connect(ui.sliderPositionServo2, SIGNAL(valueChanged(int)), ui.spinBoxPositionServo2, SLOT(setValue(int)));
	// and vice versa
	connect(ui.spinBoxPositionServo1, SIGNAL(valueChanged(int)), ui.sliderPositionServo1, SLOT(setValue(int)));
	//connect(ui.spinBoxPositionServo2, SIGNAL(valueChanged(int)), ui.sliderPositionServo2, SLOT(setValue(int)));
	
	// change the value of the spinBoxBotSlot when the value of the corresponding slider changes
	connect(ui.sliderBotSlot, SIGNAL(valueChanged(int)), ui.spinBoxBotSlot, SLOT(setValue(int)));
	// and vice versa
	connect(ui.spinBoxBotSlot, SIGNAL(valueChanged(int)), ui.sliderBotSlot, SLOT(setValue(int)));
	connect(ui.spinBoxBotSlot, SIGNAL(valueChanged(int)), obstCheckThread, SLOT(setRobotSlot(int)));
	
	// TODO: to init the value for the obstacle check thread emit something?!?
	//emit (); ?!?
	
	// change the value of a spinBox when the value of the corresponding slider changes
	connect(ui.sliderMotor1Speed, SIGNAL(valueChanged(int)), ui.spinBoxMotor1Speed, SLOT(setValue(int)));
	connect(ui.sliderMotor2Speed, SIGNAL(valueChanged(int)), ui.spinBoxMotor2Speed, SLOT(setValue(int)));
	// and vice versa
	connect(ui.spinBoxMotor1Speed, SIGNAL(valueChanged(int)), ui.sliderMotor1Speed, SLOT(setValue(int)));
	connect(ui.spinBoxMotor2Speed, SIGNAL(valueChanged(int)), ui.sliderMotor2Speed, SLOT(setValue(int)));
	
	//----------------------------------------------------------------------------
	// connect camDataComplete from the cam thread to signal "setCamImage"
	// (Whenever the image is complete, the image is shown in the GUI)
	//----------------------------------------------------------------------------
	connect(cam1, SIGNAL( camDataComplete(IplImage*) ), this, SLOT( setCamImage(IplImage*) ));
	
	
	//----------------------------------------------------------------------------
	// connect networkThread signal to "dataReceived"
	// (Whenever data were received, the data are shown in the GUI)
	//----------------------------------------------------------------------------
	connect(netThread, SIGNAL( dataReceived(QString) ), this, SLOT( appendNetworkLog(QString) ));
	

	//----------------------------------------------------------------------------
	// Plot stuff
	//----------------------------------------------------------------------------
	initializePlots();
	
	
	//----------------------------------------------------------------------------
	// Laser Scanner graphics Stuff (scene, view, lines, OpenGL etc.)
	//----------------------------------------------------------------------------
	createLaserScannerObjects();
	
	//createLaserDistanceObjects();
	
	// zoom into the laser lines by factor 3
	//on_sliderZoom_valueChanged(3);
}


Gui::~Gui()
{
	delete pixmapBot1;
	
	/*
	TODO: empty list
	// empty QList
	while (!laserDistanceLineList->isEmpty())
	{
		delete laserDistanceLineList->takeFirst();
	}
	delete laserDistanceLineList;
*/

	// empty QList
	while (!laserLineList->isEmpty())
	{
		delete laserLineList->takeFirst();
	}
	delete laserLineList;	
	delete scene;
}


void Gui::closeEvent()
{
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
		mrs1->speak(text);
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
		mrs1->speak(text);
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
		mrs1->drive(START);
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
		mrs1->drive(STOP);
	}
}


//void Gui::on_sliderMotorSpeed_valueChanged(int value)
void Gui::on_sliderMotor1Speed_sliderReleased()
{
	//int value = ui.sliderMotor1Speed->value();
	motors->setMotorSpeed(1, ui.sliderMotor1Speed->value());
	
	// durch signal-slot erledit!
	//ui.spinBoxMotor1Speed->setValue(value);
}


void Gui::on_spinBoxMotor1Speed_valueChanged(int value)
{
	motors->setMotorSpeed(1, value);
	
	// durch signal-slot erledit!
	//ui.sliderMotor1Speed->setValue(value);
}


//void Gui::on_sliderMotorSpeed_valueChanged(int value)
void Gui::on_sliderMotor2Speed_sliderReleased()
{
	//int value = ui.sliderMotor2Speed->value();
	
	motors->setMotorSpeed(2, ui.sliderMotor2Speed->value());
	
	// durch signal-slot erledit!
	//ui.spinBoxMotor2Speed->setValue(value);
}


void Gui::on_spinBoxMotor2Speed_valueChanged(int value)
{
	motors->setMotorSpeed(2, value);
	//ui.sliderMotor2Speed->setValue(value);
}


void Gui::on_sliderObstacle_valueChanged(int value)
{
	// TODO: change to signal slot auto connection
	obstCheckThread->setMinObstacleDistance(value);
	ui.spinBoxObstacle->setValue(value);
}


void Gui::on_sliderObstacleLaserScanner_valueChanged(int value)
{
	// TODO: change to signal slot auto connection
	obstCheckThread->setMinObstacleDistanceLaser(value);
	ui.spinBoxObstacleLaserScanner->setValue(value);
}


void Gui::on_spinBoxObstacle_valueChanged(int value)
{
	// TODO: change to signal slot auto connection
	obstCheckThread->setMinObstacleDistance(value);
	ui.sliderObstacle->setValue(value);
}


void Gui::on_sliderPositionServo1_valueChanged(int position)
{
	//servos->setServoPosition(SERVO1, position);
	
	// pseudo code to avoid complier warning
	position++;
}


void Gui::on_btnClearLog_clicked()
{
	ui.textEditLog->clear();
}


void Gui::on_btnClearNetworkLog_clicked()
{
	ui.textEditNetworkLog->clear();
}


void Gui::on_btnReset_clicked()
{
	// reset the circuit (motors off etc.)
	circuit1->initCircuit();

	//circuit1->findRobotHardware();

	// stop motorTimer
	mrs1->drive(STOP);
 
	// stop commandTimer (for reading commands from ini-file)
	// startStopCommandClock(STOP);

	// Inizialize the (script) command counter
	//commandCounter = 0;
	// Inizialize the last command for the robot
	//lastCommand = "stop";

	// Name of the section to search for in the ini-file (script exection)
	//iniSection = "Sequence1";

	ui.textEditLog->append("Reseted.");

	// reset progressBars
	ui.progressBarSensor1->setValue(0);
	ui.progressBarSensor2->setValue(0);
	ui.progressBarSensor3->setValue(0);
	ui.progressBarSensor4->setValue(0);
	ui.progressBarSensor5->setValue(0);
	ui.progressBarSensor6->setValue(0);
	ui.progressBarSensor7->setValue(0);
	ui.progressBarSensor8->setValue(0);
	ui.progressBarSensor16->setValue(0);
	//ui.btnExecuteScript->setText("Execute s&cript");

	// set labels back in default position
	ui.lblPower1->setText("OFF");
	ui.lblPower1->setEnabled(false);
	ui.framePower1->setPalette(QPalette(QColor(255, 255, 255)));
	ui.lblPower2->setText("OFF");
	ui.lblPower2->setEnabled(false);
	ui.framePower2->setPalette(QPalette(QColor(255, 255, 255)));
	
	ui.lblDirection1->setText("FORWARD");
	ui.lblDirection1->setEnabled(false);
	ui.frameDirection1->setPalette(QPalette(QColor(255, 255, 255)));
	ui.lblDirection2->setText("FORWARD");
	ui.lblDirection2->setEnabled(false);
	ui.frameDirection2->setPalette(QPalette(QColor(255, 255, 255)));

	
	
	// reactivate buttons
	//ui.btnExecuteScript->setEnabled(true);

	// reactivate sliders & Co.
	ui.sliderMotor1Speed->setEnabled(true);
	ui.spinBoxMotor1Speed->setEnabled(true);
	
	ui.sliderMotor2Speed->setEnabled(true);
	ui.spinBoxMotor2Speed->setEnabled(true);
}


/*
void Gui::on_btnConnectToLIRC_clicked()
{
	//winLIRC->requestNewConnection();
}
*/


void Gui::on_btnResetMovement1_clicked()
{
	// reset counter
	sensThread->resetDrivenDistance(MOTORSENSOR1);
	
 	// reset labels
	ui.labelDrivenDistance1->setText("0 cm");
 	ui.labelRevolutions1->setText("0");
}


void Gui::on_btnResetMovement2_clicked()
{
	// reset counter
	sensThread->resetDrivenDistance(MOTORSENSOR2);
 
	// reset labels
	ui.labelDrivenDistance2->setText("0 cm");
	ui.labelRevolutions2->setText("0");
}


void Gui::on_btnExit_clicked()
{
	mrs1->~Mrs();
	//close(); //< < <  with this, the ~Mrs is never called!!
}


void Gui::on_btnTest_clicked()
{
	mrs1->test();
}


/*
void Gui::on_btnExecuteScript_clicked()
{
	static bool toggle = false;

	
	if (toggle == false)
	{
		toggle = true;
		
		// deactivate edit fields and buttons
		ui.btnExecuteScript->setText("Stop s&cript");
		ui.btnStartMotorClock->setEnabled(false);
		ui.sliderMotorSpeed->setEnabled(false);
		ui.spinBoxMotorSpeed->setEnabled(false);
		// deactivate groupBoxes
		ui.groupBoxPower1->setEnabled(false);
		ui.groupBoxPower2->setEnabled(false);
		ui.groupBoxDirection1->setEnabled(false);
		ui.groupBoxDirection2->setEnabled(false);

		// stop sensor timer    < < < < < < ? ? ? ? ? < < < < < <
		// sensThread->startStopSensorClock(STOP);

		// start command timer
		//startStopCommandClock(START);    < < < < <
	}
	else
	{
		toggle = false;
		
		// activate edit fields and buttons
		ui.btnExecuteScript->setText("Execute s&cript");
		ui.btnStartMotorClock->setEnabled(true);
		ui.sliderMotorSpeed->setEnabled(true);
		ui.spinBoxMotorSpeed->setEnabled(true);
		// activate groupBoxes
		ui.groupBoxPower1->setEnabled(true);
		ui.groupBoxPower2->setEnabled(true);
		ui.groupBoxDirection1->setEnabled(true);
		ui.groupBoxDirection2->setEnabled(true);

		// stop command timer
		//startStopCommandClock(STOP); < < < < <
		
		// stop motorTimer
		// (to stop the robot, if still driving!)
		mrs1->drive(STOP);
	}
}
*/


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
}


void Gui::showDrivenDistance(int sensor, int distance)
{
	// show distance value in centimeters in a text label
	// erwartet den value als distance (z.B. in cm)!
	QString str;

	
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


void Gui::showAlarm(short int sensor, unsigned char state)
{
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


void Gui::showMotorStatus(unsigned char motor, unsigned char power, unsigned char direction)
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
					ui.framePower1->setPalette(QPalette(QColor(64, 255, 64)));
					
					ui.lblDirection1->setEnabled(true);
					ui.frameDirection1->setPalette(QPalette(QColor(64, 255, 64)));
					break;
				case OFF:
					//ui.lblPower1->setText("OFF");
					ui.lblPower1->setEnabled(false);
					ui.framePower1->setPalette(QPalette(QColor(255, 255, 255)));
					
					ui.lblDirection1->setEnabled(false);
					ui.frameDirection1->setPalette(QPalette(QColor(255, 255, 255)));
					break;
				case SAME:
					break;
			}

			switch (direction)
			{
				case CLOCKWISE:
					ui.lblDirection1->setText("FORWARD");
					ui.lblDirection1->setEnabled(true);
					ui.frameDirection1->setPalette(QPalette(QColor(64, 255, 64)));
					return;
					break;
				case COUNTERCLOCKWISE:
					ui.lblDirection1->setText("BACKWARD");
					ui.lblDirection1->setEnabled(true);
					ui.frameDirection1->setPalette(QPalette(QColor(64, 255, 64)));
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
					ui.framePower2->setPalette(QPalette(QColor(64, 255, 64)));
					
					ui.lblDirection1->setEnabled(true);
					ui.frameDirection1->setPalette(QPalette(QColor(64, 255, 64)));
					break;
				case OFF:
					//ui.lblPower2->setText("OFF");
					ui.lblPower2->setEnabled(false);
					ui.framePower2->setPalette(QPalette(QColor(255, 255, 255)));
					
					ui.lblDirection2->setEnabled(false);
					ui.frameDirection2->setPalette(QPalette(QColor(255, 255, 255)));
					break;
				case SAME:
					break;
			}

			switch (direction)
			{
				case CLOCKWISE:
					ui.lblDirection2->setText("FORWARD");
					ui.lblDirection2->setEnabled(true);
					ui.frameDirection2->setPalette(QPalette(QColor(64, 255, 64)));
					return;
					break;
				case COUNTERCLOCKWISE:
					ui.lblDirection2->setText("BACKWARD");
					ui.lblDirection2->setEnabled(true);
					ui.frameDirection2->setPalette(QPalette(QColor(64, 255, 64)));
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


/*
void Gui::on_ckeckBoxSaveSettings_stateChanged(int state)
{
	// save button 
	//mrs1->saveOnExit = state;
	//setCheckBoxSaveSettings(state);
}
*/


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


int Gui::getSliderObstacleValue()
{
	return ui.sliderObstacle->value();
}


int Gui::getSliderObstacleLaserScannerValue()
{
	return ui.sliderObstacleLaserScanner->value();
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


void Gui::setSliderObstacleValue(int value)
{
	ui.sliderObstacle->setValue(value);
}


void Gui::setSliderObstacleLaserScannerValue(int value)
{
	ui.sliderObstacleLaserScanner->setValue(value);
}


/*
void Gui::setSliderPositionServo1(int value)
{
	ui.sliderPositionServo1->setValue(value);
}
*/


void Gui::setCheckBoxSaveSettings(Qt::CheckState state)
{
	ui.checkBoxSaveSettings->setCheckState(state);
}


void Gui::enableCamControls(bool state)
{
	// enable/disable all camera elements in the gui
	ui.groupBoxCamera->setEnabled(state);
}


void Gui::enableLaserScannerControls(bool state)
{
	// enable/disable all laser scanner in the gui
	//ui.groupBoxLaserScanner->setEnabled(state);
	
	ui.sliderObstacleLaserScanner->setEnabled(state);
	ui.spinBoxObstacleLaserScanner->setEnabled(state);
}


void Gui::on_btnSavePicture_clicked()
{
	saveCamImage();
}


void Gui::on_btnAutoSavePicture_clicked()
{
	static unsigned char toggle = OFF;
	
	if (toggle == OFF)
	{
		toggle = ON;
		
		// change text of button
		ui.btnAutoSavePicture->setText("&Stop");
		
		if (cam1->isRunning() == false)
		{
			appendLog("Starting cam thread...", false);
			cam1->start();
			appendLog("Camera thread started.");
		}
	}
	else
	{
		toggle = OFF;
		// change text of button
		ui.btnAutoSavePicture->setText("&Start");
		
		cam1->stop();
		appendLog("Camera thread stopped.");
	}
}


void Gui::on_btnClearPicture_clicked()
{
	// clear the picture
	//ui.label->clear();
	//ui.labelCmucam->setText("Camera");
	
	/*
	// fill the camera label with a black image
	QImage blackImage = QImage(350, 286, QImage::Format_RGB32);
	// fill image with color palette entry 0
	blackImage.fill(0); // Fixme: Funktioniert nicht!! Wie füllt man ein Bild?!?
	
	setCamImage(blackImage);
	*/
}


void Gui::setCamImage(IplImage* frame)
{
	//	ui.labelCmucam->setPixmap(QPixmap::fromImage(image.scaled(350, 350, Qt::KeepAspectRatio, Qt::FastTransformation)));
	
	// show pic only live, when ckecked in GUI
	if ( ui.checkBoxCamPicLive->isChecked() )
	{
		// set image 
		ui.frameCamera->setImage((unsigned char*)frame->imageData, frame->width, frame->height, frame->nChannels * frame->depth);
		
		// refresh picture
		ui.frameCamera->updateGL();
	}
	
	
	// save pic, when ckecked in GUI
	if ( ui.checkBoxAutoSave->isChecked() )
	{
		saveCamImage();
	}
}


void Gui::saveCamImage(void)
{
	//---------------------------
	// grab cam pic from Qt-GUI
	//---------------------------
	static QDateTime timestamp;
	
	
	// TODO: Seltsame Bildmaße! :-?
	cameraPicToSave = QPixmap::grabWindow(QWidget::winId(), ui.groupBoxCamera->x()+19, ui.groupBoxCamera->y()+19, 474, 353);
	
	//---------------------------------------------------------------------
	// save image to disk, but not within the same seond (same timestamp)
	//---------------------------------------------------------------------
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
		ui.btnDrive->setEnabled(true);
		emit enableRemoteControlListening(false);
	}
}


void Gui::on_btnSimulate_clicked()
{
	if (ui.btnSimulate->isChecked())
	{
		// nice red button background
		ui.btnSimulate->setPalette(QPalette(QColor(255, 64, 64)));
		enableLaserScannerControls(true);
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
	// show the value in a label
	ui.labelLaserTop->setText(tr("%1").arg(value));

	
	// rescale the robot pixmap to the last scale
	// (the laser lines are rescaled automatically by in the slot)
	pixmapBot1->scale( (1 / (qreal)lastZoom), (1 / (qreal)lastZoom) );
	pixmapBot2->scale( (1 / (qreal)lastZoom), (1 / (qreal)lastZoom) );
	
	// scale to the actual slider value
	pixmapBot1->scale(value, value);
	pixmapBot2->scale(value, value);
	
	// store the actual scale
	lastZoom = value;
	
	// position the bot
	drawBotPicture();
	
	
	// change the y position of the laser lines, too
	qreal x = calculateLaserXpos();
	qreal y = calculateLaserYpos();
	
	for (int i=0, angle=-90; i<laserLineList->size(); i++, angle++)
	{
		// horizontal center:
		// x = middle of the bot pixmap in the view
		// y = set manually
		laserLineList->at(i)->setPos(x, y);
	}
}


void Gui::on_checkBoxAngleView_stateChanged(int state)
{
	if (state == Qt::Checked)
	{
		//------------
		// angle view
		//------------
		ui.checkBoxHiResView->setEnabled(true);
		switchToAngleView();
	}
	else
	{
		//------------
		// flat view
		//------------
		ui.checkBoxHiResView->setEnabled(false);
		switchToFlatView();
	}
}


void Gui::refreshLaserView(float *laserScannerValues, int *laserScannerFlags)
{
	int measuredLaserDistance = 0;
	int zoomView = ui.sliderZoom->value(); // get a scale to fit the beams into the window
	/*
	QPen orgPen = laserLineList->at(0)->pen();
	QBrush orgBrush = orgPen.brush();
	*/

	//----------------------------------------------------------------------------------------
	// Change the laser lines (color and length)
	//----------------------------------------------------------------------------------------
	// First: set/change the color of each line, depending on the flag set in the laserThread
	//----------------------------------------------------------------------------------------
	for (int i=0; i<laserLineList->size(); i++)
	{
		// check if there was an obstacle
		if (laserScannerFlags[i] == OBSTACLE)
		{
			// obstacle detected!
			//orgBrush.setColor(colorLaserObstacle);
			laserLineList->at(i)->setPen(QPen(colorLaserObstacle));
		}
		else
		{
			if (laserScannerFlags[i] == LARGESTFREEWAY)
			{
				// l a r g e s t   f r e e   w a y
				laserLineList->at(i)->setPen(QPen(colorLaserPreferredDrivingDirection));
			}
			else
			{
				if (laserScannerFlags[i] == CENTEROFLARGESTFREEWAY)
				{
					//  center of free way (only one line)
					laserLineList->at(i)->setPen(QPen(colorLaserCenterDrivingDirection));
				}
				else
				{
					//   n o   o b s t a c l e
					laserLineList->at(i)->setPen(QPen(colorLaserFreeWay));
				}
			}
		}
	}
	
	
	//---------------------------------------------------------------------------
	// Second: change the *length* of each line!
	//---------------------------------------------------------------------------
	if (ui.checkBoxHiResView->checkState() == Qt::Checked) // this is NOT default
	{
		//
		// draw HIGH RES
		//
		// get the data from 0 to 180° (left to right)
		for (int i=0; i<laserLineList->size(); i++)
		{
			// get value from laser
			measuredLaserDistance = qRound(laserScannerValues[i]*FITTOFRAMEFACTOR*zoomView); // length in Pixel!!!

			//draw the first real line
			laserLineList->at(i)->setLine(0, 0, 0, measuredLaserDistance);
			
			/*
			// fill the spaces between the lines
			painter.rotate(0.25);
			painter.drawLine(0, 0, 0, measuredLaserDistance);
			painter.rotate(0.25);
			painter.drawLine(0, 0, 0, measuredLaserDistance);
			painter.rotate(0.25);
			painter.drawLine(0, 0, 0, measuredLaserDistance);
			painter.rotate(0.25);
			*/
				
			// set tool tip of the line to the distance
			//laserLineList->at(i)->setToolTip(QString("%1 m / %2 Pixel").arg(laserScannerValues[i]).arg(measuredLaserDistance));
			laserLineList->at(i)->setToolTip(QString("%1 m (%2 deg)").arg(laserScannerValues[i]).arg(i));
		}
	}
	else
	{
		//
		// draw LOW RES (faster and default)
		//
		// /get the data from 180° to 0° (right to left!!)
		for (int i=0; i<laserLineList->size(); i++)
		{
			// get value from laser and
			// draw the lines at every 1°
			measuredLaserDistance = qRound(laserScannerValues[i]*FITTOFRAMEFACTOR*zoomView); // length in Pixel!!!
			laserLineList->at(i)->setLine(0, 0, 0, measuredLaserDistance);
			
			// set tool tip of the line to the distance
			//laserLineList->at(i)->setToolTip(QString("%1 m / %2 Pixel").arg(laserScannerValues[i]).arg(measuredLaserDistance));
			laserLineList->at(i)->setToolTip(QString("%1 m (%2 deg)").arg(laserScannerValues[i]).arg(i));
		}
	}

	//------------------------------------------------------
	// Third: draw some help lines / distances / dimensons
	// (flatView=false)
	//------------------------------------------------------
	drawLaserDistances(false);
}


void Gui::drawLaserDistances(bool flatView)
{
/*
	static bool firstDraw = true;
	QString dimensionText;
	int dimensionLine = 0;

	// reduce drawing actions to the laser group box!!!
	//painter.setClipRect(ui.groupBoxLaserScanner->x(), ui.groupBoxLaserScanner->y()+10, ui.groupBoxLaserScanner->width()-50, ui.groupBoxLaserScanner->height()-10, Qt::ReplaceClip);

	
	//------------------------------------------------
	// if the scale changed during this and the last call
	// or it is the first time for this
	// draw some help lines / distances / dimensons
	//------------------------------------------------
//	if ( (lastZoom != ui.sliderZoom->value()) || (firstDraw == true) )
	// FixMe: Draw only on changes to the scale.
	if (true)
	{
		firstDraw = false;
		// store the current scale to fit the lines into the window
		lastZoom = ui.sliderZoom->value();
		
		
		// set the y-position of the line/text
		dimensionLine = (FITTOFRAMEFACTOR*lastZoom);
		int newY = ui.groupBoxLaserScanner->height();
		
		// draw!
		if (flatView == false)
		{
			//
			// draw the arcs !!!
			//
			// translate the matrix
			// "-6" is the correction value to put it on the horizontal middle of the vertival 90° line!
			painter->resetTransform();
			painter->translate(ui.groupBoxLaserScanner->x()+ui.lineLaser0->x()-12, ui.groupBoxLaserScanner->y()+ui.lineLaser90->y()-20);
			
			// choose a nice pen color
			painter->setPen(colorHelpLine);
			
			// 0.5 meter to 6 meter (step 0.5 meter)
			for (float i=0.5; i<=6; i+=0.5)
			{
				//if (i==1)
				//			x,						y,			width,		height,		startAngle, spanAngle
				painter->drawArc(ui.lineLaser90->x() - (i*dimensionLine), (newY - (i*dimensionLine)), (i*dimensionLine)*2, (i*dimensionLine)*2, 0, 2880);
			}
			
			
			// new origin at the vertical "lineLaser90" in the GUI
			painter->resetTransform();
			
			// translate the matrix
			// "-6" is the correction value to put it on the horizontal middle of the vertival 90° line!
			painter->translate(ui.groupBoxLaserScanner->x() - 6 + ui.lineLaser90->x(), ui.groupBoxLaserScanner->y());
			
			// choose a nice pen color
			painter->setPen(colorHelpLineText);
			
			//-------------
			// 360° view
			//-------------
			// 0.5 meter to 6 meter (step 0.5 meter)
			for (float i=0.5; i<=6; i+=0.5)
			{
				//dimensionText = tr("--- %1 m").arg(i);
				dimensionText = tr("  %1 m").arg(i);
				// y-4 Pixel for putting it ON the higher than the later following arcs
				painter->drawText(1, (newY - (i*dimensionLine))-6, dimensionText.toAscii());
				//ui.lblScale1->move(5, (i*dimensionLine));
			}
		}
		else
		{
			//-------------
			// flatView
			//-------------
			// new origin at the vertical "lineLaser90" in the GUI
			painter->resetTransform();
			
			// translate the matrix
			// "-6" is the correction value to put it on the horizontal middle of the vertival 90° line!
			painter->translate(ui.groupBoxLaserScanner->x() - 6 + ui.lineLaser90->x(), ui.groupBoxLaserScanner->y());
			
			// choose a nice pen color
			painter->setPen(colorHelpLine);
			
			for (float i=0.5; i<=6; i+=0.5)
			{
				dimensionText = tr("----------------------------------------------------- %1 m").arg(i);
				painter->drawText(-100, (int)(i*dimensionLine), dimensionText.toAscii());
			}
		} // flatView
	}
*/
}


void Gui::initializePlots()
{
	//--------------------------------------
	// plot curve "MOTOR CURRENT" 1
	//--------------------------------------
	ui.qwtPlotCurrent1->setTitle("Motor 1");
	//ui.qwtPlotCurrent1->insertLegend(new QwtLegend(), QwtPlot::RightLegend);

	// Set axis titles
	ui.qwtPlotCurrent1->setAxisTitle(QwtPlot::xBottom, "Time/s");
	ui.qwtPlotCurrent1->setAxisTitle(QwtPlot::yLeft, "Current/mA");

	// Set axis scale (instead of using autoscale, which is default)
	// time
	ui.qwtPlotCurrent1->setAxisScale(QwtPlot::xBottom, 0, 60.0, 10);
	// Ampere (1000 mA, Step 200)
	//ui.qwtPlotCurrent1->setAxisScale(QwtPlot::yLeft,   0, 4000.0, 400);
	
	QColor col = Qt::red;
	curve1.setRenderHint(QwtPlotItem::RenderAntialiased);
	col.setAlpha(150);
	curve1.setPen(QPen(col));
	curve1.setBrush(col);
	
	
	//--------------------------------------
	// plot curve "MOTOR CURRENT" 2
	//--------------------------------------
	ui.qwtPlotCurrent2->setTitle("Motor 2");
	//ui.qwtPlotCurrent2->insertLegend(new QwtLegend(), QwtPlot::RightLegend);

	// Set axis titles
	ui.qwtPlotCurrent2->setAxisTitle(QwtPlot::xBottom, "Time/s");
	ui.qwtPlotCurrent2->setAxisTitle(QwtPlot::yLeft, "Current/mA");

	// Set axis scale (instead of using autoscale, which is default)
	// time
	ui.qwtPlotCurrent2->setAxisScale(QwtPlot::xBottom, 0, 60.0, 10);
	// Ampere (1000 mA, Step 200)
	//ui.qwtPlotCurrent2->setAxisScale(QwtPlot::yLeft,   0, 4000.0, 400);
	
	col = Qt::blue;
	curve2.setRenderHint(QwtPlotItem::RenderAntialiased);
	col.setAlpha(150);
	curve2.setPen(QPen(col));
	curve2.setBrush(col);
	
	
	//----------------------------------------------------------------------------
	// connect plotThread signal to "setPlotData"
	// (Whenever the plot thread has new data, the data are show in the GUI)
	//----------------------------------------------------------------------------
	connect(plotThread, SIGNAL( plotDataComplete1(double *, double *, int) ), this, SLOT( setPlotData1(double *, double *, int) ));
	connect(plotThread, SIGNAL( plotDataComplete2(double *, double *, int) ), this, SLOT( setPlotData2(double *, double *, int) ));
}


void Gui::showJoystickAxes(int axisNumber, int axisValue)
{
	// TODO: put axis numbers to ini-file
	if (axisNumber == 2)
		ui.sliderJoystickY->setValue(axisValue);
	
	if (axisNumber == 3)
		ui.sliderJoystickX->setValue(axisValue);
}


void Gui::showJoystickButtons(int buttonNumber, bool buttonState)
{
	// TODO: put button numbers to ini-file
	switch (buttonNumber)
	{
		case 0:
			ui.radioBtnJoy0->setChecked(buttonState);
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
			ui.radioBtnJoy10->setChecked(buttonState);
			break;
		case 11:
			ui.radioBtnJoy11->setChecked(buttonState);
			break;
	}
}


void Gui::createLaserScannerObjects()
{
	//=======================================================
	// OpenGL stuff (Laser lines)
	//=======================================================
	
	// set some nice colors for some widgets
	colorLaserObstacle =  QColor(255, 50, 50, 180); // light red
	colorLaserFreeWay = Qt::darkRed;
	colorLaserFreeWay.setAlpha(180);
	colorLaserPreferredDrivingDirection = QColor(7, 68, 30, 150); // green
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
	laserXPos = (ui.graphicsViewLaser->width() / 2) - ( pixmapBot1->pixmap().width() / 2 / startScale * lastZoom) ;
	laserYPos = ui.progressBarSensor4->height() + 20;
	
	// change scale of the robot pic to 1/10 to fit in the window and to fit on the size of the laser lines
	pixmapBot1->scale( (1.0 / startScale), (1.0 / startScale));
	
	// horizontal center
	pixmapBot1->setPos(laserXPos, laserYPos);
	
	// add the pixmap
	scene->addItem(pixmapBot1);
	
	// FIXME doesn't work
	// put one layer up (layer 2). All others share the same (unset) layer under the pixmap.
	pixmapBot1->setZValue(1);
	
	
	//=======================================================
	// create the laser line list
	//=======================================================
	laserLineList = new QList <QGraphicsLineItem*>();

	/*
	//TODO: HIGHRES MODE with 4x180 lines
	//draw the first real line
	painter.drawLine(0, 0, 0, measuredLaserDistance);
	
	// fill the spaces between the lines
	painter.rotate(0.25);
	painter.drawLine(0, 0, 0, measuredLaserDistance);
	painter.rotate(0.25);
	painter.drawLine(0, 0, 0, measuredLaserDistance);
	painter.rotate(0.25);
	painter.drawLine(0, 0, 0, measuredLaserDistance);
	painter.rotate(0.25);
	*/

	
	// TODO: check if always 180 lines!
	// create 180 laser lines (0 to 179)
	for (int i=-90; i<90; i++)
	{
		QGraphicsLineItem *line = new QGraphicsLineItem();
		
		// FIXME doest not work: Make a thicker line to fil the gaps
		/*
		// set line color and position
		QPen pen(Qt::green, 3, Qt::DashDotLine, Qt::RoundCap, Qt::RoundJoin);
		pen.setWidth(3);
		//pen.setColor(colorLaserFreeWay);
		line->setPen(pen);
		*/
		
		// the length (and position) of the laser line in pixel
		line->setLine(0,0,0,0);
		
		// set position of each line
		line->rotate(i);
		
		// FIXME doesn't work?!
		line->setZValue(2);
		
		// add line to QList
		laserLineList->append(line);
		
		// add line to scene
		scene->addItem(line);
	}
	
	
	// set the 180° view per default
	switchToAngleView();
	

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

	
	// FIXME doesn't work
	// put one layer up (layer 2). All others share the same (unset) layer under the pixmap.
	pixmapBot2->setZValue(3);
}


void Gui::switchToAngleView()
{
	qreal x = calculateLaserXpos();
	qreal y = calculateLaserYpos();
	
	
	//for (int i=0, angle=-90; i<laserLineList->size(); i++, angle++)
	for (int i=0, angle=90; i<laserLineList->size(); i++, angle--)
	{
		// reset transform or rotation
		laserLineList->at(i)->resetTransform();
		
		// set position of each line
		laserLineList->at(i)->rotate(angle);
		
		// horizontal center:
		// x = middle of the bot pixmap in the view
		// y = set manually
		laserLineList->at(i)->setPos(x, y);
	}
}


void Gui::switchToFlatView()
{
	//for (int i=0; i<laserLineList->size(); i++)
	for (int i=laserLineList->size()-1; i>=0; i--)
	{
		// reset transform or rotation
		laserLineList->at(i)->resetTransform();
		
		// x = middle of the bot pixmap in the view + half of the number of lines (the middle laser line is on the middle of the graphicsView)
		// y = set manually
		//laserLineList->at(i)->setPos((qreal)(ui.graphicsViewLaser->width() / 2) + (laserLineList->size() / 2) - i, 228);
		laserLineList->at(i)->setPos((qreal)(ui.graphicsViewLaser->width() / 2) - (laserLineList->size() / 2) + i, 128);
	}
}


void Gui::createLaserDistanceObjects()
{
	static bool firstDraw = true;
	QString dimensionText;
	int dimensionLine = 0;
	int newY = 0;
	qreal startArcX = 0;
	qreal startArcY = 0;
	qreal endArcX = 0;
	qreal endArcY = 0;
	
	
	// set colors
	colorHelpLine = Qt::white;
	colorHelpLineText = Qt::white;
	
	
	//--------------------------------------------------------------
	// scaling
	//--------------------------------------------------------------
	// store the current scale to fit the lines into the window
	lastZoom = ui.sliderZoom->value();
	// set the y-position of the line/text
	dimensionLine = (FITTOFRAMEFACTOR*lastZoom);
	newY = ui.graphicsViewLaser->height();
	
	
	//--------------------------------------------------------------
	// create the laser line distances
	//--------------------------------------------------------------
	laserDistanceLineList = new QList <QGraphicsPathItem*>();
	
	// create and add twelve semi circles
//	for (float i=0.5; i<=6; i+=0.5)
	for (float i=0.5; i<=0.5; i+=0.5)
	{
		startArcX = ((ui.graphicsViewLaser->width() / 2) - LASERLINELENGTH);
		startArcY = laserLineList->at(0)->y();
		
		endArcX = ((ui.graphicsViewLaser->width() / 2) + LASERLINELENGTH);
		endArcY = startArcY;
		
		// first create a path
//		QPainterPath path(QPointF(startArcX, startArcY));
		QPainterPath path(QPointF(200, 200));

		// void QPainterPath::arcTo ( qreal x, qreal y, qreal width, qreal height, qreal startAngle, qreal sweepLength )
//		path.arcTo(endArcX, endArcY, (LASERLINELENGTH + LASERLINELENGTH), LASERLINELENGTH, 0, 180);
		//path.arcTo((LASERLINELENGTH + LASERLINELENGTH), 0, (LASERLINELENGTH + LASERLINELENGTH), LASERLINELENGTH, 0, -180);
		path.moveTo(0,0);
		path.arcTo(0, 0, 200, 200, 0, -180);
		
		//path.arcTo( (ui.graphicsViewLaser->width() - LASERLINELENGTH) /*- (i*dimensionLine)*/, ( laserLineList->at(i)->y() /*- (i*dimensionLine)*/), (i*dimensionLine)*2, (i*dimensionLine)*2, 0, 2880);

		//			x,						y,			width,		height,		startAngle, spanAngle
		//painter->drawArc(ui.lineLaser90->x() - (i*dimensionLine), (newY - (i*dimensionLine)), (i*dimensionLine)*2, (i*dimensionLine)*2, 0, 2880);


		// create semi circle along the created path
		QGraphicsPathItem *semiCircle = new QGraphicsPathItem(path);

		
		// set semiCircle color and position
		semiCircle->setPen(QPen(colorHelpLine));
		
 		// FIXME: does not work. no line visible :-(
		//set color and width
		//semiCircle->setPen(QPen(QBrush(colorHelpLine), 3));

		
		// set position of each semiCircle
		// TODO
		
		// horizontal center:
		// x = middle of the bot pixmap in the view
		// y = set manually
		//semiCircle->setPos((qreal)(ui.graphicsViewLaser->width()/2), 228);
		
		// FIXME doesn't work?!
		semiCircle->setZValue(4);
		
		// add semiCircle to QList
		laserDistanceLineList->append(semiCircle);
		
		// add semiCircle to scene
		scene->addItem(semiCircle);
	}


	
/*	
	//
	// draw the arcs !!!
	//
	// translate the matrix
	// "-6" is the correction value to put it on the horizontal middle of the vertival 90° line!
	// FIXME: !!check where to positionate!!
	//painter->translate(ui.groupBoxLaserScanner->x()+ui.lineLaser0->x()-12, ui.graphicsViewLaser->y()+ui.lineLaser90->y()-20);
	
	
	// 0.5 meter to 6 meter (step 0.5 meter)
	for (float i=0.5; i<=6; i+=0.5)
	{
		//if (i==1)
		//			x,						y,			width,		height,		startAngle, spanAngle
		painter->drawArc(ui.lineLaser90->x() - (i*dimensionLine), (newY - (i*dimensionLine)), (i*dimensionLine)*2, (i*dimensionLine)*2, 0, 2880);
	}
	
	
	// new origin at the vertical "lineLaser90" in the GUI
	painter->resetTransform();
	
	// translate the matrix
	// "-6" is the correction value to put it on the horizontal middle of the vertival 90° line!
	painter->translate(ui.groupBoxLaserScanner->x() - 6 + ui.lineLaser90->x(), ui.groupBoxLaserScanner->y());
	
	// choose a nice pen color
	painter->setPen(colorHelpLineText);
	
	//-------------
	// 360° view
	//-------------
	// 0.5 meter to 6 meter (step 0.5 meter)
	for (float i=0.5; i<=6; i+=0.5)
	{
		//dimensionText = tr("--- %1 m").arg(i);
		dimensionText = tr("  %1 m").arg(i);
		// y-4 Pixel for putting it ON the higher than the later following arcs
		painter->drawText(1, (newY - (i*dimensionLine))-6, dimensionText.toAscii());
		//ui.lblScale1->move(5, (i*dimensionLine));
	}
*/
}


void Gui::drawBotPicture()
{
	// recalculate the middle position of the bot pixmap!
	laserXPos = (ui.graphicsViewLaser->width() / 2) - ( pixmapBot1->pixmap().width() / 2 / startScale * lastZoom) ;
	
	// horizontal center
	pixmapBot1->setPos(laserXPos, laserYPos);
	pixmapBot2->setPos(laserXPos, laserYPos);
}


qreal Gui::calculateLaserXpos()
{
	return ui.graphicsViewLaser->width() / 2;
}


qreal Gui::calculateLaserYpos()
{
	return laserYPos + pixmapBot1->pixmap().height()/startScale*0.66*lastZoom;
}
