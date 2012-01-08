#ifndef USART_H
#define USART_H


#include <string.h>
//#include "main.h" // for LED methods


// Puffergrösse in Bytes, RX und TX sind gleich gross
#define UARTBUFFERSIZE	32


// some global variables for ISR routines
// beachte: volatile damit Wert auch außerhalb der ISR gelesen werden kann! Wird sonst vom Compiler wegoptimiert.
volatile uint8_t RXcompleted; // Flag, String komplett empfangen
volatile uint8_t TXcompleted; // Flag, String komplett gesendet

volatile uint8_t starter;    // this marks the beginning of a received string. which is '*' at the moment.
volatile uint8_t terminator; // this marks the end of a string. which is '#' at the moment.

char uart_rx_buffer[UARTBUFFERSIZE + 1]; // Empfangspuffer (+1 wg. zusätzlichem \0 in ISR RX)
char uart_tx_buffer[UARTBUFFERSIZE + 1]; // Sendepuffer    (+1 wg. zusätzlichem \0 in ISR RX)

volatile uint8_t redLEDtoggle;
volatile uint8_t greenLEDtoggle;


// set string starter
void setStarter(uint8_t startr);

// set string terminator
void setTerminator(uint8_t termi);

// einen String senden
// vor Aufruf der Funktion muss man prüfen, ob uart_t_flag==1 ist
// nur dann kann ein neuer String gesendet werden
void put_string(char *daten);

// einen empfangenen String kopieren
// vor Aufruf der Funktion muss man prüfen, ob uart_rx_flag==1 ist
// anderenfalls ist der RX Buffer noch ungültig
void get_string(char *daten);

#endif
