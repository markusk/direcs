#include "stm32f4xx.h"
#include "config.h"
#include "mi0283qt.h"


volatile uint16_t millisec_countdown = 0;

int main(void)
{

	int i = 0;

	/* TODO - Add your application code here */

	SysTick_Config(21000);

	init_mi0283qt(SPI2);

	lcd_testscreen();
	/* Infinite loop */
	while (1)
	{
		i++;
	}
}

/*
void SysTick_Handler(void)
{
	if(millisec_countdown) millisec_countdown--;
}
*/