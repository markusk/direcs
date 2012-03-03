/*************************************************************************
 *   Copyright (C) Markus Knapp                                          *
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

#include "main.h"
#include "stm32f4xx_conf.h"
#include <stdlib.h>


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


/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
	// init all system
	clockInit();

	// general port init (LED, motor bits etc.)
	gpioPortInit();

	// init USART (serial port)
	usartInit();
	
	// init timer for PWM
	timerInit();

	// init DMA
	DMAACDinit();

	// reset robot to a defined state
	resetRobot();

	while (1)
	{
//		serialBlockingReadString();

		uint16_t buchstabe = 0;


		// blocking read on serial port USART3
		while (USART_GetFlagStatus(USART3, USART_FLAG_RXNE) == RESET)
		{
		}

		buchstabe = USART_ReceiveData(USART3);
		
		// build string
		receiveChar(buchstabe);



		if (stringReceived())
		{
			// Get complete string and delete uart_rx_flag
			get_string(stringbuffer);


			// RESET / INIT
			if (strcmp(stringbuffer, "*re#") == 0)
			{
				resetRobot();
/*
				// flashlight off
				relais(OFF);
*/
				// green LED on. Know we know, that the program on the PC/Mac has initialised the Atmel
				turnLED(LEDGREEN, ON);
				
				// answer with "ok"
				// this answer is used to see if the robot is "on"
				put_string("*re#");
				
/*
				// e n a b l e  watchdog!
				watchdog(ENABLE);
*/
			}
			else
			// SLEEP (and turn off watchdog)
			if (strcmp(stringbuffer, "*sl#") == 0)
			{
				resetRobot();
/*	
				// flashlight off
				relais(OFF);
*/
				// green LED off
				turnLED(LEDGREEN, OFF);
/*
				// d i s a b l e  watchdog!
				watchdog(DISABLE);
*/				
				// answer
				put_string("*sl#");
			}
			else
			// READ_SENSOR_7 (24 V supply)
			if (strcmp(stringbuffer, "*s7#") == 0)
			{
				// convert the ADC value (from 0 to 0xFFF) to a voltage value (from 0V to 3.3V)
				// ADC3ConvertedVoltage = ADC3ConvertedValue * 3300 / 0xFFF;

				// read ADC and send answer over serial port
				// 0 - 4095d
				sendUInt( ADC3ConvertedValues[0] );
			}
			else
			// READ_SENSOR_8 (12 V supply)
			if (strcmp(stringbuffer, "*s8#") == 0)
			{
				// convert the ADC value (from 0 to 0xFFF) to a voltage value (from 0V to 3.3V)
				// ADC3ConvertedVoltage = ADC3ConvertedValue * 3300 / 0xFFF;

				// read ADC and send answer over serial port
				// 0 - 4095d
				sendUInt( ADC3ConvertedValues[1] );
			}
			else
			// MOTOR 1 OFF
			if (strcmp(stringbuffer, "*mp1of#") == 0)
			{
				GPIO_ResetBits(MOTORPORT, MOTOR1BITA | MOTOR1BITB);
				put_string("*mp1of#");
			}
			else
			// MOTOR 1 CLOCKWISE = forward
			if (strcmp(stringbuffer, "*md1cw#") == 0)
			{
				GPIO_ResetBits(MOTORPORT, MOTOR1BITA);
				GPIO_SetBits(MOTORPORT, MOTOR1BITB);
				put_string("*md1cw#");
			}
			else
			// MOTOR 1 COUNTERCLOCKWISE = backward
			if (strcmp(stringbuffer, "*md1cc#") == 0)
			{
				GPIO_SetBits(MOTORPORT, MOTOR1BITA);
				GPIO_ResetBits(MOTORPORT, MOTOR1BITB);
				put_string("*md1cc#");
			}
			else
			// MOTOR 1 SPEED SET
			if (strncmp(stringbuffer, "*mv1", 4) == 0)
			{
				// change first four chars for upcoming string conversion
				stringbuffer[0] = '0';
				stringbuffer[1] = '0';
				stringbuffer[2] = '0';
				stringbuffer[3] = '0';

				// convert to int
				i = atoi(stringbuffer);

				// set speed / PWM
				timerUpdate( i );

				// answer with "ok"
				put_string("*mv1#");
			}
			else
			// MOTOR 2 OFF
			if (strcmp(stringbuffer, "*mp2of#") == 0)
			{
				GPIO_ResetBits(MOTORPORT, MOTOR2BITA | MOTOR2BITB);
				put_string("*mp2of#");
			}
			else
			// MOTOR 2 CLOCKWISE = forward
			if (strcmp(stringbuffer, "*md2cw#") == 0)
			{
				GPIO_ResetBits(MOTORPORT, MOTOR2BITA);
				GPIO_SetBits(MOTORPORT, MOTOR2BITB);
				put_string("*md2cw#");
			}
			else
			// MOTOR 2 COUNTERCLOCKWISE = backward
			if (strcmp(stringbuffer, "*md2cc#") == 0)
			{
				GPIO_SetBits(MOTORPORT, MOTOR2BITA);
				GPIO_ResetBits(MOTORPORT, MOTOR2BITB);
				put_string("*md2cc#");
			}
			else
			// MOTOR 3 OFF
			if (strcmp(stringbuffer, "*mp3of#") == 0)
			{
				GPIO_ResetBits(MOTORPORT, MOTOR3BITA | MOTOR3BITB);
				put_string("*mp3of#");
			}
			else
			// MOTOR 3 CLOCKWISE = forward
			if (strcmp(stringbuffer, "*md3cw#") == 0)
			{
				GPIO_ResetBits(MOTORPORT, MOTOR3BITA);
				GPIO_SetBits(MOTORPORT, MOTOR3BITB);
				put_string("*md3cw#");
			}
			else
			// MOTOR 3 COUNTERCLOCKWISE = backward
			if (strcmp(stringbuffer, "*md3cc#") == 0)
			{
				GPIO_SetBits(MOTORPORT, MOTOR3BITA);
				GPIO_ResetBits(MOTORPORT, MOTOR3BITB);
				put_string("*md3cc#");
			}
			else
			// MOTOR 4 OFF
			if (strcmp(stringbuffer, "*mp4of#") == 0)
			{
				GPIO_ResetBits(MOTORPORT, MOTOR4BITA | MOTOR4BITB);
				put_string("*mp4of#");
			}
			else
			// MOTOR 4 CLOCKWISE = forward
			if (strcmp(stringbuffer, "*md4cw#") == 0)
			{
				GPIO_ResetBits(MOTORPORT, MOTOR4BITA);
				GPIO_SetBits(MOTORPORT, MOTOR4BITB);
				put_string("*md4cw#");
			}
			else
			// MOTOR 4 COUNTERCLOCKWISE = backward
			if (strcmp(stringbuffer, "*md4cc#") == 0)
			{
				GPIO_SetBits(MOTORPORT, MOTOR4BITA);
				GPIO_ResetBits(MOTORPORT, MOTOR4BITB);
				put_string("*md4cc#");
			}
			else
			// BOTSTOP
			if (strcmp(stringbuffer, "*bst#") == 0)
			{
				// turn all drive motor bits off (except PWM bits)
				GPIO_ResetBits(MOTORPORT, MOTOR1BITA | MOTOR1BITB | MOTOR2BITA | MOTOR2BITB | MOTOR3BITA | MOTOR3BITB | MOTOR4BITA | MOTOR4BITB);
				put_string("*bst#");
			}
			else
			// BOTWAIT
			if (strcmp(stringbuffer, "*bwa#") == 0)
			{
				// turn all drive motor bits off (except PWM bits)
				GPIO_ResetBits(MOTORPORT, MOTOR1BITA | MOTOR1BITB | MOTOR2BITA | MOTOR2BITB | MOTOR3BITA | MOTOR3BITB | MOTOR4BITA | MOTOR4BITB);
				put_string("*bwa#");
			}
			else
			// BOTGO = "bot go"
			if (strcmp(stringbuffer, "*bgo#") == 0)
			{
				// bot drive forward = all motors clockwise
				GPIO_ResetBits(MOTORPORT, MOTOR1BITA);
				GPIO_SetBits(MOTORPORT, MOTOR1BITB);
				GPIO_ResetBits(MOTORPORT, MOTOR2BITA);
				GPIO_SetBits(MOTORPORT, MOTOR2BITB);
				GPIO_ResetBits(MOTORPORT, MOTOR3BITA);
				GPIO_SetBits(MOTORPORT, MOTOR3BITB);
				GPIO_ResetBits(MOTORPORT, MOTOR4BITA);
				GPIO_SetBits(MOTORPORT, MOTOR4BITB);
				put_string("*bgo#");
			}
			else
			// BOTFORWARD = "bot drive forward"
			if (strcmp(stringbuffer, "*bdf#") == 0)
			{
				// bot drive forward = all motors clockwise
				GPIO_ResetBits(MOTORPORT, MOTOR1BITA);
				GPIO_SetBits(MOTORPORT, MOTOR1BITB);
				GPIO_ResetBits(MOTORPORT, MOTOR2BITA);
				GPIO_SetBits(MOTORPORT, MOTOR2BITB);
				GPIO_ResetBits(MOTORPORT, MOTOR3BITA);
				GPIO_SetBits(MOTORPORT, MOTOR3BITB);
				GPIO_ResetBits(MOTORPORT, MOTOR4BITA);
				GPIO_SetBits(MOTORPORT, MOTOR4BITB);
				put_string("*bdf#");
			}
			else
			// BOTBACKWARD = "bot drive backward"
			if (strcmp(stringbuffer, "*bdb#") == 0)
			{
				// bot drive backward = all motors counterclockwise
				GPIO_SetBits(MOTORPORT, MOTOR1BITA);
				GPIO_ResetBits(MOTORPORT, MOTOR1BITB);
				GPIO_SetBits(MOTORPORT, MOTOR2BITA);
				GPIO_ResetBits(MOTORPORT, MOTOR2BITB);
				GPIO_SetBits(MOTORPORT, MOTOR3BITA);
				GPIO_ResetBits(MOTORPORT, MOTOR3BITB);
				GPIO_SetBits(MOTORPORT, MOTOR4BITA);
				GPIO_ResetBits(MOTORPORT, MOTOR4BITB);
				put_string("*bdb#");
			}
			else
			// DRIVELEFT = "bot drive left"
			if (strcmp(stringbuffer, "*bdl#") == 0)
			{
				// bot drive left
				// MOTOR 1 COUNTERCLOCKWISE = backward
				GPIO_SetBits(MOTORPORT, MOTOR1BITA);
				GPIO_ResetBits(MOTORPORT, MOTOR1BITB);
				// MOTOR 2 CLOCKWISE = forward
				GPIO_ResetBits(MOTORPORT, MOTOR2BITA);
				GPIO_SetBits(MOTORPORT, MOTOR2BITB);
				// MOTOR 3 CLOCKWISE = forward
				GPIO_ResetBits(MOTORPORT, MOTOR3BITA);
				GPIO_SetBits(MOTORPORT, MOTOR3BITB);
				// MOTOR 4 COUNTERCLOCKWISE = backward
				GPIO_SetBits(MOTORPORT, MOTOR4BITA);
				GPIO_ResetBits(MOTORPORT, MOTOR4BITB);
				put_string("*bdl#");
			}
			else
			// DRIVERIGHT = "bot drive right"
			if (strcmp(stringbuffer, "*bdr#") == 0)
			{
				// bot drive right
				// MOTOR 1 CLOCKWISE = forward
				GPIO_ResetBits(MOTORPORT, MOTOR1BITA);
				GPIO_SetBits(MOTORPORT, MOTOR1BITB);
				// MOTOR 2 COUNTERCLOCKWISE = backward
				GPIO_SetBits(MOTORPORT, MOTOR2BITA);
				GPIO_ResetBits(MOTORPORT, MOTOR2BITB);
				// MOTOR 3 COUNTERCLOCKWISE = backward
				GPIO_SetBits(MOTORPORT, MOTOR3BITA);
				GPIO_ResetBits(MOTORPORT, MOTOR3BITB);
				// MOTOR 4 CLOCKWISE = forward
				GPIO_ResetBits(MOTORPORT, MOTOR4BITA);
				GPIO_SetBits(MOTORPORT, MOTOR4BITB);
				put_string("*bdr#");
			}
			else
			// TURNLEFT = "bot turn left"
			if (strcmp(stringbuffer, "*btl#") == 0)
			{
				// bot turn left
				// MOTOR 1 CLOCKWISE = forward
				GPIO_ResetBits(MOTORPORT, MOTOR1BITA);
				GPIO_SetBits(MOTORPORT, MOTOR1BITB);
				// MOTOR 2 COUNTERCLOCKWISE = backward
				GPIO_SetBits(MOTORPORT, MOTOR2BITA);
				GPIO_ResetBits(MOTORPORT, MOTOR2BITB);
				// MOTOR 3 CLOCKWISE = forward
				GPIO_ResetBits(MOTORPORT, MOTOR3BITA);
				GPIO_SetBits(MOTORPORT, MOTOR3BITB);
				// MOTOR 4 COUNTERCLOCKWISE = backward
				GPIO_SetBits(MOTORPORT, MOTOR4BITA);
				GPIO_ResetBits(MOTORPORT, MOTOR4BITB);
				put_string("*btl#");
			}
			else
			// TURNRIGHT = "bot turn right"
			if (strcmp(stringbuffer, "*btr#") == 0)
			{
				// bot turn right
				// MOTOR 1 COUNTERCLOCKWISE = backward
				GPIO_SetBits(MOTORPORT, MOTOR1BITA);
				GPIO_ResetBits(MOTORPORT, MOTOR1BITB);
				// MOTOR 2 CLOCKWISE = forward
				GPIO_ResetBits(MOTORPORT, MOTOR2BITA);
				GPIO_SetBits(MOTORPORT, MOTOR2BITB);
				// MOTOR 3 COUNTERCLOCKWISE = backward
				GPIO_SetBits(MOTORPORT, MOTOR3BITA);
				GPIO_ResetBits(MOTORPORT, MOTOR3BITB);
				// MOTOR 4 CLOCKWISE = forward
				GPIO_ResetBits(MOTORPORT, MOTOR4BITA);
				GPIO_SetBits(MOTORPORT, MOTOR4BITB);
				put_string("*btr#");
			}
		} // stringReceived()
	} // while (1)
}


void resetRobot(void)
{
	// LEDs off
	turnLED(LEDGREEN, OFF);
	turnLED(LEDORANGE, OFF);
	turnLED(LEDRED, OFF);
	turnLED(LEDBLUE, OFF);

	// turn all drive motor bits off (except PWM bits)
	GPIO_ResetBits(MOTORPORT, MOTOR1BITA | MOTOR1BITB | MOTOR2BITA | MOTOR2BITB | MOTOR3BITA | MOTOR3BITB | MOTOR4BITA | MOTOR4BITB);
}


void clockInit()
{
	// GPIOD Periph clock enable
	RCC_AHB1PeriphClockCmd(LEDCLOCK	, ENABLE);

	// GPIOD Periph clock enable for Motor bits
	RCC_AHB1PeriphClockCmd(MOTORCLOCK, ENABLE);

	// Timer clock enable for Motor PWM
	RCC_APB1PeriphClockCmd(MOTOR1PWMTIMCLOCK | MOTOR2PWMTIMCLOCK, ENABLE);
	RCC_APB2PeriphClockCmd(MOTOR3PWMTIMCLOCK, ENABLE);
	// Port clock enable for Motor PWM
	RCC_AHB1PeriphClockCmd(MOTOR1PWMPORTCLOCK | MOTOR2PWMPORTCLOCK | MOTOR3PWMPORTCLOCK, ENABLE);

	// Enable ADC, DMA and corresponding GPIO clocks
	RCC_AHB1PeriphClockCmd(SENSORDMACLOCK, ENABLE);
	RCC_AHB1PeriphClockCmd(SENSORPORTCLOCK, ENABLE);
	RCC_APB2PeriphClockCmd(SENSORADCCLOCK, ENABLE);
}


void gpioPortInit()
{
	GPIO_InitTypeDef GPIO_InitStructureLED;
	GPIO_InitTypeDef GPIO_InitStructureMOTOR;
	GPIO_InitTypeDef GPIO_InitStructureADC;


	// GPIOPort init, Port D (MEMS LEDs)
	// Configure port bits in output pushpull mode
	GPIO_InitStructureLED.GPIO_Pin = LEDGREEN | LEDORANGE| LEDRED| LEDBLUE;
	GPIO_InitStructureLED.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructureLED.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructureLED.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructureLED.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(LEDPORT, &GPIO_InitStructureLED);


	// Motor bits
	// Configure port bits in output pushpull mode
	GPIO_InitStructureMOTOR.GPIO_Pin = MOTOR1BITA | MOTOR1BITB | MOTOR2BITA | MOTOR2BITB | MOTOR3BITA | MOTOR3BITB | MOTOR4BITA | MOTOR4BITB;
	GPIO_InitStructureMOTOR.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructureMOTOR.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructureMOTOR.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructureMOTOR.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(MOTORPORT, &GPIO_InitStructureMOTOR);


	// Configure ADC pins / channel pins as analog input
	GPIO_InitStructureADC.GPIO_Pin = SENSORPIN24VOLT | SENSORPIN12VOLT;
	GPIO_InitStructureADC.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructureADC.GPIO_PuPd = GPIO_PuPd_NOPULL ;
	GPIO_Init(SENSORPORT, &GPIO_InitStructureADC);
}


/**
  * @brief  Configure the timer for PWM.
  * @param  None
  * @retval None
  */
void timerInit(void)
{
	GPIO_InitTypeDef 		GPIO_InitStructureTimer;
	TIM_TimeBaseInitTypeDef	TIM_TimeBaseStructure;
	TIM_OCInitTypeDef		TIM_OCInitStructure; // Timer Output Compare Init structure definition
	uint32_t				TimerCounterClock = 0;
	uint32_t				TimerOutputClock = 0;
	uint16_t				PrescalerValue = 0;
	uint32_t				PulseDurationInMicroSeconds = 0;


	// set timer frequencies
	TimerCounterClock = 1000000; //  1 MHz
	TimerOutputClock = 10000;    // 10 kHz = 100 µs period

	// set pulse duration in mili seconds (HIGH time)
	// can be up to from 0 to 99 (due to a TimerOutputClock of 10 kHz)
	PulseDurationInMicroSeconds = 50;


	//---------
	// motor 1
	//---------
	// Set PWM Port, Pin and method
	GPIO_InitStructureTimer.GPIO_Pin = MOTOR1PWMBIT;
	GPIO_InitStructureTimer.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructureTimer.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructureTimer.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructureTimer.GPIO_PuPd = GPIO_PuPd_UP ;
	GPIO_Init(MOTOR1PWMPORT, &GPIO_InitStructureTimer);

	// Connect TIM pin to Alternate Function (AF)
	GPIO_PinAFConfig(MOTOR1PWMPORT, MOTOR1PWMTIMBIT, MOTOR1PWMAF);

	// Timer base configuration
	TIM_TimeBaseStructure.TIM_Period = (uint16_t) (TimerCounterClock / TimerOutputClock);
	TIM_TimeBaseStructure.TIM_Prescaler = (uint16_t) ((SystemCoreClock /2) / TimerCounterClock) - 1;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

	// basic timer init
	TIM_TimeBaseInit(MOTOR1PWMTIMER, &TIM_TimeBaseStructure);

	// configure PWM mode and duration
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = PulseDurationInMicroSeconds; // set the duty cycle / pulse here!
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

	// Output Compare channels
	switch (MOTOR1PWMCHANNEL)
	{
		case 1:
			TIM_OC1Init(MOTOR1PWMTIMER, &TIM_OCInitStructure);
			TIM_OC1PreloadConfig(MOTOR1PWMTIMER, TIM_OCPreload_Enable);
			break;
		case 2:
			TIM_OC2Init(MOTOR1PWMTIMER, &TIM_OCInitStructure);
			TIM_OC2PreloadConfig(MOTOR1PWMTIMER, TIM_OCPreload_Enable);
			break;
		case 3:
			TIM_OC3Init(MOTOR1PWMTIMER, &TIM_OCInitStructure);
			TIM_OC3PreloadConfig(MOTOR1PWMTIMER, TIM_OCPreload_Enable);
			break;
		case 4:
			TIM_OC4Init(MOTOR1PWMTIMER, &TIM_OCInitStructure);
			TIM_OC4PreloadConfig(MOTOR1PWMTIMER, TIM_OCPreload_Enable);
			break;
	}

	// preload timer config
	TIM_ARRPreloadConfig(MOTOR1PWMTIMER, ENABLE);

	// enable timer / counter
	TIM_Cmd(MOTOR1PWMTIMER, ENABLE);


	//---------
	// motor 2
	//---------
	// Set PWM Port, Pin and method
	GPIO_InitStructureTimer.GPIO_Pin = MOTOR2PWMBIT;
	GPIO_InitStructureTimer.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructureTimer.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructureTimer.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructureTimer.GPIO_PuPd = GPIO_PuPd_UP ;
	GPIO_Init(MOTOR2PWMPORT, &GPIO_InitStructureTimer);

	// Connect TIM pin to Alternate Function (AF)
	GPIO_PinAFConfig(MOTOR2PWMPORT, MOTOR2PWMTIMBIT, MOTOR2PWMAF);

	// Timer base configuration
	TIM_TimeBaseStructure.TIM_Period = (uint16_t) (TimerCounterClock / TimerOutputClock);
	TIM_TimeBaseStructure.TIM_Prescaler = (uint16_t) ((SystemCoreClock /2) / TimerCounterClock) - 1;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

	// basic timer init
	TIM_TimeBaseInit(MOTOR2PWMTIMER, &TIM_TimeBaseStructure);

	// configure PWM mode and duration
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = PulseDurationInMicroSeconds; // set the duty cycle / pulse here!
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

	// Output Compare channels
	switch (MOTOR2PWMCHANNEL)
	{
		case 1:
			TIM_OC1Init(MOTOR2PWMTIMER, &TIM_OCInitStructure);
			TIM_OC1PreloadConfig(MOTOR2PWMTIMER, TIM_OCPreload_Enable);
			break;
		case 2:
			TIM_OC2Init(MOTOR2PWMTIMER, &TIM_OCInitStructure);
			TIM_OC2PreloadConfig(MOTOR2PWMTIMER, TIM_OCPreload_Enable);
			break;
		case 3:
			TIM_OC3Init(MOTOR2PWMTIMER, &TIM_OCInitStructure);
			TIM_OC3PreloadConfig(MOTOR2PWMTIMER, TIM_OCPreload_Enable);
			break;
		case 4:
			TIM_OC4Init(MOTOR2PWMTIMER, &TIM_OCInitStructure);
			TIM_OC4PreloadConfig(MOTOR2PWMTIMER, TIM_OCPreload_Enable);
			break;
	}

	// preload timer config
	TIM_ARRPreloadConfig(MOTOR2PWMTIMER, ENABLE);

	// enable timer / counter
	TIM_Cmd(MOTOR2PWMTIMER, ENABLE);


	//---------
	// motor 3
	//---------
	// Set PWM Port, Pin and method
	GPIO_InitStructureTimer.GPIO_Pin = MOTOR3PWMBIT;
	GPIO_InitStructureTimer.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructureTimer.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructureTimer.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructureTimer.GPIO_PuPd = GPIO_PuPd_UP ;
	GPIO_Init(MOTOR3PWMPORT, &GPIO_InitStructureTimer);

	// Connect TIM pin to Alternate Function (AF)
	GPIO_PinAFConfig(MOTOR3PWMPORT, MOTOR3PWMTIMBIT, MOTOR3PWMAF);

	// Timer base configuration
	TIM_TimeBaseStructure.TIM_Period = (uint16_t) (TimerCounterClock / TimerOutputClock);
	TIM_TimeBaseStructure.TIM_Prescaler = (uint16_t) ((SystemCoreClock /2) / TimerCounterClock) - 1;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

	// basic timer init
	TIM_TimeBaseInit(MOTOR3PWMTIMER, &TIM_TimeBaseStructure);

	// configure PWM mode and duration
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = PulseDurationInMicroSeconds; // set the duty cycle / pulse here!
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

	// Output Compare channels
	switch (MOTOR3PWMCHANNEL)
	{
		case 1:
			TIM_OC1Init(MOTOR3PWMTIMER, &TIM_OCInitStructure);
			TIM_OC1PreloadConfig(MOTOR3PWMTIMER, TIM_OCPreload_Enable);
			break;
		case 2:
			TIM_OC2Init(MOTOR3PWMTIMER, &TIM_OCInitStructure);
			TIM_OC2PreloadConfig(MOTOR3PWMTIMER, TIM_OCPreload_Enable);
			break;
		case 3:
			TIM_OC3Init(MOTOR3PWMTIMER, &TIM_OCInitStructure);
			TIM_OC3PreloadConfig(MOTOR3PWMTIMER, TIM_OCPreload_Enable);
			break;
		case 4:
			TIM_OC4Init(MOTOR3PWMTIMER, &TIM_OCInitStructure);
			TIM_OC4PreloadConfig(MOTOR3PWMTIMER, TIM_OCPreload_Enable);
			break;
	}

	// preload timer config
	TIM_ARRPreloadConfig(MOTOR3PWMTIMER, ENABLE);

	// enable timer / counter
	TIM_Cmd(MOTOR3PWMTIMER, ENABLE);
}


void timerUpdate(int speed)
{
	TIM_OCInitTypeDef TIM_OCInitStructure;


	// are within a valid range?
	if ((speed<1) || (speed>99))
	{
		return;
	}

	TIM_Cmd(MOTOR1PWMTIMER, DISABLE);

	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = i; // set the duty cycle / pulse here!
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

	// Output Compare channels
	switch (MOTOR1PWMCHANNEL)
	{
		case 1:
			TIM_OC1Init(MOTOR1PWMTIMER, &TIM_OCInitStructure);
			TIM_OC1PreloadConfig(MOTOR1PWMTIMER, TIM_OCPreload_Enable);
			break;
		case 2:
			TIM_OC2Init(MOTOR1PWMTIMER, &TIM_OCInitStructure);
			TIM_OC2PreloadConfig(MOTOR1PWMTIMER, TIM_OCPreload_Enable);
			break;
		case 3:
			TIM_OC3Init(MOTOR1PWMTIMER, &TIM_OCInitStructure);
			TIM_OC3PreloadConfig(MOTOR1PWMTIMER, TIM_OCPreload_Enable);
			break;
		case 4:
			TIM_OC4Init(MOTOR1PWMTIMER, &TIM_OCInitStructure);
			TIM_OC4PreloadConfig(MOTOR1PWMTIMER, TIM_OCPreload_Enable);
			break;
	}

	// preload timer config
	TIM_ARRPreloadConfig(MOTOR1PWMTIMER, ENABLE);

	// enable timer / counter
	TIM_Cmd(MOTOR1PWMTIMER, ENABLE);
}

void DMAACDinit(void)
{
	ADC_InitTypeDef       ADC_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	DMA_InitTypeDef       DMA_InitStructure;
	GPIO_InitTypeDef      GPIO_InitStructure;
	int n=0;


	// init array of ADC results
	for (n=0; n<NUMBEROFADCHANNELS; n++)
	{
		ADC3ConvertedValues[n] = 0;
	}

	// to be safe we reset potentially enabled streams first
	DMA_DeInit(SENSORDMASTREAM);

	// DMA 2 Stream 0 channel 2 configuration **************************************
	DMA_InitStructure.DMA_Channel = SENSORDMACHANNEL;

	// Specifies the peripheral base address for DMAy Streamx
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) ADC3_DR_ADDRESS;

	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t) &ADC3ConvertedValues; // this will hold the converted values
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
	DMA_InitStructure.DMA_BufferSize = NUMBEROFADCHANNELS;					 // for two AD channels !
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;					 // enabled since we use more than one AD channel!
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;

	DMA_Init(SENSORDMASTREAM, &DMA_InitStructure);
	DMA_Cmd(SENSORDMASTREAM, ENABLE);

	//  Function used to set the ADC configuration to the default reset state
	ADC_DeInit();

	// ADC Common Init **********************************************************
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled; // Disabled. Only relevant for multimode
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
	ADC_CommonInit(&ADC_CommonInitStructure);

	// ADC3 Init ****************************************************************
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;  	// 12 bit resolution
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;				// enabled since we use more than once AD channel!
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;		// alignment of data in ADC_DR after data conversion
	ADC_InitStructure.ADC_NbrOfConversion = NUMBEROFADCHANNELS;	// for two AD channels !
	ADC_Init(SENSORADC, &ADC_InitStructure);

	// ADC3 regular channel11 configuration *************************************
	// '1' is the order (rank) of the AD channels to be scanned
	ADC_RegularChannelConfig(SENSORADC, SENSORCHANNEL24VOLT, 1, ADC_SampleTime_3Cycles); 

	// ADC3 regular channel12 configuration *************************************
	// '2' is the order (rank) of the AD channels to be scanned
	ADC_RegularChannelConfig(SENSORADC, SENSORCHANNEL12VOLT, 2, ADC_SampleTime_3Cycles);

	// Enable DMA request after last transfer (Single-ADC mode)
	ADC_DMARequestAfterLastTransferCmd(SENSORADC, ENABLE);

	// Enable ADC3 DMA
	ADC_DMACmd(SENSORADC, ENABLE);

	// Enable ADC3
	ADC_Cmd(SENSORADC, ENABLE);

	// Start AD conversion
	ADC_SoftwareStartConv(SENSORADC);
}


void turnLED(int led, int state)
{
	if (state == ON)
	{
		GPIO_SetBits(LEDPORT, led);
	}
	else
	{
		GPIO_ResetBits(LEDPORT, led);
	}
}


void sendUInt(uint16_t value)
{
	uint8_t length = 0;
	
	// start the answer string to send with a '*'
	stringbuffer[0] = getStarter();


	// convert int to ascii (to Basis 10)
	// (but don't overwrite the first char which is the 'starter' *.)
	ltoa(value, stringbuffer+1);
	//sprintf(stringbuffer+1, "%d", value);


	// get the length of the string
	length = strlen(stringbuffer);


	// add m string terminator '#' at the end of the buffer
	stringbuffer[length] = getTerminator();


	// String mit \0 terminieren
	stringbuffer[length+1] = 0;


	// send answer
	put_string(stringbuffer);
}


/**
   http://www.jb.man.ac.uk/~slowe/cpp/itoa.html
 * C++ version 0.4 char* style "itoa":
 * Written by Lukás Chmela
 * Released under GPLv3.
 */
#define BASE 10

char* ltoa(long value, char* result)
{
    char* ptr = result, *ptr1 = result, tmp_char;
    long tmp_value;


    do
    {
        tmp_value = value;
        value /= BASE;
        *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz" [35 + (tmp_value - value * BASE)];
    } while ( value );

    // Apply negative sign
    if (tmp_value < 0)
        *ptr++ = '-';

    *ptr-- = '\0';

    while(ptr1 < ptr)
    {
        tmp_char = *ptr;
        *ptr--= *ptr1;
        *ptr1++ = tmp_char;
    }

    return result;
}


/**
	* @brief  delay Function.
	* @param  nCount:specifies the delay time length.
  * @retval None
	*/
void delay(__IO uint32_t nCount)
{
	while(nCount--)
	{
	}
}


int atoi(const char *s)
{
	static const char digits[] = "0123456789";  // legal digits in order
	unsigned val=0;         // value we're accumulating
	int neg=0;              // set to true if we see a minus sign

	// skip whitespace
	while (*s==' ' || *s=='\t')
	{
		s++;
	}

	// check for sign
	if (*s=='-')
	{
		neg=1;
		s++;
	}
	else if (*s=='+')
	{
		s++;
	}

	// process each digit
	while (*s)
	{
		const char *where;
		unsigned digit;

		// look for the digit in the list of digits (using  char * strchr ( const char *, int ); )
		where = strchr(digits, *s); // warning: assignment makes pointer from integer without a cast
		if (where==NULL)
		{
			// not found; not a digit, so stop
			break;
		}

		// get the index into the digit list, which is the value
		digit = (where - digits);

		// could (should?) check for overflow here

		// shift the number over and add in the new digit
		val = val*10 + digit;

		// look at the next character
		s++;
	}

	// handle negative numbers
	if (neg)
	{
		return -val;
	}

	// done
	return val;
 }


#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
	/* User can add his own implementation to report the file name and line number,
	  ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

	// Infinite loop
	while (1)
	{
	}
}
#endif
