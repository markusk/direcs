#include "pwm.h"


void setPWMwidth(uint8_t motor, uint8_t value)
{
	// set compare value for the timer channels B and C to "value"
	// When using standard PWM, this is a 8-bit value!
	// (max: 0xFF or 255d)
	
	switch (motor)
	{
		case 1:
			OCR5B = value;
			break;
		case 2:
			OCR5C = value;
			break;
	}
}


void startPWM(void)
{
	// turn OFF "power saving mode" for timer 5!
	// (turn on power for timer)
	PRR1 &= ~(1<<PRTIM5);

	// STOP timer 5
	TCCR5B &= ~(1<<CS51);
	
	// activate non-inverted PWM for counter 5 and channel B and C (mode 1)
	TCCR5A |= (1<<WGM50) | (1<<COM5B1) | (1<<COM5C1);
	
	// set timer value to 0
	// This is a 16-bit value! However, the compiler handles the 16-bit access! (max: 0xFFF or 65535d)
	TCNT5 = 0;
	
	// START timer 5 and set prescaler to 1024
	// Now one cycle of the counter is:  16 MHz / 1024 = 16 kHz -> 0,000064 s -> 64 µs
	TCCR5B |= (1<<CS50) | (1<<CS52);
}


void stopPWM(void)
{
	// STOP timer 5
	TCCR5B &= ~(1<<CS51);
}
