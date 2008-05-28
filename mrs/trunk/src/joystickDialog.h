/*************************************************************************
 *   Copyright (C) 2008 by Markus Knapp                                  *
 *   mrs @ direcs.de                                                     *
 *                                                                       *
 *   This file is part of mrs.                                           *
 *                                                                       *
 *   mrs is free software: you can redistribute it and/or modify it      *
 *   under the terms of the GNU General Public License as published      *
 *   by the Free Software Foundation, version 3 of the License.          *
 *                                                                       *
 *   mrs is distributed in the hope that it will be useful,              *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of      *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the        *
 *   GNU General Public License for more details.                        *
 *                                                                       *
 *   You should have received a copy of the GNU General Public License   *
 *   along with mrs. If not, see <http://www.gnu.org/licenses/>.         *
 *                                                                       *
 *************************************************************************/

#ifndef JOYSTICKDIALOG_H
#define JOYSTICKDIALOG_H

#include <QtGui>
#include <QDialog>

#include "ui_joystickDialog.h"


/**
Dialog for showing information about the joystick movement and the pressed buttons.
*/
class JoystickDialog : public QDialog
{
	Q_OBJECT
	
	public:
		JoystickDialog(QWidget* parent = 0/*, Qt::WFlags fl = 0*/);


	protected slots:
		/**
		Event handler for the "About Qt" button.
		*/
		//void on_btnAboutQt_clicked(bool checked);
		
	private:
		Ui::joystickDialog ui;
};

#endif
