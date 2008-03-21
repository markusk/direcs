#ifndef MOTOR_H
#define MOTOR_H

//-------------------------------------------------------------------
#include "interfaceAvr.h"
//-------------------------------------------------------------------
#include <QtGui>
#include <QCoreApplication>
#include <QMutex>
//-------------------------------------------------------------------

/**
\brief Gives access to the motors of the robot.

The Motor class gives access to the motors of the robot.
*/
class Motor : public QObject
{
    Q_OBJECT

	public:
		Motor(InterfaceAvr *i, QMutex *m);
		~Motor();
		
		/**
		Controls a motor (normal and stepper!).
		@param motor is the motor number.
		@param power can be ON or OFF.
		@param direction can be CLOCKWISE or COUNTERCLOCKWISE.
		
		@sa makeSteps() for generating steps for stepper motors
		*/
		void motorControl(unsigned char motor, bool power, unsigned char direction);
		
		
		/**
		@return The speed of a motor.
		@param motor is the motor number.
		*/
		int getMotorSpeed(int motor);

		//bool isMotorActive();
		//unsigned int getSteps1();
		//unsigned int getSteps2();
		
		/**
		@return The revolutions made of a motor1 (stored in the microcontroller till power off).
		*/
		// TODO: change to one method?!?
		unsigned int getRevolutions1();
		
		/**
		@return The revolutions made of a motor2 (stored in the microcontroller till power off).
		*/
		// TODO: change to one method?!?
		unsigned int getRevolutions2();
		
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
		
		/**
		Turns the flashlight ON or OFF.
		@param state can be ON or OFF.
		*/
		// TODO: put it somewhere else...
		void flashlight(bool state);
		
		/*
		Generates steps for stepper motors.
		@param steps are the number of steps.
		void makeSteps(int steps);

		void parkStepper(unsigned char motor); // FIXME: park steppers uin a safe position!
		*/

	public slots:
		/**
		Sets the speed of a motor.
		@param motor is the motor number.
		@param speed is the speed (0 - 255).
		 */
		void setMotorSpeed(int motor, int speed);
		
		
	private:
		// FIXME: check the conversion value and make it a const!
		void calculateMovement();

		mutable QMutex *mutex; // make this class thread-safe
		InterfaceAvr *interface1;
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

		
		static const unsigned char FLASHLIGHT_OFF 		= 40;
		static const unsigned char FLASHLIGHT_ON 		= 41;

};

#endif
