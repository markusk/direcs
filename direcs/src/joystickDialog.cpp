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

#include "joystickDialog.h"

JoystickDialog::JoystickDialog(QWidget* parent) : QDialog(parent)
{
	ui.setupUi(this);
}


void JoystickDialog::showJoystickButtons(int buttonNumber, bool buttonState)
{
	/// @sa Direcs::executeJoystickCommand
	static bool toggle0 = false;
	static bool toggle1 = false;
	//static bool toggle2 = false;
	//static bool toggle3 = false;
	static bool toggle4 = false;
	//static bool toggle5 = false;
	static bool toggle10 = false;
	static bool toggle11 = false;


	/// \todo put button numbers to ini-file
	switch (buttonNumber)
	{
		case 0:
			if (buttonState==true)
			{
				toggle0 = !toggle0;
			}
			ui.radioBtnJoy0->setChecked(toggle0);
			break;
		case 1:
			if (buttonState)
			{
				toggle1 = !toggle1;
			}
			ui.radioBtnJoy1->setChecked(toggle1);
			break;
		case 2:
			ui.radioBtnJoy2->setChecked(buttonState);
			break;
		case 3:
			ui.radioBtnJoy3->setChecked(buttonState);
			break;
		case 4:
			if (buttonState==true)
			{
				toggle4 = !toggle4;
			}
			ui.radioBtnJoy4->setChecked(toggle4);
			break;
		case 5:
			ui.radioBtnJoy5->setChecked(buttonState);
			break;
		case 10:
			if (buttonState)
			{
				toggle10 = !toggle10;
			}
			ui.radioBtnJoy10->setChecked(toggle10);
			break;
		case 11:
			if (buttonState==true)
			{
				toggle11 = !toggle11;
			}
			ui.radioBtnJoy11->setChecked(toggle11);
			break;
	}
}


void JoystickDialog::showJoystickPOVButtons(int buttonsState)
{
	switch (buttonsState)
	{
	case 0:
		ui.radioBtnJoy4Left->setChecked(false);
		ui.radioBtnJoy4Right->setChecked(false);
		ui.radioBtnJoy5Up->setChecked(false);
		ui.radioBtnJoy5Down->setChecked(false);
		break;
	case 1: // up
		ui.radioBtnJoy4Left->setChecked(false);
		ui.radioBtnJoy4Right->setChecked(false);
		ui.radioBtnJoy5Up->setChecked(true);
		ui.radioBtnJoy5Down->setChecked(false);
		break;
	case 2: // up right
		ui.radioBtnJoy4Left->setChecked(false);
		ui.radioBtnJoy4Right->setChecked(true);
		ui.radioBtnJoy5Up->setChecked(true);
		ui.radioBtnJoy5Down->setChecked(false);
		break;
	case 3: // right
		ui.radioBtnJoy4Left->setChecked(false);
		ui.radioBtnJoy4Right->setChecked(true);
		ui.radioBtnJoy5Up->setChecked(false);
		ui.radioBtnJoy5Down->setChecked(false);
		break;
	case 4: // down right
		ui.radioBtnJoy4Left->setChecked(false);
		ui.radioBtnJoy4Right->setChecked(true);
		ui.radioBtnJoy5Up->setChecked(false);
		ui.radioBtnJoy5Down->setChecked(true);
		break;
	case 5: // down
		ui.radioBtnJoy4Left->setChecked(false);
		ui.radioBtnJoy4Right->setChecked(false);
		ui.radioBtnJoy5Up->setChecked(false);
		ui.radioBtnJoy5Down->setChecked(true);
		break;
	case 6: // down left
		ui.radioBtnJoy4Left->setChecked(true);
		ui.radioBtnJoy4Right->setChecked(false);
		ui.radioBtnJoy5Up->setChecked(false);
		ui.radioBtnJoy5Down->setChecked(true);
		break;
	case 7: // left
		ui.radioBtnJoy4Left->setChecked(true);
		ui.radioBtnJoy4Right->setChecked(false);
		ui.radioBtnJoy5Up->setChecked(false);
		ui.radioBtnJoy5Down->setChecked(false);
		break;
	case 8: // up left
		ui.radioBtnJoy4Left->setChecked(true);
		ui.radioBtnJoy4Right->setChecked(false);
		ui.radioBtnJoy5Up->setChecked(true);
		ui.radioBtnJoy5Down->setChecked(false);
		break;
	}
}


void JoystickDialog::showJoystickAxes(int axisNumber, int axisValue)
{
	/// \todo put axis numbers to ini-file
	switch (axisNumber)
	{
		case JOYSTICKAXISX3:
			// X axis
			ui.sliderJoystickX->setValue(axisValue);
			break;

		case JOYSTICKAXISY2:
			// Y axis
			ui.sliderJoystickY->setValue(axisValue);
			break;

		case JOYSTICKAXISY5:
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

		case JOYSTICKAXISX4:
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
