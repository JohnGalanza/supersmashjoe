//MIDTERM 2 
//This program reads values from the APDS9960 and uses the esp module and uart to send it to the website thingspeak
 
#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#define BAUD_RATE 9600
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
//some other headers here
#include "APDS9960_def.h"
#include "i2c_master.h"
#include "uart.h"

void usart_init(void);
void adc_init(void);
void read_adc(void);
void usart_tx_string(char * data);
void usart_send(unsigned char ch);
void usart_print(char* str);

#define APDS9960_write 0x72
#define APDS9960_read 0x73
float clear, red, blue, green;				//Values for the lights

void init_uart(uint16_t baudrate){

	uint16_t UBRR_val = (F_CPU/16)/(baudrate-1);

	UBRR0H = UBRR_val >> 8;
	UBRR0L = UBRR_val;

	UCSR0B |= (1<<TXEN0) | (1<<RXEN0) | (1<<RXCIE0);    // UART TX (Transmit - send)
	UCSR0C |= (1<<USBS0) | (3<<UCSZ00);                    //Modus Asynchron 8N1 (8 Databits, No Parity, 1 Stopbit)
}

void uart_putc(unsigned char c){

	while(!(UCSR0A & (1<<UDRE0)));                        // wait until sending is possible
	UDR0 = c;                                            // output character saved in c
}

void uart_puts(char *s){
	
	while(*s)
	{
		uart_putc(*s);
		s++;
	}
}
void init_APDS9960(void)
{
	_delay_ms(150);
	i2c_start(APDS9960_write);
	i2c_write(APDS9960_ENABLE);
	i2c_write(APDS9960_PON);
	i2c_stop();
	
	i2c_start(APDS9960_write);
	i2c_write(APDS9960_ENABLE);
	i2c_write(APDS9960_AEN);
	i2c_stop();
	
	i2c_start(APDS9960_write);
	i2c_write(APDS9960_ATIME);
	i2c_write(0XB6);
	i2c_stop();
	
	i2c_start(APDS9960_write);
	i2c_write(APDS9960_CONTROL);
	i2c_write(AGAIN_1X);
	i2c_stop();
}

void getreading(void){

	i2c_start(APDS9960_write);
	i2c_write(APDS9960_CDATAL);        // set pointer
	i2c_stop();

	i2c_start(APDS9960_I2C_ADDR);
	/*Store the values */
	clear = (((int)i2c_read_ack()) | (int)i2c_read_ack()<<8);
	red = (((int)i2c_read_ack()<<8) | (int)i2c_read_ack());
	blue = (((int)i2c_read_ack()<<8) | (int)i2c_read_ack());
	green = (((int)i2c_read_ack()<<8) | (int)i2c_read_ack());

	i2c_stop();
}


int main(void)
{
	char buffer[20];
	char float_[10];
	
	init_uart(9600);
	i2c_init();
	usart_init();
	init_APDS9960();
	
	while (1)
	{
		getreading();
			
		//Sets the MODE for wifi settings (AP or station mode)
		char setMODE[] = "AT+CWMODE=1\r\n";
		usart_print(setMODE);
		_delay_ms(1000);
		
		//Connects the ESP to wifi
		//Will change password after this assignment
		char setWIFI[] = "AT+CWJAP=\"Samsung Galaxy S7 edge 3661\",\"biggestboi\"\r\n";
		usart_print(setWIFI);
		_delay_ms(1000);

		//Sets up the proper MUX settings
		char setMUX[] = "AT+CIPMUX=0\r\n";
		usart_print(setMUX);
		_delay_ms(1000);
		
		//Connect to thingspeak website
		char initThingSpeak[] = "AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",80\r\n" ;
		usart_print(initThingSpeak);
		_delay_ms(1000);
		
		//Sets up for the GET function with the amount of characters to be sent
		char sendThingSpeak[] = "AT+CIPSEND=80\r\n";
		usart_print(sendThingSpeak);
		_delay_ms(1000);
		
		//Sends clear value in a to thingspeak
		char getThingSpeak[150];
		snprintf(getThingSpeak, "GET https://api.thingspeak.com/update?api_key=BEN1XMSRAD5TIZIE&field1=%d\r\n", clear);
		usart_print(getThingSpeak);
		_delay_ms(1000);
		
		//Sends red value in a to thingspeak
		char getThingSpeak[150];
		snprintf(getThingSpeak, "GET https://api.thingspeak.com/update?api_key=BEN1XMSRAD5TIZIE&field2=%d\r\n", red);
		usart_print(getThingSpeak);
		_delay_ms(1000);
		
		//Sends green value in a to thingspeak
		char getThingSpeak[150];
		snprintf(getThingSpeak, "GET https://api.thingspeak.com/update?api_key=BEN1XMSRAD5TIZIE&field3=%d\r\n", green);
		usart_print(getThingSpeak);
		_delay_ms(1000);
		
		//Sends blue value in a to thingspeak
		char getThingSpeak[150];
		snprintf(getThingSpeak, "GET https://api.thingspeak.com/update?api_key=BEN1XMSRAD5TIZIE&field4=%d\r\n", blue);
		usart_print(getThingSpeak);
		_delay_ms(1000);
		//End thing speak connection
		char endThingSpeak[] = "AT+CIPCLOSE\r\n";
		usart_print(endThingSpeak);
		
		//Wait about 15 seconds
		_delay_ms(15000);
	}
}


void usart_init(void)
{
	UCSR0B = (1<<TXEN0) | (1<<RXEN0);
	UCSR0C = (1<<UCSZ01) | (1<<UCSZ00);
	UBRR0L = 8;
}


void usart_send(unsigned char ch)
{
	while (!(UCSR0A & (1<<UDRE0)));
	UDR0 = ch;
}

void usart_print(char* str)
{
	int i = 0;
	while(str[i] != 0)
	{
		usart_send(str[i]);
		i++;
	}
}