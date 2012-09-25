/*
 * main.h
 *
 *  Created on: Dec 17, 2010
 *      Author: Sebastian M.
 */

#ifndef MAIN_H_
#define MAIN_H_

/* Includes */
#include <stddef.h>
#include <math.h>
#include "stm32f10x.h"
#include "tft.h"
#include "D_HaLayer.h"

/* Private typedef */
/* Private define  */
/* Private macro */
/* Private variables */
volatile unsigned char sek;
volatile unsigned char min;
volatile unsigned char hour;

volatile unsigned char flag;

static __IO uint32_t TimingDelay;
/* Private function prototypes */
void displayClock(void);
void Init_Timer2(void);
void NVIC_Configuration(void);
void TimingDelay_Decrement(void);
void Delay(__IO uint32_t nTime);
/* Private functions */

#endif /* MAIN_H_ */
