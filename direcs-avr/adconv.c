#include "adconv.h"


uint16_t readADC(unsigned char channel)
{
	uint8_t  i = 0;
	uint16_t  result = 0;

	
	// Den ADC aktivieren und Teilungsfaktor auf 64 stellen
	//
	// ADCSRA = AD Control and Status Register A
	// ADEN = AD Enable
	//
	// Original: ADCSRA = (1<<ADEN) | (1<<ADPS2) | (1<<ADPS1);
	ADCSRA = (1<<ADEN) | (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0);

	
	// set MUX5 bit in ADCSRB because of acces of the ADC8 to ADC15 !!
	if (channel < 32)
	{
		// Kanal des Multiplexers waehlen
		// (set only MUX0 to 4 !!)
		ADMUX = channel;
		// delete bit
		ADCSRB &= ~(1<<MUX5);
	}
	else
	{
		//
		// channel >= 32
		//
		
		// Kanal des Multiplexers waehlen
		// (set only MUX5 to 4 !!)
		ADMUX = (channel-32);
		// set bit
		ADCSRB |= (1<<MUX5);
	}
	
	// Interne Referenzspannung verwenden (also 2,56 V)
        ADMUX |= (1<<REFS1) | (1<<REFS0);

	// Den ADC initialisieren und einen sog. Dummyreadout machen
	//
	// ADCSRA = AD Control and Status Register A
	// ADSC = Start Conversion
	//
	ADCSRA |= (1<<ADSC);
	
        while (ADCSRA & (1<<ADSC))
	{
	};

	
	// Jetzt 4x die analoge Spannung and Kanal channel auslesen
	// und dann Durchschnittswert ausrechnen.
	for (i=0; i<4; i++)
	{
		// Eine Wandlung
		ADCSRA |= (1<<ADSC);
		
		// Auf Ergebnis warten...
		while(ADCSRA & (1<<ADSC))
		{
		};

		result += ADCW;
	}

	// ADC wieder deaktivieren
	//
	// ADCSRA = AD Control and Status Register A
	// ADEN = AD Enable
	//
	ADCSRA &= ~(1<<ADEN);

	result /= 4;
	
	return result;
}

