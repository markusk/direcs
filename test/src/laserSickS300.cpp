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

#include "laserSickS300.h"


SickS300::SickS300()
{
	// The S300 Standard works only with a fixed baudrate of 38400 thru the diagnostic port.
	baudRate = 38400;

	// creating the serial port object
	serialPort = new DirecsSerial();

	laserSerialPort = "NOTSET";

	// -----------------------------------------------------------------------------------------
	// The scanner S300 has a half degree resolution! and a field of view (fov) of 270 degrees!
	// -----------------------------------------------------------------------------------------

	// some laser data init
	/*
	data.min_angle = -45*M_PI/270; // 270 degrees has the laser
	data.max_angle = 225*M_PI/270; // 270 degrees: angle from -45 deg to 225 deg
	data.resolution = 0.5*M_PI/270;// 270 degreed

	data.max_range = 5.0;
	data.ranges_count = DEFAULT_LASER_SAMPLES;

	data.intensity_count = 0; // do not know this
	data.id = 0;
	*/

	/*
	for (int i=0; i<LASERSAMPLES; i++)
	{
		scanResult[i] = 0.0;
	}
	*/

	// forward the messages from direcsSerial directly
	connect(serialPort, SIGNAL(message(QString)), this, SIGNAL(message(QString)));

	emit message(QString("Using baudrate of %1 baud.").arg(baudRate));
}


SickS300::~SickS300()
{
//	QTime x;
//	qDebug("S300 shut down @ %d:%d:%d-%d", x.currentTime().hour(), x.currentTime().minute(), x.currentTime().second(), x.currentTime().msec());
	emit message("Shutting laserscanner SICK S300 down");
	closeComPort();
	emit message("OKAY");

	delete serialPort;
}


void SickS300::setDevicePort(QString serialPort)
{
	laserSerialPort = serialPort;
}


bool SickS300::openComPort()
{
	// for QString to char* conversion
	QByteArray ba = laserSerialPort.toLatin1();


	// check if serial port was set
	if (laserSerialPort == "NOTSET")
	{
		emit message("<font color=\"#FF0000\">Serial port not set! (SickS300::openComPortd)</font>");

		// emitting a signal to other modules which don't get the return value but need to know that we have a sensor error here.
		emit systemerror(-1);

		return false;
	}

	// check if file (serial port) exists
	if (QFile::exists(laserSerialPort) == false)
	{
		emit message( QString("<font color=\"#FF0000\">Path %1 not found!</font>").arg(laserSerialPort) );

		// emitting a signal to other modules which don't get the return value but need to know that we have a sensor error here.
		emit systemerror(-1);

		return false;
	}


	// serial port config (57600, no HWFLCTRL, 8N1) and flush also done in openAtmelPort!
	if (serialPort->openAtmelPort( ba.data(), baudRate ) == -1)
	{

		// emitting a signal to other modules which don't get the return value but need to know that we have a sensor error here.
		emit systemerror(-1);

		return false;
	}

	return true;
}


int SickS300::closeComPort()
{
	// see SICK document "telegram listing standard", 9090807/2007-05-09, page 9, "Release Token" (Telegram type SEND (0x41 0x44))
	const unsigned char releaseTokenCommand[]={0x00,0x00,0x41,0x44,0x19,0x00,0x00,0x05,0xFF,0x07,0x19,0x00,0x00,0x05,0xFF,0x07,0x00,0x00,0xE7,0xB8};
	unsigned int i = 0;


	emit message("Shutting down Sick S300:");

	// send "release token" to laser
	emit message("Sending 'release token'...");
	for (i=0; i<sizeof(releaseTokenCommand); i++)
	{
		if (sendChar(releaseTokenCommand[i]) == false)
		{
			emit message( QString("ERROR sending byte no. %1.").arg(i) );

			// emitting a signal to other modules which don't get the return value but need to know that we have a sensor error here.
			emit systemerror(-1);

			return -1;
		}
	}
	emit message("OKAY");


	emit message("Receiving answer...");
	unsigned char answer = 255;

	// getting anser 0x00 0x00 0x00 0x00
	for (i=0; i<4; i++)
	{
		if (receiveChar(&answer) == true)
		{
			// emit message(QString("Received byte: 0x%1").arg(answer, 2, 16, QLatin1Char('0')));

			// error
			if (answer == 255)
			{
				// emitting a signal to other modules which don't get the return value but need to know that we have a sensor error here.
				emit systemerror(-1);

				emit message("ERROR getting answer from laser.");
			}
		}
	}
	emit message("OKAY");

	// close serial port
	serialPort->closeAtmelPort();

	return 0;
}


bool SickS300::sendChar(unsigned char character)
{
// 	static int receiveErrorCounter = 0;


	// send one byte to the serial port with direcsSerial
	if (serialPort->writeAtmelPort(&character) <= 0)
	{
		emit message("<font color=\"#FF0000\">ERROR writing serial port (sendChar, SickS300)!<font>");

		return false;
	}

	return true;
}


bool SickS300::receiveChar(unsigned char *character)
{
	// reading one char with direcsSerial
	// Must return 1 (1 character succussfull read)!
	if (serialPort->readAtmelPort(character, 1) != 1)
	{
		// ERROR
		emit message("<font color=\"#FF0000\">ERROR reading serial port (receiveChar, SickS300)!<font>");
		return false;
	}

	return true;
}


int SickS300::setup()
{
	// see SICK document "telegram listing standard", 9090807/2007-05-09, page 9, "Get Token" (Telegram type SEND (0x41 0x44))
	const unsigned char getTokenCommand[]={0x00,0x00,0x41,0x44,0x19,0x00,0x00,0x05,0xFF,0x07,0x19,0x00,0x00,0x05,0xFF,0x07,0x07,0x0F,0x9F,0xD0};
	unsigned char answer = 255;
	unsigned int i = 0;


	emit message("Initialising Sick S300:");

	// send "get token" to laser
	emit message("Sending 'get token'...");
	for (i=0; i<sizeof(getTokenCommand); i++)
	{
		if (sendChar(getTokenCommand[i]) == false)
		{
			emit message( QString("<font color=\"#FF0000\">ERROR sending byte no. %1 (SickS300::setup).</font>").arg(i+1) );

			// emitting a signal to other modules which don't get the return value but need to know that we have a sensor error here.
			emit systemerror(-1);

			return -1;
		}
	}
	emit message("OKAY");

	// getting anser 0x00 0x00 0x00 0x00
	emit message("Receiving answer...");
	for (i=0; i<4; i++)
	{
		if (receiveChar(&answer) == true)
		{
			if (answer != 0)
			{
				emit message(QString("<font color=\"#FF0000\">ERROR: answer byte no. %1 was 0x%2 instead 0x00 (SickS300::readRequestTelegram).</font>").arg(i+1).arg(answer, 2, 16, QLatin1Char('0')));

				// emitting a signal to other modules which don't get the return value but need to know that we have a sensor error here. e.g. obstacle check thread.
				emit systemerror(-1);

				return -1;
			}
		}
		else
		{
			// error
			emit message("<font color=\"#FF0000\">ERROR</font>");

			// emitting a signal to other modules which don't get the return value but need to know that we have a sensor error here.
			emit systemerror(-1);

			return -1;
		}

		// emit message(QString("Received byte: 0x%1").arg(answer, 2, 16, QLatin1Char('0')));
	}
	emit message("Sick laser S300 setup OKAY");

	return 0;
}


int SickS300::readRequestTelegram()
{
	// see SICK document "telegram listing standard", 9090807/2007-05-09, page 9, "Read Scandata (block 12)" (Telegram type FETCH (0x45 0x44))
	// 00 00
	// 45		0x45 means fetch telegram (request data)
	// 44		0x44 means data typ: block access
	// 0C		destination address, e.g. block 12 (0x0C) for scan data
	// 00		source address
	// 02 22	size (this one was measured with a serial port analyzer, using the original SICK software CDS.
	//			The following is the original code, regarding the original SICK documentation:	 02 FE = size
	// FF		coordination flag, always 0xFF
	// 07		device address is always 0x07, when we have only one S300
	const unsigned char readScandataCommand[]={0x00,0x00,0x45,0x44,0x0C,0x00,0x02,0x22,0xFF,0x07};
	unsigned char answer = 255;
	unsigned int i = 0;
	float angle = 0.0;
	int result = -1;


//	QTime x;
//	qDebug("S300 start scan @ %d:%d:%d-%d", x.currentTime().hour(), x.currentTime().minute(), x.currentTime().second(), x.currentTime().msec());

	// flushing serial input buffer
	result = serialPort->purgeRx();

	if (result != 0)
	{
		emit message(QString("<font color=\"#FF0000\">ERROR %1 flushing serial port (SickS300::readRequestTelegram).</font>").arg(result));

		// emitting a signal to other modules which don't get the return value but need to know that we have a sensor error here. e.g. obstacle check thread.
		emit systemerror(-1);

		return -1;
	}

	// send "get scan data" to laser
	// emit message("Sending 'get scan data'...");
	for (i=0; i<sizeof(readScandataCommand); i++)
	{
		if (sendChar(readScandataCommand[i]) == false)
		{
			emit message( QString("<font color=\"#FF0000\">ERROR sending byte no. %1 (SickS300::readRequestTelegram).</font>").arg(i+1) );

			// emitting a signal to other modules which don't get the return value but need to know that we have a sensor error here. e.g. obstacle check thread.
			emit systemerror(-1);

			return -1;
		}
	}
	//emit message("OKAY");


	// Reading answer, 4 byte (00 00 00 00)
	//emit message("Receiving answer...");
	for (i=0; i<4; i++)
	{
		if (receiveChar(&answer) == true)
		{
			// check if every answer bit is 0x00
			// the last byte (no 4) contains the error code if != 0.
			if (answer != 0x00)
			{
				emit message(QString("<font color=\"#FF0000\">ERROR: answer byte no. %1 was 0x%2 instead 0x00 (SickS300::readRequestTelegram).</font>").arg(i+1).arg(answer, 2, 16, QLatin1Char('0')));

				// emitting a signal to other modules which don't get the return value but need to know that we have a sensor error here. e.g. obstacle check thread.
				emit systemerror(-1);

				return -1;
			}
		}
		else
		{
			// error
			emit message(QString("<font color=\"#FF0000\">ERROR receiving 00 00 00 00 answer at byte no. %1 (SickS300::readRequestTelegram).</font>").arg(i+1));

			// emitting a signal to other modules which don't get the return value but need to know that we have a sensor error here. e.g. obstacle check thread.
			emit systemerror(-1);

			return -1;
		}
	}
	//emit message("OKAY");


	//-----------------------------------------------------
	// reading repeated header, 6 byte (0C 00 02 22 FF 07)
	//-----------------------------------------------------
	if (receiveChar(&answer) == true)
	{

		if (answer != 0x0C)
		{
			emit message(QString("ERROR: answer byte no.1 was 0x%1 instead of 0x0C.").arg( answer, 2, 16, QLatin1Char('0') ));
			emit systemerror(-1);
			return -1;
		}
	}
	else
	{
		// error
		emit message("<font color=\"#FF0000\">ERROR receiving repeated header at byte no. 1 (SickS300::readRequestTelegram).</font>");
		emit systemerror(-1);
		return -1;
	}

	if (receiveChar(&answer) == true)
	{

		if (answer != 0x00)
		{
			emit message(QString("ERROR: answer byte no.2 was 0x%1 instead of 0x00.").arg( answer, 2, 16, QLatin1Char('0') ));
			emit systemerror(-1);
			return -1;
		}
	}
	else
	{
		// error
		emit message("<font color=\"#FF0000\">ERROR receiving repeated header at byte no. 2 (SickS300::readRequestTelegram).</font>");
		emit systemerror(-1);
		return -1;
	}

	if (receiveChar(&answer) == true)
	{

		if (answer != 0x02)
		{
			emit message(QString("ERROR: answer byte no.3 was 0x%1 instead of 0x02.").arg( answer, 2, 16, QLatin1Char('0') ));
			emit systemerror(-1);
			return -1;
		}
	}
	else
	{
		// error
		emit message("<font color=\"#FF0000\">ERROR receiving repeated header at byte no. 3 (SickS300::readRequestTelegram).</font>");
		emit systemerror(-1);
		return -1;
	}

	if (receiveChar(&answer) == true)
	{

		if (answer != 0x22)
		{
			emit message(QString("ERROR: answer byte no.4 was 0x%1 instead of 0x22.").arg( answer, 2, 16, QLatin1Char('0') ));
			emit systemerror(-1);
			return -1;
		}
	}
	else
	{
		// error
		emit message("<font color=\"#FF0000\">ERROR receiving repeated header at byte no. 4 (SickS300::readRequestTelegram).</font>");
		emit systemerror(-1);
		return -1;
	}

	if (receiveChar(&answer) == true)
	{

		if (answer != 0xFF)
		{
			emit message(QString("ERROR: answer byte no.5 was 0x%1 instead of 0xFF.").arg( answer, 2, 16, QLatin1Char('0') ));
			emit systemerror(-1);
			return -1;
		}
	}
	else
	{
		// error
		emit message("<font color=\"#FF0000\">ERROR receiving repeated header at byte no. 5 (SickS300::readRequestTelegram).</font>");
		emit systemerror(-1);
		return -1;
	}

	if (receiveChar(&answer) == true)
	{

		if (answer != 0x07)
		{
			emit message(QString("ERROR: answer byte no.6 was 0x%1 instead of 0x07.").arg( answer, 2, 16, QLatin1Char('0') ));
			emit systemerror(-1);
			return -1;
		}
	}
	else
	{
		// error
		emit message("<font color=\"#FF0000\">ERROR receiving repeated header at byte no. 6 (SickS300::readRequestTelegram).</font>");
		emit systemerror(-1);
		return -1;
	}


	//-----------------------------------------------------
	// reading 2 unknow bytes, 6 byte (usually 0x00 0x08 ??)
	//-----------------------------------------------------
	if (receiveChar(&answer) == true)
	{

		if (answer != 0x00)
		{
			emit message(QString("ERROR: 1st answer byte was 0x%1 instead of 0x00.").arg( answer, 2, 16, QLatin1Char('0') ));
			emit systemerror(-1);
			return -1;
		}
	}
	else
	{
		// error
		emit message("<font color=\"#FF0000\">ERROR receiving 1st 'unknown byte' (SickS300::readRequestTelegram).</font>");
		emit systemerror(-1);
		return -1;
	}

	if (receiveChar(&answer) == true)
	{

		if (answer != 0x08)
		{
			emit message(QString("ERROR: 2nd answer byte was 0x%1 instead of 0x08.").arg( answer, 2, 16, QLatin1Char('0') ));
			emit systemerror(-1);
			return -1;
		}
	}
	else
	{
		// error
		emit message("<font color=\"#FF0000\">ERROR receiving 2nd 'unknown byte' (SickS300::readRequestTelegram).</font>");
		emit systemerror(-1);
		return -1;
	}


	//-------------------------------------------------------------------------------
	// Reading scan data (the distances!), LASERSAMPLES bytes (@sa laserSickS300.h)
	//-------------------------------------------------------------------------------
	for (i=0; i<LASERSAMPLES; i++)
	{
		if (receiveChar(&answer) == true)
		{
			// emit message( QString("Received byte no. %1: 0x%2").arg(i+1).arg(answer, 2, 16, QLatin1Char('0')) );
			// store the data temporary because we get them separated by LowByte and HighByte
			scanData[i] = answer;
		}
		else
		{
			// error
			emit message(QString("<font color=\"#FF0000\">ERROR receiving scan data at byte no. %1 from %2 (SickS300::readRequestTelegram).</font>").arg(i+1).arg(LASERSAMPLES+1));

			// emitting a signal to other modules which don't get the return value but need to know that we have a sensor error here. e.g. obstacle check thread.
			emit systemerror(-1);

			return -1;
		}
	}
	//emit message("OKAY");


	//----------------------
	// Reading CRC, 2 bytes
	//----------------------
	for (i=0; i<2; i++)
	{
		if (receiveChar(&answer) == true)
		{
			// emit message(QString("Received byte: 0x%1").arg(answer, 2, 16, QLatin1Char('0')));

/*
			// FIXME: check CRC !!
			if (answer != 0)
			{
				emit message(QString("ERROR: answer byte no. %1 was not 0x00").arg(i+1));

				// emitting a signal to other modules which don't get the return value but need to know that we have a sensor error here. e.g. obstacle check thread.
				emit systemerror(-1);

				return -1;
			}
*/
		}
		else
		{
			// error
			emit message(QString("<font color=\"#FF0000\">ERROR receiving CRC at byte no. %1 (SickS300::readRequestTelegram).</font>").arg(i+1));

			// emitting a signal to other modules which don't get the return value but need to know that we have a sensor error here. e.g. obstacle check thread.
			emit systemerror(-1);

			return -1;
		}
	}
	//emit message("OKAY");


	//----------------------------------------------------
	// convert data from 2 x 16 Bit to one 16 Bit value
	// and RETURN the distances
	//----------------------------------------------------
	for (i=0; i<LASERSAMPLES; i++)
	{
		if (angle < 270.0)
		{
			distances[i] = (float) ( ((scanData[2*i+1] & 0x1f)<<8) | scanData[2*i] );
			// qDebug("Copied: no. %d from %d values at %f�. Distance %f cm", i, LASERSAMPLES, angle, distances[i]);
			// emit message( QString("Measured distance at angle %1: %2 cm.").arg(angle, 4, 'f', 1).arg(scanResult[i]) );
			angle += 0.5;

			/*
			// If a measured laser distance is greater than LASERMAXLENGTH, it will be set to the maximum of possible "free" meters!
			// (This is due to a bug when reading angle 0, which results in a lenght of 2048 cm)
			//
			// This value was set to 0.0 m in the past, but doesn't help if we have a 0m line in the middle of "free" lines when navigation...!!
			if (distances[i] > LASERMAXLENGTH)
			{
				distances[i] = LASERMAXLENGTH;
			}
			*/
		}
	}

//	qDebug("S300 scan end @ %d:%d:%d-%d", x.currentTime().hour(), x.currentTime().minute(), x.currentTime().second(), x.currentTime().msec());
	return 0;
}


int SickS300::readUnknownTelegram()
{
	// see also SICK document "telegram listing standard", 9090807/2007-05-09, page 9, "Read Scandata (block 12)" (Telegram type FETCH (0x45 0x44))
	// 00 00
	// 45		0x45 means fetch telegram (request data)
	// 44		0x44 means data typ: block access
	// 0B		destination address, block 11 (0x0B) > > > > > > > I don't know what this block means! I logged it with a serial port logger!
	// 00		source address
	// 00 7B	size (this one was measured with a serial port analyser, using the original SICK software CDS.
	// FF		coordination flag, always 0xFF
	// 07		device address is always 0x07, when we have only one S300
	const unsigned char unknownCommand[]={0x00,0x00,0x45,0x44,0x0B,0x00,0x00,0x7B,0xFF,0x07};
	unsigned char answer = 255;
	unsigned int i = 0;
	int result = -1;


//	QTime x;
//	qDebug("S300 start scan @ %d:%d:%d-%d", x.currentTime().hour(), x.currentTime().minute(), x.currentTime().second(), x.currentTime().msec());

	// flushing serial input buffer
	result = serialPort->purgeRx();

	if (result != 0)
	{
		emit message("<font color=\"#FF0000\">ERROR flushing serial port (SickS300::readRequestTelegram).</font>");

		// emitting a signal to other modules which don't get the return value but need to know that we have a sensor error here. e.g. obstacle check thread.
		emit systemerror(-1);

		return -1;
	}

	// send "read block 0B" to laser
	 emit message("Sending 'read block 0B'...");
	for (i=0; i<sizeof(unknownCommand); i++)
	{
		if (sendChar(unknownCommand[i]) == false)
		{
			emit message( QString("ERROR sending byte no. %1.").arg(i+1) );

			// emitting a signal to other modules which don't get the return value but need to know that we have a sensor error here. e.g. obstacle check thread.
			emit systemerror(-1);

			return -1;
		}
	}
	emit message("OKAY");


	// Reading answer, 4 byte (00 00 00 00)
	emit message("Receiving answer...");
	for (i=0; i<4; i++)
	{
		if (receiveChar(&answer) == true)
		{
			// emit message(QString("Received byte: 0x%1").arg(answer, 2, 16, QLatin1Char('0')));

			if (answer != 0)
			{
				emit message(QString("ERROR: answer byte no. %1 was 0x%2 instead 0x00").arg(i+1).arg(answer, 2, 16, QLatin1Char('0')));

				// emitting a signal to other modules which don't get the return value but need to know that we have a sensor error here. e.g. obstacle check thread.
				emit systemerror(-1);

				return -1;
			}
		}
		else
		{
			// error
			emit message(QString("ERROR receiving 00 00 00 00 answer at byte no. %1").arg(i+1));

			// emitting a signal to other modules which don't get the return value but need to know that we have a sensor error here. e.g. obstacle check thread.
			emit systemerror(-1);

			return -1;
		}
	}
	emit message("OKAY");


	// reading repeated header, 6 byte (0B 00 00 7B FF 07)
	emit message("Reading repeated header...");
	for (i=0; i<6; i++)
	{
		if (receiveChar(&answer) == true)
		{
			// emit message(QString("Received byte: 0x%1").arg(answer, 2, 16, QLatin1Char('0')));

/*
			if (answer != 0)
			{
				emit message(QString("ERROR: answer byte no. %1 was not XXXX").arg(i+1)); // FIXME: check the repeated header

				// emitting a signal to other modules which don't get the return value but need to know that we have a sensor error here. e.g. obstacle check thread.
				emit systemerror(-1);

				return -1;
			}
*/
		}
		else
		{
			// error
			emit message(QString("ERROR receiving repeated header at byte no. %1").arg(i+1));

			// emitting a signal to other modules which don't get the return value but need to know that we have a sensor error here. e.g. obstacle check thread.
			emit systemerror(-1);

			return -1;
		}
	}
	emit message("OKAY");


	// Reading resulting data (don't know, what they mead!)
	emit message("Now reading resulting unknown data...");
	for (i=0; i<236; i++) // why 240 steps?
	{
		if (receiveChar(&answer) == true)
		{
			// emit message( QString("Received byte no. %1: 0x%2").arg(i+1).arg(answer, 2, 16, QLatin1Char('0')) );
			// store the data temporary because we get them separated by LowByte and HighByte
			// unknownData[i] = answer;
		}
		else
		{
			// error
			emit message(QString("ERROR receiving scan data at byte no. %1").arg(i+1));

			// emitting a signal to other modules which don't get the return value but need to know that we have a sensor error here. e.g. obstacle check thread.
			emit systemerror(-1);

			return -1;
		}
	}
	emit message("OKAY");


	// Reading CRC, 4 bytes
	emit message("Reading CRC...");
	for (i=0; i<4; i++)
	{
		if (receiveChar(&answer) == true)
		{
			// emit message(QString("Received byte: 0x%1").arg(answer, 2, 16, QLatin1Char('0')));

/*
			// FIXME: check CRC !!
			if (answer != 0)
			{
				emit message(QString("ERROR: answer byte no. %1 was not 0x00").arg(i+1));

				// emitting a signal to other modules which don't get the return value but need to know that we have a sensor error here. e.g. obstacle check thread.
				emit systemerror(-1);

				return -1;
			}
*/
		}
		else
		{
			// error
			emit message(QString("ERROR receiving CRC at byte no. %1").arg(i+1));

			// emitting a signal to other modules which don't get the return value but need to know that we have a sensor error here. e.g. obstacle check thread.
			emit systemerror(-1);

			return -1;
		}
	}
	emit message("OKAY");

/*
	// convert data from 2 x 16 Bit to one 16 Bit value
	// and RETURN the data
	for (i=0; i<LASERSAMPLES; i++)
	{
		if (angle < 270.0)
		{
			data[i] = (float) ( ((unknownData[2*i+1] & 0x1f)<<8) | unknownData[2*i] );
			// qDebug("Copied: no. %d from %d values at %f�. Distance %f cm", i, LASERSAMPLES, angle, distances[i]);
			// emit message( QString("Measured distance at angle %1: %2 cm.").arg(angle, 4, 'f', 1).arg(scanResult[i]) );
			angle += 0.5;
		}
	}
*/
//	qDebug("S300 scan end @ %d:%d:%d-%d", x.currentTime().hour(), x.currentTime().minute(), x.currentTime().second(), x.currentTime().msec());

	return 0;
}


float SickS300::getDistance(int angleIndex)
{
	if ( (angleIndex<0) || (angleIndex>(270*2)) )
	{
		emit message(QString("ERROR: angle index with %1 out of range (0 - 549) (SickS300::getDistance)").arg(angleIndex));
		return 0;
	}

	// here we convert from cm to m (meters)!!
	return (distances[angleIndex] / 100);
}
