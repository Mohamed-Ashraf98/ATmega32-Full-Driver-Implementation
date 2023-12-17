/*
 =========================================================================================
 Name        : lm35_sensor.c
 Author      : Mohamed Ashraf El-Sayed
 Version     : 1.0.0
 Copyright   : Your copyright notice
 date        : TUE, Oct 10 2023
 time        :
 Description : LM35 Sensor Driver Source file , Ansi-style
 =========================================================================================
*/

/* ----------------------------------------------------------------------------------- */
/* ------------------Includes section------------------- */
#include "lm35_sensor.h"
#include "adc.h"


/* ----------------------------------------------------------------------------------- */
/* ------------functions definition section------------- */


/**
 * @brief  calculate the temperature from the ADC digital value
 * @return the temperature value
 */
uint8 LM35_getTemperature(void)
{
	/* create a local variable to hold the temperature value */
	uint8 temp_val = ZERO_INIT;

	/* create a local variable to hold the value of the ADC Conversion Data */
	uint16 adc_val = ZERO_INIT;

	/* Read ADC channel where the temperature sensor is connected */
	adc_val = ADC_readChannelBlocking(LM35_SENSOR_CHANNEL_ID);

	/* Calculate the temperature from the ADC value*/
	temp_val = (uint8)( ( (uint32)adc_val * LM35_SENSOR_MAX_TEMPERATURE * ADC_REF_VOLTAGE_VALUE  )/( LM35_SENSOR_MAX_VOLTAGE * ADC_MAXIMUM_VALUE ) );

	return temp_val;
}


/* ----------------------------------------------------------------------------------- */
