/*************************************************************************
 *   Copyright (C) 2008 by Markus Knapp                                  *
 *   mrs @ direcs.de                                                     *
 *                                                                       *
 *   This file is part of mrs.                                           *
 *                                                                       *
 *   mrs is free software: you can redistribute it and/or modify it      *
 *   under the terms of the GNU General Public License as published      *
 *   by the Free Software Foundation, version 3 of the License.          *
 *                                                                       *
 *   mrs is distributed in the hope that it will be useful,              *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of      *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the        *
 *   GNU General Public License for more details.                        *
 *                                                                       *
 *   You should have received a copy of the GNU General Public License   *
 *   along with mrs. If not, see <http://www.gnu.org/licenses/>.         *
 *                                                                       *
 *************************************************************************/

#include "joystickDialog.h"

JoystickDialog::JoystickDialog(QWidget* parent) : QDialog(parent)
{
	ui.setupUi(this);
}


void JoystickDialog::showJoystickButtons(int buttonNumber, bool buttonState)
{
	static bool toggle0 = false;

	//static bool toggle1 = false;
	//static bool toggle2 = false;
	//static bool toggle3 = false;
	//static bool toggle4 = false;
	//static bool toggle5 = false;

	static bool toggle10 = false;
	static bool toggle11 = false;


	// TODO: put button numbers to ini-file
	switch (buttonNumber)
	{
		case 0:
			if (buttonState==true)
			{
				if (toggle0 == false)
				{
					toggle0=true;
				}
				else
				{
					toggle0=false;
				}
			}
			ui.radioBtnJoy0->setChecked(toggle0);
			break;
		case 1:
			ui.radioBtnJoy1->setChecked(buttonState);
			break;
		case 2:
			ui.radioBtnJoy2->setChecked(buttonState);
			break;
		case 3:
			ui.radioBtnJoy3->setChecked(buttonState);
			break;
		case 4:
			ui.radioBtnJoy4->setChecked(buttonState);
			break;
		case 5:
			ui.radioBtnJoy5->setChecked(buttonState);
			break;
		case 10:
			if (buttonState==true)
			{
				if (toggle10 == false)
				{
					toggle10=true;
				}
				else
				{
					toggle10=false;
				}
			}
			ui.radioBtnJoy10->setChecked(toggle10);
			break;
		case 11:
			if (buttonState==true)
			{
				if (toggle11 == false)
				{
					toggle11=true;
				}
				else
				{
					toggle11=false;
				}
			}
			ui.radioBtnJoy11->setChecked(toggle10);
			break;
	}
}


void JoystickDialog::showJoystickAxes(int axisNumber, int axisValue)
{
	// TODO: put axis numbers to ini-file
	switch (axisNumber)
	{
		case JOYSTICKAXISX:
			// X axis
			ui.sliderJoystickX->setValue(axisValue);
			break;
		case JOYSTICKAXISY:
			// Y axis
			ui.sliderJoystickY->setValue(axisValue);
			break;
		case JOYSTICKAXIS2Y:
			// Y axis "buttons"
			// up
			if (axisValue < 0)
			{
				ui.radioBtnJoy5Up->setChecked(true);
			}

			// down
			if (axisValue > 0)
			{
				ui.radioBtnJoy5Down->setChecked(true);
			}

			if (axisValue == 0)
			{
				ui.radioBtnJoy5Up->setChecked(false);
				ui.radioBtnJoy5Down->setChecked(false);
			}
			break;
		case JOYSTICKAXIS2X:
			// X axis "buttons"
			// left
			if (axisValue < 0)
			{
				ui.radioBtnJoy4Left->setChecked(true);
			}

			// right
			if (axisValue > 0)
			{
				ui.radioBtnJoy4Right->setChecked(true);
			}

			if (axisValue == 0)
			{
				ui.radioBtnJoy4Left->setChecked(false);
				ui.radioBtnJoy4Right->setChecked(false);
			}
			break;
	}
}
