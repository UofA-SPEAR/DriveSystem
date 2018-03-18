///*
 //* DriveSystem.c
 //*
 //* Created: 2018-01-16 11:16:23 AM
 //* Author : Brad and Brian Ofrim
 //*/ 
#include <avr/interrupt.h>
#include "hardware_config.h"
#include "uart.h"
#include "motor_control.h"
#include "timer.h"


#define BUF_SIZE 32


volatile int LOOP_RUN_FLAG = 0;
char INPUT_COMMAND_STRING[BUF_SIZE];
struct skid_steer CURRENT_SKID_COMMAND;

void setup_pins();

int main (void) 
{
	// disable watchdog during initialization
	//wdt_disable();
	// Initialize
	reset_motor_instructions(&CURRENT_SKID_COMMAND);
	
	struct drive_motors target_motor_state;
	struct drive_motors current_motor_state;
	
	setup_timer();
	setup_pins();
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
		set_motor_controls(&CURRENT_SKID_COMMAND);
	}
}

void setup_pins()
{
	// left motor PWM
	LEFT_PWM_IO_REG |= _BV(LEFT_PWM_IO_PIN);
	LEFT_PWM_REG |= _BV(LEFT_PWM_PIN);
	// left motor direction
	LEFT_DIR_IO_REG |= _BV(LEFT_DIR_IO_PIN);
	LEFT_DIR_REG |= _BV(LEFT_DIR_PIN);
	// right motor PWM
	RIGHT_PWM_IO_REG |= _BV(RIGHT_PWM_IO_PIN);
	RIGHT_PWM_REG |= _BV(RIGHT_PWM_PIN);
	// right motor direction
	RIGHT_DIR_IO_REG |= _BV(RIGHT_DIR_IO_PIN);
	RIGHT_DIR_REG |= _BV(RIGHT_DIR_PIN);
}



// Incoming message interrupt
ISR(USART0_RX_vect)
{
	char receivedByte = USART0_RX_BUF; // Fetch incoming byte
	USART0_TX_BUF = receivedByte; // Echo directly back
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

ISR(WDT_vect) 
{
	WDTCSR |= (1<<WDIF);
	//printf("wdt\n");
	reset_motor_instructions(&CURRENT_SKID_COMMAND);
}


