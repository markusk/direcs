#include "pwm.h"


void setPWMwidth(uint8_t motor, uint8_t value)
{
	// set compare value for the timer channels B and C to "value"
	// When using standard PWM, this is a 8-bit value!
	// (max: 0xFF or 255d)
	
	switch (motor)
	{
		case 1: // drive 1
			OCR5B = value;
			break;
		case 2: // drive 2
			OCR5C = value;
			break;
		case 3: // cam pan *and* tilt (same, at the moment)
			OCR0B = value;
			break;
		case 4: // cam pan *and* tilt (same, at the moment)
			OCR0B = value;
			break;
	}
}


void startPWM(void)
{
	//------------------------------------------------
	// drive motors
	//------------------------------------------------
	// turn OFF "power saving mode" for timer 5!
	// (turn on power for timer)
	PRR1 &= ~(1<<PRTIM5);

	// STOP timer 5
	//TCCR5B &= ~(1<<CS51);
	TCCR5B &= ~( (1<<CS50) | (1<<CS51) | (1<<CS52) );
	
	// activate non-inverted phase-correct 8-bit PWM, for counter 5, channel B and C (mode 1)
	// (Clear OCnB and OCnC on compare match (set output level to zero))
	TCCR5A |= (1<<WGM50) | (1<<COM5B1) | (1<<COM5C1);
	
	// set timer value to 0
	// This is a 16-bit value! However, the compiler handles the 16-bit access! (max: 0xFFF or 65535d)
	TCNT5 = 0;
	
	// START timer 5 and set prescaler to 1024
	// Now one cycle of the counter is:  16 MHz / 1024 = 16 kHz -> 0,000064 s -> 64 µs
	TCCR5B |= (1<<CS50) | (1<<CS52);

	
	//------------------------------------------------
	// camera motors (pan *and* tilt)
	//------------------------------------------------
	// turn OFF "power saving mode" for timer 0!
	// (turn on power for timer)
	// -> not necessary for timer 0.

	// STOP timer 0
	TCCR0B &= ~(1<<CS00);
	
	// activate non-inverted phase-correct 8-bit PWM, for counter 0, channel B (mode 1)
	// (Clear OCnB on compare match (set output level to zero))
	TCCR0A |= (1<<WGM00) | (1<<COM0B1);
	
	// set timer value to 0
	// This is a 8
	TCNT0 = 0;
	
	// START timer 0 and set prescaler to 1024
	// Now one cycle of the counter is:  16 MHz / 1024 = 16 kHz -> 0,000064 s -> 64 µs
	TCCR0B |= (1<<CS00) | (1<<CS02);
}


void stopPWM(void)
{
	// STOP timer 5
//	TCCR5B &= ~(1<<CS51);
	TCCR5B &= ~( (1<<CS50) | (1<<CS51) | (1<<CS52) );
	// TODO: turn ON "power saving mode" for timer 5!
	//PRR1 |= (1<<PRTIM5);

	// STOP timer 0
//	TCCR0B &= ~(1<<CS00);
	TCCR0B &= ~( (1<<CS00) | (1<<CS01) | (1<<CS02) );
	// turn ON "power saving mode" for timer 0!
	// -> not available
}
