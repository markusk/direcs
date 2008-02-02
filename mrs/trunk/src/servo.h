#ifndef SERVO_H
#define SERVO_H

//-------------------------------------------------------------------
#include "interfaceAvr.h"
//-------------------------------------------------------------------


/**
\brief This class controls connected servos
*/
class Servo : public QObject
{
    Q_OBJECT

	public:
		Servo(InterfaceAvr *i);
		~Servo();

		
	public slots:
		void setServoPosition(unsigned char servo, unsigned char position);

		
	private:
		InterfaceAvr *interface1;
		
		static const unsigned char SERVO1 = 10; // Servo 1
		static const unsigned char SERVO2 = 20; // Servo 2

		static const unsigned char SET_SERVO1 = 45;
		static const unsigned char SET_SERVO2 = 46;
};

#endif
