/*
 =========================================================================================
 Name        : buzzer.c
 Author      : Mohamed Ashraf El-Sayed
 Version     : 1.0.0
 Copyright   : Your copyright notice
 date        : TUE, Oct 31 2023
 time        :
 Description : BUZZER Driver Source file , Ansi-style
 =========================================================================================
*/

/* ----------------------------------------------------------------------------------- */
/* ------------------Includes section------------------- */
#include "buzzer.h"


/* ----------------------------------------------------------------------------------- */
/* -------------------Global section-------------------- */

/* create a static variable to hold the information of the Buzzer */
static gpio_config_t buzzer_pin;


/* ----------------------------------------------------------------------------------- */
/* ------------functions definition section------------- */


/**
 * @brief  initialize Buzzer :
 * 			1- Setup the direction for the Buzzer pin as output pin through the GPIO driver
 * 			2- Turn off the Buzzer through the GPIO
 */
void Buzzer_init(void)
{
	/* 1- Setup the direction for the Buzzer pin as output pin through the GPIO driver */

	/* Buzzer pin information */
	buzzer_pin.port = BUZZER_PORT_INDEX;
	buzzer_pin.pin = BUZZER_PIN_INDEX;
	buzzer_pin.mode = GPIO_MODE_OUTPUT;

	/* setup the direction */
	GPIO_setupPinDirection(&buzzer_pin);

	/* 2- Turn off the Buzzer through the GPIO */
	Buzzer_off();
}


/**
 * @brief  Enable the Buzzer through the GPIO
 */
void Buzzer_on(void)
{
	GPIO_writePin(&buzzer_pin,GPIO_HIGH);
}


/**
 * @brief  Disable the Buzzer through the GPIO
 */
void Buzzer_off(void)
{
	GPIO_writePin(&buzzer_pin,GPIO_LOW);
}


/* ----------------------------------------------------------------------------------- */
