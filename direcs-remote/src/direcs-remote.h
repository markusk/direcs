#ifndef DIRECSREMOTE_H
#define DIRECSREMOTE_H

//-------------------------------------------------------------------
#include "gui.h"
#include "plotThread.h"

#include <QtGui>
#include <QtNetwork>
//-------------------------------------------------------------------

// forward declarations because of circular includes!
class Gui;
class QUdpSocket;


class DirecsRemote : public QObject
{
    Q_OBJECT


	public:
		DirecsRemote();
		~DirecsRemote();
		void testPort(int function);
	
	
	public slots:
		void sendNetworkCommand(QString command);
	
	
	private slots:
		/**
		This slot is automatically called by the framework, in the case of pending datagrams.
		*/
		void processPendingDatagrams();

	
	signals:
		/**
		Emits values read from the network to show them in the gui
		*/
		void showMotorCurrent(int motor, int value);


	private:
		/**
		Parses the network string for sensor values and emits them to the gui.
		*/
		void parseNetworkString(QString text);
		
		Gui *gui;
		QUdpSocket *udpSocket;
		QUdpSocket *udpSocketReceiver;

		// Give the infrared sensors some names
		//
		// DONT CHANGE THIS NUMBERS!
		// THEY ARE ALSO USED TO ADRESS THE ARRAY "iRSensorValue[]" !!
		//
		static const short int SENSOR1 = 1;
		static const short int SENSOR2 = 2;
		static const short int SENSOR3 = 4;
		static const short int SENSOR4 = 8;
		static const short int SENSOR5 = 16;
		static const short int SENSOR6 = 32;
		static const short int SENSOR7 = 64;
		static const short int SENSOR8 = 128;
		
		// ultrasonic sensor
		static const short int SENSOR16 = 256;
		
		static const short int MOTORSENSOR1 = 0;
		static const short int MOTORSENSOR2 = 1;
		static const short int MOTORSENSOR3 = 2;
		static const short int MOTORSENSOR4 = 3;
		
		/**
		 Give the contacts for the pan tilt cam end positions some nice names
		 */
		static const unsigned char CONTACT1 = 0;
		static const unsigned char CONTACT2 = 1;
		static const unsigned char CONTACT3 = 2;
		static const unsigned char CONTACT4 = 3;
};
#endif
