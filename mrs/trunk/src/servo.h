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
		Sets the servos maximum start, end and its default position.
		@param servo is the servo number.
		@param type can be SVSTART, SVEND or SVDEFAULT
		@param position is the position (0 - 255).
		 */
		void setServoPosition(int servo, unsigned char type, unsigned char position);
		
		/**
		Moves all servos into their default positions.
		 */
		void init(void);


	public slots:
		/**
		Moves a servo.
		@param servo is the servo number.
		@param position is the position (0 - 255).
		 */
		void moveServo(unsigned char servo, unsigned char position);


	signals:
		/**
		Emits a message to be displayed in the GUI or be used somewhere else (e.g. error message).
		*/
		void message(QString text);


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
		
		//! the servo numbers
		static const unsigned char SERVO1 = 1;
		static const unsigned char SERVO2 = 2;
		static const unsigned char SERVO3 = 3;
		static const unsigned char SERVO4 = 4;
		static const unsigned char SERVO5 = 5;
		static const unsigned char SERVO6 = 6;

		//! the command for the microcontroller
		static const unsigned char SET_SERVO1 = 42;
		static const unsigned char SET_SERVO2 = 43;
		static const unsigned char SET_SERVO3 = 44;
		static const unsigned char SET_SERVO4 = 45;
		static const unsigned char SET_SERVO5 = 46;
		static const unsigned char SET_SERVO6 = 47;
};

#endif
