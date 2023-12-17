/*
 =========================================================================================
 Name        : lm35_sensor.h
 Author      : Mohamed Ashraf El-Sayed
 Version     : 1.0.0
 Copyright   : Your copyright notice
 date        : TUE, Oct 10 2023
 time        :
 Description : LM35 Sensor Driver Header file , Ansi-style
 =========================================================================================
*/

#ifndef _LM35_SENSOR_H_
#define _LM35_SENSOR_H_
/* ----------------------------------------------------------------------------------- */
/* ------------------Includes section------------------- */
#include "std_types.h"


/* ----------------------------------------------------------------------------------- */
/* --------------Macro declaration section-------------- */


/* --------------------------------- */
/* @ref : LM35 Sensor Channel Select */

#define LM35_SENSOR_CHANNEL_ID			LM35_ADC_CHANNEL2

/* --------------------------------- */

/* LM35 Sensor Maximum Voltage */
#define LM35_SENSOR_MAX_VOLTAGE			1.5

/* LM35 Sensor Maximum Temperature */
#define LM35_SENSOR_MAX_TEMPERATURE		150

/* --------------------------------- */


/* ----------------------------------------------------------------------------------- */
/* -----user_defined data type declaration section------ */

/* @ref : LM35 Sensor Channel ID */
typedef enum{
	LM35_ADC_CHANNEL0 = 0,				/* connected to ADC Channel 0 */
	LM35_ADC_CHANNEL1,					/* connected to ADC Channel 1 */
	LM35_ADC_CHANNEL2,					/* connected to ADC Channel 2 */
	LM35_ADC_CHANNEL3,					/* connected to ADC Channel 3 */
	LM35_ADC_CHANNEL4,					/* connected to ADC Channel 4 */
	LM35_ADC_CHANNEL5,					/* connected to ADC Channel 5 */
	LM35_ADC_CHANNEL6,					/* connected to ADC Channel 6 */
	LM35_ADC_CHANNEL7					/* connected to ADC Channel 7 */
}lm35_channel_select_t;


/* ----------------------------------------------------------------------------------- */
/* ------------functions declaration section------------ */


/**
 * @brief  calculate the temperature from the ADC digital value
 * @return the temperature value
 */
uint8 LM35_getTemperature(void);


/* ----------------------------------------------------------------------------------- */
#endif /* _LM35_SENSOR_H_ */
