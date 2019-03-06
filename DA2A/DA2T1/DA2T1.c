#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
	DDRB|= (1<<2);			//sets portb2 as an output
	
	while(1)
	{
		PORTB |= (1<<2);	//set bit number 2 of portb
		_delay_ms(435);		//sets delay of .435s
		PORTB &= ~(1<<2);	//clears the bit in portb
		_delay_ms(29);		//sets delay of .435s
	}
	return 0;
}