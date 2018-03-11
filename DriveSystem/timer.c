/*
 * timer.c
 *
 * Created: 2018-03-11 10:17:48 AM
 *  Author: spear
 */ 
#include "timer.h"

void setup_timer()
{
	TCCR3B |= _BV(WGM32); // configure timer3 for CTC mode
	TIMSK3 |= _BV(OCIE3A); // enable the CTC interrupt b
	TCCR3B |= _BV(CS31);
	OCR3A = COUNTER_MAX_16;
	
	TCCR1A |= _BV(COM1A1) | _BV(COM1B1); // set none-inverting mode
	TCCR1A |= _BV(WGM10); // set PWN Phase Corrected
	TCCR1B |= _BV(CS11); // start the timer at 16MHz/1024
	
	OCR1A = 0; // set the CTC compare value = left motor
	OCR1B = 0; // set the CTC compare value = right motor
}

void WDT_interrupt_enable(void)
{
	/* Timed sequence writing WDCE and WDE. See data sheet chapter 13*/
	WDTCSR |= (1<<WDCE)|(1<<WDE);				//Change enabled
	WDTCSR = (1<< WDIE)|(1<<WDP2)|(1<<WDP0);	//Configure interrupt with 1 sec timeout
}