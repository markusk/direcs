#ifndef MOTOR_H
#define MOTOR_H

//-------------------------------------------------------------------
#include "interface.h"
//-------------------------------------------------------------------
#include <QtGui>
#include <QCoreApplication>
//-------------------------------------------------------------------


class Motor : public QObject
{
    Q_OBJECT

	public:
		Motor(InterfaceAvr *i);
		~Motor();
		void motorControl(unsigned char motor, unsigned char power, unsigned char direction);
		void setMotorSpeed(int motor, int speed);
		int getMotorSpeed(int motor);

		//bool isMotorActive();
		//unsigned int getSteps1();
		//unsigned int getSteps2();
		unsigned int getRevolutions1();
		unsigned int getRevolutions2();
		double getDrivenDistance1();
		double getDrivenDistance2();
		void resetMovementCounter(short int motor);
		void flashlight(unsigned char state);


	private:
		void calculateMovement();

		InterfaceAvr *interface1;
		int motor1Speed;
		int motor2Speed;

		//unsigned int steps1;
		//unsigned int steps2;
		unsigned int revolutions1;
		unsigned int revolutions2;
		double drivenDistance1;
		double drivenDistance2;
		
		static const unsigned char ON  = 1;  // For motor "ON"
		static const unsigned char OFF = 0;  // For motor "OFF"

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
		static const unsigned char MOTOR1           = 10; // Motor 1
		static const unsigned char MOTOR2           = 20; // Motor 2

		// The bits for controling the USB-Circuit
		static const unsigned char MOTOR1A = BIT0;
		static const unsigned char MOTOR1B = BIT1;
		static const unsigned char MOTOR2A = BIT2;
		static const unsigned char MOTOR2B = BIT3;
		static const unsigned char MOTORCLOCK = BIT4;
		
		// the "serial" commands for the MC -> see "mrs-ser.h" in the microcontroller source code!
		static const unsigned char MOTOR1_OFF 			= 20;
		static const unsigned char MOTOR1_CLOCKWISE 		= 21;
		static const unsigned char MOTOR1_COUNTERCLOCKWISE 	= 22;
		static const unsigned char MOTOR2_OFF 			= 23;
		static const unsigned char MOTOR2_CLOCKWISE 		= 24;
		static const unsigned char MOTOR2_COUNTERCLOCKWISE 	= 25;

		static const unsigned char MOTOR1_SPEED_SET		= 26;
		static const unsigned char MOTOR2_SPEED_SET		= 27;
		
		static const unsigned char FLASHLIGHT_OFF 		= 40;
		static const unsigned char FLASHLIGHT_ON 		= 41;

};

#endif
