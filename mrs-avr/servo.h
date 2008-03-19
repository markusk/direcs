#ifndef SERVO_H
#define SERVO_H

#include <avr/io.h>

void setServoPosition(uint8_t servo, uint16_t value);
void startPWMServo(uint8_t servo);
void stopPWMServo(uint8_t servo);

#endif
