#ifndef MAIN_H
#define MAIN_H

#include <stdlib.h>
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

// just nice to have
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
#define MOTOR1BITA				GPIO_Pin_0
#define MOTOR1BITB				GPIO_Pin_1
#define MOTOR2BITA				GPIO_Pin_2
#define MOTOR2BITB				GPIO_Pin_4
#define MOTOR3BITA				GPIO_Pin_5
#define MOTOR3BITB				GPIO_Pin_7
#define MOTOR4BITA				GPIO_Pin_8
#define MOTOR4BITB				GPIO_Pin_12
#define MOTORCLOCK				RCC_AHB1Periph_GPIOB

// TIM4, PB7, Channel 2 for motor speed (PWM)
#define MOTORPWMPORT			GPIOB
#define MOTORPWMBIT				GPIO_Pin_7
#define MOTORPWMTIMBIT			GPIO_PinSource7
#define MOTORPWMPORTCLOCK		RCC_AHB1Periph_GPIOB
#define MOTORPWMTIMER			TIM4
#define	MOTORPWMCHANNEL			2
#define MOTORPWMTIMCLOCK		RCC_APB1Periph_TIM4
#define MOTORPWMAF 				GPIO_AF_TIM4

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


// Private variables ---------------------------------------------------------
__IO uint16_t ADC3ConvertedValues[NUMBEROFADCHANNELS];
__IO uint32_t ADC3ConvertedVoltage = 0;

// stores the serial received command and the string which will be sent as an answer
char stringbuffer[64];
int i;


// the prototypes ------------------------------------------------------------
int main(void);

// init of the several system clocks
void clockInit();

// GPIOPort init
void gpioPortInit();

// Timer init for PWM
void timerInit(void);

// Timer speed / PWM duty cycle update
void timerUpdate(int speed);

// initialize DMA + ACD
void DMAACDinit(void);

// turns a LED on or off
void turnLED(int led, int state);

// brings the bot into my own reset state
void resetRobot(void);

// sends a u int value over the serial line as ASCII
void sendUInt(uint16_t value);

char* ltoa(long value, char* result);
int atoi(const char *s);

void delay(__IO uint32_t nCount);


#endif