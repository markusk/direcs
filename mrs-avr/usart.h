#ifndef USART_H
#define USART_H

#ifndef F_CPU
#define F_CPU 16000000
#endif

#ifndef USART_BAUD_RATE
#define USART_BAUD_RATE 9600
#endif

#define USART_BAUD_SELECT (F_CPU / (USART_BAUD_RATE * 16L) - 1)
#define CR 13
#define LF 10


#include <avr/io.h>


	 void UsartInit(void);
	 void UsartTransmit(uint16_t c);
	 void UsartTransmitString(unsigned char *string);
unsigned char UsartReceive(void);


#endif
