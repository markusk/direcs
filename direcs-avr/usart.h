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
#include <string.h>


// Puffergrösse in Bytes, RX und TX sind gleich gross
#define uart_buffer_size 32

// some global variables for ISR routines
// beachte: volatile damit Wert auch außerhalb der ISR gelesen werden kann! Wird sonst vom Compiler wegoptimiert.
volatile uint8_t uart_rx_flag; // Flag, String komplett empfangen
volatile uint8_t uart_tx_flag; // Flag, String komplett gesendet
char uart_rx_buffer[uart_buffer_size]; // Empfangspuffer
char uart_tx_buffer[uart_buffer_size]; // Sendepuffer


	 void UsartInit(void);
	 void UsartTransmit(uint16_t c);
	 void UsartTransmitString(unsigned char *string);
unsigned char UsartReceive(void);


// einen String senden
// vor Aufruf der Funktion muss man prüfen, ob uart_t_flag==1 ist
// nur dann kann ein neuer String gesendet werden
void put_string(char *daten);


// einen empfangenen String kopieren
// vor Aufruf der Funktion muss man prüfen, ob uart_rx_flag==1 ist
// anderenfalls ist der RX Buffer noch ungültig
void get_string(char *daten);


#endif
