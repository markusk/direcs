/*
    Magnetometer.c - Communicate with MicroMag3 Magnetometer
    Written by Frank Kienast in November, 2007

    Target is ATMega16 with 12MHz xtal clock.
    Sparkfun Serial LCD (9600 baud) connected to TX.


    Found at http://forum.sparkfun.com/viewtopic.php?t=8813
*/


 int main()
 { 
    char s[20]; 
    int delay; 
 
    int16_t x = 0, y = 0, z = 0, b = 0; 
 
    init_serial(); 
    init_spi(); 
 
 
 
    for(;;) 
    { 
       //x axis 
       //lcd_writes("1"); 
       PORT_SPI &= ~(1<<DD_SS); 
       //Pulse RST 
       PORT_SPI |= (1<<DD_RST); 
       _delay_ms(1); 
       PORT_SPI &= ~(1<<DD_RST); 
       spi_comm(0b01110001); 
       while(!(PIN_SPI & (1<<DD_RDY))); 
       x = (spi_comm(0) << 8) | spi_comm(0); 
       PORT_SPI |= (1<<DD_SS); 
 
     
       //y axis 
       //lcd_writes("2"); 
       PORT_SPI &= ~(1<<DD_SS); 
       //Pulse RST 
       PORT_SPI |= (1<<DD_RST); 
       _delay_ms(1); 
       PORT_SPI &= ~(1<<DD_RST); 
       spi_comm(0b01110010); 
       while(!(PIN_SPI & (1<<DD_RDY))); 
       y = (spi_comm(0) << 8) | spi_comm(0); 
       PORT_SPI |= (1<<DD_SS); 
 
     
       //z axis 
       PORT_SPI &= ~(1<<DD_SS); 
       //Pulse RST 
       PORT_SPI |= (1<<DD_RST); 
       _delay_ms(1); 
       PORT_SPI &= ~(1<<DD_RST); 
       spi_comm(0b01110011); 
       while(!(PIN_SPI & (1<<DD_RDY))); 
       z = (spi_comm(0) << 8) | spi_comm(0); 
       PORT_SPI |= (1<<DD_SS); 
 
       //Net 
       b = sqrt((double)x*(double)x + (double)y*(double)y + (double)z*(double)z); 
        
 
 
       lcd_clear(); 
       sprintf(s,"%5d %5d",x,y); 
       lcd_writes(s); 
        
       lcd_line2(); 
       sprintf(s,"%5d %5d",z,b); 
       lcd_writes(s); 
        
 
       //Delay between measurements 
       for(delay = 0; delay < 500; delay++) 
          _delay_ms(1); 
    } 
 } 
 
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
 
    // Put data into buffer, sends the data */ 
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
 
 void init_spi() 
 { 
 
    //SS high 
    PORT_SPI |= (1<<DD_SS); 
 
    //RST low 
    PORT_SPI &= ~(1<<DD_RST); 
 
    // Set SS, MOSI, SCK, RST output, rest input 
    DDR_SPI = (1 << DD_SS) | (1<<DD_MOSI) | (1<<DD_SCK) | (1<<DD_RST); 
 
    // Enable SPI, Master, set clock rate fck/64 
    SPCR = (1<<SPE)|(1<<MSTR)| (1<<SPR1) | (1<<SPR0); 
 
 } 
 
 
 
 uint8_t spi_comm(uint8_t data) 
 { 
 
    // Start transmission 
    SPDR = data; 
 
    // Wait for transmission complete 
    while(!(SPSR & (1<<SPIF))); 
 
    return SPDR; 
 
 }
