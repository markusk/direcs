#ifndef HEAD_H
#define HEAD_H

//-------------------------------------------------------------------
#include "servo.h"
//-------------------------------------------------------------------

/**
\brief Gives access to the robots head.

The Head class gives access to the head of the robot. The head may turn right, up, down, smile, look left ... (to be defined/implemented).
*/
class Head : public QObject
{
    Q_OBJECT

	public:
		Head(Servo *s);
		~Head();
		
		/**
		@return The speed of a motor.
		@param motor is the motor number.
		*/
		int getMotorSpeed(int motor);

		/**
		@return The driven distance in cm made of wheel1.
		*/
		// TODO: change to one method?!?
		// FIXME: check the conversion value and make it a const!
		double getDrivenDistance1();
		
		/**
		@return The driven distance in cm made of wheel2.
		*/
		// TODO: change to one method?!?
		// FIXME: check the conversion value and make it a const!
		double getDrivenDistance2();
		
		/**
		Resets the stored driven distances and made revolutions in this class and in the microcontroller.
		@param motor is the motor number.
		*/
		void resetMovementCounter(short int motor);
		

	public slots:
		/**
		bla bla bla
		*/ 
		void look(QString direction);
		
		/**
		Sets the speed of a motor.
		@param motor is the motor number.
		@param speed is the speed (0 - 255).
		 */
		void setMotorSpeed(int motor, int speed);
		
		
	private:
		void eye(unsigned char whichEye, QString direction);
		// FIXME: check the conversion value and make it a const!
		void calculateMovement();

		Servo *servos;
		int motor1Speed;
		int motor2Speed;
		int motor3Speed;
		int motor4Speed;

		//unsigned int steps1;
		//unsigned int steps2;
		unsigned int revolutions1;
		unsigned int revolutions2;
		double drivenDistance1;
		double drivenDistance2;
		
		static const char ON  = true;   /** For motor "ON" */
		static const char OFF = false;  /** For motor "OFF" */
		static const char CLOCK = 1;  /** For stepper motor steps (clock) */

		static const unsigned char HIGH = 1;  // For set bit to 1
		static const unsigned char LOW  = 0;  // For set bit to 0

		static const unsigned char BIT0 = 1;
		static const unsigned char BIT1 = 2;
		static const unsigned char BIT2 = 4;
		static const unsigned char BIT3 = 8;
		static const unsigned char BIT4 = 16;
		static const unsigned char BIT5 = 32;
		static const unsigned char BIT6 = 64;
		static const unsigned char BIT7 = 128;

		static const unsigned char CLOCKWISE        = 0;  // Motor direction "CLOCKWISE"
		static const unsigned char COUNTERCLOCKWISE = 1;  // Motor direction "COUNTERCLOCKWISE"
		static const unsigned char SAME             = 3;  // Motor direction/power "same like before"
		static const unsigned char MOTOR1           = 10; //! Motor 1 (drive)
		static const unsigned char MOTOR2           = 20; //! Motor 2 (drive)
		static const unsigned char MOTOR3           = 30; //! Motor 3 (camera pan)
		static const unsigned char MOTOR4           = 40; //! Motor 4 (camera pan)

		// The bits for controling the USB-Circuit
		static const unsigned char MOTOR1A = BIT0;
		static const unsigned char MOTOR1B = BIT1;
		static const unsigned char MOTOR2A = BIT2;
		static const unsigned char MOTOR2B = BIT3;
		static const unsigned char MOTORCLOCK = BIT4;
		
		// the "serial" commands for the MC -> see "main.h" in the microcontroller source code( mrs avr)!
		static const unsigned char MOTOR1_OFF 				= 20;
		static const unsigned char MOTOR1_CLOCKWISE 		= 21;
		static const unsigned char MOTOR1_COUNTERCLOCKWISE 	= 22;
		static const unsigned char MOTOR2_OFF 				= 23;
		static const unsigned char MOTOR2_CLOCKWISE 		= 24;
		static const unsigned char MOTOR2_COUNTERCLOCKWISE 	= 25;

		static const unsigned char MOTOR1_SPEED_SET			= 26;
		static const unsigned char MOTOR2_SPEED_SET			= 27;

		static const unsigned char MOTOR3_OFF 				= 50;
		static const unsigned char MOTOR3_CLOCKWISE 		= 51;
		static const unsigned char MOTOR3_COUNTERCLOCKWISE 	= 52;
		static const unsigned char MOTOR4_OFF 				= 53;
		static const unsigned char MOTOR4_CLOCKWISE 		= 54;
		static const unsigned char MOTOR4_COUNTERCLOCKWISE 	= 55;

		static const unsigned char MOTOR3_SPEED_SET			= 56;
		static const unsigned char MOTOR4_SPEED_SET			= 57;

		//! the possible eyes
		static const unsigned char LEFTEYE  = 1;
		static const unsigned char RIGHTEYE = 2;

		//! the possible servo data
		static const unsigned char SVSTART   = 0;
		static const unsigned char SVEND     = 1;
		static const unsigned char SVDEFAULT = 2;
		static const unsigned char SVCURRENT = 3;
		
		//! the servo numbers
		static const unsigned char SERVO1 = 0;
		static const unsigned char SERVO2 = 1;
		static const unsigned char SERVO3 = 2;
		static const unsigned char SERVO4 = 3;
		static const unsigned char SERVO5 = 4;
		static const unsigned char SERVO6 = 5;
};

#endif
