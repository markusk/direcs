#ifndef MAIN_H
#define MAIN_H

#ifndef F_CPU
#define F_CPU 		16000000
#endif

//
// the "serial" commands for the MC
//
#define READ_SENSOR_1			1
#define READ_SENSOR_2			2
#define READ_SENSOR_3			3
#define READ_SENSOR_4			4
#define READ_SENSOR_5			5
#define READ_SENSOR_6			6
#define READ_SENSOR_7			7
#define READ_SENSOR_8			8
#define READ_SENSOR_16			16

#define	MOTOR1_OFF				20
#define	MOTOR1_CLOCKWISE		21
#define	MOTOR1_COUNTERCLOCKWISE	22
#define	MOTOR2_OFF				23
#define	MOTOR2_CLOCKWISE		24
#define	MOTOR2_COUNTERCLOCKWISE	25

#define MOTOR1_SPEED_SET		26
#define MOTOR2_SPEED_SET		27

#define READ_MOTOR_SENSOR1		28
#define READ_MOTOR_SENSOR2		29

#define READ_MOTOR_DISTANCE1	30
#define READ_MOTOR_DISTANCE2	31

#define RESET_MOTOR_DISTANCE1	32
#define RESET_MOTOR_DISTANCE2	33

#define	FLASHLIGHT_OFF			40
#define	FLASHLIGHT_ON			41

#define SET_SERVO1				42
#define SET_SERVO2				43
#define SET_SERVO3				44
#define SET_SERVO4				45
#define SET_SERVO5				46
#define SET_SERVO6				47

#define MOTOR3_OFF 				50
#define MOTOR3_CLOCKWISE 		51
#define MOTOR3_COUNTERCLOCKWISE 52
#define MOTOR4_OFF 				53
#define MOTOR4_CLOCKWISE 		54
#define MOTOR4_COUNTERCLOCKWISE 55

#define MOTOR3_SPEED_SET		56
#define MOTOR4_SPEED_SET		57

#define READ_CONTACT1			70
#define READ_CONTACT2			71
#define READ_CONTACT3			72
#define READ_CONTACT4			73

#define	INIT					250


// these numbers represent the Ports of the A/D converter C  !!!
// for the infrared sensors and the motor sensors
#define SENSOR1			0  // ADC0
#define SENSOR2			1  // ADC1
#define SENSOR3			2  // ADC2
#define SENSOR4			3  // ADC3
#define SENSOR5			4  // ADC4
#define SENSOR6			5  // ADC5
#define SENSOR7			6  // ADC6
#define SENSOR8			7  // ADC7

#define SENSORMOTOR1	38 // ADC14
#define SENSORMOTOR2	39 // ADC15


#include <avr/io.h>
//#include <avr/interrupt.h> // (no longer needed since avr-libc 1.4)
// #include <signal.h> ERROR?

#include <inttypes.h>
#include <avr/interrupt.h>
#include <avr/signal.h>

#include <stdlib.h> // für utoa und itoa
#include <avr/delay.h>

#include "usart.h"
#include "adconv.h"
#include "ultra.h"
#include "pwm.h"
#include "servo.h"


//
// the prototypes
//
int main(void);


#endif
