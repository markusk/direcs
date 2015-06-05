void setup()
{
  #define RGBLED1red    3
  #define RGBLED1green  4
  #define RGBLED1blue   5
  
  #define RGBLED2red    6
  #define RGBLED2green  7
  #define RGBLED2blue   8
  
  #define RGBLED3red    9
  #define RGBLED3green 10
  #define RGBLED3blue  11
/*  
  #define RGBLED4red    3 // same like above from here
  #define RGBLED4green  4
  #define RGBLED4blue   5
  
  #define RGBLED5red    6
  #define RGBLED5green  7
  #define RGBLED5blue   8
  
  #define RGBLED6red    9
  #define RGBLED6green 10
  #define RGBLED6blue  11
*/

  analogWrite(RGBLED1red,   128);
  analogWrite(RGBLED1green,   0);
  analogWrite(RGBLED1blue,    0);

  analogWrite(RGBLED2red,   128);
  analogWrite(RGBLED2green,   0);
  analogWrite(RGBLED2blue,    0);

  analogWrite(RGBLED3red,   128);
  analogWrite(RGBLED3green,   0);
  analogWrite(RGBLED3blue,    0);
/*
  analogWrite(RGBLED4red,     0);
  analogWrite(RGBLED4green, 128);
  analogWrite(RGBLED4blue,    0);

  analogWrite(RGBLED5red,     0);
  analogWrite(RGBLED5green, 128);
  analogWrite(RGBLED5blue,    0);
  
  analogWrite(RGBLED6red,     0);
  analogWrite(RGBLED6green, 128);
  analogWrite(RGBLED6blue,    0);
*/
}


void loop()
{
  delay(1000);
}
