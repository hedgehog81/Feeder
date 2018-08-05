#include <avr/io.h>
#include <stdlib.h>
#include "servo.h"
#include "timer.h"


//Based on 17ms cycle (~61 hz)

#define POS_TO_VAL(x) (unsigned long)((x) * 512l / 3060l)


static const BASE_MS = (unsigned long)(550l * 256l / 17000l);
static uint8_t  s_position = 0; 


void Servo_Init(void)
{
	TCCR0A 	= _BV(WGM01) | _BV(WGM00) | _BV(COM0A1);
	TCCR0B 	= _BV(CS02) | _BV(CS00);
	OCR0A 	= 0;

	DDRD 	|= _BV(PD6);

	Servo_SetPosition(0);
}

void Servo_SetPosition(uint8_t position)
{
	s_position = position;
	OCR0A = BASE_MS + POS_TO_VAL(position);
}


void Servo_SetPositionRaw(uint8_t position)
{
	OCR0A = position;
}


void Servo_SetPositionTimed(uint8_t position, uint16_t ms)
{
	if (ms == 0)
	{
		Servo_SetPosition(position);
	}
	else
	{	
		uint8_t  steps = abs(position - s_position);
		uint16_t delay = ms /  steps / 10;
		
		int8_t step = position > s_position ? 1 : -1;
	
		
		while (steps--)
		{
			Servo_SetPosition(s_position + step);
			timer_delay(delay);
		}

	}
	
}
