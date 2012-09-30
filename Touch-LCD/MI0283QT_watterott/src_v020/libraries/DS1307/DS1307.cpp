#ifdef __cplusplus
extern "C" {
#endif
  #include <inttypes.h>
  #include <avr/io.h>
  #include <util/delay.h>
#ifdef __cplusplus
}
#endif
#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
#include "../Wire/Wire.h"
#include "DS1307.h"


#define DS1307_ADDR 0x68


//-------------------- Constructor --------------------


DS1307::DS1307(void)
{
  Wire.begin();

  return;
}


//-------------------- Public --------------------


void DS1307::start(void)
{
  uint8_t sec;

  //get second and CH bit
  Wire.beginTransmission(DS1307_ADDR);
  Wire.write(byte(0x00));
  Wire.endTransmission();
  Wire.requestFrom(DS1307_ADDR, 1);
  sec = Wire.read();
  Wire.read();

  //set second and clear CH bit
  Wire.beginTransmission(DS1307_ADDR);
  Wire.write(byte(0x00));	
  Wire.write(sec & 0x7F);
  Wire.endTransmission();

  return;
}


void DS1307::stop(void)
{
  uint8_t sec, d;

  //get second and CH bit
  Wire.beginTransmission(DS1307_ADDR);
  Wire.write(byte(0x00));
  Wire.endTransmission();
  Wire.requestFrom(DS1307_ADDR, 1);
  sec = Wire.read();
  d   = Wire.read();

  //set second and CH bit
  Wire.beginTransmission(DS1307_ADDR);
  Wire.write(byte(0x00));	
  Wire.write(sec | 0x80); //hold
  Wire.endTransmission();

  return;
}


void DS1307::get(uint8_t *sec, uint8_t *min, uint8_t *hour, uint8_t *day, uint8_t *month, uint16_t *year)
{
  //get time and date
  Wire.beginTransmission(DS1307_ADDR);
  Wire.write(byte(0x00));
  Wire.endTransmission();
  Wire.requestFrom(DS1307_ADDR, 7);
  *sec   = bcd2bin(Wire.read() & 0x7F);
  *min   = bcd2bin(Wire.read());
  *hour  = bcd2bin(Wire.read());
           bcd2bin(Wire.read()); //day of week
  *day   = bcd2bin(Wire.read());
  *month = bcd2bin(Wire.read());
  *year  = bcd2bin(Wire.read()) + 2000;

  return;
}


void DS1307::get(int *sec, int *min, int *hour, int *day, int *month, int *year)
{
  return get((uint8_t*)sec, (uint8_t*)min, (uint8_t*)hour, (uint8_t*)day, (uint8_t*)month, (uint16_t*)year);
}


void DS1307::set(uint8_t sec, uint8_t min, uint8_t hour, uint8_t day, uint8_t month, uint16_t year)
{
  uint8_t ch, d;

  //get CH bit
  Wire.beginTransmission(DS1307_ADDR);
  Wire.write(byte(0x00));
  Wire.endTransmission();
  Wire.requestFrom(DS1307_ADDR, 1);
  ch = Wire.read();
  d  = Wire.read();
  
  if(ch & 0x80) //CH set?
  {
    ch = 0x80;
  }
  else
  {
    ch = 0x00;
  }

  //set time and date
  Wire.beginTransmission(DS1307_ADDR);
  Wire.write(byte(0x00));
  Wire.write(bin2bcd(sec) | ch);
  Wire.write(bin2bcd(min));
  Wire.write(bin2bcd(hour));
  Wire.write(bin2bcd(0));
  Wire.write(bin2bcd(day));
  Wire.write(bin2bcd(month));
  Wire.write(bin2bcd(year - 2000));
  Wire.endTransmission();

  return;
}


void DS1307::set(int sec, int min, int hour, int day, int month, int year)
{
  return set((uint8_t)sec, (uint8_t)min, (uint8_t)hour, (uint8_t)day, (uint8_t)month, (uint16_t)year);
}


//-------------------- Private --------------------


uint8_t DS1307::bin2bcd(uint8_t val)
{
  return val + 6 * (val / 10);
}


uint8_t DS1307::bcd2bin(uint8_t val)
{
  return val - 6 * (val >> 4);
}
