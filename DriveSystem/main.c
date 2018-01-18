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

volatile bool usart_rx_flag = false;

void setup_pins();
void setup_timer();

int main (void) {
	// Initialize
	setup_pins();
	setup_timer();
	uart_init();
	uart_set_io_streams(&stdin, &stdout);
	
	// Enable global interrupts
	sei();

	// Super loop
	while(1) 
	{
		while(usart_rx_flag == false);
		usart_rx_flag = false;
		PORTB ^= _BV(PORTB5); // toggle the LED
	}
}

void setup_pins()
{
	DDRB |= _BV(DDB5);
	PORTB |= _BV(PORTB5);
}

void setup_timer()
{
	TCCR1B |= _BV(WGM12); // configure timer1 for CTC mode
	TIMSK1 |= _BV(OCIE1A); // enable the CTC interrupt b
	TCCR1B |= _BV(CS10) | _BV(CS12); // start the timer at 20MHz/1024
	OCR1A   = 19531/10; // set the CTC compare value
}

ISR(USART_RX_vect) {
	char receivedByte = UDR0; // Fetch the received byte value into the variable "ByteReceived"
	UDR0 = receivedByte; // Echo back the received byte back to the computer
	usart_rx_flag = true;
}


//ISR(TIMER1_COMPA_vect) { // this function is called every time the timer reaches the threshold we set
//
	//
	//flag = true;
//
//}



