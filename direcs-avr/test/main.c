#include "main.h"

#ifndef F_CPU
#define F_CPU 		16000000
#endif

int main(void)
{
	// Port A comlplete to output
	DDRA = 0xff;
	
	// switch port C bits to output
	//
	// Bit0 = yellow LED
	// Bit1 = flashlight
// 	DDRC = (1 << DDC0) | (1 << DDC1);
	DDRC = 0xff;

	// switch some bits on port D to output [red LED]
	DDRD = (1 << DDD5);


	while(1)
	{
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
/*		
		_delay_ms(250);
		_delay_ms(250);
		
		PORTA &=  ~(1<<PIN0);
		PORTA &=  ~(1<<PIN1);
		PORTA &=  ~(1<<PIN2);
		PORTA &=  ~(1<<PIN3);
		PORTA &=  ~(1<<PIN4);
		PORTA &=  ~(1<<PIN5);
		PORTA &=  ~(1<<PIN6);
		PORTA &=  ~(1<<PIN7);
		
		PORTC &= ~(1<<PIN1); // fl off
		PORTD |= (1<<PIN5); // red LED off
		
		_delay_ms(250);
		_delay_ms(250);
		*/
	}
	
	// this line es never reached
	return 0;
}
