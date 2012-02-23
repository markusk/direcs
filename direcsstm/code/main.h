#ifndef MAIN_H
#define MAIN_H

#include "stm32f4xx_conf.h"


// these numbers represent the Ports of the A/D converter C  !!!
// for the infrared sensors and the motor sensors
#define SENSOR1					0  // ADC0
#define SENSOR2					1  // ADC1
#define SENSOR3					2  // ADC2
#define SENSOR4					3  //
#define SENSOR5					4  // ADC4
#define SENSOR6					5  // ADC5
#define SENSOR7					6  // ADC6
#define SENSOR8					7  // ADC7

#define SENSORMOTOR1			38 // ADC14
#define SENSORMOTOR2			39 // ADC15

#define FORWARD 				10 /// Motor direction (formerly "clockwise")
#define BACKWARD 				20 /// Motor direction (formerly "counterclockwise"
#define LEFT					30
#define RIGHT					40
#define TURNLEFT				50
#define TURNRIGHT				60
#define START					70
#define STOP					80
#define WAIT					90
// #define DIAGONAL_FORWARD_LEFT
// #define DIAGONAL_FORWARD_RIGHT
// #define DIAGONAL_BACKWARD_LEFT
// #define DIAGONAL_BACKWARD_RIGHT

#define SAME 					200 /// Motor direction/power "same like before"
#define MOTOR1    				210 /// Motor 1 front left
#define MOTOR2    				220 /// Motor 2 front right
#define MOTOR3    				230 /// Motor 3 back left
#define MOTOR4    				240 /// Motor 4 back right
#define ALLMOTORS 				250 /// used for letting the robot getting only one command for all motors (like 'forward all').

#define ON						1
#define OFF						0


// The port bits -------------------------------------------------------------
#define LEDPORT					GPIOD
#define LEDGREEN				GPIO_Pin_12
#define LEDORANGE				GPIO_Pin_13
#define LEDRED					GPIO_Pin_14
#define LEDBLUE					GPIO_Pin_15
#define LEDCLOCK				RCC_AHB1Periph_GPIOD

#define MOTORPORT				GPIOB
#define MOTOR1A					GPIO_Pin_0
#define MOTOR1B					GPIO_Pin_1
#define MOTOR2A					GPIO_Pin_2
#define MOTOR2B					GPIO_Pin_4
#define MOTOR3A					GPIO_Pin_5
#define MOTOR3B					GPIO_Pin_7
#define MOTOR4A					GPIO_Pin_8
#define MOTOR4B					GPIO_Pin_14
#define MOTORCLOCK				RCC_AHB1Periph_GPIOB

// TIM4, PB7, Channel 2 for motor speed (PWM)
#define MOTORPWMPORT			GPIOB
#define MOTORPWMBIT				GPIO_Pin_7
#define MOTORPWMTIMBIT			GPIO_PinSource7
#define MOTORPWMPORTCLOCK		RCC_AHB1Periph_GPIOE
#define MOTORPWMTIMER			TIM1
#define	MOTORPWMCHANNEL			2
#define MOTORPWMTIMCLOCK		RCC_APB1Periph_TIM1
#define MOTORPWMAF 				GPIO_AF_TIM1

// ADC and DMA for battery / voltage sensors
#define SENSORPORT				GPIOC
#define SENSORPIN24VOLT			GPIO_Pin_1
#define SENSORPIN12VOLT			GPIO_Pin_2
#define SENSORPORTCLOCK			RCC_AHB1Periph_GPIOC
#define SENSORDMACHANNEL		DMA_Channel_2;
#define SENSORDMASTREAM			DMA2_Stream0
#define SENSORDMACLOCK			RCC_AHB1Periph_DMA2
#define SENSORADC 				ADC3
#define	SENSORCHANNEL24VOLT		ADC_Channel_11
#define	SENSORCHANNEL12VOLT		ADC_Channel_12
#define SENSORADCCLOCK			RCC_APB2Periph_ADC3

#define NUMBEROFADCHANNELS		2

// ADC DMA stuff
#define ADC3_DR_ADDRESS			0x4001224C
// 0x4001224C = 0x40012000 + 0x200 for ADC3 + 0x4C for ADC_DR) see p. 51, 247 and 248 in the Referance Manual for STM32F4

#endif