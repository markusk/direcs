#ifndef INTERFACEAVR_H
#define INTERFACEAVR_H

//-------------------------------------------------------------------
#include "qextserialport.h"
#include <QObject>
//-------------------------------------------------------------------

// forward declarations because of circular includes!
class QextSerialPort;


class InterfaceAvr : public QObject // FixMe: error when compiling?!?
{
    Q_OBJECT
	
	public:
		InterfaceAvr();
		~InterfaceAvr();
		bool openComPort(QString comPort);
		void closeComPort();
		bool sendChar(char character);
		bool receiveChar(char *character);
		bool receiveInt(int *value);
	
	signals:
		void tooMuchErrors();

	private:
		QextSerialPort *serialPort;
};

#endif
