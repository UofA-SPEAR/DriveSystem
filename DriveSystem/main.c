///*
 //* DriveSystem.c
 //*
 //* Created: 2018-01-16 11:16:23 AM
 //* Author : Brad and Brian Ofrim
 //*/ 
#include <avr/wdt.h> 
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

#define COUNTER_MAX_16 65535
#define THRUST_LEVEL 128

// watchdog time outs
#define WDTO_500MS 5

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
	int left_dir;
	double left_pwm;
	int right_dir;
	double right_pwm;
};

void WDT_interrupt_enable(void)
{
	/* Timed sequence writing WDCE and WDE. See datasheet chp. 13*/
	WDTCSR |= (1<<WDCE)|(1<<WDE);				//Change enabled
	WDTCSR = (1<< WDIE)|(1<<WDP2)|(1<<WDP0);	//Configure interrupt with 1 sec timeout
	
}

void set_motor_controls(struct skid_steer* skid_steer_cmd);

volatile int LOOP_RUN_FLAG = 0;
char INPUT_COMMAND_STRING[BUF_SIZE];
// TODO: refactor current_command
struct skid_steer CURRENT_SKID_COMMAND;

void setup_pins();
void setup_timer();


void reset_motor_instructions(){
	CURRENT_SKID_COMMAND.left_pwm = 0.0;
	CURRENT_SKID_COMMAND.left_dir = 1;
	CURRENT_SKID_COMMAND.right_pwm = 0.0;
	CURRENT_SKID_COMMAND.right_dir = 1;
}

int main (void) 
{
	// disable watchdog during initialization
	//wdt_disable();
	// Initialize
	reset_motor_instructions();
	
	struct drive_motors target_motor_state;
	struct drive_motors current_motor_state;
	
	setup_pins();
	setup_timer();
	uart_init();
	//uart_set_io_streams(stdin, stdout);
    stdin = &uart_input;
    stdout =  &uart_output;
	
	// Enable the watchdog timer
	WDT_interrupt_enable();
	
	// Enable global interrupts
	sei();
	
	// Super loop
	while(1) 
	{
		while(LOOP_RUN_FLAG == 0);
		LOOP_RUN_FLAG = 0;
		
		// CURRENT_SKID_COMMAND is set, do something
		//printf("left: %i, right: %i\n", (int) (10000*CURRENT_SKID_COMMAND.left_pwm), (int) (10000*CURRENT_SKID_COMMAND.right_pwm));
		//printf("dir_left: %i, dir_right: %i\n", CURRENT_SKID_COMMAND.left_dir, CURRENT_SKID_COMMAND.right_dir);
		set_motor_controls(&CURRENT_SKID_COMMAND);
	}
}

void setup_pins()
{

	
	// left motor PWM
	// pin 12 (OC1B) 
	DDRB |= _BV(DDB6);
	PORTB |= _BV(PORTB6);
	// left motor direction
	// pin 10
	DDRB |= _BV(DDB4);
	PORTB |= _BV(PORTB4);
	
	// right motor PWM
	// pin 11 (OC1A)
	DDRB |= _BV(DDB5);
	PORTB |= _BV(PORTB5);
	// right motor direction
	// pin 13
	DDRB |= _BV(DDB7);
	PORTB |= _BV(PORTB7);
}

void setup_timer()
{
	TCCR3B |= _BV(WGM32); // configure timer1 for CTC mode
	TIMSK3 |= _BV(OCIE3A); // enable the CTC interrupt b
	TCCR3B |= _BV(CS31); 
	OCR3A = COUNTER_MAX_16;
	
	TCCR1A |= _BV(COM1A1) | _BV(COM1B1); // set none-inverting mode
	TCCR1A |= _BV(WGM10); // set PWN Phase Corrected
	TCCR1B |= _BV(CS11); // start the timer at 16MHz/1024
	OCR1A = 0; // set the CTC compare value = left motor
	OCR1B = 0; // set the CTC compare value = right motor
}

void command_to_polar(char* in_str, struct polar_coordinate* out_polar_coord)
{
	char* mag_str = strtok(in_str, " ");	
	out_polar_coord->mag = atof(mag_str);
	char* dir_str = strtok(NULL, " ");
	out_polar_coord->dir = atof(dir_str);
}

void command_to_skid_steer(char* in_str, struct skid_steer* out_skid_steer)
{
	char* left_str = strtok(in_str, " ");
	float left_f = atof(left_str);
	out_skid_steer->left_pwm = fabs(left_f);
	out_skid_steer->left_dir = (left_f < 0) ? 0 : 1;
	char* right_str = strtok(NULL, " ");
	float right_f = atof(right_str);
	out_skid_steer->right_pwm = fabs(right_f);
	out_skid_steer->right_dir = (right_f < 0) ? 0 : 1;
}

void set_motor_controls(struct skid_steer* skid_steer_cmd){
	// Set the ouput pin levels
	OCR1A = skid_steer_cmd-> right_pwm * THRUST_LEVEL;
	OCR1B = skid_steer_cmd-> left_pwm * THRUST_LEVEL;
	//printf("Output compare A: %u, B: %u\n\n", OCR1A, OCR1B);
	// Set the direciton pins
	if(skid_steer_cmd->right_dir == 0)
		PORTB &= ~(_BV(PORTB7));
	else
		PORTB |= _BV(PORTB7);
	
	if(skid_steer_cmd->left_dir == 0)
		PORTB &= ~(_BV(PORTB4));
	else
		PORTB |= _BV(PORTB4);
	
}

// control system
//struct drive_motors skid_steer_output(struct skid_steer command)
//{
	//struct drive_motors output_levels;
	//int8_t left_point = command.left;
	//int8_t right_point = command.right;
	//output_levels.front_left.operating_level = left_point;
	//output_levels.front_left.operating_level = left_point;
	//output_levels.front_left.operating_level = left_point;
	//output_levels.front_right.operating_level = right_point;
	//output_levels.front_right.operating_level = right_point;
	//output_levels.front_right.operating_level = right_point;
	//return output_levels;
//}

// Incoming message interrupt
ISR(USART0_RX_vect)
{
	char receivedByte = UDR0; // Fetch incoming byte
	// UDR0 = receivedByte; // Echo directly back
	if(receivedByte != '\n')
	{
		strncat(INPUT_COMMAND_STRING, &receivedByte, sizeof(receivedByte));
	}
	else
	{
		command_to_skid_steer(INPUT_COMMAND_STRING, &CURRENT_SKID_COMMAND);
		memset(INPUT_COMMAND_STRING, 0, sizeof(INPUT_COMMAND_STRING)); // reset INPUT_COMMAND_STRING
		wdt_reset(); // reset the watchdog
	}
}

// Timer interrupt
ISR(TIMER3_COMPA_vect)
{
	LOOP_RUN_FLAG = 1;
}

ISR(WDT_vect) {
	WDTCSR |= (1<<WDIF);
	//printf("wdt\n");
	reset_motor_instructions();
}


