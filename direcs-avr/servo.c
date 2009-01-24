#include "servo.h"


void setServoPosition(uint8_t servo, uint16_t value)
{
	// set compare value for the timer channels A and B
	// When using standard PWM, this is a 8-bit value!
	// (max: 0xFF or 255d)
	
	switch (servo)
	{
		case 1:
			OCR4A = value; // channel A = Servo 1 = timer 4
			break;
		case 2:
			OCR4B = value; // channel B = Servo 2 = timer 4
			break;
		case 3:
			OCR4C = value; // channel C = Servo 3 = timer 4
			break;
		case 4:
			OCR2B = value; // channel B = Servo 4 = timer 2
			break;
		case 5:
			OCR3A = value; // channel A = Servo 5 = timer 3
			break;
		case 6:
			OCR3B = value; // channel B = Servo 6 = timer 3
			break;
	}
}


void startPWMServo(uint8_t servo)
{
	switch (servo)
	{
		case 1:
		case 2:
		case 3:
			//-----------------------------------
			// timer 4 for servos 1,2 3 [16 Bit timer!]
			//-----------------------------------
		
			// STOP timer 4
			TCCR4B &= ~( (1<<CS40) | (1<<CS41) | (1<<CS42) );
			
			// activate "Clear Timer on Compare Match (CTC) Mode" (Mode 5, Fast PWM, 8-bit)
			// for timer No. 4 (timer/counter 4 control register A = TCCR4A)
			// for channels A (COM4A1) and B (COM4B1) and C
			// set output to low level on compare match
			//
			TCCR4A = (1<<WGM40) | (1<<COM4A1) | (1<<COM4B1) | (1<<COM4C1);
			TCCR4B |= (1<<WGM42); // WGMx2 is in TCCRxB !!
			
			// set timer value to 0
			// This is a 16-bit value! However, the compiler handles the 16-bit access! (max: 0xFFF or 65535d)
			TCNT4 = 0;
		
			// turn OFF "power saving mode" for timer 4 -> turn ON power for timer 4
			PRR1 &= ~(1<<PRTIM4);
			
			// START timer 4 and set prescaler to 1024
			// Now one cycle of the counter is:  16 MHz : 1024 = 16 kHz -> 0,000064 s -> 64 µs
			TCCR4B |= (1<<CS40) | (1<<CS42);
			break;

			
			case 4:
				//-----------------------------------
				// timer 2 for servo 4  [8 Bit timer!]
				//-----------------------------------
				// STOP timer 2
				TCCR2B &= ~( (1<<CS20) | (1<<CS21) | (1<<CS22) );
				
				// activate "Clear Timer on Compare Match (CTC) Mode" (Mode 3, Fast PWM, 8-bit) different to other timers!!
				// for timer No. 2 (timer/counter 2 control register B = TCCR2B)
				// for channel B (COM2B1) only!!!
				// set output to low level on compare match
				//
				TCCR2A = (1<<WGM20) | (1<<WGM21) | (1<<COM2B1);
				
				// set timer value to 0
				TCNT2 = 0;

				// turn OFF "power saving mode" for timer 2 -> turn ON power for timer
				PRR0 &= ~(1<<PRTIM2);
				
				// START timer 2 and set prescaler to 1024
				// scale bits of timer 2 are different to other timers!!
				// Now one cycle of the counter is:  16 MHz : 1024 = 16 kHz -> 0,000064 s -> 64 µs
				TCCR2B |= (1<<CS20) | (1<<CS21) | (1<<CS22); // scale bits different to other timers!!
				break;
			
				
			case 5:
			case 6:
				//-----------------------------------
				// timer 3 for servos 5,6 [16 Bit timer!]
				//-----------------------------------
				// STOP timer 3
				TCCR3B &= ~( (1<<CS30) | (1<<CS31) | (1<<CS32) );
				
				// activate "Clear Timer on Compare Match (CTC) Mode" (Mode 5, Fast PWM, 8-bit)
				// for timer No. 3 (timer/counter 3 control register A = TCCR3A)
				// for channels A (COM3A1) and B (COM3B1)
				// set output to low level on compare match
				//
				TCCR3A = (1<<WGM30) | (1<<COM3A1) | (1<<COM3B1);
				TCCR3B |= (1<<WGM32); // WGMx2 is in TCCRxB !!
				
				// set timer value to 0
				// This is a 16-bit value! However, the compiler handles the 16-bit access! (max: 0xFFF or 65535d)
				TCNT3 = 0;

				// turn OFF "power saving mode" for timer 3 -> turn ON power for timer 3
				PRR1 &= ~(1<<PRTIM3);

				// START timer 3  and set prescaler to 1024
				// Now one cycle of the counter is:  16 MHz : 1024 = 16 kHz -> 0,000064 s -> 64 µs
				TCCR3B |= (1<<CS30) | (1<<CS32);
				break;
	}
}


void stopPWMServo(uint8_t servo)
{
	switch (servo)
	{
		case 1:
		case 2:
		case 3:
			// STOP timer 4
			TCCR4B &= ~( (1<<CS40) | (1<<CS41) | (1<<CS42) );
			// activate "power saving mode" for timer 4
			PRR1 |= (1<<PRTIM4);
			break;
		
			
		case 4:
			// STOP timer 2
			TCCR2B &= ~( (1<<CS20) | (1<<CS21) | (1<<CS22) );
			// activate "power saving mode" for timer 2
			PRR0 |= (1<<PRTIM2);
			break;
			
			
		case 5:
		case 6:
			// STOP timer 3
			TCCR3B &= ~( (1<<CS30) | (1<<CS31) | (1<<CS32) );
			// activate "power saving mode" for timer 3
			PRR1 |= (1<<PRTIM3);
			break;
	}
}
