/*
 =========================================================================================
 Name        : usart.c
 Author      : Mohamed Ashraf El-Sayed
 Version     : 1.0.0
 Copyright   : Your copyright notice
 date        : Mon, Oct 16 2023
 time        :
 Description : USART ASYNCHRONOUS Mode Driver Source file , Ansi-style
 =========================================================================================
*/

/* ----------------------------------------------------------------------------------- */
/* ------------------Includes section------------------- */
#include <avr/interrupt.h> 			/* For UART TX/RX ISR */

#include "usart.h"


/* ----------------------------------------------------------------------------------- */
/* -------------------Global section-------------------- */

/* create a pointer to function to hold the address of the call back function */

/* UART RX Call Back */

#if USART_CFG_RX_COMPLETE_INTERRUPT
static void (* USART_RX_Complete_InterruptHandler)(void) = NULL_PTR;
#endif

/* UART TX Call Back */

#if USART_CFG_TX_BUFFER_EMPTY_INTERRUPT
static void (* USART_TX_BufferEmpty_InterruptHandler)(void) = NULL_PTR;
#endif

#if USART_CFG_TX_COMPLETE_INTERRUPT
static void (* USART_TX_Complete_InterruptHandler)(void) = NULL_PTR;
#endif


/* ----------------------------------------------------------------------------------- */
/* ------------functions definition section------------- */


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
Std_ReturnType UART_init(uart_config_t *uart_obj)
{
	/* create a local variable to hold the status of the performed operation */
	Std_ReturnType l_status = ZERO_INIT;

	/* check if the address is valid or not */
	if(uart_obj == NULL_PTR)
	{
		/* NULL pointer is passed */

		l_status = E_NOK;		/* operation failed */
	}
	else
	{
		/* the address passed is valid */

		l_status = E_OK;		/* operation success */

		/* USART Asynchronous Mode Init */

		/* --------------------------------- */
		/* Enable/Disable USART Transmitter/Receiver */
		_UCSRB._RXEN = uart_obj->receiver_enable;

		/* Enable/Disable USART Transmitter */
		_UCSRB._TXEN = uart_obj->transmitter_enable;
		/* --------------------------------- */

		/* --------------------------------- */
		/* Clear USART Transmit Complete Flag bit by writing a one to its bit location */
		_UCSRA._TXC = SET;

		/* Clear Frame Error Flag bit, Always set this bit to zero when writing to UCSRA */
		_UCSRA._FE = UART_FRAME_ERROR_NOT_DETECTED;

		/* Clear Data OverRun Flag bit, Always set this bit to zero when writing to UCSRA */
		_UCSRA._DOR = UART_DATA_OVERRUN_NOT_DETECTED;

		/* Clear Parity Error Flag bit, Always set this bit to zero when writing to UCSRA */
		_UCSRA._PE = UART_PARITY_ERROR_NOT_DETECTED;

		/* Disable Multi-processor Communication Mode */
		_UCSRA._MPCM = RESET;
		/* --------------------------------- */

		/* --------------------------------- */
		/* The URSEL must be one when writing the UCSRC */
		_UCSRC._URSEL = SET;

		/* Select USART Mode : Asynchronous Mode */
		_UCSRC._UMSEL = USART_ASYNCHRONOUS_MODE;

		/* Selects USART Asynchronous Transmitter Speed */
		_UCSRA._U2X = uart_obj->asynchronous_tx_speed;

		/* Enable/Disable Parity Mode */
		_UCSRC._UPMx = uart_obj->parity_mode;

		/* selects the number of Stop Bits to be inserted by the Transmitter */
		_UCSRC._USBS = uart_obj->stop_mode;

		/* sets the number of data bits (Character Size) in a frame the receiver and transmitter use */
		switch(uart_obj->char_size)
		{
			case UART_CHARACTER_SIZE_5_BITS	:	/* Data Bits = 5 bits */
												_UCSRC._UCSZ0 = RESET; _UCSRC._UCSZ1 = RESET; _UCSRB._UCSZ2 = RESET;
												break;

			case UART_CHARACTER_SIZE_6_BITS	:	/* Data Bits = 6 bits */
												_UCSRC._UCSZ0 = SET;   _UCSRC._UCSZ1 = RESET; _UCSRB._UCSZ2 = RESET;
												break;

			case UART_CHARACTER_SIZE_7_BITS	:	/* Data Bits = 7 bits */
												_UCSRC._UCSZ0 = RESET; _UCSRC._UCSZ1 = SET;   _UCSRB._UCSZ2 = RESET;
												break;

			case UART_CHARACTER_SIZE_8_BITS	:	/* Data Bits = 8 bits */
												_UCSRC._UCSZ0 = SET;   _UCSRC._UCSZ1 = SET;   _UCSRB._UCSZ2 = RESET;
												break;

			case UART_CHARACTER_SIZE_9_BITS	:	/* Data Bits = 9 bits */
												_UCSRC._UCSZ0 = SET;   _UCSRC._UCSZ1 = SET;   _UCSRB._UCSZ2 = SET;
												break;

			default	:	/* Nothing */
						break;
		}
		/* --------------------------------- */

		/* --------------------------------- */
		/* Selects the BAUD RATE value and initialize the UBRR value */
		UART_setBaudRate(uart_obj->Baud_Rate);
		/* --------------------------------- */

		/* --------------------------------- */
		/* Enable/Disable RX Complete Interrupt */
		_UCSRB._RXCIE = uart_obj->rx_complete_interrupt_en;

		/* Set RX Complete Call Back if RX Complete Interrupt is enabled */
		#if USART_CFG_RX_COMPLETE_INTERRUPT
		if(uart_obj->rx_complete_interrupt_en == USART_RX_COMPLETE_INTERRUPT_DISABLE)
		{
			/* RX Complete Interrupt is disabled */
			USART_RX_Complete_InterruptHandler = NULL_PTR;
		}
		else if(uart_obj->rx_complete_interrupt_en == USART_RX_COMPLETE_INTERRUPT_ENABLE)
		{
			/* RX Complete Interrupt is enabled */
			USART_RX_Complete_InterruptHandler = uart_obj->USART_RX_Complete_DefaultHandler;
		}
		else{ /* Nothing */ }
		#endif


		/* Enable/Disable TX Complete Interrupt */
		_UCSRB._TXCIE = uart_obj->tx_complete_interrupt_en;

		#if USART_CFG_TX_COMPLETE_INTERRUPT
		/* Set TX Complete Call Back if TX Complete Interrupt is enabled */
		if(uart_obj->tx_complete_interrupt_en == USART_TX_COMPLETE_INTERRUPT_DISABLE)
		{
			/* TX Complete Interrupt is disabled */
			USART_TX_Complete_InterruptHandler = NULL_PTR;
		}
		else if(uart_obj->tx_complete_interrupt_en == USART_TX_COMPLETE_INTERRUPT_ENABLE)
		{
			/* TX Complete Interrupt is enabled */
			USART_TX_Complete_InterruptHandler = uart_obj->USART_TX_Complete_DefaultHandler;
		}
		else{ /* Nothing */ }
		#endif


		/* Enable/Disable USART Data Register Empty Interrupt */
		_UCSRB._UDRIE = uart_obj->tx_buffer_reg_empty_interrupt_en;

		#if USART_CFG_TX_BUFFER_EMPTY_INTERRUPT
		/* Set USART Data Register Empty Call Back if USART Data Register Empty Interrupt is enabled */
		if(uart_obj->tx_buffer_reg_empty_interrupt_en == USART_TX_BUFFER_REGISTER_EMPTY_INTERRUPT_DISABLE)
		{
			/* USART Data Register Empty Interrupt is disabled */
			USART_TX_BufferEmpty_InterruptHandler = NULL_PTR;
		}
		else if(uart_obj->tx_buffer_reg_empty_interrupt_en == USART_TX_BUFFER_REGISTER_EMPTY_INTERRUPT_ENABLE)
		{
			/* TX Complete Interrupt is enabled or USART Data Register Empty Interrupt is enabled */
			USART_TX_BufferEmpty_InterruptHandler = uart_obj->USART_TX_BufferEmpty_DefaultHandler;
		}
		else{ /* Nothing */ }
		#endif
		/* --------------------------------- */
	}

	return l_status;
}


/**
 * @brief  Send byte to another device through UART
 * @param  (data) the data you want to send
 */
void UART_sendByte(const uint16 data)
{
	/* If UDRE is one, the buffer is empty, and therefore ready to be written */
	/* wait till UART Transmit Data Buffer is empty */
	while( !(_UCSRA._UDRE) );

	/* UART Transmit Data Buffer is empty and ready to receive new data */
	/* TXB8 is the ninth data bit in the character to be transmitted when operating with serial frames with nine data bits
	 	 Must be written before writing the low bits to UDR */
	if(_UCSRB._UCSZ2 == SET)
	{
		/* Data Bits = 9 bits */
		_UCSRB._TXB8 = (data >> 8) & 0x01;
	}

	/* write on the UART TX Data Register */
	_UDR.Byte = (uint8)data;
}


/**
 * @brief  Receive byte from another device through UART
 * @return the data received
 */
uint16 UART_recieveByte(void)
{
	/* If RXC is one, the buffer is not empty which means there is a new unread data ready to be read */
	/* wait till UART Receive Data Buffer to be full(not empty) */
	while( !(_UCSRA._RXC) );

	/*  UART Receive Data Buffer is not empty and there is a new data ready to be read from it */
	/* RXB8 is the ninth data bit of the received character when operating with serial frames with nine data bits
	   	 Must be read before reading the low bits from UDR */
	if(_UCSRB._UCSZ2 == SET)
	{
		/* Data Bits = 9 bits */
		return ( (uint16)( ( ( (_UCSRB._RXB8) << 8 ) | (uint16)(_UDR.Byte) ) & 0x1FF ) );
	}
	else
	{
		/* Data Bits is less than 9 bits */
		return (_UDR.Byte);
	}
}


/**
 * @brief  Selects the BAUD RATE value and initialize the UBRR value
 * @param  (baud_rate) the value of the BAUD RATE
 */
void UART_setBaudRate(uint32 baud_rate)
{
	/* create a local variable to hold the value of UBRR */
	uint16 ubrr_val = ZERO_INIT;

	/* check which transmit speed is configured */
	switch(_UCSRA._U2X)
	{
		case UART_NORMAL_SPEED_MODE	:	/* Asynchronous Normal Mode : the divisor of the baud rate divider remains 16 */
										ubrr_val = (uint16)( ( (CPU_FREQUENCY) / (16 * (baud_rate)) ) - 1 );
										break;

		case UART_DOUBLE_SPEED_MODE	:	/* Asynchronous Double Speed Mode : reduce the divisor of the baud rate divider from 16 to 8  */
										ubrr_val = (uint16)( ( (CPU_FREQUENCY) / (8 * (baud_rate)) ) - 1 );
										break;

		default		:	/* Nothing */
						break;
	}

	/* The URSEL must be zero when writing the UBRRH */
	//_UBRRH._URSEL = RESET;
	/* Set the UBRR value for the required BAUD RATE */
	_UBRRL.Byte = (uint8)(ubrr_val);
	_UBRRH.Byte= (uint8)( (ubrr_val) >> 8 );
}


/**
 * @brief  Send string to another device through UART
 * @param  (p_str)   pointer to the first character of the string you want to send
 *@return (l_status) status of the performed operation
 *              (E_NOT_OK)  operation failed
 *              (E_OK)      operation success
 */
Std_ReturnType UART_sendString(const uint8 * const p_str)
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
			UART_sendByte( *(p_str + char_index) );
			/* increment the character index to point to the next character */
			char_index++;
		}
	}

	return l_status;
}


/**
 * @brief  Receive string from another device through UART
 * @param  (p_str)   pointer to the first character of the string you want to receive
 *@return (l_status) status of the performed operation
 *              (E_NOT_OK)  operation failed
 *              (E_OK)      operation success
 */
Std_ReturnType UART_receiveString(uint8 * const p_str)
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
		*(p_str + char_index) = (uint8)( UART_recieveByte() );

		/* Receive the whole string until the '#' */

		while( *(p_str + char_index) != '#' )
		{
			/* increment the character index to point to the next character */
			char_index++;

			/* receive the character */
			*(p_str + char_index) = (uint8)( UART_recieveByte() );
		}

		/* Add NULL Character in the end of the string */
		*(p_str + char_index) = '\0';
	}

	return l_status;
}


/* ----------------------------------------------------------------------------------- */
/* --------------------ISR section---------------------- */


/**
 * @brief  USART, Rx Complete ISR
 */
#if USART_CFG_RX_COMPLETE_INTERRUPT
ISR(USART_RXC_vect)
{
	/* check if the call back notification contains NULL or not */
	if(USART_RX_Complete_InterruptHandler)
	{
		/* Call Back */
		(*USART_RX_Complete_InterruptHandler)();
	}
	else{ /* Nothing */ }
}
#endif


#if USART_CFG_TX_COMPLETE_INTERRUPT
/**
 * @brief  USART, Tx Complete ISR
 */
ISR(USART_TXC_vect)
{
	/* check if the call back notification contains NULL or not */
	if(USART_TX_Complete_InterruptHandler)
	{
		/* Call Back */
		(*USART_TX_Complete_InterruptHandler)();
	}
	else{ /* Nothing */ }
}
#endif


#if USART_CFG_TX_BUFFER_EMPTY_INTERRUPT
/**
 * @brief  USART Data Buffer Register Empty ISR
 */
ISR(USART_UDRE_vect)
{
	/* check if the call back notification contains NULL or not */
	if(USART_TX_BufferEmpty_InterruptHandler)
	{
		/* Call Back */
		(*USART_TX_BufferEmpty_InterruptHandler)();
	}
	else{ /* Nothing */ }
}
#endif


/* ----------------------------------------------------------------------------------- */
