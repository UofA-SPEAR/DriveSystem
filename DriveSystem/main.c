/*
 * DriveSystem.c
 *
 * Created: 2018-01-16 11:16:23 AM
 * Author : spear
 */ 
#define F_CPU 8000000UL

#include <avr/io.h>
#include<avr/interrupt.h>
#include "uart.h"

int super_loop_flag = 0;
int isr_count = 0;

// function definitions 
void super_loop_init();
void output_pins_init();
void toggle_LED13();

int main(void)
{
	//Initialize serial coms
	stdout = &uart_output;
	uart_init();

	// set up the super loop isr
	super_loop_init();
	
	output_pins_init();

	while (1) //main loop
	{
		printf("Start main loop\n");		
		while(super_loop_flag == 0)
		{
			//printf("Waiting for loop\n");
		}
		//printf("flag set\n");
		isr_count++;
		isr_count = isr_count % 256;
		//printf("isr_count %i\n", isr_count);
		if(isr_count == 0){
			toggle_LED13();
		}
		// read any incoming data
		
		// calculate motor signals
		
		// read motor feedback
		super_loop_flag = 0;
	}
}

ISR (TIMER0_COMPA_vect)  // timer0 overflow interrupt
{
	//printf("Service Routine hit\n");
	if(super_loop_flag == 1){
		//printf("loop time too short\n");
	}
	super_loop_flag = 1;
	//event to be exicuted every 4ms here
	//printf("ISR count: %i\n", isrCout ++);
}

void super_loop_init(){
	// Set the Timer Mode to CTC
	TCCR0A |= (1 << WGM01);

	// Set the value that you want to count to
	OCR0A = 0xFFFF;

	TIMSK0 |= (1 << OCIE0A);    //Set the ISR COMPA vect

	sei();         //enable interrupts
	TCCR0B |= (1 << CS02) | (1 << CS00);
	// set prescaler to 1024 and start the timer
}

void output_pins_init(){
	// set as output pins
	DDRB |= _BV(DDB3);
	PORTB |= _BV(PORTB3);
}

void toggle_LED13(){
	printf("toggle led");
	PORTB ^= _BV(PORTB3);
}



