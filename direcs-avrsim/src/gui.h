/*************************************************************************
 *   Copyright (C) 2009 by Markus Knapp                                  *
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


//-------------------------------------------------------------------
#include <QtGui>
//-------------------------------------------------------------------
#include "ui_mainWindow.h"
//-------------------------------------------------------------------



/**
\brief The class for all GUI actions
*/
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
		@param CR adds a carriage return (CR) to the text, if true (default). This parameter is optional!
		@param sayIt If true, the text is also spoken (default=false). This parameter is optional!
		 */
		void appendLog(QString text, bool CR=true, bool sayIt=false);

		/**
		Turns the red LED on or off (coloured or gray pixmap).
		@param state can be ON or OFF.
		*/
		void setLEDRed(bool state);

		/**
		Turns the green LED on or off (coloured or gray pixmap).
		@param state can be ON or OFF.
		*/
		void setLEDGreen(bool state);

		/**
		Turns the GUI flashlight on or off
		@param state can be ON or OFF.
		*/
		void setFlashlight(bool state);


	protected:
		void closeEvent(QCloseEvent *event);


	signals:
		/**
		Exits the whole programm.
		 */
		void shutdown();

		/**
		Enables or disables the robots simulation mode.
		This signal is sent from the simulation button and received from all threads which deliver real signals from the bot.
		Once the signal is received, the slots switches to simulation mode.
		@param state can be true or false.
		@sa Direcs::setSimulationMode()
		@sa SensorThread::setSimulationMode()
		@sa LaserThread::setSimulationMode()
		@sa ObstackeCheckThread::setSimulationMode()
		*/
		void simulate(bool state);

		/**
		Emits a signal to call the test method in the Direcs class..
		 */
		void test();


	protected slots:
		/**
		Action if a menu item or a toolbar button is clicked
		*/
		void on_actionExit_activated();
		void on_actionLog_activated();
		void on_actionState_activated();
		void on_actionSimulate_activated();


	private:
		void init();

		/**
		Removes HTML code from a given string.
		@param text is the string to be cleaned
		@return the cleaned string
		*/
		QString removeHtml(QString text);

		Ui::mainWindow ui;
		QColor labelFillColorRed;
		QColor labelFillColorGreen;
		QColor labelFillColorBlue;

		static const bool ON  = true;   /** For motor "ON" */
		static const bool OFF = false;  /** For motor "OFF" */

		/// @sa Direcs::drive() [Slot]
		static const unsigned char START = 7; //! For button signal "start driving"
		static const unsigned char STOP  = 8; //! For button signal "stop driving"
		static const unsigned char MOTOR1FW		= 10; /// for the test widget
		static const unsigned char MOTOR1BW		= 11; /// for the test widget
		static const unsigned char MOTOR1OFF	= 12; /// for the test widget
		static const unsigned char MOTOR2FW		= 13; /// for the test widget
		static const unsigned char MOTOR2BW		= 14; /// for the test widget
		static const unsigned char MOTOR2OFF	= 15; /// for the test widget
		static const unsigned char MOTOR3FW		= 16; /// for the test widget
		static const unsigned char MOTOR3BW		= 17; /// for the test widget
		static const unsigned char MOTOR3OFF	= 18; /// for the test widget
		static const unsigned char MOTOR4FW		= 19; /// for the test widget
		static const unsigned char MOTOR4BW		= 20; /// for the test widget
		static const unsigned char MOTOR4OFF	= 21; /// for the test widget

		static const unsigned char CLOCKWISE        = 0;  /// Motor direction "CLOCKWISE"
		static const unsigned char COUNTERCLOCKWISE = 1;  /// Motor direction "COUNTERCLOCKWISE
		static const unsigned char SAME             = 3;  /// Motor direction/power "same like before"
		static const unsigned char MOTOR1           = 10; /// Motor 1 front left
		static const unsigned char MOTOR2           = 20; /// Motor 2 front right
		static const unsigned char MOTOR3           = 30; /// Motor 3 back left
		static const unsigned char MOTOR4           = 40; /// Motor 4 back right

		/**
		Give the sensors some names

		DONT CHANGE THIS NUMBERS!
		THEY ARE ALSO USED TO ADRESS THE ARRAY "iRSensorValue[]" !!
		*/
		static const short int SENSOR1 = 1;
		static const short int SENSOR2 = 2;
		static const short int SENSOR3 = 4;
		static const short int SENSOR4 = 8;
		static const short int SENSOR5 = 16;
		static const short int SENSOR6 = 32;
		static const short int SENSOR7 = 64;
		static const short int SENSOR8 = 128;

		/** ultrasonic sensor */
		static const short int SENSOR16 = 256;
		/** Value if no sensor has a value to react */
		static const short int NONE = 0;

		static const short int MOTORSENSOR1 = 0;
		static const short int MOTORSENSOR2 = 1;

		static const short int VOLTAGESENSOR1 = 0;
		static const short int VOLTAGESENSOR2 = 1;

		static const float MINIMUMVOLTAGE1 = 10.80; // TODO: put min. voltage 1 in the ini-file
		static const float MINIMUMVOLTAGE2 = 21.60; // TODO: put min. voltage 2 in the ini-file

		static const float MAXIMUMVOLTAGE1 = 12.00; /// for progress bars TODO: put max. voltage 1 in the ini-file
		static const float MAXIMUMVOLTAGE2 = 24.00; /// for progress bars TODO: put max. voltage 2 in the ini-file

		/**
		Factor for fitting 6 meters (measured from the laser scanner) into a frame with a height of 270 pixels at a default zoom level of 5 !!
		Example:

		laservalue = 0.18 cm
		fit factor = 45
		zoom level = 5

		0.18 x 45 x 5 = 40.5 Pixel -> 41 Pixel is the length of the laser line in the GUI
		*/
		static const int FITTOFRAMEFACTOR = 45;
		static const int STARTZOOMLEVEL = 3;

		//! the initial Y position of the laser lines and the robot image in the GUI
		//! This is the difference in pixel measured drom the absolute center within the robot pixmap!
		static const int INITIALLASERYPOSFRONT = 60.0; // FIXME: when values are different, the distance line do not fit on the lsaer lines in on_sliderZoom_valueChanged
		static const int INITIALLASERYPOSREAR  = 60.0; // FIXME: when values are different, the distance line do not fit on the lsaer lines in on_sliderZoom_valueChanged

		//! Number of distance semi circles
		static const int LASERDISTANCECIRCLES = 27;
		//! the size (diameter) of the first (innerst) circle in pixels
		static const int LASERDISTANCEFIRSTCIRCLE = 82;
		//! Distances between the lines to show the laser distances in pixels
		static const int LASERDISTANCEDISTANCE = 60;

		// the tags for the laser lines
		static const int FREEWAY = 0;
		static const int OBSTACLE = 1;
		static const int LARGESTFREEWAY = 2;
		static const int CENTEROFLARGESTFREEWAY = 3;

		// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		/**
		Defines the size of the laserLines[] array
		This es equal to the number of degrees.
		See also laserThread.h */
		//static const unsigned char LASERSCANNERARRAYSIZE = 181;

		/**
		For the laserThread
		*/
		static const short int LASER1 = 1;
		static const short int LASER2 = 2;
		static const short int LASER3 = 4;
		static const short int LASER4 = 8;
		static const short int LASER5 = 16;
};
#endif
