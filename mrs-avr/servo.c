#include "servo.h"


void setServoPosition(uint8_t servo, uint16_t value)
{
	// set compare value for the timer channels A and B
	// When using standard PWM, this is a 8-bit value!
	// (max: 0xFF or 255d)
	
	switch (servo)
	{
		case 1:
			OCR4A = value; // channel A = Servo 1
			break;
		case 2:
			OCR4B = value; // channel B = Servo 2
			break;
	}
}


void startPWMServo(void)
{
	// turn OFF "power saving mode" for timer 4 -> turn ON power for timer 4
	PRR1 &= ~(1<<PRTIM4);

	// STOP timer 4
	TCCR4B &= ~(1<<CS41);
	
	// activate "Clear Timer on Compare Match (CTC) Mode" (Mode 4 = WGM42)
	// for timer No. 4 (timer/counter 4 control register A = TCCR4A)
	// for channels A (COM4A1) and B (COM4B1)
	// set output to low level on compare match
//	TCCR4A = (1<<WGM42) | (1<<COM4A1)  | (1<<COM4A0) /*| (1<<COM4B1)*/; // < < < < < < < < < < < < < < < < < <
// okay: servo 1	TCCR4A = (1<<WGM40) | (1<<WGM42) | (1<<COM4A1)  /*| (1<<COM4B1)*/; // < < < < < < < < < < < < < < < < < <
	TCCR4A = (1<<WGM40) | (1<<WGM42) | (1<<COM4A1) | (1<<COM4B1);
	
	// set timer value to 0
	// This is a 16-bit value! However, the compiler handles the 16-bit access! (max: 0xFFF or 65535d)
	TCNT4 = 0;
	
	// START timer 4 and set prescaler to 1024
	// Now one cycle of the counter is:  16 MHz : 1024 = 16 kHz -> 0,000064 s -> 64 Âµs
	TCCR4B |= (1<<CS40) | (1<<CS42);
}


void stopPWMServo()
{
	// STOP timer 4
	TCCR4B &= ~(1<<CS41);
}
