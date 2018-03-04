/*
 * motor_control.h
 *
 * Created: 2018-03-04 10:47:16 AM
 *  Author: spear
 */ 


#ifndef MOTOR_CONTROL_H_
#define MOTOR_CONTROL_H_

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



#endif /* MOTOR_CONTROL_H_ */