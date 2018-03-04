/*
 * hardware_config.h
 *
 * Created: 2018-03-04 3:30:24 PM
 *  Author: spear
 */ 


#ifndef HARDWARE_CONFIG_H_
#define HARDWARE_CONFIG_H_

/* Motor Controls */

/*
 * Configure pin I/O direction registers and pins
 * Used for setting input or output direction of hardware pins
 */
#define LEFT_PWM_IO_REG				DDRB
#define LEFT_PWM_IO_PIN				DDB6
#define LEFT_DIR_IO_REG				DDRB
#define LEFT_DIR_IO_PIN				DDB4
#define RIGHT_PWM_IO_REG			DDRB
#define RIGHT_PWM_IO_PIN			DDB5
#define RIGHT_DIR_IO_REG			DDRB
#define RIGHT_DIR_IO_PIN			DDB7

/*
 * Configure registers used for controlling motor direction
 */
#define LEFT_DIR_REG				PORTB
#define LEFT_PWM_REG				PORTB
#define RIGHT_DIR_REG				PORTB
#define RIGHT_PWM_REG				PORTB

/*
 * Configure pins used for controlling motor direction
 */
#define LEFT_DIR_PIN			PORTB4
#define RIGHT_DIR_PIN			PORTB7

/*
 * Configure register used for controlling motor speed
 * These registers are the output compare registers used in PWM signals
 */
#define LEFT_PWM_LEVEL			OCR1B
#define LEFT_PWM_PIN			PORTB6
#define RIGHT_PWM_LEVEL			OCR1A
#define RIGHT_PWM_PIN			PORTB5

#endif /* HARDWARE_CONFIG_H_ */