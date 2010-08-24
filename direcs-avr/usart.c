#include "usart.h"
#include "main.h"


void setStarter(uint8_t startr)
{
//	static uint8_t initDone = 0;
	
	// this terminates a string with a '#'
	starter = startr;
}


void setTerminator(uint8_t termi)
{
//	static uint8_t initDone = 0;
	
	// this terminates a string with a '#'
	terminator = termi;
}


void put_string(char *daten)
{
	if (uart_tx_flag == 1)
	{
		// String daten ind en Sendepuffer kopieren
		strcpy(uart_tx_buffer, daten);      

		// replace string terminator \0 by my own terminator
		// this terminator is checked when the string is send.
		// uart_tx_buffer[strlen(uart_tx_buffer)] = terminator;

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

		// add a usual string terminator
		// daten[strlen(daten)] = '\0';

		// Flag löschen
		uart_rx_flag = 0;                    
		redLED(OFF);
	}
}


// UART RX complete interrupt
// hier werden Daten vom PC empfangen und in einem String zwischengespeichert
// Wird ein Stringterminator empfangen, wird ein Flag gesetzt, welches dem 
// Hauptprogramm den kompletten Empfang signalisiert
ISR(USART3_RX_vect)
{
	static uint8_t uart_rx_cnt;     	// Zähler für empfangene Zeichen
	static uint8_t string_started = 0;	// Sind wir jetzt im String?
	uint8_t data;

 
	// Daten auslesen, dadurch wird das Interruptflag gelöscht              
	data = UDR3;

	// toggling the red LED on and off with every received serial commmand
	if (redLEDtoggle == 0)
	{
		redLEDtoggle = 1;
	}
	else
	{
		redLEDtoggle = 0;
	}
	
	redLED(redLEDtoggle);


	// Ist Puffer frei für neue Daten?
	if (!uart_rx_flag)
	{
		// Puffer voll?
		if (uart_rx_cnt < (uart_buffer_size-1))
		{     
			// string start
			// string speichern, wenn mit 'starter' begonnen!
			if  (data == starter)
			{
				string_started = 1;
				greenLED(ON);
				// Daten in Puffer speichern
				uart_rx_cnt = 0;
				uart_rx_buffer[uart_rx_cnt] = data;
				uart_rx_cnt++; // Zähler erhöhen
				return;
			}

			// string stop
			// Ist das Ende des Strings (RETURN) erreicht?
			if (data == terminator)
			{
				// ja, dann String terminieren
				uart_rx_buffer[uart_rx_cnt] = terminator;
				// Flag für 'Empfangspuffer voll' setzen
				uart_rx_flag = 1;
				// Zähler zurücksetzen
				uart_rx_cnt = 0;
				// green LED off
				greenLED(OFF);
				string_started = 0;
				return;
			}

			// string middle
			// string nur speichern, wenn zuvor der starter mal war.
			if  (string_started == 1)
			{
				// Daten in Puffer speichern
				uart_rx_buffer[uart_rx_cnt] = data;
				uart_rx_cnt++; // Zähler erhöhen
				return;
			}
		}
		else
		{
			// Puffer ist vollgelaufen!
			// (Gleicher code wie terminator empfangen!)
			// ja, dann String terminieren
			uart_rx_buffer[uart_rx_cnt] = terminator;
			// Flag für 'Empfangspuffer voll' setzen
			uart_rx_flag = 1;
			// Zähler zurücksetzen
			uart_rx_cnt = 0;
			// green LED off
			greenLED(OFF);
			string_started = 0;
			return;
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
	

/* 
	// toggling the red LED on and off with every received serial commmand
	if (greenLEDtoggle == 0)
	{
		greenLEDtoggle = 1;
	}
	else
	{
		greenLEDtoggle = 0;
	}
	
	greenLED(greenLEDtoggle);
*/

	// zu sendendes Zeichen lesen,
	// Zeiger auf Sendepuffer erhöhen
	data = *uart_tx_p++;
	
	// Ende des nullterminierten Strings erreicht?
	if (data == terminator)
	{
		UDR3 = data;                // Terminator noch senden!
		
		UCSR3B &= ~(1<<UDRIE3);     // dann UDRE Interrupt ausschalten
		uart_tx_p = uart_tx_buffer; // Pointer zurücksetzen
		uart_tx_flag = 1;           // Flag setzen, Übertragung beeendet
		uart_rx_flag = 0;           // Flag löschen, bereit für nächsten Empfang!
	}
	else
	{
		UDR3 = data;                // nein, Daten senden
	}
}

/*
void UsartInit(void)
{
	// no interrupts please!  this is *here* for setting the interrupt control registers.
//	cli();
	
	// Enable UART 3
	//
	// RXCIE: would cause interrupt when a character is received from the UART
	//	UCSR3B |= (1 << RXCIE3) | (1 << RXEN3) | (1 << TXEN3);
	
	UCSR3B |= (1 << RXEN3) | (1 << TXEN3);

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
*/
