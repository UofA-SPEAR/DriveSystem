///*
 //* DriveSystem.c
 //*
 //* Created: 2018-01-16 11:16:23 AM
 //* Author : Brad and Brian Ofrim
 //*/ 
 
#include <avr/interrupt.h>
#include <stdbool.h>
#include "uart.h"

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

volatile bool flag = false;
volatile char ReceivedByte;

int main (void) {

	//// set as output pins
	DDRB |= _BV(DDB5);
	PORTB |= _BV(PORTB5);

	//Initialize
	uart_init();
	uart_set_io_streams(&stdin, &stdout);

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



