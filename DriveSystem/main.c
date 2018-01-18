///*
 //* DriveSystem.c
 //*
 //* Created: 2018-01-16 11:16:23 AM
 //* Author : Brad and Brian Ofrim
 //*/ 
 
#include <avr/interrupt.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "uart.h"

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#define F_CLK_1024 F_CPU / 1024UL
#define F_CLK_256 F_CPU / 256UL
#define F_CLK_64 F_CPU / 64UL
#define F_CLK_8 F_CPU / 8UL

#define TIMER_PRESCALE 10

#define BUF_SIZE 32

struct polar_coordinate
{
	double mag;
	double dir;	
};

volatile bool loop_run_flag = false;
char input_command_str[BUF_SIZE];
// TODO: refactor current_command
struct polar_coordinate current_command;


void setup_pins();
void setup_timer();

int main (void) 
{
	
	// Initialize
	current_command.mag = 0.0;
	current_command.dir = 0.0;
	
	setup_pins();
	setup_timer();
	uart_init();
	uart_set_io_streams();
	//
	//// Enable global interrupts
	sei();

	// Super loop
	while(1) 
	{
		while(loop_run_flag == false);
		loop_run_flag = false;
		printf("mag: %i, dir %i\n", (int) current_command.mag, (int) current_command.dir);
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
	TCCR1B |= _BV(CS10) | _BV(CS12); // start the timer at 16MHz/1024
	OCR1A = F_CLK_1024; // set the CTC compare value
}

void command_to_polar(char* in_str, struct polar_coordinate* out_polar_coord)
{
	char* mag_str = strtok(in_str, " ");	
	out_polar_coord->mag = atof(mag_str);
	char* dir_str = strtok(NULL, " ");
	out_polar_coord->dir = atof(dir_str);
	
}

ISR(USART_RX_vect)
{
	char receivedByte = UDR0; // Fetch the received byte value into the variable "ByteReceived"
	PORTB ^= _BV(PORTB5); // toggle the LED
	if(receivedByte != '\n')
	{
		strncat(input_command_str, &receivedByte, sizeof(receivedByte));
	}
	else
	{
		command_to_polar(input_command_str, &current_command);
		memset(input_command_str, 0, sizeof(input_command_str));
	}
}


ISR(TIMER1_COMPA_vect) // this function is called every time the timer reaches the threshold we set
{ 
	loop_run_flag = true;
}



