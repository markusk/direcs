/*
    Magnetometer.c - Communicate with MicroMag3 Magnetometer
    Written by Frank Kienast in November, 2007
    Found at http://forum.sparkfun.com/viewtopic.php?t=8813

    Modified by Markus Knapp in April, 2009
    Target is ATMega2560 with 16 MHz xtal clock.
*/


#ifndef F_CPU
#define F_CPU 		16000000
#endif

#define BAUD 9600
#define UBRR (((F_CPU / (BAUD * 16UL))) - 1)


#define PORT_SPI PORTB		// SPI Port of Atmel Microcontroller
#define DDR_SPI DDRB		// SPI Data Register of Atmel Microcontroller (DDR Port B)
#define PIN_SPI PINB		// SPI PIN of Atmel Microcontroller (Port B)
#define DD_SS DDB0		// SPI Slave Select		(PB0, Pin 19)
#define DD_SCK DDB1		// SPI Serial Clock		(PB1, Pin 20)
#define DD_MOSI DDB2		// SPI Master Out Slave in	(PB2, Pin 21)
#define DD_RDY DDB4		// SPI Data Ready		(PB4, Pin 23)
#define DD_RST DDB5		// SPI Reset			(PB5, Pin 24)


#define	READ_AXIS_X				61
#define	READ_AXIS_Y				62
#define	READ_AXIS_Z				63


#include <avr/io.h>
#include <string.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "i2c/i2cmaster.h"


/**
read axis value from micromag3 sensor
@return the axis value
@param the axis x, y or z (can be READ_AXIS_X, READ_AXIS_Y or READ_AXIS_Z)
*/
uint16_t readMicromag(unsigned char axis);
void     init_spi(void);
uint8_t  spi_comm(uint8_t data);
