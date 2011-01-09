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

	/// set mode of left camera widget to depth draw (1 = depth)
	ui.frameDepth->setMode(1);
}


Gui::~Gui()
{
}


void Gui::closeEvent(QCloseEvent *event)
{
	// no compiler warning "unused"
	Q_UNUSED(event);

	emit shutdown();
}


void Gui::on_actionExit_activated()
{
	close();
}


void Gui::on_actionTest_activated()
{
	emit test();
}


void Gui::on_radioButtonLEDoff_clicked(bool checked)
{
	if (checked)
		emit setLedOff();
}


void Gui::on_radioButtonLEDred_clicked(bool checked)
{
	if (checked)
		emit setRedLed();
}


void Gui::on_radioButtonLEDgreen_clicked(bool checked)
{
	if (checked)
		emit setGreenLed();
}


void Gui::on_radioButtonLEDyellow_clicked(bool checked)
{
	if (checked)
		emit setYellowLed();
}


void Gui::on_radioButtonLEDflashRed_clicked(bool checked)
{
	if (checked)
		emit setRedLedFlash();
}


void Gui::on_radioButtonLEDflashGreen_clicked(bool checked)
{
	if (checked)
		emit setGreenLedFlash();
}


void Gui::on_radioButtonLEDflashYellow_clicked(bool checked)
{
	if (checked)
		emit setYellowLedFlash();
}


void Gui::on_radioButtonVideoRGB_clicked(bool checked)
{
	if (checked)
		emit setVideoMode(0);
}


void Gui::on_radioButtonVideoYUVRGB_clicked(bool checked)
{
	if (checked)
		emit setVideoMode(1);
}


void Gui::on_radioButtonVideoIR8Bit_clicked(bool checked)
{
	if (checked)
		emit setVideoMode(2);
}


void Gui::on_spinBoxAngle_valueChanged(int i)
{
	emit setAngle(i);
}


void Gui::on_pushButtonResetAngle_clicked()
{
	ui.spinBoxAngle->setValue(0);

	emit resetAngle();
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
