#include "i2c_impl.h"
#include <util/twi.h>


uint8_t I2C_ReadByte(uint8_t last)
{
				
				
	TWCR = _BV(TWINT) | _BV(TWEN);
	
	if (!last)
	{
		TWCR |= _BV(TWEA);
	} 

	
	while (!(TWCR & _BV(TWINT)));

	if ((TWSR & 0xF8) == TW_MR_DATA_ACK)
		return TWDR;
	
	return 0;
}

uint8_t I2C_WriteByte(uint8_t ch)
{
	
	uint8_t status = 0;

	TWDR = ch; 
	TWCR = _BV(TWINT) | _BV(TWEN);
	
	while (!(TWCR & _BV(TWINT)));
			
	status = (TWSR & 0xF8);

	return ((status ==  TW_MT_SLA_ACK) || (status ==  TW_MT_DATA_ACK) || (status ==  TW_MR_SLA_ACK));

}




void I2C_Stop(void)
{
	TWCR = _BV(TWINT) | _BV(TWEN)| _BV(TWSTO);
}


uint8_t I2C_Start(void)
{
	
	TWCR = _BV(TWINT) | _BV(TWSTA) | _BV(TWEN);

	while (!(TWCR & _BV(TWINT)));
		
	return 	((TWSR & 0xF8) == TW_START);
}





void  I2C_Init(void)
{
	TWSR = 0x00;
	TWBR = 72;
}

