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


#include "joystick.h"


Joystick::Joystick()
{
	stopped = false;
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
	axisButtonNumber = 0;
	axisButtonValue = 0;
	lastAxisButtonNumber = 0;
	lastAxisButtonValue = 0;
	lastButtonState = false;

	
	// check if joystick is accessible
	if (isConnected() == false)
	{
		stopped = true;
		return;
	}

	
	ioctl(fd, JSIOCGVERSION, &version);
	ioctl(fd, JSIOCGAXES, &axes);
	ioctl(fd, JSIOCGBUTTONS, &buttons);
	ioctl(fd, JSIOCGNAME(NAME_LENGTH), name);
	ioctl(fd, JSIOCGAXMAP, axmap);
	ioctl(fd, JSIOCGBTNMAP, btnmap);


	//qDebug("Driver version is %d.%d.%d.\n", version >> 16, (version >> 8) & 0xff, version & 0xff);
	//qDebug("Joystick (%s) has %d axes (", name, axes);
	//qDebug("and %d buttons (", buttons);

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

	
	//
	//  start "threading"...
	//
	while (!stopped)
	{
		// let the thread sleep some time
		// for having more time for the other threads
		msleep(THREADSLEEPTIME);
		
		
		if (read(fd, &js, sizeof(struct js_event)) != sizeof(struct js_event))
		{
			emit emitMessage("Error reading joystick device!");
			//stopped = true;
			//return;
			//
			// try further!
		}
		else
		{
		QDateTime now = QDateTime::currentDateTime();
		qDebug() << "joystick event" << now.toString("hh:mm:ss");
			switch(js.type & ~JS_EVENT_INIT)
			{
				case JS_EVENT_BUTTON:
					axisButtonNumber = js.number;
					if (js.value == 0)
					{
						// only emit message. when joystick state has changed!
// 						if ((axisButtonNumber != lastAxisButtonNumber) && (lastButtonState != false) )
// 						{
							qDebug("other button / state");
							emit joystickButtonPressed(axisButtonNumber, false);
// 						}
						// store actual joystick state
						lastAxisButtonNumber = axisButtonNumber;
						lastButtonState = false;
						break;
					}
					if (js.value == 1)
					{
 						// only emit message. when joystick state has changed!
// 						if ((axisButtonNumber != lastAxisButtonNumber) && (lastButtonState != true) )
// 						{
							qDebug("other button / state");
							emit joystickButtonPressed(axisButtonNumber, true);
// 						}
						// store actual joystick state
						lastAxisButtonNumber = axisButtonNumber;
						lastButtonState = true;
						break;
					}
					break;
				case JS_EVENT_AXIS:
					axisButtonNumber = js.number;
					axisButtonValue = js.value;
					emit joystickMoved(axisButtonNumber, axisButtonValue);
					break;
			}
		}
	}

	stopped = false;
}


bool Joystick::isConnected()
{
	if ((fd = open(joystickPort.toAscii(), O_RDONLY)) < 0)
	{
		emit emitMessage( QString(QString("No joystick found at %1").arg(joystickPort)).toAscii() );
		stopped = true;
		return false;
	}
	
	return true;
}


void Joystick::setPort(QString port)
{
	joystickPort = port;
}
