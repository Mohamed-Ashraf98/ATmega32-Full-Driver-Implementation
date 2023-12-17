/*
 =========================================================================================
 Name        : timer0.c
 Author      : Mohamed Ashraf El-Sayed
 Version     : 1.0.0
 Copyright   : Your copyright notice
 date        : Wed, Oct 4 2023
 time        :
 Description : TIMER0 Driver Source file , Ansi-style
 =========================================================================================
*/

/* ----------------------------------------------------------------------------------- */
/* ------------------Includes section------------------- */
#include <avr/interrupt.h> 			/* For TIMER0 ISR */

#include "timer0.h"

#if (TIMER0_MODE_SELECT == TIMER0_MODE_PWM) || (TIMER0_MODE_SELECT == TIMER0_MODE_CLEAR_TIMER_ON_COMPARE_MATCH)
#include "gpio.h"					/* For TIMER0 OC0 pin setup */
#endif


/* ----------------------------------------------------------------------------------- */
/* -------------------Global section-------------------- */


/* create a pointer to function to hold the address of the call back function */

#if TIMER0_MODE_SELECT == TIMER0_MODE_OVER_FLOW
/* TIMER0 is configured in Normal (OverFlow) Mode */

/* TIMER0 Over Flow Interrupt Call Back */
static void (* TIMER0_OVF_InterruptHandler)(void) = NULL_PTR;

#elif TIMER0_MODE_SELECT == TIMER0_MODE_CLEAR_TIMER_ON_COMPARE_MATCH
/* TIMER0 is configured in CTC (Clear Timer on Compare Match) Mode */

/* TIMER0 Clear Timer on Compare Match Interrupt Call Back */
static void (* TIMER0_CTC_InterruptHandler)(void) = NULL_PTR;

#endif


/* ----------------------------------------------------------------------------------- */
/* ------------functions definition section------------- */


#if TIMER0_MODE_SELECT == TIMER0_MODE_OVER_FLOW
/* TIMER0 is configured in Normal (OverFlow) Mode */


/**
 * @brief  initialize TIMER0 :
 *			1- Select Mode : Over Flow (Normal) Mode
 *			2- Select Clock Source
 *				- No clock source (Timer/Counter stopped)
 *				- clkI/O/(No pre-scaler)
 *				- clkI/O/8 (From pre-scaler)
 *				- clkI/O/64 (From pre-scaler)
 *				- clkI/O/256 (From pre-scaler)
 *				- clkI/O/1024 (From pre-scaler)
 *			3- Set the value of the Timer/Counter Register to be Zero
 *			4- Set TIMER0 Over Flow Call Back if Over Flow Interrupt enabled
 * @param  (timer0_ovf_obj) pointer to the TIMER0 Over Flow Mode object passed by reference
 * @return (l_status) 		status of the performed operation
 *              (E_NOT_OK)  operation failed
 *              (E_OK)      operation success
 */
Std_ReturnType TIMER0_OVF_init(timer0_overflow_config_t *timer0_ovf_obj)
{
	/* create a local variable to hold the status of the performed operation */
	Std_ReturnType l_status = ZERO_INIT;

	/* check if the address is valid or not */
	if(timer0_ovf_obj == NULL_PTR)
	{
		/* NULL pointer is passed */

		l_status = E_NOK;		/* operation failed */
	}
	else
	{
		/* the address passed is valid */

		l_status = E_OK;		/* operation success */

		/* TIMER0 Over Flow (Normal) Mode Init */

		/* --------------------------------- */
		/* Non PWM Mode (OverFlow or Output Compare Match) */
		_TCCR0._FOC0 = TIMER0_FORCE_OUTPUT_COMPARE_ENABLE;

		/* Set TIMER0 Mode to be Over Flow (Normal) Mode */
		_TCCR0._WGM00 = RESET;
		_TCCR0._WGM01 = RESET;

		/* Normal port operation, OC0 disconnected */
		_TCCR0._COM0x = RESET;
		/* --------------------------------- */

		/* --------------------------------- */
		/* Set the value of the Timer/Counter Register to be Zero */
		_TCNT0.Byte = ZERO_INIT;
		/* --------------------------------- */

		/* --------------------------------- */
		/* Enable/Disable Timer/Counter0 Overflow Interrupt */
		_TIMSK._TOIE0 = timer0_ovf_obj->overflow_interrupt_enable;

		/* Set TIMER0 Over Flow Call Back if Over Flow Interrupt enabled */
		if(timer0_ovf_obj->overflow_interrupt_enable == TIMER0_OVERFLOW_INTERRUPT_DISABLE)
		{
			/* TIMER0 Over Flow Interrupt is Disabled */
			TIMER0_OVF_InterruptHandler = NULL_PTR;
		}
		else if(timer0_ovf_obj->overflow_interrupt_enable == TIMER0_OVERFLOW_INTERRUPT_ENABLE)
		{
			/* TIMER0 Over Flow Interrupt is Enabled */
			TIMER0_OVF_InterruptHandler = timer0_ovf_obj->TIMER0_OVF_DefaultHandler;
		}
		else{ /* Nothing */ }
		/* --------------------------------- */

		/* --------------------------------- */
		/* select TIMER0 Clock Source */
		_TCCR0._CS0x = timer0_ovf_obj->clock_source;
		/* --------------------------------- */
	}

	return l_status;
}


/**
 * @brief  Set the value of TIMER0 Timer/Counter0 Register
 * @param  (timer0_val) holds the value to be set in the TIMER0 Timer/Counter0 Register
 */
void TIMER0_setTimerValue(uint8 timer0_val)
{
	_TCNT0.Byte = timer0_val;
}


/**
 * @brief  Set TIMER0 Over Flow Call Back
 * @param  (TIMER0_OVF_Handler) holds the address of the call back function
 */
void TIMER0_OVF_setCallBack( void (* TIMER0_OVF_Handler)(void) )
{
	TIMER0_OVF_InterruptHandler = TIMER0_OVF_Handler;
}


/**
 * @brief  Disable Timer0 Over Flow
 */
void TIMER0_OVF_deInit(void)
{
	/* Reset the Timer/Counter Control Register including Clock Select */
	_TCCR0.Byte = ZERO_INIT;

	/* Set the value of the Timer/Counter Register to be Zero */
	_TCNT0.Byte = ZERO_INIT;

	/* Disable Timer/Counter0, Overflow Interrupt */
	_TIMSK._TOIE0 = TIMER0_OVERFLOW_INTERRUPT_DISABLE;

	/* Reset TIMER0 Over Flow Call Back as Over Flow Interrupt is disabled */
	TIMER0_OVF_InterruptHandler = NULL_PTR;
}


#elif TIMER0_MODE_SELECT == TIMER0_MODE_CLEAR_TIMER_ON_COMPARE_MATCH
/* TIMER0 is configured in CTC (Clear Timer on Compare Match) Mode */


/**
 * @brief  initialize TIMER0 :
 *			1- Select Mode : Clear Timer on Compare Match (CTC) Mode
 *			2- Select Clock Source
 *				- No clock source (Timer/Counter stopped)
 *				- clkI/O/(No pre-scaler)
 *				- clkI/O/8 (From pre-scaler)
 *				- clkI/O/64 (From pre-scaler)
 *				- clkI/O/256 (From pre-scaler)
 *				- clkI/O/1024 (From pre-scaler)
 *			3- Set the value of the Timer/Counter Register to be Zero
 *			4- Set the value of the Output Compare Register to be Zero
 *			5- Set TIMER0 Output Compare Match Call Back if Output Compare Match Interrupt enabled
 * @param  (timer0_ctc_obj) pointer to the TIMER0 CTC Mode object passed by reference
 * @return (l_status) 		status of the performed operation
 *              (E_NOT_OK)  operation failed
 *              (E_OK)      operation success
 */
Std_ReturnType TIMER0_CTC_init(timer0_ctc_config_t *timer0_ctc_obj)
{
	/* create a local variable to hold the status of the performed operation */
	Std_ReturnType l_status = ZERO_INIT;

	/* check if the address is valid or not */
	if(timer0_ctc_obj == NULL_PTR)
	{
		/* NULL pointer is passed */

		l_status = E_NOK;		/* operation failed */
	}
	else
	{
		/* the address passed is valid */

		l_status = E_OK;		/* operation success */

		/* TIMER0 Clear Timer on Compare Match (CTC) Mode Init */

		/* --------------------------------- */
		/* Non PWM Mode (OverFlow or Output Compare Match) */
		_TCCR0._FOC0 = TIMER0_FORCE_OUTPUT_COMPARE_ENABLE;

		/* Set TIMER0 Mode to be Output Compare Match (CTC) Mode */
		_TCCR0._WGM00 = RESET;
		_TCCR0._WGM01 = SET;

		/* control the Output Compare pin (OC0) behavior */
		_TCCR0._COM0x = timer0_ctc_obj->OC_pin_mode;
		/* --------------------------------- */

		/* --------------------------------- */
		/* Set the value of the Timer/Counter Register to be Zero */
		_TCNT0.Byte = ZERO_INIT;

		/* Set the value of the Output Compare Register to be Zero */
		_OCR0.Byte = ZERO_INIT;
		/* --------------------------------- */

		/* --------------------------------- */
		/* Setup the direction for OC0 as output pin through the GPIO driver if enabled */
		if(timer0_ctc_obj->OC_pin_mode != TIMER0_CTC_NORMAL_MODE)
		{
			/* configure OC0 pin to be output for the wave generation */
			gpio_config_t ctc_pin;

			ctc_pin.port = PWM_CTC_OC0_PORT_INDEX;
			ctc_pin.pin = PWM_CTC_OC0_PIN_INDEX;
			ctc_pin.mode = GPIO_MODE_OUTPUT;

			l_status |= GPIO_setupPinDirection(&ctc_pin);
		}
		else{ /* Nothing */ }
		/* --------------------------------- */

		/* --------------------------------- */
		/* Enable/Disable Timer/Counter0 Output Compare Match Interrupt */
		_TIMSK._OCIE0 = timer0_ctc_obj->ctc_interrupt_enable;

		/* Set TIMER0 Output Compare Match Call Back if Output Compare Match Interrupt enabled */
		if(timer0_ctc_obj->ctc_interrupt_enable == TIMER0_OUTPUT_COMPARE_MATCH_INTERRUPT_DISABLE)
		{
			/* TIMER0 Output Compare Match Interrupt is Disabled */
			TIMER0_CTC_InterruptHandler = NULL_PTR;
		}
		else if(timer0_ctc_obj->ctc_interrupt_enable == TIMER0_OUTPUT_COMPARE_MATCH_INTERRUPT_ENABLE)
		{
			/* TIMER0 Output Compare Match Interrupt is Enabled */
			TIMER0_CTC_InterruptHandler = timer0_ctc_obj->TIMER0_CTC_DefaultHandler;
		}
		else{ /* Nothing */ }
		/* --------------------------------- */

		/* --------------------------------- */
		/* select TIMER0 Clock Source */
		TIMER0_ENABLE(timer0_ctc_obj->clock_source);
		/* --------------------------------- */
	}

	return l_status;
}


/**
 * @brief  Set the value of TIMER0 Timer/Counter0 Register
 * @param  (timer0_val) holds the value to be set in the TIMER0 Timer/Counter0 Register
 */
void TIMER0_TCNT0_setValue(uint8 timer0_val)
{
	_TCNT0.Byte = timer0_val;
}


/**
 * @brief  Set the value of TIMER0 Output Compare Register
 * @param  (timer0_val) holds the value to be set in the TIMER0 Output Compare Register
 */
void TIMER0_OCR0_setValue(uint8 timer0_val)
{
	_OCR0.Byte = timer0_val;
}


/**
 * @brief  Set TIMER0 Output Compare Match Call Back
 * @param  (TIMER0_CTC_Handler) holds the address of the call back function
 */
void TIMER0_CTC_setCallBack( void (* TIMER0_CTC_Handler)(void) )
{
	TIMER0_CTC_InterruptHandler = TIMER0_CTC_Handler;
}


/**
 * @brief  Disable Timer0 Output Compare Match
 */
void TIMER0_CTC_deInit(void)
{
	/* Reset the Timer/Counter Control Register including Clock Select */
	_TCCR0.Byte = ZERO_INIT;

	/* Set the value of the Timer/Counter Register to be Zero */
	_TCNT0.Byte = ZERO_INIT;

	/* Set the value of the Output Compare Register to be Zero */
	_OCR0.Byte = ZERO_INIT;

	/* Disable Timer/Counter0, Output Compare Match Interrupt */
	_TIMSK._OCIE0 = TIMER0_OUTPUT_COMPARE_MATCH_INTERRUPT_DISABLE;

	/* Reset TIMER0 Output Compare Match Call Back as Output Compare Match Interrupt is disabled */
	TIMER0_CTC_InterruptHandler = NULL_PTR;
}


#elif TIMER0_MODE_SELECT == TIMER0_MODE_PWM
/* TIMER0 is configured in PWM (Fast or Phase Correct) Mode */


/**
 * @brief  initialize TIMER0 :
 *			1- mode
 *				- Fast PWM Mode
 *					. Non Inverting
 *					. Inverting
 *				- Phase Correct PWM Mode
 *					. Clear OC0 on compare match when up-counting. Set OC0 on compare match when down-counting
 *					. Set OC0 on compare match when up-counting. Clear OC0 on compare match when down-counting
 *			2- Clock Source
 *				- No clock source (Timer/Counter stopped)
 *				- clkI/O/(No pre-scaler)
 *				- clkI/O/8 (From pre-scaler)
 *				- clkI/O/64 (From pre-scaler)
 *				- clkI/O/256 (From pre-scaler)
 *				- clkI/O/1024 (From pre-scaler)
 *			3- Set the value of the Timer/Counter Register to be Zero
 *			4- Set the value of the Output Compare Register to be Zero
 *			5- Setup the direction for OC0 as output pin through the GPIO driver
 * @param  (pwm_obj) 	pointer to the TIMER0 PWM Mode object passed by reference
 * @return (l_status) 	status of the performed operation
 *              (E_NOT_OK)  operation failed
 *              (E_OK)      operation success
 */
Std_ReturnType TIMER0_PWM_init(timer0_pwm_config_t *pwm_obj)
{
	/* create a local variable to hold the status of the performed operation */
	Std_ReturnType l_status = ZERO_INIT;

	/* check if the address is valid or not */
	if(pwm_obj == NULL_PTR)
	{
		/* NULL pointer is passed */

		l_status = E_NOK;		/* operation failed */
	}
	else
	{
		/* the address passed is valid */

		/* TIMER0 PWM Mode Init */

		/* --------------------------------- */
		/* PWM Mode (Fast or Phase Correct) */
		_TCCR0._FOC0 = TIMER0_FORCE_OUTPUT_COMPARE_DISABLE;

		/* select the timer mode (Fast PWM Mode or Phase Correct PWM Mode) */
		if(pwm_obj->mode == TIMER0_PHASE_CORRECT_PWM_MODE)
		{
			/* TIMER0 is configured to operate in Phase Correct PWM Mode */
			_TCCR0._WGM00 = SET;
			_TCCR0._WGM01 = RESET;
		}
		else if(pwm_obj->mode == TIMER0_FAST_PWM_MODE)
		{
			/* TIMER0 is configured to operate in Fast PWM Mode */
			_TCCR0._WGM00 = SET;
			_TCCR0._WGM01 = SET;
		}
		else{ /* Nothing */ }
		/* --------------------------------- */

		/* --------------------------------- */
		/* control the Output Compare pin (OC0) behavior */
		_TCCR0._COM0x = pwm_obj->OC_pin_mode;
		/* --------------------------------- */

		/* --------------------------------- */
		/* Set the value of the Timer/Counter Register to be Zero */
		_TCNT0.Byte = ZERO_INIT;

		/* Set the value of the Output Compare Register to be Zero */
		_OCR0.Byte = ZERO_INIT;
		/* --------------------------------- */

		/* --------------------------------- */
		/* Setup the direction for OC0 as output pin through the GPIO driver */
		gpio_config_t pwm_pin;

		pwm_pin.port = PWM_CTC_OC0_PORT_INDEX;
		pwm_pin.pin = PWM_CTC_OC0_PIN_INDEX;
		pwm_pin.mode = GPIO_MODE_OUTPUT;

		l_status = GPIO_setupPinDirection(&pwm_pin);
		/* --------------------------------- */

		/* --------------------------------- */
		/* select Clock Source */
		_TCCR0._CS0x = pwm_obj->clock_source;
		/* --------------------------------- */

		/* --------------------------------- */
		/* set Duty Cycle to be Zero initially */
		TIMER0_PWM_Start(0);
		/* --------------------------------- */
	}

	return l_status;
}


/**
 * @brief  generate the required duty cycle percentage of the generated PWM signal. Its value should be from 0 → 100
 * @param  (duty_cycle) the value of the duty cycle
 */
void TIMER0_PWM_Start(uint8 duty_cycle)
{
	/* calculate the OCR value according to the duty cycle selected */
	_OCR0.Byte = (uint8)( (duty_cycle / 100.0) * 255 );
}


#endif


/* ----------------------------------------------------------------------------------- */
/* --------------------ISR section---------------------- */


#if TIMER0_MODE_SELECT == TIMER0_MODE_OVER_FLOW
/* TIMER0 is configured in Normal (OverFlow) Mode */


/**
 * @brief  Timer/Counter0 Overflow ISR
 */
ISR(TIMER0_OVF_vect)
{
	/* Clear Timer/Counter0 Over Flow Flag by writing a logic one to its bit location */
	_TIFR._TOV0 = SET;

	/* check if the call back notification contains NULL or not */
	if(TIMER0_OVF_InterruptHandler)
	{
		/* Call Back */
		(*TIMER0_OVF_InterruptHandler)();
	}
	else{ /* Nothing */ }
}


#elif TIMER0_MODE_SELECT == TIMER0_MODE_CLEAR_TIMER_ON_COMPARE_MATCH
/* TIMER0 is configured in CTC (Clear Timer on Compare Match) Mode */

/**
 * @brief  Timer/Counter0 Output Compare Match ISR
 */
ISR(TIMER0_COMP_vect)
{
	/* Clear Timer/Counter0 Output Compare Match Flag by writing a logic one to its bit location */
	_TIFR._OCF0 = SET;

	/* check if the call back notification contains NULL or not */
	if(TIMER0_CTC_InterruptHandler)
	{
		/* Call Back */
		(*TIMER0_CTC_InterruptHandler)();
	}
	else{ /* Nothing */ }
}

#elif TIMER0_MODE_SELECT == TIMER0_MODE_PWM
/* TIMER0 is configured in PWM (Fast or Phase Correct) Mode */

#endif


/* ----------------------------------------------------------------------------------- */
