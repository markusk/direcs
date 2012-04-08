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

#ifndef RGBLED_H
#define RGBLED_H

//-------------------------------------------------------------------
#include "interfaceAvr.h"
//-------------------------------------------------------------------
#include <QThread>
#include <QMutex>
//-------------------------------------------------------------------


/**
\author Markus Knapp
\brief This class controls connected rgbLeds with PWM (brightness)

It has to be a thread because of communicating at the same time with the microcontroller via the class. @sa InterfaceAvr() !
*/
class RgbLed : public QObject
{
	Q_OBJECT

	public:
		RgbLed(InterfaceAvr *i, QMutex *m);
		~RgbLed();

		/**
		Stores the RGB LEDs minimum and maximum brightness and its default brightness.
		@param rgbLed is the RGB LED number.
		@param type can be RGBLEDMIN, RGBLEDMAX, RGBLEDSTART, RGBLEDDEFAULT or RGBLEDACTUAL
		@param bness is the brightness (0 - 255).
		 */
		void storeBrightness(int rgbLed, unsigned char type, unsigned char bness);

		/**
		Returns the rgbLed start, end, default or current position.
		@param rgbLed is the rgbLed number.
		@param type can be SVMIN, SVMAX, SVSTART, SVEND, SVDEFAULT or SVCURRENT=default
		@return the rgbLed position (0 - 255)
		 */
		unsigned char getRgbLedBrightness(int rgbLed, unsigned char type=RGBLEDACTUAL);


	public slots:
		/**
		Sets the brightness of a RGB LED.
		@param rgbLed is the RGBLED number.
		@param bness is the brightness.
		@return true on success
		 */
		bool setBrightness(unsigned char rgbLed, unsigned char bness);

		/**
		Inits all RGB LEDs to their default brightness.
		 */
		void init(void);

		/**
		This slots takes the robot (circuit) state, to know if the robot is ON or OFF.
		When the class knows this, unnecessary communication with the interface can be avoided.
		@param state can be ON or OFF
		 */
		void setRobotState(bool state);


	signals:
		/**
		Emits a message to be displayed in the GUI or be used somewhere else (e.g. error message).
		*/
		void message(QString text);


	private:
		QString className;	/// this will contain the name of this class at runtime. @sa takeCommandAnswer()
		mutable QMutex *mutex; // make this class thread-safe
		InterfaceAvr *interface1;
		bool robotState; // stores the robot state within this class

		// Every thread sleeps some time, for having a bit more time fo the other threads!
		// Time in milliseconds
		static const unsigned long THREADSLEEPTIME = 500; // Default: 25 ms

		/// defines the size of the rgbLed[] arrays.
		static const unsigned char NUMBEROFRGBLEDS = 6; /// \todo also defined in direcs.h !!!
		int minBrightness[NUMBEROFRGBLEDS-1];
		int maxBrightness[NUMBEROFRGBLEDS-1];
		int defaultBrightness[NUMBEROFRGBLEDS-1];
		int brightness[NUMBEROFRGBLEDS]; //! the current position of the rgbLed!

		static const bool ON  = true;   /// For motor or robot "ON"
		static const bool OFF = false;  /// For motor or robot "OFF"

		static const int MINPWM = 1;  /// this is analog to the value in  microcontroller software @sa direcsSTM
		static const int MAXPWM = 99; /// this is analog to the value in  microcontroller software @sa direcsSTM

		/// the possible rgbLed data
		static const unsigned char RGBLEDDEFAULT = 0;
		static const unsigned char RGBLEDACTUAL  = 1;
		static const unsigned char RGBLEDMIN     = 2;
		static const unsigned char RGBLEDMAX     = 3;

		/// the RGB LED numbers
		static const unsigned char RGBLED1 = 0;
		static const unsigned char RGBLED2 = 1;
		static const unsigned char RGBLED3 = 2;
		static const unsigned char RGBLED4 = 3;
		static const unsigned char RGBLED5 = 4;
		static const unsigned char RGBLED6 = 5;

		/// the command for the microcontroller
		static const unsigned char SET_RGBLED1 = 42;
		static const unsigned char SET_RGBLED2 = 43;
		static const unsigned char SET_RGBLED3 = 44;
		static const unsigned char SET_RGBLED4 = 45;
		static const unsigned char SET_RGBLED5 = 46;
		static const unsigned char SET_RGBLED6 = 47;
};

#endif
