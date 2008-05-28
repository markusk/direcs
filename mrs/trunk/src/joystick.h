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

#ifndef JOYSTICK_H
#define JOYSTICK_H

#include <sys/ioctl.h>
#include <sys/time.h>
#include <sys/types.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#include <linux/input.h>
#include <linux/joystick.h>

#define NAME_LENGTH 128

#include <QThread>

/**
\brief This class is for controling the robot with a joystick.
It opens a joystick device and emits siglans on joystick movements or when a button is pressed.
*/
class Joystick : public QThread
{
    Q_OBJECT

	public:
		Joystick();
		~Joystick();
		void stop();
		virtual void run();
		
		/**
		Sets the joystick port.
		@param port is the joystick port (e.g. /dev/input/js0)
		 */
		void setPort(QString port);
		
		/**
		Returns the state of a connected joystick.
		@return true, if connected
		*/
		bool isConnected();


	signals:
		/**
		Sends a string to the GUI log.
		@param text is the message to be emitted
		*/
		void emitMessage(QString text);
		
		/**
		Emits a signal when an joystick move event occured.
		@param axisNumber is the number of the joysticks axis
		@param axisValue is the axis value
		*/
		void joystickMoved(int axisNumber, int axisValue);
		
		/**
		Emits a signal when an joystick button-pressed event occured.
		@param axisNumber is the number of the joysticks button
		@param buttonState is the buttons state (true when button is down)
		*/
		void joystickButtonPressed(int axisNumber, bool buttonState);
		

	private:
		//mutable QMutex mutex; // make this class thread-safe
		volatile bool stopped;
		QString joystickPort;
		int fd, i;
		unsigned char axes;
		unsigned char buttons;
		int version;
		char name[NAME_LENGTH];
		uint16_t btnmap[KEY_MAX - BTN_MISC + 1];
		uint8_t axmap[ABS_MAX + 1];
		
		int *axis;
		char *button;
		struct js_event js;
		short int axisButtonNumber;
		short int axisButtonValue;
};
#endif
