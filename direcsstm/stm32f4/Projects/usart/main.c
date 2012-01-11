#include "main.h"


/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
	/*!< At this stage the microcontroller clock setting is already configured, 
		 this is done through SystemInit() function which is called from startup
		 file (startup_stm32f4xx.s) before to branch to application main.
		 To reconfigure the default setting of SystemInit() function, refer to
		  system_stm32f4xx.c file
	*/

	// /dev/tty.usbserial-A900J1T0

	// call my new USART init
	usartInit();

	//	--------------------------------------------------------------------------------
	//	GPIOPort init
	// 	Don't know why, but this code has to be here (not in a seperate method)
	//	--------------------------------------------------------------------------------
	// GPIOD Periph clock enable
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

	// Configure PD12, PD13, PD14 and PD15 in output pushpull mode
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13| GPIO_Pin_14| GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	//	--------------------------------------------------------------------------------


	while (1)
	{
		serialBlockingReadString();
/*
		uint16_t buchstabe = 0;


		// blocking read on serial port USART2
		while (USART_GetFlagStatus(USART2, USART_FLAG_RXNE) == RESET)
		{
		}

		buchstabe = USART_ReceiveData(USART2);
		
		// build string
		receiveChar(buchstabe);
*/

		if (RXcompleted)
		{
			// Get complete string and delete uart_rx_flag
			get_string(stringbuffer);


			// RESET / INIT
			if (strcmp(stringbuffer, "*re#") == 0)
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
		} // RXcompleted

	}
}


void gpioPortInit()
{
	/* GPIOD Periph clock enable */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

	/* Configure PD12, PD13, PD14 and PD15 in output pushpull mode */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13| GPIO_Pin_14| GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
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
