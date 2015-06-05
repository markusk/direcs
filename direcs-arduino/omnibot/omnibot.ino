// For Apple Remote
#include <IRremote.h>

// For Adafruit FONA
#include <Adafruit_FONA.h>

// For serial communication to the FONA module
#include <SoftwareSerial.h>

#define ledRed       34
#define ledGreen     35
#define ledYellow    36

#define relaisPin    37

// motor pins
#define motor1aPin   22
#define motor1bPin   23
#define motor1PWMPin  2 // all motors same speed
#define motor2aPin   24
#define motor2bPin   25
#define motor2PWMPin  2 // all motors same speed
#define motor3aPin   26
#define motor3bPin   27
#define motor3PWMPin  2 // all motors same speed
#define motor4aPin   28
#define motor4bPin   29
#define motor4PWMPin  2 // all motors same speed


// RGB LED pins
#define RGBLED1red    3
#define RGBLED1green  4
#define RGBLED1blue   5

#define RGBLED2red    6
#define RGBLED2green  7
#define RGBLED2blue   8

#define RGBLED3red    9
#define RGBLED3green 10
#define RGBLED3blue  11

#define RGBLED4red    3 // same like above from here
#define RGBLED4green  4
#define RGBLED4blue   5

#define RGBLED5red    6
#define RGBLED5green  7
#define RGBLED5blue   8

#define RGBLED6red    9
#define RGBLED6green 10
#define RGBLED6blue  11


// IR pins for Apple Remote
// pin for TSOP1736 IR sensor output
#define IR_Rcv_PIN   38

// Adafruit FONA pins (GSM)  @todo: update pins to user a real HW serial port on Arduino/seeduino mega!! < < < <
#define FONA_TX      16
#define FONA_RX      17
#define FONA_RST     18


// Analog input pin for measuring battery voltage 1
#define analogInPin  A0  
// value read from the battery 1
int sensorValue = 0;


/*------------------ DEBUG 1 ------------------------
#include <Wire.h>
#include <Adafruit_MCP23017.h>
#include <Adafruit_RGBLCDShield.h>

// The shield uses the I2C SCL and SDA pins. On classic Arduinos
// this is Analog 4 and 5 so you can't use those for analogRead() anymore
// However, you can connect other I2C sensors to the I2C bus and share
// the I2C bus.
Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();

// These #defines make it easy to set the backlight color
#define RED 0x1
#define YELLOW 0x3
#define GREEN 0x2
#define TEAL 0x6
#define BLUE 0x4
#define VIOLET 0x5
#define WHITE 0x7
//------------------ DEBUG 1 ------------------------*/


/* ------------------ DEBUG 2 ------------------------ /
// Using Adafruit bi-color 8x8 LED matrix
// Pins used: A4 = SDA  and  A5 = SCK

#include <Wire.h>
#include "Adafruit_LEDBackpack.h"
#include "Adafruit_GFX.h"

Adafruit_BicolorMatrix matrix = Adafruit_BicolorMatrix();

//------------------ DEBUG 2 ------------------------*/


//--------------------------------------
// Apple IR Remote stuff
//--------------------------------------
// Storage for the recorded code
         int  codeType = -1;     // The type of code
unsigned long codeValue;         // The code value if not raw
unsigned int  rawCodes[RAWBUF];  // The durations if raw
         int  codeLen;           // The length of the code
         int toggle = 0;         // The RC5/6 toggle state

// The IR reveiver object
IRrecv irrecv(IR_Rcv_PIN);

decode_results results;

// These values are sent by the Apple Remote "round Donut, Aluminium"
unsigned long oben   = 2011287637;
unsigned long unten  = 2011279445;
unsigned long links  = 2011238485;
unsigned long rechts = 2011291733;
unsigned long ok     = 2011282005;
unsigned long menu   = 2011250773;
unsigned long play   = 2011265621;
unsigned long repeat = 2011242581; // Wird bei ok und play zusätzlich gesendet

/*
// These values are sent by the Apple Remote "flat Donut, Aluminium"
 unsigned long oben   = 2011287644;
 unsigned long unten  = 2011279452;
 unsigned long links  = 2011238492;
 unsigned long rechts = 2011291740;
 unsigned long ok     = 2011282012;
 unsigned long menu   = 2011250780;
 unsigned long play   = 2011265628;
 unsigned long repeat = 2011242588; // Wird bei ok und play zusätzlich gesendet
 */



//------------------------------------------------------------
// Adafruit FONA stuff
//------------------------------------------------------------
//const int buttonPin = A0; // Pushbutton
int8_t smsnum = 0; // the number of available SMS's
int returnValue = 0;
// this is a large buffer for replies (SMS)
char replybuffer[160]; // < < < < max 160 character length ! ! !!

SoftwareSerial fonaSS = SoftwareSerial(FONA_TX, FONA_RX);
Adafruit_FONA fona = Adafruit_FONA(FONA_RST);

// store the state if FONA is okay or not (init okay etc.)
boolean FONAstate = false;
//------------------------------------------------------------



//-------  from main.h  -------------------------------

//
// the "serial" commands for the MC
//
#define READ_SENSOR_1      1
#define READ_SENSOR_2      2
#define READ_SENSOR_3      3
#define READ_SENSOR_4      4
#define READ_SENSOR_5      5
#define READ_SENSOR_6      6
#define READ_SENSOR_7      7
#define READ_SENSOR_8      8
#define READ_SENSOR_16      16

#define  MOTOR1_OFF        20
#define  MOTOR1_CLOCKWISE    21
#define  MOTOR1_COUNTERCLOCKWISE  22
#define  MOTOR2_OFF        23
#define  MOTOR2_CLOCKWISE    24
#define  MOTOR2_COUNTERCLOCKWISE  25

#define MOTOR1_SPEED_SET    26
#define MOTOR2_SPEED_SET    27

#define READ_MOTOR_SENSOR1    28
#define READ_MOTOR_SENSOR2    29

#define READ_MOTOR_DISTANCE1  30
#define READ_MOTOR_DISTANCE2  31

#define RESET_MOTOR_DISTANCE1  32
#define RESET_MOTOR_DISTANCE2  33

#define  FLASHLIGHT_OFF      40
#define  FLASHLIGHT_ON      41

#define SET_SERVO1        42
#define SET_SERVO2        43
#define SET_SERVO3        44
#define SET_SERVO4        45
#define SET_SERVO5        46
#define SET_SERVO6        47

#define MOTOR3_OFF         50
#define MOTOR3_CLOCKWISE     51
#define MOTOR3_COUNTERCLOCKWISE 52
#define MOTOR4_OFF         53
#define MOTOR4_CLOCKWISE     54
#define MOTOR4_COUNTERCLOCKWISE 55

#define MOTOR3_SPEED_SET    56
#define MOTOR4_SPEED_SET    57

#define SPEED_SET_ALLMOTORS    60

#define  READ_AXIS_X        61
#define  READ_AXIS_Y        62
#define  READ_AXIS_Z        63

#define  INIT          64
#define SLEEP          65

#define READ_CONTACT1      70
#define READ_CONTACT2      71
#define READ_CONTACT3      72
#define READ_CONTACT4      73

/// Some direct driving commmands for the robot. @sa Direcs::drive() @sa Direcs::motorControl()
#define BOTFORWARD         80
#define BOTBACKWARD       81
#define BOTLEFT          82
#define BOTRIGHT        83
#define BOTTURNLEFT        84
#define BOTTURNRIGHT      85
#define BOTSTART        86
#define BOTSTOP          87
#define BOTWAIT          88
// #define BOTDIAGONAL_FORWARD_LEFT
// #define BOTDIAGONAL_FORWARD_RIGHT
// #define BOTDIAGONAL_BACKWARD_LEFT
// #define BOTDIAGONAL_BACKWARD_RIGHT

/* Commands for interaction with the GSM FONA module
  gsmi  = init GSM module
  gsmp  = unlock GSM module with PIN
  gsms  = (get) GSM status

  smsc  = count available SMS
  smsl  = read Last SMS
  smsr  = read SMS #
  smss  = send SMS
  smsd  = delete SMS #
*/

// these numbers represent the pins on the Arduino !!
// for the infrared sensors and the motor sensors
#define SENSOR1          0  // ADC0
#define SENSOR2          1  // ADC1
#define SENSOR3          2  // ADC2
#define SENSOR4          3  // ADC3
#define SENSOR5          4  // ADC4
#define SENSOR6          5  // ADC5
#define SENSOR7          A0
#define SENSOR8          A1

#define SENSORMOTOR1      38 // ADC14
#define SENSORMOTOR2      39 // ADC15

// just nice to have
#define ON            1
#define OFF            0

// just nice to have
#define ENABLE          1
#define DISABLE          0

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


//-------------------------------------------------------------------------------------------------
// string command check stuff
//-------------------------------------------------------------------------------------------------
int starter    = 42; // this marks the beginning of a received string. which is '*' at the moment.
int terminator = 35; // this marks the end of a string. which is '#' at the moment.

// Puffergrösse in Bytes für den Serial port
#define stringSize 32

String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete
String command = "";
//-------------------------------------------------------------------------------------------------


const int DEFAULTMOTORSPEED = 60;


void setup()
{
  /*------------------ DEBUG 1 ------------------------
  // set up the LCD's number of columns and rows: 
  lcd.begin(16, 2);
  lcd.setBacklight(RED);
  //------------------ DEBUG 1 ------------------------*/

  /*------------------ DEBUG 2 ------------------------ /
  // Bi-color 8x8 LED display with I2C
  matrix.begin(0x70);  // pass in the address

  matrix.setRotation(0);
  matrix.setTextWrap(false);  // we dont want text to wrap so it scrolls nicely
  matrix.setTextSize(1);
  
  // display letter "S"
  letter("S", LED_GREEN);
  //------------------ DEBUG 2 ------------------------*/


  //-------------------------------------------------------------------------------------------------
  // string command check stuff
  //-------------------------------------------------------------------------------------------------
  // initialize serial communication on the USB port
  Serial.begin(9600);

  // reserve 200 bytes for the inputString
  inputString.reserve(stringSize);
  command.reserve(stringSize);
  stringComplete = false;  // Flag, String komplett empfangen
  //-------------------------------------------------------------------------------------------------


  //-------------------------------------------------------------------------------------------------
  // initialize some digital pins as an output.
  //-------------------------------------------------------------------------------------------------
  pinMode(ledRed, OUTPUT);
  pinMode(ledGreen, OUTPUT);
  pinMode(ledYellow, OUTPUT);

  pinMode(motor1aPin, OUTPUT);
  pinMode(motor1bPin, OUTPUT);
  pinMode(motor1PWMPin, OUTPUT);

  pinMode(motor2aPin, OUTPUT);
  pinMode(motor2bPin, OUTPUT);
  pinMode(motor2PWMPin, OUTPUT);

  pinMode(motor3aPin, OUTPUT);
  pinMode(motor3bPin, OUTPUT);
  pinMode(motor3PWMPin, OUTPUT);

  pinMode(motor4aPin, OUTPUT);
  pinMode(motor4bPin, OUTPUT);
  pinMode(motor4PWMPin, OUTPUT);

  pinMode(RGBLED1red, OUTPUT);
  pinMode(RGBLED1green, OUTPUT);
  pinMode(RGBLED1blue, OUTPUT);

  pinMode(RGBLED2red, OUTPUT);
  pinMode(RGBLED2green, OUTPUT);
  pinMode(RGBLED2blue, OUTPUT);

  pinMode(RGBLED3red, OUTPUT);
  pinMode(RGBLED3green, OUTPUT);
  pinMode(RGBLED3blue, OUTPUT);

  pinMode(RGBLED4red, OUTPUT);
  pinMode(RGBLED4green, OUTPUT);
  pinMode(RGBLED4blue, OUTPUT);

  pinMode(RGBLED5red, OUTPUT);
  pinMode(RGBLED5green, OUTPUT);
  pinMode(RGBLED5blue, OUTPUT);

  pinMode(RGBLED6red, OUTPUT);
  pinMode(RGBLED6green, OUTPUT);
  pinMode(RGBLED6blue, OUTPUT);
  //-------------------------------------------------------------------------------------------------


  //-------------------------------------------------------------------------------------------------
  // initialize some digital pins as an output.
  //-------------------------------------------------------------------------------------------------
  pinMode(analogInPin, INPUT);
  pinMode(IR_Rcv_PIN, INPUT);
  //-------------------------------------------------------------------------------------------------


  leftWheelCounter = 0;
  rightWheelCounter = 0;

  //  leftRevolutionsCounter = 0;
  //  rightRevolutionsCounter = 0;

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
   
  */

  // red LED on. Now we know, that the program runs.
  redLED(ON);

  // yelow LED off
  yellowLED(OFF);

  // green LED off
  greenLED(OFF);

  // flashlight off
  relais(OFF);
  
  //-------------------------------------------------------------------------------------------------
  // turn all drive motor bits off
  //-------------------------------------------------------------------------------------------------
  digitalWrite(motor1aPin, LOW);
  digitalWrite(motor1bPin, LOW);
  digitalWrite(motor1PWMPin, LOW);

  digitalWrite(motor2aPin, LOW);
  digitalWrite(motor2bPin, LOW);
  digitalWrite(motor2PWMPin, LOW);

  digitalWrite(motor3aPin, LOW);
  digitalWrite(motor3bPin, LOW);
  digitalWrite(motor3PWMPin, LOW);

  digitalWrite(motor4aPin, LOW);
  digitalWrite(motor4bPin, LOW);
  digitalWrite(motor4PWMPin, LOW);

/*
  //-------------------------------------------------------------------------------------------------
  // open serial connection to FONA
  //-------------------------------------------------------------------------------------------------
  // make it slow so its easy to read!
  fonaSS.begin(4800); // if you're using software serial
  //Serial1.begin(4800); // if you're using hardware serial

  // See if the FONA is responding (this is the former "gsmi" command)
  if (! fona.begin(fonaSS))           // can also try fona.begin(Serial1) 
  {
    // store state
    FONAstate = false;

    // all LEDs red
    allLEDsRed();
  }
  else
  {
    // store state
    FONAstate = true;
  }
*/

  //-------------------------------------------------------------------------------------------------
  // setup complete - all LEDs  g r e e n
  //-------------------------------------------------------------------------------------------------
  allLEDsGreen();

  /* omnibot to pe ported
   
  //-------------------------------------------------------------
  // no interrupts please!
  // this is *here* for setting the interrupt control registers
  //-------------------------------------------------------------
  cli();


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

*/

  // set motor default speed (PWM, 0-255)
  setMotorSpeed();

/*
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

  //----------------------------------------------------------------------------
  // enable global interrupts
  //----------------------------------------------------------------------------
  sei();
  */  // end to be ported

/*
  // Start the IR receiver
  irrecv.enableIRIn();
*/

  //-------------------------------------------------------------------------------------------------
  // setup complete - all LEDs  g r e e n
  //-------------------------------------------------------------------------------------------------
  allLEDsGreen();
}


void loop()
{
  static uint8_t string_started = 0;  // Sind wir jetzt im String?

  //  letter("w", LED_RED);


/*
  //- - - - - - - - -
  // IR TEST  START
  //- - - - - - - - -


  // wait for IR signal
  if (irrecv.decode(&results))
  {
    // LED on
    digitalWrite(ledRed, HIGH);

    // store result for convenience    
    storeCode(&results);

    // resume receiver
    irrecv.resume();

    //---------------------    
    // "translate" to text
    //---------------------    
    if (codeValue == menu)
    {

    }

  } // IR signal received


  //- - - - - - - - -
  // IR TEST  END
  //- - - - - - - - -
*/


  do
  {
    // do we have something on the USB port?
    if (Serial.available())
    {
      // get the new byte
      char inChar = (char)Serial.read();

      // max String length reached?
      if (inputString.length() >= stringSize)
      {
        greenLED(OFF);
        yellowLED(OFF);
        
        /*------------------ DEBUG 1 ------------------------
        lcd.setCursor(0,0);
        lcd.print("ERROR:");
        lcd.setCursor(1,0);
        lcd.print("OVERFLOW");
        lcd.setBacklight(RED);
        //------------------ DEBUG 1 ------------------------*/

        // clear string
        inputString = "";

        // delete flags
        stringComplete = false;
        string_started = 0;
      }
      else
      {
        // Ist Puffer frei für neue Daten?
        if (stringComplete == false)
        {
          //-----------------
          // string start
          //-----------------
          // string speichern, wenn mit 'starter' begonnen!
          if (inChar == starter)
          {
            greenLED(ON);
            yellowLED(OFF);

            //Serial.println("STARTER");
            /*------------------ DEBUG 1 ------------------------
            lcd.setCursor(0,0);
            lcd.print(inChar);
            lcd.setBacklight(YELLOW);
            //------------------ DEBUG 1 ------------------------*/

            /*------------------ DEBUG 2 ------------------------/
            letter("*", LED_YELLOW);
            //------------------ DEBUG 2 ------------------------*/


            // clear the string
            inputString = "";
            // start inputString:
            inputString += inChar;

            // string has started
            string_started = 1;
          }
          else
          {
            //-----------------
            // string stop
            //-----------------
            // Ist das Ende des Strings (terminator) erreicht?
            if (inChar == terminator)
            {
              yellowLED(ON);
              greenLED(OFF);

              //Serial.println("TERMINATOR");
              /*------------------ DEBUG 1 ------------------------
              lcd.print(inChar);
              lcd.setBacklight(GREEN);
              //------------------ DEBUG 1 ------------------------*/
              
              /*------------------ DEBUG 2 ------------------------/
              letter("#", LED_YELLOW);
              //------------------ DEBUG 2 ------------------------*/

              // ja, dann terminator anhängen
              inputString += inChar;

              // copy input string to command string (used in loop)
              command = inputString;

              // clear input striing
              inputString = "";

              // if the incoming character is a "terminator", set a flag
              // so //the main loop can do something about it:
              stringComplete = true;

              // reset flag
              string_started = 0;

              /*------------------ DEBUG ------------------------
              lcd.setCursor(0,0);
              //------------------ DEBUG ------------------------*/
            }
            else
            {
              //-----------------
              // string middle
              //-----------------
              // string nur speichern, wenn zuvor der starter mal war.
              if  (string_started == 1)
              {
                greenLED(OFF);
                yellowLED(OFF);

                //Serial.println("MITTE");
                /*------------------ DEBUG ------------------------
                lcd.print(inChar);
                lcd.setBacklight(WHITE);
                //------------------ DEBUG ------------------------*/

                /*------------------ DEBUG 2 ------------------------/
                letter((String) inChar, LED_YELLOW);
                //------------------ DEBUG 2 ------------------------*/
                
                // Daten in Puffer speichern
                inputString += inChar;
              } // string complete
              else
              {
                // everything else
                greenLED(OFF);
                yellowLED(OFF);

                /*------------------ DEBUG 2 ------------------------/
                letter((String) inChar, LED_YELLOW);
                //------------------ DEBUG 2 ------------------------*/
              } // any string
            }
          }
        } // string complete = false
      }
    } // Serial.available
  } while (stringComplete == false);

/*
  // print the string when a newline arrives:
  if (stringComplete) 
  {
    Serial.print("stringComplete (loop):"); 

    Serial.print(inputString); 

    Serial.println("<END>"); 

    // clear the string
    inputString = "";

    stringComplete = false;
  }
*/

  // Wurde ein kompletter String empfangen und ist der Buffer ist leer?
  // delete flag
  stringComplete = false;

  //--------------------------
  // check what was received
  //--------------------------

  /*------------------ DEBUG ------------------------
  lcd.setBacklight(BLUE);
  lcd.setCursor(0,1);
  lcd.print("commmand=");
  lcd.print(command);
  lcd.print(".");
  //------------------ DEBUG ------------------------*/

  // RESET / INIT
  if (command == "*re#")
  {
    greenLED(OFF);
    yellowLED(OFF);
    
    greenLED(OFF);
    yellowLED(OFF);
    
    // turn all drive motor bits off (except PWM bits)
    digitalWrite(motor1aPin, LOW);
    digitalWrite(motor1bPin, LOW);
    digitalWrite(motor2aPin, LOW);
    digitalWrite(motor2bPin, LOW);
    digitalWrite(motor3aPin, LOW);
    digitalWrite(motor3bPin, LOW);

    // set motor default speed (PWM, 0-255)
    setMotorSpeed();

    // RGB LEDs off
    allLEDsWhite();
     
    // flashlight off
    relais(OFF);
     
    // red LED off. Know we know, that the program on the PC/Mac has initialised the Atmel
    redLED(OFF);

    // answer with "ok"
    // this answer is used to see if the robot is "on"
    Serial.print("*re#");
    // write all data immediately!
    Serial.flush();
  }
  else
  // SLEEP (and turn off watchdog)
  if (command == "*sl#")
  {
    // turn all drive motor bits off (except PWM bits)
    digitalWrite(motor1aPin, LOW);
    digitalWrite(motor1bPin, LOW);
    digitalWrite(motor2aPin, LOW);
    digitalWrite(motor2bPin, LOW);
    digitalWrite(motor3aPin, LOW);
    digitalWrite(motor3bPin, LOW);
    
    // flashlight off
    relais(OFF);
     
    // red LED off
    redLED(OFF);
     
    /* to be ported to Arduino
    //
    // d i s a b l e  watchdog!
    watchdog(DISABLE);
    */
     
    // answer
    Serial.print("*sl#");
    Serial.flush();
  }
  /* to be ported    
  else
  // READ_SENSOR_1
  if (command == "*s1#")
  {
    // read ADC and send answer over serial port
    sendUInt( readADC(SENSOR1) );
  }
  else
  // READ_SENSOR_2
  if (command == "*s2#")
  {
    // read ADC and send answer over serial port
    sendUInt( readADC(SENSOR2) );
  }
  else
  // READ_SENSOR_3
  if (command == "*s3#")
  {
    // read ADC and send answer over serial port
    sendUInt( readADC(SENSOR3) );
  }
  else
  // READ_SENSOR_4
  if (command == "*s4#")
  {
    // read ADC and send answer over serial port
    sendUInt( readADC(SENSOR4) );
  }
  else
  // READ_SENSOR_5
  if (command == "*s5#")
  {
    // read ADC and send answer over serial port
    sendUInt( readADC(SENSOR5) );
  }
  else
  // READ_SENSOR_6
  if (command == "*s6#")
  {
    // read ADC and send answer over serial port
    sendUInt( readADC(SENSOR6) );
  }
  */
  else
  // READ_SENSOR_7 (24 V supply)
  if (command == "*s7#")
  {
    // read the analog in value
    // print the results to the serial monitor:
    if (Serial.print("*") < 1)
    {
      // ERROR!!
      delay(10000);
      return;
    }
    // write all data immediately!
    Serial.flush();

    if (Serial.print( analogRead(SENSOR7) ) < 1)
    {
      // ERROR!!
      delay(10000);
      return;
    }
    // write all data immediately!
    Serial.flush();
    
    if (Serial.print("#") < 1)
    {
      // ERROR!!
      delay(10000);
      return;
    }
    // write all data immediately!
    Serial.flush();
  }
  else
  // READ_SENSOR_8 (12 V supply)
  if (command == "*s8#")
  {
    // read the analog in value
    // print the results to the serial monitor:
    if (Serial.print("*") < 1)
    {
      // ERROR!!
      delay(10000);
      return;
    }
    // write all data immediately!
    Serial.flush();

    if (Serial.print( analogRead(SENSOR8) ) < 1)
    {
      // ERROR!!
      delay(10000);
      return;
    }
    // write all data immediately!
    Serial.flush();

    if (Serial.print("#") < 1)
    {
      // ERROR!!
      delay(10000);
      return;
    }
    // write all data immediately!
    Serial.flush();
  }
/*     else
  // READ_SENSOR_16
  // ultra sonic sensor !!
  if (command == "*s16#")
  {
    // read ADC and send answer over serial port
    sendUInt( ultraschall_messung() );
  }
  else
  // READ 3D COMPASS CONNECTION
  if (command == "*cc#")
  {
    // check if micromag is connected to Atmel-Board (PB6 = high)
    if ( bit_is_set(PINB,PIN6) )
    {
    Serial.print("*cc#");
    }
    else
    {
      // not connected.
      Serial.print("*er#");
    }
  }
  else
  // READ_AXIS_X
  if (command == "*cx#")
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
  if (command == "*cy#")
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
  if (command == "*cz#")
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
  if (command == "*ms1#")
  {
    // read ADC and send answer over serial port
    sendUInt( readADC(SENSORMOTOR1) );
  }
  else
  // READ_MOTOR_SENSOR2
  if (command == "*ms2#")
  {
    // read ADC and send answer over serial port
    sendUInt( readADC(SENSORMOTOR2) );
  }
  else
  // READ_MOTOR_DISTANCE1
  if (command == "*dd1#")
  {
    // send driven distance of motor 1 encoder over serial port
    sendUInt( rightDistanceCounter );
  }
  else
  // READ_MOTOR_DISTANCE2
  if (command == "*dd2#")
  {
    // send driven distance of motor 2 encoder over serial port
    sendUInt( leftDistanceCounter );
  }
  else
  // RESET_MOTOR_DISTANCE1
  if (command == "*id1#")
  {
    // init driven distance of motor 1 (encoder)
    rightDistanceCounter = 0;
    rightWheelCounter = 0;
    // answer with "ok"
    Serial.print("*ok#");
    Serial.flush();
  }
  else
  // RESET_MOTOR_DISTANCE2
  if (command == "*id2#")
  {
    // init driven distance of motor 2 (encoder)
    leftDistanceCounter = 0;
    leftWheelCounter = 0;
    // answer with "ok"
    Serial.print("*ok#");
    Serial.flush();
  }
  */
  else
  // FLASHLIGHT ON
  if (command == "*f0on#")
  {
    relais(ON);
    yellowLED(ON);
    Serial.print("*f0on#");
    Serial.flush();
  }
  else
  // FLASHLIGHT OFF
  if (command == "*f0of#")
  {
    relais(OFF);
    yellowLED(OFF);
    Serial.print("*f0of#");
    Serial.flush();
  }
  else
  // MOTOR1_OFF
  if (command == "*mp1of#")
  {
    digitalWrite(motor1aPin, LOW);
    digitalWrite(motor1bPin, LOW);
    Serial.print("*mp1of#");
    Serial.flush();
  }
  else
  // MOTOR 1 CLOCKWISE = forward
  if (command == "*md1cw#")
  {
    digitalWrite(motor1aPin, LOW);
    digitalWrite(motor1bPin, HIGH);
    Serial.print("*mp1cw#");
    Serial.flush();
  }
  else
  // MOTOR 1 COUNTERCLOCKWISE = backward
  if (command == "*md1cc#")
  {
    digitalWrite(motor1aPin, HIGH);
    digitalWrite(motor1bPin, LOW);
    Serial.print("*md1cc#");
    Serial.flush();
  }
  else
  // MOTOR1_SPEED_SET
  if (command.startsWith("*mv1"))
  {
    // get the 3 digits which contain the speed
    command = command.substring(5, command.length() - 1);
    // get value from string and set speed
    analogWrite(motor1PWMPin, command.toInt());
    Serial.print("*mv1#");
    Serial.flush();
  }
  else
  // MOTOR2_OFF
  if (command == "*mp2of#")
  {
    digitalWrite(motor2aPin, LOW);
    digitalWrite(motor2bPin, LOW);
    Serial.print("*mp2of#");
    Serial.flush();
  }
  else
  // MOTOR 2 CLOCKWISE = forward
  if (command == "*md2cw#")
  {
    digitalWrite(motor2aPin, LOW);
    digitalWrite(motor2bPin, HIGH);
    Serial.print("*mp2cw#");
    Serial.flush();
  }
  else
  // MOTOR 2 COUNTERCLOCKWISE = backward
  if (command == "*md2cc#")
  {
    digitalWrite(motor2aPin, HIGH);
    digitalWrite(motor2bPin, LOW);
    Serial.print("*md2cc#");
    Serial.flush();
  }
  else
  // MOTOR2_SPEED_SET
  if (command.startsWith("*mv2"))
  {
    // get the 3 digits which contain the speed
    command = command.substring(5, command.length() - 1);
    // get value from string and set speed
    analogWrite(motor2PWMPin, command.toInt());
    Serial.print("*mv2#");
    Serial.flush();
  }
  else
  // MOTOR3_OFF
  if (command == "*mp3of#")
  {
    digitalWrite(motor3aPin, LOW);
    digitalWrite(motor3bPin, LOW);
    Serial.print("*mp3of#");
    Serial.flush();
  }
  else
  // MOTOR 3 CLOCKWISE = forward
  if (command == "*md3cw#")
  {
    digitalWrite(motor3aPin, LOW);
    digitalWrite(motor3bPin, HIGH);
    Serial.print("*mp3cw#");
    Serial.flush();
  }
  else
  // MOTOR 3 COUNTERCLOCKWISE = backward
  if (command == "*md3cc#")
  {
    digitalWrite(motor3aPin, HIGH);
    digitalWrite(motor3bPin, LOW);
    Serial.print("*md3cc#");
    Serial.flush();
  }
  else
  // MOTOR3_SPEED_SET
  if (command.startsWith("*mv3"))
  {
    // get the 3 digits which contain the speed
    command = command.substring(5, command.length() - 1);
    // get value from string and set speed
    analogWrite(motor3PWMPin, command.toInt());
    Serial.print("*mv3#");
    Serial.flush();
  }
  else
  // MOTOR4_OFF
  if (command == "*mp4of#")
  {
    digitalWrite(motor4aPin, LOW);
    digitalWrite(motor4bPin, LOW);
    Serial.print("*mp4of#");
    Serial.flush();
  }
  else
  // MOTOR 4 CLOCKWISE = forward
  if (command == "*md4cw#")
  {
    digitalWrite(motor4aPin, LOW);
    digitalWrite(motor4bPin, HIGH);
    Serial.print("*md4cw#");
    Serial.flush();
  }
  else
  // MOTOR 4 COUNTERCLOCKWISE = backward
  if (command == "*md4cc#")
  {
    digitalWrite(motor4aPin, HIGH);
    digitalWrite(motor4bPin, LOW);
    Serial.print("*md4cc#");
    Serial.flush();
  }
  else
  // MOTOR4_SPEED_SET
  if (command.startsWith("*mv4"))
  {
    // get the 3 digits which contain the speed
    command = command.substring(5, command.length() - 1);
    // get value from string and set speed
    analogWrite(motor4PWMPin, command.toInt());
    Serial.print("*mv4#");
    Serial.flush();
  }
  else
  // SPEED_SET_ALLMOTORS
  if (command.startsWith("*mv0"))
  {
    // get the 3 digits which contain the speed
    command = command.substring(5, command.length() - 1);
    // get value from string and set speed
    analogWrite(motor1PWMPin, command.toInt());
    analogWrite(motor2PWMPin, command.toInt());
    analogWrite(motor3PWMPin, command.toInt());
    Serial.print("*mv0#");
    Serial.flush();
  }
  else
  // RGB LED 1 red
  if (command == "*rgb1")
  {
    // get the 3 digits which contain the brightness
    command = command.substring(5, command.length() - 1);
    // get value from string and set value
    analogWrite(RGBLED1red, command.toInt());
    Serial.print("*rgb1#");
    Serial.flush();
  }
  else
  // RGB LED 1 green
  if (command == "*rgb2")
  {
    // get the 3 digits which contain the brightness
    command = command.substring(5, command.length() - 1);
    // get value from string and set value
    analogWrite(RGBLED1green, command.toInt());
    Serial.print("*rgb2#");
    Serial.flush();
  }
  else
  // RGB LED 1 blue
  if (command == "*rgb3")
  {
    // get the 3 digits which contain the brightness
    command = command.substring(5, command.length() - 1);
    // get value from string and set value
    analogWrite(RGBLED1blue, command.toInt());
    Serial.print("*rgb3#");
    Serial.flush();
  }
  else
  // RGB LED 2 red
  if (command == "*rgb4")
  {
    // get the 3 digits which contain the brightness
    command = command.substring(5, command.length() - 1);
    // get value from string and set value
    analogWrite(RGBLED2red, command.toInt());
    Serial.print("*rgb4#");
    Serial.flush();
  }
  else
  // RGB LED 2 green
  if (command == "*rgb5")
  {
    // get the 3 digits which contain the brightness
    command = command.substring(5, command.length() - 1);
    // get value from string and set value
    analogWrite(RGBLED2green, command.toInt());
    Serial.print("*rgb5#");
    Serial.flush();
  }
  else
  // RGB LED 2 blue
  if (command == "*rgb6")
  {
    // get the 3 digits which contain the brightness
    command = command.substring(5, command.length() - 1);
    // get value from string and set value
    analogWrite(RGBLED2blue, command.toInt());
    Serial.print("*rgb6#");
    Serial.flush();
  }
  else
  // RGB LED 3 red
  if (command == "*rgb7")
  {
    // get the 3 digits which contain the brightness
    command = command.substring(5, command.length() - 1);
    // get value from string and set value
    analogWrite(RGBLED3red, command.toInt());
    Serial.print("*rgb7#");
    Serial.flush();
  }
  else
  // RGB LED 3 green
  if (command == "*rgb8")
  {
    // get the 3 digits which contain the brightness
    command = command.substring(5, command.length() - 1);
    // get value from string and set value
    analogWrite(RGBLED3green, command.toInt());
    Serial.print("*rgb8#");
    Serial.flush();
  }
  else
  // RGB LED 3 blue
  if (command == "*rgb9")
  {
    // get the 3 digits which contain the brightness
    command = command.substring(5, command.length() - 1);
    // get value from string and set value
    analogWrite(RGBLED3blue, command.toInt());
    Serial.print("*rgb9#");
    Serial.flush();
  }
  else
  // BOTSTOP
  if (command == "*bst#")
  {
    digitalWrite(motor1aPin, LOW);
    digitalWrite(motor1bPin, LOW);
    digitalWrite(motor2aPin, LOW);
    digitalWrite(motor2bPin, LOW);
    digitalWrite(motor3aPin, LOW);
    digitalWrite(motor3bPin, LOW);

    // answer with "ok"
    Serial.print("*bst#");
    Serial.flush();
  }
  else
  // BOTWAIT
  if (command == "*bwa#")
  {
    digitalWrite(motor1aPin, LOW);
    digitalWrite(motor1bPin, LOW);
    digitalWrite(motor2aPin, LOW);
    digitalWrite(motor2bPin, LOW);
    digitalWrite(motor3aPin, LOW);
    digitalWrite(motor3bPin, LOW);

    // answer with "ok"
    Serial.print("*bwa#");
    Serial.flush();
  }
  else
  // BOTSTART = "bot go"
  if (command == "*bgo#")
  {
    // motor 1 off
    digitalWrite(motor1aPin, LOW);
    digitalWrite(motor1bPin, LOW);
    // motor 2 = left = backward = counterclockwise
    digitalWrite(motor2aPin, HIGH);
    digitalWrite(motor2bPin, LOW);
    // motor 3 = right = forward = clockwise
    digitalWrite(motor3aPin, LOW);
    digitalWrite(motor3bPin, HIGH);

    Serial.print("*bgo#");
    Serial.flush();
  }
  else
  // BOTFORWARD = "bot drive forward"
  if (command == "*bdf#")
  {
    // motor 1 off
    digitalWrite(motor1aPin, LOW);
    digitalWrite(motor1bPin, LOW);
    // motor 2 = left = backward = counterclockwise
    digitalWrite(motor2aPin, HIGH);
    digitalWrite(motor2bPin, LOW);
    // motor 3 = right = forward = clockwise
    digitalWrite(motor3aPin, LOW);
    digitalWrite(motor3bPin, HIGH);

    // answer with "ok"
    Serial.print("*bdf#");
    Serial.flush();
  } /*
  else
  // BOTBACKWARD = "bot drive backward"
  if (command == "*bdb#")
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
    Serial.print("*bdb#");
    Serial.flush();
  }
  else
  // BOTLEFT = "bot drive left"
  if (command == "*bdl#")
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
    Serial.print("*bdl#");
    Serial.flush();
  }
  else
  // BOTRIGHT = "bot drive right"
  if (command == "*bdr#")
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
    Serial.print("*bdr#");
    Serial.flush();
  }*/
  else
  // TURNLEFT = "bot turn left"
  if (command == "*btl#")
  {
    // motor 1 = right = forward = clockwise
    digitalWrite(motor1aPin, LOW);
    digitalWrite(motor1bPin, HIGH);
    // motor 2 = right = forward = clockwise
    digitalWrite(motor2aPin, LOW);
    digitalWrite(motor2bPin, HIGH);
    // motor 3 = right = forward = clockwise
    digitalWrite(motor3aPin, LOW);
    digitalWrite(motor3bPin, HIGH);

    // answer with "ok"
    Serial.print("*btl#");
    Serial.flush();
  }
  else
  // TURNRIGHT = "bot turn right"
  if (command == "*btr#")
  {
    // motor 1 = left = backward = counterclockwise
    digitalWrite(motor1aPin, HIGH);
    digitalWrite(motor1bPin, LOW);
    // motor 2 = left = backward = counterclockwise
    digitalWrite(motor2aPin, HIGH);
    digitalWrite(motor2bPin, LOW);
    // motor 3 = left = backward = counterclockwise
    digitalWrite(motor3aPin, HIGH);
    digitalWrite(motor3bPin, LOW);

    // answer with "ok"
    Serial.print("*btr#");
    Serial.flush();
  }
  else
  // GSM init (FONA) = "gsmi"
  if (command == "*gsmi#")
  {
    // FONA init in setup() okay?    
    if (FONAstate == false)
    {
      // answer "error"
      if (Serial.print("*err#") < 5)
      {
        // ERROR!!
        delay(10000);
        return;
      }
      // write all data immediately!
      Serial.flush();
    }
    else
    {
      // answer "ok"
      if (Serial.print("*gsmi#") < 6)
      {
        // ERROR!!
        delay(10000);
        return;
      }
      // write all data immediately!
      Serial.flush();
    }
  } // gsmi
  else
  // GSM PIN unlock (FONA) =  "gsmp"
  if (command == "*gsmp#")
  {
    if (unlockSIM() == -1)
    {
      // ERROR

      // store state
      FONAstate = false;

      // all LEDs red
      allLEDsRed();

      // answer "ok"
      if (Serial.print("*err#") < 5)
      {
        // ERROR!!
        delay(10000);
        return;
      }
      // write all data immediately!
      Serial.flush();
    }
    else
    {
      // OKAY

      // store state
      FONAstate = true;      

      // answer "ok"
      if (Serial.print("*gsmp#") < 6)
      {
        // ERROR!!
        delay(10000);
        return;
      }
      // write all data immediately!
      Serial.flush();
    } // PIN okay
  } // gsmp
  // (get) GSM status (FONA) = "gsms"
  if (command == "*gsms#")
  {
    // read the network/cellular status
    uint8_t networkStatus = fona.getNetworkStatus();

    if (Serial.print("*") < 1)
    {
      // ERROR!!
      delay(10000);
      return;
    }
    // write all data immediately!
    Serial.flush();

    // print network status
    if (Serial.print( networkStatus ) < 1)
    {
      // ERROR!!
      delay(10000);
      return;
    }
    // write all data immediately!
    Serial.flush();

    if (Serial.print("#") < 1)
    {
      // ERROR!!
      delay(10000);
      return;
    }
    // write all data immediately!
    Serial.flush();
  } // gsms
  else
  // SMS_COUNT / SMS_CHECK = "smsc"
  if (command == "*smsc#")
  {
    // read and store(!) the number of SMS's
    smsnum = fona.getNumSMS();

    // success ?
    if (smsnum < 0)
    {
      // answer with "ERROR"
      Serial.print("*err#");
      Serial.flush();
    }
    else
    {
      if (Serial.print("*") < 1)
      {
        // ERROR!!
        delay(10000);
        return;
      }
      // write all data immediately!
      Serial.flush();

      // print no of SMS
      if (Serial.print( smsnum ) < 1)
      {
        // ERROR!!
        delay(10000);
        return;
      }
      // write all data immediately!
      Serial.flush();

      if (Serial.print("#") < 1)
      {
        // ERROR!!
        delay(10000);
        return;
      }
      // write all data immediately!
      Serial.flush();
    } // okay
  } // smsc
  else
  // read Last SMS = "smsl"
  if (command == "*smsl#")
  {
    // Retrieve content of SMS No. "smsnum"
    uint16_t smslength;

    // pass in buffer and max len (255)!
    if (! fona.readSMS(smsnum, replybuffer, 250, &smslength))
    {
      // answer with "ERROR"
      Serial.print("*err#");
      Serial.flush();
    }
    else
    {
      // success
      if (Serial.print("*") < 1)
      {
        // ERROR!!
        delay(10000);
        return;
      }
      // write all data immediately!
      Serial.flush();

      // print SMS content
      if (Serial.print( replybuffer ) < 1)
      {
        // ERROR!!
        delay(10000);
        return;
      }
      // write all data immediately!
      Serial.flush();

      if (Serial.print("#") < 1)
      {
        // ERROR!!
        delay(10000);
        return;
      }
      // write all data immediately!
      Serial.flush();
    } // okay
  } // smsl

  // no valid command found (i.e. *wtf# )
  // delete command string
  command = "";

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
   Serial.print(stringbuffer);
   */  // to be ported    
}


void setMotorSpeed()
{
  // PWM motor 1
  analogWrite(motor1PWMPin, DEFAULTMOTORSPEED);
  // PWM motor 2
  analogWrite(motor2PWMPin, DEFAULTMOTORSPEED);
  // PWM motor 3
  analogWrite(motor3PWMPin, DEFAULTMOTORSPEED);
  // PWM motor 4
  analogWrite(motor4PWMPin, DEFAULTMOTORSPEED);
}


void redLED(uint8_t state)
{
  if (state == ON)
  {
    // red LED on
    digitalWrite(ledRed, HIGH);
  }
  else
  {
    // red LED off
    digitalWrite(ledRed, LOW);
  }
}


void yellowLED(uint8_t state)
{
  if (state == ON)
  {
    // yellow LED on
    digitalWrite(ledYellow, HIGH);
  }
  else
  {
    // yellow LED off
    digitalWrite(ledYellow, LOW);
  }
}


void greenLED(uint8_t state)
{
  if (state == ON)
  {
     // green LED on
     digitalWrite(ledGreen, HIGH);
  }
  else
  {
    // green LED off
    digitalWrite(ledGreen, LOW);
  }
}


void allLEDsRed()
{
  digitalWrite(RGBLED1red,   128);
  digitalWrite(RGBLED1green,   0);
  digitalWrite(RGBLED1blue,    0);

  digitalWrite(RGBLED2red,   128);
  digitalWrite(RGBLED2green,   0);
  digitalWrite(RGBLED2blue,    0);

  digitalWrite(RGBLED3red,   128);
  digitalWrite(RGBLED3green,   0);
  digitalWrite(RGBLED3blue,    0);
  
  digitalWrite(RGBLED4red,   128);
  digitalWrite(RGBLED4green,   0);
  digitalWrite(RGBLED4blue,    0);
  
  digitalWrite(RGBLED5red,   128);
  digitalWrite(RGBLED5green,   0);
  digitalWrite(RGBLED5blue,    0);
  
  digitalWrite(RGBLED6red,   128);
  digitalWrite(RGBLED6green,   0);
  digitalWrite(RGBLED6blue,    0);
}


void allLEDsGreen()
{
  analogWrite(RGBLED1red,     0);
  analogWrite(RGBLED1green, 128);
  analogWrite(RGBLED1blue,    0);

  analogWrite(RGBLED2red,     0);
  analogWrite(RGBLED2green, 128);
  analogWrite(RGBLED2blue,    0);

  analogWrite(RGBLED3red,     0);
  analogWrite(RGBLED3green, 128);
  analogWrite(RGBLED3blue,    0);

  analogWrite(RGBLED4red,     0);
  analogWrite(RGBLED4green, 128);
  analogWrite(RGBLED4blue,    0);

  analogWrite(RGBLED5red,     0);
  analogWrite(RGBLED5green, 128);
  analogWrite(RGBLED5blue,    0);
  
  analogWrite(RGBLED6red,     0);
  analogWrite(RGBLED6green, 128);
  analogWrite(RGBLED6blue,    0);
}


void allLEDsBlue()
{
  analogWrite(RGBLED1red,    0);
  analogWrite(RGBLED1green,  0);
  analogWrite(RGBLED1blue, 128);

  analogWrite(RGBLED2red,    0);
  analogWrite(RGBLED2green,  0);
  analogWrite(RGBLED2blue, 128);

  analogWrite(RGBLED3red,    0);
  analogWrite(RGBLED3green,  0);
  analogWrite(RGBLED3blue, 128);
  
  analogWrite(RGBLED4red,    0);
  analogWrite(RGBLED4green,  0);
  analogWrite(RGBLED4blue, 128);
  
  analogWrite(RGBLED5red,    0);
  analogWrite(RGBLED5green,  0);
  analogWrite(RGBLED5blue, 128);
  
  analogWrite(RGBLED6red,    0);
  analogWrite(RGBLED6green,  0);
  analogWrite(RGBLED6blue, 128);
}


void allLEDsWhite()
{
  analogWrite(RGBLED1red,   128);
  analogWrite(RGBLED1green, 128);
  analogWrite(RGBLED1blue,  128);

  analogWrite(RGBLED2red,   128);
  analogWrite(RGBLED2green, 128);
  analogWrite(RGBLED2blue,  128);

  analogWrite(RGBLED3red,   128);
  analogWrite(RGBLED3green, 128);
  analogWrite(RGBLED3blue,  128);

  analogWrite(RGBLED4red,   128);
  analogWrite(RGBLED4green, 128);
  analogWrite(RGBLED4blue,  128);

  analogWrite(RGBLED5red,   128);
  analogWrite(RGBLED5green, 128);
  analogWrite(RGBLED5blue,  128);

  analogWrite(RGBLED6red,   128);
  analogWrite(RGBLED6green, 128);
  analogWrite(RGBLED6blue,  128);
}


void allLEDsOff()
{
  analogWrite(RGBLED1red, 0);
  analogWrite(RGBLED1green, 0);
  analogWrite(RGBLED1blue, 0);

  analogWrite(RGBLED2red, 0);
  analogWrite(RGBLED2green, 0);
  analogWrite(RGBLED2blue, 0);

  analogWrite(RGBLED3red, 0);
  analogWrite(RGBLED3green, 0);
  analogWrite(RGBLED3blue, 0);
  
  analogWrite(RGBLED4red, 0);
  analogWrite(RGBLED4green, 0);
  analogWrite(RGBLED4blue, 0);
  
  analogWrite(RGBLED5red, 0);
  analogWrite(RGBLED5green, 0);
  analogWrite(RGBLED5blue, 0);
  
  analogWrite(RGBLED6red, 0);
  analogWrite(RGBLED6green, 0);
  analogWrite(RGBLED6blue, 0);
}


void relais(uint8_t state)
{
  if (state == ON)
  {
     // relais on
     digitalWrite(relaisPin, HIGH);
  }
  else
  {
    // relais off
    digitalWrite(relaisPin, LOW);
  }
}


// print a letter on the adafruit bi-color 8x8 led matrix
void letter(String text, uint16_t color)
{
  /*------------------ DEBUG 2 ------------------------/
  matrix.clear();
  matrix.setTextColor(color);
  matrix.setCursor(0,0);
  matrix.print(text);
  matrix.writeDisplay();
  //------------------ DEBUG 2 ------------------------*/
}


// Stores the IR codes
// Most of this code is just logging
void storeCode(decode_results *results)
{
  codeType = results->decode_type;
  int count = results->rawlen;

  if (codeType == UNKNOWN)
  {
//    Serial.println("Received unknown code, saving as raw");

    codeLen = results->rawlen - 1;

    // To store raw codes:
    // Drop first value (gap)
    // Convert from ticks to microseconds
    // Tweak marks shorter, and spaces longer to cancel out IR receiver distortion
    for (int i = 1; i <= codeLen; i++)
    {
      if (i % 2)
      {
        // Mark
        rawCodes[i - 1] = results->rawbuf[i]*USECPERTICK - MARK_EXCESS;
//        Serial.print(" m");
      } 
      else
      {
        // Space
        rawCodes[i - 1] = results->rawbuf[i]*USECPERTICK + MARK_EXCESS;
//        Serial.print(" s");
      }
//      Serial.print(rawCodes[i - 1], DEC);
    }
//    Serial.println("");
  }
  else
  {
    if (codeType == NEC)
    {
      //      Serial.print("Received NEC / Apple: ");

      if (results->value == REPEAT) 
      {
        // Don't record a NEC repeat value as that's useless.
        //        Serial.println("repeat; ignoring.");
        return;
      }
    }
    /* 
     else
     {    
     if (codeType == SONY)
     {
     Serial.print("Received SONY: ");
     } 
     else
     {
     if (codeType == RC5)
     {
     Serial.print("Received RC5: ");
     } 
     else
     {
     if (codeType == RC6)
     {
     Serial.print("Received RC6: ");
     } 
     else
     {
     Serial.print("Unexpected codeType ");
     Serial.print(codeType, DEC);
     Serial.println("");
     }
     }
     }
     }
     */

    //    Serial.println(results->value, HEX);
    //    Serial.println(results->value, DEC);

    codeValue = results->value;
    codeLen = results->bits;
  }
}


// FONA: read the number of SMS's
int8_t readNumSMS()
{
  int8_t smsnum = fona.getNumSMS();


  if (smsnum < 0)
  {
    return -1;
  }

  return smsnum; 
}


// FONA: Unlock the SIM with a PIN code
int unlockSIM()
{
  char PIN[5];


  // PIN
  PIN[0] = '5';
  PIN[1] = '5';
  PIN[2] = '5';
  PIN[3] = '5';
  PIN[4] = NULL;

  // unlock
  if (! fona.unlockSIM(PIN))
  {
    // error
    return -1;
  } 
  else
  {
    // ok
    return 0;
  }        
}
