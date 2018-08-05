#ifndef SETTINGS_H
#define SETTINGS_H

#include <stdint.h>


#ifdef __cplusplus
extern "C"
{
#endif


typedef struct
{
	uint16_t triggertime;
	uint8_t  twigcycle;

} settings_t;

extern settings_t g_settings;



void Settings_Flush(void);
void Settings_Init(void);


#ifdef __cplusplus
}
#endif


#endif
