///*
 //* DriveSystem.c
 //*
 //* Created: 2018-01-16 11:16:23 AM
 //* Author : spear
 //*/ 

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#ifndef BAUD
#define BAUD 9600
#endif

#include <avr/io.h>

#include <avr/interrupt.h> // notice that we have swapped libraries, from delay to interrupt
#include <util/delay.h>
#include <stdbool.h>
#include "uart.h"

volatile bool flag = false;
volatile char ReceivedByte;
int main (void) {

	//// set as output pins
	DDRB |= _BV(DDB5);
	PORTB |= _BV(PORTB5);

	//Initialize
	stdout = &uart_output;
	stdin =  &uart_input;
	uart_init();

	TCCR1B |= (1 << WGM12); // configure timer1 for CTC mode

	TIMSK1 |= (1 << OCIE1A); // enable the CTC interrupt b 
	

	sei(); // enable global interrupts

	OCR1A   = 19531/10; // set the CTC compare value

	TCCR1B |= ((1 << CS10) | (1 << CS12)); // start the timer at 20MHz/1024

	while(1) { // main loop - do anything you like here!
		while(flag == false);
		flag = false;
		PORTB ^= _BV(PORTB5); // toggle the LED
	}

}

ISR(USART_RX_vect) {
	ReceivedByte = UDR0; // Fetch the received byte value into the variable "ByteReceived"
	UDR0 = ReceivedByte; // Echo back the received byte back to the computer
	flag = true;
}


//ISR(TIMER1_COMPA_vect) { // this function is called every time the timer reaches the threshold we set
//
	//
	//flag = true;
//
//}



