#ifndef ADCONV_H
#define ADCONV_H

#include <avr/io.h>
#include <stdlib.h> // für utoa und itoa

/*
/// initialise the AD converter
void initADC();
*/

// read a value from the AD the normal way (without Interrupt!)
uint16_t readADC(unsigned char channel);

#endif
