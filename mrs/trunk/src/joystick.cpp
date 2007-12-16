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


//
// C++ Implementation: joystick
//
// Description: 
//
//
// Author: Markus Knapp <webmaster@direcs.de>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//


#include "joystick.h"


Joystick::Joystick()
{
	//QMutexLocker locker(&mutex); // make this class thread-safe
	stopped = false;
	
	
	*axis_names[0] = "X";
	*axis_names[1] = "Y";
	*axis_names[2] = "Z";
	*axis_names[3] = "Rx";
	*axis_names[4] = "Ry";
	*axis_names[5] = "Rz";
	*axis_names[6] = "Throttle";
	*axis_names[7] = "Rudder";
	*axis_names[8] = "Wheel";
	*axis_names[9] = "Gas";
	*axis_names[10] = "Brake";
	*axis_names[11] = "?";
	*axis_names[12] = "?";
	*axis_names[13] = "?";
	*axis_names[14] = "?";
	*axis_names[15] = "Hat0X";
	*axis_names[16] = "Hat0Y";
	*axis_names[17] = "Hat1X";
	*axis_names[18] = "Hat1Y";
	*axis_names[19] = "Hat2X";
	*axis_names[20] = "Hat2Y";
	*axis_names[21] = "Hat3X";
	*axis_names[22] = "Hat3Y";
	*axis_names[23] = "?";
	*axis_names[24] = "?";
	*axis_names[25] = "?";
	*axis_names[26] = "?";
	*axis_names[27] = "?";
	*axis_names[28] = "?";
	*axis_names[29] = "?";
	
	
	*button_names[0] = "Btn0";
	*button_names[1] = "Btn1";
	*button_names[2] = "Btn2";
	*button_names[3] = "Btn3";
	*button_names[4] = "Btn4";
	*button_names[5] = "Btn5";
	*button_names[6] = "Btn6";
	*button_names[7] = "Btn7";
	*button_names[8] = "Btn8";
	*button_names[9] = "Btn9";
	*button_names[10] = "?";
	*button_names[11] = "?";
	*button_names[12] = "?";
	*button_names[13] = "?";
	*button_names[14] = "?";
	*button_names[15] = "?";
	*button_names[16] = "LeftBtn";
	*button_names[17] = "RightBtn";
	*button_names[18] = "MiddleBtn";
	*button_names[19] = "SideBtn";
	*button_names[20] = "ExtraBtn";
	*button_names[21] = "ForwardBtn";
	*button_names[22] = "BackBtn";
	*button_names[23] = "TaskBtn";
	*button_names[24] = "?";
	*button_names[25] = "?";
	*button_names[26] = "?";
	*button_names[27] = "?";
	*button_names[28] = "?";
	*button_names[29] = "?";
	*button_names[30] = "?";
	*button_names[31] = "?";
	*button_names[32] = "Trigger";
	*button_names[33] = "ThumbBtn";
	*button_names[34] = "ThumbBtn2";
	*button_names[35] = "TopBtn";
	*button_names[36] = "TopBtn2";
	*button_names[37] = "PinkieBtn";
	*button_names[38] = "BaseBtn";
	*button_names[39] = "BaseBtn2";
	*button_names[40] = "BaseBtn3";
	*button_names[41] = "BaseBtn4";
	*button_names[42] = "BaseBtn5";
	*button_names[43] = "BaseBtn6";
	*button_names[44] = "BtnDead";
	*button_names[45] = "BtnA";
	*button_names[46] = "BtnB";
	*button_names[47] = "BtnC";
	*button_names[48] = "BtnX";
	*button_names[49] = "BtnY";
	*button_names[50] = "BtnZ";
	*button_names[51] = "BtnTL";
	*button_names[52] = "BtnTR";
	*button_names[53] = "BtnTL2";
	*button_names[54] = "BtnTR2";
	*button_names[55] = "BtnSelect";
	*button_names[56] = "BtnStart";
	*button_names[57] = "BtnMode";
	*button_names[58] = "BtnThumbL";
	*button_names[59] = "BtnThumbR";
	*button_names[60] = "?";
	*button_names[61] = "?";
	*button_names[62] = "?";
	*button_names[63] = "?";
	*button_names[64] = "?";
	*button_names[65] = "?";
	*button_names[66] = "?";
	*button_names[67] = "?";
	*button_names[68] = "?";
	*button_names[69] = "?";
	*button_names[70] = "?";
	*button_names[71] = "?";
	*button_names[72] = "?";
	*button_names[73] = "?";
	*button_names[74] = "?";
	*button_names[75] = "?";
	*button_names[76] = "?";
	*button_names[77] = "WheelBtn";
	*button_names[78] = "Gear up";
}


Joystick::~Joystick()
{
}


void Joystick::stop()
{
	stopped = true;
}


void Joystick::run()
{
	axes = 2;
	buttons = 2;
	//version = 0x000800;
	//name[NAME_LENGTH] = "Unknown";

	/*
	if (argc < 2 || argc > 3 || !strcmp("--help", argv[1])) {
		puts("");
		puts("Usage: jstest [<mode>] <device>");
		puts("");
		puts("Modes:");
		puts("  --normal           One-line mode showing immediate status");
		puts("  --old              Same as --normal, using 0.x interface");
		puts("  --event            Prints events as they come in");
		puts("  --nonblock         Same as --event, in nonblocking mode");
		puts("  --select           Same as --event, using select() call");
		puts("");
		return 1;
	}
	*/
	
	//if ((fd = open(argv[argc - 1], O_RDONLY)) < 0) {
	if ((fd = open("/dev/input/js0", O_RDONLY)) < 0)
	{
		//perror("jstest");
		qDebug("NO joystick connected (Could not open /dev/input/jd0)!");
		stopped = true;
		return;
	}

	ioctl(fd, JSIOCGVERSION, &version);
	ioctl(fd, JSIOCGAXES, &axes);
	ioctl(fd, JSIOCGBUTTONS, &buttons);
	ioctl(fd, JSIOCGNAME(NAME_LENGTH), name);
	ioctl(fd, JSIOCGAXMAP, axmap);
	ioctl(fd, JSIOCGBTNMAP, btnmap);


	qDebug("Driver version is %d.%d.%d.\n", version >> 16, (version >> 8) & 0xff, version & 0xff);

	qDebug("Joystick (%s) has %d axes (", name, axes);
	for (i = 0; i < axes; i++)
		qDebug("%s%s", i > 0 ? ", " : "", axis_names[axmap[i]]);
	puts(")");

	qDebug("and %d buttons (", buttons);
	for (i = 0; i < buttons; i++)
		printf("%s%s", i > 0 ? ", " : "", button_names[btnmap[i] - BTN_MISC]);
	puts(").");

	qDebug("Testing ... (interrupt to exit)\n");

/*
 * Old (0.x) interface.
 */
/*
	if ((argc == 2 && version < 0x010000) || !strcmp("--old", argv[1])) {

		struct JS_DATA_TYPE js;

		while (1) {

			if (read(fd, &js, JS_RETURN) != JS_RETURN) {
				perror("\njstest: error reading");
				exit(1);
			}

			printf("Axes: X:%3d Y:%3d Buttons: A:%s B:%s\r",
				js.x, js.y, (js.buttons & 1) ? "on " : "off", (js.buttons & 2) ? "on " : "off");

			fflush(stdout);

			usleep(10000);
		}
	}
*/

	
	/*
	* Event interface, single line readout.
	*/
	axis = calloc(axes, sizeof(int));
	button = calloc(buttons, sizeof(char));
/*
	//
	//  start "threading"...
	//
	while (!stopped)
	{
		if (read(fd, &js, sizeof(struct js_event)) != sizeof(struct js_event))
		{
			qDebug("\nerror reading joystick device / file.");
			stopped = true;
			return;
		}

		switch(js.type & ~JS_EVENT_INIT)
		{
		case JS_EVENT_BUTTON:
			button[js.number] = js.value;
			break;
		case JS_EVENT_AXIS:
			axis[js.number] = js.value;
			break;
		}

		printf("\r");

		if (axes) {
			printf("Axes: ");
			for (i = 0; i < axes; i++)
				qDebug("%2d:%6d ", i, axis[i]);
		}

		if (buttons) {
			qDebug("Buttons: ");
			for (i = 0; i < buttons; i++)
				qDebug("%2d:%s ", i, button[i] ? "on " : "off");
		}

		fflush(stdout);
		
		//emit joystickMoved(&laserScannerValues[0], &laserScannerFlags[0]);
	}
*/

/*
 * Event interface, events being printed.
 */
/*
	if (!strcmp("--event", argv[1])) {

		struct js_event js;

		while (1) {
			if (read(fd, &js, sizeof(struct js_event)) != sizeof(struct js_event)) {
				perror("\njstest: error reading");
				exit(1);
			}

			printf("Event: type %d, time %d, number %d, value %d\n",
				js.type, js.time, js.number, js.value);

			fflush(stdout);
		}
	}
*/
/*
 * Reading in nonblocking mode.
 */
/*
	if (!strcmp("--nonblock", argv[1])) {

		struct js_event js;

		fcntl(fd, F_SETFL, O_NONBLOCK);

		while (1) {

			while (read(fd, &js, sizeof(struct js_event)) == sizeof(struct js_event))  {
				printf("Event: type %d, time %d, number %d, value %d\n",
					js.type, js.time, js.number, js.value);
			}

			if (errno != EAGAIN) {
				perror("\njstest: error reading");
				exit(1);
			}

			usleep(10000);
		}
	}
*/
/*
 * Using select() on joystick fd.
 */
/*
	if (!strcmp("--select", argv[1])) {

		struct js_event js;
		struct timeval tv;
		fd_set set;

		tv.tv_sec = 1;
		tv.tv_usec = 0;

		while (1) {

			FD_ZERO(&set);
			FD_SET(fd, &set);

			if (select(fd+1, &set, NULL, NULL, &tv)) {

				if (read(fd, &js, sizeof(struct js_event)) != sizeof(struct js_event)) {
					perror("\njstest: error reading");
					exit(1);
				}

				printf("Event: type %d, time %d, number %d, value %d\n",
					js.type, js.time, js.number, js.value);

			}

		}
	}
*/
	//printf("jstest: unknown mode: %s\n", argv[1]);
	stopped = false;
}
