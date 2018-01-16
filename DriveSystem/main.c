///*
 //* DriveSystem.c
 //*
 //* Created: 2018-01-16 11:16:23 AM
 //* Author : spear
 //*/ 

#define F_CPU 16000000UL
#include <avr/io.h>

#include <avr/interrupt.h> // notice that we have swapped libraries, from delay to interrupt
#include <util/delay.h>
#include <stdbool.h>

volatile bool flag = false;

int main (void) {

	//// set as output pins
	DDRB |= _BV(DDB5);
	PORTB |= _BV(PORTB5);

	

	TCCR1B |= (1 << WGM12); // configure timer1 for CTC mode

	TIMSK1 |= (1 << OCIE1A); // enable the CTC interrupt b 

	sei(); // enable global interrupts

	OCR1A   = 19531/10; // set the CTC compare value

	TCCR1B |= ((1 << CS10) | (1 << CS12)); // start the timer at 20MHz/1024

	while(1) { // main loop - do anything you like here!
		while(flag == false);
		PORTB ^= _BV(PORTB5); // toggle the LED
		flag = false;
	}

}



ISR(TIMER1_COMPA_vect) { // this function is called every time the timer reaches the threshold we set

	
	flag = true;

}



