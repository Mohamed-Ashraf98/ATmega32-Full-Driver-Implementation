/*
 =========================================================================================
 Name        : gpio.h
 Author      : Mohamed Ashraf El-Sayed
 Version     : 1.0.0
 Copyright   : Your copyright notice
 date        : Sun, Sep 24 2023
 time        :
 Description : GPIO Driver Header file , Ansi-style
 =========================================================================================
*/

#ifndef _GPIO_H_
#define _GPIO_H_
/* ----------------------------------------------------------------------------------- */
/* ------------------Includes section------------------- */
#include "ATmega32.h"


/* ----------------------------------------------------------------------------------- */
/* --------------Macro declaration section-------------- */

/* status of the gpio pin */
#define GPIO_LOW					0
#define GPIO_HIGH               	1

/* gpio port index */
#define GPIO_PORTA              	0
#define GPIO_PORTB              	1
#define GPIO_PORTC              	2
#define GPIO_PORTD              	3
	
/* gpio pin index */	
#define GPIO_PIN0               	0
#define GPIO_PIN1               	1
#define GPIO_PIN2               	2
#define GPIO_PIN3               	3
#define GPIO_PIN4               	4
#define GPIO_PIN5               	5
#define GPIO_PIN6               	6
#define GPIO_PIN7               	7

/* port configurations */
#define GPIO_PORT_INPUT				0x00
#define GPIO_PORT_OUTPUT			0xFF

/* port mask */
#define GPIO_PORT_MASK_ALL_CLEAR	0x00
#define GPIO_PORT_MASK_ALL_SET		0xFF

/* pin/port maximum number */
#define PIN_MAX                 	8
#define PORT_MAX                	4


/* ----------------------------------------------------------------------------------- */
/* -----user_defined data type declaration section------ */

/* gpio config structure */
typedef struct{
	/* holds the port ID >> @ref : gpio port index */
    uint8 port	:2;
    /* holds the number of the pin >> @ref : gpio pin index */
    uint8 pin	:3;
    /* holds the configuration mode of the pin >> @ref : gpio mode select */
    uint8 mode	:2;
    /* Reserved */
	uint8		:1;
}gpio_config_t;


/* gpio mode select */
typedef enum{
	GPIO_MODE_OUTPUT = 0,
	GPIO_MODE_INPUT_WITHOUT_INTERNAL_PULL_UP_RES,
	GPIO_MODE_INPUT_WITH_INTERNAL_PULL_UP_RES
}gpio_mode_t;


/* ----------------------------------------------------------------------------------- */
/* ------------functions declaration section------------ */


/**
 * @brief  initialize the direction of the pin to be one of the following modes :
 *				1- Output mode
 *				2- Input without internal pull up resistance
 *				3- Input with internal pull up resistance
 * @param  (gpio_obj) pointer to the gpio pin object passed by reference
 * @return (l_status) status of the performed operation
 *              (E_NOT_OK)  operation failed
 *              (E_OK)      operation success
 */
Std_ReturnType GPIO_setupPinDirection(gpio_config_t *gpio_obj);


/**
 * @brief  write logic HIGH/LOW on a specific pin
 * @param  (gpio_obj) pointer to the gpio pin object passed by reference
 * @param  (logic)    holds the logic value (HIGH/LOW) to be written on the pin
 * @return (l_status) status of the performed operation
 *              (E_NOT_OK)  operation failed
 *              (E_OK)      operation success
 */
Std_ReturnType GPIO_writePin(gpio_config_t *gpio_obj,uint8 logic);


/**
 * @brief  read logic from a specific pin
 * @param  (gpio_obj) pointer to the gpio pin object passed by reference
 * @return (logic)    the logic status of the pin
 *              (GPIO_LOW)  	read logic 0/input port number or pin number are not correct
 *              (GPIO_HIGH)		read logic 1
 */
uint8 GPIO_readPin(gpio_config_t *gpio_obj);


/**
 * @brief  initialize the direction of the port to be one of the following modes :
 *				1- Output mode
 *				2- Input without internal pull up resistance
 *				3- Input with internal pull up resistance
 * @param  (port_num)  holds the ID of the port to be set
 * @param  (direction) holds the direction of the port
 * @return (l_status)  status of the performed operation
 *              (E_NOT_OK)  operation failed
 *              (E_OK)      operation success
 */
Std_ReturnType GPIO_setupPortDirection(uint8 port_num,uint8 direction);


/**
 * @brief  write logic HIGH/LOW on a specific port
 * @param  (port_num) holds the ID of the port to be set
 * @param  (logic)    holds the logic values (HIGH/LOW) to be written on the port
 * @return (l_status) status of the performed operation
 *              (E_NOT_OK)  operation failed
 *              (E_OK)      operation success
 */
Std_ReturnType GPIO_writePort(uint8 port_num,uint8 logic);


/**
 * @brief  read logic from a specific port
 * @param  (port_num) holds the ID of the port to be set
 * @return (logic)    the logic status of the port
 *              (GPIO_LOW)  	read logic 0 if input port number or pin number are not correct
 */
uint8 GPIO_readPort(uint8 port_num);


/* ----------------------------------------------------------------------------------- */
#endif /* _GPIO_H_ */
