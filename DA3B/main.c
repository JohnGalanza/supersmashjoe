#define F_CPU 16000000UL
#define BAUD_RATE 9600
//#define UBRR_9600 103				

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
//#include <util/setbaud.h>
#include <stdio.h>
#include <stdlib.h>					//all the headerss

void read_adc(void);				//Function declarations
void adc_init(void);
void USART_init();
void USART_tx_string(char * data);
void USART_send(unsigned char ch);
void USART_print(char* str);


volatile uint8_t OVFCount; 
volatile unsigned int adc_temp;
char outs[20];


int main(void)
{
    adc_init();							//Initializes ADC
	USART_init();				//Initialize USART
	//USART_tx_string("Connected!\r\n");	//system is working here
	/*TIMSK0 |= (1<<TOIE0);
	TCCR0A = 0x00;						//NORMAL mode
	TCCR0B	|= (1<<CS02)|(1<<CS00);		//Prescaler set to 1024
	TCNT0 = 0x1F;
*/
//	sei();
	
    while (1) 
    {
		ADCSRA|=(1<<ADSC);
		while((ADCSRA & (1<<ADIF))==0);
	
		ADCSRA |= (1<<ADIF);
		
		int a = ADCL;
		a = a | (ADCH<<8);
		a = (a/1024.0)* 5000/10;
		USART_send((a/100)+'0');    
		a = a % 100;
		USART_send((a/10)+'0');
		a = a % 10;
		USART_send((a)+'0'); //converts the number 
		USART_send('\r');
		
		_delay_ms(1000);
	}
	return 0;
}

void adc_init(void)
{
	//set up and enable ADC
	ADMUX = (0<<REFS1)| //Reference selection bits
			(1<<REFS0)|	//AVcc - external cap at AREF
			(0<<ADLAR)|	//ADC Left adjust result 	
			(1<<MUX2)|	//analog channel selection bit
			(0<<MUX1)|	//chooses pc5 pin28
			(1<<MUX0);
			
	ADCSRA =(1<<ADEN)|
			(0<<ADSC)|
			(0<<ADATE)|
			(0<<ADIF)|
			(0<<ADIE)|
			(1<<ADPS2)|	//prescaler 
			(0<<ADPS1)|
			(1<<ADPS0);
}

void read_adc(void)
{
	unsigned char i = 4;
	adc_temp = 0;
	while(i--)
	{
		ADCSRA|=(1<<ADSC);
		while(ADCSRA & (1<<ADSC));
		adc_temp += ADC;
		_delay_ms(50);
	}
	adc_temp = adc_temp/4;			//averages a few samples
}

void USART_init(void)
{
	UBRR0L = F_CPU/16/BAUD_RATE-1;
	UCSR0C = (1<<UCSZ01)|(1<<UCSZ00);	//asynchronous 8 N 1	
	UCSR0B = (1<<TXEN0);		//enable receiver, transmitter & RX interrupt
}

void USART_send(unsigned char ch)
{
	while(!(UCSR0A & (1<<UDRE0)));
	UDR0 = ch;
}

void USART_print(char* str)
{
	int i = 0;
	while (str[i] != 0)
		USART_send(str[i]);
	
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

//Interrupt where temperature will be displayed and 1s delay will happen 
/*ISR(TIMER0_OVF_vect)
{
	if(OVFCount<70)
	{
		TCNT0 = 0x1F;
		OVFCount++;
		if(OVFCount ==70)
		{
			TCNT0 = 0x1F;
			OVFCount = 0;
			read_adc();
			snprintf(outs,sizeof(outs),"%3d\r\n",adc_temp);
			USART_tx_string(outs);
			
		}	
	}
}
*/


