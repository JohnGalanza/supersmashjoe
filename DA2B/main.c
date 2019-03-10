#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

int main()
{
	DDRB |= (1<<2);
	//|= (1<<5) | (1<<4);
	PORTD |= 1<<2;
	EICRA = 0x2;	
	EIMSK = (1<<INT0);
	sei();
	
	while(1)
	{
		PORTB |= (1<<2);
	}
}

ISR (INT0_vect)
{
	PORTB &= (0<<2);
	_delay_ms(1250);
}