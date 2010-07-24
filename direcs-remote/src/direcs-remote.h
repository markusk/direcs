#ifndef DIRECSREMOTE_H
#define DIRECSREMOTE_H

//-------------------------------------------------------------------
#include "gui.h"
#include "plotThread.h"
#include "laserThread.h"
#include "network.h"

#include <QtGui>
#include <QtNetwork>
#include <QDebug>
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

		/**
		Emits values read from the network to show them in the gui
		*/
		void showVoltage(int voltage, float value);

		/**
		Emits motor current values, read from the network, to the plotThread (which emits them to the gui)
		*/
		void plotValueReceived(int motor, int value);

		/**
		Emits laser scanner current values, read from the network, to the laserThread (which emits them to the gui)
		*/
		void laserValueReceived(int laserScanner, int angle, float value);

		/**
		This signal is emitted every TODO: seconds, when a specific value over the network was received.
		@sa Gui::setLEDHeartbeat()
		*/
		void heartbeat(unsigned char state);


	private:
		/**
		Parses the network string for sensor values and emits them to the gui.
		*/
		void parseNetworkString(QString text);

		/**
		Converts a read voltage value from a sensor to a value in Volt (V).
		@param voltage is the number of the voltage (0, 1, 2...)
		@param value is the value to be converted!
		@return The voltage in Volt.
		*/
		float convertToVolt(int voltage, int value);

		Gui *gui;
		PlotThread *plotThread;
		LaserThread *laserThread;
		Network *network;
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

		/**
		*/
		static const short int LASER1 = 1;
		static const short int LASER2 = 2;
		static const short int LASER3 = 4;
		static const short int LASER4 = 8;
		static const short int LASER5 = 16;

		static const short int VOLTAGESENSOR1 = 0;
		static const short int VOLTAGESENSOR2 = 1;

		/**
		Defines the conversion factor for the voltage sensors to convert the sensor value in a "real world" value.
		For example, a measured sensor value for a voltage sensor is 685, this divided by a conversion factor 57.0 results in 12 V.
		*/
		static const float CONVERSIONFACTORVOLTAGESENSOR1 = 57.000;
		static const float CONVERSIONFACTORVOLTAGESENSOR2 = 36.125;

		/**
		This is for the @sa heartbeat signal (the GUI LED colors)
		*/
		static const unsigned char LEDOFF	= 0;
		static const unsigned char RED		= 1;
		static const unsigned char GREEN	= 2;
};
#endif
