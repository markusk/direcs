#include "usart.h"
#include "main.h"


void setStarter(int startr)
{
	// this terminates a string with a '#'
	starter = startr;
}


void setTerminator(int termi)
{
	// this terminates a string with a '#'
	terminator = termi;
}


void put_string(char *daten)
{
	if (TXcompleted == 1)
	{
		// String daten ind en Sendepuffer kopieren
		strcpy(uart_tx_buffer, daten);

		// Flag für 'Senden ist komplett' löschen, 
		TXcompleted = 0;                    

		// UDRE Interrupt einschalten, los gehts
// @todo		UCSR3B |= (1<<UDRIE3); 
   }
}


void get_string(char *daten)
{
	if (RXcompleted == 1)
	{
		// String kopieren
		strcpy(daten, uart_rx_buffer);

		// Flag löschen
		RXcompleted = 0;

		redLED(OFF);
	}
}


// UART RX complete interrupt
// hier werden Daten vom PC empfangen und in einem String zwischengespeichert
// Wird ein Stringterminator empfangen, wird ein Flag gesetzt, welches dem 
// Hauptprogramm den kompletten Empfang signalisiert
ISR(USART3_RX_vect)
{
	static int counter = 0;    	// Zähler für empfangene Zeichen
	static int string_started = 0;	// Sind wir jetzt im String?
	int data;

 
	// Daten auslesen, dadurch wird das Interruptflag gelöscht              
// @todo	data = UDR3;

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
	if (RXcompleted == 0)
	{
		// Puffer voll?
		if (counter < (uart_buffer_size-1))
		{     
			// string start
			// string speichern, wenn mit 'starter' begonnen!
			if  (data == starter)
			{
				greenLED(ON);
				// da string startet, zähler auf 0!
				counter = 0;
				// Daten in Puffer speichern
				uart_rx_buffer[counter] = data;
				// Zähler erhöhen
				counter++;
				// string has started
				string_started = 1;
				return;
			}

			// string stop
			// Ist das Ende des Strings (terminator) erreicht?
			if (data == terminator)
			{
				// ja, dann String terminieren
				uart_rx_buffer[counter] = terminator;
				// String terminieren
				uart_rx_buffer[counter+1] = 0;
				// Flag für 'Empfangspuffer voll' setzen
				RXcompleted = 1;
				// Zähler zurücksetzen
				counter = 0;
				// green LED off
				greenLED(OFF);
				// reset flag
				string_started = 0;
				return;
			}

			// string middle
			// string nur speichern, wenn zuvor der starter mal war.
			if  (string_started == 1)
			{
				// Daten in Puffer speichern
				uart_rx_buffer[counter] = data;
				// Zähler erhöhen
				counter++;
				return;
			}
		}
		else
		{
			// Puffer ist vollgelaufen!
			// Flag auf 'Empfangspuffer wurde geleert' zurücksetzen
			RXcompleted = 0;
			// Zähler zurücksetzen
			counter = 0;
			// green LED off
			greenLED(OFF);
			// reset flag
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
	int data;
	

	// zu sendendes Zeichen lesen,
	// Zeiger auf Sendepuffer erhöhen
	data = *uart_tx_p++;
	
	// Ende des nullterminierten Strings erreicht?
	if (data == 0)
	{
		// Abschließendes \0 nicht mehr senden!
		
// @todo		UCSR3B &= ~(1<<UDRIE3);     // UDRE Interrupt ausschalten
		uart_tx_p = uart_tx_buffer; // Pointer zurücksetzen
		TXcompleted = 1;           // Flag setzen, Übertragung beeendet
		RXcompleted = 0;           // Flag löschen, bereit für nächsten Empfang!
	}
	else
	{
// @todo		UDR3 = data;                // nein, Daten senden
	}
}
