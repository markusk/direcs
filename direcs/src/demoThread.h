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

#ifndef DEMOTHREAD_H
#define DEMOTHREAD_H

#include <QThread>
#include <QTime>

#ifdef Q_OS_LINUX
#include <time.h> // for rand
#include <stdlib.h> // for rand
#endif


/**
\author Markus Knapp
\brief Some test code for presenting the robot itself.

tbd
*/
class DemoThread : public QThread
{
	Q_OBJECT

	public:
		DemoThread();
		~DemoThread();

		/**
		Stops the thread.
		*/
		void stop();

		/**
		Starts the thread.
		*/
		virtual void run();

		/**
		tbd
		@return true on success, false on error
		*/
		bool init();


	signals:
		/**
		Emits a string to the GUI log / console.
		@sa Gui::appendLog()
		@param text is the message to be emitted
		@param CR is true when a CR/LF should be added at the end of the line (default)
		@param sayIt is true when the text for the log also should be spoken (default=false)
		@param addTimestamp If true, the a timestamp is added in front of the text. This parameter is optional.
		*/
		void message(QString text, bool CR=true, bool sayIt=false, bool addTimestamp=true);

		/**
		Sends a signal fro setting the robots RGB brightness to do some nive blinky flashy stuff.
		*/
		bool setRGBLEDBrightness(unsigned char rgbLed, unsigned char brightness);


	private:
		volatile bool stopped;
		bool demoMode;

		static const int timeToNetworkCheck = 3; // time in seconds

		// Time in milliseconds every ms the run method is called
		static const unsigned long THREADSLEEPTIME = 200; // 250 ms

		/// the RGB LED numbers @sa rgbLed::rgbLed(), direcs.h, demoThread
		static const unsigned char RGBLED1 = 0; // RGB 1 R
		static const unsigned char RGBLED2 = 1; // RGB 1 G
		static const unsigned char RGBLED3 = 2; // RGB 1 B

		static const unsigned char RGBLED4 = 3; // RGB 2 R
		static const unsigned char RGBLED5 = 4; // RGB 2 G
		static const unsigned char RGBLED6 = 5; // RGB 2 B

		static const unsigned char RGBLED7 = 6; // RGB 3 R
		static const unsigned char RGBLED8 = 7; // RGB 3 G
		static const unsigned char RGBLED9 = 8; // RGB 3 B

		static const unsigned char RGBLED10 = 9; // RGB 4 R
		static const unsigned char RGBLED11 = 10; // RGB 4 G
		static const unsigned char RGBLED12 = 11; // RGB 4 B

		static const unsigned char RGBLED13 = 12; // RGB 5 R
		static const unsigned char RGBLED14 = 13; // RGB 5 G
		static const unsigned char RGBLED15 = 14; // RGB 5 B

		static const unsigned char RGBLED16 = 15; // RGB 6 R
		static const unsigned char RGBLED17 = 16; // RGB 6 G
		static const unsigned char RGBLED18 = 17; // RGB 6 B
};

#endif
