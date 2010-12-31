/*************************************************************************
 *   Copyright (C) 2011 by Markus Knapp                                  *
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
	numJoysticks = 0;
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
	int currentJoystick = 0;
	int buttonOrAxis = 0;


	//
	//  start "threading"...
	//
	while (!stopped)
	{
		// let the thread sleep some time for having more time for the other threads
		msleep(THREADSLEEPTIME);


		if (numJoysticks != 0)
		{
			for (currentJoystick=0; currentJoystick<numJoysticks; currentJoystick++)
			{
				//
				// read joysticks state
				//
				joystick[currentJoystick].Read();


				//
				// Axes
				//
				for (buttonOrAxis=0; buttonOrAxis<JoyReaderMaxNumAxis; buttonOrAxis++)
				{
					if (joystick[currentJoystick].axis[buttonOrAxis].exist != 0)
					{
						// we convert, round and multiply with 32767 the original value here to have a nive int from range -32767 to +32767 for the GUI
						emit joystickMoved(buttonOrAxis, (int) qRound(joystick[currentJoystick].axis[buttonOrAxis].GetCalibratedValue() * 32767) );
					}
				}


				//
				// Buttons
				//
				for (buttonOrAxis=0; buttonOrAxis<JoyReaderMaxNumButton; buttonOrAxis++)
				{
					if (joystick[currentJoystick].button[buttonOrAxis].exist != 0)
					{
						if (joystick[currentJoystick].button[buttonOrAxis].value == 1)
						{
							emit joystickButtonPressed(buttonOrAxis, true);
						}
						else
						{
							emit joystickButtonPressed(buttonOrAxis, false);
						}
					}
				}


				//
				// POV (Hat Switch)
				//
				for (buttonOrAxis=0; buttonOrAxis<JoyReaderMaxNumHatSwitch; buttonOrAxis++)
				{
					if (joystick[currentJoystick].hatSwitch[buttonOrAxis].exist != 0)
					{
						// emits the value 0 to 8 to Gui::JoystickDialog
						emit joystickPOVButtonPressed(joystick[currentJoystick].hatSwitch[buttonOrAxis].value);


						// the following is a workaround for Mac only since we cannon identify the POV under Linux and therefor we would need a different Direcs::executeJoystickCommand
						// so here we're emitting another signal for this Slot (which is for drive-by-joystick). And this is the POV button layout:
						//
						//            1
						//          8   2
						//        7   0   3
						//          6   4
						//            5
						//
						switch (joystick[currentJoystick].hatSwitch[buttonOrAxis].value)
						{
						case 0:
							// no POV button pressed
							emit joystickMoved(JOYSTICKAXISY5, 0);
							break;
						case 1:
							// Y axis, UP, drive forward
							emit joystickMoved(JOYSTICKAXISY5, -32767);
							break;
						case 3:
							// x axis, RIGHT, drive right
							emit joystickMoved(JOYSTICKAXISX4, 32767);
							break;
						case 5:
							// Y axis, DOWN, drive backward
							emit joystickMoved(JOYSTICKAXISY5, 32767);
							break;
						case 7:
							// x axis, LEFT, drive left
							emit joystickMoved(JOYSTICKAXISX4, -32767);
							break;
						}
					}
				}
			}
		} // numJoysticks
	} // while
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
	JoyReaderSetUpJoystick(numJoysticks, joystick, maxNumJoystick);
	JoyReaderLoadJoystickCalibrationInfo(numJoysticks, joystick);

	if (numJoysticks != 0)
	{
		return true;
	}

	return false;
#endif

	return false; // we return false. Noy joystick supported -> NO joystick "connected"!
}


void Joystick::setPort(QString port)
{
	joystickPort = port;
}
