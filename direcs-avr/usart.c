#include "usart.h"
#include "main.h"


void UsartInit(void)
{
	// no interrupts please!  this is *here* for setting the interrupt control registers.
//	cli();
	
	// Enable UART 3
	//
	// RXCIE: cause interrupt when a character is received from the UART
	UCSR3B |= /*(1 << RXCIE3) | */(1 << RXEN3) | (1 << TXEN3);

	// TXCIE: cause interrupt when a character is sent from the UART
	// UCSR3B &= ~(1 << TXCIE3);

	// enable global interrupts
//	sei();
	
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

/*
ISR(USART3_UDRE_vect)
{
	// UART RX 3 complete interrupt
	// this reoutine is executed, when the Receive Complete Interrupt Enable (RXCIE3) is set.
	//
	// all available data have must be read. Otherwise a new interrupt occur oce this interrupt routine is left!


	// Daten auslesen, dadurch wird das Interruptflag gelöscht
	uart_rx_data = UDRE3;
}
*/


void put_string(char *daten)
{
	if (uart_tx_flag == 1)
	{
		// String daten ind en Sendepuffer kopieren
		strcpy(uart_tx_buffer, daten);      
		// Flag für 'Senden ist komplett' löschen, 
		uart_tx_flag = 0;                    
		// UDRE Interrupt einschalten, los gehts
		UCSR3B |= (1<<UDRIE3); 
   }
}
