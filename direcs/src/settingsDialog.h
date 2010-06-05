/*************************************************************************
 *   Copyright (C) 2010 by Markus Knapp                                  *
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

#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QtGui>
#include <QDialog>

#include "ui_settingsDialog.h"

/**
\brief The dialog for all the programm settings.

Dialog for showing the current settings or to let them change / interact with the main window.
*/
class SettingsDialog : public QDialog
{
	Q_OBJECT
	
	public:
		SettingsDialog(QWidget* parent = 0);
		
		/**
		@return The slider value of a motor speed.
		@param motor is the motor number.
		*/
		int getSliderMotorSpeed(int motor);
		
		/**
		@return The slider value of the minimum speed.
		*/
		int getSliderMinimumSpeed(void);
		
		/**
		@return The slider value of the maximum speed.
		*/
		int getSliderMaximumSpeed(void);
		
		/**
		@return The slider value of an obstacle distance for the infrared and ultrasonic sensors (when a alarm should be released).
		*/
		int getSliderObstacleValue();
		
		/**
		@return The slider value of an obstacle distance for the laser scanner (when a alarm should be released).
		*/
		int getSliderObstacleLaserScannerValue();
		
		/**
		@return The angle in degrees to which the robot has to fit between.
		*/
		int getSliderRobotSlotValue();

		/**
		@return The width of a possible robot slot in centimeters (cm) where to drive through.
		*/
		int getSliderRobotSlotWidth();

		/**
		@return The slider value of the deviation to drive forward (deviation to 90 degrees).
		*/
		int getSliderStraightForwardDeviationValue();
		
		/**
		@return The state of a Qt checkBox.
		*/
		Qt::CheckState getCheckBoxSaveSettings();
		//Qt::CheckState getCheckBoxAutoSave();
		
		/**
		Sets the slider MotorSpeed.
		@param motor is the motor number.
		@param value is the speed of the motor.
		*/
		void setSliderMotorSpeed(int motor, int value);
		
		/**
		Sets the slider minimum speed.
		@param speed is the minimum speed of the motors.
		*/
		void setSliderMinimumSpeed(int speed);
		
		/**
		Sets the slider maximum speed.
		@param speed is the maximum speed of the motors.
		*/
		void setSliderMaximumSpeed(int speed);
		
		/**
		Sets the slider value of an obstacle distance for the infrared and ultrasonic sensors (when a alarm should be released).
		@param value is distance in cm.
		*/
		void setSliderObstacleValue(int value);
		
		/**
		Sets the slider value of an obstacle distance for the laser scanner (when a alarm should be released).
		@param value is distance in cm.
		*/
		void setSliderObstacleLaserScannerValue(int value);
		
		/**
		Sets the slider value of the robot slot, where it has to fit through.
		@param angle is the minimum angle in degrees.
		*/
		void setSliderRobotSlot(int angle);

		/**
		Sets the slider value of the robot slot, where it has to fit through.
		@param width is the minimum width in centimeters (cm).
		*/
		void setSliderRobotSlotWidth(int width);

		/**
		Sets the slider value of the deviation to drive forward (deviation to 90 degrees).
		@param angle is the maximum angle in degrees.
		*/
		void setSliderStraightForwardDeviation(int angle);
		
		/**
		Checks or unckecks the SaveSettings checkbox, depending on the value read from the ini-file.
		*/
		void setCheckBoxSaveSettings(Qt::CheckState state);


	signals:
		/**
		Emits the speed of a motor.
		@param motor is the motor number.
		@param speed is the speed (0 - 255).
		*/
		void setMotorSpeed(int motor, int speed);
		/**
		Emits the new maximum speed of the robot(s motors).
		@param speed is the speed (0 - 255).
		*/
		void setMaximumSpeed(int speed);
		
		/**
		This signal emits the angle, where the robots has to fit through.
		@sa ObstacleCheckThread
		*/
		void setRobotSlot(int angle);

		/**
		This signal emits the width in centimeters (cm), where the robots has to fit through.
		@sa ObstacleCheckThread
		*/
		void setRobotSlotWidth(int width);

		/**
		This signal emit the deviation to 90 degrees when the robot drives forward.
		@param deviation is the angle in degrees.
		@sa ObstacleCheckThread
		*/
		void setStraightForwardDeviation(int deviation);
		
		/**
		This signal emits set the minimum distance, which the robot needs. Used by the infrared and ultra sonic sensors.
		@param distance in centimeters
		@sa ObstacleCheckThread
		*/
		void setMinObstacleDistance(int distance);
		
		/**
		This signal emits the minimum distance, which the robot needs. Used by the Laser scanner.
		@param distance in centimeters
		@sa ObstacleCheckThread
		 */
		void setMinObstacleDistanceLaser(int distance);


	private slots:
		void on_sliderMotor1Speed_sliderReleased();
		void on_sliderMotor2Speed_sliderReleased();
		void on_sliderMotor3Speed_sliderReleased();
		void on_sliderMotor4Speed_sliderReleased();
		void on_sliderAllMotorsSpeed_sliderReleased();
		void on_sliderMaximumSpeed_sliderReleased();


	protected slots:
		/**
		Event handler for the "About Qt" button.
		*/
		//void on_btnAboutQt_clicked(bool checked);
		
		
	private:
		Ui::settingsDialog ui;
		
		static const unsigned char MOTOR1           = 10; //! Motor 1 (front left)
		static const unsigned char MOTOR2           = 20; //! Motor 2 (front right)
		static const unsigned char MOTOR3			= 30; //! Motor 3 (back left)
		static const unsigned char MOTOR4			= 40; //! Motor 4 (back right)
		static const unsigned char ALLMOTORS		= 90; //! used the hole robot gets one command for all motors (like 'forwardall'). This is so save transmissions on the serial line.
};

#endif
