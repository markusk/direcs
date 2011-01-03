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


Gui::Gui(QMainWindow *parent) : QMainWindow(parent)
{
	// startup the GUI
	ui.setupUi(this);

}


Gui::~Gui()
{
}


void Gui::closeEvent(QCloseEvent *event)
{
	// no compiler warning "unused"
	Q_UNUSED(event);

	// emit shutdown();
}


void Gui::on_actionExit_activated()
{
	close();
}


void Gui::on_actionResetDrivenDistance_activated()
{
}


void Gui::appendLog(QString text, bool CR)
{
	// get the current date and time for a timestimp in the log
	now = QDateTime::currentDateTime();

	// prepend timestamp
	text = QString("%1:%2:%3 %4").arg(now.toString("hh")).arg(now.toString("mm")).arg(now.toString("ss")).arg(text);

	// insert the new text in the GUI
	ui.textEdit->insertHtml(text);

	if (CR == true) // default!
		ui.textEdit->insertHtml("<br>");

	// Ensures that the cursor is visible by scrolling the text edit if necessary.
	ui.textEdit->ensureCursorVisible();
}
