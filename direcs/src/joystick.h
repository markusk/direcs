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

#ifndef JOYSTICK_H
#define JOYSTICK_H

#include <QtGlobal> // for Q_OS_* Makro!

#ifdef Q_OS_LINUX // joystick support for linux:
#include <sys/ioctl.h>
#include <sys/time.h>
#include <sys/types.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <stdint.h>
#include <linux/input.h>
#include <linux/joystick.h>
#endif

#ifdef Q_OS_MAC // joystick support for Mac OS:
#include "ysjoyreader.h"

#include <ctype.h>
#include <sys/errno.h>
#include <sysexits.h>
#include <IOKit/hid/IOHIDLib.h>
#endif

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>


#define NAME_LENGTH 128

#include <QThread>

/**
\brief This class is for controling the robot with a joystick.

It opens a joystick device and emits signals on joystick movements or when a button is pressed.
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
		Emits a info or error message to a slot.
		This slot can be used to display a text on a splash screen, log file, to print it to a console...
		@param text is the message to be emitted
		*/
		void message(QString text);
		
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
		volatile bool stopped;
		QString joystickPort;

#ifdef Q_OS_LINUX // joystick support for linux:
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
#endif

		// Every thread sleeps some time, for having a bit more time fo the other threads!
		// Time in milliseconds
		static const unsigned long THREADSLEEPTIME = 100; // Default: 25 ms

#ifdef Q_OS_MAC // joystick support for Mac OS:
		int numJoystick;
		static const int maxNumJoystick = 4;
		YsJoyReader joystick[maxNumJoystick]; // the Mac OS joystick object
#endif
};


/*
#ifdef Q_OS_MAC // joystick support for Mac OS:
static const int YsJoyReaderMaxNumAxis = 6;
static const int YsJoyReaderMaxNumButton = 32;
static const int YsJoyReaderMaxNumHatSwitch = 4;
#endif

class YsJoyReaderElement
{
public:
	int exist;
	IOHIDElementRef elem;
	int value;

	YsJoyReaderElement();
};

class YsJoyReaderAxis : public YsJoyReaderElement
{
public:
	int min,max;
	int scaledMin,scaledMax;
	int calibCenter,calibMin,calibMax;

	YsJoyReaderAxis();
	double GetCalibratedValue(void) const;

	void CaptureCenter(void);
	void BeginCaptureMinMax(void);
	void CaptureMinMax(void);
	void CenterFromMinMax(void);
};

class YsJoyReaderButton : public YsJoyReaderElement
{
public:
	YsJoyReaderButton();
};

class YsJoyReaderHatSwitch : public YsJoyReaderElement
{
public:
	YsJoyReaderHatSwitch();
	int valueNeutral;
	int value0Deg;
	int value90Deg;
	int value180Deg;
	int value270Deg;
	int GetDiscreteValue(void) const;
};

class  YsJoyReader
{
public:
	static IOHIDManagerRef hidManager;
	static CFMutableArrayRef devArray;

	int joyId;
	IOHIDDeviceRef hidDev;
	char regPath[512];

	YsJoyReaderAxis axis[YsJoyReaderMaxNumAxis];
	YsJoyReaderButton button[YsJoyReaderMaxNumButton];
	YsJoyReaderHatSwitch hatSwitch[YsJoyReaderMaxNumHatSwitch];

	YsJoyReader();
	int SetUpInterface(int joyId,IOHIDDeviceRef hidDev);
	void Read(void);
	void ReleaseInterface(void);

	int WriteCalibInfoFile(FILE *fp) const;
	int ReadCalibInfoFile(FILE *fp);

protected:
	void AddAxis(int axisId,IOHIDElementRef elem,int min,int max,int scaledMin,int scaledMax);

public:
	static int SetUpJoystick(int &nJoystick,YsJoyReader joystick[],int maxNumJoystick);
};


int YsJoyReaderSetUpJoystick(int &nJoystick,YsJoyReader joystick[],int maxNumJoystick);

FILE *YsJoyReaderOpenJoystickCalibrationFile(const char mode[]);

int YsJoyReaderSaveJoystickCalibrationInfo(int nJoystick,YsJoyReader joystick[]);
int YsJoyReaderLoadJoystickCalibrationInfo(int nJoystick,YsJoyReader joystick[]);
#endif // mac os
*/
#endif
