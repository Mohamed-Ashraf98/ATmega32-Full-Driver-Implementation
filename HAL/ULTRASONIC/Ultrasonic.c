/*
 =========================================================================================
 Name        : Ultrasonic.c
 Author      : Mohamed Ashraf El-Sayed
 Version     : 1.0.0
 Copyright   : Your copyright notice
 date        : Tue, Oct 17 2023
 time        :
 Description : ULTRASONIC Driver Source file , Ansi-style
 =========================================================================================
*/

/* ----------------------------------------------------------------------------------- */
/* ------------------Includes section------------------- */
#include "Ultrasonic.h"
#include "util/delay.h"				/* To use the delay functions */


/* ----------------------------------------------------------------------------------- */
/* -------------------Global section-------------------- */


#if TIMER1_MODE_SELECT == TIMER1_MODE_OVER_FLOW
/* create a pointer to function to hold the address of the call back function */
static void (* ULTRASONIC_InterruptHandler)(void) = NULL_PTR;


/* create Ultrasonic Trigger pin Object of type gpio_config_t */
gpio_config_t Ultrasonic_TriggerPin;


/* create a static variable to hold the value of the pre-scaler of TIMER1 */
static uint16 Ultrasonic_prescaler = 0;

#endif


/* ----------------------------------------------------------------------------------- */
/* ------------functions definition section------------- */


#if TIMER1_MODE_SELECT == TIMER1_MODE_OVER_FLOW
/**
 * @brief  initialize the Ultrasonic :
 * 				1- Initialize the ICU Driver
 * 				2- Setup the ICU Call Back function
 * 				3- Setup the Ultrasonic Call Back function
 * 				4- Setup the direction of the Trigger pin as Output through the GPIO Driver
 * @param  (ultrasonic_obj)  pointer to the Ultrasonic object passed by reference
 * @return (l_status) 	     status of the performed operation
 *              (E_NOT_OK)  	operation failed
 *              (E_OK)      	operation success
 */
Std_ReturnType Ultrasonic_init(ultrasonic_config_t *ultrasonic_obj)
{
	/* create a local variable to hold the status of the performed operation */
	Std_ReturnType l_status = ZERO_INIT;

	/* check if the address is valid or not */
	if(ultrasonic_obj == NULL_PTR)
	{
		/* NULL pointer is passed */

		l_status = E_NOK;		/* operation failed */
	}
	else
	{
		/* the address passed is valid */

		l_status = E_OK;		/* operation success */

		/* Ultasonic Init */

		/* Initialize the value of the pre-scaler of TIMER1 */
		switch(ultrasonic_obj->icu_cfg.clock_source)
		{
			case TIMER1_CLOCK_SOURCE_DIV_1		:	Ultrasonic_prescaler = 1;		break;
			case TIMER1_CLOCK_SOURCE_DIV_8		:	Ultrasonic_prescaler = 8;		break;
			case TIMER1_CLOCK_SOURCE_DIV_64		:	Ultrasonic_prescaler = 64;		break;
			case TIMER1_CLOCK_SOURCE_DIV_256	:	Ultrasonic_prescaler = 256;		break;
			case TIMER1_CLOCK_SOURCE_DIV_1024	:	Ultrasonic_prescaler = 1024;	break;
			default		:	/* Nothing */	break;
		}

		/* Setup the ICU Call Back function */
		/* Ultrasonic_edgeProcessing is the call back function called by the ICU Driver */
		ultrasonic_obj->icu_cfg.TIMER1_ICU_DefaultHandler = Ultrasonic_edgeProcessing;


		/* Initialize the ICU Driver */
		l_status = TIMER1_OVERFLOW_init( &(ultrasonic_obj->icu_cfg) );


		/* Setup the Ultrasonic Call Back function */
		ULTRASONIC_InterruptHandler = ultrasonic_obj->ULTRASONIC_DefaultHandler;


		/* Setup the direction of the Trigger pin as Output through the GPIO Driver */
		l_status = GPIO_setupPinDirection(&Ultrasonic_TriggerPin);
		GPIO_writePin( &Ultrasonic_TriggerPin , GPIO_LOW );
	}

	return l_status;
}


/**
 * @brief  Send the Trigger pulse to the Ultrasonic
 */
void Ultrasonic_Trigger(void)
{
	/* create a local variable to hold the number of ticks needed to count 10us */
	uint32 counts_10us;

	/* write logic 1 on the Trigger pin for 10us */
	GPIO_writePin( &Ultrasonic_TriggerPin , GPIO_HIGH );

	/* calculate the number of ticks needed to count 10us */
	counts_10us = 10.0 / PERIOD_US(Ultrasonic_prescaler);

	/* update the Timer/Counter1 Register to start count from it and Over Flow after 10us */
	TIMER1_setTimerValue( (65536 - counts_10us) );

	/* wait for Over Flow to happen after 10us */
	while( !(_TIFR._TOV1) );

	/* Clear the Timer/Counter1 Over Flow Flag by writing '1' to it's bit location */
	_TIFR._TOV1 = SET;

	/* write logic 0 on the Trigger pin after 10us pulse */
	GPIO_writePin( &Ultrasonic_TriggerPin , GPIO_LOW );
}


/**
 * @brief  	- Send the Trigger pulse using Ultrasonic_Trigger function
 * 			- Start the measurements by the ICU from this moment
 * @param  (counts) holds the number of ticks of Timer/Counter1 during the high pulse of the echo pin captured by ICU
 * @return the returned distance in Centimeter
 */
uint16 Ultrasonic_readDistance(uint16 counts)
{
	/* calculate the distance measured by the Ultrasonic in Centimeter */
	uint16 distance = counts / ( 1000.0 / (17.0 * PERIOD_US(Ultrasonic_prescaler)) );

	/* wait for 60ms before sending another Trigger Pulse */
	_delay_ms(60);

	/* send another Trigger Pulse after 60ms */
	Ultrasonic_Trigger();

	/* Configure the Input Capture Pin to detect Rising Edge */
	TIMER1_ICU_setEdgeDetectionType(ICU_TRIGGER_RISING_EDGE);

	return distance;
}


/**
 * @brief  	- this is the call back function called by the ICU Driver
 * 			- this is used to calculate the high time (pulse time) generated by the Ultrasonic sensor
 */
void Ultrasonic_edgeProcessing(void)
{
	/* check if the call back notification contains NULL or not */
	if(ULTRASONIC_InterruptHandler)
	{
		/* Call Back */
		(*ULTRASONIC_InterruptHandler)();
	}
	else{ /* Nothing */ }
}
#endif


/* ----------------------------------------------------------------------------------- */
