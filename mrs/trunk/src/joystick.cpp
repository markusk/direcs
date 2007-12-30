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
		if (read(fd, &js, sizeof(struct js_event)) != sizeof(struct js_event))
		{
			emit emitMessage("Error reading joystick device!");
			stopped = true;
			return;
		}

		switch(js.type & ~JS_EVENT_INIT)
		{
		case JS_EVENT_BUTTON:
			axisButtonNumber = js.number;
			if (js.value == 0)
				emit joystickButtonPressed(axisButtonNumber, false);
			if (js.value == 1)
				emit joystickButtonPressed(axisButtonNumber, true);
			break;
		case JS_EVENT_AXIS:
			axisButtonNumber = js.number;
			axisButtonValue = js.value;
			emit joystickMoved(axisButtonNumber, axisButtonValue);
			break;
		}
	}

	stopped = false;
}


bool Joystick::isConnected()
{
	if ((fd = open(joystickPort.toAscii(), O_RDONLY)) < 0)
	{
		//qDebug( QString(QString("NO joystick found at %1").arg(joystickPort)).toAscii() );
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
