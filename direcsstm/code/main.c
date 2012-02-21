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

#define MOTOR1PORT				GPIOB
#define MOTOR1BITA				GPIO_Pin_0
#define MOTOR1BITB				GPIO_Pin_1
#define MOTOR1CLOCK				RCC_AHB1Periph_GPIOB

// TIM4, PB7, Channel 2 for motor speed (PWM)
#define MOTORPWMTIMER			TIM4
#define	MOTORPWMCHANNEL			2
#define MOTORPWMPORT			GPIOB
#define MOTORPWMBIT				GPIO_Pin_7
#define MOTORPWMTIMBIT			GPIO_PinSource7
#define MOTORPWMPORTCLOCK		RCC_AHB1Periph_GPIOB
#define MOTORPWMTIMCLOCK		RCC_APB1Periph_TIM4
#define MOTORPWMAF 				GPIO_AF_TIM4

// ADC and DMA for battery / voltage sensors
#define SENSORDMACHANNEL		DMA_Channel_2;
#define SENSORPORT				GPIOC
#define SENSORPIN24VOLT			GPIO_Pin_1
#define SENSORPIN12VOLT			GPIO_Pin_2
#define SENSORPORTCLOCK			RCC_AHB1Periph_GPIOC
#define SENSORDMACLOCK			RCC_AHB1Periph_DMA2
#define SENSORDMASTREAM			DMA2_Stream0
#define SENSORADC 				ADC3
#define SENSORADCCLOCK			RCC_APB2Periph_ADC3
#define	SENSORCHANNEL24VOLT		ADC_Channel_11
#define	SENSORCHANNEL12VOLT		ADC_Channel_12

#define NUMBEROFADCHANNELS		2


// ADC DMA stuff
#define ADC3_DR_ADDRESS	((uint32_t)0x4001224C)
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

void LEDblink();

// turns a LED on or off
void turnLED(int led, int state);

// sends a u int value over the serial line as ASCII
void sendUInt(uint16_t value);

char* ltoa(long value, char* result);
int atoi(const char *s);

void Delay(__IO uint32_t nCount);



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

	// LEDs off
	turnLED(LEDGREEN, OFF);
	turnLED(LEDORANGE, OFF);
	turnLED(LEDRED, OFF);
	turnLED(LEDBLUE, OFF);


	while (1)
	{
//		serialBlockingReadString();

		uint16_t buchstabe = 0;


		// blocking read on serial port USART2
		while (USART_GetFlagStatus(USART2, USART_FLAG_RXNE) == RESET)
		{
		}

		buchstabe = USART_ReceiveData(USART2);
		
		// build string
		receiveChar(buchstabe);



		if (stringReceived())
		{
			// Get complete string and delete uart_rx_flag
			get_string(stringbuffer);


			// RESET / INIT
			if (strcmp(stringbuffer, "*re#") == 0)
			{
//				turnLED(LEDGREEN, ON);
/*
				// turn all drive motor bits off (except PWM bits)
				PORTL &= ~(1<<PIN0);
				PORTL &= ~(1<<PIN1);
				PORTL &= ~(1<<PIN2);
				PORTL &= ~(1<<PIN3);
				PORTL &= ~(1<<PIN6);
				PORTL &= ~(1<<PIN7);
				PORTD &= ~(1<<PIN6);
				PORTD &= ~(1<<PIN7);
				// flashlight off
				relais(OFF);
				// red LED off. Know we know, that the program on the PC/Mac has initialised the Atmel
				redLED(OFF);
				
				// setServoPosition(1, 17); // <- exact position now in the mrs.ini!
				// setServoPosition(2, 19); // <- exact position now in the mrs.ini!
				// setServoPosition(3, 23); // <- exact position now in the mrs.ini!
				// setServoPosition(4, 19); // <- exact position now in the mrs.ini!
				// setServoPosition(5, 19); // <- exact position now in the mrs.ini!
				// setServoPosition(6, 22); // <- exact position now in the mrs.ini!
*/
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
/*
				// turn all drive motor bits off (except PWM bits)
				PORTL &= ~(1<<PIN0);
				PORTL &= ~(1<<PIN1);
				PORTL &= ~(1<<PIN2);
				PORTL &= ~(1<<PIN3);
				PORTL &= ~(1<<PIN6);
				PORTL &= ~(1<<PIN7);
				PORTD &= ~(1<<PIN6);
				PORTD &= ~(1<<PIN7);
	
				// flashlight off
				relais(OFF);

				// red LED off
				redLED(OFF);

				// d i s a b l e  watchdog!
				watchdog(DISABLE);
*/				
				// answer
				put_string("*sl#");
			}
			else
			// READ_SENSOR_1
			if (strcmp(stringbuffer, "*s1#") == 0)
			{
				// read ADC and send answer over serial port
// @todo:				sendUInt( readADC(SENSOR1) );
				sendUInt( 42 );
			}
			else
			// MOTOR1_OFF
			if (strcmp(stringbuffer, "*mp1of#") == 0)
			{
				// delete Motor1 A and B bits
				GPIO_ResetBits(MOTOR1PORT, MOTOR1BITA | MOTOR1BITB);
				// answer with "ok"
				put_string("*mp1of#");
			}
			else
			// MOTOR 1 CLOCKWISE = forward
			if (strcmp(stringbuffer, "*md1cw#") == 0)
			{
				// delete Motor1 A bit
				GPIO_ResetBits(MOTOR1PORT, MOTOR1BITA);
				// set Motor1 B bit
				GPIO_SetBits(MOTOR1PORT, MOTOR1BITB);

				// answer with "ok"
				put_string("*md1cw#");
			}
			else
			// MOTOR 1 COUNTERCLOCKWISE = backward
			if (strcmp(stringbuffer, "*md1cc#") == 0)
			{
				// set Motor1 A bit
				GPIO_SetBits(MOTOR1PORT, MOTOR1BITA);
				// delete Motor1 B bit
				GPIO_ResetBits(MOTOR1PORT, MOTOR1BITB);
				
				// answer with "ok"
				put_string("*md1cc#");
			}
			else
			// MOTOR1_SPEED_SET
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
		} // stringReceived()

	} // while (1)
}


void clockInit()
{
	// GPIOD Periph clock enable for Motor bits
	RCC_AHB1PeriphClockCmd(MOTOR1CLOCK, ENABLE);

	// Timer clock enable for Motor PWM
	RCC_APB1PeriphClockCmd(MOTORPWMTIMCLOCK, ENABLE);

	// Port clock enable for Motor PWM
	RCC_AHB1PeriphClockCmd(MOTORPWMPORTCLOCK, ENABLE);
}


void gpioPortInit()
{
	GPIO_InitTypeDef GPIO_InitStructureLED;
	GPIO_InitTypeDef GPIO_InitStructureADC;


/*
	// GPIOD Periph clock enable
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

	// Configure PD12, PD13, PD14 and PD15 in output pushpull mode
	GPIO_InitStructureLED.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13| GPIO_Pin_14| GPIO_Pin_15;
	GPIO_InitStructureLED.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructureLED.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructureLED.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructureLED.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOD, &GPIO_InitStructureLED);


	//	--------------------------------------------------------------------------------
	//	GPIOPort init, Port D (MEMS LEDs)
	//	--------------------------------------------------------------------------------
	// GPIOD Periph clock enable
	RCC_AHB1PeriphClockCmd(LEDCLOCK	, ENABLE);

	// Configure port bits in output pushpull mode
	GPIO_InitStructureLED.GPIO_Pin = LEDGREEN | LEDORANGE| LEDRED| LEDBLUE;
	GPIO_InitStructureLED.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructureLED.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructureLED.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructureLED.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(LEDPORT, &GPIO_InitStructureLED);
	//	--------------------------------------------------------------------------------
*/

	// Motor bits
	// Configure port bits in output pushpull mode
	GPIO_InitStructureLED.GPIO_Pin = MOTOR1BITA | MOTOR1BITB; // enable motor 1 A+B
	GPIO_InitStructureLED.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructureLED.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructureLED.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructureLED.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(MOTOR1PORT, &GPIO_InitStructureLED);
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
	uint32_t TimerCounterClock = 0;
	uint32_t TimerOutputClock = 0;
	uint16_t PrescalerValue = 0;
	uint32_t PulseDurationInMicroSeconds = 0;


	// set timer frequencies
	TimerCounterClock = 1000000; //  1 MHz
	TimerOutputClock = 10000;    // 10 kHz = 100 µs period

	// set pulse duration in mili seconds (HIGH time)
	// can be up to from 0 to 99 (due to a TimerOutputClock of 10 kHz)
	PulseDurationInMicroSeconds = 50;

	// Set PWM Port, Pin and method
	GPIO_InitStructureTimer.GPIO_Pin = MOTORPWMBIT;
	GPIO_InitStructureTimer.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructureTimer.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructureTimer.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructureTimer.GPIO_PuPd = GPIO_PuPd_UP ;
	GPIO_Init(MOTORPWMPORT, &GPIO_InitStructureTimer); 

	// Connect TIM pin to Alternate Function (AF)
	GPIO_PinAFConfig(MOTORPWMPORT, MOTORPWMTIMBIT, MOTORPWMAF);

	// Timer base configuration
	TIM_TimeBaseStructure.TIM_Period = (uint16_t) (TimerCounterClock / TimerOutputClock);
	TIM_TimeBaseStructure.TIM_Prescaler = (uint16_t) ((SystemCoreClock /2) / TimerCounterClock) - 1;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

	// basic timer init
	TIM_TimeBaseInit(MOTORPWMTIMER, &TIM_TimeBaseStructure);

	// configure PWM mode and duration
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = PulseDurationInMicroSeconds; // set the duty cycle / pulse here!
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

	// Output Compare channels
	switch (MOTORPWMCHANNEL)
	{
		case 1:
			TIM_OC1Init(MOTORPWMTIMER, &TIM_OCInitStructure);
			TIM_OC1PreloadConfig(MOTORPWMTIMER, TIM_OCPreload_Enable);
			break;
		case 2:
			TIM_OC2Init(MOTORPWMTIMER, &TIM_OCInitStructure);
			TIM_OC2PreloadConfig(MOTORPWMTIMER, TIM_OCPreload_Enable);
			break;
		case 3:
			TIM_OC3Init(MOTORPWMTIMER, &TIM_OCInitStructure);
			TIM_OC3PreloadConfig(MOTORPWMTIMER, TIM_OCPreload_Enable);
			break;
		case 4:
			TIM_OC4Init(MOTORPWMTIMER, &TIM_OCInitStructure);
			TIM_OC4PreloadConfig(MOTORPWMTIMER, TIM_OCPreload_Enable);
			break;
	}

	// preload timer config
	TIM_ARRPreloadConfig(MOTORPWMTIMER, ENABLE);

	// enable timer / counter
	TIM_Cmd(MOTORPWMTIMER, ENABLE);
}


void timerUpdate(int speed)
{
	TIM_OCInitTypeDef TIM_OCInitStructure;


	// are within a valid range?
	if ((speed<1) || (speed>99))
	{
		return;
	}

	TIM_Cmd(MOTORPWMTIMER, DISABLE);

	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = i; // set the duty cycle / pulse here!
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

	// Output Compare channels
	switch (MOTORPWMCHANNEL)
	{
		case 1:
			TIM_OC1Init(MOTORPWMTIMER, &TIM_OCInitStructure);
			TIM_OC1PreloadConfig(MOTORPWMTIMER, TIM_OCPreload_Enable);
			break;
		case 2:
			TIM_OC2Init(MOTORPWMTIMER, &TIM_OCInitStructure);
			TIM_OC2PreloadConfig(MOTORPWMTIMER, TIM_OCPreload_Enable);
			break;
		case 3:
			TIM_OC3Init(MOTORPWMTIMER, &TIM_OCInitStructure);
			TIM_OC3PreloadConfig(MOTORPWMTIMER, TIM_OCPreload_Enable);
			break;
		case 4:
			TIM_OC4Init(MOTORPWMTIMER, &TIM_OCInitStructure);
			TIM_OC4PreloadConfig(MOTORPWMTIMER, TIM_OCPreload_Enable);
			break;
	}

	// preload timer config
	TIM_ARRPreloadConfig(MOTORPWMTIMER, ENABLE);

	// enable timer / counter
	TIM_Cmd(MOTORPWMTIMER, ENABLE);
}

void DMAACDinit(void)
{
	ADC_InitTypeDef       ADC_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	DMA_InitTypeDef       DMA_InitStructure;
	GPIO_InitTypeDef      GPIO_InitStructure;


	ADC3ConvertedValues[0] = 0;
	ADC3ConvertedValues[1] = 0;


	// to be safe we reset potentially enabled streams first
	DMA_DeInit(SENSORDMASTREAM);

	// Enable ADC3, DMA2 and GPIO clocks ****************************************
	RCC_AHB1PeriphClockCmd(SENSORDMACLOCK | SENSORPORTCLOCK, ENABLE);
	RCC_APB2PeriphClockCmd(SENSORADCCLOCK, ENABLE);

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

	// Configure ADC 3 Channel1 two pins as analog input ******************************
	GPIO_InitStructure.GPIO_Pin = SENSORPIN24VOLT | SENSORPIN12VOLT;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
	GPIO_Init(SENSORPORT, &GPIO_InitStructure);

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


/*
void LEDblink()
{
	//
	// LED blink blink blink
	//
	// PD12 to be toggled
	GPIO_SetBits(GPIOD, GPIO_Pin_12);

	// Insert delay
	Delay(0x3FFFFF);

	// PD13 to be toggled
	GPIO_SetBits(GPIOD, GPIO_Pin_13);

	// Insert delay
	Delay(0x3FFFFF);

	// PD14 to be toggled
	GPIO_SetBits(GPIOD, GPIO_Pin_14);

	// Insert delay
	Delay(0x3FFFFF);

	// PD15 to be toggled
	GPIO_SetBits(GPIOD, GPIO_Pin_15);

	// Insert delay
	Delay(0x7FFFFF);

	GPIO_ResetBits(GPIOD, GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);

	// Insert delay
	Delay(0xFFFFFF);
}
*/


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
	* @brief  Delay Function.
	* @param  nCount:specifies the Delay time length.
  * @retval None
	*/
void Delay(__IO uint32_t nCount)
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

		// look for the digit in the list of digits
		where = strchr(digits, *s);
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
