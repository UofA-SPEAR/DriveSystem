/*
 * motor_control.c
 *
 * Created: 2018-03-04 10:51:20 AM
 *  Author: spear
 */ 
#include "motor_control.h"

void set_motor_controls(struct skid_steer* skid_steer_cmd)
{
	// Set the ouput pin levels
	OCR1A = skid_steer_cmd-> right_pwm * MAX_THRUST_LEVEL;
	OCR1B = skid_steer_cmd-> left_pwm * MAX_THRUST_LEVEL;
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

void reset_motor_instructions(struct skid_steer* command)
{
	command->left_pwm = 0.0;
	command->left_dir = 1;
	command->right_pwm = 0.0;
	command->right_dir = 1;
}
