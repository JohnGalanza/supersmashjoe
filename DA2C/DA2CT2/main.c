#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

int main(void)
{
	uint8_t OVFCount =0;
    DDRB |= (1<<2);					//PORTB bit 2 is an output
    DDRC &= (0<<1);					//PORTC bit 2 is an input
    PORTC |= (1<<1);				//Enable pull-up
    
	TCCR0A = 0x00;					//NORMAL mode
	TCCR0B	|= (1<<CS02);
	TCCR0B	|= (1<<CS00);			//PRESCALER set to 1024	

    while (1)
    {
		if(!(PINC & (1<<PINC1)))			//checks if button is pressed
		{
			for(OVFCount =0;OVFCount<80; OVFCount++) //OVF
			{
				PORTB &= ~(1<<2);				//Turns LED ON
				TCNT0 = 0x0A;					//resets counter to 0
				while((TIFR0 & 0x01) == 0);
				TIFR0 = 0x01;
			}
			
		}
		else
			{
				PORTB |= (1<<2);			//keeps pb2 off
			}
	}
}

