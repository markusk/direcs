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

#ifndef SERVO_H
#define SERVO_H

//-------------------------------------------------------------------
#include "interfaceAvr.h"
//-------------------------------------------------------------------
#include <QThread>
#include <QMutex>
//-------------------------------------------------------------------


/**
\brief This class controls connected servos

It has to be a thread because of communicating at the same time with the microcontroller via the class @saInterface() !
*/
class Servo : public QThread
{
    Q_OBJECT

	public:
		Servo(InterfaceAvr *i, QMutex *m);
		~Servo();
		
		/**
		Stops the thread.
		*/
		void stop();
		
		/**
		Starts the thread.
		*/
		virtual void run();
		
		/**
		Sets the servos maximum start, end and its default position.
		@param servo is the servo number.
		@param type can be SVMIN, SVMAX, SVSTART, SVEND, SVDEFAULT or SVCURRENT
		@param position is the position (0 - 255).
		 */
		void setServoPosition(int servo, unsigned char type, unsigned char position);
		
		/**
		Returns the servo start, end, default or current position.
		@param servo is the servo number.
		@param type can be SVMIN, SVMAX, SVSTART, SVEND, SVDEFAULT or SVCURRENT=default
		@return the servo position (0 - 255)
		 */
		unsigned char getServoPosition(int servo, unsigned char type=SVCURRENT);


	public slots:
		/**
		Moves a servo.
		@param servo is the servo number.
		@param position is the position (0 - 255).
		 */
		void moveServo(unsigned char servo, unsigned char position);
		
		/**
		Moves all servos into their default positions.
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
		mutable QMutex *mutex; // make this class thread-safe
		InterfaceAvr *interface1;
		volatile bool stopped;
		bool robotState; // stores the robot state within this class
		
		// Every thread sleeps some time, for having a bit more time fo the other threads!
		// Time in milliseconds
		static const unsigned long THREADSLEEPTIME = 100; // Default: 25 ms
		
		//! defines the size of the servo[] arrays.
		static const unsigned char NUMBEROFSERVOS = 6; // TODO: also defined in direcs.h !!!
		int servoStartPosition[NUMBEROFSERVOS];
		int servoEndPosition[NUMBEROFSERVOS];
		int servoMinPosition[NUMBEROFSERVOS];
		int servoMaxPosition[NUMBEROFSERVOS];
		int servoDefaultPosition[NUMBEROFSERVOS];
		int servoPosition[NUMBEROFSERVOS]; //! the current position of the servo!
		//int servoPositionConvHelper[NUMBEROFSERVOS]; //! a conversion helper, if the servo is fixed the other way round!

		static const bool ON  = true;   /** For motor or robot "ON" */
		static const bool OFF = false;  /** For motor or robot "OFF" */
		
		//! the possible servo data
		static const unsigned char SVSTART   = 0;
		static const unsigned char SVEND     = 1;
		static const unsigned char SVDEFAULT = 2;
		static const unsigned char SVCURRENT = 3;
		static const unsigned char SVMIN     = 4;
		static const unsigned char SVMAX     = 5;
		
		//! the servo numbers
		static const unsigned char SERVO1 = 0;
		static const unsigned char SERVO2 = 1;
		static const unsigned char SERVO3 = 2;
		static const unsigned char SERVO4 = 3;
		static const unsigned char SERVO5 = 4;
		static const unsigned char SERVO6 = 5;

		//! the command for the microcontroller
		static const unsigned char SET_SERVO1 = 42;
		static const unsigned char SET_SERVO2 = 43;
		static const unsigned char SET_SERVO3 = 44;
		static const unsigned char SET_SERVO4 = 45;
		static const unsigned char SET_SERVO5 = 46;
		static const unsigned char SET_SERVO6 = 47;
};

#endif
