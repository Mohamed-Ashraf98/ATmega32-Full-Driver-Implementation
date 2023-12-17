/*
 =========================================================================================
 Name        : keypad.c
 Author      : Mohamed Ashraf El-Sayed
 Version     : 1.0.0
 Copyright   : Your copyright notice
 date        : Sun, Sep 24 2023
 time        :
 Description : KEYPAD Driver Source file , Ansi-style
 =========================================================================================
*/

/* ----------------------------------------------------------------------------------- */
/* ------------------Includes section------------------- */
#include "keypad.h"
#include "util/delay.h"				/* To use the delay functions */


/* ----------------------------------------------------------------------------------- */
/* -------------------Global section-------------------- */

/* --------------------------------- */
/* it will be active if the 4x3 mode is activated */
/* NOTE: 4x3 mode and 4x4 mode can't be activated at the same time */

#if KEYPAD_CONFIG_4X3_MODE

/* it will be active if the Proteus mode is activated */
/* NOTE: Proteus mode and Eta32_mini mode can't be activated at the same time */
#if KEYPAD_CONFIG_PROTEUS
const static uint8 keypad_pattern[KEYPAD_ROW_MAX_SIZE][KEYPAD_COLUMN_MAX_SIZE] =	{	{'1' , '2' , '3'},
																						{'4' , '5' , '6'},
																						{'7' , '8' , '9'},
																						{'*' , '0' , '#'}
																					};
#endif

/* it will be active if the Eta32_mini mode is activated */
/* NOTE: Proteus mode and Eta32_mini mode can't be activated at the same time */
#if KEYPAD_CONFIG_ETA32_KIT
const static uint8 keypad_pattern[KEYPAD_ROW_MAX_SIZE][KEYPAD_COLUMN_MAX_SIZE] =	{	{0 , 1 , 2 },
																						{3 , 4 , 5 },
																						{6 , 7 , 8 },
																						{9 ,10 , 11}
																					};
#endif

#endif
/* --------------------------------- */
/* --------------------------------- */
/* it will be active if the 4x4 mode is activated */
/* NOTE: 4x3 mode and 4x4 mode can't be activated at the same time */

#if KEYPAD_CONFIG_4X4_MODE

/* it will be active if the Proteus mode is activated */
/* NOTE: Proteus mode and Eta32_mini mode can't be activated at the same time */
#if KEYPAD_CONFIG_PROTEUS
const static uint8 keypad_pattern[KEYPAD_ROW_MAX_SIZE][KEYPAD_COLUMN_MAX_SIZE] =	{	{'7' , '8' , '9' , '/'},
																						{'4' , '5' , '6' , '*'},
																						{'1' , '2' , '3' , '-'},
																						{'C' , '0' , '=' , '+'}
																					};
#endif

/* it will be active if the Eta32_mini mode is activated */
/* NOTE: Proteus mode and Eta32_mini mode can't be activated at the same time */
#if KEYPAD_CONFIG_ETA32_KIT
const static uint8 keypad_pattern[KEYPAD_ROW_MAX_SIZE][KEYPAD_COLUMN_MAX_SIZE] =	{	{0 , 1 , 2 , 3 },
																						{4 , 5 , 6 , 7 },
																						{8 , 9 , 10, 11},
																						{12, 13, 14, 15}
																					};
#endif

#endif
/* --------------------------------- */


/* ----------------------------------------------------------------------------------- */
/* ------------functions definition section------------- */


/**
 * @brief  get the value of the pressed button in the keypad
 * @param  (keypad_obj) 	pointer to the keypad object passed by reference
 * @return (keypad_value)   holds the value of the pressed button in the keypad
 */
uint8 KEYPAD_getPressedKey(keypad_config_t *keypad_obj)
{
	uint8 keypad_val = '$';
	/* --------------------------------- */
	/* create a local variables to hold rows/columns iterations */

	uint8 row_counter = ZERO_INIT;
	uint8 column_counter = ZERO_INIT;

	/* --------------------------------- */
	/* set all the pins connected to the keypad to be input */

	/* rows */
	GPIO_setupPinDirection( &(keypad_obj->rows[KEYPAD_ROW_1]) );
	GPIO_setupPinDirection( &(keypad_obj->rows[KEYPAD_ROW_2]) );
	GPIO_setupPinDirection( &(keypad_obj->rows[KEYPAD_ROW_3]) );
	GPIO_setupPinDirection( &(keypad_obj->rows[KEYPAD_ROW_4]) );

	/* columns */
	GPIO_setupPinDirection( &(keypad_obj->columns[KEYPAD_COLUMN_1]) );
	GPIO_setupPinDirection( &(keypad_obj->columns[KEYPAD_COLUMN_2]) );
	GPIO_setupPinDirection( &(keypad_obj->columns[KEYPAD_COLUMN_3]) );
	/* column 4 in 4x4 mode */
	#if KEYPAD_CONFIG_4X4_MODE
	GPIO_setupPinDirection( &(keypad_obj->columns[KEYPAD_COLUMN_4]) );
	#endif

	/* --------------------------------- */
	/* loop for rows */
	for(row_counter = 0; row_counter < KEYPAD_ROW_MAX_SIZE; row_counter++)
	{
		/* set the current row pin to be output */
		keypad_obj->rows[row_counter].mode = GPIO_MODE_OUTPUT;
		GPIO_setupPinDirection( &(keypad_obj->rows[row_counter]) );

		/* Set/Clear the row output pin */
		GPIO_writePin( &(keypad_obj->rows[row_counter]) , KEYPAD_PRESSED );

		/* loop for columns */
		for(column_counter = 0; column_counter < KEYPAD_COLUMN_MAX_SIZE; column_counter++)
		{
			/* Check if the button is pressed in this column */
			if( GPIO_readPin( &(keypad_obj->columns[column_counter]) ) == KEYPAD_PRESSED )
			{
				/* keypad button is pressed */
				/* wait for 30ms for button de-bounce and check if the button is still pressed */
				_delay_ms(30);

				if( GPIO_readPin( &(keypad_obj->columns[column_counter]) ) == KEYPAD_PRESSED )
				{
					/* keypad button is pressed */
					keypad_val = keypad_pattern[row_counter][column_counter];
				}

				/* single pressing */
				while(GPIO_readPin( &(keypad_obj->columns[column_counter]) ) == KEYPAD_PRESSED);

				/* keypad button is pressed */
				return keypad_val;
			}
		}

		/* set the current row pin again to be input */
		keypad_obj->rows[row_counter].mode = GPIO_MODE_INPUT_WITHOUT_INTERNAL_PULL_UP_RES;
		GPIO_setupPinDirection( &(keypad_obj->rows[row_counter]) );

		/* Add small delay to fix CPU load issue in proteus */
		_delay_ms(5);
	}

	/* --------------------------------- */
	/* return Keypad Idle when no button is pressed */
	return keypad_val;
}


/* ----------------------------------------------------------------------------------- */
