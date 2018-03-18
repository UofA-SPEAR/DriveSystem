/*
 * timer.h
 *
 * Created: 2018-03-11 10:16:26 AM
 *  Author: spear
 */ 
#include <avr/wdt.h>

#ifndef TIMER_H_
#define TIMER_H_

#define TIMER_PRESCALE 10
#define COUNTER_MAX_16 65535
#define WDTO_500MS 5

#define T1_PRESCALE_0		0x00 | _BV(CS10)
#define T1_PRESCALE_8		0x00 | _BV(CS11)
#define T1_PRESCALE_32		0x00 | _BV(CS11) | _BV(CS10)
#define T1_PRESCALE_64		0x00 | _BV(CS12)
#define T1_PRESCALE_128		0x00 | _BV(CS12) | _BV(CS10)
#define T1_PRESCALE_256		0x00 | _BV(CS12) | _BV(CS11)
#define T1_PRESCALE_1024	0x00 | _BV(CS12) | _BV(CS11) | _BV(CS10)

#define T3_PRESCALE_0		0x00 | _BV(CS30)
#define T3_PRESCALE_8		0x00 | _BV(CS31)
#define T3_PRESCALE_32		0x00 | _BV(CS31) | _BV(CS30)
#define T3_PRESCALE_64		0x00 | _BV(CS32)
#define T3_PRESCALE_128		0x00 | _BV(CS32) | _BV(CS10)
#define T3_PRESCALE_256		0x00 | _BV(CS32) | _BV(CS31)
#define T3_PRESCALE_1024	0x00 | _BV(CS32) | _BV(CS31) | _BV(CS30)

void setup_timer();
void WDT_interrupt_enable(void);

#endif /* TIMER_H_ */