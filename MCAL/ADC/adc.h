/*
 =========================================================================================
 Name        : adc.h
 Author      : Mohamed Ashraf El-Sayed
 Version     : 1.0.0
 Copyright   : Your copyright notice
 date        : TUE, Oct 10 2023
 time        :
 Description : ADC Driver Header file , Ansi-style
 =========================================================================================
*/

#ifndef _ADC_H_
#define _ADC_H_
/* ----------------------------------------------------------------------------------- */
/* ------------------Includes section------------------- */
#include "ATmega32.h"


/* ----------------------------------------------------------------------------------- */
/* --------------Macro declaration section-------------- */


/* --------------------------------- */
/* ADC Configurations for any Sensor Calculation */

/* ADC (10-bit resolution) */
#define ADC_MAXIMUM_VALUE    			1023

/* ADC Reference Voltage value */
#define ADC_REF_VOLTAGE_VALUE			2.56

/* --------------------------------- */
/* @ref : ADC Left Adjust Result */

#define ADC_LEFT_ADJUST_RESULT_DISABLE	0
#define ADC_LEFT_ADJUST_RESULT_ENABLE	1

/* --------------------------------- */
/* @ref : ADC Interrupt Enable/Disable */

#define ADC_INTERRUPT_DISABLE			0
#define ADC_INTERRUPT_ENABLE			1

/* --------------------------------- */
/* @ref : ADC Auto Trigger Enable/Disable */

#define ADC_AUTO_TRIGGER_DISABLE		0
#define ADC_AUTO_TRIGGER_ENABLE			1

/* --------------------------------- */


/* ----------------------------------------------------------------------------------- */
/* --------Macro functions declaration section---------- */

/* ADC Enable/Disable */
#define ADC_DISABLE()					(_ADCSRA._ADEN = RESET)
#define ADC_ENABLE()					(_ADCSRA._ADEN = SET)

/* Clear ADC Interrupt Flag */
#define ADC_CLEAR_INTERRUPT_FLAG()		(_ADCSRA._ADIF = SET)

/* ADC Start Conversion */
#define ADC_START_CONVERSION()			(_ADCSRA._ADSC = SET)


/* ----------------------------------------------------------------------------------- */
/* -----user_defined data type declaration section------ */

/* ADC config structure */
typedef struct{
	/* ADC Call Back if Interrupt is enabled */
	void (* ADC_DefaultHandler)(void);
	/* select the voltage reference for the ADC, >> @ref : adc_voltage_reference_select_t */
	uint16 voltage_reference_selection :2;
	/* selects which combination of analog inputs are connected to the ADC >> @ref : adc_channel_select_config_t */
	uint16 channel					   :3;
	/* determine the division factor between the XTAL frequency and the input clock to the ADC >> @ref : adc_prescaler_t */
	uint16 prescaler				   :3;
	/* ADC Conversion Complete Interrupt is activated >> @ref : ADC Interrupt Enable/Disable */
	uint16 interrupt_enable			   :1;
	/* affects the presentation of the ADC conversion result in the ADC Data Register >> @ref : ADC Left Adjust Result */
	uint16 adjust_result 			   :1;
	/* ADC will start a conversion on a positive edge of the selected trigger signal >> @ref : ADC Auto Trigger Enable/Disable */
	uint16 auto_trigger_enable		   :1;
	/* selects which source will trigger an ADC conversion >> @ref : adc_auto_trigger_source_config_t */
	uint16 auto_trigger_source		   :3;
	/* Reserved */
	uint16							   :2;
}adc_config_t;


/* @ref : adc_voltage_reference_select_t */
typedef enum{
	ADC_AREF_VOLTAGE_SELECT = 0,						/* ADC Reference Voltage value = 0v --> 5v connected to it */
	ADC_AVCC_VOLTAGE_SELECT,							/* ADC Reference Voltage value = 5v */
	ADC_INTERNAL_VOLTAGE_SELECT = 3						/* ADC Reference Voltage value = 2.56v (Internal 2.56 Voltage Reference) */
}adc_voltage_reference_select_t;

/* @ref : adc_channel_select_config_t */
typedef enum{
	ADC_CHANNEL0 = 0,									/* ADC Channel 0 */
	ADC_CHANNEL1,										/* ADC Channel 1 */
	ADC_CHANNEL2,										/* ADC Channel 2 */
	ADC_CHANNEL3,										/* ADC Channel 3 */
	ADC_CHANNEL4,										/* ADC Channel 4 */
	ADC_CHANNEL5,										/* ADC Channel 5 */
	ADC_CHANNEL6,										/* ADC Channel 6 */
	ADC_CHANNEL7										/* ADC Channel 7 */
}adc_channel_select_config_t;

/* @ref : adc_prescaler_t */
typedef enum{
	ADC_FCPU_DIV_2 = 1,									/* F_CPU is divided by 2 */
	ADC_FCPU_DIV_4,										/* F_CPU is divided by 4 */
	ADC_FCPU_DIV_8,										/* F_CPU is divided by 8 */
	ADC_FCPU_DIV_16,									/* F_CPU is divided by 16 */
	ADC_FCPU_DIV_32,									/* F_CPU is divided by 32 */
	ADC_FCPU_DIV_64,									/* F_CPU is divided by 64 */
	ADC_FCPU_DIV_128									/* F_CPU is divided by 128 */
}adc_prescaler_t;

/* @ref : adc_auto_trigger_source_config_t */
typedef enum{
	ADC_AUTO_TRIGGER_FREE_RUNNING_MODE = 0,				/* Free Running mode */
	ADC_AUTO_TRIGGER_ANALOG_COMPARATOR,					/* Analog Comparator */
	ADC_AUTO_TRIGGER_EXTERNAL_INTERRUPT_REQUEST_0,		/* External Interrupt Request 0 */
	ADC_AUTO_TRIGGER_TIMER0_COMPARE_MATCH,				/* Timer/Counter0 Compare Match */
	ADC_AUTO_TRIGGER_TIMER0_OVERFLOW,					/* Timer/Counter0 Overflow */
	ADC_AUTO_TRIGGER_TIMER1_COMPARE_MATCH_B,			/* Timer/Counter1 Compare Match B */
	ADC_AUTO_TRIGGER_TIMER1_OVERFLOW,					/* Timer/Counter1 Overflow */
	ADC_AUTO_TRIGGER_TIMER1_CAPTURE_EVENT				/* Timer/Counter1 Capture Event */
}adc_auto_trigger_source_config_t;


/* ----------------------------------------------------------------------------------- */
/* ------------functions declaration section------------ */


/**
 * @brief  initialize the ADC module
 * @param  (adc_obj)  pointer to the ADC object passed by reference
 * @return (l_status) status of the performed operation
 *              (E_NOT_OK)  operation failed
 *              (E_OK)      operation success
 */
Std_ReturnType ADC_init(adc_config_t *adc_obj);


/**
 * @brief  read analog data from a certain ADC channel(polling mechanism)
 * @param  (channel)  the channel you want to read from
 * @return the value of the ADC Data Conversion
 */
uint16 ADC_readChannelBlocking(adc_channel_select_config_t channel);


/**
 * @brief  read analog data from a certain ADC channel(interrupt mechanism)
 * @param  (channel)  the channel you want to read from
 */
void ADC_readChannelNonBlocking(adc_channel_select_config_t channel);


/* ----------------------------------------------------------------------------------- */
#endif /* _ADC_H_ */
