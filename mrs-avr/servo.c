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
		case 3:
			OCR4C = value; // channel B = Servo 3
			break;
		case 4:
			OCR2B = value; // channel B = Servo 4
			break;
		case 5:
			OCR3A = value; // channel B = Servo 5
			break;
		case 6:
			OCR3B = value; // channel B = Servo 6
			break;
	}
}


void startPWMServo(void)
{
	//-----------------------------------
	// timer 4 for servos 1,2 3
	//-----------------------------------
	// turn OFF "power saving mode" for timer 4 -> turn ON power for timer 4
	PRR1 &= ~(1<<PRTIM4);

	// STOP timer 4
	//TCCR4B &= ~(1<<CS41);
	TCCR4B &= ~( (1<<CS40) | (1<<CS41) | (1<<CS42) );
	
	// activate "Clear Timer on Compare Match (CTC) Mode" (Mode 5, PWM phase correct)
	// for timer No. 4 (timer/counter 4 control register A = TCCR4A)
	// for channels A (COM4A1) and B (COM4B1)
	// set output to low level on compare match
	//
	TCCR4A = (1<<WGM40) | (1<<WGM42) | (1<<COM4A1) | (1<<COM4B1);
	
	// set timer value to 0
	// This is a 16-bit value! However, the compiler handles the 16-bit access! (max: 0xFFF or 65535d)
	TCNT4 = 0;
	
	// START timer 4 and set prescaler to 1024
	// Now one cycle of the counter is:  16 MHz : 1024 = 16 kHz -> 0,000064 s -> 64 Âµs
	TCCR4B |= (1<<CS40) | (1<<CS42);

	
	//-----------------------------------
	// timer 2 for servo 4
	//-----------------------------------
	// turn OFF "power saving mode" for timer 2 -> turn ON power for timer
	PRR0 &= ~(1<<PRTIM2);

	// STOP timer 2
	TCCR2B &= ~( (1<<CS20) | (1<<CS21) | (1<<CS22) );
	
	// activate "Clear Timer on Compare Match (CTC) Mode" (Mode 5, PWM phase correct)
	// for timer No. 2 (timer/counter 2 control register B = TCCR2B)
	// for channel B (COM2B1) only!!!
	// set output to low level on compare match
	//
	TCCR2B = (1<<WGM20) | (1<<WGM22) | (1<<COM2B1);
	
	// set timer value to 0
	TCNT2 = 0;
	
	// START timer 2 and set prescaler to 1024
	// scale bits of timer 2 are different to other timers!!
	// Now one cycle of the counter is:  16 MHz : 1024 = 16 kHz -> 0,000064 s -> 64 Âµs
	TCCR2B |= (1<<CS20) | (1<<CS21) | (1<<CS22); // scale bits different to other timers!!
}


void stopPWMServo()
{
	// STOP timer 4
	//TCCR4B &= ~(1<<CS41);
	TCCR4B &= ~( (1<<CS40) | (1<<CS41) | (1<<CS42) );
	// STOP timer 2
	//TCCR2B &= ~(1<<CS20);
	TCCR2B &= ~( (1<<CS20) | (1<<CS21) | (1<<CS22) );

	// TODO: turn ON "power saving mode" for timer 2
	//PRR0 |= (1<<PRTIM2);
	// TODO: turn ON "power saving mode" for timer 4
	//PRR1 |= (1<<PRTIM4);
}
