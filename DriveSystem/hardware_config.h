/*
 * hardware_config.h
 *
 * Created: 2018-03-04 3:30:24 PM
 *  Author: spear
 */ 


#ifndef HARDWARE_CONFIG_H_
#define HARDWARE_CONFIG_H_

/*
 * Configure registers used for controlling motor direction
 */
#define LEFT_DIR_REG			PORTB
#define RIGHT_DIR_REG			PORTB

/*
 * Configure pins used for controlling motor direction
 */
#define LEFT_DIR_PIN			PORTB4
#define RIGHT_DIR_PIN			PORTB7

/*
 * Configure register used for controlling motor speed
 * These registers are the output compare registers used in PWM signals
 */
#define LEFT_PWM_REG			OCR1B
#define RIGHT_PWM_REG			OCR1A

#endif /* HARDWARE_CONFIG_H_ */