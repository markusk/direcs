#ifndef MAIN_H
#define MAIN_H

#include "stm32f4xx_conf.h"

#define MOTORPORT				GPIOB
#define MOTOR1BITA				GPIO_Pin_0
#define MOTOR1BITB				GPIO_Pin_1
#define MOTOR2BITA				GPIO_Pin_2
#define MOTOR2BITB				GPIO_Pin_4
#define MOTOR3BITA				GPIO_Pin_5
#define MOTOR3BITB				GPIO_Pin_7
#define MOTOR4BITA				GPIO_Pin_8
#define MOTOR4BITB				GPIO_Pin_12
#define MOTORCLOCK				RCC_AHB1Periph_GPIOB


#endif