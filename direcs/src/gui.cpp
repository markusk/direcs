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


Gui::Gui(bool useSmallGUI, SettingsDialog *s, JoystickDialog *j, AboutDialog *a, QMainWindow *parent) : QMainWindow(parent)
{
	// copy the pointer from the original SensorThread object
	settingsDialog = s;
	joystickDialog = j;
	aboutDialog = a;

	robotIsOn = false;
	consoleMode = false;
	useLargeGUI = true;

#ifdef ACTIVELASERVIEW
	laserXPos = 0; // correct value is set in the initLaserView()!!
	laserYPos = 0; // correct value is set in the initLaserView()!!
#endif

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
	if (useLargeGUI)
		ui.setupUi(this);
	else
		uiSmall.setupUi(this);


	// get background color for compass widget
//	QColor compassBackground = this->palette().window().color();
//	QColor compassBackground = this->palette().light().color();

	// change background color of compass gl object to background color from the main window
//	ui.frameCompass->setBackgroundColor(compassBackground);  // doesn't work. Qt Bug?

/*
	QPalette pal = ui.mainWindow->palette();
	pal.setColor(QPalette::Base, pal.color(QPalette::Window));
	ui.frameCompass->setPalette(pal);
*/

	// remote control is enabled by default. @sa Direcs::init()
	if (useLargeGUI)
		ui.actionRemote->setChecked(true);
	else
		uiSmall.actionRemote->setChecked(true);

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


	//----------------------------------------------------------------------------------
	// Whenever the state of the face detect check box changes, set the detection mode
	//----------------------------------------------------------------------------------
	if (useLargeGUI)
		connect(ui.checkBoxFaceDetection, SIGNAL( stateChanged(int) ), SIGNAL( enableFaceDetection(int) ));
	else
		connect(uiSmall.checkBoxFaceDetection, SIGNAL( stateChanged(int) ), SIGNAL( enableFaceDetection(int) ));

	//----------------------------------------------------------------------------------
	// Whenever the state of the face tracking check box changes, set the tracking mode
	//----------------------------------------------------------------------------------
	if (useLargeGUI)
		connect(ui.checkBoxFaceTracking, SIGNAL( stateChanged(int) ), SIGNAL( enableFaceTracking(int) ));
	else
		connect(uiSmall.checkBoxFaceTracking, SIGNAL( stateChanged(int) ), SIGNAL( enableFaceTracking(int) ));


#ifndef BUILDFORROBOT
	//----------------------------------------------------------------------------
	// Plot stuff
	//----------------------------------------------------------------------------
	initPlots();
#endif

	//----------------------------------------------------------------------------
	// Laser Scanner graphics Stuff (scene, view, lines, OpenGL etc.)
	//----------------------------------------------------------------------------
	// -> the laser lines and pixmap init stuff is now done AFTER the laser settings are read in the direcs main class!


	//----------------------------------
	// change text of buttons
	//----------------------------------
	if (useLargeGUI)
	{
		ui.actionDrive->setText("&Drive");
		ui.actionDrive->setToolTip("Start driving");
		ui.actionDrive->setStatusTip("Start driving");
	}
	else
	{
		uiSmall.actionDrive->setText("&Drive");
		uiSmall.actionDrive->setToolTip("Start driving");
		uiSmall.actionDrive->setStatusTip("Start driving");
	}
}


Gui::~Gui()
{
#ifdef ACTIVELASERVIEW
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
#endif

/*
	if (cameraOpened)
	{
		cvReleaseCapture( &capture );
		cameraOpened = false;
	}
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
	if (useLargeGUI)
	{
		ui.actionDrive->setEnabled(state);
		ui.actionReset->setEnabled(state);
		ui.actionSleep->setEnabled(state);
		/*
		ui.btnResetMovement1->setEnabled(state);
		ui.btnResetMovement2->setEnabled(state);
		ui.btnResetMovement3->setEnabled(state);
		ui.btnResetMovement4->setEnabled(state);
		*/
	}
	else
	{
		uiSmall.actionDrive->setEnabled(state);
		uiSmall.actionReset->setEnabled(state);
		uiSmall.actionSleep->setEnabled(state);
		/*
		uiSmall.btnResetMovement1->setEnabled(state);
		uiSmall.btnResetMovement2->setEnabled(state);
		uiSmall.btnResetMovement3->setEnabled(state);
		uiSmall.btnResetMovement4->setEnabled(state);
		*/
	}

	if (!robotIsOn)
	{
		/// \todo test if this really is needed. Is the heartbeaet in the sensorthread sufficient?
		// set the state LED to red
		setLEDHeartbeat(RED);
	}

	/// \todo still needed? state already checked in motorControl at lower level. :-)
	/*
	ui.btnPower1->setEnabled(state);
	ui.btnPower2->setEnabled(state);
	ui.btnPower3->setEnabled(state);
	ui.btnPower4->setEnabled(state);
	*/
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
		uiSmall.textEditGlobalLog->insertHtml(text);
	}

	if (CR == true) // default!
	{
		if (useLargeGUI)
		{
			ui.textEditGlobalLog->insertHtml("<br>");
		}
		else
		{
			uiSmall.textEditGlobalLog->insertHtml("<br>");
		}
	}

	// Ensures that the cursor is visible by scrolling the text edit if necessary.
	if (useLargeGUI)
	{
		ui.textEditGlobalLog->ensureCursorVisible();
	}
	else
	{
		uiSmall.textEditGlobalLog->ensureCursorVisible();
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
		uiSmall.textEditNetworkLog->insertHtml(newText);
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
			uiSmall.textEditNetworkLog->insertHtml("<br>");
		}
	}

	// Ensures that the cursor is visible by scrolling the text edit if necessary.
	if (useLargeGUI)
	{
		ui.textEditNetworkLog->ensureCursorVisible();
	}
	else
	{
		uiSmall.textEditNetworkLog->ensureCursorVisible();
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
		uiSmall.textEditSerialLog->insertHtml(newText);
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
			uiSmall.textEditSerialLog->insertHtml("<br>");
		}
	}

	// Ensures that the cursor is visible by scrolling the text edit if necessary.
	if (useLargeGUI)
	{
		ui.textEditSerialLog->ensureCursorVisible();
	}
	else
	{
		uiSmall.textEditSerialLog->ensureCursorVisible();
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
		else
		{
			uiSmall.actionDrive->setText("Stop &driving");
			uiSmall.actionDrive->setToolTip("Stop driving");
			uiSmall.actionDrive->setStatusTip("Stop driving");
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
		else
		{
			uiSmall.actionDrive->setText("&Drive");
			uiSmall.actionDrive->setToolTip("Start driving");
			uiSmall.actionDrive->setStatusTip("Start driving");
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

	// reset progressBars
	/*
  if (useLargeGUI)
  {
 ui.progressBarSensor1->setValue(0);
 ui.progressBarSensor2->setValue(0);
 ui.progressBarSensor3->setValue(0);
 ui.progressBarSensor4->setValue(0);
 ui.progressBarSensor5->setValue(0);
 ui.progressBarSensor6->setValue(0);
 ui.progressBarSensor7->setValue(0);
 ui.progressBarSensor8->setValue(0);
 ui.progressBarSensor16->setValue(0);
  }
  else
  {
 uiSmall.progressBarSensor1->setValue(0);
 uiSmall.progressBarSensor2->setValue(0);
 uiSmall.progressBarSensor3->setValue(0);
 uiSmall.progressBarSensor4->setValue(0);
 uiSmall.progressBarSensor5->setValue(0);
 uiSmall.progressBarSensor6->setValue(0);
 uiSmall.progressBarSensor7->setValue(0);
 uiSmall.progressBarSensor8->setValue(0);
 uiSmall.progressBarSensor16->setValue(0);
  }
	*/

	// reset labels
/*
  if (useLargeGUI)
  {
 ui.btnDirection1->setText("FORWARD");
 ui.btnDirection2->setText("FORWARD");
 ui.btnDirection3->setText("FORWARD");
 ui.btnDirection4->setText("FORWARD");
  }
  else
  {
 uiSmall.btnDirection1->setText("FORWARD");
 uiSmall.btnDirection2->setText("FORWARD");
 uiSmall.btnDirection3->setText("FORWARD");
 uiSmall.btnDirection4->setText("FORWARD");
  }
*/
}


void Gui::on_actionSleep_activated()
{
	// let the circuit sleep
	emit sleepCircuit();
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
	else
	{
		uiSmall.labelDrivenDistance1->setText("0 cm");
		uiSmall.labelRevolutions1->setText("0");
	}

	// reset counter
	emit resetDrivenDistance(MOTORSENSOR2);
	// reset labels
	if (useLargeGUI)
	{
		ui.labelDrivenDistance2->setText("0 cm");
		ui.labelRevolutions2->setText("0");
	}
	else
	{
		uiSmall.labelDrivenDistance2->setText("0 cm");
		uiSmall.labelRevolutions2->setText("0");
	}

	// reset counter
	emit resetDrivenDistance(MOTORSENSOR3);
	// reset labels
	if (useLargeGUI)
	{
		ui.labelDrivenDistance3->setText("0 cm");
		ui.labelRevolutions3->setText("0");
	}
	else
	{
		uiSmall.labelDrivenDistance3->setText("0 cm");
		uiSmall.labelRevolutions3->setText("0");
	}

	// reset counter
	emit resetDrivenDistance(MOTORSENSOR4);
	// reset labels
	if (useLargeGUI)
	{
		ui.labelDrivenDistance4->setText("0 cm");
		ui.labelRevolutions4->setText("0");
	}
	else
	{
		uiSmall.labelDrivenDistance4->setText("0 cm");
		uiSmall.labelRevolutions4->setText("0");
	}
}


/*
void Gui::on_btnResetMovement1_clicked()
{
	// reset counter
	emit resetDrivenDistance(MOTORSENSOR1);

	// reset labels
  if (useLargeGUI)
  {
 ui.labelDrivenDistance1->setText("0 cm");
 ui.labelRevolutions1->setText("0");
  }
  else
  {
 uiSmall.labelDrivenDistance1->setText("0 cm");
 uiSmall.labelRevolutions1->setText("0");
  }
}


void Gui::on_btnResetMovement2_clicked()
{
	// reset counter
	emit resetDrivenDistance(MOTORSENSOR2);

	// reset labels
  if (useLargeGUI)
  {
 ui.labelDrivenDistance2->setText("0 cm");
 ui.labelRevolutions2->setText("0");
  }
  else
  {
 uiSmall.labelDrivenDistance2->setText("0 cm");
 uiSmall.labelRevolutions2->setText("0");
  }
}


void Gui::on_btnResetMovement3_clicked()
{
	// reset counter
	emit resetDrivenDistance(MOTORSENSOR3);

	// reset labels
  if (useLargeGUI)
  {
 ui.labelDrivenDistance3->setText("0 cm");
 ui.labelRevolutions3->setText("0");
  }
  else
  {
 uiSmall.labelDrivenDistance3->setText("0 cm");
 uiSmall.labelRevolutions3->setText("0");
  }
}


void Gui::on_btnResetMovement4_clicked()
{
	// reset counter
	emit resetDrivenDistance(MOTORSENSOR4);

	// reset labels
  if (useLargeGUI)
  {
 ui.labelDrivenDistance4->setText("0 cm");
 ui.labelRevolutions4->setText("0");
  }
  else
  {
 uiSmall.labelDrivenDistance4->setText("0 cm");
 uiSmall.labelRevolutions4->setText("0");
  }
}
*/

void Gui::on_actionTest_activated()
{
	emit test();

/*
	capture = cvCreateCameraCapture(0);
	cameraOpened = true;

	this->processCam();
*/

	//
	// OpenCV tests
	//

/*
	kinect->getRGB(m_rgb);
//	kinect->getDepth(m_rgb);

	glBindTexture(GL_TEXTURE_2D, m_rgbTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 640, 480, 0, GL_RGB, GL_UNSIGNED_BYTE, &m_rgb[0]);
*/


//	ui.frameOpenCV->processOpenCV();


/*
	// grab image from the kinect frame in the GUI
	QImage qimage = ui.frameDepth->grabFrameBuffer();

	// convert QImage to OpeneCV's cv::Mat
	//
	// http://permalink.gmane.org/gmane.comp.lib.opencv/37800
	//
	//Mat qimage2mat(const QImage& qimage) {

	cv::Mat mat = cv::Mat(qimage.height(), qimage.width(), CV_8UC4, (uchar*)qimage.bits(), qimage.bytesPerLine());
	cv::Mat mat2 = cv::Mat(mat.rows, mat.cols, CV_8UC3 );
	int from_to[] = { 0,0,  1,1,  2,2 };
	cv::mixChannels( &mat, 1, &mat2, 1, from_to, 3 );

	// result is now in 'mat2'

// -- - -- --


	mImage.release(); // reset picture

	// load JPEG from disc
//	cv::Mat tmpImg = cv::imread( "puzzle.jpg" );
//	cv::cvtColor( tmpImg, mImage, CV_BGR2RGB);

	// depth image from Kinect now! (mat2)
	cv::cvtColor( mat2, mImage, CV_BGR2RGB);

	QImage tmp( (uchar*)mImage.data, mImage.cols, mImage.rows, mImage.step, QImage::Format_RGB888 );
	ui.lblOpenCV->setPixmap( QPixmap::fromImage( tmp ) );


	if (mImage.data)
	{
		cv::Mat gray;
		cv::cvtColor( mImage, gray, CV_RGB2GRAY );
		cv::Canny( gray, gray, 10, 30, 3 );

		QImage tmp( (uchar*)gray.data, gray.cols, gray.rows, gray.step, QImage::Format_Indexed8  );
		ui.lblOpenCV->setPixmap( QPixmap::fromImage( tmp ) );
	}
*/
}


#ifndef USEROBOTGUI
void Gui::on_actionAll_activated()
{
	if (useLargeGUI)
	{
		ui.dockVoltage->show();
		//	ui.dockCurrent->show();
		ui.dockState->show();
		ui.dockLog->show();

		// 	settingsDialog->show();
		// 	joystickDialog->show();

		// only show the cam window, if the cam usage is enabled
		if (ui.dockCamera->isEnabled())
		{
			ui.dockCamera->show();
		}
	}
	else
	{
		// no dock widgets in the small GUI
	}
}
#endif


#ifndef USEROBOTGUI
void Gui::on_actionCamera_activated()
{
//	if (ui.dockCamera->isEnabled())
//	{
	if (useLargeGUI)
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
	else
	{
		// no dock widgets in the small GUI
	}
//	}
}
#endif


#ifndef USEROBOTGUI
void Gui::on_actionVoltage_activated()
{
	if (useLargeGUI)
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
	else
	{
		// no dock widgets in the small GUI
	}
}
#endif


/*
#ifndef USEROBOTGUI
void Gui::on_actionCurrent_activated()
{
  if (useLargeGUI)
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
  else
  {
 if (uiSmall.dockCurrent->isVisible())
 {
	 uiSmall.dockCurrent->hide();
 }
 else
 {
	 uiSmall.dockCurrent->show();
 }
  }
}
#endif
*/


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
	else
	{
		// no dock widgets in the small GUI
	}
}
#endif


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
	else
	{
		// no dock widgets in the small GUI
	}
}
#endif


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
			else
			{
				uiSmall.labelDrivenDistance1->setText(QString("%1 cm").arg(distance));
			}
			return;
			break;
		case MOTORSENSOR2:
			if (useLargeGUI)
			{
				ui.labelDrivenDistance2->setText(QString("%1 cm").arg(distance));
			}
			else
			{
				uiSmall.labelDrivenDistance2->setText(QString("%1 cm").arg(distance));
			}
			return;
			break;
		case MOTORSENSOR3:
			if (useLargeGUI)
			{
				ui.labelDrivenDistance3->setText(QString("%1 cm").arg(distance));
			}
			else
			{
				uiSmall.labelDrivenDistance3->setText(QString("%1 cm").arg(distance));
			}
			return;
			break;
		case MOTORSENSOR4:
			if (useLargeGUI)
			{
				ui.labelDrivenDistance4->setText(QString("%1 cm").arg(distance));
			}
			else
			{
				uiSmall.labelDrivenDistance4->setText(QString("%1 cm").arg(distance));
			}
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
			if (useLargeGUI)
			{
				ui.lblVoltage1->setText(QString("%1").setNum(voltage, 'f', 1).append(" V"));
//	 			ui.progressBarVoltage1->setValue(voltage);
			}
			else
			{
				uiSmall.lblVoltage1->setText(QString("%1").setNum(voltage, 'f', 1).append(" V"));
// 				uiSmall.progressBarVoltage1->setValue(voltage);
			}

			// change color of the label depending on the voltage
			if (voltage < MINIMUMVOLTAGE1)
			{
				// change icon
				if (voltage > 0.0)
				{
					if (useLargeGUI)
					{
						ui.lblBattery1->setPixmap(QPixmap(":/images/images/battery-060.png"));
					}
					else
					{
						uiSmall.lblBattery1->setPixmap(QPixmap(":/images/images/battery-060.png"));
					}
				}
				else
				{
					if (useLargeGUI)
					{
						ui.lblBattery1->setPixmap(QPixmap(":/images/images/battery-missing.png"));
					}
					else
					{
						uiSmall.lblBattery1->setPixmap(QPixmap(":/images/images/battery-missing.png"));
					}
				}
			}
			else
			{
				// change icon
				if (useLargeGUI)
				{
					ui.lblBattery1->setPixmap(QPixmap(":/images/images/battery-100.png"));
				}
				else
				{
					uiSmall.lblBattery1->setPixmap(QPixmap(":/images/images/battery-100.png"));
				}
			}

			return;
			break;
		case VOLTAGESENSOR2:
			if (useLargeGUI)
			{
				ui.lblVoltage2->setText(QString("%1").setNum(voltage, 'f', 1).append(" V"));
//				ui.progressBarVoltage2->setValue(voltage);
			}
			else
			{
				uiSmall.lblVoltage2->setText(QString("%1").setNum(voltage, 'f', 1).append(" V"));
//				uiSmall.progressBarVoltage2->setValue(voltage);
			}

			// change color of the label depending on the voltage
			if (voltage < MINIMUMVOLTAGE2)
			{
				// change icon
				if (voltage > 0.0)
				{
					if (useLargeGUI)
					{
						ui.lblBattery2->setPixmap(QPixmap(":/images/images/battery-060.png"));
					}
					else
					{
						uiSmall.lblBattery2->setPixmap(QPixmap(":/images/images/battery-060.png"));
					}
				}
				else
				{
					if (useLargeGUI)
					{
						ui.lblBattery2->setPixmap(QPixmap(":/images/images/battery-missing.png"));
					}
					else
					{
						uiSmall.lblBattery2->setPixmap(QPixmap(":/images/images/battery-missing.png"));
					}
				}
			}
			else
			{
				// change icon
				if (useLargeGUI)
				{
					ui.lblBattery2->setPixmap(QPixmap(":/images/images/battery-100.png"));
				}
				else
				{
					uiSmall.lblBattery2->setPixmap(QPixmap(":/images/images/battery-100.png"));
				}
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
//					ui.btnPower1->setPalette(QPalette(labelFillColorGreen));
//					ui.btnDirection1->setPalette(QPalette(labelFillColorGreen));
					break;
				case OFF:
//					ui.btnPower1->setPalette( QApplication::palette() );
//					ui.btnDirection1->setPalette( QApplication::palette() );
					if (useLargeGUI)
					{
						ui.lblLEDBackRight1->setPixmap(QPixmap(":/images/images/led_gray.gif"));
					}
					else
					{
						uiSmall.lblLEDBackRight1->setPixmap(QPixmap(":/images/images/led_gray.gif"));
					}
					break;
			}

			switch (direction)
			{
				case FORWARD:
//					ui.btnDirection1->setText("BACKWARD");
//					ui.btnDirection1->setPalette(QPalette(labelFillColorGreen));
					if (useLargeGUI)
					{
						ui.lblLEDBackRight1->setPixmap(QPixmap(":/images/images/led_green.gif"));
					}
					else
					{
						uiSmall.lblLEDBackRight1->setPixmap(QPixmap(":/images/images/led_green.gif"));
					}
					break;
				case BACKWARD:
//					ui.btnDirection1->setText("FORWARD");
//					ui.btnDirection1->setPalette(QPalette(labelFillColorGreen));
					if (useLargeGUI)
					{
						ui.lblLEDBackRight1->setPixmap(QPixmap(":/images/images/led_red.gif"));
					}
					else
					{
						uiSmall.lblLEDBackRight1->setPixmap(QPixmap(":/images/images/led_red.gif"));
					}
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
//					ui.btnPower2->setPalette(QPalette(labelFillColorGreen));
//					ui.btnDirection2->setPalette(QPalette(labelFillColorGreen));
					break;
				case OFF:
//					ui.btnPower2->setPalette( QApplication::palette() );
//					ui.btnDirection2->setPalette( QApplication::palette() );
					if (useLargeGUI)
					{
						ui.lblLEDBackLeft2->setPixmap(QPixmap(":/images/images/led_gray.gif"));
					}
					else
					{
						uiSmall.lblLEDBackLeft2->setPixmap(QPixmap(":/images/images/led_gray.gif"));
					}
					break;
			}

			switch (direction)
			{
				case FORWARD:
//					ui.btnDirection2->setText("BACKWARD");
//					ui.btnDirection2->setPalette(QPalette(labelFillColorGreen));
					if (useLargeGUI)
					{
						ui.lblLEDBackLeft2->setPixmap(QPixmap(":/images/images/led_green.gif"));
					}
					else
					{
						uiSmall.lblLEDBackLeft2->setPixmap(QPixmap(":/images/images/led_green.gif"));
					}
					break;
				case BACKWARD:
//					ui.btnDirection2->setText("FORWARD");
//					ui.btnDirection2->setPalette(QPalette(labelFillColorGreen));
					if (useLargeGUI)
					{
						ui.lblLEDBackLeft2->setPixmap(QPixmap(":/images/images/led_red.gif"));
					}
					else
					{
						uiSmall.lblLEDBackLeft2->setPixmap(QPixmap(":/images/images/led_red.gif"));
					}
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
//					ui.btnPower3->setPalette(QPalette(labelFillColorGreen));
//					ui.btnDirection3->setPalette(QPalette(labelFillColorGreen));
					break;
				case OFF:
//					ui.btnPower3->setPalette( QApplication::palette() );
//					ui.btnDirection3->setPalette( QApplication::palette() );
					if (useLargeGUI)
					{
						ui.lblLEDFrontRight3->setPixmap(QPixmap(":/images/images/led_gray.gif"));
					}
					else
					{
						uiSmall.lblLEDFrontRight3->setPixmap(QPixmap(":/images/images/led_gray.gif"));
					}
					break;
			}

			switch (direction)
			{
				case FORWARD:
//					ui.btnDirection3->setText("BACKWARD");
//					ui.btnDirection3->setPalette(QPalette(labelFillColorGreen));
					if (useLargeGUI)
					{
						ui.lblLEDFrontRight3->setPixmap(QPixmap(":/images/images/led_green.gif"));
					}
					else
					{
						uiSmall.lblLEDFrontRight3->setPixmap(QPixmap(":/images/images/led_green.gif"));
					}
					break;
				case BACKWARD:
//					ui.btnDirection3->setText("FORWARD");
//					ui.btnDirection3->setPalette(QPalette(labelFillColorGreen));
					if (useLargeGUI)
					{
						ui.lblLEDFrontRight3->setPixmap(QPixmap(":/images/images/led_red.gif"));
					}
					else
					{
						uiSmall.lblLEDFrontRight3->setPixmap(QPixmap(":/images/images/led_red.gif"));
					}
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
//					ui.btnPower4->setPalette(QPalette(labelFillColorGreen));
//					ui.btnDirection4->setPalette(QPalette(labelFillColorGreen));
					break;
				case OFF:
//					ui.btnPower4->setPalette( QApplication::palette() );
//					ui.btnDirection4->setPalette( QApplication::palette() );
					if (useLargeGUI)
					{
						ui.lblLEDFrontLeft4->setPixmap(QPixmap(":/images/images/led_gray.gif"));
					}
					else
					{
						uiSmall.lblLEDFrontLeft4->setPixmap(QPixmap(":/images/images/led_gray.gif"));
					}
					break;
			}

			switch (direction)
			{
				case FORWARD:
//					ui.btnDirection4->setText("BACKWARD");
//					ui.btnDirection4->setPalette(QPalette(labelFillColorGreen));
					if (useLargeGUI)
					{
						ui.lblLEDFrontLeft4->setPixmap(QPixmap(":/images/images/led_green.gif"));
					}
					else
					{
						uiSmall.lblLEDFrontLeft4->setPixmap(QPixmap(":/images/images/led_green.gif"));
					}
					break;
				case BACKWARD:
//					ui.btnDirection4->setText("FORWARD");
//					ui.btnDirection4->setPalette(QPalette(labelFillColorGreen));
					if (useLargeGUI)
					{
						ui.lblLEDFrontLeft4->setPixmap(QPixmap(":/images/images/led_red.gif"));
					}
					else
					{
						uiSmall.lblLEDFrontLeft4->setPixmap(QPixmap(":/images/images/led_red.gif"));
					}
					break;
				case SAME:
					break;
			}
			break;
	} // switch
}


void Gui::showLaserFrontAngles(int largestFreeAreaStart, int largestFreeAreaEnd, int centerOfFreeWay, float width)
{
	int configuredRobotSlotWidth = settingsDialog->getSliderRobotSlotWidth();
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


	if (useLargeGUI)
	{
		ui.lblLaserFrontFreeArea->setText(QString("%1 - %2 deg").arg(largestFreeAreaStart).arg(largestFreeAreaEnd));
	//	ui.lblLaserFrontFreeEnd->setText(QString("%1 deg").arg(largestFreeAreaEnd));
		ui.lblLaserFrontFreeCenter->setText(QString("%1 deg").arg(centerOfFreeWay));
	}
	else
	{
		uiSmall.lblLaserFrontFreeArea->setText(QString("%1 - %2 deg").arg(largestFreeAreaStart).arg(largestFreeAreaEnd));
	//	uiSmall.lblLaserFrontFreeEnd->setText(QString("%1 deg").arg(largestFreeAreaEnd));
		uiSmall.lblLaserFrontFreeCenter->setText(QString("%1 deg").arg(centerOfFreeWay));
	}


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
			uiSmall.lblLaserFrontFreeWidth->setText(QString("%1").setNum(width, 'f', 0).append(" cm"));
		}

		// show the currently configured robot slot with with a prepended '>' or '<'
		if ( width > configuredRobotSlotWidth)
		{
			if (useLargeGUI)
			{
				ui.lblRobotSlotWidth->setText( QString("> %1 cm").arg(configuredRobotSlotWidth) );
			}
			else
			{
				uiSmall.lblRobotSlotWidth->setText( QString("> %1 cm").arg(configuredRobotSlotWidth) );
			}
		}
		else
		{
			if (useLargeGUI)
			{
				ui.lblRobotSlotWidth->setText( QString("< %1 cm").arg(configuredRobotSlotWidth) );
			}
			else
			{
				uiSmall.lblRobotSlotWidth->setText( QString("< %1 cm").arg(configuredRobotSlotWidth) );
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
			uiSmall.lblLaserFrontFreeWidth->setText("oo");
		}

		// show the currently configured robot slot with with a prepended '>' or '<'
		if (useLargeGUI)
		{
			ui.lblRobotSlotWidth->setText( QString("> %1 cm").arg(configuredRobotSlotWidth) );
		}
		else
		{
			uiSmall.lblRobotSlotWidth->setText( QString("> %1 cm").arg(configuredRobotSlotWidth) );
		}
	}


#ifdef ACTIVELASERVIEW
	// show the same width in the scene
	if (useLargeGUI)
	{
		widthTextFront->setText( ui.lblLaserFrontFreeWidth->text() );
	}
	else
	{
		widthTextFront->setText( uiSmall.lblLaserFrontFreeWidth->text() );
	}
#endif
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
			uiSmall.lblPreferredDirection->setPixmap(QPixmap(":/images/images/up.png"));
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
			uiSmall.lblPreferredDirection->setPixmap(QPixmap(":/images/images/left.png"));
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
			uiSmall.lblPreferredDirection->setPixmap(QPixmap(":/images/images/right.png"));
		}
		return;
	}
}

void Gui::showFaceTrackDirection(QString direction)
{
	if (direction == "UP")
	{
		if (useLargeGUI)
		{
			ui.lblFaceTrackTo->setPixmap(QPixmap(":/images/images/up.png"));
		}
		else
		{
			uiSmall.lblFaceTrackTo->setPixmap(QPixmap(":/images/images/up.png"));
		}
		return;
	}

	if (direction == "UPLEFT")
	{
		if (useLargeGUI)
		{
			ui.lblFaceTrackTo->setPixmap(QPixmap(":/images/images/upleft.png"));
		}
		else
		{
			uiSmall.lblFaceTrackTo->setPixmap(QPixmap(":/images/images/upleft.png"));
		}
		return;
	}

	if (direction == "UPRIGHT")
	{
		if (useLargeGUI)
		{
			ui.lblFaceTrackTo->setPixmap(QPixmap(":/images/images/upright.png"));
		}
		else
		{
			uiSmall.lblFaceTrackTo->setPixmap(QPixmap(":/images/images/upright.png"));
		}
		return;
	}

	if (direction == "DOWN")
	{
		if (useLargeGUI)
		{
			ui.lblFaceTrackTo->setPixmap(QPixmap(":/images/images/down.png"));
		}
		else
		{
			uiSmall.lblFaceTrackTo->setPixmap(QPixmap(":/images/images/down.png"));
		}
		return;
	}

	if (direction == "DOWNLEFT")
	{
		if (useLargeGUI)
		{
			ui.lblFaceTrackTo->setPixmap(QPixmap(":/images/images/downleft.png"));
		}
		else
		{
			uiSmall.lblFaceTrackTo->setPixmap(QPixmap(":/images/images/downleft.png"));
		}
		return;
	}

	if (direction == "DOWNRIGHT")
	{
		if (useLargeGUI)
		{
			ui.lblFaceTrackTo->setPixmap(QPixmap(":/images/images/downright.png"));
		}
		else
		{
			uiSmall.lblFaceTrackTo->setPixmap(QPixmap(":/images/images/downright.png"));
		}
		return;
	}

	if (direction == "LEFT")
	{
		if (useLargeGUI)
		{
			ui.lblFaceTrackTo->setPixmap(QPixmap(":/images/images/left.png"));
		}
		else
		{
			uiSmall.lblFaceTrackTo->setPixmap(QPixmap(":/images/images/left.png"));
		}
		return;
	}

	if (direction == "RIGHT")
	{
		if (useLargeGUI)
		{
			ui.lblFaceTrackTo->setPixmap(QPixmap(":/images/images/right.png"));
		}
		else
		{
			uiSmall.lblFaceTrackTo->setPixmap(QPixmap(":/images/images/right.png"));
		}
		return;
	}

	if (direction == "NONE")
	{
		if (useLargeGUI)
		{
			ui.lblFaceTrackTo->setPixmap(QPixmap(":/images/images/middle.png"));
		}
		else
		{
			uiSmall.lblFaceTrackTo->setPixmap(QPixmap(":/images/images/middle.png"));
		}
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
/*

 currently not in use...

	// show coordinates of the first detected face
	ui.lblFaces->setNum(faces);

	ui.lblFaceX->setNum(faceX);
	ui.lblFaceY->setNum(faceY);
	ui.lblFaceRadius->setNum(faceRadius);

	ui.lblLastFaceX->setNum(lastFaceX);
	ui.lblLastFaceY->setNum(lastFaceY);
*/

	Q_UNUSED(faces);
	Q_UNUSED(faceX);
	Q_UNUSED(faceY);
	Q_UNUSED(faceRadius);
	Q_UNUSED(lastFaceX);
	Q_UNUSED(lastFaceY);
}


void Gui::disableFaceDetection()
{
	appendLog("Face detection disabled!");
	if (useLargeGUI)
	{
		ui.checkBoxFaceDetection->setEnabled(false);
		ui.checkBoxFaceTracking->setEnabled(false);
	}
	else
	{
		uiSmall.checkBoxFaceDetection->setEnabled(false);
		uiSmall.checkBoxFaceTracking->setEnabled(false);
	}
}


void Gui::hideCameraControls()
{
#ifndef USEROBOTGUI
	if (useLargeGUI)
	{
		ui.dockCamera->hide();
	}
	else
	{
		// no dock widgets in the small GUI
	}
#endif
}


void Gui::disableCompass()
{
	appendLog("to do");
}


void Gui::setCamImage(QImage* image)
{

	if (useLargeGUI)
	{
		ui.lblCamera->setPixmap(pixmap.fromImage(*(image)));
		//ui.imageFrame->setPixmap( QPixmap::fromImage( tmp ) );
	}
	else
	{
		uiSmall.lblCamera->setPixmap(pixmap.fromImage(*(image)));
		//uiSmall.imageFrame->setPixmap( QPixmap::fromImage( tmp ) );
	}

/*
	// save pic, when ckecked in GUI
	if ( ui.checkBoxAutoSave->isChecked() )
	{
		saveCamImage();
	}
*/
}


void Gui::setCamImageDepth(QImage* image)
{

	if (useLargeGUI)
	{
		ui.lblCameraDepth->setPixmap(pixmap.fromImage(*(image)));
	}
	else
	{
		uiSmall.lblCameraDepth->setPixmap(pixmap.fromImage(*(image)));
	}
}


void Gui::setCamImageOpenCV(QImage* image)
{
	if (useLargeGUI)
	{
		ui.lblCameraOpenCV->setPixmap(pixmap.fromImage(*(image)));
	}
	else
	{
		uiSmall.lblCameraOpenCV->setPixmap(pixmap.fromImage(*(image)));
	}
}


/*
void Gui::on_btnKinectVideoRGB_clicked(bool checked)
{
	if (checked)
		emit setVideoMode(0);
}


void Gui::on_btnKinectVideoYUVRGB_clicked(bool checked)
{
	if (checked)
		emit setVideoMode(1);
}


void Gui::on_btnKinectVideoIR8Bit_clicked(bool checked)
{
	if (checked)
		emit setVideoMode(2);
}
*/

void Gui::on_spinBoxKinectAngle_valueChanged(int i)
{
	emit setKinectAngle(i);
}


void Gui::on_btnKinectResetAngle_clicked()
{
	if (useLargeGUI)
	{
		ui.spinBoxKinectAngle->setValue(0);
	}
	else
	{
		uiSmall.spinBoxKinectAngle->setValue(0);
	}

	emit resetKinectAngle();
}

void Gui::on_sliderThreshold_valueChanged(int value)
{
	emit setThreshold(value);
}


void Gui::saveCamImage(void)
{
/*
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
*/
}


/*
#ifndef BUILDFORROBOT
void Gui::setPlotData1(double *xval, double *yval, int size)
{
	//---------------
	// curveCurrent1
	//---------------

	// set curve with data
	curveCurrent1.setData(xval, yval, size);

	// attach data to curve
	curveCurrent1.attach(&qwtPlotCurrent1);

	// after changing the values, replot the curve
	qwtPlotCurrent1.replot();
}
#endif


#ifndef BUILDFORROBOT
void Gui::setPlotData2(double *xval, double *yval, int size)
{
	//---------------
	// curveCurrent2
	//---------------

	// set curve with data
	curveCurrent2.setData(xval, yval, size);

	// attach data to curve
	curveCurrent2.attach(&qwtPlotCurrent1); // add curve to qwtPlot 1 !!

	// after changing the values, replot the curve
	qwtPlotCurrent1.replot(); // replot qwtPlot 1 !!
}
#endif


#ifndef BUILDFORROBOT
void Gui::setPlotData3(double *xval, double *yval, int size)
{
	//---------------
	// curveCurrent3
	//---------------

	// set curve with data
	curveCurrent3.setData(xval, yval, size);

	// attach data to curve
	curveCurrent3.attach(&qwtPlotCurrent2); // curve 3 to qwtPlot 2

	// after changing the values, replot the curve
	qwtPlotCurrent2.replot();
}
#endif


#ifndef BUILDFORROBOT
void Gui::setPlotData4(double *xval, double *yval, int size)
{
	//---------------
	// curveCurrent4
	//---------------

	// set curve with data
	curveCurrent4.setData(xval, yval, size);

	// attach data to curve
	curveCurrent4.attach(&qwtPlotCurrent2); // curve 4 to qwtPlot 2

	// after changing the values, replot the curve
	qwtPlotCurrent2.replot();  // replot qwtPlot 2 !!
}
#endif
*/


#ifndef BUILDFORROBOT
void Gui::setPlotData5(double *xval, double *yval, int size)
{
	//---------------
	// curveVoltage1
	//---------------

	// set curve with data
	curveVoltage1.setData(xval, yval, size);

	// attach data to curve
	curveVoltage1.attach(&qwtPlotVoltage1);

	// after changing the values, replot the curve
	qwtPlotVoltage1.replot();
}
#endif


#ifndef BUILDFORROBOT
void Gui::setPlotData6(double *xval, double *yval, int size)
{
	//---------------
	// curveVoltage2
	//---------------

	// set curve with data
	curveVoltage2.setData(xval, yval, size);

	// attach data to curve
	curveVoltage2.attach(&qwtPlotVoltage2);

	// after changing the values, replot the curve
	qwtPlotVoltage2.replot();
}
#endif


#ifndef BUILDFORROBOT
void Gui::setPlotDataHeartbeat(double *xval, double *yval, int size)
{
	//---------------
	// Heartbeat
	//---------------

	// set curve with data
	curveHeartbeat.setData(xval, yval, size);

	// attach data to curve
	curveHeartbeat.attach(&qwtPlotHeartbeat);

	// after changing the values, replot the curve
	qwtPlotHeartbeat.replot();
}
#endif

/*
void Gui::on_btnPower1_toggled(bool checked)
{
	if (checked)
	{
		// motor on
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
		emit drive(MOTOR1OFF);
	}
}


void Gui::on_btnPower2_toggled(bool checked)
{
	if (checked)
	{
		// motor on
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
		emit drive(MOTOR2OFF);
	}
}


void Gui::on_btnPower3_toggled(bool checked)
{
	if (checked)
	{
		// motor on
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
		emit drive(MOTOR3OFF);
	}
}


void Gui::on_btnPower4_toggled(bool checked)
{
	if (checked)
	{
		// motor on
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
		emit drive(MOTOR4OFF);
	}
}


void Gui::on_btnDirection1_toggled(bool checked)
{
	if (checked)
	{
		// backward
		ui.btnDirection1->setText("BACKWARD");
		if (ui.btnPower1->isChecked())
		{
			emit drive(MOTOR1BW);
		}
	}
	else
	{
		// forward
		ui.btnDirection1->setText("FORWARD");
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
		ui.btnDirection2->setText("BACKWARD");
		if (ui.btnPower2->isChecked())
		{
			emit drive(MOTOR2BW);
		}
	}
	else
	{
		// forward
		ui.btnDirection2->setText("FORWARD");
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
		ui.btnDirection3->setText("BACKWARD");
		if (ui.btnPower3->isChecked())
		{
			emit drive(MOTOR3BW);
		}
	}
	else
	{
		// forward
		ui.btnDirection3->setText("FORWARD");
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
		ui.btnDirection4->setText("BACKWARD");
		if (ui.btnPower4->isChecked())
		{
			emit drive(MOTOR4BW);
		}
	}
	else
	{
		// forward
		ui.btnDirection4->setText("FORWARD");
		if (ui.btnPower4->isChecked())
		{
			emit drive(MOTOR4FW);
		}
	}
}
*/

void Gui::on_actionRemote_activated()
{
	// if remote control is enabled, disable some controls in the GUI
	if (useLargeGUI)
	{
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
	else
	{
		if (uiSmall.actionRemote->isChecked())
		{
			uiSmall.actionDrive->setEnabled(false);
			emit enableRemoteControlListening(true);
		}
		else
		{
			if (robotIsOn)
			{
				uiSmall.actionDrive->setEnabled(true);
			}
			emit enableRemoteControlListening(false);
		}
	}
}


void Gui::on_actionSimulate_activated()
{
	if (useLargeGUI)
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
	else
	{
		if (uiSmall.actionSimulate->isChecked())
		{
			uiSmall.actionSimulate->setStatusTip("Disable simulation");
			uiSmall.actionSimulate->setToolTip("Disable simulation");
			emit simulate(true);
		}
		else
		{
			uiSmall.actionSimulate->setStatusTip("Simulate");
			uiSmall.actionSimulate->setToolTip("Simulate");
			emit simulate(false);
		}
	}
}


#ifdef ACTIVELASERVIEW
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
#endif


void Gui::on_checkBoxMirror_stateChanged(int state)
{
	Q_UNUSED(state);

	// QtGL class!!
	//	ui.frameCamera->enableMirrorMode(state);
	//	uiSmall.frameCamera->enableMirrorMode(state);
}


void Gui::on_checkBoxFaceDetection_stateChanged(int state)
{
	// en/disable face *tracking* checkbox
	if (state == Qt::Checked)
	{
		if (useLargeGUI)
		{
			ui.checkBoxFaceTracking->setEnabled(true);
		}
		else
		{
			uiSmall.checkBoxFaceTracking->setEnabled(true);
		}
	}
	else
	{
		if (useLargeGUI)
		{
			ui.checkBoxFaceTracking->setEnabled(false);
		}
		else
		{
			uiSmall.checkBoxFaceTracking->setEnabled(false);
		}
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
		if (useLargeGUI)
		{
			ui.lblFaceTrackTo->setPixmap(QPixmap(":/images/images/middle.png"));
		}
		else
		{
			uiSmall.lblFaceTrackTo->setPixmap(QPixmap(":/images/images/middle.png"));
		}
	}
}


#ifdef ACTIVELASERVIEW
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
	if (useLargeGUI)
	{
		laserXPos = (ui.graphicsViewLaser->width()  / 2);
		laserYPos = (ui.graphicsViewLaser->height() / 2);
	}
	else
	{
		laserXPos = (uiSmall.graphicsViewLaser->width()  / 2);
		laserYPos = (uiSmall.graphicsViewLaser->height() / 2);
	}


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
	// rotate every laser line by one degree
	// Be sure to rotate that the middle of the laserAngle is always at a vertical 90 degree in the scene!
	// (Rotate laser line counterclockwise by 180 degrees and further by the half of the laserAngle)
	i = 0;
	angle = ( 180 - (laserscannerAngleFront/2) );

	while (i<laserLineListFront->size())
	{
		laserLineListFront->at(i)->rotate(angle);

		i++;
		angle += laserscannerResolutionFront;
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
		uiSmall.sliderZoom->setValue(STARTZOOMLEVEL);
	}
}
#endif


#ifdef ACTIVELASERVIEW
void Gui::refreshLaserViewFront(QList <float> laserScannerValues, QList <int> laserScannerFlags)
{
	qreal x = 0.0;
	qreal y = 0.0;
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
	if (useLargeGUI)
	{
		zoomView = ui.sliderZoom->value(); // get a scale to fit the beams into the window
	}
	else
	{
		zoomView = uiSmall.sliderZoom->value(); // get a scale to fit the beams into the window
	}


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
	// /get the data from 180 to 0 (right to left!!)
	for (int i=0; i<laserLineListFront->size(); i++)
	{
		// get value from laser and
		// draw the lines at every 1
		laserLineLength = qRound(laserScannerValues[i]*FITTOFRAMEFACTOR*zoomView); // length in Pixel!!!

		laserLineListFront->at(i)->setLine(0, 0, 0, laserLineLength);

		// set tool tip of the line to the distance
		laserLineListFront->at(i)->setToolTip( QString("FRONT: %1 m / %2 deg / Flag=%3 / %4 Pixel").arg(laserScannerValues[i]).arg(i).arg(laserScannerFlags[i]).arg(laserLineLength) );

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
#endif


#ifdef ACTIVELASERVIEW
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
	if (useLargeGUI)
	{
		zoomView = ui.sliderZoom->value(); // get a scale to fit the beams into the window
	}
	else
	{
		zoomView = uiSmall.sliderZoom->value(); // get a scale to fit the beams into the window
	}


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
#endif


#ifdef ACTIVELASERVIEW
void Gui::setRobotPosition(QGraphicsSceneMouseEvent* mouseEvent)
{
	//qreal diff = laserFrontYPos - laserRearYPos;

	// new y pos
	laserYPos = mouseEvent->scenePos().y();
	//laserRearYPos = laserFrontYPos - diff;

	// new x pos
	laserXPos = mouseEvent->scenePos().x();

	// refresh laserView
	if (useLargeGUI)
	{
		on_sliderZoom_valueChanged(ui.sliderZoom->value());
	}
	else
	{
		on_sliderZoom_valueChanged(uiSmall.sliderZoom->value());
	}
}
#endif


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


#ifdef ACTIVELASERVIEW
void Gui::zoomLaserView(QGraphicsSceneWheelEvent* wheelEvent)
{
	int zoomValue = 0;


	if (useLargeGUI)
	{
		zoomValue = ui.sliderZoom->value();
	}
	else
	{
		zoomValue = uiSmall.sliderZoom->value();
	}


	if (wheelEvent->delta() > 0)
	{
		zoomValue++;
	}
	else
	{
		zoomValue--;
	}

	// refresh laserView (set zoom slider)
	if (useLargeGUI)
	{
		ui.sliderZoom->setValue(zoomValue);
	}
	else
	{
		uiSmall.sliderZoom->setValue(zoomValue);
	}
}
#endif


#ifdef ACTIVELASERVIEW
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
	if (useLargeGUI)
	{
		ui.graphicsViewLaser->setScene(scene);
	}
	else
	{
		uiSmall.graphicsViewLaser->setScene(scene);
	}

	// enable OpenGL rendering with antialiasing (and direct hardware rendering (if supportet by the hardware))
	if (useLargeGUI)
	{
		ui.graphicsViewLaser->setViewport(new QGLWidget(QGLFormat(QGL::DoubleBuffer | QGL::DirectRendering)));
	}
	else
	{
		uiSmall.graphicsViewLaser->setViewport(new QGLWidget(QGLFormat(QGL::DoubleBuffer | QGL::DirectRendering)));
	}

	//=======================================================
	// add robot picture1
	//=======================================================
	// add items to the scene
	pixmapBot1 = new QGraphicsPixmapItem(QPixmap(":/images/images/bot_from_above.png"));

	// init the scale for the laser line / distances drawing stuff
	if (useLargeGUI)
	{
		lastZoom = ui.sliderZoom->value();
	}
	else
	{
		lastZoom = uiSmall.sliderZoom->value();
	}

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
#endif


#ifdef ACTIVELASERVIEW
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
#endif


#ifdef ACTIVELASERVIEW
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
#endif


#ifdef ACTIVELASERVIEW
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
#endif


#ifndef BUILDFORROBOT
void Gui::initPlots()
{
	// create to qwt plot objects and place them in the GUI
	if (useLargeGUI)
	{
		qwtPlotVoltage1.setParent(ui.widgetVoltage1);
		qwtPlotVoltage2.setParent(ui.widgetVoltage2);
	}
	else
	{
		qwtPlotVoltage1.setParent(uiSmall.widgetVoltage1);
		qwtPlotVoltage2.setParent(uiSmall.widgetVoltage2);
	}

	//	qwtPlotCurrent1.setParent(ui.widgetCurrent1and2);
	//	qwtPlotCurrent2.setParent(ui.widgetCurrent3and4);       or
	//	qwtPlotCurrent1.setParent(uiSmall.widgetCurrent1and2);
	//	qwtPlotCurrent2.setParent(uiSmall.widgetCurrent3and4);

	if (useLargeGUI)
	{
		qwtPlotHeartbeat.setParent(ui.widgetHeartbeat);
	//	ui.widgetHeartbeat->resize(205, 100); // 305 x 161
	//	qwtPlotHeartbeat.resize(205, 100);
	}
	else
	{
		qwtPlotHeartbeat.setParent(uiSmall.widgetHeartbeat);
	//	uiSmall.widgetHeartbeat->resize(205, 100); // 305 x 161
	//	qwtPlotHeartbeat.resize(205, 100);
	}

	// get the current application font
	QFont applicationFont = QApplication::font();


	//--------------------------------------
	// plot curve "MOTOR CURRENT" 1
	//--------------------------------------
	// set the font for the plot widget
	qwtPlotCurrent1.setAxisFont(QwtPlot::xBottom, applicationFont);
	qwtPlotCurrent1.setAxisFont(QwtPlot::yLeft, applicationFont);
	qwtPlotCurrent1.setAxisFont(QwtPlot::axisCnt, applicationFont);

	// add a grid
	gridCurrent1.attach(&qwtPlotCurrent1);
	gridCurrent1.setPen( QPen(QColor(gridColor), 0.0, Qt::DotLine) );

	// set title
	qwtPlotCurrent1.setTitle("Motor 1 + 2");

	// Set axis scale (instead of using autoscale, which is default)
	// min (0.0), time (60.0 sec)
	qwtPlotCurrent1.setAxisScale(QwtPlot::xBottom, 0.0, 60.0);

	curveCurrent1.setRenderHint(QwtPlotItem::RenderAntialiased);
	// set curve color
	curveCurrent1.setPen(QPen(labelFillColorBlue));
	// fill the area below the curve
	// curveCurrent1.setBrush(labelFillColorBlue);


	//--------------------------------------
	// plot curve "MOTOR CURRENT" 2
	//--------------------------------------
	// get the current application font again
	applicationFont = QApplication::font();

	// set the font for the plot widget
	qwtPlotCurrent2.setAxisFont(QwtPlot::xBottom, applicationFont);
	qwtPlotCurrent2.setAxisFont(QwtPlot::yLeft, applicationFont);
	qwtPlotCurrent2.setAxisFont(QwtPlot::axisCnt, applicationFont);

	// add a grid
	gridCurrent2.attach(&qwtPlotCurrent2);
	gridCurrent2.setPen( QPen(QColor(gridColor), 0.0, Qt::DotLine) );

	// set title
	qwtPlotCurrent2.setTitle("Motor 3 + 4");

	// Set axis scale (instead of using autoscale, which is default)
	// time (60 sec)
	qwtPlotCurrent2.setAxisScale(QwtPlot::xBottom, 0.0, 60.0);

	curveCurrent2.setRenderHint(QwtPlotItem::RenderAntialiased);
	// set curve color
	curveCurrent2.setPen(QPen(labelFillColorRed));
	// fill the area below the curve
	// curveCurrent2.setBrush(labelFillColorRed);


	//--------------------------------------
	// plot curve "MOTOR CURRENT" 3
	//--------------------------------------
	curveCurrent3.setRenderHint(QwtPlotItem::RenderAntialiased);
	// set curve color
	curveCurrent3.setPen(QPen(labelFillColorBlue));
	// fill the area below the curve
	// curveCurrent3.setBrush(labelFillColorBlue);

	//--------------------------------------
	// plot curve "MOTOR CURRENT" 4
	//--------------------------------------
	curveCurrent4.setRenderHint(QwtPlotItem::RenderAntialiased);
	// set curve color
	curveCurrent4.setPen(QPen(labelFillColorRed));
	// fill the area below the curve
	// curveCurrent4.setBrush(labelFillColorRed);


	//--------------------------------------
	// plot curve "VOLTAGE 1"
	//--------------------------------------
	// get the current application font
	applicationFont = QApplication::font();
	// set this font for the plot widget
	qwtPlotVoltage1.setAxisFont(QwtPlot::xBottom, applicationFont);
	qwtPlotVoltage1.setAxisFont(QwtPlot::yLeft, applicationFont);
	qwtPlotVoltage1.setAxisFont(QwtPlot::axisCnt, applicationFont);

	// add a grid
	gridVoltage1.attach(&qwtPlotVoltage1);
	gridVoltage1.setPen( QPen(QColor(gridColor), 0.0, Qt::DotLine) );

	// set background color
	//qwtPlotVoltage1.setCanvasBackground(QColor(Qt::black));

	// on the smal GUI there is no space for a title
	if (useLargeGUI)
	{
		// set title
		qwtPlotVoltage1.setTitle("12 V Battery");
	}

	// Set axis scale (instead of using autoscale, which is default)
	// maximum voltage value at which the axis should and, and the steps between each divider
	qwtPlotVoltage1.setAxisScale(QwtPlot::yLeft, 0.0, MAXIMUMPLOTVOLTAGE1);

	// Set axis scale (instead of using autoscale, which is default)
	// time (60 sec)
	qwtPlotVoltage1.setAxisScale(QwtPlot::xBottom, 0.0, 60.0);

	curveVoltage1.setRenderHint(QwtPlotItem::RenderAntialiased);
	// set curve color
	curveVoltage1.setPen(QPen(labelFillColorBlue));
	// fill the area below the curve
	// curveVoltage1.setBrush(labelFillColorBlue);


	//--------------------------------------
	// plot curve "VOLTAGE 2"
	//--------------------------------------
	// get the current application font
	applicationFont = QApplication::font();
	// set this font for the plot widget
	qwtPlotVoltage2.setAxisFont(QwtPlot::xBottom, applicationFont);
	qwtPlotVoltage2.setAxisFont(QwtPlot::yLeft, applicationFont);
	qwtPlotVoltage2.setAxisFont(QwtPlot::axisCnt, applicationFont);

	// add a grid
	gridVoltage2.attach(&qwtPlotVoltage2);
	gridVoltage2.setPen( QPen(QColor(gridColor), 0.0, Qt::DotLine) );

	// set background color
	//qwtPlotVoltage2.setCanvasBackground(QColor(Qt::black));

	// on the smal GUI there is no space for a title
	if (useLargeGUI)
	{
		// set title
		qwtPlotVoltage2.setTitle("24 V Battery");
	}

	// Set axis scale (instead of using autoscale, which is default)
	// maximum voltage value at which the axis should and, and the steps between each divider
	qwtPlotVoltage2.setAxisScale(QwtPlot::yLeft, 0.0, MAXIMUMPLOTVOLTAGE2);

	// Set axis scale (instead of using autoscale, which is default)
	// time (60 sec)
	qwtPlotVoltage2.setAxisScale(QwtPlot::xBottom, 0.0, 60.0);

	curveVoltage2.setRenderHint(QwtPlotItem::RenderAntialiased);
	// set curve color
	curveVoltage2.setPen(QPen(labelFillColorBlue));
	// fill the area below the curve
	// curveVoltage2.setBrush(labelFillColorBlue);


	//--------------------------------------
	// plot curve "HEARTBEAT"
	//--------------------------------------
	// get the current application font
	applicationFont = QApplication::font();
	qwtPlotHeartbeat.setAxisFont(QwtPlot::xBottom, applicationFont);
	qwtPlotHeartbeat.setAxisFont(QwtPlot::yLeft, applicationFont);
	qwtPlotHeartbeat.setAxisFont(QwtPlot::axisCnt, applicationFont);

	// add a grid
	gridHeartbeat.attach(&qwtPlotHeartbeat);
	gridHeartbeat.setPen( QPen(QColor(gridColor), 0.0, Qt::DotLine) );

	// set background color
	// qwtPlotHeartbeat.setCanvasBackground(QColor(Qt::black));

	// set title
	qwtPlotHeartbeat.setTitle("Heartbeat");

	// Set axis scale (instead of using autoscale, which is default)
	// maximum voltage value at which the axis should and, and the steps between each divider
	qwtPlotHeartbeat.setAxisScale(QwtPlot::yLeft, 0.0, MAXIMUMPLOTHEARTBEAT);

	// Set axis scale (instead of using autoscale, which is default)
	// time (60 sec)
	qwtPlotHeartbeat.setAxisScale(QwtPlot::xBottom, 0.0, 60.0);

	curveHeartbeat.setRenderHint(QwtPlotItem::RenderAntialiased);
	// set curve color
	curveHeartbeat.setPen(QPen(labelFillColorRed));
	// fill the area below the curve
	// curveHeartbeat.setBrush(labelFillColorBlue);


	//------------------------------------------------------------
	// resize qwt plot items to the correct underlying frame size
	//------------------------------------------------------------
	if (useLargeGUI)
	{
		qwtPlotVoltage1.setGeometry( ui.widgetVoltage1->rect() );
		qwtPlotVoltage2.setGeometry( ui.widgetVoltage2->rect() );

	//	qwtPlotCurrent1.setGeometry( ui.widgetCurrent1and2->rect() );
	//	qwtPlotCurrent2.setGeometry( ui.widgetCurrent1and2->rect() );

		qwtPlotHeartbeat.setGeometry( ui.widgetHeartbeat->rect() );
	}
	else
	{
		qwtPlotVoltage1.setGeometry( uiSmall.widgetVoltage1->rect() );
		qwtPlotVoltage2.setGeometry( uiSmall.widgetVoltage2->rect() );

	//	qwtPlotCurrent1.setGeometry( uiSmall.widgetCurrent1and2->rect() );
	//	qwtPlotCurrent2.setGeometry( uiSmall.widgetCurrent1and2->rect() );

		qwtPlotHeartbeat.setGeometry( uiSmall.widgetHeartbeat->rect() );
	}
}
#endif


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
	if (useLargeGUI)
	{
		ui.lblCompassX->setText( QString("%1").setNum(x, 'f', 1).append(" deg") );
		ui.lblCompassY->setText( QString("%1").setNum(y, 'f', 1).append(" deg") );
		ui.lblCompassZ->setText( QString("%1").setNum(z, 'f', 1).append(" deg") );
		ui.lblCompassHeading->setText( QString("%1").setNum(heading, 'f', 1).append(" deg") );

	// 	ui.qwtCompass->setValue(x);

		// set the 3D OpenGL compass!
		ui.frameCompass->setAllRotations(x, y, z); /// \todo and what to do with the 'heading'?
	}
	else
	{
		uiSmall.lblCompassX->setText( QString("%1").setNum(x, 'f', 1).append(" deg") );
		uiSmall.lblCompassY->setText( QString("%1").setNum(y, 'f', 1).append(" deg") );
		uiSmall.lblCompassZ->setText( QString("%1").setNum(z, 'f', 1).append(" deg") );
		uiSmall.lblCompassHeading->setText( QString("%1").setNum(heading, 'f', 1).append(" deg") );

	// 	uiSmall.qwtCompass->setValue(x);

		// set the 3D OpenGL compass!
		uiSmall.frameCompass->setAllRotations(x, y, z); /// \todo and what to do with the 'heading'?
	}
}


void Gui::setLEDCircuit(unsigned char state)
{
	switch (state)
	{
		case RED:
			if (useLargeGUI)
			{
				ui.lblLEDCircuit->setPixmap(QPixmap(":/images/images/led_red.gif"));
			}
			else
			{
				uiSmall.lblLEDCircuit->setPixmap(QPixmap(":/images/images/led_red.gif"));
			}
			break;
		case GREEN:
			if (useLargeGUI)
			{
				ui.lblLEDCircuit->setPixmap(QPixmap(":/images/images/led_green.gif"));
			}
			else
			{
				uiSmall.lblLEDCircuit->setPixmap(QPixmap(":/images/images/led_green.gif"));
			}
			break;
		case LEDOFF:
			if (useLargeGUI)
			{
				ui.lblLEDCircuit->setPixmap(QPixmap(":/images/images/led_gray.gif"));
			}
			else
			{
				uiSmall.lblLEDCircuit->setPixmap(QPixmap(":/images/images/led_gray.gif"));
			}
			break;
	}
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
				uiSmall.lblLEDHeartbeat->setPixmap(QPixmap(":/images/images/led_red.gif"));
			}
			break;
		case GREEN:
			if (useLargeGUI)
			{
				ui.lblLEDHeartbeat->setPixmap(QPixmap(":/images/images/led_green.gif"));
			}
			else
			{
				uiSmall.lblLEDHeartbeat->setPixmap(QPixmap(":/images/images/led_green.gif"));
			}
			break;
		case LEDOFF:
			if (useLargeGUI)
			{
				ui.lblLEDHeartbeat->setPixmap(QPixmap(":/images/images/led_gray.gif"));
			}
			else
			{
				uiSmall.lblLEDHeartbeat->setPixmap(QPixmap(":/images/images/led_gray.gif"));
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
				uiSmall.lblLEDCompass->setPixmap(QPixmap(":/images/images/led_red.gif"));
			}
			break;
		case GREEN:
			if (useLargeGUI)
			{
				ui.lblLEDCompass->setPixmap(QPixmap(":/images/images/led_green.gif"));
			}
			else
			{
				uiSmall.lblLEDCompass->setPixmap(QPixmap(":/images/images/led_green.gif"));
			}
			break;
		case LEDOFF:
			if (useLargeGUI)
			{
				ui.lblLEDCompass->setPixmap(QPixmap(":/images/images/led_gray.gif"));
			}
			else
			{
				uiSmall.lblLEDCompass->setPixmap(QPixmap(":/images/images/led_gray.gif"));
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
				uiSmall.lblLEDJoystick->setPixmap(QPixmap(":/images/images/led_red.gif"));
			}
			break;
		case GREEN:
			if (useLargeGUI)
			{
				ui.lblLEDJoystick->setPixmap(QPixmap(":/images/images/led_green.gif"));
			}
			else
			{
				uiSmall.lblLEDJoystick->setPixmap(QPixmap(":/images/images/led_green.gif"));
			}
			break;
		case LEDOFF:
			if (useLargeGUI)
			{
				ui.lblLEDJoystick->setPixmap(QPixmap(":/images/images/led_gray.gif"));
			}
			else
			{
				uiSmall.lblLEDJoystick->setPixmap(QPixmap(":/images/images/led_gray.gif"));
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
				uiSmall.lblLEDCamera->setPixmap(QPixmap(":/images/images/led_red.gif"));
			}
			break;
		case GREEN:
			if (useLargeGUI)
			{
				ui.lblLEDCamera->setPixmap(QPixmap(":/images/images/led_green.gif"));
			}
			else
			{
				uiSmall.lblLEDCamera->setPixmap(QPixmap(":/images/images/led_green.gif"));
			}
			break;
		case LEDOFF:
			if (useLargeGUI)
			{
				ui.lblLEDCamera->setPixmap(QPixmap(":/images/images/led_gray.gif"));
			}
			else
			{
				uiSmall.lblLEDCamera->setPixmap(QPixmap(":/images/images/led_gray.gif"));
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
				uiSmall.lblLEDNetwork->setPixmap(QPixmap(":/images/images/led_red.gif"));
			}
			break;
		case GREEN:
			if (useLargeGUI)
			{
				ui.lblLEDNetwork->setPixmap(QPixmap(":/images/images/led_green.gif"));
			}
			else
			{
				uiSmall.lblLEDNetwork->setPixmap(QPixmap(":/images/images/led_green.gif"));
			}
			break;
		case LEDOFF:
			if (useLargeGUI)
			{
				ui.lblLEDNetwork->setPixmap(QPixmap(":/images/images/led_gray.gif"));
			}
			else
			{
				uiSmall.lblLEDNetwork->setPixmap(QPixmap(":/images/images/led_gray.gif"));
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
				uiSmall.lblLEDLaser->setPixmap(QPixmap(":/images/images/led_red.gif"));
			}
			break;
		case GREEN:
			if (useLargeGUI)
			{
				ui.lblLEDLaser->setPixmap(QPixmap(":/images/images/led_green.gif"));
			}
			else
			{
				uiSmall.lblLEDLaser->setPixmap(QPixmap(":/images/images/led_green.gif"));
			}
			break;
		case LEDOFF:
			if (useLargeGUI)
			{
				ui.lblLEDLaser->setPixmap(QPixmap(":/images/images/led_gray.gif"));
			}
			else
			{
				uiSmall.lblLEDLaser->setPixmap(QPixmap(":/images/images/led_gray.gif"));
			}
			break;
	}
}


void Gui::showKinectAngle(double angle)
{
	// we do a type cast here since the original Kinect method uses 'double'.
	if (useLargeGUI)
	{
		ui.spinBoxKinectAngle->setValue((int) angle);
	}
	else
	{
		uiSmall.spinBoxKinectAngle->setValue((int) angle);
	}
}


void Gui::showThreshold(int threshold)
{
	if (useLargeGUI)
	{
		ui.sliderThreshold->setValue(threshold);
	}
	else
	{
		uiSmall.sliderThreshold->setValue(threshold);
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


void Gui::processCam()
{
/*
	if (cameraOpened)
	{
		timer.restart();

		frame = cvQueryFrame( capture );

		if (frame.data)
		{
			// add brightness and contrast
//			this->processFrame(frame);

  if (useLargeGUI)
  {
  }
  else
  {
  }
			ui.frameOpenCV->sendImage( &frame );

			QTimer::singleShot(25, this, SLOT(processCam()));
		}
	}
	return;
*/
}
