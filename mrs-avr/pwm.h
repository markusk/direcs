#ifndef PWM_H
#define PWM_H

#include <avr/io.h>

void setPWMwidth(uint8_t motor, uint8_t value);
void startPWM(void);
void stopPWM(void);

#endif
