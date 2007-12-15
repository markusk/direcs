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

// /usr/include/linux/input.h
#include <linux/input.h>
// /usr/include/linux/joystick.h
#include <linux/joystick.h>

#define NAME_LENGTH 128


class Joystick
{
	public:
		Joystick();
		~Joystick();
		
	private:
		//char *axis_names[ABS_MAX + 1];
		//char *button_names[KEY_MAX - BTN_MISC + 1];
		int fd, i;
		unsigned char axes;
		unsigned char buttons;
		int version;
		char name[NAME_LENGTH];
		uint16_t btnmap[KEY_MAX - BTN_MISC + 1];
		uint8_t axmap[ABS_MAX + 1];
		
		
		static const char *axis_names[ABS_MAX + 1] = {
		"X", "Y", "Z", "Rx", "Ry", "Rz", "Throttle", "Rudder", 
		"Wheel", "Gas", "Brake", "?", "?", "?", "?", "?",
		"Hat0X", "Hat0Y", "Hat1X", "Hat1Y", "Hat2X", "Hat2Y", "Hat3X", "Hat3Y",
		"?", "?", "?", "?", "?", "?", "?", 
		};
	
		static const char *button_names[KEY_MAX - BTN_MISC + 1] = {
		"Btn0", "Btn1", "Btn2", "Btn3", "Btn4", "Btn5", "Btn6", "Btn7", "Btn8", "Btn9", "?", "?", "?", "?", "?", "?",
		"LeftBtn", "RightBtn", "MiddleBtn", "SideBtn", "ExtraBtn", "ForwardBtn", "BackBtn", "TaskBtn", "?", "?", "?", "?", "?", "?", "?", "?",
		"Trigger", "ThumbBtn", "ThumbBtn2", "TopBtn", "TopBtn2", "PinkieBtn", "BaseBtn", "BaseBtn2", "BaseBtn3", "BaseBtn4", "BaseBtn5", "BaseBtn6", "BtnDead",
		"BtnA", "BtnB", "BtnC", "BtnX", "BtnY", "BtnZ", "BtnTL", "BtnTR", "BtnTL2", "BtnTR2", "BtnSelect", "BtnStart", "BtnMode", "BtnThumbL", "BtnThumbR", "?",
		"?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?", 
		"WheelBtn", "Gear up",
		};
}
#endif
