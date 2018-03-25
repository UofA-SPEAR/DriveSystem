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
#include "hardware_config.h"

#define MAX_THRUST_LEVEL 255
#define THRUST_LEVEL 128
#define TRUST_FRAC THRUST_LEVEL / MAX_THRUST_LEVEL
#define MAX_DELTA 10
#define FORWARD_DIR 1
#define BACKWARD_DIR 0

#define SIGN_DIR(DIR) (DIR >= 0 ? FORWARD_DIR : BACKWARD_DIR)	
#define DIR_SIGN(DIR) (DIR == FORWARD_DIR ? 1 : -1)										// DIR=FORWARD_DIR -> +1, DIR=BACKWARD_DIR -> -1 
#define SIGN_OF(VAL) (VAL >= 0 ? 1 : -1)													// Positive or negative
#define SIGNED_LEVEL(MAG, DIR_BIT) (MAG * DIR_SIGN(DIR_BIT))							// Magnitued * signed direction
#define LIMIT_MAG(VAL, LIMIT) ((abs(VAL) < LIMIT ? abs(VAL) : LIMIT) * SIGN_OF(VAL))	// Cap the magnitude of VAL at LIMIT


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

void configure_motor_pins();
void update_motor_controls(struct skid_steer* skid_steer_cmd);
void reset_motor_instructions(struct skid_steer* command);
void command_to_polar(char* in_str, struct polar_coordinate* out_polar_coord);
struct skid_steer command_to_skid_steer(char* in_str);


#endif /* MOTOR_CONTROL_H_ */