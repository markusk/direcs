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

#ifndef JOYSTICKDIALOG_H
#define JOYSTICKDIALOG_H

#include <QtGui>
#include <QDialog>

#include "ui_joystickDialog.h"


/**
\author Markus Knapp
\brief Dialog for showing the joystick axes and buttons.

Dialog for showing information about the joystick movement and the pressed buttons.
*/
class JoystickDialog : public QDialog
{
	Q_OBJECT

	public:
		JoystickDialog(QWidget* parent = 0);


	public slots:
		/**
		Shows the "standard" joystick button states when pressed.
		*/
		void showJoystickButtons(int buttonNumber, bool buttonState);

		/**
		Shows the POV / Hat Switch joystick buttons states when pressed.
		*/
		void showJoystickPOVButtons(int buttonsState);

		/**
		Shows the joystick values when it moves.
		*/
		void showJoystickAxes(int axisNumber, int axisValue);


	protected slots:
		/**
		Event handler for the "About Qt" button.
		*/
		//void on_btnAboutQt_clicked(bool checked);


	private:
		Ui::joystickDialog ui;

		/// \todo put axis numbers to ini-file
		/// Joystick axis numbers
		/// @sa Joystick()
		static const int JOYSTICKAXISY2 = 2; // ok
		static const int JOYSTICKAXISX3 = 3; // ok
		static const int JOYSTICKAXISX4 = 4;
		static const int JOYSTICKAXISY5 = 5;
};

#endif
