/*
 =========================================================================================
 Name        : adc.c
 Author      : Mohamed Ashraf El-Sayed
 Version     : 1.0.0
 Copyright   : Your copyright notice
 date        : TUE, Oct 10 2023
 time        :
 Description : ADC Driver Source file , Ansi-style
 =========================================================================================
*/

/* ----------------------------------------------------------------------------------- */
/* ------------------Includes section------------------- */
#include <avr/interrupt.h> 			/* For ADC ISR */

#include "adc.h"


/* ----------------------------------------------------------------------------------- */
/* -------------------Global section-------------------- */

/* create a pointer to function to hold the address of the call back function */
static void (* ADC_InterruptHandler)(void) = NULL_PTR;


/* ----------------------------------------------------------------------------------- */
/* ------------functions definition section------------- */


/**
 * @brief  initialize the ADC module
 * @param  (adc_obj)  pointer to the ADC object passed by reference
 * @return (l_status) status of the performed operation
 *              (E_NOT_OK)  operation failed
 *              (E_OK)      operation success
 */
Std_ReturnType ADC_init(adc_config_t *adc_obj)
{
	/* create a local variable to hold the status of the performed operation */
	Std_ReturnType l_status = ZERO_INIT;

	/* check if the address is valid or not */
	if(adc_obj == NULL_PTR)
	{
		/* NULL pointer is passed */

		l_status = E_NOK;		/* operation failed */
	}
	else
	{
		/* the address passed is valid */

		l_status = E_OK;		/* operation success */

		/* ADC init */

		/* --------------------------------- */
		/* Select Reference Voltage */
		_ADMUX._REFSx = adc_obj->voltage_reference_selection;

		/* Select the Channel */
		_ADMUX._MUXx = adc_obj->channel;
		/* --------------------------------- */

		/* --------------------------------- */
		/* Select ADC Left/Right Adjust */
		_ADMUX._ADLAR = adc_obj->adjust_result;
		/* --------------------------------- */

		/* --------------------------------- */
		/* enable/disable Auto Trigger */
		_ADCSRA._ADATE = adc_obj->auto_trigger_enable;

		/* Select Auto Trigger Source if enabled */
		if(adc_obj->auto_trigger_enable == ADC_AUTO_TRIGGER_ENABLE)
		{
			/* Auto Trigger Source is enabled */

			/* Select Auto Trigger Source */
			_SFIOR._ADTSx = adc_obj->auto_trigger_source;
		}
		else
		{
			/* Auto Trigger Source is disabled */

			/* Free Running Mode */
			_SFIOR._ADTSx = ADC_AUTO_TRIGGER_FREE_RUNNING_MODE;
		}
		/* --------------------------------- */

		/* --------------------------------- */
		/* Enable/Disable ADC Interrupt */
		_ADCSRA._ADIE = adc_obj->interrupt_enable;

		/* Set the Call Back Function if ADC Interrupt is enabled */
		if(adc_obj->interrupt_enable == ADC_INTERRUPT_ENABLE)
		{
			/* ADC Interrupt is enabled */
			ADC_InterruptHandler = adc_obj->ADC_DefaultHandler;
		}
		else
		{
			/* ADC Interrupt is disabled */
			ADC_InterruptHandler = NULL_PTR;
		}
		/* --------------------------------- */

		/* --------------------------------- */
		/* ADC Pre-scaler Select */
		_ADCSRA._ADPSx = adc_obj->prescaler;
		/* --------------------------------- */

		/* --------------------------------- */
		/* ADC Enable */
		ADC_ENABLE();
		/* --------------------------------- */
	}

	return l_status;
}


/**
 * @brief  read analog data from a certain ADC channel(polling mechanism)
 * @param  (channel)  the channel you want to read from
 * @return the value of the ADC Data Conversion
 */
uint16 ADC_readChannelBlocking(adc_channel_select_config_t channel)
{
	/* Select the Channel */
	_ADMUX._MUXx = channel;

	/* Start conversion by writing '1' to ADSC */
	ADC_START_CONVERSION();

	/* Wait for conversion to complete, ADIF becomes '1' */
	while( !(_ADCSRA._ADIF) );

	/* Clear ADC Interrupt Flag */
	/* Clear ADIF by writing '1' to it */
	ADC_CLEAR_INTERRUPT_FLAG();

	/* Read the digital value from the data register */
	return (_ADC.TwoBytes);
}


/**
 * @brief  read analog data from a certain ADC channel(interrupt mechanism)
 * @param  (channel)  the channel you want to read from
 */
void ADC_readChannelNonBlocking(adc_channel_select_config_t channel)
{
	/* Select the Channel */
	_ADMUX._MUXx = channel;

	/* Start conversion by writing '1' to ADSC */
	ADC_START_CONVERSION();
}


/* ----------------------------------------------------------------------------------- */
/* --------------------ISR section---------------------- */


/**
 * @brief  ADC ISR
 */
ISR(ADC_vect)
{
	/* Clear ADC Interrupt Flag */
	/* Clear ADIF by writing '1' to it */
	ADC_CLEAR_INTERRUPT_FLAG();

	/* check if the call back notification contains NULL or not */
	if(ADC_InterruptHandler)
	{
		/* Call Back */
		(*ADC_InterruptHandler)();
	}
	else{ /* Nothing */ }
}


/* ----------------------------------------------------------------------------------- */
