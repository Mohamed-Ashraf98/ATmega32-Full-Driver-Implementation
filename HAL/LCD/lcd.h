/*
 =========================================================================================
 Name        : lcd.h
 Author      : Mohamed Ashraf El-Sayed
 Version     : 1.0.0
 Copyright   : Your copyright notice
 date        : Sun, Oct 4 2023
 time        :
 Description : LCD Driver Source file , Ansi-style
 =========================================================================================
*/

#ifndef _LCD_H_
#define _LCD_H_
/* ----------------------------------------------------------------------------------- */
/* ------------------Includes section------------------- */
#include "gpio.h"


/* ----------------------------------------------------------------------------------- */
/* --------------Macro declaration section-------------- */

/* --------------------------------- */
/* LCD configurations for 8-bit mode or 4-bit mode */
/* NOTE: one of them must be enabled and the other is disabled */

/* LCD 4-bit mode */
#define LCD_CONFIG_4_BIT_MODE		DISABLE
/* LCD 8-bit mode */
#define LCD_CONFIG_8_BIT_MODE		ENABLE

/* --------------------------------- */
/* size of data bus connected with the LCD */

/* LCD 4-bit mode */
#if LCD_CONFIG_4_BIT_MODE
#define LCD_DATA_BUS_SIZE			4
#endif

/* LCD 8-bit mode */
#if LCD_CONFIG_8_BIT_MODE
#define LCD_DATA_BUS_SIZE			8
#endif

/* --------------------------------- */
/* Read/Write LCD pin configurations enable/disable */

#define LCD_CONFIG_READ_WRITE_PIN	DISABLE

/* --------------------------------- */
/* character LCD Commands */

/* Clear Display */
#define LCD_CLEAR_DISPLAY                                  	0x01

/* Cursor Home */
#define LCD_CURSOR_HOME                                    	0x02

/* Entry Mode Set */
/* decrement address counter without display shift */
#define LCD_DEC_WITHOUT_SHIFT                              	0x04
/* decrement address counter with display shift */
#define LCD_DEC_WITH_SHIFT                                 	0x05
/* increment address counter without display shift */
#define LCD_INC_WITHOUT_SHIFT                              	0x06
/* increment address counter with display shift */
#define LCD_INC_WITH_SHIFT                                 	0x07

/* Display On/Off */
/* display off underline cursor off blink off */
#define LCD_DISPLAY_OFF_UNDERLINE_OFF_BLINK_OFF            	0x08
/* display on underline cursor off blink off */
#define LCD_DISPLAY_ON_UNDERLINE_OFF_BLINK_OFF             	0x0C
/* display on underline cursor off blink on */
#define LCD_DISPLAY_ON_UNDERLINE_OFF_BLINK_ON              	0x0D
/* display on underline cursor on blink off */
#define LCD_DISPLAY_ON_UNDERLINE_ON_BLINK_OFF              	0x0E
/* display on underline cursor on blink on */
#define LCD_DISPLAY_ON_UNDERLINE_ON_BLINK_ON               	0x0F

/* Move Cursor and Shift Display */
/* shift cursor position left */
#define LCD_MOVE_CURSOR_LEFT                               	0x10
/* shift cursor position right */
#define LCD_MOVE_CURSOR_RIGHT                              	0x14
/* shift display left */
#define LCD_DISPLAY_SHIFT_LEFT                             	0x18
/* shift display right */
#define LCD_DISPLAY_SHIFT_RIGHT                            	0x1C

/* Function Set */
/* LCD 4 bit Mode Initialization */
#define LCD_TWO_LINES_FOUR_BITS_MODE_INIT1   				0x33
#define LCD_TWO_LINES_FOUR_BITS_MODE_INIT2   				0x32

/* Function Set */
/* lcd 4-bit mode, 1 row mode, 5x8 dot matrix */
#define LCD_4_bit_1_row_5x8                                	0x20
/* lcd 4-bit mode, 1 row mode, 5x11 dot matrix */
#define LCD_4_bit_1_row_5x11                               	0x24
/* lcd 4-bit mode, 2 row mode, 5x8 dot matrix */
#define LCD_4_bit_2_row_5x8                                	0x28
/* lcd 4-bit mode, 2 row mode, 5x11 dot matrix */
#define LCD_4_bit_2_row_5x11                               	0x2C
/* lcd 8-bit mode, 1 row mode, 5x8 dot matrix */
#define LCD_8_bit_1_row_5x8                                	0x30
/* lcd 8-bit mode, 1 row mode, 5x11 dot matrix */
#define LCD_8_bit_1_row_5x11                               	0x34
/* lcd 8-bit mode, 2 row mode, 5x8 dot matrix */
#define LCD_8_bit_2_row_5x8                                	0x38
/* lcd 8-bit mode, 2 row mode, 5x11 dot matrix */
#define LCD_8_bit_2_row_5x11                               	0x3C

/* Address Set CGRAM */
#define LCD_CGRAM_SET_ADDRESS                              	0x40

/* Address Set DDRAM */
#define LCD_DDRAM_SET_ADDRESS                              	0x80

/* --------------------------------- */
/* index of the data bus lines in the LCD */

#define LCD_DATA_BUS_1				0
#define LCD_DATA_BUS_2				1
#define LCD_DATA_BUS_3				2
#define LCD_DATA_BUS_4				3
#if LCD_CONFIG_8_BIT_MODE
/* LCD 8-bit mode */
#define LCD_DATA_BUS_5				4
#define LCD_DATA_BUS_6				5
#define LCD_DATA_BUS_7				6
#define LCD_DATA_BUS_8				7
#endif

/* --------------------------------- */
/* Rows in any character LCD can be represented from these 4 rows */

#define LCD_ROW_1					1
#define LCD_ROW_2                   2
#define LCD_ROW_3                   3
#define LCD_ROW_4                   4

/* --------------------------------- */


/* ----------------------------------------------------------------------------------- */
/* -----user_defined data type declaration section------ */

/* LCD config structure */
typedef struct{
	/* selects between data and command to be sent to the LCD >> @ref : lcd_rs_t */
	gpio_config_t register_select;
	/* enable/disable the read/write in the LCD */
	gpio_config_t enable;
	/* hold the information of the LCD data bus pins */
	gpio_config_t data_bus[LCD_DATA_BUS_SIZE];
	/* selects to whether to read from or write to the LCD >> @ref : lcd_rw_t */
	#if LCD_CONFIG_READ_WRITE_PIN
	gpio_config_t read_write;
	#endif
}lcd_config_t;


/* @ref : lcd_rs_t */
typedef enum{
	LCD_SELECT_COMMAND = 0,
	LCD_SELECT_DATA
}lcd_rs_t;

#if LCD_CONFIG_READ_WRITE_PIN
/* @ref lcd_rw_t */
typedef enum{
    LCD_WRITE = 0,
    LCD_READ
}lcd_rw_t;
#endif


/* ----------------------------------------------------------------------------------- */
/* ------------functions declaration section------------ */


/**
 * @brief  Initialize the LCD :
 *				1- Setup the LCD pins directions by use the GPIO driver
 *				2- Setup the LCD Data Mode 4-bits or 8-bits
 * @param  (lcd_obj)  pointer to the LCD object passed by reference
 * @return (l_status) status of the performed operation
 *              (E_NOT_OK)  operation failed
 *              (E_OK)      operation success
 */
Std_ReturnType LCD_init(lcd_config_t *lcd_obj);


/**
 * @brief  send the command to the LCD whether it's 4-bit mode or 8-bit mode
 * @param  (lcd_obj)  pointer to the LCD object passed by reference
 * @param  (command)  holds the command sent to the LCD
 * @return (l_status) status of the performed operation
 *              (E_NOT_OK)  operation failed
 *              (E_OK)      operation success
 */
Std_ReturnType LCD_sendCommand(lcd_config_t *lcd_obj,uint8 command);


/**
 * @brief  send the data to the LCD whether it's 4-bit mode or 8-bit mode
 * @param  (lcd_obj)  pointer to the LCD object passed by reference
 * @param  (data)     holds the data sent to the LCD
 * @return (l_status) status of the performed operation
 *              (E_NOT_OK)  operation failed
 *              (E_OK)      operation success
 */
Std_ReturnType LCD_displayCharacter(lcd_config_t *lcd_obj,uint8 data);


/**
 * @brief  Display the required string on the LCD
 * @param  (lcd_obj)  pointer to the LCD object passed by reference
 * @param  (Str)      pointer to the first character of the string sent to the LCD
 * @return (l_status) status of the performed operation
 *              (E_NOT_OK)  operation failed
 *              (E_OK)      operation success
 */
Std_ReturnType LCD_displayString(lcd_config_t *lcd_obj,const char *str);


/**
 * @brief  Move the cursor to a specified row and column index on the LCD
 * @param  (lcd_obj)  pointer to the LCD object passed by reference
 * @param  (row)      the row position of the cursor(starts from 1)
 * @param  (column)   the column position of the cursor(starts from 1)
 * @return (l_status) status of the performed operation
 *              (E_NOT_OK)  operation failed
 *              (E_OK)      operation success
 */
Std_ReturnType LCD_moveCursor(lcd_config_t *lcd_obj,uint8 row,uint8 column);


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
Std_ReturnType LCD_displayStringRowColumn(lcd_config_t *lcd_obj,uint8 row,uint8 column,const char *str);


/**
 * @brief  Display the required decimal value on the LCD
 * @param  (lcd_obj)  pointer to the LCD object passed by reference
 * @param  (data)     holds the value to be displayed on the  LCD
 * @return (l_status) status of the performed operation
 *              (E_NOT_OK)  operation failed
 *              (E_OK)      operation success
 */
Std_ReturnType LCD_intgerToString(lcd_config_t *lcd_obj,sint32 data);


/**
 * @brief  Send the clear screen command to the LCD
 * @param  (lcd_obj)  pointer to the LCD object passed by reference
 * @return (l_status) status of the performed operation
 *              (E_NOT_OK)  operation failed
 *              (E_OK)      operation success
 */
Std_ReturnType LCD_clearScreen(lcd_config_t *lcd_obj);


/* ----------------------------------------------------------------------------------- */
#endif /* _LCD_H_ */
