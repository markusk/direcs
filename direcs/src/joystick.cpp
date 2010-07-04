/*************************************************************************
 *   Copyright (C) 2010 by Markus Knapp                                  *
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

	#ifdef Q_OS_MAC // joystick support for Mac OS:
	numJoystick = 0;

	YsJoyReaderSetUpJoystick(numJoystick, joystick, maxNumJoystick);
	YsJoyReaderLoadJoystickCalibrationInfo(numJoystick, joystick);
	#endif
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
	#ifdef Q_OS_MAC // joystick support for Mac OS:
	int i = 0;
	int j = 0;


	//
	//  start "threading"...
	//
	while (!stopped)
	{
		// let the thread sleep some time for having more time for the other threads
		msleep(THREADSLEEPTIME);

		for (i=0; i<numJoystick; i++)
		{
			// read joysticks state
			joystick[i].Read();

			for (j=0; j<YsJoyReaderMaxNumAxis; j++)
			{
				if (joystick[i].axis[j].exist!=0)
				{
//					emit message( QString("Joystick %3, Axis %1 = %2").arg(j).arg(joystick[i].axis[j].GetCalibratedValue()).arg(i) );
				}
			}

			for (j=0; j<YsJoyReaderMaxNumButton; j++)
			{
				if (joystick[i].button[j].exist != 0)
				{

					// emit message( QString("Button %1 = %2").arg(j).arg(joystick[i].button[j].value) );
					if (joystick[i].button[j].value == 1)
					{
						emit joystickButtonPressed(j, true);
					}
					else
					{
						emit joystickButtonPressed(j, false);
					}
				}
			}

			for (j=0; j<YsJoyReaderMaxNumHatSwitch; j++)
			{
				if (joystick[i].hatSwitch[j].exist!=0)
				{
//					emit message( QString("POV %1 = %2").arg(j).arg(joystick[i].hatSwitch[j].value) );
				}
			}
		}
	}
	#endif

	#ifdef Q_OS_LINUX // joystick support for Linux:
	axes = 2;
	buttons = 2;
	axisButtonNumber = 0;
	axisButtonValue = 0;
	
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
			emit message("Error reading joystick device!");
			// no 'return' here, try further!
		}
		else
		{
// 			switch(js.type & ~JS_EVENT_INIT)
			switch(js.type)
			{
				case JS_EVENT_INIT:
					emit message("Joystick initialised.");
					break;
				case JS_EVENT_BUTTON:
					axisButtonNumber = js.number;
					if (js.value == 0)
					{
						emit joystickButtonPressed(axisButtonNumber, false);
						break;
					}
					if (js.value == 1)
					{
						emit joystickButtonPressed(axisButtonNumber, true);
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
#endif

	stopped = false;
}


bool Joystick::isConnected()
{
#ifdef Q_OS_LINUX // joystick support for Linux:
	if ((fd = open(joystickPort.toAscii(), O_RDONLY)) < 0)
	{
		emit message( QString(QString("No joystick found at %1").arg(joystickPort)).toAscii() );
		stopped = true;
		return false;
	}

	// joystick is connected
	return true;

#endif

#ifdef Q_OS_MAC // joystick support for Mac OS:
	return true; // TODO: check if a joystick is found!
#endif

	return false; // we return false. Noy joystick supported -> NO joystick "connected"!
}


void Joystick::setPort(QString port)
{
	joystickPort = port;
}
