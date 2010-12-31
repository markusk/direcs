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

#ifndef MACOSXJOYSTICK_IS_INCLUDED
#define MACOSXJOYSTICK_IS_INCLUDED
/* { */

const int JoyReaderMaxNumAxis=6;
const int JoyReaderMaxNumButton=32;
const int JoyReaderMaxNumHatSwitch=4;

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/errno.h>
#include <sysexits.h>
#include <IOKit/hid/IOHIDLib.h>
#include <QDebug>


class JoyReaderElement
{
public:
	int exist;
	IOHIDElementRef elem;
	int value;

	JoyReaderElement();
};

class JoyReaderAxis : public JoyReaderElement
{
public:
	int min,max;
	int scaledMin,scaledMax;
	int calibCenter,calibMin,calibMax;

	JoyReaderAxis();
	double GetCalibratedValue(void) const;

	void CaptureCenter(void);
	void BeginCaptureMinMax(void);
	void CaptureMinMax(void);
	void CenterFromMinMax(void);
};

class JoyReaderButton : public JoyReaderElement
{
public:
	JoyReaderButton();
};

class JoyReaderHatSwitch : public JoyReaderElement
{
public:
	JoyReaderHatSwitch();
	int valueNeutral;
	int value0Deg;
	int value90Deg;
	int value180Deg;
	int value270Deg;
	int GetDiscreteValue(void) const;
};

class  JoyReader
{
public:
	static IOHIDManagerRef hidManager;
	static CFMutableArrayRef devArray;

	int joyId;
	IOHIDDeviceRef hidDev;
	char regPath[512];

	JoyReaderAxis axis[JoyReaderMaxNumAxis];
	JoyReaderButton button[JoyReaderMaxNumButton];
	JoyReaderHatSwitch hatSwitch[JoyReaderMaxNumHatSwitch];

	JoyReader();
	int SetUpInterface(int joyId,IOHIDDeviceRef hidDev);
	void Read(void);
	void ReleaseInterface(void);

	int WriteCalibInfoFile(FILE *fp) const;
	int ReadCalibInfoFile(FILE *fp);

protected:
	void AddAxis(int axisId,IOHIDElementRef elem,int min,int max,int scaledMin,int scaledMax);

public:
	static int SetUpJoystick(int &nJoystick,JoyReader joystick[],int maxNumJoystick);
};




int JoyReaderSetUpJoystick(int &nJoystick,JoyReader joystick[],int maxNumJoystick);

FILE *JoyReaderOpenJoystickCalibrationFile(const char mode[]);

int JoyReaderSaveJoystickCalibrationInfo(int nJoystick,JoyReader joystick[]);
int JoyReaderLoadJoystickCalibrationInfo(int nJoystick,JoyReader joystick[]);

/* } */
#endif
