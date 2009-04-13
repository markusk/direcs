/*
    Magnetometer.c - Communicate with MicroMag3 Magnetometer
    Written by Frank Kienast in November, 2007

    Target is ATMega16 with 12MHz xtal clock.
    Sparkfun Serial LCD (9600 baud) connected to TX.


    Found at http://forum.sparkfun.com/viewtopic.php?t=8813
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
			// Pulse RESET
			PORT_SPI |= (1<<DD_RST);
			_delay_ms(1);
			PORT_SPI &= ~(1<<DD_RST);
			// send command (on the MOSI line)
			// org: spi_comm(0b01110001) = 113d (112+1)
			// neu:  spi_comm(0b00100001) = 32d (32+1) anderer period select
			spi_comm(32);
			// wait for DATA READY to become HIGH (data are ready)
			while(!(PIN_SPI & (1<<DD_RDY))); // TODO: check for timeout (max. delay is 2 ms)!
			// TODO: only wait for Data Ready?

			// take data
			value = (spi_comm(0) << 8) | spi_comm(0);
			// Slave Selsect to HIGH (unselect device)
			PORT_SPI |= (1<<DD_SS);
			// value = 45;
			return value;
			break;
		case READ_AXIS_Y:
			// y axis
			PORT_SPI &= ~(1<<DD_SS);
			//Pulse RST
			PORT_SPI |= (1<<DD_RST);
			_delay_ms(1);
			PORT_SPI &= ~(1<<DD_RST);
			// org: spi_comm(0b01110010) = 114d (112+2)
			// neu:  spi_comm(0b00100001) = 34d (32+2) anderer period select
			spi_comm(34);
			while(!(PIN_SPI & (1<<DD_RDY)));
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
			// org: spi_comm(0b01110011) = 115d (112+3)
			// neu:  spi_comm(0b00100001) = 35d (32+3) anderer period select
			spi_comm(35);
			while(!(PIN_SPI & (1<<DD_RDY)));
			value = (spi_comm(0) << 8) | spi_comm(0);
			PORT_SPI |= (1<<DD_SS);
			return value;
			break;
	}

	// this line should never be reached!
	value = -1;
	return value;
}


int test(void)
 {
    //char s[20];
    int delay;

    int16_t x = 0, y = 0, z = 0;//, b = 0;

    //init_serial();
    init_spi();



    for(;;)
    {
       // x axis
       PORT_SPI &= ~(1<<DD_SS);
       //Pulse RST
       PORT_SPI |= (1<<DD_RST);
       _delay_ms(1);
       PORT_SPI &= ~(1<<DD_RST);
       // org: spi_comm(0b01110001);
       spi_comm(113);
       while(!(PIN_SPI & (1<<DD_RDY)));
       x = (spi_comm(0) << 8) | spi_comm(0);
       PORT_SPI |= (1<<DD_SS);


       // y axis
       PORT_SPI &= ~(1<<DD_SS);
       //Pulse RST
       PORT_SPI |= (1<<DD_RST);
       _delay_ms(1);
       PORT_SPI &= ~(1<<DD_RST);
       // org: spi_comm(0b01110010);
       spi_comm(114);
       while(!(PIN_SPI & (1<<DD_RDY)));
       y = (spi_comm(0) << 8) | spi_comm(0);
       PORT_SPI |= (1<<DD_SS);


       // z axis
       PORT_SPI &= ~(1<<DD_SS);
       //Pulse RST
       PORT_SPI |= (1<<DD_RST);
       _delay_ms(1);
       PORT_SPI &= ~(1<<DD_RST);
       // org: spi_comm(0b01110011);
       spi_comm(115);
       while(!(PIN_SPI & (1<<DD_RDY)));
       z = (spi_comm(0) << 8) | spi_comm(0);
       PORT_SPI |= (1<<DD_SS);

       //Net
       // Org: b = sqrt((double)x*(double)x + (double)y*(double)y + (double)z*(double)z);


 /*
       lcd_clear();
       sprintf(s,"%5d %5d",x,y);
       lcd_writes(s);

       lcd_line2();
       sprintf(s,"%5d %5d",z,b);
       lcd_writes(s);
 */

       //Delay between measurements
       for(delay = 0; delay < 500; delay++)
          _delay_ms(1);
    }
 }

/*
 void init_serial()
 {

    // Set baud rate
    UBRRH = (UBRR>>8);
    UBRRL = UBRR;

    // Set frame format: 8data, 1stop bit
    UCSRC = (1<<URSEL) |(3<<UCSZ0);

    // Enable transmitter
    UCSRB = (1<<TXEN);

 }

 void lcd_clear()
 {
    lcd_writec(0xfe);
    lcd_writec(0x01);
 }

 void lcd_line2()
 {
    lcd_writec(0xfe);
    lcd_writec(0xc0);
 }

 void lcd_writec(unsigned char c)
 {
    // Wait for empty transmit buffer
    while ( !( UCSRA & (1<<UDRE)));

    // Put data into buffer, sends the data
    UDR = c;

    // Wait for empty transmit buffer
    while ( !( UCSRA & (1<<UDRE)));

 }

 void lcd_writes(char *s)
 {
    int i;

    for(i = 0; i < strlen(s); i++)
       lcd_writec(s[i]);

 }
*/

void init_spi(void)
{
	// 'Slave Select' HIGH (don't select the device *now*)
	PORT_SPI |= (1<<DD_SS);

	// 'Reset' is usually LOW
	PORT_SPI &= ~(1<<DD_RST);

	// Set SS, MOSI, SCK, RST output, rest input (MOSI is output, means that the Atmel is the master!)
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
