/*************************************************************************
 *   Copyright (C) 2011 by Markus Knapp                                  *
 *   www.direcs.de                                                       *
 *                                                                       *
 *   This file is part of direcs.                                        *
 *                                                                       *
 *   direcs is free software: you can redistribute it and/or modify it   *
 *   under the terms of the GNU General Public License as published      *
 *   by the Free Software Foundation, version 3 of the License.          *
 *                                                                       *
 *   direcs is distributed in the hope that it will be useful,           *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of      *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the        *
 *   GNU General Public License for more details.                        *
 *                                                                       *
 *   You should have received a copy of the GNU General Public License   *
 *   along with direcs. If not, see <http://www.gnu.org/licenses/>.      *
 *                                                                       *
 *************************************************************************/

#ifndef MOTOR_H
#define MOTOR_H

//-------------------------------------------------------------------
#include "interfaceAvr.h"
//-------------------------------------------------------------------
#include <QCoreApplication>
#include <QMutex>
#include <QTime>
#include <QTimer>
//-------------------------------------------------------------------

/**
\author Markus Knapp
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
		@return The speed of a motor.
		@param motor is the motor number.
		*/
		int getMotorSpeed(int motor);

		//bool isMotorActive();
		//unsigned int getSteps1();
		//unsigned int getSteps2();

		/**
		@param motor is the motor number.
		@return The revolutions made of a motor (stored in the microcontroller till power off).
		*/
		unsigned int getRevolutions(unsigned char motor);

		/**
		Returns the driven distance, if the motor is a stepper motor!
		@param motor is the motor number.
		@return The driven distance in cm made of a whee1.
		*/
		double getDrivenDistance(unsigned char motor);

		/**
		Resets the stored driven distances and made revolutions in this class and in the microcontroller.
		@param motor is the motor number.
		*/
		void resetMovementCounter(short int motor);

		/*
		Generates steps for stepper motors.
		@param steps are the number of steps.
		void makeSteps(int steps);

		void parkStepper(unsigned char motor);
		*/

	public slots:
		/**
		Controls a motor (normal and stepper!).
		@param motor is the motor number.
		@param power can be ON or OFF.
		@param direction can be CLOCKWISE or COUNTERCLOCKWISE.
		@return true on access (answer from Atmel was okay)

		@sa makeSteps() for generating steps for stepper motors
		*/
		bool motorControl(int motor, bool power, int direction);

		/**
		Turns the flashlight ON or OFF.
		@param light can be ON or OFF.
		@todo put this somewhere else...
		*/
		void flashlight(bool light);

		/**
		This slot takes the received answers from the Atmel after sending a command string to it.
		@param atmelAnswer contains the answer
		@param regardingCommand contains the command which was used in the Slot @sa InterfaceAvr::commandCompleted()
		*/
		void takeCommandAnswer(QString atmelAnswer, QString regardingCommand);

		/**
		Sets the speed of a motor.
		@param motor is the motor number (MOTOR1, MOTOR2, MOTOR3, MOTOR4, ALLMOTORS).
		@param speed is the speed (0 - 255).
		return true on success
		 */
		bool setMotorSpeed(int motor, int speed);

		/**
		Sets the maximum speed of alls motors (the robot).
		@param speed is the speed (0 - 255).
		 */
		void setMaximumSpeed(int speed);

		/**
		This slots takes the robot (circuit) state, to know if the robot is ON or OFF.
		When the class knows this, unnecessary communication with the interface can be avoided.

		@param state can be ON or OFF
		 */
		void setRobotState(bool state);


	private slots:
		/**
		This Slot is called if we never get an answer from the Atmel
		*/
		void timeout();


	signals:

		/**
		Sends a string to the GUI log.
		@param text is the message to be emitted
		*/
		void message(QString text);


	private:
		void calculateMovement(); /// \todo check the conversion value and make it a const!

		QString className;	/// this will contain the name of this class at runtime. @sa takeCommandAnswer()
		mutable QMutex *mutex; // make this class thread-safe
		InterfaceAvr *interface1;
		bool robotState; // stores the robot state within this class
		int motor1Speed;
		int motor2Speed;
		int motor3Speed;
		int motor4Speed;
		int motorSpeedAllMotors; // this is the speed for all motors together, when set in one command

		//unsigned int steps1;
		//unsigned int steps2;
		unsigned int revolutions1;
		unsigned int revolutions2;
		unsigned int revolutions3;
		unsigned int revolutions4;
		double drivenDistance1;
		double drivenDistance2;
		double drivenDistance3;
		double drivenDistance4;


//		bool circuitState; // stores the robot state within this class
//		bool firstInitDone;
//		bool compassCircuitState; // stores the state of the compass module within this class
		bool commandExecutedSuccessfull; /// set to true, if command executed successfull. In this case a later timeout slot will check this first!

		QString commandFlashlightOn;	/// *f0on#
		QString commandFlashlightOff;	/// *f0of#
		QString commandDriveForward;	/// *bdf#
		QString commandDriveBackward;	/// *bdb#
		QString commandDriveLeft;		/// *bdl#
		QString commandDriveRight;		/// *bdr#
		QString commandTurnLeft;		/// *btl#
		QString commandTurnRight;		/// *btr#
		QString commandBotGo;			/// *bgo#
		QString commandBotStop;			/// *bst#
		QString commandBotWait;			/// *bwa#

		QString atmelCommand; /// this is the command for the Atmel
		QString expectedAtmelAnswer; /// this stores the answer which the Atmel should Answer from the last command he got.
		bool answerTimeout; /// this is set to true, when we have a timout while waiting for an Atmel answer

		QTime duration; /// for measuring between sending an command to Atmel and the time it needs till the Atmel answers
		static const int ATMELTIMEOUT = 500; /// timeout in ms


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

		/// Some driving directions *and* motor directions for the robot. @sa Direcs::drive() [Slot]
		static const int FORWARD    = 10; /// Motor direction (formerly "clockwise")
		static const int BACKWARD   = 20; /// Motor direction (formerly "counterclockwise"
		static const int LEFT		= 30;
		static const int RIGHT		= 40;
		static const int TURNLEFT	= 50;
		static const int TURNRIGHT	= 60;
		static const int START		= 70;
		static const int STOP		= 80;
		static const int WAIT		= 90;
		// static const int DIAGONAL_FORWARD_LEFT
		// static const int DIAGONAL_FORWARD_RIGHT
		// static const int DIAGONAL_BACKWARD_LEFT
		// static const int DIAGONAL_BACKWARD_RIGHT

		static const int SAME      = 200; /// Motor direction/power "same like before"
		static const int MOTOR1    = 210; /// Motor 1 front left
		static const int MOTOR2    = 220; /// Motor 2 front right
		static const int MOTOR3    = 230; /// Motor 3 back left
		static const int MOTOR4    = 240; /// Motor 4 back right
		static const int ALLMOTORS = 250; /// used for letting the robot getting only one command for all motors (like 'forward all'). This is to reduce commands on the serial line.

		static const int MOTOR1FW	= 300;
		static const int MOTOR1BW	= 310;
		static const int MOTOR1OFF	= 320;
		static const int MOTOR2FW	= 330;
		static const int MOTOR2BW	= 340;
		static const int MOTOR2OFF	= 350;
		static const int MOTOR3FW	= 360;
		static const int MOTOR3BW	= 370;
		static const int MOTOR3OFF	= 380;
		static const int MOTOR4FW	= 390;
		static const int MOTOR4BW	= 400;
		static const int MOTOR4OFF	= 410;

		static const int READ_AXIS_X = 500;
		static const int READ_AXIS_Y = 510;
		static const int READ_AXIS_Z = 520;

		//
		/// The bits for controling the USB-Circuit
		//
		static const unsigned char MOTOR1A = BIT0;
		static const unsigned char MOTOR1B = BIT1;
		static const unsigned char MOTOR2A = BIT2;
		static const unsigned char MOTOR2B = BIT3;
		static const unsigned char MOTORCLOCK = BIT4;

		/// the "serial" commands for the MC -> see "main.h" in the microcontroller source code (direcs-avr)!
		static const unsigned char MOTOR1_OFF 				= 20;
		static const unsigned char MOTOR1_CLOCKWISE 		= 21;
		static const unsigned char MOTOR1_COUNTERCLOCKWISE = 22;
		static const unsigned char MOTOR2_OFF 				= 23;
		static const unsigned char MOTOR2_CLOCKWISE 		= 24;
		static const unsigned char MOTOR2_COUNTERCLOCKWISE = 25;

		static const unsigned char MOTOR1_SPEED_SET		= 26;
		static const unsigned char MOTOR2_SPEED_SET		= 27;

		static const unsigned char MOTOR3_OFF 				= 50;
		static const unsigned char MOTOR3_CLOCKWISE 		= 51;
		static const unsigned char MOTOR3_COUNTERCLOCKWISE = 52;
		static const unsigned char MOTOR4_OFF 				= 53;
		static const unsigned char MOTOR4_CLOCKWISE 		= 54;
		static const unsigned char MOTOR4_COUNTERCLOCKWISE = 55;

		static const unsigned char MOTOR3_SPEED_SET		= 56;
		static const unsigned char MOTOR4_SPEED_SET		= 57;

		static const unsigned char SPEED_SET_ALLMOTORS		= 60;

		/// The serial driving commands for the robot
		static const unsigned char BOTFORWARD				= 80;
		static const unsigned char BOTBACKWARD				= 81;
		static const unsigned char BOTLEFT					= 82;
		static const unsigned char BOTRIGHT					= 83;
		static const unsigned char BOTTURNLEFT				= 84;
		static const unsigned char BOTTURNRIGHT			= 85;
		static const unsigned char BOTSTART					= 86;
		static const unsigned char BOTSTOP					= 87;
		static const unsigned char BOTWAIT					= 88;
		// static const unsigned char BOTDIAGONAL_FORWARD_LEFT
		// static const unsigned char BOTDIAGONAL_FORWARD_RIGHT
		// static const unsigned char BOTDIAGONAL_BACKWARD_LEFT
		// static const unsigned char BOTDIAGONAL_BACKWARD_RIGHT

		static const unsigned char FLASHLIGHT_OFF 		= 40;
		static const unsigned char FLASHLIGHT_ON 		= 41;

};

#endif
