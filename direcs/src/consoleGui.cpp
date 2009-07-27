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

#include "consoleGui.h"


ConsoleGui::ConsoleGui()
{
}


ConsoleGui::~ConsoleGui()
{
}

/*
TODO: void ConsoleGui::closeEvent()
{
	qDebug("closeEvent");
	emit shutdown();
}
*/


void ConsoleGui::setRobotControls(bool state)
{
	// store the state
	robotIsOn = state;
}


void ConsoleGui::appendLog(QString text, bool CR, bool sayIt)
{
	Q_UNUSED(CR);
	Q_UNUSED(sayIt);
	QByteArray textForConsole;
	
	//------------------------------
	// remove HTML tags from string
	//------------------------------
	int start= -1;
	do
	{
		// search for the first HTML "<"
		start = text.indexOf("<");

		if (start != 1)
		{
			text.remove(start, text.indexOf(">") + 1 - start );
		}
	} while (text.contains(">"));
	// till the last HTML ">" is found
	
	// print text to console
	// qDebug() << text; is NOT used, because it adds quotation marks to all strings
	textForConsole = text.toLatin1();
	qDebug("%s", textForConsole.data());
}


void ConsoleGui::appendNetworkLog(QString text, bool CR, bool sayIt)
{
	Q_UNUSED(CR);
	Q_UNUSED(sayIt);
	QByteArray textForConsole;
	
	// print text to console
	// qDebug() << text; is NOT used, because it adds quotation marks to all strings
	textForConsole = text.toLatin1();
	qDebug("NETWORK COMMAND: %s", textForConsole.data());
}




void ConsoleGui::showDistanceGraphical(int sensor, int distance)
{
	if ((sensor < SENSOR1) || (sensor > SENSOR16))
	{
	  return;
	}


	switch (sensor)
	{
		/*
		case SENSOR1:
			ui.progressBarSensor1->setValue(distance);
			return;
			break;
		case SENSOR2:
			ui.progressBarSensor2->setValue(distance);
			return;
			break;
		case SENSOR3:
			ui.progressBarSensor3->setValue(distance);
			return;
			break;
		case SENSOR4:
			ui.progressBarSensor4->setValue(distance);
			return;
			break;
		case SENSOR5:
			ui.progressBarSensor5->setValue(distance);
			return;
			break;
		case SENSOR6:
			ui.progressBarSensor6->setValue(distance);
			return;
			break;
		case SENSOR7:
			ui.progressBarSensor7->setValue(distance);
			return;
			break;
		case SENSOR8:
			ui.progressBarSensor8->setValue(distance);
			return;
			break;
		*/
		case SENSOR16:
			// TODO:show values in console
			Q_UNUSED(sensor);
			Q_UNUSED(distance);
			return;
			break;
	}
}


void ConsoleGui::showDrivenDistance(int sensor, int distance)
{
	// show distance value in centimeters in a text label
	// erwartet den value als distance (z.B. in cm)!
	if ((sensor < MOTORSENSOR1) || (sensor > MOTORSENSOR2))
	{
	  return;
	}

	Q_UNUSED(sensor);
	Q_UNUSED(distance);

	switch (sensor)
	{
		case MOTORSENSOR1:
			// TODO:show values in console
			return;
			break;
		case MOTORSENSOR2:
			// TODO:show values in console
			return;
			break;
	}
}


void ConsoleGui::showAlarm(short int sensor, bool state)
{
	if ((sensor < SENSOR1) || (sensor > SENSOR16))
	{
		qDebug("error in showAlarm! sensorValue=%d", sensor);
		return;
	}

	Q_UNUSED(sensor);
	Q_UNUSED(state);

	switch (sensor)
	{
		/*
		case SENSOR1:
			if (state == ON)
			{
				// change sensor-pixmap color to red     [blue = 0, 70, 213]
				//ui.labelSensorPic1->setPalette(QPalette(labelFillColorRed));
				//ui.labelSensorAlarm1->setEnabled(true);
				//ui.frameSensorAlarm1->setPalette(QPalette(labelFillColorRed));
				// red progressBar background
				ui.progressBarSensor1->setPalette(QPalette(labelFillColorRed));
			}
			else
			{
				// change sensor-pixmap color to white
				//ui.labelSensorPic1->setPalette(QPalette(QColor(255,255,255)));
				//ui.labelSensorAlarm1->setEnabled(false);
				//ui.frameSensorAlarm1->setPalette(QPalette(QColor(255,255,255)));
				// progressBar background white
				ui.progressBarSensor1->setPalette(QPalette(QColor(255, 255, 255)));
			}
			return;
			break;
		case SENSOR2:
			if (state == ON)
			{
				// change sensor-pixmap color to red     [blue = 0, 70, 213]
				//ui.labelSensorPic2->setPalette(QPalette(labelFillColorRed));
				//ui.labelSensorAlarm2->setEnabled(true);
				//ui.frameSensorAlarm2->setPalette(QPalette(labelFillColorRed));
				// red progressBar background
				ui.progressBarSensor2->setPalette(QPalette(labelFillColorRed));
			}
			else
			{
				// change sensor-pixmap color to white
				//ui.labelSensorPic2->setPalette(QPalette(QColor(255,255,255)));
				//ui.labelSensorAlarm2->setEnabled(false);
				//ui.frameSensorAlarm2->setPalette(QPalette(QColor(255,255,255)));
				// progressBar background white
				ui.progressBarSensor2->setPalette(QPalette(QColor(255, 255, 255)));
			}
			return;
			break;
		case SENSOR3:
			if (state == ON)
			{
				// change sensor-pixmap color to red     [blue = 0, 70, 213]
				//ui.labelSensorPic3->setPalette(QPalette(labelFillColorRed));
				//ui.labelSensorAlarm3->setEnabled(true);
				//ui.frameSensorAlarm3->setPalette(QPalette(labelFillColorRed));
				// red progressBar background
				ui.progressBarSensor3->setPalette(QPalette(labelFillColorRed));
			}
			else
			{
				// change sensor-pixmap color to white
				//ui.labelSensorPic3->setPalette(QPalette(QColor(255,255,255)));
				//ui.labelSensorAlarm3->setEnabled(false);
				//ui.frameSensorAlarm3->setPalette(QPalette(QColor(255,255,255)));
				// progressBar background white
				ui.progressBarSensor3->setPalette(QPalette(QColor(255, 255, 255)));
			}
			return;
			break;
		case SENSOR4:
			if (state == ON)
			{
				// change sensor-pixmap color to red     [blue = 0, 70, 213]
				//ui.labelSensorPic4->setPalette(QPalette(labelFillColorRed));
				//ui.labelSensorAlarm4->setEnabled(true);
				//ui.frameSensorAlarm4->setPalette(QPalette(labelFillColorRed));
				// red progressBar background
				ui.progressBarSensor4->setPalette(QPalette(labelFillColorRed));
			}
			else
			{
				// change sensor-pixmap color to white
				//ui.labelSensorPic4->setPalette(QPalette(QColor(255,255,255)));
				//ui.labelSensorAlarm4->setEnabled(false);
				//ui.frameSensorAlarm4->setPalette(QPalette(QColor(255,255,255)));
				// progressBar background white
				ui.progressBarSensor4->setPalette(QPalette(QColor(255, 255, 255)));
			}
			return;
			break;
		case SENSOR5:
			if (state == ON)
			{
				// change sensor-pixmap color to red     [blue = 0, 70, 213]
				//ui.labelSensorPic5->setPalette(QPalette(labelFillColorRed));
				//ui.labelSensorAlarm5->setEnabled(true);
				//ui.frameSensorAlarm5->setPalette(QPalette(labelFillColorRed));
				// red progressBar background
				ui.progressBarSensor5->setPalette(QPalette(labelFillColorRed));
			}
			else
			{
				// change sensor-pixmap color to white
				//ui.labelSensorPic5->setPalette(QPalette(QColor(255,255,255)));
				//ui.labelSensorAlarm5->setEnabled(false);
				//ui.frameSensorAlarm5->setPalette(QPalette(QColor(255,255,255)));
				// progressBar background white
				ui.progressBarSensor5->setPalette(QPalette(QColor(255, 255, 255)));
			}
			return;
			break;
		case SENSOR6:
			if (state == ON)
			{
				// change sensor-pixmap color to red     [blue = 0, 70, 213]
				//ui.labelSensorPic6->setPalette(QPalette(labelFillColorRed));
				//ui.labelSensorAlarm6->setEnabled(true);
				//ui.frameSensorAlarm6->setPalette(QPalette(labelFillColorRed));
				// red progressBar background
				ui.progressBarSensor6->setPalette(QPalette(labelFillColorRed));
			}
			else
			{
				// change sensor-pixmap color to white
				//ui.labelSensorPic6->setPalette(QPalette(QColor(255,255,255)));
				//ui.labelSensorAlarm6->setEnabled(false);
				//ui.frameSensorAlarm6->setPalette(QPalette(QColor(255,255,255)));
				// progressBar background white
				ui.progressBarSensor6->setPalette(QPalette(QColor(255, 255, 255)));
			}
			return;
			break;
		case SENSOR7:
			if (state == ON)
			{
				// change sensor-pixmap color to red     [blue = 0, 70, 213]
				//ui.labelSensorPic7->setPalette(QPalette(labelFillColorRed));
				//ui.labelSensorAlarm7->setEnabled(true);
				//ui.frameSensorAlarm7->setPalette(QPalette(labelFillColorRed));
				// red progressBar background
				ui.progressBarSensor7->setPalette(QPalette(labelFillColorRed));
			}
			else
			{
				// change sensor-pixmap color to white
				//ui.labelSensorPic7->setPalette(QPalette(QColor(255,255,255)));
				//ui.labelSensorAlarm7->setEnabled(false);
				//ui.frameSensorAlarm7->setPalette(QPalette(QColor(255,255,255)));
				// progressBar background white
				ui.progressBarSensor7->setPalette(QPalette(QColor(255, 255, 255)));
			}
			return;
			break;
		case SENSOR8:
			if (state == ON)
			{
				// change sensor-pixmap color to red     [blue = 0, 70, 213]
				//ui.labelSensorPic8->setPalette(QPalette(labelFillColorRed));
				//ui.labelSensorAlarm8->setEnabled(true);
				//ui.frameSensorAlarm8->setPalette(QPalette(labelFillColorRed));
				// red progressBot background
				ui.progressBarSensor8->setPalette(QPalette(labelFillColorRed));
			}
			else
			{
				// change sensor-pixmap color to white
				//ui.labelSensorPic8->setPalette(QPalette(QColor(255,255,255)));
				//ui.labelSensorAlarm8->setEnabled(false);
				//ui.frameSensorAlarm8->setPalette(QPalette(QColor(255,255,255)));
				// progressBar background white
				ui.progressBarSensor8->setPalette(QPalette(QColor(255, 255, 255)));
			}
			return;
			break;
		*/
		case SENSOR16:
			if (state == ON)
			{
				// change sensor-pixmap color to red     [blue = 0, 70, 213]
				//ui.labelSensorPic16->setPalette(QPalette(labelFillColorRed));
				//ui.labelSensorAlarm16->setEnabled(true);
				//ui.frameSensorAlarm16->setPalette(QPalette(labelFillColorRed));
				// TODO:show values RED in console
			}
			else
			{
				// change sensor-pixmap color to white
				//ui.labelSensorPic16->setPalette(QPalette(QColor(255,255,255)));
				//ui.labelSensorAlarm16->setEnabled(false);
				//ui.frameSensorAlarm16->setPalette(QPalette(QColor(255,255,255)));
				// TODO:show values NORMAL in console
			}
			return;
			break;
	}
}


void ConsoleGui::showMotorStatus(unsigned char motor, bool power, unsigned char direction)
{

	Q_UNUSED(motor);
	Q_UNUSED(power);
	Q_UNUSED(direction);
/*	
	switch (motor)
	{
		//-------------
		case MOTOR1:
		//-------------
			switch (power)
			{
				case ON:
					// TODO:show value/state in console
					break;
					// TODO:show value/state in console
					break;
				case SAME:
					break;
			}

			switch (direction)
			{
				case CLOCKWISE:
					// TODO:show value/state in console
					break;
				case COUNTERCLOCKWISE:
					// TODO:show value/state in console
					break;
				case SAME:
					break;
			}
			break;

		//-------------
		case MOTOR2:
		//-------------
			switch (power)
			{
				case ON:
					// TODO:show value/state in console
					break;
				case OFF:
					// TODO:show value/state in console
					break;
				case SAME:
					break;
			}

			switch (direction)
			{
				case CLOCKWISE:
					// TODO:show value/state in console
					break;
				case COUNTERCLOCKWISE:
					// TODO:show value/state in console
					break;
				case SAME:
					break;
			}
			break;

		//-------------
		case MOTOR3:
		//-------------
			switch (power)
			{
				case ON:
					// TODO:show value/state in console
					break;
				case OFF:
					// TODO:show value/state in console
					break;
				case SAME:
					break;
			}

			switch (direction)
			{
				case CLOCKWISE:
					// TODO:show value/state in console
					break;
				case COUNTERCLOCKWISE:
					// TODO:show value/state in console
					break;
				case SAME:
					break;
			}
			break;

		//-------------
		case MOTOR4:
		//-------------
			switch (power)
			{
				case ON:
					// TODO:show value/state in console
					break;
				case OFF:
					// TODO:show value/state in console
					break;
				case SAME:
					break;
			}

			switch (direction)
			{
				case CLOCKWISE:
					// TODO:show value/state in console
					break;
				case COUNTERCLOCKWISE:
					// TODO:show value/state in console
					break;
				case SAME:
					break;
			}
			break;
	} // switch
	*/
}


void ConsoleGui::showLaserFrontAngles(int largestFreeAreaStart, int largestFreeAreaEnd, int centerOfFreeWay)
{
	Q_UNUSED(largestFreeAreaStart);
	Q_UNUSED(largestFreeAreaEnd);
	Q_UNUSED(centerOfFreeWay);
	// TODO:show value/state in console
}


void ConsoleGui::showPreferredDirection(QString direction)
{
	Q_UNUSED(direction);
	// TODO:show value/state in console
	if (direction == "FORWARD")
	{
		return;
	}

	if (direction == "LEFT")
	{
		return;
	}

	if (direction == "RIGHT")
	{
		return;
	}
}


void ConsoleGui::refreshLaserViewFront(float *laserScannerValues, int *laserScannerFlags)
{
	// TODO:show value/state in console
	Q_UNUSED(laserScannerValues);
	Q_UNUSED(laserScannerFlags);
}


void ConsoleGui::refreshLaserViewRear(float *laserScannerValues, int *laserScannerFlags)
{
	// TODO:show value/state in console
	Q_UNUSED(laserScannerValues);
	Q_UNUSED(laserScannerFlags);
}
