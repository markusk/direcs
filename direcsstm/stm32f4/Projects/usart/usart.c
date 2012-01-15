#include <stdlib.h>
#include "usart.h"
//#include "main.h"


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

		// los gehts
		sendChar(); 
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

// @todo		redLED(OFF);
	}
}


// UART RX complete interrupt
// hier werden Daten vom PC empfangen und in einem String zwischengespeichert
// Wird ein Stringterminator empfangen, wird ein Flag gesetzt, welches dem 
// Hauptprogramm den kompletten Empfang signalisiert
// original direwcs-avr ISR(USART3_RX_vect)
void receiveChar(int character)
{
	static int counter = 0;    	// Zähler für empfangene Zeichen
	static int string_started = 0;	// Sind wir jetzt im String?
	int data;

 
	// Daten auslesen, dadurch wird das Interruptflag gelöscht              
	data = character;

	// toggling the red LED on and off with every received serial commmand
	if (redLEDtoggle == 0)
	{
		redLEDtoggle = 1;
	}
	else
	{
		redLEDtoggle = 0;
	}
	
// @todo	redLED(redLEDtoggle);


	// Ist Puffer frei für neue Daten?
	if (RXcompleted == 0)
	{
		// Puffer voll?
		if (counter < (UARTBUFFERSIZE-1))
		{     
			// string start
			// string speichern, wenn mit 'starter' begonnen!
			if  (data == starter)
			{
// @todo				greenLED(ON);
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
// @todo				greenLED(OFF);
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
// @todo			greenLED(OFF);
			// reset flag
			string_started = 0;
			return;
		}
	}
}


// UART TX data register empty interrupt
// hier werden neue Daten in das UART-Senderegister geladen
// original direwcs-avr ISR(USART3_UDRE_vect)
void sendChar()
{
	// Zeiger auf Sendepuffer
	static char* uart_tx_p = uart_tx_buffer;
	int data;
	

	do
	{
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
			// nein, Daten senden
			// blocking send on USART2
			while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);
			USART_SendData(USART2, data);
		}
	} while (!TXcompleted);
}


/**
*/
void usartInit(void)
{
	USART_InitTypeDef USART_InitStructure;

	USART_InitStructure.USART_BaudRate = 38400;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	//   GPIO_InitTypeDef GPIO_InitStructure;

	//configure clock for USART
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

	//configure clock for GPIO, Port A
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	//configure AF, Port A
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);

	//configure ports, &GPIO_InitStructure);
	// TX
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	// RX
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	USART_Init(USART2, &USART_InitStructure);

	/* Enable USART2 */
	USART_Cmd(USART2, ENABLE);

	// - - - -

	// own usart / string stuff
	RXcompleted = 0;	// Flag, String komplett empfangen
	TXcompleted = 1;	// Flag, String komplett gesendet
	setStarter(42);    //42 = '*'
	setTerminator(35); //35 = '#'
}


void serialBlockingReadString()
{
	uint16_t buchstabe = 0;


	// blocking read on serial port USART2
	while (USART_GetFlagStatus(USART2, USART_FLAG_RXNE) == RESET)
	{
	}

	buchstabe = USART_ReceiveData(USART2);
	
	// build string
	receiveChar(buchstabe);
}
