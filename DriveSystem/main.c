///*
 //* DriveSystem.c
 //*
 //* Created: 2018-01-16 11:16:23 AM
 //* Author : Brad and Brian Ofrim
 //*/ 
 
#include <avr/interrupt.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
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

// control system
struct drive_motor
{
	int8_t operating_level;
};

// control system
struct drive_motors
{
	struct drive_motor front_left;
	struct drive_motor center_left;
	struct drive_motor rear_left;
	struct drive_motor front_right;
	struct drive_motor center_right;
	struct drive_motor rear_right;
};

struct polar_coordinate
{
	double mag;
	double dir;	
};

struct skid_steer
{
	double left;
	double right;
};

volatile int LOOP_RUN_FLAG = 0;
char INPUT_COMMAND_STRING[BUF_SIZE];
// TODO: refactor current_command
struct polar_coordinate CURRENT_COMMAND;

void setup_pins();
void setup_timer();

int main (void) 
{
	
	// Initialize
	CURRENT_COMMAND.mag = 0.0;
	CURRENT_COMMAND.dir = 0.0;
	struct drive_motors target_motor_state;
	struct drive_motors current_motor_state;
	
	
	
	setup_pins();
	setup_timer();
	uart_init();
	//uart_set_io_streams(stdin, stdout);
    stdin = &uart_input;
    stdout =  &uart_output;
	
	// Enable global interrupts
	sei();

	// Super loop
	while(1) 
	{
		while(LOOP_RUN_FLAG == 0);
		LOOP_RUN_FLAG = 0;
		//printf("mag: %i, dir %i\n", (int) CURRENT_COMMAND.mag, (int) CURRENT_COMMAND.dir);
	}
}

void setup_pins()
{
	DDRB |= _BV(DDB7);
	PORTB |= _BV(PORTB7);
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

// control system
struct drive_motors skid_steer_output(struct polar_coordinate command)
{
	struct drive_motors output_levels;
	int8_t left_point = command.mag * (sin(command.dir)+cos(command.dir));
	int8_t right_point = command.mag * (sin(command.dir)-cos(command.dir));
	output_levels.front_left.operating_level = left_point;
	output_levels.front_left.operating_level = left_point;
	output_levels.front_left.operating_level = left_point;
	output_levels.front_right.operating_level = right_point;
	output_levels.front_right.operating_level = right_point;
	output_levels.front_right.operating_level = right_point;
	return output_levels;
}

// Incoming message interrupt
ISR(USART0_RX_vect)
{
	char receivedByte = UDR0; // Fetch incoming byte
	UDR0 = receivedByte;
	PORTB ^= _BV(PORTB7);
	if(receivedByte != '\n')
	{
		strncat(INPUT_COMMAND_STRING, &receivedByte, sizeof(receivedByte));
	}
	else
	{
		command_to_polar(INPUT_COMMAND_STRING, &CURRENT_COMMAND);
		memset(INPUT_COMMAND_STRING, 0, sizeof(INPUT_COMMAND_STRING)); // reset INPUT_COMMAND_STRING
	}
}

// Timer interrupt
ISR(TIMER1_COMPA_vect)
{
	LOOP_RUN_FLAG = 1;
}



