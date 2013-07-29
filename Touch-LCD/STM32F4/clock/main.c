/**
*****************************************************************************
**
**  File        : main.c
**
**  Abstract    : main function.
**
**  Functions   : main
**
**  Environment : Atollic TrueSTUDIO/STM32
**                STMicroelectronics STM32F10x Standard Peripherals Library
**
**  Distribution: The file is distributed “as is,” without any warranty
**                of any kind.
**
**  (c)Copyright Atollic AB.
**  You may use this file as-is or modify it according to the needs of your
**  project. Distribution of this file (unmodified or modified) is not
**  permitted. Atollic AB permit registered Atollic TrueSTUDIO(R) users the
**  rights to distribute the assembled, compiled & linked contents of this
**  file as part of an application binary file, provided that it is built
**  using the Atollic TrueSTUDIO(R) toolchain.
**
**
*****************************************************************************
*/

/* Includes ---------------------------------------------------------------*/
#include "main.h"

/**
**===========================================================================
**
**  Abstract: main program
**
**===========================================================================
*/
int main(void)
{
	if (SysTick_Config(SystemCoreClock / 1000))
	{
	    /* Capture error */
	    while (1);
	}

	/* Initialisiere SPI2 */
	D_SPI_Init();
	/* Initialisiere Display */
	lcd_reset();
	/* Hintergrundbeleuchtung einschalten */
	D_SPI_LEDHigh();
	/* SPI Speed auf Max einstellen */
	D_SPI_ChangeSPISpeed(SPI_BaudRatePrescaler_2);



	lcd_clear(COLOR_BLACK);

	/*
	// Beispiele für die TFT Lib
	lcd_fillRect(0,0,50,50, COLOR_RED);
	lcd_fillRect(55,0,105,50, COLOR_BLUE);
	lcd_fillRect(110,0,160,50, COLOR_GREEN);
	lcd_drawRect(165,0,215,100, COLOR_BLACK);


	lcd_verticalLine(10, 120, 60, COLOR_RED);
	lcd_verticalLine(20, 60, 120, COLOR_BLACK);

	lcd_horizontalLine(0, 130, 110, COLOR_BLUE);
	lcd_horizontalLine(0, 80, 60, COLOR_YELLOW);

	lcd_horizontalLine(0, 200, 110, COLOR_GREEN);
	lcd_horizontalLine(120, 200, 60, COLOR_YELLOW);

	lcd_drawLine(160, 120, 200, 90, COLOR_BLACK);

	lcd_drawCircle(160, 120, 55, COLOR_BLACK);
	lcd_fillCircle(250, 40, 20, COLOR_RED);

	lcd_drawChar(10, 6, 'H', FONT8X12, COLOR_WHITE, COLOR_RED);
	lcd_drawChar(18, 6, 'i', FONT8X12, COLOR_WHITE, COLOR_RED);
	lcd_drawChar(26, 6, ' ', FONT8X12, COLOR_WHITE, COLOR_RED);
	lcd_drawChar(34, 6, '!', FONT8X12, COLOR_WHITE, COLOR_RED);

	lcd_drawString(10, 18, "Seb is", FONT8X8, COLOR_WHITE, COLOR_RED);
	lcd_drawString(10, 26, "awesome", FONT8X8, COLOR_WHITE, COLOR_RED);

	lcd_drawInt(10, 34, -1234, FONT8X8, COLOR_BLUE, COLOR_RED);
	lcd_drawHex(10, 42, 4154, FONT8X12, COLOR_BLUE, COLOR_RED);
	*/

	sek = 0;
	min = 20;
	hour = 14;
	flag = 1;

	Init_Timer2();
	NVIC_Configuration();

	/* Infinite loop */
	while (1)
	{
		//Bildschirm wird refresht
		displayClock();
	}
}

void displayClock(){
	unsigned char bsek, bmin, bhour;
	char buffer[8];

	if(flag){

		if(sek == 0){
			bsek = 59;
			if(min == 0){
				bmin = 59;
				if(hour == 0)
					bhour = 23;
				else
					bhour = hour -1;
			}
			else
				bmin = min -1;
		}
		else{
				bsek = sek -1;
		}

		//lcd_drawCircle(160, 120, 55, COLOR_WHITE);

		//Stundenzeiger
		lcd_degreeLine(160, 120, 90 - bhour*30, 45, COLOR_BLACK);
		lcd_degreeLine(160, 120, 90 - hour*30, 45, COLOR_WHITE);

		//Minutenzeiger
		lcd_degreeLine(160, 120, 90 - bmin*6, 45, COLOR_BLACK);
		lcd_degreeLine(160, 120, 90 - min*6, 45, COLOR_WHITE);

		//Sekundenzeiger
		lcd_degreeLine(160, 120, 90 - bsek*6, 45, COLOR_BLACK);
		lcd_degreeLine(160, 120, 90 - sek*6, 45, COLOR_WHITE);

		flag = 0;

		sprintf(buffer, "%02i:%02i:%02i", hour, min, sek);

		lcd_drawString(128, 178, buffer, FONT8X8, COLOR_WHITE, COLOR_BLACK);
		}

}

/**
 * @brief Konfiguriert Timer2
 *
 * Konfiguriert den Timer2
 */
void Init_Timer2(){
	TIM_TimeBaseInitTypeDef Timer_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

	Timer_InitStructure.TIM_Prescaler = 366;
	Timer_InitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	Timer_InitStructure.TIM_Period = 0xFFFF;
	Timer_InitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	Timer_InitStructure.TIM_RepetitionCounter = 0x0000;

	TIM_TimeBaseInit(TIM2, &Timer_InitStructure);
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM2, ENABLE);

}

/**
 * @brief Konfiguriert den Nested Vector Interrupt Controller
 *
 * Aktiviert den TIM2 Interrupt
 */
void NVIC_Configuration(){
  NVIC_InitTypeDef NVIC_InitStructure;

  /* Enable the TIM2 gloabal Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

  NVIC_Init(&NVIC_InitStructure);
}


/**
  * @brief  Delay
  * @param  nTime Delay in Millisekunden
  */
void Delay(__IO uint32_t nTime)
{
  TimingDelay = nTime;

  while(TimingDelay != 0);
}

/**
  * @brief  Verringert um 1s die TimingDelay Variable
  */
void TimingDelay_Decrement(void)
{
  if (TimingDelay != 0x00)
  {
    TimingDelay--;
  }
}


/*---------------------------------------------------------------------------------------------------------------------*/
/*
 * Minimal __assert_func used by the assert() macro
 * */
void __assert_func(const char *file, int line, const char *func, const char *failedexpr)
{
  while(1)
  {}
}

/*
 * Minimal __assert() uses __assert__func()
 * */
void __assert(const char *file, int line, const char *failedexpr)
{
   __assert_func (file, line, NULL, failedexpr);
}
