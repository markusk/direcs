#ifndef MAIN_H
#define MAIN_H

// #include "usart.h"    // serial stuff


// Private typedef -----------------------------------------------------------
GPIO_InitTypeDef  GPIO_InitStructure;

// stores the serial received command and the string which will be sent as an answer
char stringbuffer[64];

//
// the prototypes
//
int main(void);
void usartInit(void);
void Delay(__IO uint32_t nCount);


#endif
