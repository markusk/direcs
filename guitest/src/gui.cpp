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
	// define some nice green and red colors
	labelFillColorGreen = QColor(64, 255, 64);
	labelFillColorRed   = QColor(255, 64, 64);
	labelFillColorBlue  = QColor(64, 64, 255);

	//-------------------------------------------------------
	// startup the GUI
	//-------------------------------------------------------
	ui.setupUi(this);

	//--------------------------------
	// do the rest of my init stuff
	//--------------------------------
	init();
}


void Gui::init()
{
}


Gui::~Gui()
{
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



void Gui::appendLog(QString text, bool CR, bool sayIt, bool addTimestamp)
{
	if (addTimestamp)
	{
		// get the current date and time for a timestimp in the log
		now = QDateTime::currentDateTime();

		// prepend timestamp
		text = QString("%1:%2:%3 %4").arg(now.toString("hh")).arg(now.toString("mm")).arg(now.toString("ss")).arg(text);
	}

	// insert the text in the GUI
	ui.textEditLog->insertHtml(text);

	if (CR == TRUE) // default!
		ui.textEditLog->insertHtml("<br>");

	// Ensures that the cursor is visible by scrolling the text edit if necessary.
	ui.textEditLog->ensureCursorVisible();

	Q_UNUSED(sayIt);
}



void Gui::appendAnswerLog(QString text, bool CR, bool sayIt, bool addTimestamp)
{
	if (addTimestamp)
	{
		// get the current date and time for a timestimp in the log
		now = QDateTime::currentDateTime();

		// prepend timestamp
		text = QString("%1:%2:%3 %4").arg(now.toString("hh")).arg(now.toString("mm")).arg(now.toString("ss")).arg(text);
	}

	// insert the text in the GUI
	ui.textEditAnswerLog->insertHtml(text);

	if (CR == TRUE) // default!
		ui.textEditAnswerLog->insertHtml("<br>");

	// Ensures that the cursor is visible by scrolling the text edit if necessary.
	ui.textEditAnswerLog->ensureCursorVisible();

	Q_UNUSED(sayIt);
}


void Gui::on_actionSimulate_activated()
{
	if (ui.actionSimulate->isChecked())
	{
		ui.actionSimulate->setIcon(QIcon(QPixmap(":/images/images/utilities-system-monitor-active.png")));
		ui.actionSimulate->setStatusTip("Disable simulation");
		ui.actionSimulate->setToolTip("Disable simulation");
		emit simulate(true);
	}
	else
	{
		ui.actionSimulate->setIcon(QIcon(QPixmap(":/images/images/utilities-system-monitor.png")));
		ui.actionSimulate->setStatusTip("Simulate");
		ui.actionSimulate->setToolTip("Simulate");
		emit simulate(false);
	}
}


void Gui::on_actionTest_activated()
{
	appendLog("Test!");
	emit test();
}


void Gui::setLEDGreen(bool state)
{
	if (state)
	{
		ui.lblLEDGreen->setPixmap(QPixmap(":/images/images/led_green.gif"));
	}
	else
	{
		ui.lblLEDGreen->setPixmap(QPixmap(":/images/images/led_gray.gif"));
		return;
	}
}


void Gui::setLEDRed(bool state)
{
	if (state)
	{
		ui.lblLEDRed->setPixmap(QPixmap(":/images/images/led_red.gif"));
	}
	else
	{
		ui.lblLEDRed->setPixmap(QPixmap(":/images/images/led_gray.gif"));
		return;
	}
}


void Gui::setFlashlight(bool state)
{
	if (state)
	{
		ui.lblRelais->setPixmap(QPixmap(":/images/images/relais_on.png"));
		ui.lblFlashlight->setPixmap(QPixmap(":/images/images/strobo_on.png"));
	}
	else
	{
		ui.lblRelais->setPixmap(QPixmap(":/images/images/relais_off.png"));
		ui.lblFlashlight->setPixmap(QPixmap(":/images/images/strobo_off.png"));
		return;
	}
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
