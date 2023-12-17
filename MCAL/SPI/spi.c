/*
 =========================================================================================
 Name        : spi.c
 Author      : Mohamed Ashraf El-Sayed
 Version     : 1.0.0
 Copyright   : Your copyright notice
 date        : Fri, Oct 20 2023
 time        :
 Description : SPI Driver Source file , Ansi-style
 =========================================================================================
*/

/* ----------------------------------------------------------------------------------- */
/* ------------------Includes section------------------- */
#include <avr/interrupt.h> 			/* For SPI ISR */

#include "spi.h"
#include "common_macros.h"


/* ----------------------------------------------------------------------------------- */
/* -------------------Global section-------------------- */

/* create a pointer to function to hold the address of the call back function */
static void (* SPI_InterruptHandler)(void) = NULL_PTR;


/* ----------------------------------------------------------------------------------- */
/* ------------functions declaration section------------ */


/**
 * @brief  Setup the SPI pins direction depending on the SPI Mode Selected
 * @param  (spi_mode) SPI Mode which is Master or Slave
 * @return (l_status) status of the performed operation
 *              (E_NOK)  	operation failed
 *              (E_OK)      operation success
 */
static Std_ReturnType SPI_GPIO_pinSetup(uint8 spi_mode);


/* ----------------------------------------------------------------------------------- */
/* ------------functions definition section------------- */


/**
 * @brief  initialize SPI :
 * 			1- Select SPI Mode
 * 					- SPI Master
 * 					- SPI Slave
 * 			2- Setup the SPI pins direction
 * 					- SPI Master Mode
 * 						. MOSI >> Output
 * 						. MISO >> Input
 * 						. SCK  >> Output
 * 						. SS   >> Output/Input
 * 					- SPI Slave Mode
 * 					 	. MOSI >> Input
 * 						. MISO >> Output
 * 						. SCK  >> Input
 * 						. SS   >> Input
 * 			3- Select SPI Clock Rate
 * 					- SPI Master Mode
 * 						. Enable/Disable Double Speed Bit(if enabled minimum SCK period will be two CPU clock periods)
 * 					- SPI Slave Mode
 * 						. SPI is only guaranteed to work at fosc/4 or lower
 * 			4- Select Data Order
 * 					- MSB of the data word is transmitted first
 * 					- LSB of the data word is transmitted first
 * 			5- Select Clock Polarity
 * 					- SCK is LOW when Idle
 * 					- SCK is HIGH when Idle
 * 			6- Select Clock Phase
 * 					- data is sampled on the leading (first) edge
 * 					- data is sampled on the trailing (last) edge
 * 			7- Enable/Disable SPI Interrupt
 * 			8- Set SPI Transfer Complete Call Back if it's interrupt is enabled
 * @param  (spi_obj)  pointer to the SPI object passed by reference
 * @return (l_status) status of the performed operation
 *              (E_NOK)  	operation failed
 *              (E_OK)      operation success
 */
Std_ReturnType SPI_init(spi_config_t *spi_obj)
{
	/* create a local variable to hold the status of the performed operation */
	Std_ReturnType l_status = ZERO_INIT;

	/* check if the address is valid or not */
	if(spi_obj == NULL_PTR)
	{
		/* NULL pointer is passed */

		l_status = E_NOK;		/* operation failed */
	}
	else
	{
		/* the address passed is valid */

		l_status = E_OK;		/* operation success */

		/* SPI Init */

		/* --------------------------------- */
		/* 1- Select SPI Mode(Master/Slave) */
		_SPCR._MSTR = spi_obj->mode_select;
		/* --------------------------------- */

		/* --------------------------------- */
		/* 2- Setup the SPI pins direction */
		l_status |= SPI_GPIO_pinSetup(spi_obj->mode_select);
		/* --------------------------------- */

		/* --------------------------------- */
		/* 3- Select SPI Clock Rate */

		switch(spi_obj->clk_rate)
		{
			case SPI_CLOCK_SOURCE_DIV_4			:	_SPSR._SPI2X = RESET; _SPCR._SPR1 = RESET; _SPCR._SPR0 = RESET; break;
			case SPI_CLOCK_SOURCE_DIV_16		:	_SPSR._SPI2X = RESET; _SPCR._SPR1 = RESET; _SPCR._SPR0 = SET; 	break;
			case SPI_CLOCK_SOURCE_DIV_64		:	_SPSR._SPI2X = RESET; _SPCR._SPR1 = SET;   _SPCR._SPR0 = RESET; break;
			case SPI_CLOCK_SOURCE_DIV_128		:	_SPSR._SPI2X = RESET; _SPCR._SPR1 = SET;   _SPCR._SPR0 = SET;   break;
			case SPI_DOUBLE_CLOCK_SOURCE_DIV_2	:	_SPSR._SPI2X = SET;   _SPCR._SPR1 = RESET; _SPCR._SPR0 = RESET; break;
			case SPI_DOUBLE_CLOCK_SOURCE_DIV_8	:	_SPSR._SPI2X = SET;   _SPCR._SPR1 = RESET; _SPCR._SPR0 = SET;   break;
			case SPI_DOUBLE_CLOCK_SOURCE_DIV_32	:	_SPSR._SPI2X = SET;   _SPCR._SPR1 = SET;   _SPCR._SPR0 = RESET; break;
			case SPI_DOUBLE_CLOCK_SOURCE_DIV_64	:	_SPSR._SPI2X = SET;   _SPCR._SPR1 = SET;   _SPCR._SPR0 = SET;   break;
			default 	:	/* Nothing */	break;
		}
		/* --------------------------------- */

		/* --------------------------------- */
		/* 4- Select Data Order */
		_SPCR._DORD = spi_obj->data_order;

		/* 5- Select Clock Polarity */
		_SPCR._CPOL = spi_obj->clk_polarity;

		/* 6- Select Clock Phase */
		_SPCR._CPHA = spi_obj->clk_phase;
		/* --------------------------------- */

		/* --------------------------------- */
		/* 7- Enable/Disable SPI Interrupt */
		_SPCR._SPIE = spi_obj->interrupt_en;

		/* 8- Set SPI Transfer Complete Call Back if it's interrupt is enabled */
		if(spi_obj->interrupt_en == SPI_INTERRUPT_DISABLE)
		{
			/* SPI Interrupt is disabled */
			SPI_InterruptHandler = NULL_PTR;
		}
		else if(spi_obj->interrupt_en == SPI_INTERRUPT_ENABLE)
		{
			/* SPI Interrupt is enabled */
			SPI_InterruptHandler = spi_obj->SPI_DefaultHandler;
		}
		else{ /* Nothing */ }
		/* --------------------------------- */

		/* --------------------------------- */
		/* Enable SPI */
		SPI_ENABLE();
		/* --------------------------------- */
	}

	return l_status;
}


/**
 * @brief  Send data to a device through SPI
 * @param  (data)  the data you want to send
 */
void SPI_sendByte(uint8 data)
{
	/* Dummy Byte to read the SPI Data Register */
	uint8 dummy_byte = ZERO_INIT;

	/* Initiate the communication and send data by SPI */
	_SPDR.Byte = data;

	/* Wait until SPI interrupt flag SPIF = 1 (data has been sent/received correctly) */
	/* When a serial transfer is complete, the SPIF Flag is set */
	while( BIT_IS_CLEARED(_SPSR.Byte,SPIF) );

	/*
	 * Note : the SPIF bit is cleared by first reading the SPI Status Register with SPIF set,
	 *  then accessing the SPI Data Register (SPDR)
	 */

	/* Dummy Read */
	dummy_byte = _SPDR.Byte;
}


/**
 * @brief  Receive data from a device through SPI
 * @return the data you want to receive
 */
uint8 SPI_receiveByte(void)
{
	/* Dummy Byte to write the SPI Data Register */
	uint8 dummy_byte = ZERO_INIT;

	/* Initiate the communication and send data by SPI */
	/* Dummy Write */
	_SPDR.Byte = dummy_byte;

	/* Wait until SPI interrupt flag SPIF = 1 (data has been sent/received correctly) */
	/* When a serial transfer is complete, the SPIF Flag is set */
	while( BIT_IS_CLEARED(_SPSR.Byte,SPIF) );

	/*
	 * Note : the SPIF bit is cleared by first reading the SPI Status Register with SPIF set,
	 *  then accessing the SPI Data Register (SPDR)
	 */

	return (_SPDR.Byte);
}


/**
 * @brief  Send data to and receive data from a device through SPI at the same time
 * @param  (data)  the data you want to send
 * @return the data you want to receive
 */
uint8 SPI_sendReceiveByte(uint8 data)
{
	/* Initiate the communication and send data by SPI */
	_SPDR.Byte = data;

	/* Wait until SPI interrupt flag SPIF = 1 (data has been sent/received correctly) */
	/* When a serial transfer is complete, the SPIF Flag is set */
	while( BIT_IS_CLEARED(_SPSR.Byte,SPIF) );

	/*
	 * Note : the SPIF bit is cleared by first reading the SPI Status Register with SPIF set,
	 *  then accessing the SPI Data Register (SPDR)
	 */

	return (_SPDR.Byte);
}


/**
 * @brief  Send string to another device through SPI
 * @param  (p_str)   pointer to the first character of the string you want to send
 *@return (l_status) status of the performed operation
 *              (E_NOT_OK)  operation failed
 *              (E_OK)      operation success
 */
Std_ReturnType SPI_sendString(const uint8 * const p_str)
{
	/* create a local variable to hold the status of the performed operation */
	Std_ReturnType l_status = ZERO_INIT;

	/* create a local variable to hold the index of character inside the string */
	uint8 char_index = ZERO_INIT;

	/* check if the address is valid or not */
	if(p_str == NULL_PTR)
	{
		/* NULL pointer is passed */

		l_status = E_NOK;		/* operation failed */
	}
	else
	{
		/* the address passed is valid */

		l_status = E_OK;		/* operation success */

		/* send string by sending character at a time */
		while( *(p_str + char_index) != '\0' )
		{
			/* send the character */
			SPI_sendByte( *(p_str + char_index) );
			/* increment the character index to point to the next character */
			char_index++;
		}
	}

	return l_status;
}


/**
 * @brief  Receive string from another device through SPI
 * @param  (p_str)   pointer to the first character of the string you want to receive
 *@return (l_status) status of the performed operation
 *              (E_NOT_OK)  operation failed
 *              (E_OK)      operation success
 */
Std_ReturnType SPI_receiveString(uint8 * const p_str)
{
	/* create a local variable to hold the status of the performed operation */
	Std_ReturnType l_status = ZERO_INIT;

	/* create a local variable to hold the index of character inside the string */
	uint8 char_index = ZERO_INIT;

	/* check if the address is valid or not */
	if(p_str == NULL_PTR)
	{
		/* NULL pointer is passed */

		l_status = E_NOK;		/* operation failed */
	}
	else
	{
		/* the address passed is valid */

		l_status = E_OK;		/* operation success */

		/* receive string by receiving character at a time */

		/* receive the character */
		*(p_str + char_index) = SPI_receiveByte();

		/* Receive the whole string until the '#' */
		while( *(p_str + char_index) != '#' )
		{
			/* increment the character index to point to the next character */
			char_index++;

			/* receive the character */
			*(p_str + char_index) = SPI_receiveByte();
		}

		/* Add NULL Character in the end of the string */
		*(p_str + char_index) = '\0';
	}

	return l_status;
}


/**
 * @brief  Setup the SPI pins direction depending on the SPI Mode Selected
 * @param  (spi_mode) SPI Mode which is Master or Slave
 * @return (l_status) status of the performed operation
 *              (E_NOK)  	operation failed
 *              (E_OK)      operation success
 */
static Std_ReturnType SPI_GPIO_pinSetup(uint8 spi_mode)
{
	/* create a local variable to hold the status of the performed operation */
	Std_ReturnType l_status = E_OK;

	/* create a local object of type gpio_config_t to hold the configurations of SPI pins */
	gpio_config_t spi_pin_obj;

	/* check if SPI is in Master/Slave Mode */
	if(spi_mode == SPI_MODE_MASTER_SELECT)
	{
		/* SPI Master Mode Selected */

		/* MOSI >> Output */
		spi_pin_obj.port = SPI_MOSI_PORT_INDEX;
		spi_pin_obj.pin = SPI_MOSI_PIN_INDEX;
		spi_pin_obj.mode = GPIO_MODE_OUTPUT;

		l_status |= GPIO_setupPinDirection(&spi_pin_obj);

		/* MISO >> Input */
		spi_pin_obj.port = SPI_MISO_PORT_INDEX;
		spi_pin_obj.pin = SPI_MISO_PIN_INDEX;
		spi_pin_obj.mode = GPIO_MODE_INPUT_WITHOUT_INTERNAL_PULL_UP_RES;

		l_status |= GPIO_setupPinDirection(&spi_pin_obj);

		/* SCK  >> Output */
		spi_pin_obj.port = SPI_SCK_PORT_INDEX;
		spi_pin_obj.pin = SPI_SCK_PIN_INDEX;
		spi_pin_obj.mode = GPIO_MODE_OUTPUT;

		l_status |= GPIO_setupPinDirection(&spi_pin_obj);

		/* SS   >> Output/Input */
		spi_pin_obj.port = SPI_SS_PORT_INDEX;
		spi_pin_obj.pin = SPI_SS_PIN_INDEX;
		spi_pin_obj.mode = SPI_SS_MASTER_MODE;

		l_status |= GPIO_setupPinDirection(&spi_pin_obj);
	}
	else if(spi_mode == SPI_MODE_SLAVE_SELECT)
	{
		/* SPI Slave Mode Selected */

		/* MOSI >> Input */
		spi_pin_obj.port = SPI_MOSI_PORT_INDEX;
		spi_pin_obj.pin = SPI_MOSI_PIN_INDEX;
		spi_pin_obj.mode = GPIO_MODE_INPUT_WITHOUT_INTERNAL_PULL_UP_RES;

		l_status |= GPIO_setupPinDirection(&spi_pin_obj);

		/* MISO >> Output */
		spi_pin_obj.port = SPI_MISO_PORT_INDEX;
		spi_pin_obj.pin = SPI_MISO_PIN_INDEX;
		spi_pin_obj.mode = GPIO_MODE_OUTPUT;

		l_status |= GPIO_setupPinDirection(&spi_pin_obj);

		/* SCK  >> Input */
		spi_pin_obj.port = SPI_SCK_PORT_INDEX;
		spi_pin_obj.pin = SPI_SCK_PIN_INDEX;
		spi_pin_obj.mode = GPIO_MODE_INPUT_WITHOUT_INTERNAL_PULL_UP_RES;

		l_status |= GPIO_setupPinDirection(&spi_pin_obj);

		/* SS   >> Input */
		spi_pin_obj.port = SPI_SS_PORT_INDEX;
		spi_pin_obj.pin = SPI_SS_PIN_INDEX;
		spi_pin_obj.mode = GPIO_MODE_INPUT_WITHOUT_INTERNAL_PULL_UP_RES;

		l_status |= GPIO_setupPinDirection(&spi_pin_obj);
	}
	else{ /* Nothing */ }

	return l_status;
}


/* ----------------------------------------------------------------------------------- */
/* --------------------ISR section---------------------- */


/**
 * @brief  SPI Serial Transfer Complete ISR
 */
ISR(SPI_STC_vect)
{
	/* check if the call back notification contains NULL or not */
	if(SPI_InterruptHandler)
	{
		/* Call Back */
		(*SPI_InterruptHandler)();
	}
	else{ /* Nothing */ }
}


/* ----------------------------------------------------------------------------------- */
