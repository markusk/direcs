/*************************************************************************
 *   Copyright (C) Markus Knapp                                          *
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

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/errno.h>
#include <sysexits.h>
#include <IOKit/hid/IOHIDLib.h>

#include "joyreaderMacOS.h"


JoyReaderElement::JoyReaderElement()
{
	exist=0;
	elem=NULL;
	value=0;
}

JoyReaderAxis::JoyReaderAxis()
{
	min=0;
	max=0;
	scaledMin=0;
	scaledMax=0;
	calibCenter=0;
	calibMin=0;
	calibMax=0;
}

double JoyReaderAxis::GetCalibratedValue(void) const
{
	double calib;

	if(calibCenter<value && calibMax!=calibCenter)
	{
		calib=(double)(value-calibCenter)/(double)(calibMax-calibCenter);
	}
	else if(value<calibCenter && calibMin!=calibCenter)
	{
		calib=(double)(value-calibCenter)/(double)(calibCenter-calibMin);
	}
	else
	{
		return 0.0;
	}

	if(calib>1.0)
	{
		calib=1.0;
	}
	if(calib<-1.0)
	{
		calib=-1.0;
	}

	return calib;
}

void JoyReaderAxis::CaptureCenter(void)
{
	calibCenter=value;
}

void JoyReaderAxis::BeginCaptureMinMax(void)
{
	calibMin=calibCenter+1000;
	calibMax=calibCenter-1000;
}

void JoyReaderAxis::CaptureMinMax(void)
{
	if(value<calibMin)
	{
		calibMin=value;
	}
	if(value>calibMax)
	{
		calibMax=value;
	}
}

void JoyReaderAxis::CenterFromMinMax(void)
{
	calibCenter=(calibMin+calibMax)/2;
}

JoyReaderButton::JoyReaderButton()
{
}

JoyReaderHatSwitch::JoyReaderHatSwitch()
{
	valueNeutral=0;
	value0Deg=1;
	value90Deg=3;
	value180Deg=5;
	value270Deg=7;
}

int JoyReaderHatSwitch::GetDiscreteValue(void) const
{
	if(value==valueNeutral)
	{
		return 0;
	}
	else if(value==value0Deg)
	{
		return 1;
	}
	else if(value==value90Deg)
	{
		return 3;
	}
	else if(value==value180Deg)
	{
		return 5;
	}
	else if(value270Deg==value)
	{
		return 7;
	}
	else if(value0Deg<value && value<value90Deg)
	{
		return 2;
	}
	else if(value90Deg<value && value<value180Deg)
	{
		return 4;
	}
	else if(value180Deg<value && value<value270Deg)
	{
		return 6;
	}
	else if(value270Deg<value)
	{
		return 8;
	}
	return 0;
}


IOHIDManagerRef JoyReader::hidManager=NULL;
CFMutableArrayRef JoyReader::devArray=NULL;

JoyReader::JoyReader()
{
	hidDev=NULL;
}

int JoyReader::SetUpInterface(int joyId,IOHIDDeviceRef hidDev)
{
	this->joyId=joyId;

	if(hidDev!=NULL)
	{
		CFArrayRef elemAry=IOHIDDeviceCopyMatchingElements(hidDev,NULL,0);
		int nElem=(int)CFArrayGetCount(elemAry);
		int isMouse=0,isJoystick=0,isKeyboard=0,isGamePad=0;

//		qDebug("This HID Device has %d elements.",nElem);

		int j;
		for(j=0; j<nElem; j++)
		{
			IOHIDElementRef elem=(IOHIDElementRef)CFArrayGetValueAtIndex(elemAry,j);
//			IOHIDElementType elemType=IOHIDElementGetType(elem);
			unsigned int usage=IOHIDElementGetUsage(elem);
			unsigned int usagePage=IOHIDElementGetUsagePage(elem);
/*
			printf("Element %3d",j);
			switch(elemType)
			{
			case kIOHIDElementTypeInput_ScanCodes:
				printf(" ScanCode  ");
				break;
			case kIOHIDElementTypeInput_Misc:
				printf(" Misc      ");
				break;
			case kIOHIDElementTypeInput_Button:
				printf(" Button    ");
				break;
			case kIOHIDElementTypeInput_Axis:
				printf(" Axis      ");
				break;
			case kIOHIDElementTypeOutput:
				printf(" Output    ");
				break;
			case kIOHIDElementTypeFeature:
				printf(" Feature   ");
				break;
			case kIOHIDElementTypeCollection:
				printf(" Collection");
				break;
			}

			printf("  Usage %3d  UsagePage %3d\n",usage,usagePage);
*/
			if(kHIDPage_GenericDesktop==usagePage)
			{
				switch(usage)
				{
				case kHIDUsage_GD_Mouse:
//					printf("    Can function as mouse\n");
					isMouse=1;
					break;
				case kHIDUsage_GD_Keyboard:
//					printf("    Can function as Keyboard\n");
					isKeyboard=1;
					break;
				case kHIDUsage_GD_Joystick:
//					qDebug("Found a Joystick.");
					isJoystick=1;
					break;
				case kHIDUsage_GD_GamePad:
//					qDebug("Found a GamePad.");
					isGamePad=1;
					break;
				}
			}
		}

		if(0!=isJoystick)
		{
			int nAxis=0;
			int nHat=0;

			int j;
			for(j=0; j<nElem; j++)
			{
				IOHIDElementRef elem=(IOHIDElementRef)CFArrayGetValueAtIndex(elemAry,j);
				IOHIDElementType elemType=IOHIDElementGetType(elem);
				unsigned int usage=IOHIDElementGetUsage(elem);
				unsigned int usagePage=IOHIDElementGetUsagePage(elem);
				// The following two returned 0 and 255
				// IOHIDElementGetPhysicalMin(elem);
				// IOHIDElementGetPhysicalMax(elem);
				int min=IOHIDElementGetLogicalMin(elem);
				int max=IOHIDElementGetLogicalMax(elem);
				int scaledMin=min;
				int scaledMax=max;

				if(elemType==kIOHIDElementTypeInput_Misc ||
				   elemType==kIOHIDElementTypeInput_Button ||
				   elemType==kIOHIDElementTypeInput_Axis ||
				   elemType==kIOHIDElementTypeInput_ScanCodes)
				{
					switch(usagePage)
					{
					case kHIDPage_GenericDesktop:
						switch(usage)
						{
						case kHIDUsage_GD_Mouse:
							break;
						case kHIDUsage_GD_Keyboard:
							break;
						case kHIDUsage_GD_Joystick:
							break;
						case kHIDUsage_GD_GamePad:
							break;
						case kHIDUsage_GD_X:
//							printf("    This element is for X-Axis (%d->%d) Scaled(%d->%d)\n",min,max,scaledMin,scaledMax);
							AddAxis(nAxis++,elem,min,max,scaledMin,scaledMax);
							break;
						case kHIDUsage_GD_Y:
//							printf("    This element is for Y-Axis (%d->%d) Scaled(%d->%d)\n",min,max,scaledMin,scaledMax);
							AddAxis(nAxis++,elem,min,max,scaledMin,scaledMax);
							break;
						case kHIDUsage_GD_Z:
//							printf("    This element is for Z-Axis (%d->%d) Scaled(%d->%d)\n",min,max,scaledMin,scaledMax);
							AddAxis(nAxis++,elem,min,max,scaledMin,scaledMax);
							break;
						case kHIDUsage_GD_Rx:
//							printf("    This element is for Rx-Axis (%d->%d) Scaled(%d->%d)\n",min,max,scaledMin,scaledMax);
							AddAxis(nAxis++,elem,min,max,scaledMin,scaledMax);
							break;
						case kHIDUsage_GD_Ry:
//							printf("    This element is for Ry-Axis (%d->%d) Scaled(%d->%d)\n",min,max,scaledMin,scaledMax);
							AddAxis(nAxis++,elem,min,max,scaledMin,scaledMax);
							break;
						case kHIDUsage_GD_Rz:
//							printf("    This element is for Rz-Axis (%d->%d) Scaled(%d->%d)\n",min,max,scaledMin,scaledMax);
							AddAxis(nAxis++,elem,min,max,scaledMin,scaledMax);
							break;
						case kHIDUsage_GD_Slider:
//							printf("    This element is for Slider (%d->%d) Scaled(%d->%d)\n",min,max,scaledMin,scaledMax);
							AddAxis(nAxis++,elem,min,max,scaledMin,scaledMax);
							break;
						case kHIDUsage_GD_Wheel:
//							printf("    This element is for Wheel (%d->%d) Scaled(%d->%d)\n",min,max,scaledMin,scaledMax);
							break;
						case kHIDUsage_GD_Hatswitch:
//							printf("    This element is for Hatswitch (%d->%d) Scaled(%d->%d)\n",min,max,scaledMin,scaledMax);
							if(nHat<JoyReaderMaxNumHatSwitch)
							{
								hatSwitch[nHat].exist=1;
								hatSwitch[nHat].elem=elem;
								CFRetain(elem);
								nHat++;
							}
							break;
						}
						break;
					case kHIDPage_Button:
//						printf("    This element is for Button %d\n",usage-1);
						usage--;
						if(0<=usage && usage<JoyReaderMaxNumButton)
						{
							button[usage].exist=1;
							button[usage].elem=elem;
							CFRetain(elem);
						}
						break;
					}
				}
			}
			CFRelease(elemAry);
			this->hidDev=hidDev;
			return 1;

		}

		CFRelease(elemAry);
	}

	return 0;
}

void JoyReader::Read(void)
{
	int i;
	IOHIDValueRef valueRef;
	for(i=0; i<JoyReaderMaxNumAxis; i++)
	{
		if(axis[i].exist!=0)
		{
			IOHIDDeviceGetValue(hidDev,axis[i].elem,&valueRef);
			axis[i].value=IOHIDValueGetIntegerValue(valueRef);
		}
	}
	for(i=0; i<JoyReaderMaxNumButton; i++)
	{
		if(button[i].exist!=0)
		{
			IOHIDDeviceGetValue(hidDev,button[i].elem,&valueRef);
			button[i].value=IOHIDValueGetIntegerValue(valueRef);
		}
	}
	for(i=0; i<JoyReaderMaxNumHatSwitch; i++)
	{
		if(hatSwitch[i].exist!=0)
		{
			IOHIDDeviceGetValue(hidDev,hatSwitch[i].elem,&valueRef);

			double scaled=IOHIDValueGetScaledValue(valueRef,kIOHIDValueScaleTypePhysical);
			if(scaled<-0.001 || 359.999<scaled)
			{
				hatSwitch[i].value=0;
			}
			else
			{
				hatSwitch[i].value=1+(int)((scaled+22.5)/45.0);
			}
		}
	}
}

void JoyReader::ReleaseInterface(void)
{
	if(hidDev!=NULL)
	{
		// Honestly, I don't know what to do.
		//
		// Should I do
		//   CFRelease(hidDev);
		// ?
		//
		// This hidDev was copied from a copy of IOHIDManager's device list.
		// Who owns it?  Why did I have to make a copy?
		//
		// The Creare Rule implies that I have the ownership.
		// http://developer.apple.com/mac/library/documentation/CoreFoundation/Conceptual/CFMemoryMgmt/Concepts/Ownership.html#//apple_ref/doc/uid/20001148-SW1
		//
		// Then, I suppose I should release it.  Am I right?
		CFRelease(hidDev);
		hidDev=NULL;
	}
}

void JoyReader::AddAxis(int axisId,IOHIDElementRef elem,int min,int max,int scaledMin,int scaledMax)
{
	if(0<=axisId && axisId<JoyReaderMaxNumAxis)
	{
		axis[axisId].exist=1;
		axis[axisId].elem=elem;
		axis[axisId].min=min;
		axis[axisId].max=max;
		axis[axisId].scaledMin=scaledMin;
		axis[axisId].scaledMax=scaledMax;

		axis[axisId].calibCenter=(min+max)/2;
		axis[axisId].calibMin=min;
		axis[axisId].calibMax=max;

		CFRetain(elem);
	}
}

void CFSetCopyCallBack(const void *value,void *context)
{
	CFArrayAppendValue((CFMutableArrayRef)context,value);
}

int JoyReader::SetUpJoystick(int &nJoystick,JoyReader joystick[],int maxNumJoystick)
{
	nJoystick=0;

	if(NULL==hidManager)
	{
		hidManager=IOHIDManagerCreate(kCFAllocatorDefault,kIOHIDOptionsTypeNone);
	}

	if(NULL!=hidManager)
	{
		IOHIDManagerSetDeviceMatching(hidManager,NULL);  // Just enumrate all devices
		IOHIDManagerOpen(hidManager,kIOHIDOptionsTypeNone);

		CFSetRef copyOfDevices=IOHIDManagerCopyDevices(hidManager);
		if(NULL!=devArray)
		{
			CFRelease(devArray);
			devArray=NULL;
		}
		devArray=CFArrayCreateMutable(kCFAllocatorDefault,0,&kCFTypeArrayCallBacks);
		CFSetApplyFunction(copyOfDevices,CFSetCopyCallBack,(void *)devArray);

		CFIndex nDev=CFArrayGetCount(devArray);

		qDebug("%d HID devices found. Looking for joysticks...", (int) nDev);

		CFRelease(copyOfDevices);



		int i;
		for(i=0; i<nDev && nJoystick<maxNumJoystick; i++)
		{
			IOHIDDeviceRef hidDev=(IOHIDDeviceRef)CFArrayGetValueAtIndex(devArray,i);
			if(joystick[nJoystick].SetUpInterface(nJoystick,hidDev)!=0)
			{
				nJoystick++;
				// CFRelease(hidDev);  // Doesn't it destroy integrity of devArray?
			}
		}
	}


	qDebug("%d joystick(s) found.", nJoystick);

	return nJoystick;
}

int JoyReader::WriteCalibInfoFile(FILE *fp) const
{
	int i;
	fprintf(fp,"BGNJOY %d\n",joyId);
	for(i=0; i<JoyReaderMaxNumAxis; i++)
	{
		if(0!=axis[i].exist)
		{
			fprintf(fp,"AXSINF %d %d %d %d\n",i,axis[i].calibCenter,axis[i].calibMin,axis[i].calibMax);
		}
	}
#ifdef YSJOYREADER_USE_HAT_CALIBRATION
	for(i=0; i<JoyReaderMaxNumHatSwitch; i++)
	{
		if(0!=hatSwitch[i].exist)
		{
			fprintf(fp,"HATINF %d %d %d %d %d %d\n",
				i,
				hatSwitch[i].valueNeutral,
				hatSwitch[i].value0Deg,
				hatSwitch[i].value90Deg,
				hatSwitch[i].value180Deg,
				hatSwitch[i].value270Deg);
		}
	}
#endif
	fprintf(fp,"ENDJOY\n");
	return 1;
}

int JoyReader::ReadCalibInfoFile(FILE *fp)
{
	char str[256];
	while(fgets(str,255,fp)!=NULL)
	{
		if(strncmp(str,"AXSINF",6)==0)
		{
			int axisId,cen,min,max;
			sscanf(str,"%*s %d %d %d %d",&axisId,&cen,&min,&max);
			if(0<=axisId && axisId<JoyReaderMaxNumAxis)
			{
				axis[axisId].calibCenter=cen;
				axis[axisId].calibMin=min;
				axis[axisId].calibMax=max;
			}
		}
#ifdef YSJOYREADER_USE_HAT_CALIBRATION
		else if(strncmp(str,"HATINF",6)==0)
		{
			int hatId;
			int valueNeutral=0,value0Deg=1,value90Deg=3,value180Deg=5,value270Deg=7;
			sscanf(str,"%*s %d %d %d %d %d %d",&hatId,&valueNeutral,&value0Deg,&value90Deg,&value180Deg,&value270Deg);
			if(0<=hatId && hatId<JoyReaderMaxNumHatSwitch)
			{
				hatSwitch[hatId].valueNeutral=valueNeutral;
				hatSwitch[hatId].value0Deg=value0Deg;
				hatSwitch[hatId].value90Deg=value90Deg;
				hatSwitch[hatId].value180Deg=value180Deg;
				hatSwitch[hatId].value270Deg=value270Deg;
			}
		}
#endif
		else if(strncmp(str,"ENDJOY",6)==0)
		{
			return 1;
		}
	}
	return 0;
}

int JoyReaderSetUpJoystick(int &nJoystick,JoyReader joystick[],int maxNumJoystick)
{
	return JoyReader::SetUpJoystick(nJoystick,joystick,maxNumJoystick);
}


extern "C" FILE *JoyReaderOpenJoystickCalibrationFileC(const char mode[]);

FILE *JoyReaderOpenJoystickCalibrationFile(const char mode[])
{
  return JoyReaderOpenJoystickCalibrationFileC(mode);
}

int JoyReaderSaveJoystickCalibrationInfo(int nJoystick,JoyReader joystick[])
{
	FILE *fp;
	fp=JoyReaderOpenJoystickCalibrationFile("w");

	if(fp!=NULL)
	{
		int i;
		for(i=0; i<nJoystick; i++)
		{
			joystick[i].WriteCalibInfoFile(fp);
		}

		fclose(fp);
		return 1;
	}
	return 0;
}

int JoyReaderLoadJoystickCalibrationInfo(int nJoystick,JoyReader joystick[])
{
	FILE *fp;
	fp=JoyReaderOpenJoystickCalibrationFile("r");

	if(fp!=NULL)
	{
		char str[256];
		while(fgets(str,255,fp)!=NULL)
		{
			if(strncmp(str,"BGNJOY",6)==0)
			{
				int joyId;
				sscanf(str,"%*s %d",&joyId);
				if(0<=joyId && joyId<nJoystick)
				{
					joystick[joyId].ReadCalibInfoFile(fp);
				}
			}
		}
		fclose(fp);
		return 1;
	}
	return 0;
}
