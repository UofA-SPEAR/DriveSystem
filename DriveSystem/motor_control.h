/*
 * motor_control.h
 *
 * Created: 2018-03-04 10:47:16 AM
 *  Author: spear
 */ 


#ifndef MOTOR_CONTROL_H_
#define MOTOR_CONTROL_H_
#include <avr/io.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

#define MAX_THRUST_LEVEL 128

struct drive_motor
{
	int8_t operating_level;
};

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

void set_motor_controls(struct skid_steer* skid_steer_cmd);
void reset_motor_instructions(struct skid_steer* command);
void command_to_polar(char* in_str, struct polar_coordinate* out_polar_coord);
void command_to_skid_steer(char* in_str, struct skid_steer* out_skid_steer);
//struct drive_motors skid_steer_output(struct skid_steer command)



#endif /* MOTOR_CONTROL_H_ */