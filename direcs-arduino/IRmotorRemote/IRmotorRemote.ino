#include <IRremote.h>

#include <Wire.h>
#include <Adafruit_MotorShield.h>

// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 
// Or, create it with a different I2C address (say for stacking)
// Adafruit_MotorShield AFMS = Adafruit_MotorShield(0x61); 

// Select which 'port' M1, M2, M3 or M4. In this case, M1
Adafruit_DCMotor *myMotor1 = AFMS.getMotor(1);
// You can also make another motor on port M2
Adafruit_DCMotor *myMotor2 = AFMS.getMotor(2);
// You can also make another motor on port M3
Adafruit_DCMotor *myMotor3 = AFMS.getMotor(3);


// Storage for the recorded code
         int  codeType = -1;     // The type of code
unsigned long codeValue;         // The code value if not raw
unsigned int  rawCodes[RAWBUF];  // The durations if raw
         int  codeLen;           // The length of the code
         int toggle = 0;         // The RC5/6 toggle state


// Werte, die die Apple Remote "runder Donut, Alu" sendet
unsigned long oben   = 2011287637;
unsigned long unten  = 2011279445;
unsigned long links  = 2011238485;
unsigned long rechts = 2011291733;
unsigned long ok     = 2011282005;
unsigned long menu   = 2011250773;
unsigned long play   = 2011265621;
unsigned long repeat = 2011242581; // Wird bei ok und play zusätzlich gesendet

/*
// Werte, die die Apple Remote "flacher Donut, Alu" sendet
unsigned long oben   = 2011287644;
unsigned long unten  = 2011279452;
unsigned long links  = 2011238492;
unsigned long rechts = 2011291740;
unsigned long ok     = 2011282012;
unsigned long menu   = 2011250780;
unsigned long play   = 2011265628;
unsigned long repeat = 2011242588; // Wird bei ok und play zusätzlich gesendet
*/

const int RECV_PIN  = 2;  // pin for TSOP1736 IR sensor output
const int LED_PIN   = 13; // pin for the onboard LED

boolean relayState = false;

IRrecv irrecv(RECV_PIN);

decode_results results;


void setup()
{
  Serial.begin(9600);
  irrecv.enableIRIn(); // Start the receiver

  pinMode(LED_PIN, OUTPUT);

  AFMS.begin();  // create with the default PWM frequency 1.6KHz
  //AFMS.begin(1000);  // OR with a different PWM frequency, say 1KHz
  
  // Set the speed to start, from 0 (off) to 255 (max speed)
  myMotor1->setSpeed(150);
  myMotor2->setSpeed(150);
  myMotor3->setSpeed(150);
}


void loop()
{
  // wait for IR signal
  if (irrecv.decode(&results))
  {
    // LED on
    digitalWrite(LED_PIN, HIGH);

    // store result for convenience    
    storeCode(&results);
    
    // resume receiver
    irrecv.resume();

    //---------------------    
    // "translate" to text
    //---------------------    
    if (codeValue == menu)
    {
      Serial.println("[MENU]");
    }
    else
    {
      if (codeValue == play)
      {
        Serial.println("[PLAY]");
      }
      else
      {
        if (codeValue == oben)
        {
          Serial.println("[OBEN]");
        }
        else
        {
          if (codeValue == unten)
          {
            Serial.println("[UNTEN]");
          }
          else
          {
            if (codeValue == links)
            {
              myMotor1->run(FORWARD);
              myMotor2->run(FORWARD);
              myMotor3->run(FORWARD);

              Serial.println("[LINKS]");
            }
            else
            {
              if (codeValue == rechts)
              {
                myMotor1->run(BACKWARD);
                myMotor2->run(BACKWARD);
                myMotor3->run(BACKWARD);

                Serial.println("[RECHTS]");
              }
              else
              {
                if (codeValue == ok)
                {
                  // turn OFF motors
                  myMotor1->run(RELEASE);
                  myMotor2->run(RELEASE);
                  myMotor3->run(RELEASE);

                  Serial.println("[OK]");
                }
              }
            }
          }
        }
      }
    }
  
    // LED off
    digitalWrite(LED_PIN, LOW);
  }
}


// Stores the code for later playback
// Most of this code is just logging
void storeCode(decode_results *results)
{
  codeType = results->decode_type;
  int count = results->rawlen;
  
  if (codeType == UNKNOWN)
  {
    Serial.println("Received unknown code, saving as raw");

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
        Serial.print(" m");
      } 
      else
      {
        // Space
        rawCodes[i - 1] = results->rawbuf[i]*USECPERTICK + MARK_EXCESS;
        Serial.print(" s");
      }
      Serial.print(rawCodes[i - 1], DEC);
    }
    Serial.println("");
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

