#include <string.h>
#include <stdio.h>

#include "stm32f4_discovery.h"
#include "stm32f4xx_conf.h"

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

#define SPEED_SET_ALLMOTORS		60

#define	READ_AXIS_X				61
#define	READ_AXIS_Y				62
#define	READ_AXIS_Z				63

#define	INIT					64
#define SLEEP					65

#define READ_CONTACT1			70
#define READ_CONTACT2			71
#define READ_CONTACT3			72
#define READ_CONTACT4			73

/// Some direct driving commmands for the robot. @sa Direcs::drive() @sa Direcs::motorControl()
#define BOTFORWARD 				80
#define BOTBACKWARD 			81
#define BOTLEFT					82
#define BOTRIGHT				83
#define BOTTURNLEFT				84
#define BOTTURNRIGHT			85
#define BOTSTART				86
#define BOTSTOP					87
#define BOTWAIT					88
// #define BOTDIAGONAL_FORWARD_LEFT
// #define BOTDIAGONAL_FORWARD_RIGHT
// #define BOTDIAGONAL_BACKWARD_LEFT
// #define BOTDIAGONAL_BACKWARD_RIGHT


// these numbers represent the Ports of the A/D converter C  !!!
// for the infrared sensors and the motor sensors
#define SENSOR1					0  // ADC0
#define SENSOR2					1  // ADC1
#define SENSOR3					2  // ADC2
#define SENSOR4					3  // ADC3
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
#define LEDCLOCK				RCC_AHB1Periph_GPIOD
#define LEDPORT					GPIOD
#define LEDGREEN				GPIO_Pin_12
#define LEDORANGE				GPIO_Pin_13
#define LEDRED					GPIO_Pin_14
#define LEDBLUE					GPIO_Pin_15

#define MOTOR1PERIPH			RCC_AHB1Periph_GPIOB
#define MOTOR1PORT				GPIOB
#define MOTOR1BITA				GPIO_Pin_0
#define MOTOR1BITB				GPIO_Pin_1

#define MOTORPWMTIMER			TIM4
#define MOTORPWMTIMCLOCK		RCC_APB1Periph_TIM4
#define MOTORPWMPORTCLOCK		RCC_AHB1Periph_GPIOD
#define MOTORPWMAF 				GPIO_AF_TIM4
#define MOTORPWMPORT			GPIOD
#define MOTORPWMBIT				GPIO_Pin_12
#define MOTORPWMTIMBIT			GPIO_PinSource12


// Private variables ---------------------------------------------------------

// stores the serial received command and the string which will be sent as an answer
char stringbuffer[64];
int i;


// the prototypes ------------------------------------------------------------
int main(void);

// GPIOPort init
void gpioPortInit();

// Timer init for PWM
void TimerInit(void);

void LEDblink();

// turns a LED on or off
void turnLED(int led, int state);

// sends a u int value over the serial line as ASCII
void sendUInt(uint16_t value);

char* ltoa(long value, char* result);

void Delay(__IO uint32_t nCount);



/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
	// general port init (LED, motor bits etc.)
	gpioPortInit();

	// init USART (serial port)
	usartInit();
	
	// init timer for PWM
	TimerInit();


	//
	//	motor pwm test ! ! !
	//
	TIM_OCInitTypeDef		TIM_OCInitStructure;


	while(1)
	{
		for (i=1; i<99; i++)
		{
			Delay(0x0BFFFF);
			TIM_Cmd(MOTORPWMTIMER, DISABLE);

			TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
			TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
			TIM_OCInitStructure.TIM_Pulse = i; // set the duty cycle / pulse here!
			TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
			TIM_OC1Init(MOTORPWMTIMER, &TIM_OCInitStructure);
			TIM_OC1PreloadConfig(MOTORPWMTIMER, TIM_OCPreload_Enable);

			TIM_ARRPreloadConfig(MOTORPWMTIMER, ENABLE);

			TIM_Cmd(MOTORPWMTIMER, ENABLE);
		}

		Delay(0xFFFFFF);
		Delay(0xFFFFFF);
		Delay(0xFFFFFF);
		Delay(0xFFFFFF);

		for (i=99; i>2; i--)
		{
			Delay(0x0BFFFF);
			TIM_Cmd(MOTORPWMTIMER, DISABLE);

			TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
			TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
			TIM_OCInitStructure.TIM_Pulse = i; // set the duty cycle / pulse here!
			TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
			TIM_OC1Init(MOTORPWMTIMER, &TIM_OCInitStructure);
			TIM_OC1PreloadConfig(MOTORPWMTIMER, TIM_OCPreload_Enable);

			TIM_ARRPreloadConfig(MOTORPWMTIMER, ENABLE);

			TIM_Cmd(MOTORPWMTIMER, ENABLE);
		}

		Delay(0xFFFFFF);
		Delay(0xFFFFFF);
		Delay(0xFFFFFF);
		Delay(0xFFFFFF);
	}


/*
	// LEDs off
	turnLED(LEDGREEN, OFF);
	turnLED(LEDORANGE, OFF);
	turnLED(LEDRED, OFF);
	turnLED(LEDBLUE, OFF);
*/

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
		} // stringReceived()

	} // while (1)
}


void gpioPortInit()
{
	GPIO_InitTypeDef GPIO_InitStructureLED;


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

	//	--------------------------------------------------------------------------------
	//	GPIOPort init, Port B (Motor bits)
	//	--------------------------------------------------------------------------------
	// GPIOD Periph clock enable
	RCC_AHB1PeriphClockCmd(MOTOR1PERIPH, ENABLE);

	// Configure port bits in output pushpull mode
	GPIO_InitStructureLED.GPIO_Pin = MOTOR1BITA | MOTOR1BITB; // enable motor 1 A+B
	GPIO_InitStructureLED.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructureLED.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructureLED.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructureLED.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(MOTOR1PORT, &GPIO_InitStructureLED);
	//	--------------------------------------------------------------------------------
*/
}


/**
  * @brief  Configure the timer for PWM.
  * @param  None
  * @retval None
  */
void TimerInit(void)
{
	GPIO_InitTypeDef 		GPIO_InitStructureTimer;
	TIM_TimeBaseInitTypeDef	TIM_TimeBaseStructure;
	TIM_OCInitTypeDef		TIM_OCInitStructure;
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

	// Timer clock enable
	RCC_APB1PeriphClockCmd(MOTORPWMTIMCLOCK, ENABLE);

	// Port clock enable
	RCC_AHB1PeriphClockCmd(MOTORPWMPORTCLOCK, ENABLE);

	// Set PWM Port, Pin and method
	GPIO_InitStructureTimer.GPIO_Pin = MOTORPWMBIT;
	GPIO_InitStructureTimer.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructureTimer.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructureTimer.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructureTimer.GPIO_PuPd = GPIO_PuPd_UP ;
	GPIO_Init(MOTORPWMPORT, &GPIO_InitStructureTimer); 

	// Connect TIM pin to AF
	GPIO_PinAFConfig(MOTORPWMPORT, MOTORPWMTIMBIT, MOTORPWMAF);


	// Timer base configuration
	// 	Don't know why, but this code has to be here (not in a seperate method)
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
	TIM_OC1Init(MOTORPWMTIMER, &TIM_OCInitStructure);
	TIM_OC1PreloadConfig(MOTORPWMTIMER, TIM_OCPreload_Enable);

	// preload timer config
	TIM_ARRPreloadConfig(MOTORPWMTIMER, ENABLE);

	// enable timer / counter
	TIM_Cmd(MOTORPWMTIMER, ENABLE);
}


void LEDblink()
{
	//
	// LED blink blink blink
	//
	/* PD12 to be toggled */
	GPIO_SetBits(GPIOD, GPIO_Pin_12);

	/* Insert delay */
	Delay(0x3FFFFF);

	/* PD13 to be toggled */
	GPIO_SetBits(GPIOD, GPIO_Pin_13);

	/* Insert delay */
	Delay(0x3FFFFF);

	/* PD14 to be toggled */
	GPIO_SetBits(GPIOD, GPIO_Pin_14);

	/* Insert delay */
	Delay(0x3FFFFF);

	/* PD15 to be toggled */
	GPIO_SetBits(GPIOD, GPIO_Pin_15);

	/* Insert delay */
	Delay(0x7FFFFF);

	GPIO_ResetBits(GPIOD, GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);

	/* Insert delay */
	Delay(0xFFFFFF);
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

	/* Infinite loop */
	while (1)
	{
	}
}
#endif
