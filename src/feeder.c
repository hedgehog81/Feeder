#include "feeder.h"

#include "servo.h"
#include "settings.h"

void FeedCycle(void)
{
	uint8_t i;

	Servo_SetPosition(90);
	timer_delay(40);
	
	for (i = 0; i < g_settings.twigcycle; ++i)
	{
		Servo_SetPosition(180);
		timer_delay(40);
		Servo_SetPosition(90);
		timer_delay(40);
	}

	Servo_SetPosition(0);


}
