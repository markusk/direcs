#include "ultra.h"

#ifndef F_CPU
#define F_CPU 		16000000
#endif

#include <util/delay.h>


// Zeit/srf05 Messung, beliebige Pins können gewählt werden

uint16_t ultraschall_messung(void)
{
	volatile uint16_t wert = 0;


	// Trigger-Puls auf high
	PORTC |= (1<<PIN2);
	
	_delay_us(26); // 20 µs in methode dauern -> 10 µs lt. Oszi !
	
	// Trigger-Puls Eingang wieder auf low
	PORTC &= ~(1<<PIN2);
	
	// wait 600 µs
	_delay_us(200); // 200 µs in methode dauern -> 100 µs lt. Oszi !
	_delay_us(200); // 200 µs in methode dauern -> 100 µs lt. Oszi !
	_delay_us(200); // 200 µs in methode dauern -> 100 µs lt. Oszi !
	_delay_us(200); // 200 µs in methode dauern -> 100 µs lt. Oszi !
	_delay_us(200); // 200 µs in methode dauern -> 100 µs lt. Oszi !
	_delay_us(200); // 200 µs in methode dauern -> 100 µs lt. Oszi !

	
	//--------------------------------------------
	// Warten bis Echo/Mess Eingang auf HIGH geht
	//--------------------------------------------
	do
	{
	} while ( !(PINC & (1<<PIN3)) );
	// solange Bit "nicht ungleich 0", also "0" ist
	// warten auf HIGH!
	
	
	//---------------------
	// HIGH wurde erkannt
	//---------------------
	// Timer auf 0 setzen
	TCNT1 = 0;
	// Timer starten und Prescaler = 8
	// Damit dauert ein Takt des Zählers bei 16 MHz genau 0,5 µs (entspricht 2 MHz)
	TCCR1B |= (1<<CS11);
	
	
	//-------------------------------------------
	// Warten bis Echo/Mess Eingang auf low geht
	//-------------------------------------------
	// warte und zähle mit Timer "solange high ist"
	while (PINC & (1<<PIN3))
	{
		wert = TCNT1;
	}
	
	// Timer stoppen
	// org: TCCR1B &= ~(1<<CS11);
	TCCR1B = 0;
	
	// Korrektur des Zählerwertes, da ein Zählertakt 0,5µs dauert.
	// -> Umwandlung in Millisekunden ( wert : 2 )
	wert /= 2;


	// SRF05 Messung. Umrechung in cm.
	// Gültiger Bereich (max 25 ms + Reserve = 30ms -> 30.000 µs)
	if (wert < 30000)
	{
		// umrechnen in cm    (laut Datenblatt )
		wert /= 58;
	}
	else
	{
		// Timeout.Also maximale Entfernung. Beim SFR05 = 400 cm
		wert = 400;
	}
	
	
	return wert;
}
