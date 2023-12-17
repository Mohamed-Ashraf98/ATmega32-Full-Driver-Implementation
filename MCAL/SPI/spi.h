/*
 =========================================================================================
 Name        : spi.h
 Author      : Mohamed Ashraf El-Sayed
 Version     : 1.0.0
 Copyright   : Your copyright notice
 date        : Fri, Oct 20 2023
 time        :
 Description : SPI Driver Header file , Ansi-style
 =========================================================================================
*/

#ifndef _SPI_H_
#define _SPI_H_
/* ----------------------------------------------------------------------------------- */
/* ------------------Includes section------------------- */
#include "ATmega32.h"
#include "gpio.h"					/* for SPI pin configurations */


/* ----------------------------------------------------------------------------------- */
/* --------------Macro declaration section-------------- */


/* --------------------------------- */
/* Enable/Disable SPI Interrupt */

/*
 * MCU Data Sheet :
 * SPI Control Register – SPCR
 * Bit 7 – SPIE: SPI Interrupt Enable
 *
 * This bit causes the SPI interrupt to be executed if SPIF bit in the SPSR Register is set and the if
 *	the global interrupt enable bit in SREG is set.
*/

/* @ref : SPIE: SPI Interrupt Enable */
#define SPI_INTERRUPT_DISABLE					0
#define SPI_INTERRUPT_ENABLE					1

/* --------------------------------- */
/* SPI Data Order */

/*
 * MCU Data Sheet :
 * SPI Control Register – SPCR
 * Bit 5 – DORD: Data Order
 *
 * When the DORD bit is written to one, the LSB of the data word is transmitted first.
 * When the DORD bit is written to zero, the MSB of the data word is transmitted first.
*/

/* @ref : DORD: Data Order */
#define SPI_DATA_ORDER_MSB_TRANSMITTED_FIRST	0
#define SPI_DATA_ORDER_LSB_TRANSMITTED_FIRST	1

/* --------------------------------- */
/* SPI Master/Slave Mode Select */

/*
 * MCU Data Sheet :
 * SPI Control Register – SPCR
 * Bit 4 – MSTR: Master/Slave Select
 *
 * This bit selects Master SPI mode when written to one, and Slave SPI mode when written logic
 *  zero. If SS is configured as an input and is driven low while MSTR is set, MSTR will be cleared,
 *	and SPIF in SPSR will become set. The user will then have to set MSTR to re-enable SPI Master mode.
*/

/* @ref : MSTR: Master/Slave Select */
#define SPI_MODE_SLAVE_SELECT					0
#define SPI_MODE_MASTER_SELECT					1

/* --------------------------------- */
/* SPI Clock Polarity */

/*
 * MCU Data Sheet :
 * SPI Control Register – SPCR
 * Bit 3 – CPOL: Clock Polarity
 *
 * When this bit is written to one, SCK is high when idle. When CPOL is written to zero, SCK is low when idle.
 * 		CPOL 			Leading Edge 			Trailing Edge
 * 		----			------------			-------------
 * 		0 				Rising 					Falling
 * 		1 				Falling 				Rising
*/

/* @ref : CPOL: Clock Polarity */
#define SPI_CLK_POLARITY_IDLE_LOW				0
#define SPI_CLK_POLARITY_IDLE_HIGH				1

/* --------------------------------- */
/* SPI Clock Phase */

/*
 * MCU Data Sheet :
 * SPI Control Register – SPCR
 * Bit 2 – CPHA: Clock Phase
 *
 * The settings of the Clock Phase bit (CPHA) determine if data is sampled on the leading (first) or
 *  trailing (last) edge of SCK.
 * 		CPHA 			Leading Edge 			Trailing Edge
 * 		----			------------			-------------
 * 		0 				Sample 					Setup
 * 		1 				Setup 					Sample
*/

/* @ref : CPHA: Clock Phase */
#define SPI_CLK_PHASE_SAMPLE_LEADING_EDGE		0
#define SPI_CLK_PHASE_SAMPLE_TRAILING_EDGE		1

/* --------------------------------- */
/* SPI pins index */

/* --SS-- */
#define SPI_SS_PORT_INDEX						GPIO_PORTB
#define SPI_SS_PIN_INDEX						GPIO_PIN4
/* Set SPI SS pin in Master Mode to be Output/Input */
#define SPI_SS_MASTER_MODE						GPIO_MODE_OUTPUT

/* --MOSI-- */
#define SPI_MOSI_PORT_INDEX						GPIO_PORTB
#define SPI_MOSI_PIN_INDEX						GPIO_PIN5

/* --MISO-- */
#define SPI_MISO_PORT_INDEX						GPIO_PORTB
#define SPI_MISO_PIN_INDEX						GPIO_PIN6

/* --SCK-- */
#define SPI_SCK_PORT_INDEX						GPIO_PORTB
#define SPI_SCK_PIN_INDEX						GPIO_PIN7

/* --------------------------------- */


/* ----------------------------------------------------------------------------------- */
/* --------Macro functions declaration section---------- */


/* --------------------------------- */
/* SPI Enable/Disable */

/*
 * MCU Data Sheet :
 * SPI Control Register – SPCR
 * Bit 6 – SPE: SPI Enable
 *
 * When the SPE bit is written to one, the SPI is enabled. This bit must be set to enable any SPI
 *	operations.
*/

#define SPI_DISABLE()							(_SPCR._SPE = RESET)
#define SPI_ENABLE()							(_SPCR._SPE = SET)

/* --------------------------------- */


/* ----------------------------------------------------------------------------------- */
/* -----user_defined data type declaration section------ */


/* --------------------------------- */
/* SPI config structure */

typedef struct{
	 /* pointer to function to hold the function called in the APPLICATION layer when a serial transfer is complete if it's Interrupt is enabled */
	void (* SPI_DefaultHandler)(void);

	/* Selects SPI to be Master or Slave >> @ref : MSTR: Master/Slave Select */
	uint8 mode_select	:1;
	/* Selects SPI Clock Polarity >> @ref : CPOL: Clock Polarity */
	uint8 clk_polarity	:1;
	/* Selects SPI Clock Phase >> @ref : CPHA: Clock Phase */
	uint8 clk_phase		:1;
	/* Selects SPI Clock Rate >> @ref : spi_clk_rate_select_t */
	uint8 clk_rate		:3;
	/* Selects SPI Data Order(MSB or LSB) first >> @ref : DORD: Data Order */
	uint8 data_order	:1;

	/* Enable/Disable SPI Interrupt >> @ref : SPIE: SPI Interrupt Enable */
	uint8 interrupt_en	:1;
}spi_config_t;

/* --------------------------------- */
/* SPI Clock Rate Select */

/*
 * MCU Data Sheet :
 *
 * SPI Status Register – SPSR
 * Bit 0 – SPI2X: Double SPI Speed Bit
 *
 * When this bit is written logic one the SPI speed (SCK Frequency) will be doubled when the SPI
 *	is in Master mode. This means that the minimum SCK period will be two CPU
 *	clock periods. When the SPI is configured as Slave, the SPI is only guaranteed to work at fosc/4 or lower.
 *
 * SPI Control Register – SPCR
 * Bits 1, 0 – SPR1, SPR0: SPI Clock Rate Select 1 and 0
 *
 * These two bits control the SCK rate of the device configured as a Master. SPR1 and SPR0 have
 *	no effect on the Slave. The relationship between SCK and the Oscillator Clock frequency fosc is
 *	shown in the following table:
 *		SPI2X 				SPR1 				SPR0 				SCK Frequency
 *		-----				----				----				-------------
 *		0 					0 					0 					fosc/4
 *		0 					0 					1 					fosc/16
 *		0 					1 					0 					fosc/64
 *		0 					1 					1 					fosc/128
 *		1 					0 					0 					fosc/2
 *		1 					0 					1 					fosc/8
 *		1 					1 					0 					fosc/32
 *		1 					1 					1 					fosc/64
*/

/* @ref : spi_clk_rate_select_t */
typedef enum{
	SPI_CLOCK_SOURCE_DIV_4 = 0,
	SPI_CLOCK_SOURCE_DIV_16,
	SPI_CLOCK_SOURCE_DIV_64,
	SPI_CLOCK_SOURCE_DIV_128,
	SPI_DOUBLE_CLOCK_SOURCE_DIV_2,
	SPI_DOUBLE_CLOCK_SOURCE_DIV_8,
	SPI_DOUBLE_CLOCK_SOURCE_DIV_32,
	SPI_DOUBLE_CLOCK_SOURCE_DIV_64
}spi_clk_rate_select_t;

/* --------------------------------- */


/* ----------------------------------------------------------------------------------- */
/* ------------functions declaration section------------ */


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
Std_ReturnType SPI_init(spi_config_t *spi_obj);


/**
 * @brief  Send data to a device through SPI
 * @param  (data)  the data you want to send
 */
void SPI_sendByte(uint8 data);


/**
 * @brief  Receive data from a device through SPI
 * @return the data you want to receive
 */
uint8 SPI_receiveByte(void);


/**
 * @brief  Send data to and receive data from a device through SPI at the same time
 * @param  (data)  the data you want to send
 * @return the data you want to receive
 */
uint8 SPI_sendReceiveByte(uint8 data);


/**
 * @brief  Send string to another device through SPI
 * @param  (p_str)   pointer to the first character of the string you want to send
 *@return (l_status) status of the performed operation
 *              (E_NOT_OK)  operation failed
 *              (E_OK)      operation success
 */
Std_ReturnType SPI_sendString(const uint8 * const p_str);


/**
 * @brief  Receive string from another device through SPI
 * @param  (p_str)   pointer to the first character of the string you want to receive
 *@return (l_status) status of the performed operation
 *              (E_NOT_OK)  operation failed
 *              (E_OK)      operation success
 */
Std_ReturnType SPI_receiveString(uint8 * const p_str);


/* ----------------------------------------------------------------------------------- */
#endif /* _SPI_H_ */
