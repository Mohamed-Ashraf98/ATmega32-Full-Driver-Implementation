/*
 =========================================================================================
 Name        : ext_eeprom.c
 Author      : Mohamed Ashraf El-Sayed
 Version     : 1.0.0
 Copyright   : Your copyright notice
 date        : Thu, Oct 26 2023
 time        :
 Description : EXTERNAL EEPROM Driver Source file , Ansi-style
 =========================================================================================
*/

/* ----------------------------------------------------------------------------------- */
/* ------------------Includes section------------------- */
#include "ext_eeprom.h"
#include "i2c.h"


/* ----------------------------------------------------------------------------------- */
/* ------------functions definition section------------- */


/**
 * @brief  Send Byte to the External EEPROM
 * @param  (address) the address you want to write in the EEPROM
 * @param  (data)    the data you want to write inside that address
 * @return (l_status) status of the performed operation
 *              (E_NOK)  	operation failed
 *              (E_OK)      operation success
 */
Std_ReturnType EXT_EEPROM_writeByte(uint16 address,uint8 data)
{
	/* Send the Start Bit */
	I2C_start();
    if (I2C_getStatus() != I2C_STATUS_START_CONDITION_TRANSMITTED)
        return E_NOK;

    /* Send the device address, we need to get A8 A9 A10 address bits from the
     * memory location address and R/W=0 (write) */
    I2C_writeByte((uint8)(0xA0 | ((address & 0x0700)>>7)));
    if (I2C_getStatus() != I2C_STATUS_SLA_W_TRANSMITTED_ACK_RECEIVED)
        return E_NOK;

    /* Send the required memory location address */
    I2C_writeByte((uint8)(address));
    if (I2C_getStatus() != I2C_STATUS_DATA_TRANSMITTED_ACK_RECEIVED)
        return E_NOK;

    /* write byte to eeprom */
    I2C_writeByte(data);
    if (I2C_getStatus() != I2C_STATUS_DATA_TRANSMITTED_ACK_RECEIVED)
        return E_NOK;

    /* Send the Stop Bit */
    I2C_stop();

    return E_OK;
}


/**
 * @brief  Receive Byte from the External EEPROM
 * @param  (address) the address you want to receive from the EEPROM
 * @param  (p_data)  pointer to the data received from that address inside the EEPROM
 * @return (l_status) status of the performed operation
 *              (E_NOK)  	operation failed
 *              (E_OK)      operation success
 */
Std_ReturnType EXT_EEPROM_readByte(uint16 address,uint8 *p_data)
{
	/* Send the Start Bit */
	I2C_start();
	if (I2C_getStatus() != I2C_STATUS_START_CONDITION_TRANSMITTED)
		return E_NOK;

	/* Send the device address, we need to get A8 A9 A10 address bits from the
	 * memory location address and R/W=0 (write) */
	I2C_writeByte((uint8)(0xA0 | ((address & 0x0700)>>7)));
	if (I2C_getStatus() != I2C_STATUS_SLA_W_TRANSMITTED_ACK_RECEIVED)
		return E_NOK;

	/* Send the required memory location address */
	I2C_writeByte((uint8)(address));
	if (I2C_getStatus() != I2C_STATUS_DATA_TRANSMITTED_ACK_RECEIVED)
		return E_NOK;

    /* Send the Repeated Start Bit */
	I2C_start();
    if (I2C_getStatus() != I2C_STATUS_REPEATED_START_CONDITION_TRANSMITTED)
        return E_NOK;

    /* Send the device address, we need to get A8 A9 A10 address bits from the
     * memory location address and R/W=1 (Read) */
    I2C_writeByte((uint8)((0xA0) | ((address & 0x0700)>>7) | 1));
    if (I2C_getStatus() != I2C_STATUS_SLA_R_TRANSMITTED_ACK_RECEIVED)
        return E_NOK;

    /* Read Byte from Memory without send ACK */
    *p_data = I2C_readByteWithNACK();
    if (I2C_getStatus() != I2C_STATUS_DATA_RECEIVED_NACK_TRANSMITTED)
        return E_NOK;

    /* Send the Stop Bit */
	I2C_stop();

	return E_OK;
}


/* ----------------------------------------------------------------------------------- */
