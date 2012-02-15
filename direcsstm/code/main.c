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


// The port bits -------------------------------------------------------------
// ADC PC0 for battery voltage sensor
#define SENSOR7DMACHANNEL		DMA_Channel_2;
#define SENSOR7PORT				GPIOC
#define SENSOR7PIN				GPIO_Pin_0
#define SENSOR7CLOCK			RCC_AHB1Periph_GPIOC
#define SENSOR7DMACLOCK			RCC_AHB1Periph_DMA2
#define SENSOR7ADCCLOCK			RCC_APB2Periph_ADC3



// ADC DMA stuff
#define ADC3_DR_ADDRESS	((uint32_t)0x4001244C)


// Private variables ---------------------------------------------------------
__IO uint16_t ADC3ConvertedValue = 0;
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

// initialize DMA
void DMAinit(void);

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
	
	// init DMA
	DMAinit();

	// Start ADC3 Software Conversion
	ADC_SoftwareStartConv(ADC3);


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
			// READ_SENSOR_7 (24 V supply)
			if (strcmp(stringbuffer, "*s7#") == 0)
			{
				// convert the ADC value (from 0 to 0xFFF) to a voltage value (from 0V to 3.3V)
				ADC3ConvertedVoltage = ADC3ConvertedValue * 3300 / 0xFFF;

				i = ADC3ConvertedVoltage;

				// read ADC and send answer over serial port
				sendUInt( i );
			}
		} // stringReceived()

	} // while (1)
}


void clockInit()
{
	// Port clock enable for ADC for battery voltage
	RCC_AHB1PeriphClockCmd(SENSOR7CLOCK, ENABLE);

	// ADC clock enable for battery voltage
	RCC_APB2PeriphClockCmd(SENSOR7ADCCLOCK, ENABLE);

	// DMA clock for ADC
	RCC_AHB1PeriphClockCmd(SENSOR7DMACLOCK, ENABLE);
}


void gpioPortInit()
{
	GPIO_InitTypeDef GPIO_InitStructureADC;

/*
	// Configure ADC channel as anlog input
	GPIO_InitStructureADC.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_InitStructureADC.GPIO_Pin = SENSOR7PIN;
	GPIO_InitStructureADC.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(SENSOR7PORT, &GPIO_InitStructureADC);
*/
}


void DMAinit(void)
{
	ADC_InitTypeDef       ADC_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	DMA_InitTypeDef       DMA_InitStructure;
	GPIO_InitTypeDef      GPIO_InitStructure;

	/* Enable ADC3, DMA2 and GPIO clocks ****************************************/
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2 | RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3, ENABLE);

	/* DMA2 Stream0 channel0 configuration **************************************/
	DMA_InitStructure.DMA_Channel = DMA_Channel_2;  
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) ADC3_DR_ADDRESS;
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t) &ADC3ConvertedValue; // this will hold the value
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
	DMA_InitStructure.DMA_BufferSize = 1;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_Init(DMA2_Stream0, &DMA_InitStructure);
	DMA_Cmd(DMA2_Stream0, ENABLE);

	/* Configure ADC3 Channel12 pin as analog input ******************************/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	/* ADC Common Init **********************************************************/
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
	ADC_CommonInit(&ADC_CommonInitStructure);

	/* ADC3 Init ****************************************************************/
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfConversion = 1;
	ADC_Init(ADC3, &ADC_InitStructure);

	/* ADC3 regular channel12 configuration *************************************/
	ADC_RegularChannelConfig(ADC3, ADC_Channel_12, 1, ADC_SampleTime_3Cycles);

	/* Enable DMA request after last transfer (Single-ADC mode) */
	ADC_DMARequestAfterLastTransferCmd(ADC3, ENABLE);

	/* Enable ADC3 DMA */
	ADC_DMACmd(ADC3, ENABLE);

	/* Enable ADC3 */
	ADC_Cmd(ADC3, ENABLE);
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
	static const char digits[] = "0123456789";  /* legal digits in order */
	unsigned val=0;         /* value we're accumulating */
	int neg=0;              /* set to true if we see a minus sign */

	/* skip whitespace */
	while (*s==' ' || *s=='\t')
	{
		s++;
	}

	/* check for sign */
	if (*s=='-')
	{
		neg=1;
		s++;
	}
	else if (*s=='+')
	{
		s++;
	}

	/* process each digit */
	while (*s)
	{
		const char *where;
		unsigned digit;

		/* look for the digit in the list of digits */
		where = strchr(digits, *s);
		if (where==NULL)
		{
			/* not found; not a digit, so stop */
			break;
		}

		/* get the index into the digit list, which is the value */
		digit = (where - digits);

		/* could (should?) check for overflow here */

		/* shift the number over and add in the new digit */
		val = val*10 + digit;

		/* look at the next character */
		s++;
	}

	/* handle negative numbers */
	if (neg)
	{
		return -val;
	}

	/* done */
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
