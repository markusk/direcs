#include "main.h"


#ifndef F_CPU
#define F_CPU 		16000000
#endif

//#include "/usr/local/avr/include/avr/delay.h"


uint8_t leftWheelCounter = 0;
uint8_t rightWheelCounter = 0;

//uint16_t leftRevolutionsCounter = 0;
//uint16_t rightRevolutionsCounter = 0;

uint16_t leftDistanceCounter = 0;
uint16_t rightDistanceCounter = 0;

//uint8_t camPanLSwitch = 0;
//uint8_t camPanRSwitch = 0;
//uint8_t camTiltLSwitch = 0;
//uint8_t camTiltRSwitch = 0;

int main(void)
{
	//-----------------------------------------------------
	// new usart test
	//-----------------------------------------------------
	uart_rx_flag = 0;	// Flag, String komplett empfangen
	uart_tx_flag = 1;	// Flag, String komplett gesendet

    char stringbuffer[64];  // Allgemeiner Puffer für Strings
    uint8_t buffer_full=0;  // noch ein Flag, aber nur in der Hauptschleife
    char * charpointer;     // Hilfszeiger
	//-----------------------------------------------------
	//-----------------------------------------------------

	
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
	
	// switch some bits on port A to input (camera pan tilt end switches)
	//DDRA &= ~((1 << DDA0) | (1 << DDA1) | (1 << DDA2) | (1 << DDA3));

	// switch port L (all PINS) to output [drive motor 1 to 3]
	DDRL = 0xff;

	// switch some bits on port D to output [read LED, motor 4]
	DDRD |= (1 << DDD5) | (1 << DDD6) | (1 << DDD7);
	
	// switch some bits on port G to output [motor 4 pwm]
	DDRG |= (1<<PIN5);

	// switch port H (all PINS) to output [servos]
	DDRH = 0xff;
	// switch some bits on port E to output [2 more servos]
	DDRE |= (1 << DDE3) | (1 << DDE4);
	
	// red LED on. Now we know, that the program runs.
	redLED(ON);

	// yelow LED off
	yellowLED(OFF);

	// green LED off
	greenLED(OFF);

	// flashlight off
	relais(OFF);

	// turn all drive motor bits off (except PWM bits)
	// motor 1
	PORTL &= ~(1<<PIN0);
	PORTL &= ~(1<<PIN1);
	// motor 2
	PORTL &= ~(1<<PIN2);
	PORTL &= ~(1<<PIN3);
	// motor 3
	PORTL &= ~(1<<PIN6);
	PORTL &= ~(1<<PIN7);
	// motor 4
	PORTD &= ~(1<<PIN6);
	PORTD &= ~(1<<PIN7);

	// turn OFF "power saving mode" for ADC (analof digital converters)!
	// (turn on power for ADC)
	PRR0 &= ~(1<<PRADC);

	//-------------------------------------------------------------
	// no interrupts please!
	// this is *here* for setting the interrupt control registers
	//-------------------------------------------------------------
	cli();

	
	// switch some bits on port J to input
	//
	// Bit3 = Motor 1 Encoder 1
	// Bit4 = Motor 1 Encoder 2
	// Bit5 = Motor 2 Encoder 1
	// Bit6 = Motor 2 Encoder 2
	DDRJ &= ~((1 << DDJ3) | (1 << DDJ4) | (1 << DDJ5) | (1 << DDJ6));

	// switch some bits on port K to input
	//
	// Bit0 = Cam R Tilt Endswitch
	// Bit1 = Cam L Tilt Endswitch
	// Bit2 = Cam R Pan Endswitch
	// Bit3 = Cam L Pan Endswitch
	//DDRK &= ~((1 << DDK0) | (1 << DDK1) | (1 << DDK2) | (1 << DDK3));

	//----------------------------------------------------------------------------
	// Set the "Pin Change Interrupt Control Register"
	// -> any change on any enabled PCINT15:8 pin will now cause an interrupt!
	//----------------------------------------------------------------------------
	PCICR = (1 << PCIE1);
	// activate the pins which can cause an interrupt
	// At this time use only the FORWARD sensor to generate an interrupt
	// PCMSK1 =  (1 << PCINT12) | (1 << PCINT13) | (1 << PCINT14) | (1 << PCINT15);
	PCMSK1 =  (1 << PCINT12) | (1 << PCINT15);
	
	//----------------------------------------------------------------------------
	// Set the "Pin Change Interrupt Control Register"
	// -> any change on any enabled PCINT23:16 pin will now cause an interrupt!
	//----------------------------------------------------------------------------
	PCICR = (1 << PCIE2);
	// activate the pins which can cause an interrupt
	// At this time use only the FORWARD sensor to generate an interrupt
	PCMSK2 =  (1 << PCINT16) | (1 << PCINT17) | (1 << PCINT18) | (1 << PCINT19);

	//----------------------------------------------------------------------------
	// initialzie SPI stuff
	//----------------------------------------------------------------------------

	// turn OFF "power saving mode" for SPI (serial peripheral interface)!
	// (turn on power for SPI)
	PRR0 &= ~(1<<PRSPI);
	init_spi();

	// initialize the PWM timer (with compare value 100)  [this is the motor speed!]
	// This value is changed by the mrs programm, when value is read from ini-file!
	// 100 * 64 µs = 6400 µs = 6,4 ms
	//
	// drive motor 1
	setPWMwidth(1, 100);
	// drive motor 2
	setPWMwidth(2, 100);
	// drive motor 3
	setPWMwidth(3, 100);
	// drive motor 4
	setPWMwidth(4, 100);

	// start the motor PWM timers
 	startPWM();

	// initialize the PWM timer (with compare value 100)
	// This value is changed by the mrs programm, when value is read from ini-file!
	// 12 * 64 µs = 768 µs ?!?
// 	setServoPosition(1, 19); // <- exact position now in the mrs.ini!
// 	setServoPosition(2, 13); // <- exact position now in the mrs.ini!
// 	setServoPosition(3, 23); // <- exact position now in the mrs.ini!
// 	setServoPosition(4, 19); // <- exact position now in the mrs.ini!
// 	setServoPosition(5, 24); // <- exact position now in the mrs.ini!
// 	setServoPosition(6, 22); // <- exact position now in the mrs.ini!

	// start the servo PWM timer
// 	startPWMServo(1);
// 	startPWMServo(2);
// 	startPWMServo(3);
// 	startPWMServo(4);
// 	startPWMServo(5);
// 	startPWMServo(6);


	//-----------------------------------------------------
	//-----------------------------------------------------

	// UART 3 konfigurieren
	UBRR3H = (unsigned char) (USART_BAUD_SELECT >> 8);
	UBRR3L = (unsigned char) USART_BAUD_SELECT;
	UCSR3B = (1<<RXCIE3) | (1<<RXEN3) | (1<<TXEN3); 

	// Stringpuffer initialisieren
	stringbuffer[0] = '\n';
	stringbuffer[1] = '\r';
 
	//----------------------------------------------------------------------------
	// enable global interrupts
	//----------------------------------------------------------------------------
	sei();
	
	// Endlose Hauptschleife
	while(1)
	{
		
		// "Sinnvolle" CPU Tätigkeit 
		PORTD &= ~(1<<PD5);
		long_delay(300);
		PORTD |= (1<<PD5);
		long_delay(300);
		
		// Wurde ein kompletter String empfangen 
		// und der Buffer ist leer?
		if (uart_rx_flag==1 && buffer_full==0) {    
			// ja, dann String lesen, 
			// die ersten zwei Zeichen 
			// aber nicht überschreiben
			get_string(stringbuffer+2);             
			buffer_full=1;
		}
		
		// Ist letzte Stringsendung abgeschlossen 
		// und ein neuer String verfügbar?
		if (uart_tx_flag==1 && buffer_full==1) {    
			// Newline + Carrige return anfügen
			strcat(stringbuffer, "\n\r");           
			put_string(stringbuffer); // zurücksenden
			buffer_full=0; // Buffer ist wieder verfügbar
			// Alle Zeichen per LED morsen
			charpointer = stringbuffer;
			while(*charpointer) morse(*charpointer++);
		}
    }

	//-----------------------------------------------------
	//-----------------------------------------------------

/*
	// initialize USART (serial port)
	UsartInit();


	while(1)
	{
		//-----------------------------------------
		// waiting "endlessly" for serial data...
		// react on the received command
		//-----------------------------------------
		value = UsartReceive();
		
		
		// toggling the red LED on and off with every received serial commmand
		if (redLEDtoggle == 0)
		{
			redLEDtoggle = 1;
		}
		else
		{
			redLEDtoggle = 0;
		}
 
		// switch LED
		redLED(redLEDtoggle);

		
		switch (value)
		{
			case INIT:
				// turn all drive motor bits off (except PWM bits)
				PORTL &= ~(1<<PIN0);
				PORTL &= ~(1<<PIN1);
				PORTL &= ~(1<<PIN2);
				PORTL &= ~(1<<PIN3);
				PORTL &= ~(1<<PIN6);
				PORTL &= ~(1<<PIN7);
				PORTD &= ~(1<<PIN6);
				PORTD &= ~(1<<PIN7);
				// flashlight off
				relais(OFF);
				// red LED off. Know we know, that the program on the PC/Mac has initialised the Atmel
				redLED(OFF);
				
// 				setServoPosition(1, 17); // <- exact position now in the mrs.ini!
// 				setServoPosition(2, 19); // <- exact position now in the mrs.ini!
// 				setServoPosition(3, 23); // <- exact position now in the mrs.ini!
// 				setServoPosition(4, 19); // <- exact position now in the mrs.ini!
// 				setServoPosition(5, 19); // <- exact position now in the mrs.ini!
// 				setServoPosition(6, 22); // <- exact position now in the mrs.ini!
				
				// "answer" with "@" [Ascii Dezimal @ = 64]
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

			case READ_SENSOR_7: // voltage 12 V
				// read value from the analog digital converter (ADC)
				value = readADC(SENSOR7);
				// send MS-Byte
				UsartTransmit( (uint8_t)(value >> 8) );
				// send LS-Byte
				UsartTransmit( (uint8_t)(value) );
				break;

			case READ_SENSOR_8: // voltage 24V
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

			case MOTOR3_CLOCKWISE: // cam pan R
				// only, when end switch is clear
// 				if ( bit_is_clear(PINK,PIN2) )
// 				{
					// delete Motor3 A bit
					PORTL &= ~(1<<PIN6);
					// set Motor3 B bit
					PORTL |= (1<<PIN7);
// 				}
				break;

			case MOTOR3_COUNTERCLOCKWISE: // cam pan L
				// only, when end switch is clear
// 				if ( bit_is_clear(PINK,PIN3) )
// 				{
					// set Motor3 A bit
					PORTL |= (1<<PIN6);
					// delete Motor3 B bit
					PORTL &= ~(1<<PIN7);
// 				}
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

			case MOTOR4_CLOCKWISE: // cam tilt top
				// only, when end switch is clear
// 				if ( bit_is_clear(PINK,PIN1) )
// 				{
					// delete Motor4 A bit
					PORTD &= ~(1<<PIN6);
					// set Motor4 B bit
					PORTD |= (1<<PIN7);
// 				}
				break;

			case MOTOR4_COUNTERCLOCKWISE: // cam tilt bottom
// 				if ( bit_is_clear(PINK,PIN0) )
// 				{
					// set Motor4 A bit
					PORTD |= (1<<PIN6);
					// delete Motor4 B bit
					PORTD &= ~(1<<PIN7);
// 				}
				break;

			case MOTOR4_SPEED_SET:
				// wait for the (second) value to set the pwm!
				value = UsartReceive();
				setPWMwidth(4, value);
				break;
				
			case SPEED_SET_ALLMOTORS:
				// wait for the (second) value to set the pwm!
				value = UsartReceive();
				setPWMwidth(1, value);
				setPWMwidth(2, value);
				setPWMwidth(3, value);
				setPWMwidth(4, value);
				break;
				
			//-------------------------------
			case BOTSTART:
			case BOTFORWARD:
				// MOTOR 1 CLOCKWISE
				// MOTOR 2 COUNTERCLOCKWISE
				// MOTOR 3 CLOCKWISE
				// MOTOR 4 COUNTERCLOCKWISE
				// set Motor1 B bit
				// set Motor2 A bit
				// set Motor3 B bit
				// set Motor4 A bit
				PORTL |= (1<<PIN1) | (1<<PIN2) | (1<<PIN7);
				PORTD |= (1<<PIN6);
				// delete Motor1 A bit
				// delete Motor2 B bit
				// delete Motor3 A bit
				// delete Motor4 B bit
				PORTL &= ~( (1<<PIN0) | (1<<PIN3) | (1<<PIN6) );
				PORTD &= ~(1<<PIN7);
				// turn red LED ON, so we know, that all instructions were executed successfully!
				// redLED(ON);
				break;
				
			case BOTBACKWARD:
				// MOTOR 1 COUNTERCLOCKWISE
				// set Motor1 A bit
				PORTL |= (1<<PIN0);
				// delete Motor1 B bit
				PORTL &= ~(1<<PIN1);
				// MOTOR 2 CLOCKWISE
				// delete Motor2 A bit
				PORTL &= ~(1<<PIN2);
				// set Motor2 B bit
				PORTL |= (1<<PIN3);
				// MOTOR 3 COUNTERCLOCKWISE
				// set Motor3 A bit
				PORTL |= (1<<PIN6);
				// delete Motor3 B bit
				PORTL &= ~(1<<PIN7);
				// MOTOR 4 CLOCKWISE
				// delete Motor4 A bit
				PORTD &= ~(1<<PIN6);
				// set Motor4 B bit
				PORTD |= (1<<PIN7);
				break;
				
			case BOTLEFT:
				// MOTOR 1 COUNTERCLOCKWISE
				// set Motor1 A bit
				PORTL |= (1<<PIN0);
				// delete Motor1 B bit
				PORTL &= ~(1<<PIN1);
				// MOTOR 2 COUNTERCLOCKWISE
				// set Motor2 A bit
				PORTL |= (1<<PIN2);
				// delete Motor2 B bit
				PORTL &= ~(1<<PIN3);
				// MOTOR 3 CLOCKWISE
				// delete Motor3 A bit
				PORTL &= ~(1<<PIN6);
				// set Motor3 B bit
				PORTL |= (1<<PIN7);
				// MOTOR 4 CLOCKWISE
				// delete Motor4 A bit
				PORTD &= ~(1<<PIN6);
				// set Motor4 B bit
				PORTD |= (1<<PIN7);
				break;
				
			case BOTRIGHT:
				// MOTOR 1 CLOCKWISE
				// delete Motor1 A bit
				PORTL &= ~(1<<PIN0);
				// set Motor1 B bit
				PORTL |= (1<<PIN1);
				// MOTOR 2 CLOCKWISE
				// delete Motor2 A bit
				PORTL &= ~(1<<PIN2);
				// set Motor2 B bit
				PORTL |= (1<<PIN3);
				// MOTOR 3 COUNTERCLOCKWISE
				// set Motor3 A bit
				PORTL |= (1<<PIN6);
				// delete Motor3 B bit
				PORTL &= ~(1<<PIN7);
				// MOTOR 4 COUNTERCLOCKWISE
				// set Motor4 A bit
				PORTD |= (1<<PIN6);
				// delete Motor4 B bit
				PORTD &= ~(1<<PIN7);
				break;
				
			case BOTTURNLEFT:
				break;
				
			case BOTTURNRIGHT:
				break;
				
			case BOTSTOP:
			case BOTWAIT:
				// MOTOR 1 OFF
				// delete Motor1 A bit
				PORTL &= ~(1<<PIN0);
				// delete Motor1 B bit
				PORTL &= ~(1<<PIN1);
				// MOTOR 2 OFF
				// delete Motor2 A bit
				PORTL &= ~(1<<PIN2);
				// delete Motor2 B bit
				PORTL &= ~(1<<PIN3);
				// MOTOR 3 OFF
				// delete Motor3 A bit
				PORTL &= ~(1<<PIN6);
				// delete Motor3 B bit
				PORTL &= ~(1<<PIN7);
				// MOTOR 4 OFF
				// delete Motor4 A bit
				PORTD &= ~(1<<PIN6);
				// delete Motor4 B bit
				PORTD &= ~(1<<PIN7);
				// turn red LED OFF, so we know, that all instructions were executed successfully!
				// redLED(OFF);
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

			case SET_SERVO3:
				// wait for the (second) value to set the pwm!
				value = UsartReceive();
				setServoPosition(3, value);
				break;


			case SET_SERVO4:
				// wait for the (second) value to set the pwm!
				value = UsartReceive();
				setServoPosition(4, value);
				break;


			case SET_SERVO5:
				// wait for the (second) value to set the pwm!
				value = UsartReceive();
				setServoPosition(5, value);
				break;


			case SET_SERVO6:
				// wait for the (second) value to set the pwm!
				value = UsartReceive();
				setServoPosition(6, value);
				break;

			//-------------------------------
			case FLASHLIGHT_ON:
				relais(ON);
				yellowLED(ON);
				break;
			case FLASHLIGHT_OFF:
				relais(OFF);
				yellowLED(OFF);
				break;

			//-------------------------------
			case READ_CONTACT1:
				// contact cam tilt R/BOTTOM
				// send 1 Byte (8 bit!)
				UsartTransmit( (uint8_t) bit_is_set(PINK,PIN3) );
				break;

			case READ_CONTACT2:
				// contact cam tilt L/TOP
				// send 1 Byte (8 bit!)
				UsartTransmit( (uint8_t) bit_is_set(PINK,PIN2) );
				break;

			case READ_CONTACT3:
				// contact cam pan R
				// send 1 Byte (8 bit!)
				UsartTransmit( (uint8_t) bit_is_set(PINK,PIN1) );
				break;

			case READ_CONTACT4:
				// contact cam pan L
				// send 1 Byte (8 bit!)
				UsartTransmit( (uint8_t) bit_is_set(PINK,PIN0) );
				break;

			case READ_AXIS_X:
				// read x axis from the micromag3 sensor
				value = readMicromag(READ_AXIS_X);
				// send MS-Byte
				UsartTransmit( (uint8_t)(value >> 8) );
				// send LS-Byte
				UsartTransmit( (uint8_t)(value) );
				break;

			case READ_AXIS_Y:
				// read x axis from the micromag3 sensor
				value = readMicromag(READ_AXIS_Y);
				// send MS-Byte
				UsartTransmit( (uint8_t)(value >> 8) );
				// send LS-Byte
				UsartTransmit( (uint8_t)(value) );
				break;

			case READ_AXIS_Z:
				// read x axis from the micromag3 sensor
				value = readMicromag(READ_AXIS_Z);
				// send MS-Byte
				UsartTransmit( (uint8_t)(value >> 8) );
				// send LS-Byte
				UsartTransmit( (uint8_t)(value) );
				break;
		}
	} // while (1)
*/

	// this line is never reached!
	return 0;
}


void redLED(uint8_t state)
{
	if (state == ON)
	{
		// red LED on
		// (low active!)
		PORTD &= ~(1<<PIN5);
	}
	else
	{
		// red LED off
		// (low active!)
		PORTD |= (1<<PIN5);
	}
}


void yellowLED(uint8_t state)
{
	if (state == ON)
	{
		// yellow LED on
		// (low active!)
		PORTC &= ~(1<<PIN0);
	}
	else
	{
		// yellow LED off
		// (low active!)
		PORTC |= (1<<PIN0);
	}
}


void greenLED(uint8_t state)
{
	if (state == ON)
	{
		// green LED on
		// (low active!)
		PORTC &= ~(1<<PIN4);
	}
	else
	{
		// green LED off
		// (low active!)
		PORTC |= (1<<PIN4);
	}
}


void relais(uint8_t state)
{
/*
 
 TURNED OFF DUE TO timeout ERRORS AT SELECT() STATEMENT AT readAtmelPort at direcs::direcsSerial() !!
 
	if (state == ON)
	{
		// relais on
		// (low active!)
		PORTC &= ~(1<<PIN1);
	}
	else
	{
		// relais off
		// (low active!)
		PORTC |= (1<<PIN1);
	}
 */
}


/*
SIGNAL(PCINT1_vect) // todo: replace this old SIGNAL by ISR with correct _vect name!!
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


	//----------------------------
	// if left wheel moves
	//----------------------------
	if ( bit_is_set(PINJ,PIN3) )
	{
		leftWheelCounter++;

		if (leftWheelCounter == 6)
		{
			leftDistanceCounter++;
			leftWheelCounter = 0;

			//
			// TEST TEST TEST
			//
// 			if (value == 0)
// 			{
// 				value = 1;
// 				PORTC &= ~(1<<PIN0); // yellow led ON (low)
// 			}
// 			else
// 			{
// 				value = 0;
// 				PORTC |= (1<<PIN0); // yelow LED off (low active -> high)
// 			}
			//
			// TEST TEST TEST
			//
		}
	}

	//----------------------------
	// if right wheel moves
	//----------------------------
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
*/
/*
SIGNAL(PCINT2_vect) // todo: replace this old SIGNAL by ISR with correct _vect name!!
{
	//----------------------------
	// if Cam Pan L switch set
	//----------------------------
	if ( bit_is_set(PINK,PIN0) )
	{
		// turn off MOTOR3 pan L bit (A)
//		PORTL &= ~(1<<PIN6);

//		camPanLSwitch = 1;
	}
	else
	{
//		camPanLSwitch = 0;
	}

	//----------------------------
	// if Cam Pan R switch set
	//----------------------------
	if ( bit_is_set(PINK,PIN1) )
	{
		// turn off MOTOR3 pan R bit (B)
//		PORTL &= ~(1<<PIN7);

//		camPanRSwitch = 1;
	}
	else
	{
//		camPanRSwitch = 0;
	}

	//----------------------------
	// if Cam Tilt L/TOP switch set
	//----------------------------
	if ( bit_is_set(PINK,PIN2) )
	{
		// turn off MOTOR4 tilt L bit (A)
//		PORTD &= ~(1<<PIN6);

//		camTiltLSwitch = 1;
	}
	else
	{
//		camTiltLSwitch = 0;
	}

	//----------------------------
	// if Cam Tilt R/BOTTOM switch set
	//----------------------------
	if ( bit_is_set(PINK,PIN3) )
	{
		// turn off MOTOR4 tilt R bit (B)
//		PORTD &= ~(1<<PIN7);

//		camTiltRSwitch = 1;
	}
	else
	{
//		camTiltRSwitch = 0;
	}
}
*/


void long_delay(uint16_t ms)
{
    for (; ms>0; ms--) _delay_ms(1);
}


void morse(uint8_t data)
{
    uint8_t i;
 
    // Startbit, immer 0
    PORTD &= ~(1 << PD5);           // LED aus
    long_delay(BITZEIT);
 
    for(i=0; i<8; i++) {
        if (data & 0x01)            // Prüfe Bit #0
            redLED(ON);             // LED an
        else
            redLED(OFF);            // LED aus
        long_delay(BITZEIT);        
        data >>= 1;                 // nächstes Bit auf Bit #0 schieben
    }
    // Stopbit, immer 1
    redLED(ON);                     // LED an
    long_delay(BITZEIT);
}
