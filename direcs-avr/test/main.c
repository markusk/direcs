#include "main.h"

#ifndef F_CPU
#define F_CPU 		16000000
#endif

int main(void)
{
	// switch port L (all PINS) to output [drive motor 1 to 3]
	DDRA = 0xff;
	DDRC |= (1 << DDC1);
	DDRD |= (1 << DDC5);
	
	
	// test mk
	PORTA |=  (1<<PIN0); // 				JP2, Pin 26
	PORTA |=  (1<<PIN1); // 				JP2, Pin 25
	PORTA |=  (1<<PIN2); // 				JP2, Pin 24
	PORTA |=  (1<<PIN3); // 				JP2, Pin 23
	PORTA |=  (1<<PIN4); // 				JP2, Pin 22
	PORTA |=  (1<<PIN5); // 				JP2, Pin 21
	PORTA |=  (1<<PIN6); // 				JP2, Pin 20
	PORTA |=  (1<<PIN7); // 				JP2, Pin 19
	
	PORTC |= (1<<PIN1); // fl on			JP2, Pin 4
	PORTD &= ~(1<<PIN5); // red LED on		JP1, Pin 45
	
	
	while(1)
	{
	}
	
	// this line es never reached
	return 0;
}
