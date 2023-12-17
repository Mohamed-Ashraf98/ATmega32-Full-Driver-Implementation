/*
 =========================================================================================
 Name        : i2c.h
 Author      : Mohamed Ashraf El-Sayed
 Version     : 1.0.0
 Copyright   : Your copyright notice
 date        : Thu, Oct 26 2023
 time        :
 Description : I2C Driver Header file , Ansi-style
 =========================================================================================
*/

#ifndef _I2C_H_
#define _I2C_H_
/* ----------------------------------------------------------------------------------- */
/* ------------------Includes section------------------- */
#include "ATmega32.h"


/* ----------------------------------------------------------------------------------- */
/* --------------Macro declaration section-------------- */


/* --------------------------------- */
/* I2C Configuration */

/* I2C Device's Own Slave Address */
#define I2C_SLAVE_ADDRESS						0x01

/* I2C Bit Rate */
#define I2C_BIT_RATE							I2C_FAST_MODE

/* --------------------------------- */

/* --------------------------------- */
/* Enable/Disable I2C Acknowledge Bit */

/*
 * MCU Data Sheet :
 * TWI Control Register – TWCR
 * Bit 6 – TWEA: TWI Enable Acknowledge Bit
 *
 * The TWEA bit controls the generation of the acknowledge pulse. If the TWEA bit is written to
 *  one, the ACK pulse is generated on the TWI bus if the following conditions are met:
 *		1. The device’s own slave address has been received.
 *		2. A general call has been received, while the TWGCE bit in the TWAR is set.
 *		3. A data byte has been received in Master Receiver or Slave Receiver mode.
 *
 * By writing the TWEA bit to zero, the device can be virtually disconnected from the Two-wire
 *  Serial Bus temporarily. Address recognition can then be resumed by writing the TWEA bit to one again.
*/

#define I2C_ACK_BIT_DISABLE						0
#define I2C_ACK_BIT_ENABLE						1

/* --------------------------------- */

/* --------------------------------- */
/* Enable/Disable I2C Interrupt */

/*
 * MCU Data Sheet :
 * TWI Control Register – TWCR
 * Bit 0 – TWIE: TWI Interrupt Enable
 *
 * When this bit is written to one, and the I-bit in SREG is set, the TWI interrupt request will be activated
 *  for as long as the TWINT Flag is high.
*/

/* @ref : TWIE: TWI Interrupt Enable */
#define I2C_INTERRUPT_DISABLE					0
#define I2C_INTERRUPT_ENABLE					1

/* --------------------------------- */

/* --------------------------------- */
/* Enable/Disable I2C General Call Recognition */

/*
 * MCU Data Sheet :
 * TWI (Slave) Address Register – TWAR
 * Bit 0 – TWGCE: TWI General Call Recognition Enable Bit
 *
 * If set, this bit enables the recognition of a General Call given over the Two-wire Serial Bus.
*/

/* @ref : TWGCE: TWI General Call Recognition Enable Bit */
#define I2C_GENERAL_CALL_DISABLE				0
#define I2C_GENERAL_CALL_ENABLE					1

/* --------------------------------- */

/* --------------------------------- */
/* I2C Transmit Speed */

/* Standard Mode (SM) up to 100 Kbit/Sec */
#define I2C_STANDARD_MODE						(uint32)100000
/* Fast Mode (FM) up to 400 Kbit/Sec */
#define I2C_FAST_MODE							(uint32)400000
/* Fast Mode Plus (FM+) up to 1 Mbit/Sec */
#define I2C_FAST_MODE_PLUS						(uint32)1000000
/* High Speed Mode (HSM) up to 3.4 Mbit/Sec */
#define I2C_HIGH_SPEED_MODE						(uint32)3400000

/* --------------------------------- */

/* --------------------------------- */
/* I2C status */

/*
 * MCU Data Sheet :
 * TWI Status Register – TWSR
 * Bits 7..3 – TWS: TWI Status
 *
 * These five bits reflect the status of the TWI logic and the Two-wire Serial Bus. The different status
 *  codes are described later in this section. Note that the value read from TWSR contains both
 *  the 5-bit status value and the 2-bit prescaler value. The application designer should mask the
 *  prescaler bits to zero when checking the Status bits. This makes status checking independent of
 *  prescaler setting. This approach is used in this datasheet, unless otherwise noted.
*/

/* A START condition has been transmitted */
#define I2C_STATUS_START_CONDITION_TRANSMITTED					0x08

/* A repeated START condition has been transmitted */
#define I2C_STATUS_REPEATED_START_CONDITION_TRANSMITTED			0x10

/* SLA+W has been transmitted; ACK has been received */
#define I2C_STATUS_SLA_W_TRANSMITTED_ACK_RECEIVED				0x18

/* SLA+W has been transmitted; NOT ACK has been received */
#define I2C_STATUS_SLA_W_TRANSMITTED_NACK_RECEIVED				0x20

/* Data byte has been transmitted; ACK has been received */
#define I2C_STATUS_DATA_TRANSMITTED_ACK_RECEIVED				0x28

/* Data byte has been transmitted; NOT ACK has been received */
#define I2C_STATUS_DATA_TRANSMITTED_NACK_RECEIVED				0x30

/* Arbitration lost in SLA+W or data bytes */
#define I2C_STATUS_ARBITRATION_LOST_IN_SLA_W_OR_DATA			0x38
/* Arbitration lost in SLA+R or NOT ACK bit */
#define I2C_STATUS_ARBITRATION_LOST_IN_SLA_R_OR_NACK			0x38

/* SLA+R has been transmitted; ACK has been received */
#define I2C_STATUS_SLA_R_TRANSMITTED_ACK_RECEIVED				0x40

/* SLA+R has been transmitted; NOT ACK has been received */
#define I2C_STATUS_SLA_R_TRANSMITTED_NACK_RECEIVED				0x48

/* Data byte has been received; ACK has been returned */
#define I2C_STATUS_DATA_RECEIVED_ACK_TRANSMITTED				0x50

/* Data byte has been received; NOT ACK has been returned */
#define I2C_STATUS_DATA_RECEIVED_NACK_TRANSMITTED				0x58

/* --------------------------------- */


/* ----------------------------------------------------------------------------------- */
/* --------Macro functions declaration section---------- */


/* --------------------------------- */
/* I2C Enable/Disable */

/*
 * MCU Data Sheet :
 * TWI Control Register – TWCR
 * Bit 2 – TWEN: TWI Enable Bit
 *
 * The TWEN bit enables TWI operation and activates the TWI interface. When TWEN is written to
 *  one, the TWI takes control over the I/O pins connected to the SCL and SDA pins, enabling the
 *  slew-rate limiters and spike filters. If this bit is written to zero, the TWI is switched off and all TWI
 *  transmissions are terminated, regardless of any ongoing operation.
*/

#define I2C_DISABLE()							(_TWCR._TWEN = RESET)
#define I2C_ENABLE()							(_TWCR._TWEN = SET)

/* --------------------------------- */


/* ----------------------------------------------------------------------------------- */
/* -----user_defined data type declaration section------ */


/* --------------------------------- */
/* I2C config structure */

typedef struct{
	/* pointer to function to hold the function called in the APPLICATION layer when I2C has finished its current job and expects application software response if it's Interrupt is enabled */
	void (* I2C_DefaultHandler)(void);

	/* Selects I2C Bit Rate Prescaler >> @ref : i2c_bit_rate_select_t */
	uint8 bit_rate			:2;

	/* Enable/Disable I2C Interrupt >> @ref : TWIE: TWI Interrupt Enable */
	uint8 interrupt_en		:1;

	/* Enable/Disable I2C General Call >> @ref : TWGCE: TWI General Call Recognition Enable Bit */
	uint8 general_call_en	:1;

	/* Reserved */
	uint8					:4;
}i2c_config_t;

/* --------------------------------- */

/* --------------------------------- */
/* I2C Control Bit Rate Prescaler */

/*
 * MCU Data Sheet :
 *
 * TWI Status Register – TWSR
 * Bits 1..0 – TWPS: TWI Prescaler Bits
 *
 * These bits can be read and written, and control the bit rate prescaler.
 *	shown in the following table:
 *		TWPS1 				TWPS0 				Prescaler Value
 *		-----				-----				---------------
 *		  0					  0						1
 *		  0					  1						4
 *		  1					  0						16
 *		  1					  1						64
*/

/* @ref : i2c_bit_rate_select_t */
typedef enum{
	I2C_CLOCK_SOURCE_DIV_1 = 0,
	I2C_CLOCK_SOURCE_DIV_4,
	I2C_CLOCK_SOURCE_DIV_16,
	I2C_CLOCK_SOURCE_DIV_64
}i2c_bit_rate_select_t;

/* --------------------------------- */


/* ----------------------------------------------------------------------------------- */
/* ------------functions declaration section------------ */


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
Std_ReturnType I2C_init(i2c_config_t *i2c_obj);


/**
 * @brief  Send Start Condition through I2C Bus
 */
void I2C_start(void);


/**
 * @brief  Send Stop Condition through I2C Bus
 */
void I2C_stop(void);


/**
 * @brief  Send data through I2C Bus
 * @param  (data)  the data you want to send
 */
void I2C_writeByte(uint8 data);


/**
 * @brief  Receive data through I2C Bus and send ACK
 * @return the data you want to receive
 */
uint8 I2C_readByteWithACK(void);


/**
 * @brief  Receive data through I2C Bus and send NACK
 * @return the data you want to receive
 */
uint8 I2C_readByteWithNACK(void);


/**
 * @brief  Get the Status of T=the I2C Bus operation
 * @return the status of I2C
 */
uint8 I2C_getStatus(void);


/**
 * @brief  Set the value of the TWI Bit Rate Register
 * @param  (i2c_speed) I2C Speed Transmitt
 */
void I2C_setBitRate(uint32 i2c_speed);


/* ----------------------------------------------------------------------------------- */
#endif /* _I2C_H_ */
