/*
 =========================================================================================
 Name        : usart.h
 Author      : Mohamed Ashraf El-Sayed
 Version     : 1.0.0
 Copyright   : Your copyright notice
 date        : Mon, Oct 16 2023
 time        :
 Description : USART ASYNCHRONOUS Mode Driver Header file , Ansi-style
 =========================================================================================
*/

#ifndef _USART_H_
#define _USART_H_
/* ----------------------------------------------------------------------------------- */
/* ------------------Includes section------------------- */
#include "ATmega32.h"


/* ----------------------------------------------------------------------------------- */
/* --------------Macro declaration section-------------- */


/* --------------------------------- */
/* USART Configurations Enable/Disable */

#define USART_CFG_DISABLE									0
#define USART_CFG_ENABLE									1

/* --------------------------------- */
/* Enable/Disable USART TX/RX Interrupts */

#define USART_CFG_RX_COMPLETE_INTERRUPT						USART_CFG_ENABLE
#define USART_CFG_TX_COMPLETE_INTERRUPT						USART_CFG_DISABLE
#define USART_CFG_TX_BUFFER_EMPTY_INTERRUPT					USART_CFG_DISABLE

/* --------------------------------- */
/* USART Transmit/Receive Flags */

/* RXC: USART Receive Complete */
#define USART_RECEIVE_BUFFER_FULL							1
#define USART_RECEIVE_BUFFER_EMPTY							0

/* TXC: USART Transmit Complete */
#define USART_TRANSMIT_SHIFT_REGISTER_EMPTY					1
#define USART_TRANSMIT_SHIFT_REGISTER_NOT_EMPTY				0

/* UDRE: USART Data Register Empty */
#define USART_TRANSMIT_BUFFER_EMPTY							1
#define USART_TRANSMIT_BUFFER_NOT_EMPTY						0

/* UART Error Flags */

/* FE: Frame Error */
#define UART_FRAME_ERROR_DETECTED							1
#define UART_FRAME_ERROR_NOT_DETECTED						0

/* DOR: Data OverRun */
#define UART_DATA_OVERRUN_DETECTED							1
#define UART_DATA_OVERRUN_NOT_DETECTED						0

/* PE: Parity Error */
#define UART_PARITY_ERROR_DETECTED							1
#define UART_PARITY_ERROR_NOT_DETECTED						0

/* --------------------------------- */
/* Enable/Disable UART TX/RX Interrupts */

/* @ref : RXCIE: RX Complete Interrupt Enable */
#define USART_RX_COMPLETE_INTERRUPT_DISABLE					0
#define USART_RX_COMPLETE_INTERRUPT_ENABLE					1

/* @ref : TXCIE: TX Complete Interrupt Enable */
#define USART_TX_COMPLETE_INTERRUPT_DISABLE					0
#define USART_TX_COMPLETE_INTERRUPT_ENABLE					1

/* @ref : UDRIE: USART Data Register Empty Interrupt Enable */
#define USART_TX_BUFFER_REGISTER_EMPTY_INTERRUPT_DISABLE	0
#define USART_TX_BUFFER_REGISTER_EMPTY_INTERRUPT_ENABLE		1

/* --------------------------------- */
/* Enable/Disable USART Transmitter/Receiver */

/* @ref : RXEN: Receiver Enable */
#define USART_RECEIVER_DISABLE								0
#define USART_RECEIVER_ENABLE								1

/* @ref : TXEN: Transmitter Enable */
#define USART_TRANSMITTER_DISABLE							0
#define USART_TRANSMITTER_ENABLE							1

/* --------------------------------- */
/* @ref : BAUD RATE Select */

#define BAUD_RATE_2400_BPS									(uint32)2400
#define BAUD_RATE_4800_BPS									(uint32)4800
#define BAUD_RATE_9600_BPS									(uint32)9600
#define BAUD_RATE_14400_BPS									(uint32)14400
#define BAUD_RATE_28800_BPS									(uint32)28800
#define BAUD_RATE_38400_BPS									(uint32)38400
#define BAUD_RATE_57600_BPS									(uint32)57600
#define BAUD_RATE_76800_BPS									(uint32)76800
#define BAUD_RATE_115200_BPS								(uint32)115200
#define BAUD_RATE_250000_BPS								(uint32)250000

/* --------------------------------- */


/* ----------------------------------------------------------------------------------- */
/* -----user_defined data type declaration section------ */

/* UART config structure */
typedef struct{
	/* pointer to function to hold the function called in the APPLICATION layer when USART Receive Complete if it's Interrupt is enabled */
	#if USART_CFG_RX_COMPLETE_INTERRUPT
	void (* USART_RX_Complete_DefaultHandler)(void);
	#endif

	/* pointer to function to hold the function called in the APPLICATION layer when USART Transmit Complete if it's Interrupt is enabled */
	#if USART_CFG_TX_BUFFER_EMPTY_INTERRUPT
	void (* USART_TX_BufferEmpty_DefaultHandler)(void);
	#endif
	#if USART_CFG_TX_COMPLETE_INTERRUPT
	void (* USART_TX_Complete_DefaultHandler)(void);
	#endif

	/* holds the value of the required BAUD RATE in USART Asynchronous Mode >> @ref : BAUD RATE Select */
	uint32 Baud_Rate;

	/* Enable/Disable USART Receiver >> @ref : RXEN: Receiver Enable */
	uint16 receiver_enable					:1;
	/* Enable/Disable USART Transmitter >> @ref : TXEN: Transmitter Enable */
	uint16 transmitter_enable				:1;

	/* Selects between Asynchronous and Synchronous mode of operation >> @ref : usart_mode_select_t */
	uint16 mode_select						:1;
	/* Selects USART Asynchronous Transmitter Speed >> @ref : USART Asynchronous Transmit Speed */
	uint16 asynchronous_tx_speed			:1;
	/* sets the number of data bits (Character Size) in a frame the receiver and transmitter use >> @ref : uart_character_size_t */
	uint16 char_size						:3;
	/* enable and set type of parity generation and check >> @ref : uart_parity_mode_select_t */
	uint16 parity_mode						:2;
	/* selects the number of Stop Bits to be inserted by the Transmitter. The Receiver ignores this setting >> @ref : uart_stop_mode_select_t */
	uint16 stop_mode						:1;

	/* Enable/Disable RX Complete Interrupt >> @ref : RXCIE: RX Complete Interrupt Enable */
	uint16 rx_complete_interrupt_en			:1;
	/* Enable/Disable TX Complete Interrupt >> @ref : TXCIE: TX Complete Interrupt Enable */
	uint16 tx_complete_interrupt_en			:1;
	/* Enable/Disable USART Data Register Empty Interrupt >> @ref : UDRIE: USART Data Register Empty Interrupt Enable */
	uint16 tx_buffer_reg_empty_interrupt_en	:1;

	/* Reserved */
	uint16									:3;
}uart_config_t;


/* @ref : usart_mode_select_t */
typedef enum{
	USART_ASYNCHRONOUS_MODE = 0,			/* USART Asynchronous Mode */
	USART_SYNCHRONOUS_MODE					/* USART Synchronous Mode */
}usart_mode_select_t;

/* @ref : USART Asynchronous Transmit Speed */
typedef enum{
	UART_NORMAL_SPEED_MODE = 0,				/* the divisor of the baud rate divider remains 16 */
	UART_DOUBLE_SPEED_MODE					/* reduce the divisor of the baud rate divider from 16 to 8  */
}uart_tx_speed_t;

/* @ref : uart_character_size_t */
typedef enum{
	UART_CHARACTER_SIZE_5_BITS = 0,			/* Data Bits = 5 bits */
	UART_CHARACTER_SIZE_6_BITS,				/* Data Bits = 6 bits */
	UART_CHARACTER_SIZE_7_BITS,				/* Data Bits = 7 bits */
	UART_CHARACTER_SIZE_8_BITS,				/* Data Bits = 8 bits */
	UART_CHARACTER_SIZE_9_BITS = 7			/* Data Bits = 9 bits */
}uart_character_size_t;

/* @ref : uart_parity_mode_select_t */
typedef enum{
	UART_PARITY_MODE_DISABLE = 0,			/* Parity Mode : Disable */
	UART_PARITY_MODE_EVEN = 2,				/* Parity Mode : Even Parity */
	UART_PARITY_MODE_ODD					/* Parity Mode : Odd Parity */
}uart_parity_mode_select_t;

/* @ref : uart_stop_mode_select_t */
typedef enum{
	UART_STOP_MODE_1_BIT = 0,				/* Stop bit select : 1 Stop Bit */
	UART_STOP_MODE_2_BITS					/* Stop bit select : 2 Stop Bits */
}uart_stop_mode_select_t;


/* ----------------------------------------------------------------------------------- */
/* ------------functions declaration section------------ */


/**
 * @brief  initialize USART :
 * 			1-  Select USART Mode : Asynchronous Mode
 * 			2-  Enable/Disable UART Receiver
 * 			3-  Enable/Disable UART Transmitter
 * 			4-  Selects USART Asynchronous Transmitter Speed
 * 					- UART Normal Speed
 * 					- UART Double Speed
 * 			5-  Sets the number of data bits (Character Size) in a frame the receiver and transmitter use
 * 					- Data Bits = 5 bits
 * 					- Data Bits = 6 bits
 * 					- Data Bits = 7 bits
 * 					- Data Bits = 8 bits
 * 					- Data Bits = 9 bits
 * 			6-  Enable/Disable and Set the Parity Mode if Enabled
 * 					- Parity Disabled
 * 					- Even Parity
 * 					- Odd Parity
 * 			7-  Selects the number of Stop Bits to be inserted by the Transmitter
 * 					- 1 Stop Bit
 * 					- 2 Stop Bits
 * 			8-  Selects the BAUD RATE value and initialize the UBRR value
 * 			9-  Set UART RX Complete Call Back if RX Complete Interrupt is enabled
 * 			10- Set UART TX Complete Call Back if TX Complete Interrupt is enabled
 * 			11- Set UART Data Register Empty Call Back if Data Register Empty Interrupt is enabled
 * 			12- Clear UART Frame Error, Data OverRun and Parity Error
 * @param  (uart_obj) pointer to the UART object passed by reference
 * @return (l_status) status of the performed operation
 *              (E_NOT_OK)  operation failed
 *              (E_OK)      operation success
 */
Std_ReturnType UART_init(uart_config_t *uart_obj);


/**
 * @brief  Send byte to another device through UART
 * @param  (data) the data you want to send
 */
void UART_sendByte(const uint16 data);


/**
 * @brief  Receive byte from another device through UART
 * @return the data received
 */
uint16 UART_recieveByte(void);


/**
 * @brief  Selects the BAUD RATE value and initialize the UBRR value
 * @param  (baud_rate) the value of the BAUD RATE
 */
void UART_setBaudRate(uint32 baud_rate);


/**
 * @brief  Send string to another device through UART
 * @param  (p_str)   pointer to the first character of the string you want to send
 *@return (l_status) status of the performed operation
 *              (E_NOT_OK)  operation failed
 *              (E_OK)      operation success
 */
Std_ReturnType UART_sendString(const uint8 * const p_str);


/**
 * @brief  Receive string from another device through UART
 * @param  (p_str)   pointer to the first character of the string you want to receive
 *@return (l_status) status of the performed operation
 *              (E_NOT_OK)  operation failed
 *              (E_OK)      operation success
 */
Std_ReturnType UART_receiveString(uint8 * const p_str);


/* ----------------------------------------------------------------------------------- */
#endif /* _USART_H_ */
