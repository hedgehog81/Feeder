#include "settings.h"
#include <avr/eeprom.h>


#define CONFIG_EEPROM_OFFSET (const void*)0x0000

settings_t g_settings;

void Settings_Flush(void)
{
	eeprom_write_block(&g_settings,CONFIG_EEPROM_OFFSET, sizeof(g_settings));
}

void Settings_Init(void)
{
	eeprom_read_block(&g_settings, CONFIG_EEPROM_OFFSET, sizeof(g_settings));
}
