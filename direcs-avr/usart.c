#include "usart.h"
#include "main.h"


void UsartInit(void)
{
	// Enable UART 3
	//
	// RXCIE: cause interrupt when a character is received from the UART
	UCSR3B |= /*(1 << RXCIE3) |*/ (1 << RXEN3) | (1 << TXEN3);
	// TXCIE: cause interrupt when a character is sent from the UART
	/*UCSR3B &= ~(1 << TXCIE3);*/
	
	UBRR3H = (unsigned char) (USART_BAUD_SELECT >> 8);
	UBRR3L = (unsigned char) USART_BAUD_SELECT;
}


void UsartTransmit(uint16_t c)
{
	while(!(UCSR3A & (1<<UDRE3))) {}
	
	UDR3 = c;
}


void UsartTransmitString(unsigned char *string)
{
	while(!(UCSR3A & (1<<UDRE3))) {}
	
	while(*string)
	{
		UsartTransmit(*string++);
	}
}


unsigned char UsartReceive(void)
{
	while(!(UCSR3A & (1<<RXC3)))
	{
		// TODO: what is with timeout?
		
		
		// check if USB is connected and turn green LED on or off
		if (bit_is_set(PINE, PIN5))
		{
			greenLED(ON);
		}
		else
		{
			greenLED(OFF);
		}
	}
	
	return UDR3;
}
