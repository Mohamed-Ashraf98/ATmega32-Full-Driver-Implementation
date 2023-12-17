/*
 =========================================================================================
 Name        : dc_motor.c
 Author      : Mohamed Ashraf El-Sayed
 Version     : 1.0.0
 Copyright   : Your copyright notice
 date        : Wed, Oct 11 2023
 time        :
 Description : DC MOTOR Driver Source file , Ansi-style
 =========================================================================================
*/

/* ----------------------------------------------------------------------------------- */
/* ------------------Includes section------------------- */
#include "dc_motor.h"

#if DC_MOTOR_MODE_SELECT == DC_MOTOR_MODE_WITHOUT_PWM

/* DC Motor is operating without PWM Mode(full speed/constant speed) */

#elif DC_MOTOR_MODE_SELECT == DC_MOTOR_MODE_WITH_TIMER0_PWM

/* DC Motor is operating with PWM Mode of TIMER0 */
#include "timer0.h"

#elif DC_MOTOR_MODE_SELECT == DC_MOTOR_MODE_WITH_TIMER1_PWM

/* DC Motor is operating with PWM Mode of TIMER1 */
#include "timer1.h"

#elif DC_MOTOR_MODE_SELECT == DC_MOTOR_MODE_WITH_TIMER2_PWM

/* DC Motor is operating with PWM Mode of TIMER2 */
//#include "timer2.h"

#endif


/* ----------------------------------------------------------------------------------- */
/* ------------functions definition section------------- */


/**
 * @brief  initialize the DC Motor :
 * 		   		1- setup the direction for the two motor pins through the GPIO driver
 * 		   		2- Stop at the DC-Motor at the beginning through the GPIO driver
 * @param  (dc_motor_obj)  pointer to the DC Motor object passed by reference
 * @return (l_status) 	   status of the performed operation
 *              (E_NOT_OK)  operation failed
 *              (E_OK)      operation success
 */
Std_ReturnType DcMotor_init(dc_motor_config_t *dc_motor_obj)
{
	/* create a local variable to hold the status of the performed operation */
	Std_ReturnType l_status = ZERO_INIT;

	/* check if the address is valid or not */
	if(dc_motor_obj == NULL_PTR)
	{
		/* NULL pointer is passed */

		l_status = E_NOK;		/* operation failed */
	}
	else
	{
		/* the address passed is valid */

		l_status = E_OK;		/* operation success */

		/* initialize the DC Motor */

		/* set the direction of the two pins of the DC Motor to be output */
		l_status = GPIO_setupPinDirection( &( dc_motor_obj->DC_Motor_pin[DC_MOTOR_PIN1] ) );
		l_status = GPIO_setupPinDirection( &( dc_motor_obj->DC_Motor_pin[DC_MOTOR_PIN2] ) );

		/* Stop at the DC-Motor at the beginning through the GPIO driver */
		l_status = DcMotor_Rotate(dc_motor_obj,DC_MOTOR_STOP,ZERO_INIT);
	}

	return l_status;
}


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
Std_ReturnType DcMotor_Rotate(dc_motor_config_t *dc_motor_obj,dc_motor_state_t dc_motor_state,uint8 speed)
{
	/* create a local variable to hold the status of the performed operation */
	Std_ReturnType l_status = ZERO_INIT;

	/* check if the address is valid or not */
	if(dc_motor_obj == NULL_PTR)
	{
		/* NULL pointer is passed */

		l_status = E_NOK;		/* operation failed */
	}
	else
	{
		/* the address passed is valid */

		l_status = E_OK;		/* operation success */

		/* Rotate the DC Motor with a specific duty cycle */

		/* check the required state of the DC Motor */
		switch(dc_motor_state)
		{
			case DC_MOTOR_STOP					:	/* stop the DC Motor by writing logic 0 on the two pins */
													l_status = GPIO_writePin(&( dc_motor_obj->DC_Motor_pin[DC_MOTOR_PIN1] ) , GPIO_LOW );
													l_status = GPIO_writePin(&( dc_motor_obj->DC_Motor_pin[DC_MOTOR_PIN2] ) , GPIO_LOW );

													#if DC_MOTOR_MODE_SELECT == DC_MOTOR_MODE_WITHOUT_PWM
													/* DC Motor is operating without PWM Mode(full speed/constant speed) */

													#elif DC_MOTOR_MODE_SELECT == DC_MOTOR_MODE_WITH_TIMER0_PWM
													/* DC Motor is operating with PWM Mode of TIMER0 */

													/* set the PWM duty cycle with zero */
													TIMER0_PWM_Start(ZERO_INIT);

													#elif DC_MOTOR_MODE_SELECT == DC_MOTOR_MODE_WITH_TIMER1_PWM
													/* DC Motor is operating with PWM Mode of TIMER1 */

													/* set the PWM duty cycle with zero */
													TIMER1_PWM_Start(ZERO_INIT);

													#elif DC_MOTOR_MODE_SELECT == DC_MOTOR_MODE_WITH_TIMER2_PWM
													/* DC Motor is operating with PWM Mode of TIMER2 */

													/* set the PWM duty cycle with zero */
													//TIMER2_PWM_Start(ZERO_INIT);

													#endif

													break;

			case DC_MOTOR_ROTATE_CLOCKWISE		:	/* Rotate the DC Motor in the clock-wise direction with the required duty cycle */
													l_status = GPIO_writePin(&( dc_motor_obj->DC_Motor_pin[DC_MOTOR_PIN1] ) , GPIO_HIGH );
													l_status = GPIO_writePin(&( dc_motor_obj->DC_Motor_pin[DC_MOTOR_PIN2] ) , GPIO_LOW );

													#if DC_MOTOR_MODE_SELECT == DC_MOTOR_MODE_WITHOUT_PWM
													/* DC Motor is operating without PWM Mode(full speed/constant speed) */

													#elif DC_MOTOR_MODE_SELECT == DC_MOTOR_MODE_WITH_TIMER0_PWM
													/* DC Motor is operating with PWM Mode of TIMER0 */

													/* set the PWM Duty Cycle */
													TIMER0_PWM_Start(speed);

													#elif DC_MOTOR_MODE_SELECT == DC_MOTOR_MODE_WITH_TIMER1_PWM
													/* DC Motor is operating with PWM Mode of TIMER1 */

													/* set the PWM Duty Cycle */
													TIMER1_PWM_Start(speed);

													#elif DC_MOTOR_MODE_SELECT == DC_MOTOR_MODE_WITH_TIMER2_PWM
													/* DC Motor is operating with PWM Mode of TIMER2 */

													/* set the PWM Duty Cycle */
													//TIMER2_PWM_Start(speed);

													#endif

													break;

			case DC_MOTOR_ROTATE_ANTI_CLOCKWISE	:	/* Rotate the DC Motor in the anti clock-wise direction with the required duty cycle */
													l_status = GPIO_writePin(&( dc_motor_obj->DC_Motor_pin[DC_MOTOR_PIN1] ) , GPIO_LOW );
													l_status = GPIO_writePin(&( dc_motor_obj->DC_Motor_pin[DC_MOTOR_PIN2] ) , GPIO_HIGH );

													#if DC_MOTOR_MODE_SELECT == DC_MOTOR_MODE_WITHOUT_PWM
													/* DC Motor is operating without PWM Mode(full speed/constant speed) */

													#elif DC_MOTOR_MODE_SELECT == DC_MOTOR_MODE_WITH_TIMER0_PWM
													/* DC Motor is operating with PWM Mode of TIMER0 */

													/* set the PWM Duty Cycle */
													TIMER0_PWM_Start(speed);

													#elif DC_MOTOR_MODE_SELECT == DC_MOTOR_MODE_WITH_TIMER1_PWM
													/* DC Motor is operating with PWM Mode of TIMER1 */

													/* set the PWM Duty Cycle */
													TIMER1_PWM_Start(speed);

													#elif DC_MOTOR_MODE_SELECT == DC_MOTOR_MODE_WITH_TIMER2_PWM
													/* DC Motor is operating with PWM Mode of TIMER2 */

													/* set the PWM Duty Cycle */
													//TIMER2_PWM_Start(speed);

													#endif

													break;

			default		:							/* Nothing */
													break;
		}
	}

	return l_status;
}


/* ----------------------------------------------------------------------------------- */
