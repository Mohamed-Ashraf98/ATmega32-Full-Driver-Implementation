/*
 =========================================================================================
 Name        : gpio.c
 Author      : Mohamed Ashraf El-Sayed
 Version     : 1.0.0
 Copyright   : Your copyright notice
 date        : Sun, Sep 24 2023
 time        :
 Description : GPIO Driver Source file , Ansi-style
 =========================================================================================
*/

/* ----------------------------------------------------------------------------------- */
/* ------------------Includes section------------------- */
#include "gpio.h"
#include "common_macros.h"


/* ----------------------------------------------------------------------------------- */
/* ------------functions definition section------------- */


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
Std_ReturnType GPIO_setupPinDirection(gpio_config_t *gpio_obj)
{
	/* create a local variable to hold the status of the performed operation */
    Std_ReturnType l_status = ZERO_INIT;

    /* check if the address is valid or not */
    /* check if the user exceeded the max number of pins or not */
    /* check if the user exceeded the max number of ports or not */
    if( (gpio_obj == NULL_PTR) || (gpio_obj->pin >= PIN_MAX) || (gpio_obj->port >= PORT_MAX) )
    {
        /* NULL pointer is passed */
        /* pin number inserted higher than the maximum number of pins */
        /* port number inserted higher than the maximum number of ports */
        
        l_status = E_NOK;		/* operation failed */
    }
    else
    {
        /* the address passed is valid */
        /* pin number inserted is within the range of pins */
        /* port number inserted is within the range of ports */

        l_status = E_OK;		/* operation success */
        
        /* initialize the direction of the pin */
		
		/* check which port is used first */
		switch(gpio_obj->port)
		{
			case GPIO_PORTA	:	/* the pin is is Port A so we will use DDRA Register to set the direction */
									
								/* check if its output or input */
								if(gpio_obj->mode == GPIO_MODE_OUTPUT)
								{
									/* the pin is set to be output */
									SET_BIT(_DDRA.Byte,gpio_obj->pin);
								}
								else if(gpio_obj->mode == GPIO_MODE_INPUT_WITHOUT_INTERNAL_PULL_UP_RES)
								{
									/* the pin is set to be input without internal pull up resistance */
									CLEAR_BIT(_DDRA.Byte,gpio_obj->pin);
									CLEAR_BIT(_PORTA.Byte,gpio_obj->pin);
								}
								else if(gpio_obj->mode == GPIO_MODE_INPUT_WITH_INTERNAL_PULL_UP_RES)
								{
									/* the pin is set to be input with internal pull up resistance */
									CLEAR_BIT(_DDRA.Byte,gpio_obj->pin);
									SET_BIT(_PORTA.Byte,gpio_obj->pin);
								}
								else{ /* Nothing */ }
									
								break;
									
			case GPIO_PORTB	:	/* the pin is is Port B so we will use DDRB Register to set the direction */
									
								/* check if its output or input */
								if(gpio_obj->mode == GPIO_MODE_OUTPUT)
								{
									/* the pin is set to be output */
									SET_BIT(_DDRB.Byte,gpio_obj->pin);
								}
								else if(gpio_obj->mode == GPIO_MODE_INPUT_WITHOUT_INTERNAL_PULL_UP_RES)
								{
									/* the pin is set to be input without internal pull up resistance */
									CLEAR_BIT(_DDRB.Byte,gpio_obj->pin);
									CLEAR_BIT(_PORTB.Byte,gpio_obj->pin);
								}
								else if(gpio_obj->mode == GPIO_MODE_INPUT_WITH_INTERNAL_PULL_UP_RES)
								{
									/* the pin is set to be input with internal pull up resistance */
									CLEAR_BIT(_DDRB.Byte,gpio_obj->pin);
									SET_BIT(_PORTB.Byte,gpio_obj->pin);
								}
								else{ /* Nothing */ }
									
								break;
									
			case GPIO_PORTC	:	/* the pin is is Port C so we will use DDRC Register to set the direction */
									
								/* check if its output or input */
								if(gpio_obj->mode == GPIO_MODE_OUTPUT)
								{
									/* the pin is set to be output */
									SET_BIT(_DDRC.Byte,gpio_obj->pin);
								}
								else if(gpio_obj->mode == GPIO_MODE_INPUT_WITHOUT_INTERNAL_PULL_UP_RES)
								{
									/* the pin is set to be input without internal pull up resistance */
									CLEAR_BIT(_DDRC.Byte,gpio_obj->pin);
									CLEAR_BIT(_PORTC.Byte,gpio_obj->pin);
								}
								else if(gpio_obj->mode == GPIO_MODE_INPUT_WITH_INTERNAL_PULL_UP_RES)
								{
									/* the pin is set to be input with internal pull up resistance */
									CLEAR_BIT(_DDRC.Byte,gpio_obj->pin);
									SET_BIT(_PORTC.Byte,gpio_obj->pin);
								}
								else{ /* Nothing */ }
									
								break;
									
			case GPIO_PORTD	:	/* the pin is is Port D so we will use DDRD Register to set the direction */
									
								/* check if its output or input */
								if(gpio_obj->mode == GPIO_MODE_OUTPUT)
								{
									/* the pin is set to be output */
									SET_BIT(_DDRD.Byte,gpio_obj->pin);
								}
								else if(gpio_obj->mode == GPIO_MODE_INPUT_WITHOUT_INTERNAL_PULL_UP_RES)
								{
									/* the pin is set to be input without internal pull up resistance */
									CLEAR_BIT(_DDRD.Byte,gpio_obj->pin);
									CLEAR_BIT(_PORTD.Byte,gpio_obj->pin);
								}
								else if(gpio_obj->mode == GPIO_MODE_INPUT_WITH_INTERNAL_PULL_UP_RES)
								{
									/* the pin is set to be input with internal pull up resistance */
									CLEAR_BIT(_DDRD.Byte,gpio_obj->pin);
									SET_BIT(_PORTD.Byte,gpio_obj->pin);
								}
								else{ /* Nothing */ }
									
								break;
									
			default	:			/* Nothing */
								break;
		}
	}
	
	return l_status;
}


/**
 * @brief  write logic HIGH/LOW on a specific pin
 * @param  (gpio_obj) pointer to the gpio pin object passed by reference
 * @param  (logic)    holds the logic value (HIGH/LOW) to be written on the pin
 * @return (l_status) status of the performed operation
 *              (E_NOT_OK)  operation failed
 *              (E_OK)      operation success
 */
Std_ReturnType GPIO_writePin(gpio_config_t *gpio_obj,uint8 logic)
{
	/* create a local variable to hold the status of the performed operation */
    Std_ReturnType l_status = ZERO_INIT;

    /* check if the address is valid or not */
    /* check if the user exceeded the max number of pins or not */
    /* check if the user exceeded the max number of ports or not */
    if( (gpio_obj == NULL_PTR) || (gpio_obj->pin >= PIN_MAX) || (gpio_obj->port >= PORT_MAX) )
    {
        /* NULL pointer is passed */
        /* pin number inserted higher than the maximum number of pins */
        /* port number inserted higher than the maximum number of ports */
        
        l_status = E_NOK;		/* operation failed */
    }
    else
    {
        /* the address passed is valid */
        /* pin number inserted is within the range of pins */
        /* port number inserted is within the range of ports */

        l_status = E_OK;		/* operation success */
        
        /* set the logic out from the pin */
		
		/* check which port is used first */
		switch(gpio_obj->port)
		{
			case GPIO_PORTA	:	/* the pin is in Port A so we will use PORTA Register to set the logic */
									
								/* check if its output or input */
								if(gpio_obj->mode == GPIO_MODE_OUTPUT)
								{
									/* the pin is set to be output */

									/* set the logic */
									if(logic == GPIO_HIGH)
									{
										/* write logic 1 on the pin */
										SET_BIT(_PORTA.Byte,gpio_obj->pin);
									}
									else if(logic == GPIO_LOW)
									{
										/* write logic 0 on the pin */
										CLEAR_BIT(_PORTA.Byte,gpio_obj->pin);
									}
									else{ /* Nothing */ }
								}
								else{ /* Nothing */ }
									
								break;
									
			case GPIO_PORTB	:	/* the pin is in Port B so we will use PORTB Register to set the logic */
									
								/* check if its output or input */
								if(gpio_obj->mode == GPIO_MODE_OUTPUT)
								{
									/* the pin is set to be output */

									/* set the logic */
									if(logic == GPIO_HIGH)
									{
										/* write logic 1 on the pin */
										SET_BIT(_PORTB.Byte,gpio_obj->pin);
									}
									else if(logic == GPIO_LOW)
									{
										/* write logic 0 on the pin */
										CLEAR_BIT(_PORTB.Byte,gpio_obj->pin);
									}
									else{ /* Nothing */ }
								}
								else{ /* Nothing */ }
									
								break;
									
			case GPIO_PORTC	:	/* the pin is in Port C so we will use PORTC Register to set the logic */
									
								/* check if its output or input */
								if(gpio_obj->mode == GPIO_MODE_OUTPUT)
								{
									/* the pin is set to be output */

									/* set the logic */
									if(logic == GPIO_HIGH)
									{
										/* write logic 1 on the pin */
										SET_BIT(_PORTC.Byte,gpio_obj->pin);
									}
									else if(logic == GPIO_LOW)
									{
										/* write logic 0 on the pin */
										CLEAR_BIT(_PORTC.Byte,gpio_obj->pin);
									}
									else{ /* Nothing */ }
								}
								else{ /* Nothing */ }
									
								break;
									
			case GPIO_PORTD	:	/* the pin is in Port D so we will use PORTD Register to set the logic */
									
								/* check if its output or input */
								if(gpio_obj->mode == GPIO_MODE_OUTPUT)
								{
									/* the pin is set to be output */

									/* set the logic */
									if(logic == GPIO_HIGH)
									{
										/* write logic 1 on the pin */
										SET_BIT(_PORTD.Byte,gpio_obj->pin);
									}
									else if(logic == GPIO_LOW)
									{
										/* write logic 0 on the pin */
										CLEAR_BIT(_PORTD.Byte,gpio_obj->pin);
									}
									else{ /* Nothing */ }
								}
								else{ /* Nothing */ }
									
								break;
									
			default	:			/* Nothing */
								break;
		}
	}
	
	return l_status;
}


/**
 * @brief  read logic from a specific pin
 * @param  (gpio_obj) pointer to the gpio pin object passed by reference
 * @return (logic)    the logic status of the pin
 *              (GPIO_LOW)  	read logic 0/input port number or pin number are not correct
 *              (GPIO_HIGH)		read logic 1
 */
uint8 GPIO_readPin(gpio_config_t *gpio_obj)
{
	/* create a local variable to hold the logic of the pin */
	uint8 logic = ZERO_INIT;
	
	/* check if the address is valid or not */
    /* check if the user exceeded the max number of pins or not */
    /* check if the user exceeded the max number of ports or not */
    if( (gpio_obj == NULL_PTR) || (gpio_obj->pin >= PIN_MAX) || (gpio_obj->port >= PORT_MAX) )
    {
        /* NULL pointer is passed */
        /* pin number inserted higher than the maximum number of pins */
        /* port number inserted higher than the maximum number of ports */
        
        logic = GPIO_LOW;
    }
    else
    {
        /* the address passed is valid */
        /* pin number inserted is within the range of pins */
        /* port number inserted is within the range of ports */
        
        /* get the logic of the pin */
		
		/* check which port is used first */
		switch(gpio_obj->port)
		{
			case GPIO_PORTA	:	/* the pin is is Port A so we will use PINA Register to get the logic */
									
								/* check if the logic reading in the PINA Register is HIGH/LOW */
								if( BIT_IS_SET(_PINA.Byte,gpio_obj->pin) )
								{
									/* logic 1 is read from PINA Register */
									logic = GPIO_HIGH;
								}
								else
								{
									/* logic 0 is read from PINA Register */
									logic = GPIO_LOW;
								}
									
								break;
									
			case GPIO_PORTB	:	/* the pin is is Port B so we will use PINB Register to get the logic */
									
								/* check if the logic reading in the PINB Register is HIGH/LOW */
								if( BIT_IS_SET(_PINB.Byte,gpio_obj->pin) )
								{
									/* logic 1 is read from PINB Register */
									logic = GPIO_HIGH;
								}
								else
								{
									/* logic 0 is read from PINB Register */
									logic = GPIO_LOW;
								}
									
								break;
									
			case GPIO_PORTC	:	/* the pin is is Port C so we will use PINC Register to get the logic */
									
								/* check if the logic reading in the PINC Register is HIGH/LOW */
								if( BIT_IS_SET(_PINC.Byte,gpio_obj->pin) )
								{
									/* logic 1 is read from PINC Register */
									logic = GPIO_HIGH;
								}
								else
								{
									/* logic 0 is read from PINC Register */
									logic = GPIO_LOW;
								}
									
								break;
									
			case GPIO_PORTD	:	/* the pin is is Port D so we will use PIND Register to get the logic */
									
								/* check if the logic reading in the PIND Register is HIGH/LOW */
								if( BIT_IS_SET(_PIND.Byte,gpio_obj->pin) )
								{
									/* logic 1 is read from PIND Register */
									logic = GPIO_HIGH;
								}
								else
								{
									/* logic 0 is read from PIND Register */
									logic = GPIO_LOW;
								}
									
								break;
									
			default	:			/* Nothing */
								break;
		}
	}
	
	return logic;
}


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
Std_ReturnType GPIO_setupPortDirection(uint8 port_num,uint8 direction)
{
	/* create a local variable to hold the status of the performed operation */
    Std_ReturnType l_status = ZERO_INIT;

    /* check if the user exceeded the max number of ports or not */
    if(port_num >= PORT_MAX)
    {
        /* port number inserted higher than the maximum number of ports */
        
        l_status = E_NOK;		/* operation failed */
    }
    else
    {
        /* port number inserted is within the range of ports */

        l_status = E_OK;		/* operation success */
		
		/* initialize the direction of the port */
		
		/* check which port is used first */
		switch(port_num)
		{
			case GPIO_PORTA	:	/* Port A so we will use DDRA Register to set the direction */
									
								/* check if its output or input */
								if(direction == GPIO_MODE_OUTPUT)
								{
									/* the port is set to be output */
									_DDRA.Byte = GPIO_PORT_OUTPUT;
								}
								else if(direction == GPIO_MODE_INPUT_WITHOUT_INTERNAL_PULL_UP_RES)
								{
									/* the port is set to be input without internal pull up resistance */
									_DDRA.Byte = GPIO_PORT_INPUT;
									_PORTA.Byte = GPIO_PORT_MASK_ALL_CLEAR;
								}
								else if(direction == GPIO_MODE_INPUT_WITH_INTERNAL_PULL_UP_RES)
								{
									/* the port is set to be input with internal pull up resistance */
									_DDRA.Byte = GPIO_PORT_INPUT;
									_PORTA.Byte = GPIO_PORT_MASK_ALL_SET;
								}
								else{ /* Nothing */ }
									
								break;
									
			case GPIO_PORTB	:	/* Port B so we will use DDRB Register to set the direction */
									
								/* check if its output or input */
								if(direction == GPIO_MODE_OUTPUT)
								{
									/* the port is set to be output */
									_DDRB.Byte = GPIO_PORT_OUTPUT;
								}
								else if(direction == GPIO_MODE_INPUT_WITHOUT_INTERNAL_PULL_UP_RES)
								{
									/* the port is set to be input without internal pull up resistance */
									_DDRB.Byte = GPIO_PORT_INPUT;
									_PORTB.Byte = GPIO_PORT_MASK_ALL_CLEAR;
								}
								else if(direction == GPIO_MODE_INPUT_WITH_INTERNAL_PULL_UP_RES)
								{
									/* the port is set to be input with internal pull up resistance */
									_DDRB.Byte = GPIO_PORT_INPUT;
									_PORTB.Byte = GPIO_PORT_MASK_ALL_SET;
								}
								else{ /* Nothing */ }
									
								break;
									
			case GPIO_PORTC	:	/* Port C so we will use DDRC Register to set the direction */
									
								/* check if its output or input */
								if(direction == GPIO_MODE_OUTPUT)
								{
									/* the port is set to be output */
									_DDRC.Byte = GPIO_PORT_OUTPUT;
								}
								else if(direction == GPIO_MODE_INPUT_WITHOUT_INTERNAL_PULL_UP_RES)
								{
									/* the port is set to be input without internal pull up resistance */
									_DDRC.Byte = GPIO_PORT_INPUT;
									_PORTC.Byte = GPIO_PORT_MASK_ALL_CLEAR;
								}
								else if(direction == GPIO_MODE_INPUT_WITH_INTERNAL_PULL_UP_RES)
								{
									/* the port is set to be input with internal pull up resistance */
									_DDRC.Byte = GPIO_PORT_INPUT;
									_PORTC.Byte = GPIO_PORT_MASK_ALL_SET;
								}
								else{ /* Nothing */ }
									
								break;
									
			case GPIO_PORTD	:	/* Port D so we will use DDRD Register to set the direction */
									
								/* check if its output or input */
								if(direction == GPIO_MODE_OUTPUT)
								{
									/* the port is set to be output */
									_DDRD.Byte = GPIO_PORT_OUTPUT;
								}
								else if(direction == GPIO_MODE_INPUT_WITHOUT_INTERNAL_PULL_UP_RES)
								{
									/* the port is set to be input without internal pull up resistance */
									_DDRD.Byte = GPIO_PORT_INPUT;
									_PORTD.Byte = GPIO_PORT_MASK_ALL_CLEAR;
								}
								else if(direction == GPIO_MODE_INPUT_WITH_INTERNAL_PULL_UP_RES)
								{
									/* the port is set to be input with internal pull up resistance */
									_DDRD.Byte = GPIO_PORT_INPUT;
									_PORTD.Byte = GPIO_PORT_MASK_ALL_SET;
								}
								else{ /* Nothing */ }
									
								break;
									
			default	:			/* Nothing */
								break;
		}
	}
	
	return l_status;
}


/**
 * @brief  write logic HIGH/LOW on a specific port
 * @param  (port_num) holds the ID of the port to be set
 * @param  (logic)    holds the logic values (HIGH/LOW) to be written on the port
 * @return (l_status) status of the performed operation
 *              (E_NOT_OK)  operation failed
 *              (E_OK)      operation success
 */
Std_ReturnType GPIO_writePort(uint8 port_num,uint8 logic)
{
	/* create a local variable to hold the status of the performed operation */
    Std_ReturnType l_status = ZERO_INIT;

    /* check if the user exceeded the max number of ports or not */
    if(port_num >= PORT_MAX)
    {
        /* port number inserted higher than the maximum number of ports */
        
        l_status = E_NOK;		/* operation failed */
    }
    else
    {
        /* port number inserted is within the range of ports */

        l_status = E_OK;		/* operation success */
		
		/* set the logic out from the port */
		
		/* check which port is used first */
		switch(port_num)
		{
			case GPIO_PORTA	:	/* Port A so we will use PORTA Register to set the logic */
									
								/* set the logic */
								_PORTA.Byte = logic;
									
								break;
									
			case GPIO_PORTB	:	/* Port B so we will use PORTB Register to set the logic */
									
								/* set the logic */
								_PORTB.Byte = logic;
									
								break;
									
			case GPIO_PORTC	:	/* Port C so we will use PORTC Register to set the logic */
									
								/* set the logic */
								_PORTC.Byte = logic;
									
								break;
									
			case GPIO_PORTD	:	/* Port D so we will use PORTD Register to set the logic */
									
								/* set the logic */
								_PORTD.Byte = logic;
									
								break;
									
			default	:			/* Nothing */
								break;
		}
	}
	
	return l_status;
}


/**
 * @brief  read logic from a specific port
 * @param  (port_num) holds the ID of the port to be set
 * @return (logic)    the logic status of the port
 *              (GPIO_LOW)  	read logic 0 if input port number or pin number are not correct
 */
uint8 GPIO_readPort(uint8 port_num)
{
	/* create a local variable to hold the logic of the port */
	uint8 logic = ZERO_INIT;
	
	/* check if the user exceeded the max number of ports or not */
    if(port_num >= PORT_MAX)
    {
        /* port number inserted higher than the maximum number of ports */
        
        logic = GPIO_PORT_MASK_ALL_CLEAR;
    }
    else
    {
        /* port number inserted is within the range of ports */
		
		/* get the logic of the port */
		
		/* check which port is used first */
		switch(port_num)
		{
			case GPIO_PORTA	:	/* Port A so we will use PINA Register to get the logic */
									
								/* get the logic */
								logic = _PINA.Byte;
									
								break;
									
			case GPIO_PORTB	:	/* Port B so we will use PINB Register to get the logic */
									
								/* get the logic */
								logic = _PINB.Byte;
									
								break;
									
			case GPIO_PORTC	:	/* Port C so we will use PINC Register to get the logic */
									
								/* get the logic */
								logic = _PINC.Byte;
									
								break;
									
			case GPIO_PORTD	:	/* Port D so we will use PIND Register to get the logic */
									
								/* get the logic */
								logic = _PIND.Byte;
									
								break;
									
			default	:			/* Nothing */
								break;
		}
	}
	
	return logic;
}


/* ----------------------------------------------------------------------------------- */
