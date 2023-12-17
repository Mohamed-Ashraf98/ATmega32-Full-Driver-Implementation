/*
 =========================================================================================
 Name        : timer1.h
 Author      : Mohamed Ashraf El-Sayed
 Version     : 1.0.0
 Copyright   : Your copyright notice
 date        : Sun, Oct 15 2023
 time        :
 Description : TIMER1 Driver Header file , Ansi-style
 =========================================================================================
*/

#ifndef _TIMER1_H_
#define _TIMER1_H_
/* ----------------------------------------------------------------------------------- */
/* ------------------Includes section------------------- */
#include "ATmega32.h"


/* ----------------------------------------------------------------------------------- */
/* --------------Macro declaration section-------------- */


/* --------------------------------- */
/* TIMER1 mode select */

#define TIMER1_MODE_OVER_FLOW									0
#define TIMER1_MODE_CLEAR_TIMER_ON_COMPARE_MATCH				1
#define TIMER1_MODE_PWM											2

#define TIMER1_MODE_SELECT										TIMER1_MODE_CLEAR_TIMER_ON_COMPARE_MATCH

/* --------------------------------- */
/* TIMER1 Interrupt Enable/Disable */

#if TIMER1_MODE_SELECT == TIMER1_MODE_OVER_FLOW

/* @ref : TIMER1 OverFlow Interrupt Enable/Disable */
#define TIMER1_OVERFLOW_INTERRUPT_DISABLE						0
#define TIMER1_OVERFLOW_INTERRUPT_ENABLE						1

/* @ref : TIMER1 Input Capture Interrupt Enable/Disable */
#define TIMER1_INPUT_CAPTURE_INTERRUPT_DISABLE					0
#define TIMER1_INPUT_CAPTURE_INTERRUPT_ENABLE					1

#elif TIMER1_MODE_SELECT == TIMER1_MODE_CLEAR_TIMER_ON_COMPARE_MATCH

/* @ref : TIMER1 Output Compare A Match Interrupt Enable/Disable */
#define TIMER1_OUTPUT_COMPARE_A_MATCH_INTERRUPT_DISABLE			0
#define TIMER1_OUTPUT_COMPARE_A_MATCH_INTERRUPT_ENABLE			1

/* @ref : TIMER1 Output Compare B Match Interrupt Enable/Disable */
#define TIMER1_OUTPUT_COMPARE_B_MATCH_INTERRUPT_DISABLE			0
#define TIMER1_OUTPUT_COMPARE_B_MATCH_INTERRUPT_ENABLE			1

#endif

/* --------------------------------- */
/* to select TIMER1 as PWM Mode or not */

/* PWM Mode (Fast or Phase Correct) */
#define TIMER1_FORCE_OUTPUT_COMPARE_A_DISABLE					0
#define TIMER1_FORCE_OUTPUT_COMPARE_B_DISABLE					0
/* Non PWM Mode (OverFlow or Output Compare Match) */
#define TIMER1_FORCE_OUTPUT_COMPARE_A_ENABLE					1
#define TIMER1_FORCE_OUTPUT_COMPARE_B_ENABLE					1

/* --------------------------------- */
#if TIMER1_MODE_SELECT == TIMER1_MODE_OVER_FLOW

/* TIMER1 Input Capture Unit configuration */

/* @ref : Input Capture Noise Canceler Enable/Disable */
#define TIMER1_ICU_NOISE_CANCELER_DISABLE						0
#define TIMER1_ICU_NOISE_CANCELER_ENABLE						1

#endif
/* --------------------------------- */

/* --------------------------------- */
/* OC1 pin configuration in CTC (Clear Timer on Compare Match) Mode or PWM Mode (Fast or Phase Correct) */

#if (TIMER1_MODE_SELECT == TIMER1_MODE_PWM) || (TIMER1_MODE_SELECT == TIMER1_MODE_CLEAR_TIMER_ON_COMPARE_MATCH)

/* OC1A pin Configuration */
#define PWM_CTC_OC1A_PORT_INDEX									GPIO_PORTD
#define PWM_CTC_OC1A_PIN_INDEX									GPIO_PIN5

/* OC1B pin Configuration */
#define PWM_CTC_OC1B_PORT_INDEX									GPIO_PORTD
#define PWM_CTC_OC1B_PIN_INDEX									GPIO_PIN4

#endif

/* --------------------------------- */


/* ----------------------------------------------------------------------------------- */
/* --------Macro functions declaration section---------- */

/* Enable/Disable TIMER1 */
#define TIMER1_DISABLE()										(_TCCR1B._CS1x = TIMER1_CLOCK_SOURCE_NONE)
#define TIMER1_ENABLE(prescaler)								(_TCCR1B._CS1x = prescaler)


/* ----------------------------------------------------------------------------------- */
/* -----user_defined data type declaration section------ */


/* --------------------------------- */
/* TIMER1 config structure */

#if TIMER1_MODE_SELECT == TIMER1_MODE_OVER_FLOW
/* TIMER1 is configured in Normal (OverFlow) Mode */

typedef struct{
	/* pointer to function to hold the function called in the APPLICATION layer when TIMER1 OverFlow occur if TIMER1 OverFlow Interrupt is enabled */
	void (* TIMER1_OVF_DefaultHandler)(void);
	/* pointer to function to hold the function called in the APPLICATION layer when (Falling/Rising) edge occur if TIMER1 ICU Interrupt is enabled */
	void (* TIMER1_ICU_DefaultHandler)(void);

	/* select the clock source to be used by the Timer/Counter >> @ref : timer1_clock_source_t */
	uint8 clock_source					:3;

	/* selects which edge on the Input Capture Pin (ICP1) that is used to trigger a capture event >> @ref : icu_edge_select_t */
	uint8 icu_edge_select 				:1;
	/* Enable/Disable Input Capture Noise Canceler >> @ref : Input Capture Noise Canceler Enable/Disable */
	uint8 icu_noise_canceler_enable		:1;

	/* TIMER1 OverFlow Interrupt Enable/Disable >> @ref : TIMER1 OverFlow Interrupt Enable/Disable */
	uint8 overflow_interrupt_enable		:1;
	/* TIMER1 Input Capture Interrupt Enable/Disable >> @ref : TIMER1 Input Capture Interrupt Enable/Disable */
	uint8 inputcapture_interrupt_enable	:1;

	/* Reserved */
	uint8								:1;
}timer1_overflow_config_t;

#elif TIMER1_MODE_SELECT == TIMER1_MODE_CLEAR_TIMER_ON_COMPARE_MATCH
/* TIMER1 is configured in CTC (Clear Timer on Compare Match) Mode */

typedef struct{
	/* pointer to function to hold the function called in the APPLICATION layer when TIMER1 Output Compare A Match occur if TIMER1 Output Compare A Match Interrupt is enabled */
	void (* TIMER1_CTC_A_DefaultHandler)(void);
	/* pointer to function to hold the function called in the APPLICATION layer when TIMER1 Output Compare B Match occur if TIMER1 Output Compare B Match Interrupt is enabled */
	void (* TIMER1_CTC_B_DefaultHandler)(void);

	/* select the timer mode (CTC Mode 1 or CTC Mode 2) >> @ref : timer1_mode_select_t */
	uint16 mode					:4;

	/* control the Output Compare pins (OC1A and OC1B) behavior >> @ref : timer1_ctc_mode_t */
	uint16 OC1A_mode			:2;
	uint16 OC1B_mode			:2;

	/* select the clock source to be used by the Timer/Counter >> @ref : timer1_clock_source_t */
	uint16 clock_source			:3;

	/* Timer/Counter1 Output Compare A Match Interrupt Enable/Disable >> @ref : TIMER1 Output Compare A Match Interrupt Enable/Disable */
	uint16 ctc_A_interrupt_en	:1;
	/* Timer/Counter1 Output Compare B Match Interrupt Enable/Disable >> @ref : TIMER1 Output Compare B Match Interrupt Enable/Disable */
	uint16 ctc_B_interrupt_en	:1;

	/* Reserved */
	uint16						:3;
}timer1_ctc_config_t;

#elif TIMER1_MODE_SELECT == TIMER1_MODE_PWM
/* TIMER1 is configured in PWM (Fast or Phase Correct) Mode */

typedef struct{
	/* select the timer mode (CTC Mode 1 or CTC Mode 2) >> @ref : timer1_mode_select_t */
	uint16 mode			:4;

	/* control the Output Compare pins (OC1A and OC1B) behavior >> @ref : timer1_fast_pwm_mode_t or @ref : timer1_phase_correct_pwm_mode_t */
	uint16 OC1A_mode	:2;
	uint16 OC1B_mode	:2;

	/* select the clock source to be used by the Timer/Counter >> @ref : timer1_clock_source_t */
	uint16 clock_source	:3;

	/* Reserved */
	uint16				:5;
}timer1_pwm_config_t;

#endif

/* --------------------------------- */
/* @ref : timer1_mode_select_t */

typedef enum{
	TIMER1_NORMAL_MODE = 0,							/* TOP = 0xFFFF */
	TIMER1_PHASE_CORRECT_PWM_8_BIT_MODE,			/* TOP = 0x00FF */
	TIMER1_PHASE_CORRECT_PWM_9_BIT_MODE,			/* TOP = 0x01FF */
	TIMER1_PHASE_CORRECT_PWM_10_BIT_MODE,			/* TOP = 0x03FF */
	TIMER1_CTC_MODE_1,								/* TOP = OCR1A */
	TIMER1_FAST_PWM_8_BIT_MODE,						/* TOP = 0x00FF */
	TIMER1_FAST_PWM_9_BIT_MODE,						/* TOP = 0x01FF */
	TIMER1_FAST_PWM_10_BIT_MODE,					/* TOP = 0x03FF */
	TIMER1_PHASE_AND_FREQUENCY_CORRECT_MODE_1,		/* TOP = ICR1 */
	TIMER1_PHASE_AND_FREQUENCY_CORRECT_MODE_2,		/* TOP = OCR1A */
	TIMER1_PHASE_CORRECT_MODE_1,					/* TOP = ICR1 */
	TIMER1_PHASE_CORRECT_MODE_2,					/* TOP = OCR1A */
	TIMER1_CTC_MODE_2,								/* TOP = ICR1 */
	TIMER1_FAST_PWM_MODE_1 = 14,					/* TOP = ICR1 */
	TIMER1_FAST_PWM_MODE_2							/* TOP = OCR1A */
}timer1_mode_select_t;

/* --------------------------------- */

/* @ref : timer1_clock_source_t */

typedef enum{
	TIMER1_CLOCK_SOURCE_NONE = 0,					/* No clock source (Timer/Counter stopped) */
	TIMER1_CLOCK_SOURCE_DIV_1,						/* TIMER Mode : clkI/O/(No prescaling) */
	TIMER1_CLOCK_SOURCE_DIV_8,						/* TIMER Mode : clkI/O/8 (From prescaler) */
	TIMER1_CLOCK_SOURCE_DIV_64,						/* TIMER Mode : clkI/O/64 (From prescaler) */
	TIMER1_CLOCK_SOURCE_DIV_256,					/* TIMER Mode : clkI/O/256 (From prescaler) */
	TIMER1_CLOCK_SOURCE_DIV_1024,					/* TIMER Mode : clkI/O/1024 (From prescaler) */
	TIMER1_CLOCK_SOURCE_T0_FALLING_EDGE,			/* COUNTER Mode : External clock source on T0 pin. Clock on falling edge */
	TIMER1_CLOCK_SOURCE_T0_RISING_EDGE				/* COUNTER Mode : External clock source on T0 pin. Clock on rising edge */
}timer1_clock_source_t;

/* --------------------------------- */

#if TIMER1_MODE_SELECT == TIMER1_MODE_OVER_FLOW
/* TIMER1 is configured in Normal (OverFlow) Mode */

/* @ref : icu_edge_select_t */
typedef enum{
	ICU_TRIGGER_FALLING_EDGE = 0,					/* Falling (negative) edge will trigger the capturer */
	ICU_TRIGGER_RISING_EDGE							/* Rising (positive) edge will trigger the capture */
}icu_edge_select_t;

#elif TIMER1_MODE_SELECT == TIMER1_MODE_CLEAR_TIMER_ON_COMPARE_MATCH
/* TIMER1 is configured in CTC (Clear Timer on Compare Match) Mode */

/* @ref : timer1_ctc_mode_t */
typedef enum{
	TIMER1_CTC_NORMAL_MODE = 0,						/* Normal port operation, OC1A/OC1B disconnected. */
	TIMER1_CTC_TOGGLE_OC1A_OC1B_ON_COMPARE_MATCH,	/* Toggle OC1A/OC1B on compare match */
	TIMER1_CTC_CLEAR_OC1A_OC1B_ON_COMPARE_MATCH,	/* Clear OC1A/OC1B on compare match (Set output to low level) */
	TIMER1_CTC_SET_OC1A_OC1B_ON_COMPARE_MATCH		/* Set OC1A/OC1B on compare match (Set output to high level) */
}timer1_ctc_mode_t;

#elif TIMER1_MODE_SELECT == TIMER1_MODE_PWM
/* TIMER1 is configured in PWM (Fast or Phase Correct) Mode */

/* @ref : timer1_fast_pwm_mode_t */
typedef enum{
	TIMER1_FAST_PWM_NORMAL_MODE = 0,				/* Normal port operation, OC1A/OC1B disconnected */
	TIMER1_FAST_PWM_TOGGLE_OC1A_MODE,				/* WGM13:0 = 15: Toggle OC1A on Compare Match, OC1B disconnected (normal port operation) For all other WGM13:0 settings, normal port operation, OC1A/OC1B disconnected */
	TIMER1_FAST_PWM_NON_INVERTING_MODE,				/* Clear OC1A/OC1B on compare match, set OC1A/OC1B at BOTTOM */
	TIMER1_FAST_PWM_INVERTING_MODE					/* Set OC1A/OC1B on compare match, clear OC1A/OC1B at BOTTOM */
}timer1_fast_pwm_mode_t;

/* @ref : timer1_phase_correct_pwm_mode_t */
typedef enum{
	TIMER1_PHASE_CORRECT_PWM_NORMAL_MODE = 0,		/* Normal port operation, OC1A/OC1B disconnected */
	TIMER1_PHASE_CORRECT_PWM_TOGGLE_OC1A_MODE,		/* WGM13:0 = 9 or 14: Toggle OC1A on Compare Match, OC1B disconnected (normal port operation) For all other WGM13:0 settings, normal port operation, OC1A/OC1B disconnected */
	TIMER1_PHASE_CORRECT_PWM_CLEAR_ON_UPCOUNT,		/* Clear OC1A/OC1B on compare match when up-counting. Set OC1A/OC1B on compare match when downcounting */
	TIMER1_PHASE_CORRECT_PWM_SET_ON_UPCOUNT			/* Set OC1A/OC1B on compare match when upcounting. Clear OC1A/OC1B on compare match when downcounting */
}timer1_phase_correct_pwm_mode_t;

#endif

/* --------------------------------- */


/* ----------------------------------------------------------------------------------- */
/* ------------functions declaration section------------ */


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
Std_ReturnType TIMER1_OVERFLOW_init(timer1_overflow_config_t *timer1_ovf_obj);


/**
 * @brief  Set the value of TIMER1 Timer/Counter1 Register
 * @param  (timer1_val) holds the value to be set in the TIMER1 Timer/Counter1 Register
 */
void TIMER1_setTimerValue(uint16 timer1_val);


/**
 * @brief  Selects which edge on the Input Capture Pin (ICP1) that is used to trigger a capture event
 * @param  (icu_edge_type) holds the edge Falling/Rising required
 */
void TIMER1_ICU_setEdgeDetectionType(icu_edge_select_t icu_edge_type);


/**
 * @brief  Get the value stored in TIMER1 Input Capture Register
 * @return TIMER1 Input Capture Register value
 */
uint16 TIMER1_ICU_getInputCaptureValue(void);


/**
 * @brief  Set TIMER1 Over Flow Call Back
 * @param  (TIMER1_OVF_Handler) holds the address of the call back function
 */
void TIMER1_OVF_setCallBack( void (* TIMER1_OVF_Handler)(void) );


/**
 * @brief  Set TIMER1 Input Capture Call Back
 * @param  (TIMER1_ICU_Handler) holds the address of the call back function
 */
void TIMER1_ICU_setCallBack( void (* TIMER1_ICU_Handler)(void) );


/**
 * @brief  Disable Timer1 Over Flow including ICU
 */
void TIMER1_OVF_deInit(void);


/**
 * @brief  Disable Timer1 ICU
 */
void TIMER1_ICU_deInit(void);


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
Std_ReturnType TIMER1_CTC_init(timer1_ctc_config_t *timer1_ctc_obj);


/**
 * @brief  Set the value of TIMER1 Timer/Counter1 Register
 * @param  (timer1_val) holds the value to be set in the TIMER1 Timer/Counter1 Register
 */
void TIMER1_TCNT1_setValue(uint16 timer1_val);


/**
 * @brief  Set the value of TIMER1 Output Compare A Register
 * @param  (timer1_val) holds the value to be set in the TIMER1 Output Compare A Register
 */
void TIMER1_OCR1A_setValue(uint16 timer1_val);


/**
 * @brief  Set the value of TIMER1 Output Compare B Register
 * @param  (timer1_val) holds the value to be set in the TIMER1 Output Compare B Register
 */
void TIMER1_OCR1B_setValue(uint16 timer1_val);


/**
 * @brief  Set the value of TIMER1 Input Compare Register
 * @param  (timer1_val) holds the value to be set in the TIMER1 Input Compare Register
 */
void TIMER1_ICR1_setValue(uint16 timer1_val);


/**
 * @brief  Set TIMER1 Output Compare A Match Call Back
 * @param  (TIMER1_CTC_A_Handler) holds the address of the call back function
 */
void TIMER1_CTC_A_setCallBack( void (* TIMER1_CTC_A_Handler)(void) );


/**
 * @brief  Set TIMER1 Output Compare B Match Call Back
 * @param  (TIMER1_CTC_B_Handler) holds the address of the call back function
 */
void TIMER1_CTC_B_setCallBack( void (* TIMER1_CTC_B_Handler)(void) );


/**
 * @brief  Disable Timer1 Output Compare Match
 */
void TIMER1_CTC_deInit(void);


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
Std_ReturnType TIMER1_PWM_init(timer1_pwm_config_t *pwm_obj);


/**
 * @brief  Set the value of TIMER1 Timer/Counter1 Register
 * @param  (timer1_val) holds the value to be set in the TIMER1 Timer/Counter1 Register
 */
void TIMER1_TCNT1_setValue(uint16 timer1_val);


/**
 * @brief  Set the value of TIMER1 Output Compare A Register
 * @param  (timer1_val) holds the value to be set in the TIMER1 Output Compare A Register
 */
void TIMER1_OCR1A_setValue(uint16 timer1_val);


/**
 * @brief  Set the value of TIMER1 Output Compare B Register
 * @param  (timer1_val) holds the value to be set in the TIMER1 Output Compare B Register
 */
void TIMER1_OCR1B_setValue(uint16 timer1_val);


/**
 * @brief  Set the value of TIMER1 Input Compare Register
 * @param  (timer1_val) holds the value to be set in the TIMER1 Input Compare Register
 */
void TIMER1_ICR1_setValue(uint16 timer1_val);


/**
 * @brief  generate the required duty cycle percentage of the generated PWM signal. Its value should be from 0 → 100
 * @param  (duty_cycle) the value of the duty cycle
 */
void TIMER1_PWM_Start(uint8 duty_cycle);


#endif


/* ----------------------------------------------------------------------------------- */
#endif /* _TIMER1_H_ */
