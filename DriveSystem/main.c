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

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#ifndef BAUD
#define BAUD 9600
#endif

#define BUF_SIZE 32


volatile int LOOP_RUN_FLAG = 0;
char INPUT_COMMAND_STRING[BUF_SIZE];
struct skid_steer CURRENT_SKID_COMMAND;


int main (void) 
{
	fflush(stdout);
	
	// disable watchdog during initialization
	//wdt_disable();
	// Initialize
	reset_motor_instructions(&CURRENT_SKID_COMMAND);
	
	struct drive_motors target_motor_state;
	struct drive_motors current_motor_state;
	
	setup_timer();
	uart_init();
	configure_motor_pins();
	//uart_set_io_streams(stdin, stdout);
    stdin = &uart_input;
    stdout =  &uart_output;
	
	// Enable the watchdog timer
	//WDT_interrupt_enable();
	
	// Enable global interrupts
	sei();
	
	// Super loop
	printf("Start\n");
	 while(1) 
	{
		while(LOOP_RUN_FLAG == 0);
		LOOP_RUN_FLAG = 0;
		
		printf("\nCurrent command: mag:%i, dir:%i\n", (int) (100* CURRENT_SKID_COMMAND.right_pwm), (int) CURRENT_SKID_COMMAND.right_dir);
		
		// CURRENT_SKID_COMMAND is set, do something
		update_motor_controls(&CURRENT_SKID_COMMAND);
		printf("Target: %i, Actual: %i\n", (int)  SIGNED_LEVEL(CURRENT_SKID_COMMAND.left_pwm * THRUST_LEVEL , CURRENT_SKID_COMMAND.left_dir), (int) SIGNED_LEVEL(LEFT_PWM_LEVEL, READ_REG_BIT(LEFT_DIR_REG, LEFT_DIR_PIN) ));
	}
}

/* ------ Interrupts ------ */

// Incoming message interrupt
ISR(USART0_RX_vect)
{
	cli();
	char receivedByte = USART0_RX_BUF; // Fetch incoming byte
	USART0_TX_BUF = receivedByte; // Echo directly back
	if(receivedByte != '\n')
	{
		strncat(INPUT_COMMAND_STRING, &receivedByte, sizeof(receivedByte));
	}
	else
	{
		printf("Incoming command string: %s\n", INPUT_COMMAND_STRING);
		CURRENT_SKID_COMMAND = command_to_skid_steer(INPUT_COMMAND_STRING); // Get the value of the new command
		printf("Current command in isr: mag:%i, dir:%i\n", (int) (100* CURRENT_SKID_COMMAND.right_pwm), (int) CURRENT_SKID_COMMAND.right_dir);
		memset(INPUT_COMMAND_STRING, 0, sizeof(INPUT_COMMAND_STRING)); // reset INPUT_COMMAND_STRING
		//wdt_reset(); // reset the watchdog
	}
	sei();
}

// Timer interrupt
ISR(TIMER3_COMPA_vect)
{
	LOOP_RUN_FLAG = 1;
}

//ISR(WDT_vect) 
//{
	//WDTCSR |= (1<<WDIF);
	//reset_motor_instructions(&CURRENT_SKID_COMMAND);
//}


