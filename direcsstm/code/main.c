#include <string.h>
#include <stdio.h>

#include "stm32f4_discovery.h"
#include "stm32f4xx_conf.h"



// The port bits -------------------------------------------------------------

// okay, and ~50 micro secs
#define MOTORPWMTIMER			TIM2
#define MOTORPWMAF 				GPIO_AF_TIM2
#define MOTORPWMTIMCLOCK		RCC_APB1Periph_TIM2
#define B1_OR_B2_TIMPERIPH		1
#define	MOTORPWMCHANNEL			4
#define MOTORPWMPORT			GPIOB
#define MOTORPWMPORTCLOCK		RCC_AHB1Periph_GPIOB
#define MOTORPWMBIT				GPIO_Pin_11
#define MOTORPWMTIMBIT			GPIO_PinSource11

/* okay, ~22 micro secs
#define MOTORPWMTIMER			TIM1
#define MOTORPWMAF 				GPIO_AF_TIM1
#define MOTORPWMTIMCLOCK		RCC_APB2Periph_TIM1
#define B1_OR_B2_TIMPERIPH		1
#define	MOTORPWMCHANNEL			1
#define MOTORPWMPORT			GPIOE
#define MOTORPWMPORTCLOCK		RCC_AHB1Periph_GPIOE
#define MOTORPWMBIT				GPIO_Pin_9
#define MOTORPWMTIMBIT			GPIO_PinSource9
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

	while (1)
	{
	}
}


void clockInit()
{
	// Timer clock enable for Motor PWM
	if (B1_OR_B2_TIMPERIPH == 1)
	{
		RCC_APB1PeriphClockCmd(MOTORPWMTIMCLOCK, ENABLE);
	}

	if (B1_OR_B2_TIMPERIPH == 2)
	{
		RCC_APB2PeriphClockCmd(MOTORPWMTIMCLOCK, ENABLE);
	}

	// Port clock enable for Motor PWM
	RCC_AHB1PeriphClockCmd(MOTORPWMPORTCLOCK, ENABLE);
}


void TimerInit(void)
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
//	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure); // what for?

	TIM_TimeBaseStructure.TIM_Period = (uint16_t) (TimerCounterClock / TimerOutputClock);
	TIM_TimeBaseStructure.TIM_Prescaler = (uint16_t) ((SystemCoreClock /2) / TimerCounterClock) - 1;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;

	// this feature is only valid for TIM1 and TIM8
	if ((MOTORPWMTIMER == TIM1) || (MOTORPWMTIMER == TIM8))
	{
		TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	}

	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

	// basic timer init
	TIM_TimeBaseInit(MOTORPWMTIMER, &TIM_TimeBaseStructure);


	// configure PWM mode and duration
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = PulseDurationInMicroSeconds; // set the duty cycle / pulse here!
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

	// these feature is only valid for TIM1 and TIM8
	if ((MOTORPWMTIMER == TIM1) || (MOTORPWMTIMER == TIM8))
	{
		TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;  // Specifies the TIM complementary Output Compare state.
		TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;   // Specifies the complementary output polarity. < < < not in use, so not relevant!
		TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;   // Specifies the TIM Output Compare pin state during Idle state.
		TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset; // Specifies the TIM Output Compare pin state during Idle state.
	}


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

	// these feature is only valid for TIM1 and TIM8
	if ((MOTORPWMTIMER == TIM1) || (MOTORPWMTIMER == TIM8))
	{
		// Main Output Enable
		TIM_CtrlPWMOutputs(MOTORPWMTIMER, ENABLE);
	}
}


void Delay(__IO uint32_t nCount)
{
	while(nCount--)
	{
	}
}
