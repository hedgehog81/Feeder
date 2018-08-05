#ifndef  I2C_H
#define  I2C_H

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef enum
{
  I2CFlagNone     =   0x00, 
  I2CFlagWrite    =   0x00,
  I2CFlagRead     =   0x01,	
  I2CFlagStart    =  (0x01<<1)
} EnumI2CFlags;

typedef struct
{
	uint8_t slaveaddr;
	uint8_t* pbuffer;
	uint8_t cbbuffer;
	uint8_t flags;

} I2C_MSG;




void  	  I2C_Init(void);

uint8_t   I2C_Command(I2C_MSG* pCmd,uint8_t cntcmd);

#ifdef __cplusplus
extern "C"
}
#endif

#endif
