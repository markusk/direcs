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

#include "settingsDialog.h"

SettingsDialog::SettingsDialog(QWidget* parent) : QDialog(parent)
{
	ui.setupUi(this);


	// change the value of a spinBox when the value of the corresponding slider changes
	connect(ui.sliderMotor1Speed, SIGNAL(valueChanged(int)), ui.spinBoxMotor1Speed, SLOT(setValue(int)));
	connect(ui.sliderMotor2Speed, SIGNAL(valueChanged(int)), ui.spinBoxMotor2Speed, SLOT(setValue(int)));
	connect(ui.sliderMotor3Speed, SIGNAL(valueChanged(int)), ui.spinBoxMotor3Speed, SLOT(setValue(int)));
	connect(ui.sliderMotor4Speed, SIGNAL(valueChanged(int)), ui.spinBoxMotor4Speed, SLOT(setValue(int)));
	// and vice versa
	connect(ui.spinBoxMotor1Speed, SIGNAL(valueChanged(int)), ui.sliderMotor1Speed, SLOT(setValue(int)));
	connect(ui.spinBoxMotor2Speed, SIGNAL(valueChanged(int)), ui.sliderMotor2Speed, SLOT(setValue(int)));
	connect(ui.spinBoxMotor3Speed, SIGNAL(valueChanged(int)), ui.sliderMotor3Speed, SLOT(setValue(int)));
	connect(ui.spinBoxMotor4Speed, SIGNAL(valueChanged(int)), ui.sliderMotor4Speed, SLOT(setValue(int)));

	// change the value of a spinBox when the value of the corresponding slider changes
	connect(ui.sliderMaximumSpeed, SIGNAL(valueChanged(int)), ui.spinBoxMaximumSpeed, SLOT(setValue(int)));
	connect(ui.sliderMinimumSpeed, SIGNAL(valueChanged(int)), ui.spinBoxMinimumSpeed, SLOT(setValue(int)));
	connect(ui.sliderAllMotorsSpeed, SIGNAL(valueChanged(int)), ui.spinBoxAllMotorsSpeed, SLOT(setValue(int)));
	// and vice versa
	connect(ui.spinBoxMaximumSpeed, SIGNAL(valueChanged(int)), ui.sliderMaximumSpeed, SLOT(setValue(int)));
	connect(ui.spinBoxMinimumSpeed, SIGNAL(valueChanged(int)), ui.sliderMinimumSpeed, SLOT(setValue(int)));
	connect(ui.spinBoxAllMotorsSpeed, SIGNAL(valueChanged(int)), ui.sliderAllMotorsSpeed, SLOT(setValue(int)));

	// change the value of a spinBox when the value of the corresponding slider changes
	connect(ui.sliderRobotSlot, SIGNAL(valueChanged(int)), ui.spinBoxRobotSlot, SLOT(setValue(int)));
	// and vice versa
	connect(ui.spinBoxRobotSlot, SIGNAL(valueChanged(int)), ui.sliderRobotSlot, SLOT(setValue(int)));

	// change the value of a spinBox when the value of the corresponding slider changes
	connect(ui.sliderRobotSlotWidth, SIGNAL(valueChanged(int)), ui.spinBoxRobotSlotWidth, SLOT(setValue(int)));
	// and vice versa
	connect(ui.spinBoxRobotSlotWidth, SIGNAL(valueChanged(int)), ui.sliderRobotSlotWidth, SLOT(setValue(int)));

	// change the value of a spinBox when the value of the corresponding slider changes
	connect(ui.sliderStraightForwardDeviation, SIGNAL(valueChanged(int)), ui.spinBoxStraightForwardDeviation, SLOT(setValue(int)));
	// and vice versa
	connect(ui.spinBoxStraightForwardDeviation, SIGNAL(valueChanged(int)), ui.sliderStraightForwardDeviation, SLOT(setValue(int)));

	// change the value of a spinBox when the value of the corresponding slider changes
	connect(ui.sliderObstacleLaserScanner, SIGNAL(valueChanged(int)), ui.spinBoxObstacleLaserScanner, SLOT(setValue(int)));
	// and vice versa
	connect(ui.spinBoxObstacleLaserScanner, SIGNAL(valueChanged(int)), ui.sliderObstacleLaserScanner, SLOT(setValue(int)));

	// change the value of a spinBox when the value of the corresponding slider changes
	connect(ui.sliderObstacle, SIGNAL(valueChanged(int)), ui.spinBoxObstacle, SLOT(setValue(int)));
	// and vice versa
	connect(ui.spinBoxObstacle, SIGNAL(valueChanged(int)), ui.sliderObstacle, SLOT(setValue(int)));

	//----------------------------------
	// and now the important stuff...
	//----------------------------------
	// emit the signal from the widget to obstacleCheckThread via Direcs!
	connect(ui.sliderRobotSlot, SIGNAL(valueChanged(int)), SIGNAL(setRobotSlot(int)));
	connect(ui.sliderRobotSlotWidth, SIGNAL(valueChanged(int)), SIGNAL(setRobotSlotWidth(int)));

	// emit the signal from Gui to obstacleCheckThread via Direcs!
	connect(ui.sliderStraightForwardDeviation, SIGNAL(valueChanged(int)), SIGNAL(setStraightForwardDeviation(int)));

	// emit the signal from Gui to obstacleCheckThread via Direcs!
	connect(ui.sliderObstacleLaserScanner, SIGNAL(valueChanged(int)), SIGNAL(setMinObstacleDistanceLaserFront(int)));
	/// @todo implement second signal for second laser!

	// emit the signal from Gui to obstacleCheckThread via Direcs!
	connect(ui.sliderObstacle, SIGNAL(valueChanged(int)), SIGNAL(setMinObstacleDistance(int)));
}


int SettingsDialog::getSliderMotorSpeed(int motor)
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


int SettingsDialog::getSliderMinimumSpeed(void)
{
	return ui.sliderMinimumSpeed->value();
}


int SettingsDialog::getSliderMaximumSpeed(void)
{
	return ui.sliderMaximumSpeed->value();
}


int SettingsDialog::getSliderObstacleValue()
{
	return ui.sliderObstacle->value();
}


int SettingsDialog::getSliderObstacleLaserScannerValue()
{
	return ui.sliderObstacleLaserScanner->value();
}


int SettingsDialog::getSliderRobotSlotValue()
{
	return ui.sliderRobotSlot->value();
}


int SettingsDialog::getSliderRobotSlotWidth()
{
	return ui.sliderRobotSlotWidth->value();
}


int SettingsDialog::getSliderStraightForwardDeviationValue()
{
	return ui.sliderStraightForwardDeviation->value();
}


Qt::CheckState SettingsDialog::getCheckBoxSaveSettings()
{
	return ui.checkBoxSaveSettings->checkState();
}


void SettingsDialog::setSliderMotorSpeed(int motor, int value)
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


void SettingsDialog::on_sliderMotor1Speed_sliderReleased()
{
	// no auto connect in constructor, because this slot has no "value" parameter!
	emit setMotorSpeed(MOTOR1, ui.sliderMotor1Speed->value());

	//int value = ui.sliderMotor1Speed->value();

	//laserDistanceLineListFront->at(0)->setStartAngle(ui.sliderMotor1Speed->value() * 16);
	//laserDistanceLineListFront->at(0)->setPos(value, value);
	//appendLog(QString("x,y=%1").arg(value));
}


void SettingsDialog::on_sliderMotor2Speed_sliderReleased()
{
	// no auto connect in constructor, because this slot has no "value" parameter!
	emit setMotorSpeed(MOTOR2, ui.sliderMotor2Speed->value());

	//laserDistanceLineListFront->at(0)->setSpanAngle(ui.sliderMotor2Speed->value() * 16);
	//appendLog(QString("spanAngle=%1").arg(ui.sliderMotor2Speed->value()));
}


void SettingsDialog::on_sliderMotor3Speed_sliderReleased()
{
	// no auto connect in constructor, because this slot has no "value" parameter!
	emit setMotorSpeed(MOTOR3, ui.sliderMotor3Speed->value());

	//laserDistanceLineListFront->at(0)->setSpanAngle(ui.sliderMotor3Speed->value() * 16);
	//appendLog(QString("spanAngle=%1").arg(ui.sliderMotor3Speed->value()));
}


void SettingsDialog::on_sliderMotor4Speed_sliderReleased()
{
	// no auto connect in constructor, because this slot has no "value" parameter!
	emit setMotorSpeed(MOTOR4, ui.sliderMotor4Speed->value());

	//laserDistanceLineListFront->at(0)->setSpanAngle(ui.sliderMotor4Speed->value() * 16);
	//appendLog(QString("spanAngle=%1").arg(ui.sliderMotor4Speed->value()));
}


void SettingsDialog::on_sliderAllMotorsSpeed_sliderReleased()
{
	/*
	int speed = ui.sliderAllMotorsSpeed->value();


	// set same speed to 4 other motor speed sliders
	ui.sliderMotor1Speed->setValue(speed);
	ui.sliderMotor2Speed->setValue(speed);
	ui.sliderMotor3Speed->setValue(speed);
	ui.sliderMotor4Speed->setValue(speed);
	*/

	// no auto connect in constructor, because this slot has no "value" parameter!
	emit setMotorSpeed(ALLMOTORS, ui.sliderAllMotorsSpeed->value());

	//laserDistanceLineListFront->at(0)->setSpanAngle(ui.sliderMotor4Speed->value() * 16);
	//appendLog(QString("spanAngle=%1").arg(ui.sliderMotor4Speed->value()));
}


void SettingsDialog::on_sliderMaximumSpeed_sliderReleased()
{
	// no auto connect in constructor, because this slot has no "value" parameter!
	emit setMaximumSpeed(ui.sliderMaximumSpeed->value());
}


void SettingsDialog::setSliderMinimumSpeed(int speed)
{
	ui.sliderMinimumSpeed->setValue(speed);
}


void SettingsDialog::setSliderMaximumSpeed(int speed)
{
	ui.sliderMaximumSpeed->setValue(speed);
}


void SettingsDialog::setSliderObstacleValue(int value)
{
	ui.sliderObstacle->setValue(value);
}


void SettingsDialog::setSliderObstacleLaserScannerValue(int value)
{
	ui.sliderObstacleLaserScanner->setValue(value);
}


void SettingsDialog::setSliderRobotSlot(int angle)
{
	ui.sliderRobotSlot->setValue(angle);
}


void SettingsDialog::setSliderRobotSlotWidth(int width)
{
	ui.sliderRobotSlotWidth->setValue(width);
}


void SettingsDialog::setSliderStraightForwardDeviation(int angle)
{
	ui.sliderStraightForwardDeviation->setValue(angle);
}


void SettingsDialog::setCheckBoxSaveSettings(Qt::CheckState state)
{
	ui.checkBoxSaveSettings->setCheckState(state);
}
