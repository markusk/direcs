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

#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include <QtGui>
#include <QDialog>

#include "ui_aboutDialog.h"


/**
Dialog for showing information about mrs.
*/
class AboutDialog : public QDialog
{
	Q_OBJECT
	
	public:
		AboutDialog(QWidget* parent = 0, Qt::WFlags fl = 0);


	protected slots:
		/**
		Event handler for the "About Qt" button.
		*/
		void on_btnAboutQt_clicked(bool checked);
		
	private:
		Ui::aboutDialog ui;
};

#endif
