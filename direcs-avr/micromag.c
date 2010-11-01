/*
    Magnetometer.c - Communicate with MicroMag3 Magnetometer
    Written by Frank Kienast in November, 2007
    Found at http://forum.sparkfun.com/viewtopic.php?t=8813

    Modified by Markus Knapp in April, 2009
    Target is ATMega2560 with 16 MHz xtal clock.
*/


#include "micromag.h"


uint16_t readMicromag(unsigned char axis)
{
	int16_t value = 0;
	

	switch (axis)
	{
		case READ_AXIS_X:
			// Slave Selsect to LOW (select device)
			PORT_SPI &= ~(1<<DD_SS);
			// Pulse RESET:
			// reset HIGH
			PORT_SPI |= (1<<DD_RST);
			_delay_ms(1);
			// reset LOW
			PORT_SPI &= ~(1<<DD_RST);
			// send command (on the MOSI line)
			//
			// set:
			// PS2 (64), PS1 (32), PS0 (16) [period select to /2048]
			// AS0 (1)
			//
			// 01100001b = 97d -> 96 + 1 (1 = x axis)
			spi_comm(97);
			// wait for DATA READY to become HIGH (data are ready)
			while(!(PIN_SPI & (1<<DD_RDY))); // TODO: check for timeout (max. delay is 2 ms)!
			// take data
			value = (spi_comm(0) << 8) | spi_comm(0);
			// Slave Selsect to HIGH (unselect device)
			PORT_SPI |= (1<<DD_SS);
			return value;
			break;
		case READ_AXIS_Y:
			// y axis
			PORT_SPI &= ~(1<<DD_SS);
			//Pulse RST
			PORT_SPI |= (1<<DD_RST);
			_delay_ms(1);
			PORT_SPI &= ~(1<<DD_RST);
			// send command (on the MOSI line)
			//
			// set:
			// PS2 (64), PS1 (32), PS0 (16) [period select to /2048]
			// AS1 (2)
			//
			// 01100010b = 98d -> 96 + 2 (2 = y axis)
			spi_comm(98);
			while(!(PIN_SPI & (1<<DD_RDY))); // TODO: check for timeout (max. delay is 2 ms)!
			value = (spi_comm(0) << 8) | spi_comm(0);
			PORT_SPI |= (1<<DD_SS);
			return value;
			break;
		case READ_AXIS_Z:
			//z axis
			PORT_SPI &= ~(1<<DD_SS);
			//Pulse RST
			PORT_SPI |= (1<<DD_RST);
			_delay_ms(1);
			PORT_SPI &= ~(1<<DD_RST);
			// send command (on the MOSI line)
			//
			// set:
			// PS2 (64), PS1 (32), PS0 (16) [period select to /2048]
			// AS1 (2), AS0 (1)
			//
			// 01100011b = 99d -> 96 + 3 (3 = y axis)
			spi_comm(99);
			while(!(PIN_SPI & (1<<DD_RDY))); // TODO: check for timeout (max. delay is 2 ms)!
			value = (spi_comm(0) << 8) | spi_comm(0);
			PORT_SPI |= (1<<DD_SS);
			return value;
			break;
	}
	
	// this line should never be reached!
	value = -1;
	return value;
}


void init_spi(void)
{
	// 'Slave Select' HIGH (don't select the device *now*)
	PORT_SPI |= (1<<DD_SS);

	// 'Reset' is usually LOW
	PORT_SPI &= ~(1<<DD_RST);

	// Set SLAVE SELECT, MOSI, SCK, RESET output, rest input (MOSI is output, means that the Atmel is the master!)
	//
	// !!  PB6 is used for checking if the micromag modul is connected. High = connected.  !!
	// !!  See also main.c
	//
	DDR_SPI = (1 << DD_SS) | (1<<DD_MOSI) | (1<<DD_SCK) | (1<<DD_RST);

	// Enable SPI, Master, set clock rate fck/128 -> 16 MHz/128 = 125 kHz
	// MSB is transmitted first
	// CPOL = 0 (low)
	SPCR = (1<<SPE) | (1<<MSTR) | (1<<SPR1) | (1<<SPR0);
}



uint8_t spi_comm(uint8_t data)
{
	// Start transmission
	SPDR = data;

	// Wait for transmission complete
	while(!(SPSR & (1<<SPIF)));

	// return SPI data register
	return SPDR;
}
