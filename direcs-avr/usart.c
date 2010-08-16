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


void get_string(char *daten)
{
 
   if (uart_rx_flag==1)
   {
      // String kopieren
      strcpy(daten, uart_rx_buffer);      
      // Flag löschen
      uart_rx_flag = 0;                    
   }
}


// UART RX complete interrupt
// hier werden Daten vom PC empfangen und in einem String zwischengespeichert
// Wird ein Stringterminator empfangen, wird ein Flag gesetzt, welches dem 
// Hauptprogramm den kompletten Empfang signalisiert
ISR(USART3_RX_vect)
{
    static uint8_t uart_rx_cnt;     // Zähler für empfangene Zeichen
    uint8_t data;
 
    // Daten auslesen, dadurch wird das Interruptflag gelöscht              
    data = UDR3;
    
    // Ist Puffer frei für neue Daten?
    if (!uart_rx_flag)
    {
        // ja, ist Ende des Strings (RETURN) erreicht?
        if (data=='\r')
        {
            // ja, dann String terminieren
            uart_rx_buffer[uart_rx_cnt]=0;
            // Flag für 'Empfangspuffer voll' setzen
            uart_rx_flag=1;
            // Zähler zurücksetzen
            uart_rx_cnt=0;
        }
        else if (uart_rx_cnt<(uart_buffer_size-1))
        {     
            // Daten in Puffer speichern
            // aber durch if() Pufferüberlauf vermeiden
            uart_rx_buffer[uart_rx_cnt]=data;
            uart_rx_cnt++; // Zähler erhöhen
        }
    }
}


// UART TX data register empty interrupt
// hier werden neue Daten in das UART-Senderegister geladen
ISR(USART3_UDRE_vect)
{
    // Zeiger auf Sendepuffer
    static char* uart_tx_p = uart_tx_buffer;
    uint8_t data;
 
    // zu sendendes Zeichen lesen,
    // Zeiger auf Sendepuffer erhöhen
    data = *uart_tx_p++;
    
    // Ende des nullterminierten Strings erreicht?
    if (data==0 )
    {
        UCSR3B &= ~(1<<UDRIE3);     // ja, dann UDRE Interrupt ausschalten
        uart_tx_p = uart_tx_buffer; // Pointer zurücksetzen
        uart_tx_flag = 1;           // Flag setzen, Übertragung beeendet
    }
    else
    {
    	UDR3 = data;               // nein, Daten senden
    }
}
