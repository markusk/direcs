int ledR1 = 3;           // the pin that the LED is attached to
int ledG1 = 5;           // the pin that the LED is attached to
int ledB1 = 6;           // the pin that the LED is attached to
int brightness = 0;    // how bright the LED is
int fadeAmount = 5;    // how many points to fade the LED by

// the setup routine runs once when you press reset:
void setup()
{
  // declare pin to be an output:
  pinMode(ledR1, OUTPUT);
  pinMode(ledG1, OUTPUT);
  pinMode(ledB1, OUTPUT);
}

// the loop routine runs over and over again forever:
void loop()
{
  // set the brightness of the pin
  analogWrite(ledR1, brightness);
  analogWrite(ledG1, brightness);
  analogWrite(ledB1, brightness);

  // change the brightness for next time through the loop:
  brightness = brightness + fadeAmount;

  // reverse the direction of the fading at the ends of the fade:
  if (brightness == 0 || brightness == 255)
  {
    fadeAmount = -fadeAmount ;
  }
  // wait for 30 milliseconds to see the dimming effect
  delay(30);
}

