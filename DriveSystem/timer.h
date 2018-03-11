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

void setup_timer();
void WDT_interrupt_enable(void);

#endif /* TIMER_H_ */