#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>


static volatile unsigned int timer0_tick = 0;

#if defined(__AVR_ATmega328P__)
ISR(TIMER2_COMPA_vect)
#else
SIGNAL(SIG_OUTPUT_COMPARE2A)
#endif

{
	timer0_tick++;
}

void timer_init (void)
{
	
	TCCR2A  = (1 << WGM21);
	TCCR2B  = (1 << CS22) | (1 << CS21) | (1 << CS20);
	TIMSK2  = (1 << OCIE2A);
	TCNT2   = 0;
	OCR2A 	= (79 * 2);//79;
	
}

unsigned int timer_count (void)
{
	unsigned int t;

    cli();               // disable interrupts to read a non-changing value
    t = timer0_tick;
    sei();               // enable interrupts
    return(t);
}

void timer_delay (unsigned int count)
{
	unsigned int start_count;

    start_count = timer_count(); /* get the start count */

    while ((timer_count() - start_count) < count)   /* wait for count "ticks" */
    {
      	set_sleep_mode(SLEEP_MODE_IDLE);
		sleep_mode();
    }
}

void delay_10ms(void)
{
	timer_delay(1);
}

void delay_1sec(void)
{
	timer_delay(100);
}

void delay_2sec(void)
{
	delay_1sec();
    delay_1sec();
}

void delay_10sec(void)
{
	delay_2sec();
    delay_2sec();
    delay_2sec();
    delay_2sec();
    delay_2sec();
}

void delay_0_5sec(void)
{
	timer_delay(50);
}

void delay_0_1sec(void)
{
	 timer_delay(10);
}
