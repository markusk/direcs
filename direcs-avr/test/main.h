#ifndef MAIN_H
#define MAIN_H

#ifndef F_CPU
#define F_CPU 		16000000
#endif

#include <avr/io.h>

#include <inttypes.h>
#include <avr/interrupt.h>
#include <avr/signal.h>
#include <stdlib.h> // für utoa und itoa
#include <avr/delay.h>

// the prototypes
int main(void);

#endif
