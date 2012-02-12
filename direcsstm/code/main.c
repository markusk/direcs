#include <string.h>
#include <stdio.h>

#include "stm32f4_discovery.h"
#include "stm32f4xx_conf.h"



// The port bits -------------------------------------------------------------
#define MOTOR1PERIPH			RCC_AHB1Periph_GPIOB
#define MOTOR1PORT				GPIOB
#define MOTOR1BITA				GPIO_Pin_0
#define MOTOR1BITB				GPIO_Pin_1

// this code works fine.
// now issues with PD12 when using this timer and portbit

/* not okay !! 
// TIM4, PB7
#define MOTORPWMTIMER			TIM4
#define MOTORPWMTIMCLOCK		RCC_APB1Periph_TIM4
#define MOTORPWMPORTCLOCK		RCC_AHB1Periph_GPIOB
#define MOTORPWMAF 				GPIO_AF_TIM4
#define MOTORPWMPORT			GPIOB
#define MOTORPWMBIT				GPIO_Pin_7
#define MOTORPWMTIMBIT			GPIO_PinSource7
*/

/* not okay !! 
// TIM2, PB11 -> does not work. Why? TIM2 and 5 are 23 bit timers... hm...
#define MOTORPWMTIMER			TIM2
#define MOTORPWMTIMCLOCK		RCC_APB1Periph_TIM2
#define MOTORPWMPORTCLOCK		RCC_AHB1Periph_GPIOB
#define MOTORPWMAF 				GPIO_AF_TIM2
#define MOTORPWMPORT			GPIOB
#define MOTORPWMBIT				GPIO_Pin_11
#define MOTORPWMTIMBIT			GPIO_PinSource11
*/

// ok, too: (branch)
// TIM3, PB4
#define MOTORPWMTIMER			TIM3
#define MOTORPWMTIMCLOCK		RCC_APB1Periph_TIM3
#define MOTORPWMPORTCLOCK		RCC_AHB1Periph_GPIOB
#define MOTORPWMAF 				GPIO_AF_TIM3
#define MOTORPWMPORT			GPIOB
#define MOTORPWMBIT				GPIO_Pin_4
#define MOTORPWMTIMBIT			GPIO_PinSource4

/* ok:
// TIM3, PC6
#define MOTORPWMTIMER			TIM3
#define MOTORPWMTIMCLOCK		RCC_APB1Periph_TIM3
#define MOTORPWMPORTCLOCK		RCC_AHB1Periph_GPIOC
#define MOTORPWMAF 				GPIO_AF_TIM3
#define MOTORPWMPORT			GPIOC
#define MOTORPWMBIT				GPIO_Pin_6
#define MOTORPWMTIMBIT			GPIO_PinSource6
*/


// the prototypes ------------------------------------------------------------
int main(void);

// init of the several system clocks
void clockInit();

// Timer init for PWM
void TimerInit(void);

void Delay(__IO uint32_t nCount);



int main(void)
{
	// init all system
	clockInit();
	
	// init timer for PWM
	TimerInit();

	while (1);

}


void clockInit()
{
	// Periph clock enable for Motor bits
//	RCC_AHB1PeriphClockCmd(MOTOR1PERIPH, ENABLE);

	// Timer clock enable for Motor PWM
	RCC_APB1PeriphClockCmd(MOTORPWMTIMCLOCK, ENABLE);

	// Port clock enable for Motor PWM
	RCC_AHB1PeriphClockCmd(MOTORPWMPORTCLOCK, ENABLE);
}


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


void Delay(__IO uint32_t nCount)
{
	while(nCount--)
	{
	}
}
