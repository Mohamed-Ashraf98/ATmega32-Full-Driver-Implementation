/*
 =========================================================================================
 Name        : i2c.c
 Author      : Mohamed Ashraf El-Sayed
 Version     : 1.0.0
 Copyright   : Your copyright notice
 date        : Thu, Oct 26 2023
 time        :
 Description : I2C Driver Source file , Ansi-style
 =========================================================================================
*/

/* ----------------------------------------------------------------------------------- */
/* ------------------Includes section------------------- */
#include <avr/interrupt.h> 			/* For SPI ISR */

#include "i2c.h"


/* ----------------------------------------------------------------------------------- */
/* -------------------Global section-------------------- */

/* create a pointer to function to hold the address of the call back function */
static void (* I2C_InterruptHandler)(void) = NULL_PTR;


/* ----------------------------------------------------------------------------------- */
/* ------------functions definition section------------- */


/**
 * @brief  initialize I2C :
 * 			1- Set the division factor for the bit rate generator using :
 * 				- CPU Clock Frequency (F_CPU)
 * 				- I2C Bit Rate Speed
 * 					. Standard Mode
 * 					. Fast Mode
 * 					. Fast Mode Plus
 * 					. High Speed Mode
 * 				- Value of the prescaler bits in the TWI Status Register
 * 					. Prescaler Value = 1
 * 					. Prescaler Value = 4
 * 					. Prescaler Value = 16
 * 					. Prescaler Value = 64
 * 			2- Disable Acknowledge Bit
 * 			3- Enable/Disable General Call
 * 			4- Set the I2C Own Slave Address
 * 			5- Enable/Disable I2C Interrupt
 * 			6- Set the I2C Call Back if it's interrupt is enabled
 * 			7- Enable I2C Module
 * @param  (i2c_obj)  pointer to the I2C object passed by reference
 * @return (l_status) status of the performed operation
 *              (E_NOK)  	operation failed
 *              (E_OK)      operation success
 */
Std_ReturnType I2C_init(i2c_config_t *i2c_obj)
{
	/* create a local variable to hold the status of the performed operation */
	Std_ReturnType l_status = ZERO_INIT;

	/* check if the address is valid or not */
	if(i2c_obj == NULL_PTR)
	{
		/* NULL pointer is passed */

		l_status = E_NOK;		/* operation failed */
	}
	else
	{
		/* the address passed is valid */

		l_status = E_OK;		/* operation success */

		/* I2C Init */

		/* --------------------------------- */
		/* 1- Set the division factor for the bit rate generator */

		/* Select the value of the prescaler bits in the TWI Status Register */
		_TWSR._TWPSx = i2c_obj->bit_rate;

		/* Set the value of the TWI Bit Rate Register */
		I2C_setBitRate(I2C_BIT_RATE);
		/* --------------------------------- */

		/* --------------------------------- */
		/* 2- Disable Acknowledge Bit */
		_TWCR._TWEA = I2C_ACK_BIT_DISABLE;

		/* 3- Enable/Disable General Call */
		_TWAR._TWGCE = i2c_obj->general_call_en;

		/* 4- Set the I2C Own Slave Address */
		_TWAR._TWAx = I2C_SLAVE_ADDRESS;
		/* --------------------------------- */

		/* --------------------------------- */
		/* 5- Enable/Disable I2C Interrupt */
		_TWCR._TWIE = i2c_obj->interrupt_en;

		/* 6- Set the I2C Call Back if it's interrupt is enabled */
		if(i2c_obj->interrupt_en == I2C_INTERRUPT_DISABLE)
		{
			/* I2C Interrupt is disabled */
			I2C_InterruptHandler = NULL_PTR;
		}
		else if(i2c_obj->interrupt_en == I2C_INTERRUPT_ENABLE)
		{
			/* I2C Interrupt is enabled */
			I2C_InterruptHandler = i2c_obj->I2C_DefaultHandler;
		}
		else{ /* Nothing */ }
		/* --------------------------------- */

		/* --------------------------------- */
		/* 7- Enable I2C Module */
		I2C_ENABLE();
		/* --------------------------------- */
	}

	return l_status;
}


/**
 * @brief  Send Start Condition through I2C Bus
 */
void I2C_start(void)
{
    /*
	 * Clear the TWINT flag before sending the start bit TWINT=1
	 * send the start bit by TWSTA=1
	 * Enable TWI Module TWEN=1
	 */
    TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);

    /* Wait for TWINT flag set in TWCR Register (start bit is send successfully) */
    while( !(_TWCR._TWINT) );
}


/**
 * @brief  Send Stop Condition through I2C Bus
 */
void I2C_stop(void)
{
	/*
	 * Clear the TWINT flag before sending the stop bit TWINT=1
	 * send the stop bit by TWSTO=1
	 * Enable TWI Module TWEN=1
	*/
	TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);
}


/**
 * @brief  Send data through I2C Bus
 * @param  (data)  the data you want to send
 */
void I2C_writeByte(uint8 data)
{
    /* Put data On TWI data Register */
    _TWDR.Byte = data;

    /*
	 * Clear the TWINT flag before sending the data TWINT=1
	 * Enable TWI Module TWEN=1
	 */
    TWCR = (1 << TWINT) | (1 << TWEN);

    /* Wait for TWINT flag set in TWCR Register(data is send successfully) */
    while( !(_TWCR._TWINT) );
}


/**
 * @brief  Receive data through I2C Bus and send ACK
 * @return the data you want to receive
 */
uint8 I2C_readByteWithACK(void)
{
	/*
	 * Clear the TWINT flag before reading the data TWINT=1
	 * Enable sending ACK after reading or receiving data TWEA=1
	 * Enable TWI Module TWEN=1
	 */
    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);

    /* Wait for TWINT flag set in TWCR Register (data received successfully) */
    while( !(_TWCR._TWINT) );

    /* Read Data */
    return (_TWDR.Byte);
}


/**
 * @brief  Receive data through I2C Bus and send NACK
 * @return the data you want to receive
 */
uint8 I2C_readByteWithNACK(void)
{
	/*
	 * Clear the TWINT flag before reading the data TWINT=1
	 * Enable TWI Module TWEN=1
	 */
    TWCR = (1 << TWINT) | (1 << TWEN);

    /* Wait for TWINT flag set in TWCR Register (data received successfully) */
    while( !(_TWCR._TWINT) );

    /* Read Data */
    return (_TWDR.Byte);
}


/**
 * @brief  Get the Status of T=the I2C Bus operation
 * @return the status of I2C
 */
uint8 I2C_getStatus(void)
{
    /* masking to eliminate first 3 bits and get the last 5 bits (status bits) */
    return ( (_TWSR.Byte) & 0xF8 );
}


/**
 * @brief  Set the value of the TWI Bit Rate Register
 * @param  (i2c_speed) I2C Speed Transmitt
 */
void I2C_setBitRate(uint32 i2c_speed)
{
	/* check which I2C Bit Rate Prescaler is selected */
	switch(_TWSR._TWPSx)
	{
		case I2C_CLOCK_SOURCE_DIV_1		:	/* Prescaler Value = 1 */
											_TWBR.Byte = (uint8)( ( ( (CPU_FREQUENCY) / (i2c_speed) ) - 16 ) / (2.0 * 1) );
											break;

		case I2C_CLOCK_SOURCE_DIV_4		:	/* Prescaler Value = 4 */
											_TWBR.Byte = (uint8)( ( ( (CPU_FREQUENCY) / (i2c_speed) ) - 16 ) / (2.0 * 4) );
											break;

		case I2C_CLOCK_SOURCE_DIV_16	:	/* Prescaler Value = 16 */
											_TWBR.Byte = (uint8)( ( ( (CPU_FREQUENCY) / (i2c_speed) ) - 16 ) / (2.0 * 16) );
											break;

		case I2C_CLOCK_SOURCE_DIV_64	:	/* Prescaler Value = 64 */
											_TWBR.Byte = (uint8)( ( ( (CPU_FREQUENCY) / (i2c_speed) ) - 16 ) / (2.0 * 64) );
											break;

		default	:	/* Nothing */
					break;
	}
}


/* ----------------------------------------------------------------------------------- */
/* --------------------ISR section---------------------- */


/**
 * @brief  Two-wire Serial Interface (TWI/I2C) ISR
 */
ISR(TWI_vect)
{
	/* Clear the TWINT flag before sending the start bit TWINT=1 */
	_TWCR._TWINT = SET;

	/* check if the call back notification contains NULL or not */
	if(I2C_InterruptHandler)
	{
		/* Call Back */
		(*I2C_InterruptHandler)();
	}
	else{ /* Nothing */ }
}


/* ----------------------------------------------------------------------------------- */
