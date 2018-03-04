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
	RIGHT_PWM_REG = skid_steer_cmd-> right_pwm * MAX_THRUST_LEVEL;
	LEFT_PWM_REG = skid_steer_cmd-> left_pwm * MAX_THRUST_LEVEL;
	//printf("Output compare A: %u, B: %u\n\n", OCR1A, OCR1B);
	// Set the direciton pins
	if(skid_steer_cmd->right_dir == 0)
		RIGHT_DIR_REG &= ~(_BV(RIGHT_DIR_PIN));
	else
		RIGHT_DIR_REG |= _BV(RIGHT_DIR_PIN);
	
	if(skid_steer_cmd->left_dir == 0)
		LEFT_DIR_REG &= ~(_BV(LEFT_DIR_PIN));
	else
		LEFT_DIR_REG |= _BV(LEFT_DIR_PIN);
	
}

void reset_motor_instructions(struct skid_steer* command)
{
	command->left_pwm = 0.0;
	command->left_dir = 1;
	command->right_pwm = 0.0;
	command->right_dir = 1;
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
