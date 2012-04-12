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

#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include <QtGui>
#include <QDialog>

#include "ui_aboutDialog.h"


/**
\author Markus Knapp
\brief Dialog for showing information about the software.
*/
class AboutDialog : public QDialog
{
	Q_OBJECT

	public:
		AboutDialog(QWidget* parent = 0, Qt::WFlags fl = 0);

	public slots:
		/**
		Sets the program version into a text label
		@param text can be free text.
		*/
		void setVersion(QString text);

	protected slots:
		/**
		Event handler for the "About Qt" button.
		*/
		void on_btnAboutQt_clicked(bool checked);

	private:
		Ui::aboutDialog ui;
};

#endif
