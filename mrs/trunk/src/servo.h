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
		
		/**
		Sets the start position of a servo.
		@param servo is the servo number.
		@param type can be SVSTART, SVEND or SVDEFAULT
		@param position is the position (0 - 255).
		 */
		void setServoPosition(int servo, unsigned char type, unsigned char position);


	public slots:
		/**
		Moves a servo.
		@param servo is the servo number.
		@param position is the position (0 - 255).
		 */
		void moveServo(unsigned char servo, unsigned char position);


	private:
		InterfaceAvr *interface1;
		
		//! defines the size of the servo[] arrays.
		static const unsigned char NUMBEROFSERVOS = 6; // TODO: also defined in mrs.h !!!
		int servoStartPosition[NUMBEROFSERVOS];
		int servoEndPosition[NUMBEROFSERVOS];
		int servoDefaultPosition[NUMBEROFSERVOS];
		
		//! the possible servo data
		static const unsigned char SVSTART   = 0;
		static const unsigned char SVEND     = 1;
		static const unsigned char SVDEFAULT = 2;
		
		static const unsigned char SERVO1 = 10; // Servo 1
		static const unsigned char SERVO2 = 20; // Servo 2

		//! the command for the microcontroller
		static const unsigned char SET_SERVO1 = 45;
		static const unsigned char SET_SERVO2 = 46;
};

#endif
