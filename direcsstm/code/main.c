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
__IO uint16_t ADCValues[NUMBEROFADCHANNELS];
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
void timerUpdate(int timer, int speed);

// initialize DMA + ACD
void DMAACDinit(void);

// turns a LED on or off
void turnLED(int led, int state);

// turns the flashing light on or off
void flashlight(int state);

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

				// flashlight off
				flashlight(OFF);

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
	
				// flashlight off
				flashlight(OFF);

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
			// flashlight on
			if (strcmp(stringbuffer, "*f0on#") == 0)
			{
				// flashlight on
				flashlight(ON);

				// answer
				put_string("*f0on#");
			}
			else
			// flashlight off
			if (strcmp(stringbuffer, "*f0of#") == 0)
			{
				// flashlight off
				flashlight(OFF);

				// answer
				put_string("*f0of#");
			}
			else
			// READ_SENSOR_7 (24 V supply)
			if (strcmp(stringbuffer, "*s7#") == 0)
			{
				// send ADC value over serial port
				// max. 25 Volt = 4095
				sendUInt( ADCValues[0] );
			}
			else
			// READ_SENSOR_8 (12 V supply)
			if (strcmp(stringbuffer, "*s8#") == 0)
			{
				// send ADC value over serial port
				// max. 13 Volt = 4095
				sendUInt( ADCValues[1] );
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
				timerUpdate(MOTOR1, i);

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
			// MOTOR 2 SPEED SET
			if (strncmp(stringbuffer, "*mv2", 4) == 0)
			{
				// change first four chars for upcoming string conversion
				stringbuffer[0] = '0';
				stringbuffer[1] = '0';
				stringbuffer[2] = '0';
				stringbuffer[3] = '0';

				// convert to int
				i = atoi(stringbuffer);

				// set speed / PWM
				timerUpdate(MOTOR2, i);

				// answer with "ok"
				put_string("*mv2#");
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
			// MOTOR 3 SPEED SET
			if (strncmp(stringbuffer, "*mv3", 4) == 0)
			{
				// change first four chars for upcoming string conversion
				stringbuffer[0] = '0';
				stringbuffer[1] = '0';
				stringbuffer[2] = '0';
				stringbuffer[3] = '0';

				// convert to int
				i = atoi(stringbuffer);

				// set speed / PWM
				timerUpdate(MOTOR3, i);

				// answer with "ok"
				put_string("*mv3#");
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
			// MOTOR 4 SPEED SET
			if (strncmp(stringbuffer, "*mv4", 4) == 0)
			{
				// change first four chars for upcoming string conversion
				stringbuffer[0] = '0';
				stringbuffer[1] = '0';
				stringbuffer[2] = '0';
				stringbuffer[3] = '0';

				// convert to int
				i = atoi(stringbuffer);

				// set speed / PWM
				timerUpdate(MOTOR4, i);

				// answer with "ok"
				put_string("*mv4#");
			}
			else
			// RGB LED 1
			if (strncmp(stringbuffer, "*rgb1", 5) == 0)
			{
				// change first five chars for upcoming string conversion
				stringbuffer[0] = '0';
				stringbuffer[1] = '0';
				stringbuffer[2] = '0';
				stringbuffer[3] = '0';
				stringbuffer[4] = '0';

				// convert to int
				i = atoi(stringbuffer);

				// set speed / PWM
				timerUpdate(RGB1, i);

				// answer with "ok"
				put_string("*rgb1#");
			}
			else
			// RGB LED 2
			if (strncmp(stringbuffer, "*rgb2", 5) == 0)
			{
				// change first five chars for upcoming string conversion
				stringbuffer[0] = '0';
				stringbuffer[1] = '0';
				stringbuffer[2] = '0';
				stringbuffer[3] = '0';
				stringbuffer[4] = '0';

				// convert to int
				i = atoi(stringbuffer);

				// set speed / PWM
				timerUpdate(RGB2, i);

				// answer with "ok"
				put_string("*rgb2#");
			}
			else
			// RGB LED 3
			if (strncmp(stringbuffer, "*rgb3", 5) == 0)
			{
				// change first five chars for upcoming string conversion
				stringbuffer[0] = '0';
				stringbuffer[1] = '0';
				stringbuffer[2] = '0';
				stringbuffer[3] = '0';
				stringbuffer[4] = '0';

				// convert to int
				i = atoi(stringbuffer);

				// set speed / PWM
				timerUpdate(RGB3, i);

				// answer with "ok"
				put_string("*rgb3#");
			}
			else
			// RGB LED 4
			if (strncmp(stringbuffer, "*rgb4", 5) == 0)
			{
				// change first five chars for upcoming string conversion
				stringbuffer[0] = '0';
				stringbuffer[1] = '0';
				stringbuffer[2] = '0';
				stringbuffer[3] = '0';
				stringbuffer[4] = '0';

				// convert to int
				i = atoi(stringbuffer);

				// set speed / PWM
				timerUpdate(RGB4, i);

				// answer with "ok"
				put_string("*rgb4#");
			}
			else
			// RGB LED 5
			if (strncmp(stringbuffer, "*rgb5", 5) == 0)
			{
				// change first five chars for upcoming string conversion
				stringbuffer[0] = '0';
				stringbuffer[1] = '0';
				stringbuffer[2] = '0';
				stringbuffer[3] = '0';
				stringbuffer[4] = '0';

				// convert to int
				i = atoi(stringbuffer);

				// set speed / PWM
				timerUpdate(RGB5, i);

				// answer with "ok"
				put_string("*rgb5#");
			}
			else
			// RGB LED 6
			if (strncmp(stringbuffer, "*rgb6", 5) == 0)
			{
				// change first five chars for upcoming string conversion
				stringbuffer[0] = '0';
				stringbuffer[1] = '0';
				stringbuffer[2] = '0';
				stringbuffer[3] = '0';
				stringbuffer[4] = '0';

				// convert to int
				i = atoi(stringbuffer);

				// set speed / PWM
				timerUpdate(RGB6, i);

				// answer with "ok"
				put_string("*rgb6#");
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

	// flashing light off
	flashlight(OFF);

	// turn all drive motor bits off (except PWM bits)
	GPIO_ResetBits(MOTORPORT, MOTOR1BITA | MOTOR1BITB | MOTOR2BITA | MOTOR2BITB | MOTOR3BITA | MOTOR3BITB | MOTOR4BITA | MOTOR4BITB);

	// motor PWM speed
	timerUpdate(MOTOR1, MOTORPWMINITIALSPEED);
	timerUpdate(MOTOR2, MOTORPWMINITIALSPEED);
	timerUpdate(MOTOR3, MOTORPWMINITIALSPEED);
	timerUpdate(MOTOR4, MOTORPWMINITIALSPEED);

	// RGB PWM
	timerUpdate(RGB1, RGBPWMINITIALVALUE);
	timerUpdate(RGB2, RGBPWMINITIALVALUE);
	timerUpdate(RGB3, RGBPWMINITIALVALUE);
	timerUpdate(RGB4, RGBPWMINITIALVALUE);
	timerUpdate(RGB5, RGBPWMINITIALVALUE);
	timerUpdate(RGB6, RGBPWMINITIALVALUE);

	// @todo turn off RGB PWM to turn off the RGB LEDs...
}


void clockInit()
{
	// Enable Periph clock for LEDs
	RCC_AHB1PeriphClockCmd(LEDCLOCK	, ENABLE);

	// Enable Periph clock for flashlight
	RCC_AHB1PeriphClockCmd(FLASHLIGHTCLOCK, ENABLE);

	// Enable Periph clock for Motor bits
	RCC_AHB1PeriphClockCmd(MOTORCLOCK, ENABLE);

	// Timer clock enable for Motor PWM
	if (MOTOR1RCC_B1_OR_B2 == 1)
	{
		RCC_APB1PeriphClockCmd(MOTOR1PWMTIMCLOCK, ENABLE);
	}
	else
	{
		RCC_APB2PeriphClockCmd(MOTOR1PWMTIMCLOCK, ENABLE);
	}

	if (MOTOR2RCC_B1_OR_B2 == 1)
	{
		RCC_APB1PeriphClockCmd(MOTOR2PWMTIMCLOCK, ENABLE);
	}
	else
	{
		RCC_APB2PeriphClockCmd(MOTOR2PWMTIMCLOCK, ENABLE);
	}

	if (MOTOR3RCC_B1_OR_B2 == 1)
	{
		RCC_APB1PeriphClockCmd(MOTOR3PWMTIMCLOCK, ENABLE);
	}
	else
	{
		RCC_APB2PeriphClockCmd(MOTOR3PWMTIMCLOCK, ENABLE);
	}

	if (MOTOR4RCC_B1_OR_B2 == 1)
	{
		RCC_APB1PeriphClockCmd(MOTOR4PWMTIMCLOCK, ENABLE);
	}
	else
	{
		RCC_APB2PeriphClockCmd(MOTOR4PWMTIMCLOCK, ENABLE);
	}


	// Timer clock enable for RGB LED PWM
	if (RGB1RCC_B1_OR_B2 == 1)
	{
		RCC_APB1PeriphClockCmd(RGB1PWMTIMCLOCK, ENABLE);
	}
	else
	{
		RCC_APB2PeriphClockCmd(RGB1PWMTIMCLOCK, ENABLE);
	}

	// Timer clock enable for RGB LED PWM
	if (RGB2RCC_B1_OR_B2 == 1)
	{
		RCC_APB1PeriphClockCmd(RGB2PWMTIMCLOCK, ENABLE);
	}
	else
	{
		RCC_APB2PeriphClockCmd(RGB2PWMTIMCLOCK, ENABLE);
	}

	// Timer clock enable for RGB LED PWM
	if (RGB3RCC_B1_OR_B2 == 1)
	{
		RCC_APB1PeriphClockCmd(RGB3PWMTIMCLOCK, ENABLE);
	}
	else
	{
		RCC_APB2PeriphClockCmd(RGB3PWMTIMCLOCK, ENABLE);
	}

	// Timer clock enable for RGB LED PWM
	if (RGB4RCC_B1_OR_B2 == 1)
	{
		RCC_APB1PeriphClockCmd(RGB4PWMTIMCLOCK, ENABLE);
	}
	else
	{
		RCC_APB2PeriphClockCmd(RGB4PWMTIMCLOCK, ENABLE);
	}

	// Timer clock enable for RGB LED PWM
	if (RGB5RCC_B1_OR_B2 == 1)
	{
		RCC_APB1PeriphClockCmd(RGB5PWMTIMCLOCK, ENABLE);
	}
	else
	{
		RCC_APB2PeriphClockCmd(RGB5PWMTIMCLOCK, ENABLE);
	}

	// Timer clock enable for RGB LED PWM
	if (RGB6RCC_B1_OR_B2 == 1)
	{
		RCC_APB1PeriphClockCmd(RGB6PWMTIMCLOCK, ENABLE);
	}
	else
	{
		RCC_APB2PeriphClockCmd(RGB6PWMTIMCLOCK, ENABLE);
	}


	// Port clock enable for Motor PWM
	RCC_AHB1PeriphClockCmd(MOTOR1PWMPORTCLOCK | MOTOR2PWMPORTCLOCK | MOTOR3PWMPORTCLOCK | MOTOR4PWMPORTCLOCK, ENABLE);

	// Port clock enable for RGB LED PWM
	RCC_AHB1PeriphClockCmd(RGB1PWMPORTCLOCK | RGB2PWMPORTCLOCK | RGB3PWMPORTCLOCK | RGB4PWMPORTCLOCK | RGB5PWMPORTCLOCK | RGB6PWMPORTCLOCK, ENABLE);

	// Enable ADC, DMA and corresponding GPIO clocks
	RCC_AHB1PeriphClockCmd(SENSORDMACLOCK, ENABLE);
	RCC_AHB1PeriphClockCmd(SENSORPORTCLOCK, ENABLE);
	RCC_APB2PeriphClockCmd(SENSORADCCLOCK, ENABLE);
}


void gpioPortInit()
{
	GPIO_InitTypeDef GPIO_InitStructureLED;
	GPIO_InitTypeDef GPIO_InitStructureFlashlight;
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

	// GPIOPort init, (Flashlight)
	// Configure port bits in output pushpull mode
	GPIO_InitStructureFlashlight.GPIO_Pin = FLASHLIGHT;
	GPIO_InitStructureFlashlight.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructureFlashlight.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructureFlashlight.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructureFlashlight.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(FLASHLIGHTPORT, &GPIO_InitStructureFlashlight);

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
	PulseDurationInMicroSeconds = MOTORPWMINITIALSPEED;


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
		default:
			// error
			return;
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
		default:
			// error
			return;
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
		default:
			// error
			return;
	}

	// preload timer config
	TIM_ARRPreloadConfig(MOTOR3PWMTIMER, ENABLE);

	// enable timer / counter
	TIM_Cmd(MOTOR3PWMTIMER, ENABLE);


	//---------
	// motor 4
	//---------
	// Set PWM Port, Pin and method
	GPIO_InitStructureTimer.GPIO_Pin = MOTOR4PWMBIT;
	GPIO_InitStructureTimer.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructureTimer.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructureTimer.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructureTimer.GPIO_PuPd = GPIO_PuPd_UP ;
	GPIO_Init(MOTOR4PWMPORT, &GPIO_InitStructureTimer);

	// Connect TIM pin to Alternate Function (AF)
	GPIO_PinAFConfig(MOTOR4PWMPORT, MOTOR4PWMTIMBIT, MOTOR4PWMAF);

	// Timer base configuration
	TIM_TimeBaseStructure.TIM_Period = (uint16_t) (TimerCounterClock / TimerOutputClock);
	TIM_TimeBaseStructure.TIM_Prescaler = (uint16_t) ((SystemCoreClock /2) / TimerCounterClock) - 1;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

	// basic timer init
	TIM_TimeBaseInit(MOTOR4PWMTIMER, &TIM_TimeBaseStructure);

	// configure PWM mode and duration
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = PulseDurationInMicroSeconds; // set the duty cycle / pulse here!
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

	// Output Compare channels
	switch (MOTOR4PWMCHANNEL)
	{
		case 1:
			TIM_OC1Init(MOTOR4PWMTIMER, &TIM_OCInitStructure);
			TIM_OC1PreloadConfig(MOTOR4PWMTIMER, TIM_OCPreload_Enable);
			break;
		case 2:
			TIM_OC2Init(MOTOR4PWMTIMER, &TIM_OCInitStructure);
			TIM_OC2PreloadConfig(MOTOR4PWMTIMER, TIM_OCPreload_Enable);
			break;
		case 3:
			TIM_OC3Init(MOTOR4PWMTIMER, &TIM_OCInitStructure);
			TIM_OC3PreloadConfig(MOTOR4PWMTIMER, TIM_OCPreload_Enable);
			break;
		case 4:
			TIM_OC4Init(MOTOR4PWMTIMER, &TIM_OCInitStructure);
			TIM_OC4PreloadConfig(MOTOR4PWMTIMER, TIM_OCPreload_Enable);
			break;
		default:
			// error
			return;
	}

	// preload timer config
	TIM_ARRPreloadConfig(MOTOR4PWMTIMER, ENABLE);

	// enable timer / counter
	TIM_Cmd(MOTOR4PWMTIMER, ENABLE);


	//---------
	// RGB 1
	//---------
	// Set PWM Port, Pin and method
	GPIO_InitStructureTimer.GPIO_Pin = RGB1PWMBIT;
	GPIO_InitStructureTimer.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructureTimer.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructureTimer.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructureTimer.GPIO_PuPd = GPIO_PuPd_UP ;
	GPIO_Init(RGB1PWMPORT, &GPIO_InitStructureTimer);

	// Connect TIM pin to Alternate Function (AF)
	GPIO_PinAFConfig(RGB1PWMPORT, RGB1PWMTIMBIT, RGB1PWMAF);

	// Timer base configuration
	TIM_TimeBaseStructure.TIM_Period = (uint16_t) (TimerCounterClock / TimerOutputClock);
	TIM_TimeBaseStructure.TIM_Prescaler = (uint16_t) ((SystemCoreClock /2) / TimerCounterClock) - 1;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

	// basic timer init
	TIM_TimeBaseInit(RGB1PWMTIMER, &TIM_TimeBaseStructure);

	// configure PWM mode and duration
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = PulseDurationInMicroSeconds; // set the duty cycle / pulse here!
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

	// Output Compare channels
	switch (RGB1PWMCHANNEL)
	{
		case 1:
			TIM_OC1Init(RGB1PWMTIMER, &TIM_OCInitStructure);
			TIM_OC1PreloadConfig(RGB1PWMTIMER, TIM_OCPreload_Enable);
			break;
		case 2:
			TIM_OC2Init(RGB1PWMTIMER, &TIM_OCInitStructure);
			TIM_OC2PreloadConfig(RGB1PWMTIMER, TIM_OCPreload_Enable);
			break;
		case 3:
			TIM_OC3Init(RGB1PWMTIMER, &TIM_OCInitStructure);
			TIM_OC3PreloadConfig(RGB1PWMTIMER, TIM_OCPreload_Enable);
			break;
		case 4:
			TIM_OC4Init(RGB1PWMTIMER, &TIM_OCInitStructure);
			TIM_OC4PreloadConfig(RGB1PWMTIMER, TIM_OCPreload_Enable);
			break;
		default:
			// error
			return;
	}

	// preload timer config
	TIM_ARRPreloadConfig(RGB1PWMTIMER, ENABLE);

	// enable timer / counter
	TIM_Cmd(RGB1PWMTIMER, ENABLE);


	//---------
	// RGB 2
	//---------
	// Set PWM Port, Pin and method
	GPIO_InitStructureTimer.GPIO_Pin = RGB2PWMBIT;
	GPIO_InitStructureTimer.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructureTimer.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructureTimer.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructureTimer.GPIO_PuPd = GPIO_PuPd_UP ;
	GPIO_Init(RGB2PWMPORT, &GPIO_InitStructureTimer);

	// Connect TIM pin to Alternate Function (AF)
	GPIO_PinAFConfig(RGB2PWMPORT, RGB2PWMTIMBIT, RGB2PWMAF);

	// Timer base configuration
	TIM_TimeBaseStructure.TIM_Period = (uint16_t) (TimerCounterClock / TimerOutputClock);
	TIM_TimeBaseStructure.TIM_Prescaler = (uint16_t) ((SystemCoreClock /2) / TimerCounterClock) - 1;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

	// basic timer init
	TIM_TimeBaseInit(RGB2PWMTIMER, &TIM_TimeBaseStructure);

	// configure PWM mode and duration
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = PulseDurationInMicroSeconds; // set the duty cycle / pulse here!
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

	// Output Compare channels
	switch (RGB2PWMCHANNEL)
	{
		case 1:
			TIM_OC1Init(RGB2PWMTIMER, &TIM_OCInitStructure);
			TIM_OC1PreloadConfig(RGB2PWMTIMER, TIM_OCPreload_Enable);
			break;
		case 2:
			TIM_OC2Init(RGB2PWMTIMER, &TIM_OCInitStructure);
			TIM_OC2PreloadConfig(RGB2PWMTIMER, TIM_OCPreload_Enable);
			break;
		case 3:
			TIM_OC3Init(RGB2PWMTIMER, &TIM_OCInitStructure);
			TIM_OC3PreloadConfig(RGB2PWMTIMER, TIM_OCPreload_Enable);
			break;
		case 4:
			TIM_OC4Init(RGB2PWMTIMER, &TIM_OCInitStructure);
			TIM_OC4PreloadConfig(RGB2PWMTIMER, TIM_OCPreload_Enable);
			break;
		default:
			// error
			return;
	}

	// preload timer config
	TIM_ARRPreloadConfig(RGB2PWMTIMER, ENABLE);

	// enable timer / counter
	TIM_Cmd(RGB2PWMTIMER, ENABLE);


	//---------
	// RGB 3
	//---------
	// Set PWM Port, Pin and method
	GPIO_InitStructureTimer.GPIO_Pin = RGB3PWMBIT;
	GPIO_InitStructureTimer.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructureTimer.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructureTimer.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructureTimer.GPIO_PuPd = GPIO_PuPd_UP ;
	GPIO_Init(RGB3PWMPORT, &GPIO_InitStructureTimer);

	// Connect TIM pin to Alternate Function (AF)
	GPIO_PinAFConfig(RGB3PWMPORT, RGB3PWMTIMBIT, RGB3PWMAF);

	// Timer base configuration
	TIM_TimeBaseStructure.TIM_Period = (uint16_t) (TimerCounterClock / TimerOutputClock);
	TIM_TimeBaseStructure.TIM_Prescaler = (uint16_t) ((SystemCoreClock /2) / TimerCounterClock) - 1;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

	// basic timer init
	TIM_TimeBaseInit(RGB3PWMTIMER, &TIM_TimeBaseStructure);

	// configure PWM mode and duration
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = PulseDurationInMicroSeconds; // set the duty cycle / pulse here!
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

	// Output Compare channels
	switch (RGB3PWMCHANNEL)
	{
		case 1:
			TIM_OC1Init(RGB3PWMTIMER, &TIM_OCInitStructure);
			TIM_OC1PreloadConfig(RGB3PWMTIMER, TIM_OCPreload_Enable);
			break;
		case 2:
			TIM_OC2Init(RGB3PWMTIMER, &TIM_OCInitStructure);
			TIM_OC2PreloadConfig(RGB3PWMTIMER, TIM_OCPreload_Enable);
			break;
		case 3:
			TIM_OC3Init(RGB3PWMTIMER, &TIM_OCInitStructure);
			TIM_OC3PreloadConfig(RGB3PWMTIMER, TIM_OCPreload_Enable);
			break;
		case 4:
			TIM_OC4Init(RGB3PWMTIMER, &TIM_OCInitStructure);
			TIM_OC4PreloadConfig(RGB3PWMTIMER, TIM_OCPreload_Enable);
			break;
		default:
			// error
			return;
	}

	// preload timer config
	TIM_ARRPreloadConfig(RGB3PWMTIMER, ENABLE);

	// enable timer / counter
	TIM_Cmd(RGB3PWMTIMER, ENABLE);


	//---------
	// RGB 4
	//---------
	// Set PWM Port, Pin and method
	GPIO_InitStructureTimer.GPIO_Pin = RGB4PWMBIT;
	GPIO_InitStructureTimer.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructureTimer.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructureTimer.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructureTimer.GPIO_PuPd = GPIO_PuPd_UP ;
	GPIO_Init(RGB4PWMPORT, &GPIO_InitStructureTimer);

	// Connect TIM pin to Alternate Function (AF)
	GPIO_PinAFConfig(RGB4PWMPORT, RGB4PWMTIMBIT, RGB4PWMAF);

	// Timer base configuration
	TIM_TimeBaseStructure.TIM_Period = (uint16_t) (TimerCounterClock / TimerOutputClock);
	TIM_TimeBaseStructure.TIM_Prescaler = (uint16_t) ((SystemCoreClock /2) / TimerCounterClock) - 1;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

	// basic timer init
	TIM_TimeBaseInit(RGB4PWMTIMER, &TIM_TimeBaseStructure);

	// configure PWM mode and duration
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = PulseDurationInMicroSeconds; // set the duty cycle / pulse here!
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

	// Output Compare channels
	switch (RGB4PWMCHANNEL)
	{
		case 1:
			TIM_OC1Init(RGB4PWMTIMER, &TIM_OCInitStructure);
			TIM_OC1PreloadConfig(RGB4PWMTIMER, TIM_OCPreload_Enable);
			break;
		case 2:
			TIM_OC2Init(RGB4PWMTIMER, &TIM_OCInitStructure);
			TIM_OC2PreloadConfig(RGB4PWMTIMER, TIM_OCPreload_Enable);
			break;
		case 3:
			TIM_OC3Init(RGB4PWMTIMER, &TIM_OCInitStructure);
			TIM_OC3PreloadConfig(RGB4PWMTIMER, TIM_OCPreload_Enable);
			break;
		case 4:
			TIM_OC4Init(RGB4PWMTIMER, &TIM_OCInitStructure);
			TIM_OC4PreloadConfig(RGB4PWMTIMER, TIM_OCPreload_Enable);
			break;
		default:
			// error
			return;
	}

	// preload timer config
	TIM_ARRPreloadConfig(RGB4PWMTIMER, ENABLE);

	// enable timer / counter
	TIM_Cmd(RGB4PWMTIMER, ENABLE);


	//---------
	// RGB 5
	//---------
	// Set PWM Port, Pin and method
	GPIO_InitStructureTimer.GPIO_Pin = RGB5PWMBIT;
	GPIO_InitStructureTimer.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructureTimer.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructureTimer.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructureTimer.GPIO_PuPd = GPIO_PuPd_UP ;
	GPIO_Init(RGB5PWMPORT, &GPIO_InitStructureTimer);

	// Connect TIM pin to Alternate Function (AF)
	GPIO_PinAFConfig(RGB5PWMPORT, RGB5PWMTIMBIT, RGB5PWMAF);

	// Timer base configuration
	TIM_TimeBaseStructure.TIM_Period = (uint16_t) (TimerCounterClock / TimerOutputClock);
	TIM_TimeBaseStructure.TIM_Prescaler = (uint16_t) ((SystemCoreClock /2) / TimerCounterClock) - 1;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

	// basic timer init
	TIM_TimeBaseInit(RGB5PWMTIMER, &TIM_TimeBaseStructure);

	// configure PWM mode and duration
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = PulseDurationInMicroSeconds; // set the duty cycle / pulse here!
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

	// Output Compare channels
	switch (RGB5PWMCHANNEL)
	{
		case 1:
			TIM_OC1Init(RGB5PWMTIMER, &TIM_OCInitStructure);
			TIM_OC1PreloadConfig(RGB5PWMTIMER, TIM_OCPreload_Enable);
			break;
		case 2:
			TIM_OC2Init(RGB5PWMTIMER, &TIM_OCInitStructure);
			TIM_OC2PreloadConfig(RGB5PWMTIMER, TIM_OCPreload_Enable);
			break;
		case 3:
			TIM_OC3Init(RGB5PWMTIMER, &TIM_OCInitStructure);
			TIM_OC3PreloadConfig(RGB5PWMTIMER, TIM_OCPreload_Enable);
			break;
		case 4:
			TIM_OC4Init(RGB5PWMTIMER, &TIM_OCInitStructure);
			TIM_OC4PreloadConfig(RGB5PWMTIMER, TIM_OCPreload_Enable);
			break;
		default:
			// error
			return;
	}

	// preload timer config
	TIM_ARRPreloadConfig(RGB5PWMTIMER, ENABLE);

	// enable timer / counter
	TIM_Cmd(RGB5PWMTIMER, ENABLE);


	//---------
	// RGB 6
	//---------
	// Set PWM Port, Pin and method
	GPIO_InitStructureTimer.GPIO_Pin = RGB6PWMBIT;
	GPIO_InitStructureTimer.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructureTimer.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructureTimer.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructureTimer.GPIO_PuPd = GPIO_PuPd_UP ;
	GPIO_Init(RGB6PWMPORT, &GPIO_InitStructureTimer);

	// Connect TIM pin to Alternate Function (AF)
	GPIO_PinAFConfig(RGB6PWMPORT, RGB6PWMTIMBIT, RGB6PWMAF);

	// Timer base configuration
	TIM_TimeBaseStructure.TIM_Period = (uint16_t) (TimerCounterClock / TimerOutputClock);
	TIM_TimeBaseStructure.TIM_Prescaler = (uint16_t) ((SystemCoreClock /2) / TimerCounterClock) - 1;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

	// basic timer init
	TIM_TimeBaseInit(RGB6PWMTIMER, &TIM_TimeBaseStructure);

	// configure PWM mode and duration
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = PulseDurationInMicroSeconds; // set the duty cycle / pulse here!
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

	// Output Compare channels
	switch (RGB6PWMCHANNEL)
	{
		case 1:
			TIM_OC1Init(RGB6PWMTIMER, &TIM_OCInitStructure);
			TIM_OC1PreloadConfig(RGB6PWMTIMER, TIM_OCPreload_Enable);
			break;
		case 2:
			TIM_OC2Init(RGB6PWMTIMER, &TIM_OCInitStructure);
			TIM_OC2PreloadConfig(RGB6PWMTIMER, TIM_OCPreload_Enable);
			break;
		case 3:
			TIM_OC3Init(RGB6PWMTIMER, &TIM_OCInitStructure);
			TIM_OC3PreloadConfig(RGB6PWMTIMER, TIM_OCPreload_Enable);
			break;
		case 4:
			TIM_OC4Init(RGB6PWMTIMER, &TIM_OCInitStructure);
			TIM_OC4PreloadConfig(RGB6PWMTIMER, TIM_OCPreload_Enable);
			break;
		default:
			// error
			return;
	}

	// preload timer config
	TIM_ARRPreloadConfig(RGB6PWMTIMER, ENABLE);

	// enable timer / counter
	TIM_Cmd(RGB6PWMTIMER, ENABLE);
}


void timerUpdate(int timer, int speed)
{
	TIM_OCInitTypeDef	TIM_OCInitStructure;
	TIM_TypeDef* 		tim;
	int					channel;


	// are within a valid range?
	if ((speed<1) || (speed>99))
	{
		return;
	}

	// which timer?
	switch (timer)
	{
		case MOTOR1:
			tim = MOTOR1PWMTIMER;
			channel = MOTOR1PWMCHANNEL;
			break;
		case MOTOR2:
			tim = MOTOR2PWMTIMER;
			channel = MOTOR2PWMCHANNEL;
			break;
		case MOTOR3:
			tim = MOTOR3PWMTIMER;
			channel = MOTOR3PWMCHANNEL;
			break;
		case MOTOR4:
			tim = MOTOR4PWMTIMER;
			channel = MOTOR4PWMCHANNEL;
			break;
		case RGB1:
			tim = RGB1PWMTIMER;
			channel = RGB1PWMCHANNEL;
			break;
		case RGB2:
			tim = RGB2PWMTIMER;
			channel = RGB2PWMCHANNEL;
			break;
		case RGB3:
			tim = RGB3PWMTIMER;
			channel = RGB3PWMCHANNEL;
			break;
		case RGB4:
			tim = RGB4PWMTIMER;
			channel = RGB4PWMCHANNEL;
			break;
		case RGB5:
			tim = RGB5PWMTIMER;
			channel = RGB5PWMCHANNEL;
			break;
		case RGB6:
			tim = RGB6PWMTIMER;
			channel = RGB6PWMCHANNEL;
			break;
		default:
			// error
			return;
	}


	TIM_Cmd(tim, DISABLE);

	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = speed; // set the duty cycle / pulse here!
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

	// Output Compare channels
	switch (channel)
	{
		case 1:
			TIM_OC1Init(tim, &TIM_OCInitStructure);
			TIM_OC1PreloadConfig(tim, TIM_OCPreload_Enable);
			break;
		case 2:
			TIM_OC2Init(tim, &TIM_OCInitStructure);
			TIM_OC2PreloadConfig(tim, TIM_OCPreload_Enable);
			break;
		case 3:
			TIM_OC3Init(tim, &TIM_OCInitStructure);
			TIM_OC3PreloadConfig(tim, TIM_OCPreload_Enable);
			break;
		case 4:
			TIM_OC4Init(tim, &TIM_OCInitStructure);
			TIM_OC4PreloadConfig(tim, TIM_OCPreload_Enable);
			break;
		default:
			// error
			return;
	}

	// preload timer config
	TIM_ARRPreloadConfig(tim, ENABLE);

	// enable timer / counter
	TIM_Cmd(tim, ENABLE);
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
		ADCValues[n] = 0;
	}

	// to be safe we reset potentially enabled streams first
	DMA_DeInit(SENSORDMASTREAM);

	// DMA 2 Stream 0 channel 2 configuration **************************************
	DMA_InitStructure.DMA_Channel = SENSORDMACHANNEL;

	// Specifies the peripheral base address for DMAy Streamx
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) ADC3_DR_ADDRESS;

	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t) &ADCValues; // this will hold the converted values
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


void flashlight(int state)
{
	if (state == ON)
	{
		GPIO_SetBits(FLASHLIGHTPORT, FLASHLIGHT);
	}
	else
	{
		GPIO_ResetBits(FLASHLIGHTPORT, FLASHLIGHT);
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
