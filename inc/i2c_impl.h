#ifndef 		I2C_IMPL_H
#define       	I2C_IMPL_H


#include <stdint.h>


#ifdef __cplusplus
extern "C" 
{
#endif




uint8_t I2C_ReadByte(uint8_t last);

uint8_t I2C_WriteByte(uint8_t ch);

void I2C_Stop(void);

uint8_t I2C_Start(void);

void  I2C_Init(void);


#ifdef __cplusplus
}
#endif


#endif
