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

#ifndef GUI_H
#define GUI_H

#include <QtGlobal> // for Q_OS_* Makro!
#include <QtGui>
#include <QtOpenGL>
#include <QGraphicsScene>

#include "RGBWindow.h"


#include "ui_kinect.h"


class Gui : public QMainWindow
{
	Q_OBJECT


	public:
		Gui(QMainWindow *parent = 0);
		~Gui();


	public slots:
		/**
		Appends text to the main log in the main window.
		@param text is the text to be displayed.
		@param CR adds a carriage return (CR) to the text, if true (default). This parameter is optional.
		 */
		void appendLog(QString text, bool CR=true);


	signals:
		void shutdown();
		void test();
		void setLedOff();
		void setRedLed();
		void setGreenLed();
		void setYellowLed();
		void setRedLedFlash();
		void setGreenLedFlash();
		void setYellowLedFlash();


	protected:
		void closeEvent(QCloseEvent *event);


	protected slots:
		void on_actionExit_activated();
		void on_actionTest_activated();
		void on_radioButtonLEDoff_clicked(bool checked);
		void on_radioButtonLEDred_clicked(bool checked);
		void on_radioButtonLEDgreen_clicked(bool checked);
		void on_radioButtonLEDyellow_clicked(bool checked);
		void on_radioButtonLEDflashRed_clicked(bool checked);
		void on_radioButtonLEDflashGreen_clicked(bool checked);
		void on_radioButtonLEDflashYellow_clicked(bool checked);


	private:
		Ui::mainWindow ui;

		QDateTime now; /// this is for the timestamp in the logs in the gui
};
#endif
