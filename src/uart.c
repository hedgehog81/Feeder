#include <avr/io.h>


//#define FOSC 8000000UL
#define BAUD 19200
#define MYUBRR F_CPU/16/BAUD-1



void UART_Init(void)
{
	
	//Set baud rate 
	UBRR0H = (unsigned char)((unsigned int)(MYUBRR)>>8);
	UBRR0L = (unsigned char)(unsigned int)(MYUBRR);
	//Enable receiver and transmitter
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	//Set frame format: 8data, 1stop bit
	UCSR0C = /*(1<<USBS0)|*/(3<<UCSZ00);

}


int UART_Rx(char* ch)
{
	
	if ( (UCSR0A & (1<<RXC0)) )
	{
		*ch = UDR0;
		return 1;
	}

	return 0;
}


void UART_Tx(char ch)
{
	/* Wait for empty transmit buffer */
	while ( !( UCSR0A & (1<<UDRE0)) );
	/* Put data into buffer, sends the data */
	UDR0 = ch;	
	
}

