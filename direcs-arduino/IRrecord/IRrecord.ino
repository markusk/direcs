#include <IRremote.h>

// Werte, die die Apple Remote "runder Donut, Alu" sendet
unsigned long oben = 2011287637;
unsigned long unten = 2011279445;
unsigned long links = 2011238485;
unsigned long rechts = 2011291733;
unsigned long ok = 2011282005;
unsigned long menu = 2011250773;
unsigned long play = 2011265621;
unsigned long repeat = 2011242581; // Wird bei ok und play zusätzlich gesendet

/*
// Werte, die die Apple Remote "flacher Donut, Alu" sendet
unsigned long oben = 2011287644;
unsigned long unten = 2011279452;
unsigned long links = 2011238492;
unsigned long rechts = 2011291740;
unsigned long ok = 2011282012;
unsigned long menu = 2011250780;
unsigned long play = 2011265628;
unsigned long repeat = 2011242588; // Wird bei ok und play zusätzlich gesendet
*/

const int RECV_PIN   = 2;
const int LED_PIN    = 13;
const int RELAIS_PIN = 5;

boolean relaisState = false;

IRrecv irrecv(RECV_PIN);

decode_results results;


void setup()
{
  Serial.begin(9600);
  irrecv.enableIRIn(); // Start the receiver

  pinMode(LED_PIN, OUTPUT);
  pinMode(RELAIS_PIN, OUTPUT);
}


// Storage for the recorded code
         int  codeType = -1;     // The type of code
unsigned long codeValue;         // The code value if not raw
unsigned int  rawCodes[RAWBUF];  // The durations if raw
         int  codeLen;           // The length of the code
         int toggle = 0;         // The RC5/6 toggle state


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
                Serial.println("[LINKS]");
            }
            else
            {
              if (codeValue == rechts)
              {
                  Serial.println("[RECHTS]");
              }
              else
              {
                if (codeValue == ok)
                {
                    // toggle relais state
                    relaisState = !relaisState;

                    // switch relais
                    digitalWrite(RELAIS_PIN, relaisState);

                    Serial.print("Relais ");

                    if (relaisState)
                    {
                      Serial.println("on.");
                    }
                    else
                    {
                      Serial.println("off.");
                    }

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

