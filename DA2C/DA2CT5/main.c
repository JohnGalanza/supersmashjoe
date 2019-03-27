#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>

volatile uint8_t OVFCount;

int main(void)
{
	DDRB |= (1<<2);			//PortB 2 is an output 
	TIMSK0 |= (1<<OCIE0A);	//Set interrupt on compare match 
	TCCR0A |= (1<<WGM01); 	//CTC mode
	TCCR0B |= (1<<CS02);	
	TCCR0B |= (1<<CS00);	//Prescaler set to 1024
	TCNT0 = 0x56;			//Timer starts at 56(86) 
	OCR0A = 0xFF;			//Counts to FF(255) 	
	
	sei();
	while(1){}
}

ISR(TIMER0_COMPA_vect)
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

