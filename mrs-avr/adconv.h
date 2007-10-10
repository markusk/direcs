#ifndef ADCONV_H
#define ADCONV_H

#include <avr/io.h>
#include <stdlib.h> // für utoa und itoa

uint16_t readADC(unsigned char channel);

#endif
