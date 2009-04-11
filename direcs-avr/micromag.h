/*
    Magnetometer.c - Communicate with MicroMag3 Magnetometer
    Written by Frank Kienast in November, 2007

    Target is ATMega16 with 12MHz xtal clock.
    Sparkfun Serial LCD (9600 baud) connected to TX.


    Found at http://forum.sparkfun.com/viewtopic.php?t=8813
*/


//#define F_CPU 12000000
#define BAUD 9600
#define UBRR (((F_CPU / (BAUD * 16UL))) - 1)

// SPI Port of Atmel Microcontroller
#define PORT_SPI PORTB
// SPI Data Register of Atmel Microcontroller (DDR Port B)
#define DDR_SPI DDRB
// SPI PIN of Atmel Microcontroller (Port B)
#define PIN_SPI PINB
// SPI Slave Select		(PB0, Pin 19)
#define DD_SS DDB0
// SPI Serial Clock		(PB1, Pin 20)
#define DD_SCK DDB1
// SPI Master Out Slave in	(PB2, Pin 21)
#define DD_MOSI DDB2
// SPI Data Ready		(PB4, Pin 23)
#define DD_RDY DDB4
// SPI Reset			(PB5, Pin 24)
#define DD_RST DDB5


#define	READ_AXIS_X				61
#define	READ_AXIS_Y				62
#define	READ_AXIS_Z				63



#include <avr/io.h>
#include <string.h>
// org: #include <util/delay.h>
#include <avr/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>


 //Forward declarations
int test();
/*
void init_serial();
void lcd_clear();
void lcd_writec(unsigned char c);
void lcd_writes(char *s);
void lcd_line2();
*/

/**
read axis value from micromag3 sensor
@return the axis value
@param the axis x, y or z (can be READ_AXIS_X, READ_AXIS_Y or READ_AXIS_Z)
*/
uint16_t readMicromag(unsigned char axis);

void init_spi();
uint8_t spi_comm(uint8_t data);
