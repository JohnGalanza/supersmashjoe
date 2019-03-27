#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>


int main(void)
{	
	uint8_t OVFCount = 0;
	DDRB |= (1<<2);			
	TCCR0A = 0x00;		//NORMAL mode
	TCCR0B	|= (1<<CS02);
	TCCR0B	|= (1<<CS00);				//PRESCALER set to 1024	
	TCNT0 = 0x56;						//Resets to 0 
	
	while(1)
	{
		for(OVFCount =0;OVFCount<40; OVFCount++)
		{
			PORTB &= ~(1<<2);				//Turns LED ON
			TCNT0 = 0x56;					//resets counter to 0 
			while((TIFR0 & 0x01) == 0);
			TIFR0 = 0x01;
		}
		for(OVFCount =0;OVFCount<40; OVFCount++)
		{
			PORTB |= (1<<2);				//Turns LED OFF
			TCNT0 = 0x89;					//resets counter to 0
			while((TIFR0 & 0x01) == 0);
			TIFR0 = 0x01;
		}
	}
}




