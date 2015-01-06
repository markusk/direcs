// Pin 13 has an LED connected on most Arduino boards.
// give it a name:
int led = 13;


//-------  from main.h  -------------------------------

//
// the "serial" commands for the MC
//
#define READ_SENSOR_1			1
#define READ_SENSOR_2			2
#define READ_SENSOR_3			3
#define READ_SENSOR_4			4
#define READ_SENSOR_5			5
#define READ_SENSOR_6			6
#define READ_SENSOR_7			7
#define READ_SENSOR_8			8
#define READ_SENSOR_16			16

#define	MOTOR1_OFF				20
#define	MOTOR1_CLOCKWISE		21
#define	MOTOR1_COUNTERCLOCKWISE	22
#define	MOTOR2_OFF				23
#define	MOTOR2_CLOCKWISE		24
#define	MOTOR2_COUNTERCLOCKWISE	25

#define MOTOR1_SPEED_SET		26
#define MOTOR2_SPEED_SET		27

#define READ_MOTOR_SENSOR1		28
#define READ_MOTOR_SENSOR2		29

#define READ_MOTOR_DISTANCE1	30
#define READ_MOTOR_DISTANCE2	31

#define RESET_MOTOR_DISTANCE1	32
#define RESET_MOTOR_DISTANCE2	33

#define	FLASHLIGHT_OFF			40
#define	FLASHLIGHT_ON			41

#define SET_SERVO1				42
#define SET_SERVO2				43
#define SET_SERVO3				44
#define SET_SERVO4				45
#define SET_SERVO5				46
#define SET_SERVO6				47

#define MOTOR3_OFF 				50
#define MOTOR3_CLOCKWISE 		51
#define MOTOR3_COUNTERCLOCKWISE 52
#define MOTOR4_OFF 				53
#define MOTOR4_CLOCKWISE 		54
#define MOTOR4_COUNTERCLOCKWISE 55

#define MOTOR3_SPEED_SET		56
#define MOTOR4_SPEED_SET		57

#define SPEED_SET_ALLMOTORS		60

#define	READ_AXIS_X				61
#define	READ_AXIS_Y				62
#define	READ_AXIS_Z				63

#define	INIT					64
#define SLEEP					65

#define READ_CONTACT1			70
#define READ_CONTACT2			71
#define READ_CONTACT3			72
#define READ_CONTACT4			73

/// Some direct driving commmands for the robot. @sa Direcs::drive() @sa Direcs::motorControl()
#define BOTFORWARD 				80
#define BOTBACKWARD 			81
#define BOTLEFT					82
#define BOTRIGHT				83
#define BOTTURNLEFT				84
#define BOTTURNRIGHT			85
#define BOTSTART				86
#define BOTSTOP					87
#define BOTWAIT					88
// #define BOTDIAGONAL_FORWARD_LEFT
// #define BOTDIAGONAL_FORWARD_RIGHT
// #define BOTDIAGONAL_BACKWARD_LEFT
// #define BOTDIAGONAL_BACKWARD_RIGHT


// these numbers represent the Ports of the A/D converter C  !!!
// for the infrared sensors and the motor sensors
#define SENSOR1					0  // ADC0
#define SENSOR2					1  // ADC1
#define SENSOR3					2  // ADC2
#define SENSOR4					3  // ADC3
#define SENSOR5					4  // ADC4
#define SENSOR6					5  // ADC5
#define SENSOR7					6  // ADC6
#define SENSOR8					7  // ADC7

#define SENSORMOTOR1			38 // ADC14
#define SENSORMOTOR2			39 // ADC15

// just nice to have
#define ON						1
#define OFF						0

// just nice to have
#define ENABLE					1
#define DISABLE					0

// "Morsedauer" für ein Bit in Millisekunden
#define BITZEIT 100     

//--------------------------------------


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


void setup()
{
  // initialize serial
  Serial.begin(9600);

  // initialize the digital pin as an output.
  pinMode(led, OUTPUT);
}


void loop()
{
/* to be ported
  	// usart stuff
	RXcompleted = 0;	// Flag, String komplett empfangen
	TXcompleted = 1;	// Flag, String komplett gesendet

	setStarter(42);    //42 = '*'
	setTerminator(35); //35 = '#'
*/

	leftWheelCounter = 0;
	rightWheelCounter = 0;


//	leftRevolutionsCounter = 0;
//	rightRevolutionsCounter = 0;


	leftDistanceCounter = 0;
	rightDistanceCounter = 0;


	//-----------------
	// I/O definitions
	//-----------------

/* omnibot to pe ported

	// switch port C bits to input / output
	//
	// Bit0 = yellow LED
	// Bit1 = flashlight
	// Bit2 = ultrasonic trigger
	// Bit3 = ultrasonic echo (input)
	//
	DDRC = (1 << DDC0) | (1 << DDC1) | (1 << DDC2) | (1 << DDC4) | (1 << DDC5) | (1 << DDC6) | (1 << DDC7);
	
	// DDRB is the SPI / I2C Port and set at init_spi in micromag.c
	// PB6 is used for checking if the micromag modul is connected. High = connected. 
	
	// switch some bits on port A to input (camera pan tilt end switches)
	//DDRA &= ~((1 << DDA0) | (1 << DDA1) | (1 << DDA2) | (1 << DDA3));


	// switch port L (all PINS) to output [drive motor 1 to 3]
	DDRL = 0xff;


	// switch some bits on port D to output [read LED, motor 4]
	DDRD |= (1 << DDD5) | (1 << DDD6) | (1 << DDD7);
	
	// switch some bits on port G to output [motor 4 pwm]
	DDRG |= (1<<PIN5);


	// switch port H (all PINS) to output [servos / RGB LEDs]
	DDRH = 0xff;
	
	// switch some bits on port E to output [2 more servos]
	DDRE |= (1 << DDE3) | (1 << DDE4);
*/	
	// red LED on. Now we know, that the program runs.
	redLED(ON);


	// yelow LED off
	yellowLED(OFF);


	// green LED off
	greenLED(OFF);


	// flashlight off
	relais(OFF);


/* omnibot to pe ported

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



	//-------------------------------------------------------------
	// no interrupts please!
	// this is *here* for setting the interrupt control registers
	//-------------------------------------------------------------
	cli();


	// init AD converter with Interrrupt etc.
	// initADC();


	// turn OFF "power saving mode" for AD converter (turn on power for ADC)
	PRR0 &= ~(1<<PRADC);
	
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
	setPWMwidth(1, 60);
	// drive motor 2
	setPWMwidth(2, 60);
	// drive motor 3
	setPWMwidth(3, 60);
	// drive motor 4
	setPWMwidth(4, 60);

	// start the motor PWM timers
 	startPWM();

	// initialize the servo PWM timers
	// This value will be changed by the direcs programm, when value is read from ini-file!
 	setServoPosition(1, 0);
 	setServoPosition(2, 0);
 	setServoPosition(3, 0);
 	setServoPosition(4, 0);
 	setServoPosition(5, 0);
 	setServoPosition(6, 0);

	// start the servo PWM timer
 	startPWMServo(1);
 	startPWMServo(2);
 	startPWMServo(3);
 	startPWMServo(4);
 	startPWMServo(5);
 	startPWMServo(6);
 	
 	unsigned char servoPosition    = 0;
 	unsigned char servoPWM123isOFF = 0;
 	unsigned char servoPWM4isOFF   = 0;
 	unsigned char servoPWM56isOFF  = 0;



	//-----------------------------------------------------
	//-----------------------------------------------------


	// UART 3 konfigurieren
	UBRR3H = (unsigned char) (USART_BAUD_SELECT >> 8);
	UBRR3L = (unsigned char) USART_BAUD_SELECT;
	// enable UART3 and Interrupts for RX and TX
	UCSR3B |= (1<<RXCIE3) | (1<<RXEN3) | (1<<TXEN3); 
 
	//----------------------------------------------------------------------------
	// enable global interrupts
	//----------------------------------------------------------------------------
	sei();
*/ // end to be ported
	

/* to be ported		
		// Wurde ein kompletter String empfangen und ist der Buffer ist leer?
		if (RXcompleted == 1)
		{
			// Everything's fine, so reset the watchdog timer (wdt).
//			wdt_reset();

			// ja, dann String lesen und uart_rx_flag löschen
			get_string(stringbuffer);
*/
  // if there's any serial available, read it:
  while (Serial.available())
  {
                        //--------------------------
			// check what was received
			//--------------------------


			// RESET / INIT
			if (Serial.findUntil("*re", "#"))
			{
/* to be ported		

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
*/
				// red LED off. Know we know, that the program on the PC/Mac has initialised the Atmel
				redLED(OFF);
				
				// setServoPosition(1, 17); // <- exact position now in the mrs.ini!
				// setServoPosition(2, 19); // <- exact position now in the mrs.ini!
				// setServoPosition(3, 23); // <- exact position now in the mrs.ini!
				// setServoPosition(4, 19); // <- exact position now in the mrs.ini!
				// setServoPosition(5, 19); // <- exact position now in the mrs.ini!
				// setServoPosition(6, 22); // <- exact position now in the mrs.ini!
				
				// answer with "ok"
				// this answer is used to see if the robot is "on"
                                Serial.print("*re#");
				
				// e n a b l e  watchdog!
/* to be ported		
				watchdog(ENABLE);
*/
			}
/* to be ported		
			else
			// SLEEP (and turn off watchdog)
			if (strcmp(stringbuffer, "*sl#") == 0)
			{
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

				// red LED off
				redLED(OFF);

				// d i s a b l e  watchdog!
				watchdog(DISABLE);
				
				// answer
				put_string("*sl#");
			}
			else
			// READ_SENSOR_1
			if (strcmp(stringbuffer, "*s1#") == 0)
			{
				// read ADC and send answer over serial port
				sendUInt( readADC(SENSOR1) );
			}
			else
			// READ_SENSOR_2
			if (strcmp(stringbuffer, "*s2#") == 0)
			{
				// read ADC and send answer over serial port
				sendUInt( readADC(SENSOR2) );
			}
			else
			// READ_SENSOR_3
			if (strcmp(stringbuffer, "*s3#") == 0)
			{
				// read ADC and send answer over serial port
				sendUInt( readADC(SENSOR3) );
			}
			else
			// READ_SENSOR_4
			if (strcmp(stringbuffer, "*s4#") == 0)
			{
				// read ADC and send answer over serial port
				sendUInt( readADC(SENSOR4) );
			}
			else
			// READ_SENSOR_5
			if (strcmp(stringbuffer, "*s5#") == 0)
			{
				// read ADC and send answer over serial port
				sendUInt( readADC(SENSOR5) );
			}
			else
			// READ_SENSOR_6
			if (strcmp(stringbuffer, "*s6#") == 0)
			{
				// read ADC and send answer over serial port
				sendUInt( readADC(SENSOR6) );
			}
			else
			// READ_SENSOR_7 (24 V supply)
			if (strcmp(stringbuffer, "*s7#") == 0)
			{
				// read ADC and send answer over serial port
				sendUInt( readADC(SENSOR7) );
			}
			else
			// READ_SENSOR_8 (12 V supply)
			if (strcmp(stringbuffer, "*s8#") == 0)
			{
				// read ADC and send answer over serial port
				sendUInt( readADC(SENSOR8) );
			}
			else
			// READ_SENSOR_16
			// ultra sonic sensor !!
			if (strcmp(stringbuffer, "*s16#") == 0)
			{
				// read ADC and send answer over serial port
				sendUInt( ultraschall_messung() );
			}
			else
			// READ 3D COMPASS CONNECTION
			if (strcmp(stringbuffer, "*cc#") == 0)
			{
				// check if micromag is connected to Atmel-Board (PB6 = high)
				if ( bit_is_set(PINB,PIN6) )
				{
					put_string("*cc#");
				}
				else
				{
					// not connected.
					put_string("*er#");
				}
			}
			else
			// READ_AXIS_X
			if (strcmp(stringbuffer, "*cx#") == 0)
			{
				// check if micromag is connected to Atmel-Board (PB6 = high)
				if ( bit_is_set(PINB,PIN6) )
				{
					// read axis from the micromag3 sensor and send answer over serial port
					sendUInt( readMicromag(READ_AXIS_X) );
				}
				else
				{
					// not connected.
					sendUInt(0);
				}
			}
			else
			// READ_AXIS_Y
			if (strcmp(stringbuffer, "*cy#") == 0)
			{
				// check if micromag is connected to Atmel-Board (PB6 = high)
				if ( bit_is_set(PINB,PIN6) )
				{
					// read axis from the micromag3 sensor and send answer over serial port
					sendUInt( readMicromag(READ_AXIS_Y) );
				}
				else
				{
					// not connected.
					sendUInt(0);
				}
			}
			else
			// READ_AXIS_Z
			if (strcmp(stringbuffer, "*cz#") == 0)
			{
				// check if micromag is connected to Atmel-Board (PB6 = high)
				if ( bit_is_set(PINB,PIN6) )
				{
					// read axis from the micromag3 sensor and send answer over serial port
					sendUInt( readMicromag(READ_AXIS_Z) );
				}
				else
				{
					// not connected.
					sendUInt(0);
				}
			}
			else
			// READ_MOTOR_SENSOR1
			if (strcmp(stringbuffer, "*ms1#") == 0)
			{
				// read ADC and send answer over serial port
				sendUInt( readADC(SENSORMOTOR1) );
			}
			else
			// READ_MOTOR_SENSOR2
			if (strcmp(stringbuffer, "*ms2#") == 0)
			{
				// read ADC and send answer over serial port
				sendUInt( readADC(SENSORMOTOR2) );
			}
			else
			// READ_MOTOR_DISTANCE1
			if (strcmp(stringbuffer, "*dd1#") == 0)
			{
				// send driven distance of motor 1 encoder over serial port
				sendUInt( rightDistanceCounter );
			}
			else
			// READ_MOTOR_DISTANCE2
			if (strcmp(stringbuffer, "*dd2#") == 0)
			{
				// send driven distance of motor 2 encoder over serial port
				sendUInt( leftDistanceCounter );
			}
			else
			// RESET_MOTOR_DISTANCE1
			if (strcmp(stringbuffer, "*id1#") == 0)
			{
				// init driven distance of motor 1 (encoder)
				rightDistanceCounter = 0;
				rightWheelCounter = 0;
				// answer with "ok"
				put_string("*ok#");
			}
			else
			// RESET_MOTOR_DISTANCE2
			if (strcmp(stringbuffer, "*id2#") == 0)
			{
				// init driven distance of motor 2 (encoder)
				leftDistanceCounter = 0;
				leftWheelCounter = 0;
				// answer with "ok"
				put_string("*ok#");
			}
			else
			// FLASHLIGHT ON
			if (strcmp(stringbuffer, "*f0on#") == 0)
			{
				relais(ON);
				yellowLED(ON);
				put_string("*f0on#");
			}
			else
			// FLASHLIGHT OFF
			if (strcmp(stringbuffer, "*f0of#") == 0)
			{
				relais(OFF);
				yellowLED(OFF);
				put_string("*f0of#");
			}
			else
			// MOTOR1_OFF
			if (strcmp(stringbuffer, "*mp1of#") == 0)
			{
				// delete Motor1 A bit
				PORTL &= ~(1<<PIN0);
				// delete Motor1 B bit
				PORTL &= ~(1<<PIN1);
				// answer with "ok"
				put_string("*mp1of#");
			}
			else
			// MOTOR 1 CLOCKWISE = forward
			if (strcmp(stringbuffer, "*md1cw#") == 0)
			{
				// delete Motor1 A bit
				PORTL &= ~(1<<PIN0);
				// set Motor1 B bit
				PORTL |= (1<<PIN1);

				// answer with "ok"
				put_string("*md1cw#");
			}
			else
			// MOTOR 1 COUNTERCLOCKWISE = backward
			if (strcmp(stringbuffer, "*md1cc#") == 0)
			{
				// set Motor1 A bit
				PORTL |= (1<<PIN0);
				// delete Motor1 B bit
				PORTL &= ~(1<<PIN1);
				
				// answer with "ok"
				put_string("*md1cc#");
			}
			else
			// MOTOR1_SPEED_SET
			if (strncmp(stringbuffer, "*mv1", 4) == 0)
			{
				// change first chars for upcoming string conversion
				stringbuffer[0] = '0';
				stringbuffer[1] = '0';
				stringbuffer[2] = '0';
				stringbuffer[3] = '0';
				// get value from string and set speed
				setPWMwidth(1, atoi(stringbuffer));
				// answer with "ok"
				put_string("*mv1#");
			}
			else
			// MOTOR2_OFF
			if (strcmp(stringbuffer, "*mp2of#") == 0)
			{
				// delete Motor2 A bit
				PORTL &= ~(1<<PIN2);
				// delete Motor2 B bit
				PORTL &= ~(1<<PIN3);
				// answer with "ok"
				put_string("*mp2of#");
			}
			else
			// MOTOR 2 CLOCKWISE = forward
			if (strcmp(stringbuffer, "*md2cw#") == 0)
			{
				// delete Motor2 A bit
				PORTL &= ~(1<<PIN2);
				// set Motor2 B bit
				PORTL |= (1<<PIN3);

				// answer with "ok"
				put_string("*md2cw#");
			}
			else
			// MOTOR 2 COUNTERCLOCKWISE = backward
			if (strcmp(stringbuffer, "*md2cc#") == 0)
			{
				// set Motor2 A bit
				PORTL |= (1<<PIN2);
				// delete Motor2 B bit
				PORTL &= ~(1<<PIN3);

				// answer with "ok"
				put_string("*md2cc#");
			}
			else
			// MOTOR2_SPEED_SET
			if (strncmp(stringbuffer, "*mv2", 4) == 0)
			{
				// change first chars for upcoming string conversion
				stringbuffer[0] = '0';
				stringbuffer[1] = '0';
				stringbuffer[2] = '0';
				stringbuffer[3] = '0';
				// get value from string and set speed
				setPWMwidth(2, atoi(stringbuffer));
				// answer with "ok"
				put_string("*mv2#");
			}
			else
			// MOTOR3_OFF
			if (strcmp(stringbuffer, "*mp3of#") == 0)
			{
				// delete Motor3 A bit
				PORTL &= ~(1<<PIN6);
				// delete Motor3 B bit
				PORTL &= ~(1<<PIN7);
				// answer with "ok"
				put_string("*mp3of#");
			}
			else
			// MOTOR 3 CLOCKWISE = forward
			if (strcmp(stringbuffer, "*md3cw#") == 0)
			{
				// delete Motor3 A bit
				PORTL &= ~(1<<PIN6);
				// set Motor3 B bit
				PORTL |= (1<<PIN7);

				// answer with "ok"
				put_string("*md3cw#");
			}
			else
			// MOTOR 3 COUNTERCLOCKWISE = backward
			if (strcmp(stringbuffer, "*md3cc#") == 0)
			{
				// set Motor3 A bit
				PORTL |= (1<<PIN6);
				// delete Motor3 B bit
				PORTL &= ~(1<<PIN7);

				// answer with "ok"
				put_string("*md3cc#");
			}
			else
			// MOTOR3_SPEED_SET
			if (strncmp(stringbuffer, "*mv3", 4) == 0)
			{
				// change first chars for upcoming string conversion
				stringbuffer[0] = '0';
				stringbuffer[1] = '0';
				stringbuffer[2] = '0';
				stringbuffer[3] = '0';
				// get value from string and set speed
				setPWMwidth(3, atoi(stringbuffer));
				// answer with "ok"
				put_string("*mv3#");
			}
			else
			// MOTOR4_OFF
			if (strcmp(stringbuffer, "*mp4of#") == 0)
			{
				// delete Motor4 A bit
				PORTD &= ~(1<<PIN6);
				// delete Motor4 B bit
				PORTD &= ~(1<<PIN7);
				// answer with "ok"
				put_string("*mp4of#");
			}
			else
			// MOTOR 4 CLOCKWISE = forward
			if (strcmp(stringbuffer, "*md4cw#") == 0)
			{
				// delete Motor4 A bit
				PORTD &= ~(1<<PIN6);
				// set Motor4 B bit
				PORTD |= (1<<PIN7);

				// answer with "ok"
				put_string("*md4cw#");
			}
			else
			// MOTOR 4 COUNTERCLOCKWISE = backward
			if (strcmp(stringbuffer, "*md4cc#") == 0)
			{
				// set Motor4 A bit
				PORTD |= (1<<PIN6);
				// delete Motor4 B bit
				PORTD &= ~(1<<PIN7);

				// answer with "ok"
				put_string("*md4cc#");
			}
			else
			// MOTOR4_SPEED_SET
			if (strncmp(stringbuffer, "*mv4", 4) == 0)
			{
				// change first chars for upcoming string conversion
				stringbuffer[0] = '0';
				stringbuffer[1] = '0';
				stringbuffer[2] = '0';
				stringbuffer[3] = '0';
				// get value from string and set speed
				setPWMwidth(4, atoi(stringbuffer));
				// answer with "ok"
				put_string("*mv4#");
			}
			else
			// SPEED_SET_ALLMOTORS
			if (strncmp(stringbuffer, "*mv0", 4) == 0)
			{
				// change first chars for upcoming string conversion
				stringbuffer[0] = '0';
				stringbuffer[1] = '0';
				stringbuffer[2] = '0';
				stringbuffer[3] = '0';
				// get value from string and set speed
				setPWMwidth(1, atoi(stringbuffer));
				setPWMwidth(2, atoi(stringbuffer));
				setPWMwidth(3, atoi(stringbuffer));
				setPWMwidth(4, atoi(stringbuffer));
				// answer with "ok"
				put_string("*mv0#");
			}
			else
			// BOTSTOP
			if (strcmp(stringbuffer, "*bst#") == 0)
			{
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

				// answer with "ok"
				put_string("*bst#");
			}
			else
			// BOTWAIT
			if (strcmp(stringbuffer, "*bwa#") == 0)
			{
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

				// answer with "ok"
				put_string("*bwa#");
			}
			else
			// BOTSTART = "bot go"
			if (strcmp(stringbuffer, "*bgo#") == 0)
			{
				// MOTOR 1 CLOCKWISE = forward
				// delete Motor1 A bit
				PORTL &= ~(1<<PIN0);
				// set Motor1 B bit
				PORTL |= (1<<PIN1);
				
				// MOTOR 2 CLOCKWISE = forward
				// delete Motor2 A bit
				PORTL &= ~(1<<PIN2);
				// set Motor2 B bit
				PORTL |= (1<<PIN3);
				
				// MOTOR 3 CLOCKWISE = forward
				// delete Motor3 A bit
				PORTL &= ~(1<<PIN6);
				// set Motor3 B bit
				PORTL |= (1<<PIN7);
				
				// MOTOR 4 CLOCKWISE = forward
				// delete Motor4 A bit
				PORTD &= ~(1<<PIN6);
				// set Motor4 B bit
				PORTD |= (1<<PIN7);
				
				// answer with "ok"
				put_string("*bgo#");
			}
			else
			// BOTFORWARD = "bot drive forward"
			if (strcmp(stringbuffer, "*bdf#") == 0)
			{
				// MOTOR 1 CLOCKWISE = forward
				// delete Motor1 A bit
				PORTL &= ~(1<<PIN0);
				// set Motor1 B bit
				PORTL |= (1<<PIN1);
				
				// MOTOR 2 CLOCKWISE = forward
				// delete Motor2 A bit
				PORTL &= ~(1<<PIN2);
				// set Motor2 B bit
				PORTL |= (1<<PIN3);
				
				// MOTOR 3 CLOCKWISE = forward
				// delete Motor3 A bit
				PORTL &= ~(1<<PIN6);
				// set Motor3 B bit
				PORTL |= (1<<PIN7);
				
				// MOTOR 4 CLOCKWISE = forward
				// delete Motor4 A bit
				PORTD &= ~(1<<PIN6);
				// set Motor4 B bit
				PORTD |= (1<<PIN7);

				// answer with "ok"
				put_string("*bdf#");
			}
			else
			// BOTBACKWARD = "bot drive backward"
			if (strcmp(stringbuffer, "*bdb#") == 0)
			{
				// MOTOR 1 COUNTERCLOCKWISE = backward
				// set Motor1 A bit
				PORTL |= (1<<PIN0);
				// delete Motor1 B bit
				PORTL &= ~(1<<PIN1);

				// MOTOR 2 COUNTERCLOCKWISE = backward
				// set Motor2 A bit
				PORTL |= (1<<PIN2);
				// delete Motor2 B bit
				PORTL &= ~(1<<PIN3);

				// MOTOR 3 COUNTERCLOCKWISE = backward
				// set Motor3 A bit
				PORTL |= (1<<PIN6);
				// delete Motor3 B bit
				PORTL &= ~(1<<PIN7);

				// MOTOR 4 COUNTERCLOCKWISE = backward
				// set Motor4 A bit
				PORTD |= (1<<PIN6);
				// delete Motor4 B bit
				PORTD &= ~(1<<PIN7);

				// answer with "ok"
				put_string("*bdb#");
			}
			else
			// BOTLEFT = "bot drive left"
			if (strcmp(stringbuffer, "*bdl#") == 0)
			{
				// MOTOR 1 COUNTERCLOCKWISE = backward
				// set Motor1 A bit
				PORTL |= (1<<PIN0);
				// delete Motor1 B bit
				PORTL &= ~(1<<PIN1);

				// MOTOR 2 CLOCKWISE = forward
				// delete Motor2 A bit
				PORTL &= ~(1<<PIN2);
				// set Motor2 B bit
				PORTL |= (1<<PIN3);
				
				// MOTOR 3 CLOCKWISE = forward
				// delete Motor3 A bit
				PORTL &= ~(1<<PIN6);
				// set Motor3 B bit
				PORTL |= (1<<PIN7);

				// MOTOR 4 COUNTERCLOCKWISE = backward
				// set Motor4 A bit
				PORTD |= (1<<PIN6);
				// delete Motor4 B bit
				PORTD &= ~(1<<PIN7);

				// answer with "ok"
				put_string("*bdl#");
			}
			else
			// BOTRIGHT = "bot drive right"
			if (strcmp(stringbuffer, "*bdr#") == 0)
			{
				// MOTOR 1 CLOCKWISE = forward
				// delete Motor1 A bit
				PORTL &= ~(1<<PIN0);
				// set Motor1 B bit
				PORTL |= (1<<PIN1);
				
				// MOTOR 2 COUNTERCLOCKWISE = backward
				// set Motor2 A bit
				PORTL |= (1<<PIN2);
				// delete Motor2 B bit
				PORTL &= ~(1<<PIN3);

				// MOTOR 3 COUNTERCLOCKWISE = backward
				// set Motor3 A bit
				PORTL |= (1<<PIN6);
				// delete Motor3 B bit
				PORTL &= ~(1<<PIN7);

				// MOTOR 4 CLOCKWISE = forward
				// delete Motor4 A bit
				PORTD &= ~(1<<PIN6);
				// set Motor4 B bit
				PORTD |= (1<<PIN7);

				// answer with "ok"
				put_string("*bdr#");
			}
			else
			// TURNLEFT = "bot turn left"
			if (strcmp(stringbuffer, "*btl#") == 0)
			{
				// MOTOR 1 CLOCKWISE = forward
				// delete Motor1 A bit
				PORTL &= ~(1<<PIN0);
				// set Motor1 B bit
				PORTL |= (1<<PIN1);
				
				// MOTOR 2 COUNTERCLOCKWISE = backward
				// set Motor2 A bit
				PORTL |= (1<<PIN2);
				// delete Motor2 B bit
				PORTL &= ~(1<<PIN3);
				
				// MOTOR 3 CLOCKWISE = forward
				// delete Motor3 A bit
				PORTL &= ~(1<<PIN6);
				// set Motor3 B bit
				PORTL |= (1<<PIN7);
				
				// MOTOR 4 COUNTERCLOCKWISE = backward
				// set Motor4 A bit
				PORTD |= (1<<PIN6);
				// delete Motor4 B bit
				PORTD &= ~(1<<PIN7);

				// answer with "ok"
				put_string("*btl#");
			}
			else
			// TURNRIGHT = "bot turn right"
			if (strcmp(stringbuffer, "*btr#") == 0)
			{
				// MOTOR 1 COUNTERCLOCKWISE = backward
				// set Motor1 A bit
				PORTL |= (1<<PIN0);
				// delete Motor1 B bit
				PORTL &= ~(1<<PIN1);

				// MOTOR 2 CLOCKWISE = forward
				// delete Motor2 A bit
				PORTL &= ~(1<<PIN2);
				// set Motor2 B bit
				PORTL |= (1<<PIN3);

				// MOTOR 3 COUNTERCLOCKWISE = backward
				// set Motor3 A bit
				PORTL |= (1<<PIN6);
				// delete Motor3 B bit
				PORTL &= ~(1<<PIN7);

				// MOTOR 4 CLOCKWISE = forward
				// delete Motor4 A bit
				PORTD &= ~(1<<PIN6);
				// set Motor4 B bit
				PORTD |= (1<<PIN7);

				// answer with "ok"
				put_string("*btr#");
			}
			else
			// SET SERVO 1
			if (strncmp(stringbuffer, "*sv1", 4) == 0)
			{
				// change first chars for upcoming string conversion
				stringbuffer[0] = '0';
				stringbuffer[1] = '0';
				stringbuffer[2] = '0';
				stringbuffer[3] = '0';
				// get value from string
				servoPosition = atoi(stringbuffer);
				if (servoPosition == 0)
				{
					// stop PWM for servos 1, 2, 3 (due to same Atmel ports)
					servoPWM123isOFF = 1;
					stopPWMServo(1);
				}
				else
				{
					// turn on PWM if it was turned off before
					if (servoPWM123isOFF == 1)
					{
						servoPWM123isOFF = 0;
						// start PWM for servos 1, 2, 3 (due to same Atmel ports)
						startPWMServo(1);
					}
					// set PWM
					setServoPosition(1, servoPosition);
				}
				// answer with "ok"
				put_string("*sv1#");
			}
			else
			// SET SERVO 2
			if (strncmp(stringbuffer, "*sv2", 4) == 0)
			{
				// change first chars for upcoming string conversion
				stringbuffer[0] = '0';
				stringbuffer[1] = '0';
				stringbuffer[2] = '0';
				stringbuffer[3] = '0';
				// get value from string
				servoPosition = atoi(stringbuffer);
				if (servoPosition == 0)
				{
					// stop PWM for servos 1, 2, 3 (due to same Atmel ports)
					servoPWM123isOFF = 1;
					stopPWMServo(2);
				}
				else
				{
					// turn on PWM if it was turned off before
					if (servoPWM123isOFF == 1)
					{
						servoPWM123isOFF = 0;
						// start PWM for servos 1, 2, 3 (due to same Atmel ports)
						startPWMServo(2);
					}
					// set PWM
					setServoPosition(2, servoPosition);
				}
				// answer with "ok"
				put_string("*sv2#");
			}
			else
			// SET SERVO 3
			if (strncmp(stringbuffer, "*sv3", 4) == 0)
			{
				// change first chars for upcoming string conversion
				stringbuffer[0] = '0';
				stringbuffer[1] = '0';
				stringbuffer[2] = '0';
				stringbuffer[3] = '0';
				// get value from string
				servoPosition = atoi(stringbuffer);
				if (servoPosition == 0)
				{
					// stop PWM for servos 1, 2, 3 (due to same Atmel ports)
					servoPWM123isOFF = 1;
					stopPWMServo(3);
				}
				else
				{
					// turn on PWM if it was turned off before
					if (servoPWM123isOFF == 1)
					{
						servoPWM123isOFF = 0;
						// start PWM for servos 1, 2, 3 (due to same Atmel ports)
						startPWMServo(3);
					}
					// set PWM
					setServoPosition(3, servoPosition);
				}
				// answer with "ok"
				put_string("*sv3#");
			}
			else
			// SET SERVO 4
			if (strncmp(stringbuffer, "*sv4", 4) == 0)
			{
				// change first chars for upcoming string conversion
				stringbuffer[0] = '0';
				stringbuffer[1] = '0';
				stringbuffer[2] = '0';
				stringbuffer[3] = '0';
				// get value from string
				servoPosition = atoi(stringbuffer);
				if (servoPosition == 0)
				{
					// stop PWM for servos 4
					servoPWM4isOFF = 1;
					stopPWMServo(4);
				}
				else
				{
					// turn on PWM if it was turned off before
					if (servoPWM4isOFF == 1)
					{
						servoPWM4isOFF = 0;
						// start PWM for servo 4
						startPWMServo(4);
					}
					// set PWM
					setServoPosition(4, servoPosition);
				}
				// answer with "ok"
				put_string("*sv4#");
			}
			else
			// SET SERVO 5
			if (strncmp(stringbuffer, "*sv5", 4) == 0)
			{
				// change first chars for upcoming string conversion
				stringbuffer[0] = '0';
				stringbuffer[1] = '0';
				stringbuffer[2] = '0';
				stringbuffer[3] = '0';
				// get value from string
				servoPosition = atoi(stringbuffer);
				if (servoPosition == 0)
				{
					// stop PWM for servos 5, 6 (due to same Atmel ports)
					servoPWM56isOFF = 1;
					stopPWMServo(5);
				}
				else
				{
					// turn on PWM if it was turned off before
					if (servoPWM56isOFF == 1)
					{
						servoPWM56isOFF = 0;
						// start PWM for servos 5, 6 (due to same Atmel ports)
						startPWMServo(5);
					}
					// set PWM
					setServoPosition(5, servoPosition);
				}
				// answer with "ok"
				put_string("*sv5#");
			}
			else
			// SET SERVO 6
			if (strncmp(stringbuffer, "*sv6", 4) == 0)
			{
				// change first chars for upcoming string conversion
				stringbuffer[0] = '0';
				stringbuffer[1] = '0';
				stringbuffer[2] = '0';
				stringbuffer[3] = '0';
				// get value from string
				servoPosition = atoi(stringbuffer);
				if (servoPosition == 0)
				{
					// stop PWM for servos 5, 6 (due to same Atmel ports)
					servoPWM56isOFF = 1;
					stopPWMServo(6);
				}
				else
				{
					// turn on PWM if it was turned off before
					if (servoPWM56isOFF == 1)
					{
						servoPWM56isOFF = 0;
						// start PWM for servos 5, 6 (due to same Atmel ports)
						startPWMServo(6);
					}
					// set PWM
					setServoPosition(6, servoPosition);
				}
				// answer with "ok"
				put_string("*sv6#");
			}
*/ // to be ported		
		} // serial data available


} // loop


void sendUInt(uint16_t value)
{
/* // to be ported		

  	uint8_t length = 0;
	
	// start the answer string to send with a '*'
	stringbuffer[0] = starter;


	// convert int to ascii (to Basis 10)
	// (but don't overwrite the first char which is the 'starter' *.)
	itoa(value, stringbuffer+1, 10);


	// get the length of the string
	length = strlen(stringbuffer);


	// add m string terminator '#' at the end of the buffer
	stringbuffer[length] = terminator;


	// String mit \0 terminieren
	stringbuffer[length+1] = 0;


	// send answer
	put_string(stringbuffer);
*/ // to be ported		
}


void redLED(uint8_t state)
{
  	if (state == ON)
	{
		// red LED on
		// yellow LED on
                digitalWrite(led, HIGH);
	}
	else
	{
		// red LED off
		// yellow LED off
                digitalWrite(led, LOW);
	}
}


void yellowLED(uint8_t state)
{
	if (state == ON)
	{
		// yellow LED on
                digitalWrite(led, HIGH);
	}
	else
	{
		// yellow LED off
                digitalWrite(led, LOW);
	}
}


void greenLED(uint8_t state)
{
/* // to be ported		

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
*/
}


void relais(uint8_t state)
{
/* // to be ported		
	if (state == ON)
	{
		// relais on
		PORTC |= (1<<PIN1);
	}
	else
	{
		// relais off
		PORTC &= ~(1<<PIN1);
	}
*/
}


/* not needed anymore. replace calls w/ delay(n);
void long_delay(uint16_t ms)
{
    for (; ms>0; ms--) _delay_ms(1);
}
*/

