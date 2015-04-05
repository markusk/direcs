// give it a name:
int ledGreen     =  2;
int ledYellow    =  3;
// Pin 13 has an LED connected on most Arduino boards.
int ledRed     = 13; /// @todo change name (color)

int relaisPin    =  4;

int motor1aPin   =  5;
int motor1bPin   =  6;
int motor1DirPin =  7;
int motor1PWMPin =  8;
int motor2aPin   =  9;
int motor2bPin   = 10;
int motor2DirPin = 11;
int motor2PWMPin = 12;
int motor3aPin   = 13;
int motor3bPin   = 14;
int motor3DirPin = 15;
int motor3PWMPin = 16;
int motor4aPin   = 17;
int motor4bPin   = 18;
int motor4DirPin = 19;
int motor4PWMPin = 20;

int RGBLED1red   = 21;
int RGBLED1green = 22;
int RGBLED1blue  = 23;
int RGBLED2red   = 24;
int RGBLED2green = 25;
int RGBLED2blue  = 26;
int RGBLED3red   = 27;
int RGBLED3green = 28;
int RGBLED3blue  = 29;

// test:
const int analogInPin = A0;  // Analog input pin that the potentiometer is attached to
int sensorValue = 0;        // value read from the poti


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
  // initialize serial
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
  pinMode(motor1DirPin, OUTPUT);
  pinMode(motor1PWMPin, OUTPUT);

  pinMode(motor2aPin, OUTPUT);
  pinMode(motor2bPin, OUTPUT);
  pinMode(motor2DirPin, OUTPUT);
  pinMode(motor2PWMPin, OUTPUT);

  pinMode(motor3aPin, OUTPUT);
  pinMode(motor3bPin, OUTPUT);
  pinMode(motor3DirPin, OUTPUT);
  pinMode(motor3PWMPin, OUTPUT);

  pinMode(motor4aPin, OUTPUT);
  pinMode(motor4bPin, OUTPUT);
  pinMode(motor4DirPin, OUTPUT);
  pinMode(motor4PWMPin, OUTPUT);

  pinMode(RGBLED1red, OUTPUT;
  pinMode(RGBLED1green, OUTPUT);
  pinMode(RGBLED1blue, OUTPUT);

  pinMode(RGBLED2red, OUTPUT;
  pinMode(RGBLED2green, OUTPUT);
  pinMode(RGBLED2blue, OUTPUT);

  pinMode(RGBLED3red, OUTPUT;
  pinMode(RGBLED3green, OUTPUT);
  pinMode(RGBLED3blue, OUTPUT);
  //-------------------------------------------------------------------------------------------------


  // this an input
  pinMode(analogInPin, INPUT);

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
  digitalWrite(motor1DirPin, LOW);  
  digitalWrite(motor1PWMPin, LOW);

  digitalWrite(motor2aPin, LOW);
  digitalWrite(motor2bPin, LOW);
  digitalWrite(motor2DirPin, LOW);  
  digitalWrite(motor2PWMPin, LOW);

  digitalWrite(motor3aPin, LOW);
  digitalWrite(motor3bPin, LOW);
  digitalWrite(motor3DirPin, LOW);  
  digitalWrite(motor3PWMPin, LOW);

  digitalWrite(motor4aPin, LOW);
  digitalWrite(motor4bPin, LOW);
  digitalWrite(motor4DirPin, LOW);  
  digitalWrite(motor4PWMPin, LOW);


  //-------------------------------------------------------------------------------------------------
  // turn all RGB LEDs
  //-------------------------------------------------------------------------------------------------
  digitalWrite(RGBLED1red, LOW);
  digitalWrite(RGBLED1green, LOW);
  digitalWrite(RGBLED1blue, LOW);

  digitalWrite(RGBLED2red, LOW);
  digitalWrite(RGBLED2green, LOW);
  digitalWrite(RGBLED2blue, LOW);

  digitalWrite(RGBLED3red, LOW);
  digitalWrite(RGBLED3green, LOW);
  digitalWrite(RGBLED3blue, LOW);

      
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

  // initialize the PWM timer (with compare value 100)  [this is the motor speed!]
  // This value is changed by the mrs programm, when value is read from ini-file!
  // 100 * 64 µs = 6400 µs = 6,4 ms
  //
  // PWM motor 1
  analogWrite(motor1PWMPin, 60);
  // PWM motor 2
  analogWrite(motor2PWMPin, 60);
  // PWM motor 3
  analogWrite(motor3PWMPin, 60);
  // PWM motor 4
  analogWrite(motor4PWMPin, 60);

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
}


void loop()
{
  static uint8_t string_started = 0;  // Sind wir jetzt im String?

  //  letter("w", LED_RED);

  do
  {
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

    // RGB LEDs off
    digitalWrite(RGBLED1red, LOW);
    digitalWrite(RGBLED1green, LOW);
    digitalWrite(RGBLED1blue, LOW);

    digitalWrite(RGBLED2red, LOW);
    digitalWrite(RGBLED2green, LOW);
    digitalWrite(RGBLED2blue, LOW);

    digitalWrite(RGBLED3red, LOW);
    digitalWrite(RGBLED3green, LOW);
    digitalWrite(RGBLED3blue, LOW);
     
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
    /*------------------ DEBUG 2 ------------------------/
    letter("z", LED_YELLOW);
    //------------------ DEBUG 2 ------------------------*/

    greenLED(OFF);
    yellowLED(OFF);

    // read the analog in value
    // print the results to the serial monitor:
    if (Serial.print("*") < 1)
    {
      /*------------------ DEBUG 2 ------------------------/
      letter("y", LED_YELLOW);
      //------------------ DEBUG 2 ------------------------*/

      // ERROR!!
      delay(10000);
      return;
    }
    // write all data immediately!
    Serial.flush();

    if (Serial.print( analogRead(SENSOR7) ) < 1)
    {
      /*------------------ DEBUG 2 ------------------------/
      letter("x", LED_YELLOW);
      //------------------ DEBUG 2 ------------------------*/

      // ERROR!!
      delay(10000);
      return;
    }
    // write all data immediately!
    Serial.flush();
    
    if (Serial.print("#") < 1)
    {
      /*------------------ DEBUG 2 ------------------------/
      letter("w", LED_YELLOW);
      //------------------ DEBUG 2 ------------------------*/

      // ERROR!!
      delay(10000);
      return;
    }
    // write all data immediately!
    Serial.flush();
    
    /*------------------ DEBUG 2 ------------------------/
    letter("A", LED_YELLOW);
    //------------------ DEBUG 2 ------------------------*/
  }
  else
  // READ_SENSOR_8 (12 V supply)
  if (command == "*s8#")
  {
    /*------------------ DEBUG 2 ------------------------/
    letter("u", LED_YELLOW);
    //------------------ DEBUG 2 ------------------------*/

    greenLED(OFF);
    yellowLED(OFF);

    // read the analog in value
    // print the results to the serial monitor:
    if (Serial.print("*") < 1)
    {
      /*------------------ DEBUG 2 ------------------------/
      letter("t", LED_YELLOW);
      //------------------ DEBUG 2 ------------------------*/

      // ERROR!!
      delay(10000);
      return;
    }
    // write all data immediately!
    Serial.flush();

    if (Serial.print( analogRead(SENSOR8) ) < 1)
    {
      /*------------------ DEBUG 2 ------------------------/
      letter("s", LED_YELLOW);
      //------------------ DEBUG 2 ------------------------*/

      // ERROR!!
      delay(10000);
      return;
    }
    // write all data immediately!
    Serial.flush();

    if (Serial.print("#") < 1)
    {
      /*------------------ DEBUG 2 ------------------------/
      letter("r", LED_YELLOW);
      //------------------ DEBUG 2 ------------------------*/

      // ERROR!!
      delay(10000);
      return;
    }
    // write all data immediately!
    Serial.flush();

    /*------------------ DEBUG 2 ------------------------/
    letter("q", LED_YELLOW);
    //------------------ DEBUG 2 ------------------------*/
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
    command = command.substring(5, 3);
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
    command = command.substring(5, 3);
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
    command = command.substring(5, 3);
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
    command = command.substring(5, 3);
    // get value from string and set speed
    analogWrite(motor4PWMPin, command.toInt());
    Serial.print("*mv4#");
    Serial.flush();
  }
  else
  // SPEED_SET_ALLMOTORS
  if (command == "*mv0#")
  {
    // get the 3 digits which contain the speed
    command = command.substring(5, 3);
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
    command = command.substring(5, 3);
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
    command = command.substring(5, 3);
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
    command = command.substring(5, 3);
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
    command = command.substring(5, 3);
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
    command = command.substring(5, 3);
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
    command = command.substring(5, 3);
    // get value from string and set value
    analogWrite(RGBLED2blue, command.toInt());
    Serial.print("*rgb6#");
    Serial.flush();
  }

  /*
  else
  // BOTSTOP
  if (command == "*bst#")
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
    Serial.print("*bst#");
    Serial.flush();
  }
  else
  // BOTWAIT
  if (command == "*bwa#")
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
    Serial.print("*bwa#");
    Serial.flush();
  }
  else
  // BOTSTART = "bot go"
  if (command == "*bgo#")
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
    Serial.print("*bgo#");
    Serial.flush();
  }
  else
  // BOTFORWARD = "bot drive forward"
  if (command == "*bdf#")
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
    Serial.print("*bdf#");
    Serial.flush();
  }
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
  }
  else
  // TURNLEFT = "bot turn left"
  if (command == "*btl#")
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
    Serial.print("*btl#");
    Serial.flush();
  }
  else
  // TURNRIGHT = "bot turn right"
  if (command == "*btr#")
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
    Serial.print("*btr#");
    Serial.flush();
  }
*/    // to be ported    

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

