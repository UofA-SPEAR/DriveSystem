/*
 * DriveSystem.c
 *
 * Created: 2018-01-16 11:16:23 AM
 * Author : spear
 */ 

#include <avr/io.h>
#include<avr/interrupt.h>
#include "uart.h"


int main(void)
{
	//Initialize serial coms
	stdout = &uart_output;
	uart_init();
		

	// Set the Timer Mode to CTC
	TCCR0A |= (1 << WGM01);

	// Set the value that you want to count to
	OCR0A = 0xFFFF;

	TIMSK0 |= (1 << OCIE0A);    //Set the ISR COMPA vect

	sei();         //enable interrupts


	TCCR0B |= (1 << CS02) | (1 << CS00);
	// set prescaler to 1024 and start the timer


	while (1)
	{
		//main loop
	}
}

ISR (TIMER0_COMPA_vect)  // timer0 overflow interrupt
{
	//event to be exicuted every 4ms here
	printf("ISR count: %i\n", isrCout ++);
}

