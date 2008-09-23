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
#ifndef SPEAK_H
#define SPEAK_H

// The QCoreApplication class provides an event loop for console Qt applications
#include <QCoreApplication>


class QextSerialPort;


class Speak : public QObject
{
	Q_OBJECT


	public:
		Speak();
		~Speak();
		
//	public slots:
//		void sendNetworkCommand(QString command);


	private:
		void receiveMsg();
		QextSerialPort *port;
		QString textToSpeak;
		QString serialPort;
		static const unsigned char FLASHLIGHT_OFF 	= 40;
		static const unsigned char FLASHLIGHT_ON 	= 41;
};


#endif
