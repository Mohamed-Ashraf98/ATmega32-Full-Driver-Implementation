/*
 =========================================================================================
 Name        : buzzer.h
 Author      : Mohamed Ashraf El-Sayed
 Version     : 1.0.0
 Copyright   : Your copyright notice
 date        : TUE, Oct 31 2023
 time        :
 Description : BUZZER Driver Header file , Ansi-style
 =========================================================================================
*/

#ifndef _BUZZER_H_
#define _BUZZER_H_
/* ----------------------------------------------------------------------------------- */
/* ------------------Includes section------------------- */
#include "gpio.h"


/* ----------------------------------------------------------------------------------- */
/* --------------Macro declaration section-------------- */

/* BUZZER pin Configuration */

/* Buzzer Port Index */
#define BUZZER_PORT_INDEX			GPIO_PORTC

/* Buzzer Pin Index */
#define BUZZER_PIN_INDEX			GPIO_PIN7


/* ----------------------------------------------------------------------------------- */
/* ------------functions declaration section------------ */


/**
 * @brief  initialize Buzzer :
 * 			1- Setup the direction for the Buzzer pin as output pin through the GPIO driver
 * 			2- Turn off the Buzzer through the GPIO
 */
void Buzzer_init(void);


/**
 * @brief  Enable the Buzzer through the GPIO
 */
void Buzzer_on(void);


/**
 * @brief  Disable the Buzzer through the GPIO
 */
void Buzzer_off(void);


/* ----------------------------------------------------------------------------------- */
#endif /* _BUZZER_H_ */
