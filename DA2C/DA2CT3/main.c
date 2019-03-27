 
#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

volatile uint8_t OVFCount;

int main(void)
{
	DDRB |= (1<<2);
    TIMSK0 |= (1<<TOIE0);
	TCCR0A = 0x00;					//NORMAL mode
	TCCR0B	|= (1<<CS02);
	TCCR0B	|= (1<<CS00);			//Prescaler set to 1024
	TCNT0 = 0x56;

	sei();	
	while(1){}
	
}

ISR(TIMER0_OVF_vect)
{
	if(OVFCount < 40)
	{
		PORTB &= ~(1<<2);				//Turns LED ON
		TCNT0 = 0x56;
		OVFCount++;
	}
	else
	{
		PORTB |= (1<<2);				//Turns LED OFF
		TCNT0 = 0x89;
		OVFCount++;
		if(OVFCount == 80)
			{
				OVFCount =0;
			}
	}
}
