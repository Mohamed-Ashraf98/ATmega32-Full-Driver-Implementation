/*
 =========================================================================================
 Name        : dc_motor.h
 Author      : Mohamed Ashraf El-Sayed
 Version     : 1.0.0
 Copyright   : Your copyright notice
 date        : Wed, Oct 11 2023
 time        :
 Description : DC MOTOR Driver Header file , Ansi-style
 =========================================================================================
*/

#ifndef _DC_MOTOR_H_
#define _DC_MOTOR_H_
/* ----------------------------------------------------------------------------------- */
/* ------------------Includes section------------------- */
#include "gpio.h"


/* ----------------------------------------------------------------------------------- */
/* --------------Macro declaration section-------------- */


/* --------------------------------- */
/* DC Motor maximum pins */
#define DC_MOTOR_MAX_PINS_SIZE			2

/* DC Motor pins index */
#define DC_MOTOR_PIN1					0
#define DC_MOTOR_PIN2					1

/* --------------------------------- */
/* select the DC Motor Mode */

#define DC_MOTOR_MODE_WITHOUT_PWM		0
#define DC_MOTOR_MODE_WITH_TIMER0_PWM	1
#define DC_MOTOR_MODE_WITH_TIMER1_PWM	2
#define DC_MOTOR_MODE_WITH_TIMER2_PWM	3				/* to do... */

#define DC_MOTOR_MODE_SELECT			DC_MOTOR_MODE_WITHOUT_PWM

/* --------------------------------- */


/* ----------------------------------------------------------------------------------- */
/* -----user_defined data type declaration section------ */

/* DC Motor config structure */
typedef struct{
	/* hold the information of the DC Motor pins */
	gpio_config_t DC_Motor_pin[DC_MOTOR_MAX_PINS_SIZE];
}dc_motor_config_t;


/* DC Motor State */
typedef enum{
	DC_MOTOR_STOP = 0,
	DC_MOTOR_ROTATE_CLOCKWISE,
	DC_MOTOR_ROTATE_ANTI_CLOCKWISE
}dc_motor_state_t;


/* ----------------------------------------------------------------------------------- */
/* ------------functions declaration section------------ */


/**
 * @brief  initialize the DC Motor :
 * 		   		1- setup the direction for the two motor pins through the GPIO driver
 * 		   		2- Stop at the DC-Motor at the beginning through the GPIO driver
 * @param  (dc_motor_obj)  pointer to the DC Motor object passed by reference
 * @return (l_status) 	   status of the performed operation
 *              (E_NOT_OK)  operation failed
 *              (E_OK)      operation success
 */
Std_ReturnType DcMotor_init(dc_motor_config_t *dc_motor_obj);


/**
 * @brief  set the DC Motor state with a specific duty cycle :
 * 		   		1- rotate the DC Motor CW/ or A-CW or stop the motor based on the state input state value
 * 		   		2- Send the required duty cycle to the PWM driver based on the required speed value
 * @param  (dc_motor_obj)    pointer to the DC Motor object passed by reference
 * @param  (dc_motor_state)  state of the DC Motor
 * @param  (speed)    		 speed of the DC Motor
 * @return (l_status) 	   status of the performed operation
 *              (E_NOT_OK)  operation failed
 *              (E_OK)      operation success
 */
Std_ReturnType DcMotor_Rotate(dc_motor_config_t *dc_motor_obj,dc_motor_state_t dc_motor_state,uint8 speed);


/* ----------------------------------------------------------------------------------- */
#endif /* _DC_MOTOR_H_ */
