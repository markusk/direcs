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

#define MOTORPORT				GPIOE
#define MOTOR1BITA				GPIO_Pin_8
#define MOTOR1BITB				GPIO_Pin_9
#define MOTOR2BITA				GPIO_Pin_10
#define MOTOR2BITB				GPIO_Pin_11
#define MOTOR3BITA				GPIO_Pin_12
#define MOTOR3BITB				GPIO_Pin_13
#define MOTOR4BITA				GPIO_Pin_14
#define MOTOR4BITB				GPIO_Pin_15
#define MOTORCLOCK				RCC_AHB1Periph_GPIOE

// timer / PWM stuff for motor 1
#define MOTOR1PWMPORT			GPIOC
#define MOTOR1PWMPORTCLOCK		RCC_AHB1Periph_GPIOC
#define MOTOR1PWMBIT			GPIO_Pin_8
#define MOTOR1PWMTIMBIT			GPIO_PinSource8
#define MOTOR1PWMTIMER			TIM3
#define MOTOR1PWMTIMCLOCK		RCC_APB1Periph_TIM3
#define MOTOR1RCC_B1_OR_B2		1
#define MOTOR1PWMAF				GPIO_AF_TIM3
#define	MOTOR1PWMCHANNEL		3

// timer / PWM stuff for motor 2
#define MOTOR2PWMPORT			GPIOC
#define MOTOR2PWMPORTCLOCK		RCC_AHB1Periph_GPIOC
#define MOTOR2PWMBIT			GPIO_Pin_9
#define MOTOR2PWMTIMBIT			GPIO_PinSource9
#define MOTOR2PWMTIMER			TIM3
#define MOTOR2PWMTIMCLOCK		RCC_APB1Periph_TIM3
#define MOTOR2RCC_B1_OR_B2		1
#define MOTOR2PWMAF				GPIO_AF_TIM3
#define	MOTOR2PWMCHANNEL		4

// timer / PWM stuff for motor 3
#define MOTOR3PWMPORT			GPIOE
#define MOTOR3PWMPORTCLOCK		RCC_AHB1Periph_GPIOE
#define MOTOR3PWMBIT			GPIO_Pin_5
#define MOTOR3PWMTIMBIT			GPIO_PinSource5
#define MOTOR3PWMTIMER			TIM9
#define MOTOR3PWMTIMCLOCK		RCC_APB2Periph_TIM9
#define MOTOR3RCC_B1_OR_B2		2
#define MOTOR3PWMAF				GPIO_AF_TIM9
#define	MOTOR3PWMCHANNEL		1

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