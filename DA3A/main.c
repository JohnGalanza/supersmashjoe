#define F_CPU 16000000UL 
#define BAUD 9600
#define BAUD_PRESCALER (((F_CPU/BAUDRATE*16UL)))-1)
#define UBRR_9600 103

#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <avr/io.h>
#include <util/setbaud.h>
#include <stdlib.h>

//FUNCTION DECLARATIONS 
void USART_init(void);
void USART_tx_string(char *data);

volatile uint8_t OVFCount;				//Global variable
volatile int yurr;						//to make delay happen 
char buffer[20];
char num[20];
char dec[20];

int main(void)
{
	TIMSK0 |= (1<<TOIE0);
	TCCR0A = 0x00;						//NORMAL mode
	TCCR0B	|= (1<<CS02)|(1<<CS00);		//Prescaler set to 1024
	TCNT0 = 0x1F;						//This and ovf of 70 produce a 1s delay

	
	 
	USART_init();							//initialize USART	
	_delay_ms(500);
	USART_tx_string("\r\nConnected!\r\n");
	sei();	
	
    while (1) 
    {
	}
}


//Initializes 
void USART_init(void)
{
	UBRR0H = UBRRH_VALUE;
	UBRR0L = UBRRL_VALUE;
	UCSR0C = _BV(UCSZ01)|_BV(UCSZ00);		// 8-Bit data
	UCSR0B = _BV(RXEN0) |_BV(TXEN0);		//Enable RX and TX 
}

//Sends data to serial port 
void USART_tx_string(char *data)
{
	while((*data!= '\0'))
	{
		while(!(UCSR0A & (1<<UDRE0)));
		UDR0 = *data;
		data++;
	}
}


unsigned char USART_Recieve(void)
{
	//wait for data to be received 
	while ( !(UCSR0A & ( 1<<RXC0)));
	//gets and returns data received
	return UDR0;
}

ISR(TIMER0_OVF_vect)
{		
	if(OVFCount<70)
	{
		TCNT0 = 0x1F;
		OVFCount++;
		if(OVFCount == 70)
		{
			TCNT0 = 0x1F;
			OVFCount = 0;
			int r = rand();						//generates a random number
			int x = rand();
			int y = rand();
		
			itoa(r, buffer, 10);				//converts int to char
			itoa(x, num, 10);					//similar to above step
			itoa(y, dec, 10);					//similar to above step
			USART_tx_string("STRING: ");
			USART_tx_string("YEE");				//a string
			USART_tx_string("\n");				//for formatting
		
			USART_tx_string("INTEGER: ");
			USART_tx_string(buffer);			//displays converted int onto terminal
			
			USART_tx_string("\n");				//for formatting
		
		USART_tx_string("FLOAT: ");
		USART_tx_string(num);				//displays a "float" number
		USART_tx_string(".");
		USART_tx_string(dec);
		USART_tx_string("\n");				//for formatting
		}
	}
}
