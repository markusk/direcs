/***************************************************************************
 *   Copyright (C) 2008 by Markus Knapp                                    *
 *   mrs@direcs.de                                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#ifndef MRSARM_H
#define MRSARM_H

//#include <QCoreApplication> // The QCoreApplication class provides an event loop for console Qt applications
#include <qextserialport.h>


class QextSerialPort;


class Mrsarm : public QObject
{
	Q_OBJECT


	public:
		Mrsarm();
		~Mrsarm();
		void speak(QString textToSpeak, char volume=0, char pitch=7, char speed=2); /// volume 0-7 (0 is loudest)  pitch 0-7 (0 is highest)  speed 0-7 (0 is slowest)
		
//	public slots:
//		void sendNetworkCommand(QString command);


	private:
		bool openSerialPort();
		void closeSerialPort();
		void receiveSerialMsg();
		
		QString serialPort;
		QextSerialPort *port;
		static const unsigned char FLASHLIGHT_OFF 	= 40;
		static const unsigned char FLASHLIGHT_ON 	= 41;
};


#endif
