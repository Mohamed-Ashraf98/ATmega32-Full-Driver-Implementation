/*
 =========================================================================================
 Name        : keypad.h
 Author      : Mohamed Ashraf El-Sayed
 Version     : 1.0.0
 Copyright   : Your copyright notice
 date        : Sun, Sep 24 2023
 time        :
 Description : KEYPAD Driver Header file , Ansi-style
 =========================================================================================
*/

#ifndef _KEYPAD_H_
#define _KEYPAD_H_
/* ----------------------------------------------------------------------------------- */
/* ------------------Includes section------------------- */
#include "gpio.h"


/* ----------------------------------------------------------------------------------- */
/* --------------Macro declaration section-------------- */

/* --------------------------------- */
/* keypad configurations for number of rows and columns mode(4x3 mode or 4x4 mode) */
/* NOTE: one of them must be enabled and the other is disabled */

#define KEYPAD_CONFIG_4X3_MODE		DISABLE
#define KEYPAD_CONFIG_4X4_MODE		ENABLE

/* --------------------------------- */
/* size of rows and columns of keypad */

#define KEYPAD_ROW_MAX_SIZE			4

#if KEYPAD_CONFIG_4X3_MODE
#define KEYPAD_COLUMN_MAX_SIZE		3
#endif
#if KEYPAD_CONFIG_4X4_MODE
#define KEYPAD_COLUMN_MAX_SIZE		4
#endif

/* --------------------------------- */
/* keypad configurations for the type of button pattern used(Proteus or Eta_32mini kit) */
/* NOTE: one of them must be enabled and the other is disabled */

#define KEYPAD_CONFIG_PROTEUS		ENABLE
#define KEYPAD_CONFIG_ETA32_KIT		DISABLE

/* --------------------------------- */
/* keypad configurations for connection type(Pull_up or Pull_down) */
/* NOTE: one of them must be enabled and the other is disabled */
/* NOTE: ETA32MINI KIT is used with KEYPAD_CONFIG_PULL_UP only */
/* NOTE: Proteus is used with both of them */

#if KEYPAD_CONFIG_ETA32_KIT
#define KEYPAD_CONFIG_PULL_UP		ENABLE
#define KEYPAD_CONFIG_PULL_DOWN		DISABLE

#else
#define KEYPAD_CONFIG_PULL_UP		ENABLE
#define KEYPAD_CONFIG_PULL_DOWN		DISABLE
#endif

/* --------------------------------- */
/* keypad configurations for (pressed/released) pull-up/pull-down */

#if KEYPAD_CONFIG_PULL_UP
#define KEYPAD_PRESSED				GPIO_LOW
#define KEYPAD_RELEASED				GPIO_HIGH
#endif

#if KEYPAD_CONFIG_PULL_DOWN
#define KEYPAD_PRESSED				GPIO_HIGH
#define KEYPAD_RELEASED				GPIO_LOW
#endif

/* --------------------------------- */
/* Keypad idle (normal) return when no button is pressed in the Keypad */

#define KEYPAD_IDLE_RETURN			'$'

/* --------------------------------- */
/* keypad rows/columns index */

#define KEYPAD_ROW_1				0
#define KEYPAD_ROW_2				1
#define KEYPAD_ROW_3				2
#define KEYPAD_ROW_4				3
#define KEYPAD_COLUMN_1				0
#define KEYPAD_COLUMN_2				1
#define KEYPAD_COLUMN_3				2
#if KEYPAD_CONFIG_4X4_MODE
#define KEYPAD_COLUMN_4				3
#endif

/* --------------------------------- */


/* ----------------------------------------------------------------------------------- */
/* -----user_defined data type declaration section------ */

/* keypad config structure */
typedef struct{
	/* hold the information of the row pins >> @ref : gpio.h */
	gpio_config_t rows[KEYPAD_ROW_MAX_SIZE];
	/* hold the information of the column pins >> @ref : gpio.h */
	gpio_config_t columns[KEYPAD_COLUMN_MAX_SIZE];
}keypad_config_t;


/* ----------------------------------------------------------------------------------- */
/* ------------functions declaration section------------ */


/**
 * @brief  get the value of the pressed button in the keypad
 * @param  (keypad_obj) 	pointer to the keypad object passed by reference
 * @return (keypad_value)   holds the value of the pressed button in the keypad
 */
uint8 KEYPAD_getPressedKey(keypad_config_t *keypad_obj);


/* ----------------------------------------------------------------------------------- */
#endif /* _KEYPAD_H_ */
