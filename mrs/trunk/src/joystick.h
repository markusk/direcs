/*
 * jstest.c  Version 1.2
 *
 * Copyright (c) 1996-1999 Vojtech Pavlik
 *
 * Sponsored by SuSE
 */

/*
 * This program can be used to test all the features of the Linux
 * joystick API, including non-blocking and select() access, as
 * well as version 0.x compatibility mode. It is also intended to
 * serve as an example implementation for those who wish to learn
 * how to write their own joystick using applications.
 *
 * Debian Package: joystick
 *
 * Example: jstest /dev/input/js0
 */

/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
 * Should you need to contact me, the author, you can do so either by
 * e-mail - mail your message to <vojtech@ucw.cz>, or by paper mail:
 * Vojtech Pavlik, Simunkova 1594, Prague 8, 182 00 Czech Republic
 */

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
