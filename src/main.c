#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include "i2c.h"
#include "rtc.h"
#include "servo.h"
#include "uart.h"
#include "timer.h"
#include "shell.h"
#include "settings.h"


uint8_t waitChar(uint16_t timeout)
{
	int16_t tick = timer_count();	
	uint8_t ch = 0;

	while (((timer_count() - tick) < timeout) && !UART_Rx(&ch));
	
	return ch;
}


static uint32_t TimeToShortDate(const TIME* pTime)
{
	return (((uint32_t)pTime->year << 16) | ((uint32_t)pTime->month << 8) | ((uint32_t)pTime->day));
}

static uint16_t TimeToShortTime(const TIME* pTime)
{
	return (((uint16_t)pTime->hour << 8) | (uint16_t)pTime->min);
}





int main(void)
{
	uint32_t tempDate = 0;
	uint32_t lastTriggerDate = 0;
	static TIME currentTime ; 
	

	cli();


	I2C_Init();
	Servo_Init();
	UART_Init();
	timer_init();
	Settings_Init();
	Shell_Init();
	
	
	DDRB |= _BV(PB5);
	PORTB &= ~_BV(PB5);

	sei();	
	

	printf_P(PSTR("Betta Feeder %s %s\r\n"),__DATE__,__TIME__);
	
	printf_P(PSTR("Press any key to enter shell\r\n"));
	
	if (waitChar(200) != 0)
	{	
		Shell_Run();
	}


	while (1)
	{
	
		
		PORTB ^= _BV(PB5);
		
		RTC_Read(&currentTime);

		tempDate = TimeToShortDate(&currentTime);		

		//printf_P(PSTR("tempDate %08Xl lastTriggerDate %08Xl\r\n"),tempDate, lastTriggerDate);

		if (tempDate > lastTriggerDate && TimeToShortTime(&currentTime) >= g_settings.triggertime)
		{

			FeedCycle();			

			lastTriggerDate = tempDate;
		}
					

		timer_delay(100);		
		
	}


	return 0;
}
