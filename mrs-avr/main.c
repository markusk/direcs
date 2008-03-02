#include "main.h"


#ifndef F_CPU
#define F_CPU 		16000000
#endif

#include </usr/avr/include/util/delay.h>


uint8_t leftWheelCounter = 0;
uint8_t rightWheelCounter = 0;

//uint16_t leftRevolutionsCounter = 0;
//uint16_t rightRevolutionsCounter = 0;

uint16_t leftDistanceCounter = 0;
uint16_t rightDistanceCounter = 0;


int main(void)
{
	uint8_t redLEDtoggle = 0;
	
	
	// stores the serial received command
	uint16_t value = 0;

	leftWheelCounter = 0;
	rightWheelCounter = 0;

//	leftRevolutionsCounter = 0;
//	rightRevolutionsCounter = 0;

	leftDistanceCounter = 0;
	rightDistanceCounter = 0;

	//-----------------
	// I/O definitions
	//-----------------
	
	// switch port C bits to input / output
	//
	// Bit0 = yellow LED
	// Bit1 = flashlight
	// Bit2 = ultrasonic trigger
	// Bit3 = ultrasonic echo (input)
	//
	DDRC = (1 << DDC0) | (1 << DDC1) | (1 << DDC2) | (1 << DDC4) | (1 << DDC5) | (1 << DDC6) | (1 << DDC7);
	
	// switch some bits on port EA to output (*red* onBoard LED)
	DDRD |= (1 << DDD5);

	// switch some bits on port A to input (camera pan tilt end switches)
	DDRA &= ~((1 << DDA0) | (1 << DDA1) | (1 << DDA2) | (1 << DDA3));
	
	// switch port L (all PINS) to output [drive motor]
	DDRL = 0xff;

	// switch some bits on port D to output [camera motor 2]
	DDRD = (1 << DDD6) | (1 << DDD7);
	// switch some bits on port G to output [camera motor pwm]
	DDRG |= (1<<PIN5);
	
	// switch port H (all PINS) to output [servos]
	DDRH = 0xff;

	// yelow LED off (low active -> turn bit high!)
	PORTC |= (1<<PIN0);
	// flashlight off
	PORTC &= ~(1<<PIN1);
	
	// turn all drive motor bits off (except PWM bits)
	PORTL &= ~(1<<PIN0);
	PORTL &= ~(1<<PIN1);
	PORTL &= ~(1<<PIN2);
	PORTL &= ~(1<<PIN3);
	
	// turn all camera motor 1 [pan] bits off
	PORTL &= ~(1<<PIN6);
	PORTL &= ~(1<<PIN7);
	
	// turn all camera motor 2 [tilt] bits off
	PORTD &= ~(1<<PIN6);
	PORTD &= ~(1<<PIN7);
	
	// turn OFF "power saving mode" for ADC (analof digital converters)!
	// (turn on power for ADC)
	PRR0 &= ~(1<<PRADC);

	// no interrupts please!
	cli();
	
	// switch some bits on port J to input
	//
	// Bit3 = Motor 1 Encoder 1
	// Bit4 = Motor 1 Encoder 2
	// Bit5 = Motor 2 Encoder 1
	// Bit6 = Motor 2 Encoder 2
	//
	DDRJ &= ~((1 << DDJ3) | (1 << DDJ4) | (1 << DDJ5) | (1 << DDJ6));
	
	//----------------------------------------------------------------------------
	// Set the "Pin Change Interrupt Control Register"
	// -> any change on any enabled PCINT15:8 pin will now cause an interrupt!
	//----------------------------------------------------------------------------
	PCICR = (1 << PCIE1);
	// activate the pins which can cause an interrupt
	// At this time use only the FORWARD sensor to generate an interrupt
	PCMSK1 =  (1 << PCINT12) /*| (1 << PCINT13) | (1 << PCINT14)*/ | (1 << PCINT15);
	
	//----------------------------------------------------------------------------
	// enable global interrupts
	//----------------------------------------------------------------------------
	sei();
	
	// initialize the PWM timer (with compare value 100)
	// This value is changed by the mrs programm, when value is read from ini-file!
	// 100 * 64 µs = 6400 µs = 6,4 ms
	//
	// drive motor 1
	setPWMwidth(1, 100);
	// drive motor 2
	setPWMwidth(2, 100);
	// camera motor pan
	setPWMwidth(3, 20);
	// camera motor tilt
	setPWMwidth(4, 20);

	// start the motor PWM timers
	startPWM();
	
	// initialize the PWM timer (with compare value 100)
	// This value is changed by the mrs programm, when value is read from ini-file!
	// 12 * 64 µs = 768 µs ?!?
	setServoPosition(1, 10); // <- correct position now set in the mrs programm!
	setServoPosition(2, 10); // <- correct position now set in the mrs programm!
	setServoPosition(4, 10); // <- correct position now set in the mrs programm!
	setServoPosition(5, 10); // <- correct position now set in the mrs programm!
	setServoPosition(6, 10); // <- correct position now set in the mrs programm!
	
	// start the servo PWM timer
	startPWMServo();

	// initialize USART (serial port)
	UsartInit();


/*
	//---------------------------------------------------------
	// T E S T
	//---------------------------------------------------------
	while(1)
	{

		for (uint8_t n=255; n>0; n--)
		{
			setPWMwidthServo(n);
			_delay_ms(255);
			_delay_ms(255);
			_delay_ms(255);
			_delay_ms(255);
		}
	}
	//---------------------------------------------------------
*/

	while(1)
	{
		//-----------------------------------------
		// waiting "endlessly" for serial data...
		// react on the received command
		//--------------------------------
		value = UsartReceive();
		
		
		switch (value)
		{
			case INIT:
				if (redLEDtoggle == 0)
				{
					redLEDtoggle = 1;
					// yelow LED on (low active!)
					PORTC &= ~(1<<PIN0);
				}
				else
				{
					redLEDtoggle = 0;
					// yelow LED off (low active!)
					PORTC |= (1<<PIN0);
				}
				// turn all drive motor bits off (except PWM bits)
				PORTL &= ~(1<<PIN0);
				PORTL &= ~(1<<PIN1);
				PORTL &= ~(1<<PIN2);
				PORTL &= ~(1<<PIN3);
				// flashlight off
				PORTC &= ~(1<<PIN1);
				// turn all camera motor bits off
				PORTL &= ~(1<<PIN6);
				PORTL &= ~(1<<PIN7);
				PORTD &= ~(1<<PIN6);
				PORTD &= ~(1<<PIN7);
				// init servo pos. <- correct position now set in the mrs programm!
				setServoPosition(1, 10);
				setServoPosition(2, 10);
				setServoPosition(4, 10);
				setServoPosition(5, 10);
				setServoPosition(6, 10);
				
				// "answer" with "@"
				// this answer is used to see if the robot is "on"
				UsartTransmit( (uint8_t)(64) );
				break;
			
			//-------------------------------
			case READ_SENSOR_1:
				// read value from the analog digital converter (ADC)
				value = readADC(SENSOR1);
				// send MS-Byte
				UsartTransmit( (uint8_t)(value >> 8) );
				// send LS-Byte
				UsartTransmit( (uint8_t)(value) );
				break;

			case READ_SENSOR_2:
				// read value from the analog digital converter (ADC)
				value = readADC(SENSOR2);
				// send MS-Byte
				UsartTransmit( (uint8_t)(value >> 8) );
				// send LS-Byte
				UsartTransmit( (uint8_t)(value) );
				break;

			case READ_SENSOR_3:
				// read value from the analog digital converter (ADC)
				value = readADC(SENSOR3);
				// send MS-Byte
				UsartTransmit( (uint8_t)(value >> 8) );
				// send LS-Byte
				UsartTransmit( (uint8_t)(value) );
				break;

			case READ_SENSOR_4:
				// read value from the analog digital converter (ADC)
				value = readADC(SENSOR4);
				// send MS-Byte
				UsartTransmit( (uint8_t)(value >> 8) );
				// send LS-Byte
				UsartTransmit( (uint8_t)(value) );
				break;

			case READ_SENSOR_5:
				// read value from the analog digital converter (ADC)
				value = readADC(SENSOR5);
				// send MS-Byte
				UsartTransmit( (uint8_t)(value >> 8) );
				// send LS-Byte
				UsartTransmit( (uint8_t)(value) );
				break;

			case READ_SENSOR_6:
				// read value from the analog digital converter (ADC)
				value = readADC(SENSOR6);
				// send MS-Byte
				UsartTransmit( (uint8_t)(value >> 8) );
				// send LS-Byte
				UsartTransmit( (uint8_t)(value) );
				break;

			case READ_SENSOR_7:
				// read value from the analog digital converter (ADC)
				value = readADC(SENSOR7);
				// send MS-Byte
				UsartTransmit( (uint8_t)(value >> 8) );
				// send LS-Byte
				UsartTransmit( (uint8_t)(value) );
				break;

			case READ_SENSOR_8:
				// read value from the analog digital converter (ADC)
				value = readADC(SENSOR8);
				// send MS-Byte
				UsartTransmit( (uint8_t)(value >> 8) );
				// send LS-Byte
				UsartTransmit( (uint8_t)(value) );
				break;
				
			case READ_SENSOR_16:
				// ultra sonic sensor !!
				value = ultraschall_messung();
				// send MS-Byte
				UsartTransmit( (uint8_t)(value >> 8) );
				// send LS-Byte
				UsartTransmit( (uint8_t)(value) );
				break;
				
			case READ_MOTOR_SENSOR1:
				// motor sensor
				value = readADC(SENSORMOTOR1);
				// send MS-Byte
				UsartTransmit( (uint8_t)(value >> 8) );
				// send LS-Byte
				UsartTransmit( (uint8_t)(value) );
				break;
				
			case READ_MOTOR_SENSOR2:
				// motor sensor
				value = readADC(SENSORMOTOR2);
				// send MS-Byte
				UsartTransmit( (uint8_t)(value >> 8) );
				// send LS-Byte
				UsartTransmit( (uint8_t)(value) );
				break;
			
			case READ_MOTOR_DISTANCE1:
				// driven distance of motor 1 (encoder)
				value = rightDistanceCounter;
				// send MS-Byte
				UsartTransmit( (uint8_t)(value >> 8) );
				// send LS-Byte
				UsartTransmit( (uint8_t)(value) );
				break;
			
			case READ_MOTOR_DISTANCE2:
				// driven distance of motor 2 (encoder)
				value = leftDistanceCounter;
				// send MS-Byte
				UsartTransmit( (uint8_t)(value >> 8) );
				// send LS-Byte
				UsartTransmit( (uint8_t)(value) );
				break;
			
			case RESET_MOTOR_DISTANCE1:
				// reset driven distance of motor 1 (encoder)
				rightDistanceCounter = 0;
				rightWheelCounter = 0;
				break;
			
			case RESET_MOTOR_DISTANCE2:
				// reset driven distance of motor 1 (encoder)
				leftDistanceCounter = 0;
				leftWheelCounter = 0;
				break;

			//-------------------------------
			case MOTOR1_OFF:
				// delete Motor1 A bit
				PORTL &= ~(1<<PIN0);
				// delete Motor1 B bit
				PORTL &= ~(1<<PIN1);
				break;

			case MOTOR1_CLOCKWISE:
				// delete Motor1 A bit
				PORTL &= ~(1<<PIN0);
				// set Motor1 B bit
				PORTL |= (1<<PIN1);
				break;

			case MOTOR1_COUNTERCLOCKWISE:
				// set Motor1 A bit
				PORTL |= (1<<PIN0);
				// delete Motor1 B bit
				PORTL &= ~(1<<PIN1);
				break;

			case MOTOR1_SPEED_SET:
				// wait for the (second) value to set the pwm!
				value = UsartReceive();
				setPWMwidth(1, value);
				break;

			//-------------------------------
			case MOTOR2_OFF:
				// delete Motor2 A bit
				PORTL &= ~(1<<PIN2);
				// delete Motor2 B bit
				PORTL &= ~(1<<PIN3);
				break;

			case MOTOR2_CLOCKWISE:
				// delete Motor2 A bit
				PORTL &= ~(1<<PIN2);
				// set Motor2 B bit
				PORTL |= (1<<PIN3);
				break;

			case MOTOR2_COUNTERCLOCKWISE:
				// set Motor2 A bit
				PORTL |= (1<<PIN2);
				// delete Motor2 B bit
				PORTL &= ~(1<<PIN3);
				break;

			case MOTOR2_SPEED_SET:
				// wait for the (second) value to set the pwm!
				value = UsartReceive();
				setPWMwidth(2, value);
				break;

			//-------------------------------
			case MOTOR3_OFF:
				// delete Motor3 A bit
				PORTL &= ~(1<<PIN6);
				// delete Motor3 B bit
				PORTL &= ~(1<<PIN7);
				break;

			case MOTOR3_CLOCKWISE:
				// delete Motor3 A bit
				PORTL &= ~(1<<PIN6);
				// set Motor3 B bit
				PORTL |= (1<<PIN7);
				break;

			case MOTOR3_COUNTERCLOCKWISE:
				// set Motor3 A bit
				PORTL |= (1<<PIN6);
				// delete Motor3 B bit
				PORTL &= ~(1<<PIN7);
				break;

			case MOTOR3_SPEED_SET:
				// wait for the (second) value to set the pwm!
				value = UsartReceive();
				setPWMwidth(3, value);
				break;

			//-------------------------------
			case MOTOR4_OFF:
				// delete Motor4 A bit
				PORTD &= ~(1<<PIN6);
				// delete Motor4 B bit
				PORTD &= ~(1<<PIN7);
				break;

			case MOTOR4_CLOCKWISE:
				// delete Motor4 A bit
				PORTD &= ~(1<<PIN6);
				// set Motor4 B bit
				PORTD |= (1<<PIN7);
				break;

			case MOTOR4_COUNTERCLOCKWISE:
				// set Motor4 A bit
				PORTD |= (1<<PIN6);
				// delete Motor4 B bit
				PORTD &= ~(1<<PIN7);
				break;

			case MOTOR4_SPEED_SET:
				// wait for the (second) value to set the pwm!
				value = UsartReceive();
				setPWMwidth(4, value);
				break;

			//-------------------------------
			case SET_SERVO1:
				// wait for the (second) value to set the pwm!
				value = UsartReceive();
				setServoPosition(1, value);
				break;

			case SET_SERVO2:
				// wait for the (second) value to set the pwm!
				value = UsartReceive();
				setServoPosition(2, value);
				break;

			//-------------------------------
			case FLASHLIGHT_ON:
				// set flashlight bit
				PORTC |= (1<<PIN1);
				PORTC &= ~(1<<PIN0); // < yellow led
				break;
			case FLASHLIGHT_OFF:
				// delete flashlight bit
				PORTC &= ~(1<<PIN1);
				PORTC |= (1<<PIN0); // < yellow led
				break;
/*
			//-------------------------------
			case STEPPER1_OFF:
				// disable stepper motor 1
				PORTA &= ~(1<<PIN1);
				break;
				
			case STEPPER2_OFF:
				// disable stepper motor 2
				PORTA &= ~(1<<PIN3);
				break;
				
			case STEPPER1_ON:
				// enable stepper motor 1
				PORTA |= (1<<PIN1);
				break;
				
			case STEPPER2_ON:
				// enable stepper motor 2
				PORTA |= (1<<PIN3);
				break;
				
			case STEPPER1_CLOCKWISE:
				// set stepper motor 1 to cw 
				PORTA &= ~(1<<PIN2);
				break;
				
			case STEPPER2_CLOCKWISE:
				// set stepper motor 2 to cw 
				PORTA &= ~(1<<PIN4);
				break;
				
			case STEPPER1_COUNTERCLOCKWISE:
				// set stepper motor 1 to ccw 
				PORTA |= (1<<PIN2);
				break;
				
			case STEPPER2_COUNTERCLOCKWISE:
				// set stepper motor 2 to ccw 
				PORTA |= (1<<PIN4);
				break;
				
			case STEPPER_CLOCK:
				// set stepper motor clock to high
				PORTA |= (1<<PIN0);
				// and then to low 
				PORTA &= ~(1<<PIN0);
				break;
*/
		}
	} // while (1)
	
	// this line is never reached!
	return 0;
}


SIGNAL(PCINT1_vect)
{
	//
	// The circumference of one wheel is 428 mm (42,8 cm).
	//
	// The disk has 120.which generate 2 interrupts per slit (one low-to-high transistion and one high-to-low transistion).
	// So we have 240 interrupts per circumference
	//
	// At each interrupt the wheel moves:  195 mm / 240 = 1,783 mm.
	// For 10 mm (1 cm) we need:  10 mm / 1,783mm = 5,60747 interrupts  ->  After 6 interruupts the robot moves 10 mm (1 cm).
	
	
	//static uint8_t value = 0;
	
	
	//
	// if left wheel moves forward
	//
	if ( bit_is_set(PINJ,PIN3) )
	{
		leftWheelCounter++;
		
		if (leftWheelCounter == 6)
		{
			leftDistanceCounter++;
			leftWheelCounter = 0;
			
			/*
			//
			// TEST TEST TEST
			//
			if (value == 0)
			{
				value = 1;
				PORTC &= ~(1<<PIN0); // yellow led ON (low)
			}
			else
			{
				value = 0;
				PORTC |= (1<<PIN0); // yelow LED off (low active -> high)
			}
			//
			// TEST TEST TEST
			//
			*/
		}
	}
	
	
	if ( bit_is_set(PINJ,PIN6) )
	{
		rightWheelCounter++;
		
		if (rightWheelCounter == 6)
		{
			rightDistanceCounter++;
			rightWheelCounter = 0;
		}
	}
}
