/*
 =========================================================================================
 Name        : ext_eeprom.h
 Author      : Mohamed Ashraf El-Sayed
 Version     : 1.0.0
 Copyright   : Your copyright notice
 date        : Thu, Oct 26 2023
 time        :
 Description : EXTERNAL EEPROM Driver Header file , Ansi-style
 =========================================================================================
*/

#ifndef _EXT_EEPROM_H_
#define _EXT_EEPROM_H_
/* ----------------------------------------------------------------------------------- */
/* ------------------Includes section------------------- */
#include "std_types.h"


/* ----------------------------------------------------------------------------------- */
/* ------------functions declaration section------------ */


/**
 * @brief  Send Byte to the External EEPROM
 * @param  (address) the address you want to write in the EEPROM
 * @param  (data)    the data you want to write inside that address
 * @return (l_status) status of the performed operation
 *              (E_NOK)  	operation failed
 *              (E_OK)      operation success
 */
Std_ReturnType EXT_EEPROM_writeByte(uint16 address,uint8 data);


/**
 * @brief  Receive Byte from the External EEPROM
 * @param  (address) the address you want to receive from the EEPROM
 * @param  (p_data)  pointer to the data received from that address inside the EEPROM
 * @return (l_status) status of the performed operation
 *              (E_NOK)  	operation failed
 *              (E_OK)      operation success
 */
Std_ReturnType EXT_EEPROM_readByte(uint16 address,uint8 *p_data);


/* ----------------------------------------------------------------------------------- */
#endif /* _EXT_EEPROM_H_ */
