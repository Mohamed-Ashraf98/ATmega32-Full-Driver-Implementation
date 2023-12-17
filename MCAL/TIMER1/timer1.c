/*
 =========================================================================================
 Name        : timer1.c
 Author      : Mohamed Ashraf El-Sayed
 Version     : 1.0.0
 Copyright   : Your copyright notice
 date        : Sun, Oct 15 2023
 time        :
 Description : TIMER1 Driver Source file , Ansi-style
 =========================================================================================
*/

/* ----------------------------------------------------------------------------------- */
/* ------------------Includes section------------------- */
#include <avr/interrupt.h> 			/* For TIMER1 ISR */

#include "timer1.h"

#if (TIMER1_MODE_SELECT == TIMER1_MODE_PWM) || (TIMER1_MODE_SELECT == TIMER1_MODE_CLEAR_TIMER_ON_COMPARE_MATCH)
#include "gpio.h"					/* For TIMER1 OC1A/OC1B pin setup */
#endif


/* ----------------------------------------------------------------------------------- */
/* -------------------Global section-------------------- */


/* create a pointer to function to hold the address of the call back function */

#if TIMER1_MODE_SELECT == TIMER1_MODE_OVER_FLOW
/* TIMER1 is configured in Normal (OverFlow) Mode */

/* TIMER1 Over Flow Call Back */
static void (* TIMER1_OVF_InterruptHandler)(void) = NULL_PTR;
/* TIMER1 Input Capture Call Back */
static void (* TIMER1_ICU_InterruptHandler)(void) = NULL_PTR;

#elif TIMER1_MODE_SELECT == TIMER1_MODE_CLEAR_TIMER_ON_COMPARE_MATCH
/* TIMER1 is configured in CTC (Clear Timer on Compare Match) Mode */

/* TIMER1 Clear Timer on Compare A Match Interrupt Call Back */
static void (* TIMER1_CTC_A_InterruptHandler)(void) = NULL_PTR;
/* TIMER1 Clear Timer on Compare B Match Interrupt Call Back */
static void (* TIMER1_CTC_B_InterruptHandler)(void) = NULL_PTR;

#elif TIMER1_MODE_SELECT == TIMER1_MODE_PWM
/* TIMER1 is configured in PWM (Fast or Phase Correct) Mode */

#endif


/* ----------------------------------------------------------------------------------- */
/* ------------functions definition section------------- */


#if TIMER1_MODE_SELECT == TIMER1_MODE_OVER_FLOW
/* TIMER1 is configured in Normal (OverFlow) Mode */


/**
 * @brief  initialize TIMER1 :
 *			1- Select Mode : Over Flow (Normal) Mode
 *			2- Select Clock Source
 *				- No clock source (Timer/Counter stopped)
 *				- clkI/O/(No pre-scaler)
 *				- clkI/O/8 (From pre-scaler)
 *				- clkI/O/64 (From pre-scaler)
 *				- clkI/O/256 (From pre-scaler)
 *				- clkI/O/1024 (From pre-scaler)
 *			3- Set the value of the Timer/Counter Register to be Zero
 *			4- Set the value of the Input Capture Register to be Zero
 *			5- Selects which edge on the Input Capture Pin (ICP1) that is used to trigger a capture event
 *				- Falling (negative) edge will trigger the capturer
 *				- Rising (positive) edge will trigger the capture
 *			6- Enable/Disable Input Capture Noise Canceler
 *			7- Set TIMER1 Over Flow Call Back if Over Flow Interrupt enabled
 *			8- Set TIMER1 Input Capture Call Back if Input Capture Interrupt is enabled
 * @param  (timer1_ovf_obj) pointer to the TIMER1 Over Flow Mode object passed by reference
 * @return (l_status) 		status of the performed operation
 *              (E_NOT_OK)  operation failed
 *              (E_OK)      operation success
 */
Std_ReturnType TIMER1_OVERFLOW_init(timer1_overflow_config_t *timer1_ovf_obj)
{
	/* create a local variable to hold the status of the performed operation */
	Std_ReturnType l_status = ZERO_INIT;

	/* check if the address is valid or not */
	if(timer1_ovf_obj == NULL_PTR)
	{
		/* NULL pointer is passed */

		l_status = E_NOK;		/* operation failed */
	}
	else
	{
		/* the address passed is valid */

		l_status = E_OK;		/* operation success */

		/* TIMER1 Over Flow (Normal) Mode Init */

		/* --------------------------------- */
		/* Non PWM Mode (OverFlow or Output Compare Match) */
		_TCCR1A._FOC1A = TIMER1_FORCE_OUTPUT_COMPARE_A_ENABLE;
		_TCCR1A._FOC1B = TIMER1_FORCE_OUTPUT_COMPARE_B_ENABLE;

		/* Set TIMER1 Mode to be Over Flow (Normal) Mode */
		_TCCR1A._WGM10 = RESET;
		_TCCR1A._WGM11 = RESET;
		_TCCR1B._WGM12 = RESET;
		_TCCR1B._WGM13 = RESET;

		/* Normal port operation, OC1A/OC1B disconnected */
		_TCCR1A._COM1Ax = RESET;
		_TCCR1A._COM1Bx = RESET;
		/* --------------------------------- */

		/* --------------------------------- */
		/* Set the value of the Timer/Counter Register to be Zero */
		_TCNT1.TwoBytes = ZERO_INIT;

		/* Set the value of the Input Capture Register to be Zero */
		_ICR1.TwoBytes = ZERO_INIT;
		/* --------------------------------- */

		/* --------------------------------- */
		/* Selects which edge on the Input Capture Pin (ICP1) that is used to trigger a capture event */
		_TCCR1B._ICES1 = timer1_ovf_obj->icu_edge_select;

		/* Enable/Disable Input Capture Noise Canceler */
		_TCCR1B._ICNC1 = timer1_ovf_obj->icu_noise_canceler_enable;
		/* --------------------------------- */

		/* --------------------------------- */
		/* Enable/Disable Timer/Counter1 Overflow Interrupt */
		_TIMSK._TOIE1 = timer1_ovf_obj->overflow_interrupt_enable;

		/* Set TIMER1 Over Flow Call Back if Over Flow Interrupt enabled */
		if(timer1_ovf_obj->overflow_interrupt_enable == TIMER1_OVERFLOW_INTERRUPT_DISABLE)
		{
			/* TIMER1 Over Flow Interrupt is Disabled */
			TIMER1_OVF_InterruptHandler = NULL_PTR;
		}
		else if(timer1_ovf_obj->overflow_interrupt_enable == TIMER1_OVERFLOW_INTERRUPT_ENABLE)
		{
			/* TIMER1 Over Flow Interrupt is Enabled */
			TIMER1_OVF_InterruptHandler = timer1_ovf_obj->TIMER1_OVF_DefaultHandler;
		}
		else{ /* Nothing */ }


		/* Enable/Disable Timer/Counter1 Input Capture Interrupt */
		_TIMSK._TICIE1 = timer1_ovf_obj->inputcapture_interrupt_enable;

		/* Set TIMER1 Input Capture Call Back if Input Capture Interrupt is enabled */
		if(timer1_ovf_obj->inputcapture_interrupt_enable == TIMER1_INPUT_CAPTURE_INTERRUPT_DISABLE)
		{
			/* TIMER1 Input Capture Interrupt is Disabled */
			TIMER1_ICU_InterruptHandler = NULL_PTR;
		}
		else if(timer1_ovf_obj->inputcapture_interrupt_enable == TIMER1_INPUT_CAPTURE_INTERRUPT_ENABLE)
		{
			/* TIMER1 Over Flow Interrupt is Enabled */
			TIMER1_ICU_InterruptHandler = timer1_ovf_obj->TIMER1_ICU_DefaultHandler;
		}
		else{ /* Nothing */ }
		/* --------------------------------- */

		/* --------------------------------- */
		/* select TIMER1 Clock Source */
		_TCCR1B._CS1x = timer1_ovf_obj->clock_source;
		/* --------------------------------- */
	}

	return l_status;
}


/**
 * @brief  Set the value of TIMER1 Timer/Counter1 Register
 * @param  (timer1_val) holds the value to be set in the TIMER1 Timer/Counter1 Register
 */
void TIMER1_setTimerValue(uint16 timer1_val)
{
	_TCNT1.TwoBytes = timer1_val;
}


/**
 * @brief  Selects which edge on the Input Capture Pin (ICP1) that is used to trigger a capture event
 * @param  (icu_edge_type) holds the edge Falling/Rising required
 */
void TIMER1_ICU_setEdgeDetectionType(icu_edge_select_t icu_edge_type)
{
	_TCCR1B._ICES1 = icu_edge_type;
}


/**
 * @brief  Get the value stored in TIMER1 Input Capture Register
 * @return TIMER1 Input Capture Register value
 */
uint16 TIMER1_ICU_getInputCaptureValue(void)
{
	return (_ICR1.TwoBytes);
}


/**
 * @brief  Set TIMER1 Over Flow Call Back
 * @param  (TIMER1_OVF_Handler) holds the address of the call back function
 */
void TIMER1_OVF_setCallBack( void (* TIMER1_OVF_Handler)(void) )
{
	TIMER1_OVF_InterruptHandler = TIMER1_OVF_Handler;
}


/**
 * @brief  Set TIMER1 Input Capture Call Back
 * @param  (TIMER1_ICU_Handler) holds the address of the call back function
 */
void TIMER1_ICU_setCallBack( void (* TIMER1_ICU_Handler)(void) )
{
	TIMER1_ICU_InterruptHandler = TIMER1_ICU_Handler;
}


/**
 * @brief  Disable Timer1 Over Flow including ICU
 */
void TIMER1_OVF_deInit(void)
{
	/* Reset the Timer/Counter Control Registers including Clock Select */
	_TCCR1A.Byte = ZERO_INIT;
	/* No clock source (Timer/Counter stopped) */
	_TCCR1B.Byte = ZERO_INIT;

	/* Set the value of the Timer/Counter Register to be Zero */
	_TCNT1.TwoBytes = ZERO_INIT;

	/* Set the value of the Input Capture Register to be Zero */
	_ICR1.TwoBytes = ZERO_INIT;

	/* Disable Timer/Counter1, Overflow Interrupt */
	_TIMSK._TOIE1 = TIMER1_OVERFLOW_INTERRUPT_DISABLE;

	/* Disable Timer/Counter1, Input Capture Interrupt */
	_TIMSK._TICIE1 = TIMER1_INPUT_CAPTURE_INTERRUPT_DISABLE;

	/* Reset TIMER1 Over Flow and Input Capture Call Backs as Over Flow Interrupt and Input Capture Interrupt are disabled */
	/* TIMER1 Over Flow Interrupt and Input Capture Interrupt are Disabled */
	TIMER1_OVF_InterruptHandler = NULL_PTR;
	TIMER1_ICU_InterruptHandler = NULL_PTR;
}


/**
 * @brief  Disable Timer1 ICU
 */
void TIMER1_ICU_deInit(void)
{
	/* Set the value of the Input Capture Register to be Zero */
	_ICR1.TwoBytes = ZERO_INIT;

	/* Disable Timer/Counter1, Input Capture Interrupt */
	_TIMSK._TICIE1 = TIMER1_INPUT_CAPTURE_INTERRUPT_DISABLE;

	/* Reset TIMER1 Input Capture Call Back as Input Capture Interrupt is disabled */
	/* TIMER1 Input Capture Interrupt is Disabled */
	TIMER1_ICU_InterruptHandler = NULL_PTR;
}


#elif TIMER1_MODE_SELECT == TIMER1_MODE_CLEAR_TIMER_ON_COMPARE_MATCH
/* TIMER1 is configured in CTC (Clear Timer on Compare Match) Mode */


/**
 * @brief  initialize TIMER1 :
 *			1- Select Mode : Clear Timer on Compare Match (CTC) Mode
 *				- CTC Mode 1 >> TOP = OCR1A
 *				- CTC Mode 1 >> TOP = ICR1
 *			2- Select Clock Source
 *				- No clock source (Timer/Counter stopped)
 *				- clkI/O/(No pre-scaler)
 *				- clkI/O/8 (From pre-scaler)
 *				- clkI/O/64 (From pre-scaler)
 *				- clkI/O/256 (From pre-scaler)
 *				- clkI/O/1024 (From pre-scaler)
 *			3- Set the value of the Timer/Counter Register to be Zero
 *			4- Set the value of the Output Compare Register to be Zero
 *			5- Setup the direction for OC1A and OC1B as output pins through the GPIO driver if enabled
 *			6- Set TIMER1 Output Compare A,B Match Call Back if Output Compare A,B Match Interrupt enabled
 * @param  (timer1_ctc_obj) pointer to the TIMER1 CTC Mode object passed by reference
 * @return (l_status) 		status of the performed operation
 *              (E_NOT_OK)  operation failed
 *              (E_OK)      operation success
 */
Std_ReturnType TIMER1_CTC_init(timer1_ctc_config_t *timer1_ctc_obj)
{
	/* create a local variable to hold the status of the performed operation */
	Std_ReturnType l_status = ZERO_INIT;

	/* check if the address is valid or not */
	if(timer1_ctc_obj == NULL_PTR)
	{
		/* NULL pointer is passed */

		l_status = E_NOK;		/* operation failed */
	}
	else
	{
		/* the address passed is valid */

		l_status = E_OK;		/* operation success */

		/* TIMER1 Clear Timer on Compare Match (CTC) Mode Init */

		/* --------------------------------- */
		/* Non PWM Mode (OverFlow or Output Compare Match) */
		_TCCR1A._FOC1A = TIMER1_FORCE_OUTPUT_COMPARE_A_ENABLE;
		_TCCR1A._FOC1B = TIMER1_FORCE_OUTPUT_COMPARE_B_ENABLE;

		/* set the TIMER1 Output Compare Match Mode */
		if(timer1_ctc_obj->mode == TIMER1_CTC_MODE_1)
		{
			/* TOP = OCR1A */
			_TCCR1A._WGM10 = RESET;
			_TCCR1A._WGM11 = RESET;
			_TCCR1B._WGM12 = SET;
			_TCCR1B._WGM13 = RESET;
		}
		else if(timer1_ctc_obj->mode == TIMER1_CTC_MODE_2)
		{
			/* TOP = ICR1 */
			_TCCR1A._WGM10 = RESET;
			_TCCR1A._WGM11 = RESET;
			_TCCR1B._WGM12 = SET;
			_TCCR1B._WGM13 = SET;
		}
		else{ /* Nothing */ }

		/* control the Output Compare pins (OC1A and OC1B) behavior */
		_TCCR1A._COM1Ax = timer1_ctc_obj->OC1A_mode;
		_TCCR1A._COM1Bx = timer1_ctc_obj->OC1B_mode;
		/* --------------------------------- */

		/* --------------------------------- */
		/* Set the value of the Timer/Counter Register to be Zero */
		_TCNT1.TwoBytes = ZERO_INIT;

		/* Set the value of the Output Compare A,B Register to be Zero */
		_OCR1A.TwoBytes = ZERO_INIT;
		_OCR1B.TwoBytes = ZERO_INIT;

		/* Set the value of the Input Capture Register to be Zero */
		_ICR1.TwoBytes = ZERO_INIT;
		/* --------------------------------- */

		/* --------------------------------- */
		/* Setup the direction for OC1A and OC1B as output pins through the GPIO driver if enabled */

		if(timer1_ctc_obj->OC1A_mode != TIMER1_CTC_NORMAL_MODE)
		{
			/* configure OC1A pin to be output for the wave generation */
			gpio_config_t OC1A_pin;

			OC1A_pin.port = PWM_CTC_OC1A_PORT_INDEX;
			OC1A_pin.pin = PWM_CTC_OC1A_PIN_INDEX;
			OC1A_pin.mode = GPIO_MODE_OUTPUT;

			l_status |= GPIO_setupPinDirection(&OC1A_pin);
		}
		else{ /* Nothing */ }

		if(timer1_ctc_obj->OC1B_mode != TIMER1_CTC_NORMAL_MODE)
		{
			/* configure OC1B pin to be output for the wave generation */
			gpio_config_t OC1B_pin;

			OC1B_pin.port = PWM_CTC_OC1B_PORT_INDEX;
			OC1B_pin.pin = PWM_CTC_OC1B_PIN_INDEX;
			OC1B_pin.mode = GPIO_MODE_OUTPUT;

			l_status |= GPIO_setupPinDirection(&OC1B_pin);
		}
		else{ /* Nothing */ }
		/* --------------------------------- */

		/* --------------------------------- */
		/* Enable/Disable Timer/Counter1 Output Compare A Match Interrupt */
		_TIMSK._OCIE1A = timer1_ctc_obj->ctc_A_interrupt_en;

		/* Set TIMER1 Output Compare A Match Call Back if Output Compare A Match Interrupt enabled */
		if(timer1_ctc_obj->ctc_A_interrupt_en == TIMER1_OUTPUT_COMPARE_A_MATCH_INTERRUPT_DISABLE)
		{
			/* TIMER1 Output Compare A Match Interrupt is Disabled */
			TIMER1_CTC_A_InterruptHandler = NULL_PTR;
		}
		else if(timer1_ctc_obj->ctc_A_interrupt_en == TIMER1_OUTPUT_COMPARE_A_MATCH_INTERRUPT_ENABLE)
		{
			/* TIMER1 Output Compare A Match Interrupt is Enabled */
			TIMER1_CTC_A_InterruptHandler = timer1_ctc_obj->TIMER1_CTC_A_DefaultHandler;
		}
		else{ /* Nothing */ }


		/* Enable/Disable Timer/Counter1 Output Compare B Match Interrupt */
		_TIMSK._OCIE1B = timer1_ctc_obj->ctc_B_interrupt_en;

		/* Set TIMER1 Output Compare B Match Call Back if Output Compare B Match Interrupt enabled */
		if(timer1_ctc_obj->ctc_B_interrupt_en == TIMER1_OUTPUT_COMPARE_B_MATCH_INTERRUPT_DISABLE)
		{
			/* TIMER1 Output Compare B Match Interrupt is Disabled */
			TIMER1_CTC_B_InterruptHandler = NULL_PTR;
		}
		else if(timer1_ctc_obj->ctc_B_interrupt_en == TIMER1_OUTPUT_COMPARE_B_MATCH_INTERRUPT_ENABLE)
		{
			/* TIMER1 Output Compare B Match Interrupt is Enabled */
			TIMER1_CTC_B_InterruptHandler = timer1_ctc_obj->TIMER1_CTC_B_DefaultHandler;
		}
		else{ /* Nothing */ }
		/* --------------------------------- */

		/* --------------------------------- */
		/* select TIMER1 Clock Source */
		TIMER1_ENABLE(timer1_ctc_obj->clock_source);
		/* --------------------------------- */
	}

	return l_status;
}


/**
 * @brief  Set the value of TIMER1 Timer/Counter1 Register
 * @param  (timer1_val) holds the value to be set in the TIMER1 Timer/Counter1 Register
 */
void TIMER1_TCNT1_setValue(uint16 timer1_val)
{
	_TCNT1.TwoBytes = timer1_val;
}


/**
 * @brief  Set the value of TIMER1 Output Compare A Register
 * @param  (timer1_val) holds the value to be set in the TIMER1 Output Compare A Register
 */
void TIMER1_OCR1A_setValue(uint16 timer1_val)
{
	_OCR1A.TwoBytes = timer1_val;
}


/**
 * @brief  Set the value of TIMER1 Output Compare B Register
 * @param  (timer1_val) holds the value to be set in the TIMER1 Output Compare B Register
 */
void TIMER1_OCR1B_setValue(uint16 timer1_val)
{
	_OCR1B.TwoBytes = timer1_val;
}


/**
 * @brief  Set the value of TIMER1 Input Compare Register
 * @param  (timer1_val) holds the value to be set in the TIMER1 Input Compare Register
 */
void TIMER1_ICR1_setValue(uint16 timer1_val)
{
	_ICR1.TwoBytes = timer1_val;
}


/**
 * @brief  Set TIMER1 Output Compare A Match Call Back
 * @param  (TIMER1_CTC_A_Handler) holds the address of the call back function
 */
void TIMER1_CTC_A_setCallBack( void (* TIMER1_CTC_A_Handler)(void) )
{
	TIMER1_CTC_A_InterruptHandler = TIMER1_CTC_A_Handler;
}


/**
 * @brief  Set TIMER1 Output Compare B Match Call Back
 * @param  (TIMER1_CTC_B_Handler) holds the address of the call back function
 */
void TIMER1_CTC_B_setCallBack( void (* TIMER1_CTC_B_Handler)(void) )
{
	TIMER1_CTC_B_InterruptHandler = TIMER1_CTC_B_Handler;
}


/**
 * @brief  Disable Timer1 Output Compare Match
 */
void TIMER1_CTC_deInit(void)
{
	/* Reset the Timer/Counter Control Registers including Clock Select */
	_TCCR1A.Byte = ZERO_INIT;
	/* No clock source (Timer/Counter stopped) */
	_TCCR1B.Byte = ZERO_INIT;

	/* Set the value of the Timer/Counter Register to be Zero */
	_TCNT1.TwoBytes = ZERO_INIT;

	/* Set the value of the Input Capture Register to be Zero */
	_ICR1.TwoBytes = ZERO_INIT;

	/* Set the value of the Output Compare A,B Register to be Zero */
	_OCR1A.TwoBytes = ZERO_INIT;
	_OCR1B.TwoBytes = ZERO_INIT;

	/* Disable Timer/Counter1 Output Compare A Match Interrupt */
	_TIMSK._OCIE1A = TIMER1_OUTPUT_COMPARE_A_MATCH_INTERRUPT_DISABLE;
	/* Disable Timer/Counter1 Output Compare B Match Interrupt */
	_TIMSK._OCIE1B = TIMER1_OUTPUT_COMPARE_B_MATCH_INTERRUPT_DISABLE;

	/* Reset TIMER1 Output Compare A,B Match Call Back as Output Compare A,B Match Interrupt is disabled */
	TIMER1_CTC_A_InterruptHandler = NULL_PTR;
	TIMER1_CTC_B_InterruptHandler = NULL_PTR;
}


#elif TIMER1_MODE_SELECT == TIMER1_MODE_PWM
/* TIMER1 is configured in PWM (Fast or Phase Correct) Mode */


/**
 * @brief  initialize TIMER1 :
 *			1- mode
 *				- Fast PWM Mode
 *					. WGM13:0 = 15: Toggle OC1A on Compare Match, OC1B disconnected (normal port operation)
 *					. Non Inverting (Clear OC1A/OC1B on compare match, set OC1A/OC1B at BOTTOM)
 *					. Inverting (Set OC1A/OC1B on compare match, clear OC1A/OC1B at BOTTOM)
 *				- Phase Correct PWM Mode
 *					. WGM13:0 = 15: Toggle OC1A on Compare Match, OC1B disconnected (normal port operation)
 *					. Clear OC1A/OC1B on compare match when up-counting. Set OC1A/OC1B on compare match when downcounting
 *					. Set OC1A/OC1B on compare match when upcounting. Clear OC1A/OC1B on compare match when downcounting
 *			2- Clock Source
 *				- No clock source (Timer/Counter stopped)
 *				- clkI/O/(No pre-scaler)
 *				- clkI/O/8 (From pre-scaler)
 *				- clkI/O/64 (From pre-scaler)
 *				- clkI/O/256 (From pre-scaler)
 *				- clkI/O/1024 (From pre-scaler)
 *			3- Set the value of the Timer/Counter Register to be Zero
 *			4- Set the value of the Output Compare Register to be Zero
 *			5- Setup the direction for OC1A and OC1B as output pins through the GPIO driver
 * @param  (pwm_obj) 	pointer to the TIMER0 PWM Mode object passed by reference
 * @return (l_status) 	status of the performed operation
 *              (E_NOT_OK)  operation failed
 *              (E_OK)      operation success
 */
Std_ReturnType TIMER1_PWM_init(timer1_pwm_config_t *pwm_obj)
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

		/* TIMER1 PWM Mode Init */

		/* --------------------------------- */
		/* PWM Mode (Fast or Phase Correct) */
		_TCCR1A._FOC1A = TIMER1_FORCE_OUTPUT_COMPARE_A_DISABLE;
		_TCCR1A._FOC1B = TIMER1_FORCE_OUTPUT_COMPARE_B_DISABLE;


		/* Select TIMER1 Mode */
		_TCCR1A._WGM10 = (pwm_obj->mode >> 0) & 0x1;
		_TCCR1A._WGM11 = (pwm_obj->mode >> 1) & 0x1;
		_TCCR1B._WGM12 = (pwm_obj->mode >> 2) & 0x1;
		_TCCR1B._WGM13 = (pwm_obj->mode >> 3) & 0x1;

		/* control the Output Compare pins (OC1A and OC1B) behavior */
		_TCCR1A._COM1Ax = pwm_obj->OC1A_mode;
		_TCCR1A._COM1Bx = pwm_obj->OC1B_mode;
		/* --------------------------------- */

		/* --------------------------------- */
		/* Set the value of the Timer/Counter Register to be Zero */
		_TCNT1.TwoBytes = ZERO_INIT;

		/* Set the value of the Output Compare A,B Register to be Zero */
		_OCR1A.TwoBytes = ZERO_INIT;
		_OCR1B.TwoBytes = ZERO_INIT;

		/* Set the value of the Input Capture Register to be Zero */
		_ICR1.TwoBytes = ZERO_INIT;
		/* --------------------------------- */

		/* --------------------------------- */
		/* Setup the direction for OC1A and OC1B as output pins through the GPIO driver if enabled */

		if( (pwm_obj->OC1A_mode != TIMER1_FAST_PWM_NORMAL_MODE) || (pwm_obj->OC1A_mode != TIMER1_PHASE_CORRECT_PWM_NORMAL_MODE) )
		{
			/* configure OC1A pin to be output for the wave generation */
			gpio_config_t OC1A_pin;

			OC1A_pin.port = PWM_CTC_OC1A_PORT_INDEX;
			OC1A_pin.pin = PWM_CTC_OC1A_PIN_INDEX;
			OC1A_pin.mode = GPIO_MODE_OUTPUT;

			l_status |= GPIO_setupPinDirection(&OC1A_pin);
		}
		else{ /* Nothing */ }

		if( (pwm_obj->OC1B_mode != TIMER1_FAST_PWM_NORMAL_MODE) || (pwm_obj->OC1B_mode != TIMER1_PHASE_CORRECT_PWM_NORMAL_MODE) )
		{
			/* configure OC1B pin to be output for the wave generation */
			gpio_config_t OC1B_pin;

			OC1B_pin.port = PWM_CTC_OC1B_PORT_INDEX;
			OC1B_pin.pin = PWM_CTC_OC1B_PIN_INDEX;
			OC1B_pin.mode = GPIO_MODE_OUTPUT;

			l_status |= GPIO_setupPinDirection(&OC1B_pin);
		}
		else{ /* Nothing */ }
		/* --------------------------------- */

		/* --------------------------------- */
		/* select TIMER1 Clock Source */
		TIMER1_ENABLE(pwm_obj->clock_source);
		/* --------------------------------- */

		/* --------------------------------- */
		/* set Duty Cycle to be Zero initially */
		TIMER1_PWM_Start(0);
		/* --------------------------------- */
	}

	return l_status;
}


/**
 * @brief  Set the value of TIMER1 Timer/Counter1 Register
 * @param  (timer1_val) holds the value to be set in the TIMER1 Timer/Counter1 Register
 */
void TIMER1_TCNT1_setValue(uint16 timer1_val)
{
	_TCNT1.TwoBytes = timer1_val;
}


/**
 * @brief  Set the value of TIMER1 Output Compare A Register
 * @param  (timer1_val) holds the value to be set in the TIMER1 Output Compare A Register
 */
void TIMER1_OCR1A_setValue(uint16 timer1_val)
{
	_OCR1A.TwoBytes = timer1_val;
}


/**
 * @brief  Set the value of TIMER1 Output Compare B Register
 * @param  (timer1_val) holds the value to be set in the TIMER1 Output Compare B Register
 */
void TIMER1_OCR1B_setValue(uint16 timer1_val)
{
	_OCR1B.TwoBytes = timer1_val;
}


/**
 * @brief  Set the value of TIMER1 Input Compare Register
 * @param  (timer1_val) holds the value to be set in the TIMER1 Input Compare Register
 */
void TIMER1_ICR1_setValue(uint16 timer1_val)
{
	_ICR1.TwoBytes = timer1_val;
}


/**
 * @brief  generate the required duty cycle percentage of the generated PWM signal. Its value should be from 0 → 100
 * @param  (duty_cycle) the value of the duty cycle
 */
void TIMER1_PWM_Start(uint8 duty_cycle)
{
	uint8 mode = ZERO_INIT;

	mode = (_TCCR1A._WGM10 << 0) | (_TCCR1A._WGM11 << 1) | (_TCCR1B._WGM12 << 2) | (_TCCR1B._WGM13 << 3);

	switch(mode)
	{
	case TIMER1_PHASE_CORRECT_PWM_8_BIT_MODE		:	/* to do.. */	break;
	case TIMER1_PHASE_CORRECT_PWM_9_BIT_MODE		:	/* to do.. */	break;
	case TIMER1_PHASE_CORRECT_PWM_10_BIT_MODE		:	/* to do.. */	break;
	case TIMER1_FAST_PWM_8_BIT_MODE					:	/* to do.. */	break;
	case TIMER1_FAST_PWM_9_BIT_MODE					:	/* to do.. */	break;
	case TIMER1_FAST_PWM_10_BIT_MODE				:	/* to do.. */	break;
	case TIMER1_PHASE_AND_FREQUENCY_CORRECT_MODE_1	:	/* to do.. */	break;
	case TIMER1_PHASE_AND_FREQUENCY_CORRECT_MODE_2	:	/* to do.. */	break;
	case TIMER1_PHASE_CORRECT_MODE_1				:	/* to do.. */	break;
	case TIMER1_PHASE_CORRECT_MODE_2				:	/* to do.. */	break;

	case TIMER1_FAST_PWM_MODE_1						:	/* TOP = ICR1 */
														_OCR1A.TwoBytes = (uint16)( (duty_cycle / 100.0) * _ICR1.TwoBytes );
														break;

	case TIMER1_FAST_PWM_MODE_2						:	/* TOP = OCR1A */
														_OCR1B.TwoBytes = (uint16)( (duty_cycle / 100.0) * _OCR1A.TwoBytes );
														break;

	default :	/* Nothing */	break;
	}
}


#endif


/* ----------------------------------------------------------------------------------- */
/* --------------------ISR section---------------------- */


#if TIMER1_MODE_SELECT == TIMER1_MODE_OVER_FLOW
/* TIMER1 is configured in Normal (OverFlow) Mode */

/**
 * @brief  Timer/Counter1 Overflow ISR
 */
ISR(TIMER1_OVF_vect)
{
	/* Clear Timer/Counter1 Over Flow Flag by writing a logic one to its bit location */
	_TIFR._TOV1 = SET;

	/* check if the call back notification contains NULL or not */
	if(TIMER1_OVF_InterruptHandler)
	{
		/* Call Back */
		(*TIMER1_OVF_InterruptHandler)();
	}
	else{ /* Nothing */ }
}

/**
 * @brief  Timer/Counter1 Capture Event ISR
 */
ISR(TIMER1_CAPT_vect)
{
	/* Clear Timer/Counter1 Input Capture Flag by writing a logic one to its bit location */
	_TIFR._ICF1 = SET;

	/* check if the call back notification contains NULL or not */
	if(TIMER1_ICU_InterruptHandler)
	{
		/* Call Back */
		(*TIMER1_ICU_InterruptHandler)();
	}
	else{ /* Nothing */ }
}

#elif TIMER1_MODE_SELECT == TIMER1_MODE_CLEAR_TIMER_ON_COMPARE_MATCH
/* TIMER1 is configured in CTC (Clear Timer on Compare Match) Mode */

/**
 * @brief  Timer/Counter1 Output Compare A Match ISR
 */
ISR(TIMER1_COMPA_vect)
{
	/* Clear Timer/Counter1 Output Compare A Match Flag by writing a logic one to its bit location */
	_TIFR._OCF1A = SET;

	/* check if the call back notification contains NULL or not */
	if(TIMER1_CTC_A_InterruptHandler)
	{
		/* Call Back */
		(*TIMER1_CTC_A_InterruptHandler)();
	}
	else{ /* Nothing */ }
}

/**
 * @brief  Timer/Counter1 Output Compare B Match ISR
 */
ISR(TIMER1_COMPB_vect)
{
	/* Clear Timer/Counter1 Output Compare B Match Flag by writing a logic one to its bit location */
	_TIFR._OCF1B = SET;

	/* check if the call back notification contains NULL or not */
	if(TIMER1_CTC_B_InterruptHandler)
	{
		/* Call Back */
		(*TIMER1_CTC_B_InterruptHandler)();
	}
	else{ /* Nothing */ }
}

#elif TIMER1_MODE_SELECT == TIMER1_MODE_PWM
/* TIMER1 is configured in PWM (Fast or Phase Correct) Mode */

#endif


/* ----------------------------------------------------------------------------------- */
