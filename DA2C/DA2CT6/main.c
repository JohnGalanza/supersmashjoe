#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

volatile uint8_t OVFCount;				//Global variable
volatile int Pressed;					//Button pressed variable

int main(void)
{
	DDRB |= (1<<2);						//PORTB bit 2 is an output
	DDRC &= (0<<1);						//PORTC bit 2 is an input
	PORTC |= (1<<1);					//Enable pull-up
	
	TIMSK0 |= (1<<OCIE0A);
	TCCR0A |= (1<<WGM01);				//CTC mode
	TCCR0B	|= (1<<CS02)|(1<<CS00);		//Prescaler set to 1024
	TCNT0 = 0x56;						//Timer starts at 86
	OCR0A = 0xFF;						//Timer ends at 255
	sei();
	while(1)
	{
		if(!(PINC & 0x04))
		{
			Pressed = 1;
		}
		
	}
}

ISR(TIMER0_COMPA_vect)
{
	if((OVFCount <= 80) && (Pressed == 1))
	{
		PORTB &= ~(1<<2);				//Turns LED ON
		TCNT0 = 0x0A;
		OVFCount++;
		
		if(OVFCount == 80)
		{
			PORTB |= (1<<2);			//Turns LED OFF
			TCNT0 = 0x0A;
			OVFCount =0;				//Resets OVF
			Pressed = 0;
		}
	}
}