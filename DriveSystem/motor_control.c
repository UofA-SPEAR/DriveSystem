/*
 * motor_control.c
 *
 * Created: 2018-03-04 10:51:20 AM
 *  Author: spear
 */ 
#include "motor_control.h"
#include <stdio.h>

void configure_motor_pins()
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

void update_motor_controls(struct skid_steer* skid_steer_cmd)
{
	// Read current output levels (-255 to 255)
	int current_right_level = SIGNED_LEVEL(RIGHT_PWM_LEVEL, READ_REG_BIT(RIGHT_DIR_REG, RIGHT_DIR_PIN));
	int current_left_level = SIGNED_LEVEL(LEFT_PWM_LEVEL, READ_REG_BIT(LEFT_DIR_REG, LEFT_DIR_PIN));
	
	// Calculate deltas
	int delta_right_level = SIGNED_LEVEL(skid_steer_cmd->right_pwm * THRUST_LEVEL, skid_steer_cmd->right_dir) - current_right_level;
	int delta_left_level = SIGNED_LEVEL(skid_steer_cmd->left_pwm * THRUST_LEVEL, skid_steer_cmd->left_dir) - current_left_level;
	printf("left curr: %i\n", current_left_level);
	
	printf("left targ: %i\n", (int) SIGNED_LEVEL(skid_steer_cmd->left_pwm * THRUST_LEVEL, skid_steer_cmd->left_dir));
	
	printf("left delta: %i\n", delta_left_level);
	
	
	// Limit acceleration
	delta_right_level = LIMIT_MAG(delta_right_level, MAX_DELTA);
	delta_left_level = LIMIT_MAG(delta_left_level, MAX_DELTA);
	
	printf("left delta lim: %i\n", delta_left_level);
	
	// Set the output pin levels
	int new_right_value = LIMIT_MAG(current_right_level + delta_right_level, MAX_THRUST_LEVEL);
	int new_left_value = LIMIT_MAG(current_left_level + delta_left_level, MAX_THRUST_LEVEL);
	
	RIGHT_PWM_LEVEL = abs(new_right_value);
	LEFT_PWM_LEVEL = abs(new_left_value);
	printf("New Left: %i\n", SIGNED_LEVEL(LEFT_PWM_LEVEL, READ_REG_BIT(LEFT_DIR_REG, LEFT_DIR_PIN)));
	
	
	
	// TODO: Problem is here
	// Set the direction pins
	if(SIGN_DIR(new_right_value) == FORWARD_DIR)
		RIGHT_DIR_REG |= _BV(RIGHT_DIR_PIN); // set
	else
		RIGHT_DIR_REG &= ~(_BV(RIGHT_DIR_PIN)); // clr
	if(SIGN_DIR(new_left_value) == FORWARD_DIR)
		LEFT_DIR_REG |= _BV(LEFT_DIR_PIN); // set
	else
		LEFT_DIR_REG &= ~(_BV(LEFT_DIR_PIN)); // clr
}

void reset_motor_instructions(struct skid_steer* command)
{
	command->left_pwm = 0.0;
	command->left_dir = FORWARD_DIR;
	command->right_pwm = 0.0;
	command->right_dir = FORWARD_DIR;
}

void command_to_polar(char* in_str, struct polar_coordinate* out_polar_coord)
{
	char* mag_str = strtok(in_str, " ");
	out_polar_coord->mag = atof(mag_str);
	char* dir_str = strtok(NULL, " ");
	out_polar_coord->dir = atof(dir_str);
}

struct skid_steer command_to_skid_steer(char* in_str)
{
	struct skid_steer out_skid_steer;
	char* left_str = strtok(in_str, " ");
	float left_f = atof(left_str);
	out_skid_steer.left_pwm = fabs(left_f);
	out_skid_steer.left_dir = (left_f < 0) ? BACKWARD_DIR : FORWARD_DIR;
	char* right_str = strtok(NULL, " ");
	float right_f = atof(right_str);
	out_skid_steer.right_pwm = fabs(right_f);
	out_skid_steer.right_dir = (right_f < 0) ? BACKWARD_DIR : FORWARD_DIR;
	return out_skid_steer;
}

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
