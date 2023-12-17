/*
 =========================================================================================
 Name        : lcd.c
 Author      : Mohamed Ashraf El-Sayed
 Version     : 1.0.0
 Copyright   : Your copyright notice
 date        : Sun, Oct 4 2023
 time        :
 Description : LCD Driver Source file , Ansi-style
 =========================================================================================
*/

/* ----------------------------------------------------------------------------------- */
/* ------------------Includes section------------------- */
#include <stdlib.h>					/* for itoa C function */

#include "lcd.h"
#include "util/delay.h"				/* To use the delay functions */
#include "common_macros.h"


/* ----------------------------------------------------------------------------------- */
/* ------------functions definition section------------- */


/**
 * @brief  Initialize the LCD :
 *				1- Setup the LCD pins directions by use the GPIO driver
 *				2- Setup the LCD Data Mode 4-bits or 8-bits
 * @param  (lcd_obj)  pointer to the LCD object passed by reference
 * @return (l_status) status of the performed operation
 *              (E_NOT_OK)  operation failed
 *              (E_OK)      operation success
 */
Std_ReturnType LCD_init(lcd_config_t *lcd_obj)
{
	/* create a local variable to hold the status of the performed operation */
	Std_ReturnType l_status = ZERO_INIT;

	/* check if the address is valid or not */
	if(lcd_obj == NULL_PTR)
	{
		/* NULL pointer is passed */

	    l_status = E_NOK;		/* operation failed */
	}
	else
	{
		/* the address passed is valid */

		l_status = E_OK;		/* operation success */

		/* initialize the LCD */

		/* Configure the direction for RS and E pins as output pins */
		GPIO_setupPinDirection( &(lcd_obj->register_select) );
		GPIO_setupPinDirection( &(lcd_obj->enable) );
		/* Configure the direction for Read/Write pin as output pin if needed */
		#if LCD_CONFIG_READ_WRITE_PIN
		GPIO_setupPinDirection( &(lcd_obj->read_write) );
		#endif

		_delay_ms(20);			/* LCD Power ON delay always > 15ms */

		/* 4-bit mode */
		#if LCD_CONFIG_4_BIT_MODE
		/* set the 4 pins of the data bus to be output */
		GPIO_setupPinDirection( &(lcd_obj->data_bus[LCD_DATA_BUS_1]) );
		GPIO_setupPinDirection( &(lcd_obj->data_bus[LCD_DATA_BUS_2]) );
		GPIO_setupPinDirection( &(lcd_obj->data_bus[LCD_DATA_BUS_3]) );
		GPIO_setupPinDirection( &(lcd_obj->data_bus[LCD_DATA_BUS_4]) );

		/* LCD 4 bit Mode Initialization */
		LCD_sendCommand(lcd_obj,LCD_TWO_LINES_FOUR_BITS_MODE_INIT1);
		LCD_sendCommand(lcd_obj,LCD_TWO_LINES_FOUR_BITS_MODE_INIT2);

		/* LCD 4-bit mode, 2 row mode, 5x8 dot matrix */
		LCD_sendCommand(lcd_obj,LCD_4_bit_2_row_5x8);
		#endif

		/* 8-bit mode */
		#if LCD_CONFIG_8_BIT_MODE
		/* set the 8 pins of the data bus to be output */
		GPIO_setupPinDirection( &(lcd_obj->data_bus[LCD_DATA_BUS_1]) );
		GPIO_setupPinDirection( &(lcd_obj->data_bus[LCD_DATA_BUS_2]) );
		GPIO_setupPinDirection( &(lcd_obj->data_bus[LCD_DATA_BUS_3]) );
		GPIO_setupPinDirection( &(lcd_obj->data_bus[LCD_DATA_BUS_4]) );
		GPIO_setupPinDirection( &(lcd_obj->data_bus[LCD_DATA_BUS_5]) );
		GPIO_setupPinDirection( &(lcd_obj->data_bus[LCD_DATA_BUS_6]) );
		GPIO_setupPinDirection( &(lcd_obj->data_bus[LCD_DATA_BUS_7]) );
		GPIO_setupPinDirection( &(lcd_obj->data_bus[LCD_DATA_BUS_8]) );

		/* LCD 8-bit mode, 2 row mode, 5x8 dot matrix */
		LCD_sendCommand(lcd_obj,LCD_8_bit_2_row_5x8);
		#endif

		/* display on underline cursor off blink off */
		LCD_sendCommand(lcd_obj,LCD_DISPLAY_ON_UNDERLINE_OFF_BLINK_OFF);

		/* clear display */
		LCD_sendCommand(lcd_obj,LCD_CLEAR_DISPLAY);
	}

	return l_status;
}


/**
 * @brief  send the command to the LCD whether it's 4-bit mode or 8-bit mode
 * @param  (lcd_obj)  pointer to the LCD object passed by reference
 * @param  (command)  holds the command sent to the LCD
 * @return (l_status) status of the performed operation
 *              (E_NOT_OK)  operation failed
 *              (E_OK)      operation success
 */
Std_ReturnType LCD_sendCommand(lcd_config_t *lcd_obj,uint8 command)
{
	/* create a local variable to hold the status of the performed operation */
	Std_ReturnType l_status = ZERO_INIT;

	/* check if the address is valid or not */
	if(lcd_obj == NULL_PTR)
	{
		/* NULL pointer is passed */

		l_status = E_NOK;		/* operation failed */
	}
	else
	{
		/* the address passed is valid */

		l_status = E_OK;		/* operation success */

		/* send the command to the LCD */

		/* Instruction Mode RS=0 */
		GPIO_writePin( &(lcd_obj->register_select) , LCD_SELECT_COMMAND );

		_delay_ms(1); 			/* delay for processing Tas = 50ns */

		/* Enable LCD E=1 */
		GPIO_writePin( &(lcd_obj->enable) , GPIO_HIGH );

		_delay_ms(1); 			/* delay for processing Tpw - Tdws = 190ns */

		/* select the write mode */
		#if LCD_CONFIG_READ_WRITE_PIN
		GPIO_writePin( &(lcd_obj->read_write) , LCD_WRITE );
		#endif

		/* 4-bit mode */
		#if LCD_CONFIG_4_BIT_MODE
		/* send the highest 4 bits in command through the data bus */
		GPIO_writePin( &(lcd_obj->data_bus[LCD_DATA_BUS_1]) , GET_BIT(command,4) );
		GPIO_writePin( &(lcd_obj->data_bus[LCD_DATA_BUS_2]) , GET_BIT(command,5) );
		GPIO_writePin( &(lcd_obj->data_bus[LCD_DATA_BUS_3]) , GET_BIT(command,6) );
		GPIO_writePin( &(lcd_obj->data_bus[LCD_DATA_BUS_4]) , GET_BIT(command,7) );

		_delay_ms(1); 			/* delay for processing Tdsw = 100ns */

		/* Disable LCD E=0 */
		GPIO_writePin( &(lcd_obj->enable) , GPIO_LOW );

		_delay_ms(1); 			/* delay for processing Th = 13ns */

		/* Enable LCD E=1 */
		GPIO_writePin( &(lcd_obj->enable) , GPIO_HIGH );

		_delay_ms(1); 			/* delay for processing Tpw - Tdws = 190ns */

		/* send the lowest 4 bits in command through the data bus */
		GPIO_writePin( &(lcd_obj->data_bus[LCD_DATA_BUS_1]) , GET_BIT(command,0) );
		GPIO_writePin( &(lcd_obj->data_bus[LCD_DATA_BUS_2]) , GET_BIT(command,1) );
		GPIO_writePin( &(lcd_obj->data_bus[LCD_DATA_BUS_3]) , GET_BIT(command,2) );
		GPIO_writePin( &(lcd_obj->data_bus[LCD_DATA_BUS_4]) , GET_BIT(command,3) );

		_delay_ms(1); 			/* delay for processing Tdsw = 100ns */

		/* Disable LCD E=0 */
		GPIO_writePin( &(lcd_obj->enable) , GPIO_LOW );

		_delay_ms(1); 			/* delay for processing Th = 13ns */
		#endif

		/* 8-bit mode */
		#if LCD_CONFIG_8_BIT_MODE
		/* send the command through the data bus */
		GPIO_writePin( &(lcd_obj->data_bus[LCD_DATA_BUS_1]) , GET_BIT(command,0) );
		GPIO_writePin( &(lcd_obj->data_bus[LCD_DATA_BUS_2]) , GET_BIT(command,1) );
		GPIO_writePin( &(lcd_obj->data_bus[LCD_DATA_BUS_3]) , GET_BIT(command,2) );
		GPIO_writePin( &(lcd_obj->data_bus[LCD_DATA_BUS_4]) , GET_BIT(command,3) );
		GPIO_writePin( &(lcd_obj->data_bus[LCD_DATA_BUS_5]) , GET_BIT(command,4) );
		GPIO_writePin( &(lcd_obj->data_bus[LCD_DATA_BUS_6]) , GET_BIT(command,5) );
		GPIO_writePin( &(lcd_obj->data_bus[LCD_DATA_BUS_7]) , GET_BIT(command,6) );
		GPIO_writePin( &(lcd_obj->data_bus[LCD_DATA_BUS_8]) , GET_BIT(command,7) );

		_delay_ms(1); 			/* delay for processing Tdsw = 100ns */

		/* Disable LCD E=0 */
		GPIO_writePin( &(lcd_obj->enable) , GPIO_LOW );

		_delay_ms(1); 			/* delay for processing Th = 13ns */
		#endif
	}

	return l_status;
}


/**
 * @brief  send the data to the LCD whether it's 4-bit mode or 8-bit mode
 * @param  (lcd_obj)  pointer to the LCD object passed by reference
 * @param  (data)     holds the data sent to the LCD
 * @return (l_status) status of the performed operation
 *              (E_NOT_OK)  operation failed
 *              (E_OK)      operation success
 */
Std_ReturnType LCD_displayCharacter(lcd_config_t *lcd_obj,uint8 data)
{
	/* create a local variable to hold the status of the performed operation */
	Std_ReturnType l_status = ZERO_INIT;

	/* check if the address is valid or not */
	if(lcd_obj == NULL_PTR)
	{
		/* NULL pointer is passed */

		l_status = E_NOK;		/* operation failed */
	}
	else
	{
		/* the address passed is valid */

		l_status = E_OK;		/* operation success */

		/* send the data to the LCD */

		/* Instruction Mode RS=1 */
		GPIO_writePin( &(lcd_obj->register_select) , LCD_SELECT_DATA );

		_delay_ms(1); 			/* delay for processing Tas = 50ns */

		/* Enable LCD E=1 */
		GPIO_writePin( &(lcd_obj->enable) , GPIO_HIGH );

		_delay_ms(1); 			/* delay for processing Tpw - Tdws = 190ns */

		/* select the write mode */
		#if LCD_CONFIG_READ_WRITE_PIN
		GPIO_writePin( &(lcd_obj->read_write) , LCD_WRITE );
		#endif

		/* 4-bit mode */
		#if LCD_CONFIG_4_BIT_MODE
		/* send the highest 4 bits in data through the data bus */
		GPIO_writePin( &(lcd_obj->data_bus[LCD_DATA_BUS_1]) , GET_BIT(data,4) );
		GPIO_writePin( &(lcd_obj->data_bus[LCD_DATA_BUS_2]) , GET_BIT(data,5) );
		GPIO_writePin( &(lcd_obj->data_bus[LCD_DATA_BUS_3]) , GET_BIT(data,6) );
		GPIO_writePin( &(lcd_obj->data_bus[LCD_DATA_BUS_4]) , GET_BIT(data,7) );

		_delay_ms(1); 			/* delay for processing Tdsw = 100ns */

		/* Disable LCD E=0 */
		GPIO_writePin( &(lcd_obj->enable) , GPIO_LOW );

		_delay_ms(1); 			/* delay for processing Th = 13ns */

		/* Enable LCD E=1 */
		GPIO_writePin( &(lcd_obj->enable) , GPIO_HIGH );

		_delay_ms(1); 			/* delay for processing Tpw - Tdws = 190ns */

		/* send the lowest 4 bits in data through the data bus */
		GPIO_writePin( &(lcd_obj->data_bus[LCD_DATA_BUS_1]) , GET_BIT(data,0) );
		GPIO_writePin( &(lcd_obj->data_bus[LCD_DATA_BUS_2]) , GET_BIT(data,1) );
		GPIO_writePin( &(lcd_obj->data_bus[LCD_DATA_BUS_3]) , GET_BIT(data,2) );
		GPIO_writePin( &(lcd_obj->data_bus[LCD_DATA_BUS_4]) , GET_BIT(data,3) );

		_delay_ms(1); 			/* delay for processing Tdsw = 100ns */

		/* Disable LCD E=0 */
		GPIO_writePin( &(lcd_obj->enable) , GPIO_LOW );

		_delay_ms(1); 			/* delay for processing Th = 13ns */
		#endif

		/* 8-bit mode */
		#if LCD_CONFIG_8_BIT_MODE
		/* send the data through the data bus */
		GPIO_writePin( &(lcd_obj->data_bus[LCD_DATA_BUS_1]) , GET_BIT(data,0) );
		GPIO_writePin( &(lcd_obj->data_bus[LCD_DATA_BUS_2]) , GET_BIT(data,1) );
		GPIO_writePin( &(lcd_obj->data_bus[LCD_DATA_BUS_3]) , GET_BIT(data,2) );
		GPIO_writePin( &(lcd_obj->data_bus[LCD_DATA_BUS_4]) , GET_BIT(data,3) );
		GPIO_writePin( &(lcd_obj->data_bus[LCD_DATA_BUS_5]) , GET_BIT(data,4) );
		GPIO_writePin( &(lcd_obj->data_bus[LCD_DATA_BUS_6]) , GET_BIT(data,5) );
		GPIO_writePin( &(lcd_obj->data_bus[LCD_DATA_BUS_7]) , GET_BIT(data,6) );
		GPIO_writePin( &(lcd_obj->data_bus[LCD_DATA_BUS_8]) , GET_BIT(data,7) );

		_delay_ms(1); 			/* delay for processing Tdsw = 100ns */

		/* Disable LCD E=0 */
		GPIO_writePin( &(lcd_obj->enable) , GPIO_LOW );

		_delay_ms(1); 			/* delay for processing Th = 13ns */
		#endif
	}

	return l_status;
}


/**
 * @brief  Display the required string on the LCD
 * @param  (lcd_obj)  pointer to the LCD object passed by reference
 * @param  (Str)      pointer to the first character of the string sent to the LCD
 * @return (l_status) status of the performed operation
 *              (E_NOT_OK)  operation failed
 *              (E_OK)      operation success
 */
Std_ReturnType LCD_displayString(lcd_config_t *lcd_obj,const char *str)
{
	/* create a local variable to hold the status of the performed operation */
	Std_ReturnType l_status = ZERO_INIT;

	/* create a local variable to traverse the string */
	uint8 counter = ZERO_INIT;

	/* check if the address is valid or not */
	if( (lcd_obj == NULL_PTR) || (str == NULL_PTR) )
	{
		/* NULL pointer is passed */

		l_status = E_NOK;		/* operation failed */
	}
	else
	{
		/* the address passed is valid */

		l_status = E_OK;		/* operation success */

		/* send the string to the LCD */

		/* send until NULL Character is encountered */
		while( *(str + counter) != '\0' )
		{
			/* send one character at a time */
			LCD_displayCharacter(lcd_obj , *(str + counter));

			counter++;
		}
	}

	return l_status;
}


/**
 * @brief  Move the cursor to a specified row and column index on the LCD
 * @param  (lcd_obj)  pointer to the LCD object passed by reference
 * @param  (row)      the row position of the cursor(starts from 1)
 * @param  (column)   the column position of the cursor(starts from 1)
 * @return (l_status) status of the performed operation
 *              (E_NOT_OK)  operation failed
 *              (E_OK)      operation success
 */
Std_ReturnType LCD_moveCursor(lcd_config_t *lcd_obj,uint8 row,uint8 column)
{
	/* create a local variable to hold the status of the performed operation */
	Std_ReturnType l_status = ZERO_INIT;

	/* check if the address is valid or not */
	if(lcd_obj == NULL_PTR)
	{
		/* NULL pointer is passed */

		l_status = E_NOK;		/* operation failed */
	}
	else
	{
		/* the address passed is valid */

		l_status = E_OK;		/* operation success */

		/* decrement column as it starts from zero */
		column--;

		/* address set DDRAM is 0x80 */
		/* the address of the first row and first column is  (0x80 = address set DDRAM + 0x00) */
		/* the address of the second row and first column is (0xC0 = address set DDRAM + 0x40) */
		/* the address of the third row and first column is  (0x94 = address set DDRAM + 0x14) */
		/* the address of the fourth row and first column is (0xD4 = address set DDRAM + 0x54) */

		/* send the command to the LCD with the new position of the DDRAM to be set in the Address Counter */
		switch(row)
		{
		    case LCD_ROW_1  :   LCD_sendCommand( lcd_obj , ( (LCD_DDRAM_SET_ADDRESS + 0x00) + column ) );
		        				break;

		    case LCD_ROW_2  :   LCD_sendCommand( lcd_obj , ( (LCD_DDRAM_SET_ADDRESS + 0x40) + column ) );
		        				break;

		    case LCD_ROW_3  :   LCD_sendCommand( lcd_obj , ( (LCD_DDRAM_SET_ADDRESS + 0x10) + column ) );
		        				break;

		    case LCD_ROW_4  :   LCD_sendCommand( lcd_obj , ( (LCD_DDRAM_SET_ADDRESS + 0x50) + column ) );
		        				break;

		    default     :   	/* Nothing */
		        				break;
		}
	}

	return l_status;
}


/**
 * @brief  Display the required string in a specified row and column index on the LCD
 * @param  (lcd_obj)  pointer to the LCD object passed by reference
 * @param  (row)      the row position of the cursor
 * @param  (column)   the column position of the cursor
 * @param  (Str)      pointer to the first character of the string sent to the LCD
 * @return (l_status) status of the performed operation
 *              (E_NOT_OK)  operation failed
 *              (E_OK)      operation success
 */
Std_ReturnType LCD_displayStringRowColumn(lcd_config_t *lcd_obj,uint8 row,uint8 column,const char *str)
{
	/* create a local variable to hold the status of the performed operation */
	Std_ReturnType l_status = ZERO_INIT;

	/* check if the address is valid or not */
	if( (lcd_obj == NULL_PTR) || (str == NULL_PTR) )
	{
		/* NULL pointer is passed */

		l_status = E_NOK;		/* operation failed */
	}
	else
	{
		/* the address passed is valid */

		l_status = E_OK;		/* operation success */

		/* go to to the required LCD position */
		LCD_moveCursor(lcd_obj,row,column);

		/* display the string */
		LCD_displayString(lcd_obj,str);
	}

	return l_status;
}


/**
 * @brief  Display the required decimal value on the LCD
 * @param  (lcd_obj)  pointer to the LCD object passed by reference
 * @param  (data)     holds the value to be displayed on the  LCD
 * @return (l_status) status of the performed operation
 *              (E_NOT_OK)  operation failed
 *              (E_OK)      operation success
 */
Std_ReturnType LCD_intgerToString(lcd_config_t *lcd_obj,sint32 data)
{
	/* create a local variable to hold the status of the performed operation */
	Std_ReturnType l_status = ZERO_INIT;

	/* check if the address is valid or not */
	if(lcd_obj == NULL_PTR)
	{
		/* NULL pointer is passed */

		l_status = E_NOK;		/* operation failed */
	}
	else
	{
		/* the address passed is valid */

		l_status = E_OK;		/* operation success */

		/* String to hold the ASCII result */
		char buffer[16];

		/* Use itoa C function to convert the data to its corresponding ASCII value, 10 for decimal */
		itoa(data,buffer,10);

		/* Display the string */
		LCD_displayString(lcd_obj,buffer);
	}

	return l_status;
}


/**
 * @brief  Send the clear screen command to the LCD
 * @param  (lcd_obj)  pointer to the LCD object passed by reference
 * @return (l_status) status of the performed operation
 *              (E_NOT_OK)  operation failed
 *              (E_OK)      operation success
 */
Std_ReturnType LCD_clearScreen(lcd_config_t *lcd_obj)
{
	/* create a local variable to hold the status of the performed operation */
	Std_ReturnType l_status = ZERO_INIT;

	/* check if the address is valid or not */
	if(lcd_obj == NULL_PTR)
	{
		/* NULL pointer is passed */

		l_status = E_NOK;		/* operation failed */
	}
	else
	{
		/* the address passed is valid */

		l_status = E_OK;		/* operation success */

		/* clear display */
		l_status = LCD_sendCommand(lcd_obj,LCD_CLEAR_DISPLAY);
	}

	return l_status;
}


/* ----------------------------------------------------------------------------------- */
