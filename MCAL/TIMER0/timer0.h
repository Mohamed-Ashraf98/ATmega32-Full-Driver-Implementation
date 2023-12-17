/*
 =========================================================================================
 Name        : timer0.h
 Author      : Mohamed Ashraf El-Sayed
 Version     : 1.0.0
 Copyright   : Your copyright notice
 date        : Wed, Oct 4 2023
 time        :
 Description : TIMER0 Driver Header file , Ansi-style
 =========================================================================================
*/

#ifndef _TIMER0_H_
#define _TIMER0_H_
/* ----------------------------------------------------------------------------------- */
/* ------------------Includes section------------------- */
#include "ATmega32.h"


/* ----------------------------------------------------------------------------------- */
/* --------------Macro declaration section-------------- */


/* --------------------------------- */
/* TIMER0 mode select */

#define TIMER0_MODE_OVER_FLOW									0
#define TIMER0_MODE_CLEAR_TIMER_ON_COMPARE_MATCH				1
#define TIMER0_MODE_PWM											2

#define TIMER0_MODE_SELECT										TIMER0_MODE_PWM

/* --------------------------------- */
/* TIMER0 Interrupt Enable/Disable */

#if TIMER0_MODE_SELECT == TIMER0_MODE_OVER_FLOW

/* @ref : TIMER0 OverFlow Interrupt Enable/Disable */

#define TIMER0_OVERFLOW_INTERRUPT_DISABLE						0
#define TIMER0_OVERFLOW_INTERRUPT_ENABLE						1

#elif TIMER0_MODE_SELECT == TIMER0_MODE_CLEAR_TIMER_ON_COMPARE_MATCH

/* TIMER0 Output Compare Match Interrupt Enable/Disable */

#define TIMER0_OUTPUT_COMPARE_MATCH_INTERRUPT_DISABLE			0
#define TIMER0_OUTPUT_COMPARE_MATCH_INTERRUPT_ENABLE			1

#endif

/* --------------------------------- */
/* to select TIMER0 as PWM mode or not */

/* PWM Mode (Fast or Phase Correct) */
#define TIMER0_FORCE_OUTPUT_COMPARE_DISABLE						0
/* Non PWM Mode (OverFlow or Output Compare Match) */
#define TIMER0_FORCE_OUTPUT_COMPARE_ENABLE						1

/* --------------------------------- */
/* OC0 pin configuration in CTC (Clear Timer on Compare Match) Mode or PWM Mode (Fast or Phase Correct) */

#if (TIMER0_MODE_SELECT == TIMER0_MODE_PWM) || (TIMER0_MODE_SELECT == TIMER0_MODE_CLEAR_TIMER_ON_COMPARE_MATCH)
#define PWM_CTC_OC0_PORT_INDEX									GPIO_PORTB
#define PWM_CTC_OC0_PIN_INDEX									GPIO_PIN3
#endif

/* --------------------------------- */


/* ----------------------------------------------------------------------------------- */
/* --------Macro functions declaration section---------- */

/* Enable/Disable TIMER0 */
#define TIMER0_DISABLE()										(_TCCR0._CS0x = RESET)
#define TIMER0_ENABLE(prescaler)								(_TCCR0._CS0x = prescaler)


/* ----------------------------------------------------------------------------------- */
/* -----user_defined data type declaration section------ */


/* --------------------------------- */
/* TIMER0 config structure */

#if TIMER0_MODE_SELECT == TIMER0_MODE_OVER_FLOW
/* TIMER0 is configured in Normal (OverFlow) Mode */

typedef struct{
	/* pointer to function to hold the function called in the APPLICATION layer when TIMER0 OverFlow occur if TIMER0 OverFlow Interrupt is enabled */
	void (* TIMER0_OVF_DefaultHandler)(void);

	/* select the clock source to be used by the Timer/Counter >> @ref : timer0_clock_source_t */
	uint8 clock_source				:3;

	/* TIMER0 OverFlow Interrupt Enable/Disable >> @ref : @ref : TIMER0 OverFlow Interrupt Enable/Disable */
	uint8 overflow_interrupt_enable	:1;

	/* Reserved */
	uint8							:4;
}timer0_overflow_config_t;

#elif TIMER0_MODE_SELECT == TIMER0_MODE_CLEAR_TIMER_ON_COMPARE_MATCH
/* TIMER0 is configured in CTC (Clear Timer on Compare Match) Mode */

typedef struct{
	/* pointer to function to hold the function called in the APPLICATION layer when TIMER0 Output Compare Match occur if TIMER0 Output Compare Match Interrupt is enabled */
	void (* TIMER0_CTC_DefaultHandler)(void);

	/* control the Output Compare pin (OC0) behavior >> @ref : timer0_ctc_mode_t */
	uint8 OC_pin_mode 			:2;

	/* select the clock source to be used by the Timer/Counter >> @ref : timer0_clock_source_t */
	uint8 clock_source			:3;

	/* Timer/Counter0 Output Compare Match Interrupt Enable/Disable */
	uint8 ctc_interrupt_enable	:1;

	/* Reserved */
	uint8						:2;
}timer0_ctc_config_t;

#elif TIMER0_MODE_SELECT == TIMER0_MODE_PWM
/* TIMER0 is configured in PWM (Fast or Phase Correct) Mode */

typedef struct{
	/* select the timer mode (Fast PWM Mode or Phase Correct PWM Mode) >> @ref : timer0_mode_select_t */
	uint8 mode 			:2;
	/* control the Output Compare pin (OC0) behavior >> @ref : timer0_fast_pwm_mode_t or >> @ref : timer0_phase_correct_pwm_mode_t */
	uint8 OC_pin_mode 	:2;
	/* select the clock source to be used by the Timer/Counter >> @ref : timer0_clock_source_t */
	uint8 clock_source	:3;
	/* Reserved */
	uint8				:1;
}timer0_pwm_config_t;

#endif

/* --------------------------------- */
/* @ref : timer0_mode_select_t */

typedef enum{
	TIMER0_NORMAL_MODE = 0,						/* TOP = 0xFF */
	TIMER0_PHASE_CORRECT_PWM_MODE,				/* TOP = 0xFF */
	TIMER0_CTC_MODE,							/* TOP = OCR0 */
	TIMER0_FAST_PWM_MODE						/* TOP = 0xFF */
}timer0_mode_select_t;

/* --------------------------------- */
/* @ref : timer0_clock_source_t */

typedef enum{
	TIMER0_CLOCK_SOURCE_NONE = 0,				/* Timer/Counter stopped */
	TIMER0_CLOCK_SOURCE_DIV_1,					/* TIMER Mode : clkI/O/(No prescaling) */
	TIMER0_CLOCK_SOURCE_DIV_8,					/* TIMER Mode : clkI/O/8 (From prescaler) */
	TIMER0_CLOCK_SOURCE_DIV_64,					/* TIMER Mode : clkI/O/64 (From prescaler) */
	TIMER0_CLOCK_SOURCE_DIV_256,				/* TIMER Mode : clkI/O/256 (From prescaler) */
	TIMER0_CLOCK_SOURCE_DIV_1024,				/* TIMER Mode : clkI/O/1024 (From prescaler) */
	TIMER0_CLOCK_SOURCE_T0_FALLING_EDGE,		/* COUNTER Mode : External clock source on T0 pin. Clock on falling edge */
	TIMER0_CLOCK_SOURCE_T0_RISING_EDGE			/* COUNTER Mode : External clock source on T0 pin. Clock on rising edge */
}timer0_clock_source_t;

/* --------------------------------- */

#if TIMER0_MODE_SELECT == TIMER0_MODE_OVER_FLOW
/* TIMER0 is configured in Normal (OverFlow) Mode */

#elif TIMER0_MODE_SELECT == TIMER0_MODE_CLEAR_TIMER_ON_COMPARE_MATCH
/* TIMER0 is configured in CTC (Clear Timer on Compare Match) Mode */

/* @ref : timer0_ctc_mode_t */
typedef enum{
	TIMER0_CTC_NORMAL_MODE = 0,						/* Normal port operation, OC0 disconnected */
	TIMER0_CTC_TOGGLE_OC0_ON_COMPARE_MATCH,			/* Toggle OC0 on compare match */
	TIMER0_CTC_CLEAR_OC0_ON_COMPARE_MATCH,			/* Clear OC0 on compare match */
	TIMER0_CTC_SET_OC0_ON_COMPARE_MATCH				/* Set OC0 on compare match */
}timer0_ctc_mode_t;

#elif TIMER0_MODE_SELECT == TIMER0_MODE_PWM
/* TIMER0 is configured in PWM (Fast or Phase Correct) Mode */

/* @ref : timer0_fast_pwm_mode_t */
typedef enum{
	TIMER0_FAST_PWM_NORMAL_MODE = 0,				/* Normal port operation, OC0 disconnected */
	TIMER0_FAST_PWM_NON_INVERTING_MODE = 2,			/* Clear OC0 on compare match, set OC0 at BOTTOM */
	TIMER0_FAST_PWM_INVERTING_MODE					/* Set OC0 on compare match, clear OC0 at BOTTOM */
}timer0_fast_pwm_mode_t;

/* @ref : timer0_phase_correct_pwm_mode_t */
typedef enum{
	TIMER0_PHASE_CORRECT_PWM_NORMAL_MODE = 0,		/* Normal port operation, OC0 disconnected */
	TIMER0_PHASE_CORRECT_PWM_CLEAR_ON_UPCOUNT = 2,	/* Clear OC0 on compare match when up-counting. Set OC0 on compare match when down-counting */
	TIMER0_PHASE_CORRECT_PWM_SET_ON_UPCOUNT			/* Set OC0 on compare match when up-counting. Clear OC0 on compare match when down-counting */
}timer0_phase_correct_pwm_mode_t;

#endif

/* --------------------------------- */


/* ----------------------------------------------------------------------------------- */
/* ------------functions declaration section------------ */


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
Std_ReturnType TIMER0_OVF_init(timer0_overflow_config_t *timer0_ovf_obj);


/**
 * @brief  Set the value of TIMER0 Timer/Counter0 Register
 * @param  (timer0_val) holds the value to be set in the TIMER0 Timer/Counter0 Register
 */
void TIMER0_setTimerValue(uint8 timer0_val);


/**
 * @brief  Set TIMER0 Over Flow Call Back
 * @param  (TIMER0_OVF_Handler) holds the address of the call back function
 */
void TIMER0_OVF_setCallBack( void (* TIMER0_OVF_Handler)(void) );


/**
 * @brief  Disable Timer0 Over Flow
 */
void TIMER0_OVF_deInit(void);


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
Std_ReturnType TIMER0_CTC_init(timer0_ctc_config_t *timer0_ctc_obj);


/**
 * @brief  Set the value of TIMER0 Timer/Counter0 Register
 * @param  (timer0_val) holds the value to be set in the TIMER0 Timer/Counter0 Register
 */
void TIMER0_TCNT0_setValue(uint8 timer0_val);


/**
 * @brief  Set the value of TIMER0 Output Compare Register
 * @param  (timer0_val) holds the value to be set in the TIMER0 Output Compare Register
 */
void TIMER0_OCR0_setValue(uint8 timer0_val);


/**
 * @brief  Set TIMER0 Output Compare Match Call Back
 * @param  (TIMER0_CTC_Handler) holds the address of the call back function
 */
void TIMER0_CTC_setCallBack( void (* TIMER0_CTC_Handler)(void) );


/**
 * @brief  Disable Timer0 Output Compare Match
 */
void TIMER0_CTC_deInit(void);


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
Std_ReturnType TIMER0_PWM_init(timer0_pwm_config_t *pwm_obj);


/**
 * @brief  generate the required duty cycle percentage of the generated PWM signal. Its value should be from 0 → 100
 * @param  (duty_cycle) the value of the duty cycle
 */
void TIMER0_PWM_Start(uint8 duty_cycle);


#endif


/* ----------------------------------------------------------------------------------- */
#endif /* _TIMER0_H_ */
