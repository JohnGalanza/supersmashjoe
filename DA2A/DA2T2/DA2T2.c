/*
 * GccApplication1.c
 *
 * Created: 3/6/2019 1:04:41 AM
 * Author : TheAlmighty
 */ 

#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>

int main(void)
{
	DDRB = 0x04;					//PORTB bit 2 is an output
	DDRC = ~(0x04);					//PORTC bit 2 is an input
	PORTC |= (1<<2);				//Enable pull-up 
 	
	while (1)	
    {
		if(!(PINC & 0x04))			//checks if button is pressed
			{
				PORTB = (1<<2);		//turns on pb2
				_delay_ms(1000);	//delay last for 1.25s
				_delay_ms(250);		
				PORTB = (0<<2);		//turn off pb2
			}
		else 
			PORTB = (0<<2);			//keeps pb2 off
    }
}

