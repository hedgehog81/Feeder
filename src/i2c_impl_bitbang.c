#include "hardware.h"
#include "i2c_impl.h"
#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <stdio.h>


#define DELAY() _delay_us(5)




static  void SetPin(uint8_t pin)
{
	
	DDR 	&= ~_BV(pin);
}

static  void ClearPin(uint8_t pin)
{
	DDR 	|= _BV(pin);
}

static  uint8_t ReadPin(uint8_t pin)
{	
	//uint8_t ret;
	
	//ret =  !!(PIN & _BV(pin));
	

	return ((PIN & _BV(pin)) != 0);
}


/*
void I2CPinTest(void)
{
	
	SetPin(SDA);
	SetPin(SCL);

	_delay_ms(1000);

	ClearPin(SDA);
	ClearPin(SCL);

	_delay_ms(1000);
}
*/



uint8_t I2C_ReadByte(uint8_t last)
{

	register unsigned char reg = 0;
	register char i = 8;


/*	
	SDA = 1;

	while ( i-- )
	{		
		SCL = 1;
		DELAY();
			
		reg <<= 1;
		reg &= ~0x01;
		reg |= SDA ;
			
		SCL = 0;
		DELAY();
	}
		
	SDA = last;
	SCL = 1;
	DELAY();
	SCL = 0;
	DELAY();
	
*/

	SetPin(SDA);

	while ( i-- )
	{		
		SetPin(SCL);
		DELAY();
			
		reg <<= 1;
		//reg &= ~0x01;
		reg |= ReadPin(SDA);
			
		ClearPin(SCL);
		DELAY();
	}
	
		
	if (last)
	{
		SetPin(SDA);
	}
	else
	{
		ClearPin(SDA);
	}
	
	
	SetPin(SCL);
	DELAY();
	ClearPin(SCL);
	DELAY();
				
	return reg;


}

uint8_t I2C_WriteByte(uint8_t ch)
{
	register char i = 8;

/*
	while ( i-- )
	{
		SDA = (ch & 0x80) ? 1 : 0;
		ch <<= 1;

		SCL = 1;
		DELAY();	
		SCL = 0;
		DELAY();
	}

	SDA = 1;
	SCL = 1;
	DELAY();	
	i = SDA;			
	SCL = 0;
	DELAY();
*/
	

	while ( i-- )
	{
		//SDA = (ch & 0x80) ? 1 : 0;
		
		if (ch & 0x80)
		{
			SetPin(SDA);
		}
		else
		{
			ClearPin(SDA);
		}

		ch <<= 1;

		SetPin(SCL);
		DELAY();	
		ClearPin(SCL);
		DELAY();
	}

	SetPin(SDA);
	SetPin(SCL);
	DELAY();	
	
	i = ReadPin(SDA);			
	
	ClearPin(SCL);
	DELAY();
	
				
	return (!i);		


}

void I2C_Stop(void)
{
/*	
	SDA = 0;
	SCL = 1;
	DELAY();
	SDA = 1;
	DELAY();
*/

	ClearPin(SDA);
	SetPin(SCL);
	DELAY();
	SetPin(SDA);
	DELAY();


}

uint8_t I2C_Start(void)
{
	
	/*
	SDA = 1;
	SCL = 1;
	DELAY();
	SDA = 0;
	DELAY();
	SCL = 0;	


	return 	!SCL;
	*/

	SetPin(SDA);
	SetPin(SCL);
	DELAY();
	ClearPin(SDA);
	DELAY();
	ClearPin(SCL);	
	

	return 	1;//!ReadPin(SCL);


}

void  I2C_Init(void)
{
	
	

	PORT &= ~(_BV(SDA) | _BV(SCL));
	DDR  &= ~(_BV(SDA) | _BV(SCL));	


	SetPin(SDA);
	SetPin(SCL);
}

