#ifndef SERVO_H
#define SERVO_H

#include <stdint.h>


#ifdef __cplusplus
extern "C"
{
#endif



void Servo_Init(void);
void Servo_SetPosition(uint8_t position);
void Servo_SetPositionRaw(uint8_t position);
void Servo_SetPositionTimed(uint8_t position, uint16_t ms);



#ifdef __cplusplus
extern "C"
}
#endif


#endif
