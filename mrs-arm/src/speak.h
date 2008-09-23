#ifndef SPEAK_H
#define SPEAK_H

#include <stdio.h>   /* Standard input/output definitions */
#include <string.h>  /* String function definitions */
#include <unistd.h>  /* UNIX standard function definitions */
#include <fcntl.h>   /* File control definitions */
#include <errno.h>   /* Error number definitions */
#include <termios.h> /* POSIX terminal control definitions */


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
		QextSerialPort *port;
		static const unsigned char FLASHLIGHT_OFF 	= 40;
		static const unsigned char FLASHLIGHT_ON 	= 41;
};


#endif
