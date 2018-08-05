#include "i2c.h"
#include <avr/io.h>
#include "i2c_impl.h"


uint8_t   I2C_Command(I2C_MSG* pCmd,	uint8_t cntcmd)
{

	register unsigned char flags;
    register unsigned char cbData;
	register unsigned char* pData;

	do
	{
		
		flags =  pCmd->flags;

		if (flags & I2CFlagStart)
		{
			if (!I2C_Start())
				goto END2;
						
		}	   	
		
	    if (!I2C_WriteByte(pCmd->slaveaddr | (flags & I2CFlagRead)))
			goto END2;
			
		cbData = pCmd->cbbuffer;
		pData = pCmd->pbuffer;

		if ( flags & I2CFlagRead )
		{
			while (cbData--)
			{
				*pData++ = I2C_ReadByte(!cbData);	
			}	

		}
		else
		{
		 	while (cbData--)
			{
				if (!I2C_WriteByte(*pData++))
					goto END1;
				 
			}		
		
		}

		++pCmd;

	} while (--cntcmd);

END1:

	I2C_Stop();

END2:

	return (cntcmd == 0x00);


}

