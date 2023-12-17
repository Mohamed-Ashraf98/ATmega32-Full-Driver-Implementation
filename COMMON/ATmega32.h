/*
 =========================================================================================
 Name        : ATmega32.h
 Author      : Mohamed Ashraf El-Sayed
 Version     : 1.0.0
 Copyright   : Your copyright notice
 date        : Sun, Sep 24 2023
 time        :
 Description : ATmega32 MCU Device Header file , Ansi-style
 =========================================================================================
*/

#ifndef _ATMEGA32_H_
#define _ATMEGA32_H_
/* ----------------------------------------------------------------------------------- */
/* ------------------Includes section------------------- */
#include "std_types.h"


/* ----------------------------------------------------------------------------------- */
/* --MCU Frequency & Clock Period Calculation section--- */


/* Calculations to get the period of one Clock Pulse */

/* MCU Frequency (F_CPU) */
#define CPU_FREQUENCY			8000000.0
/* Period of one Clock Pulse in us from Frequency of timer and the pre-scaler */
#define PERIOD_US(prescaler)	( (1000000.0) / ( (CPU_FREQUENCY) / (prescaler) ) )


/* ----------------------------------------------------------------------------------- */
/* -----------------Register Addresses------------------ */

/*
 * ---------------------------------
 * SREG : Status Register
 * ---------------------------------
 */

#define SREG_ADDRESS			0x5F	/* Status Register */

/*
 * ---------------------------------
 * SFIOR : Special Function IO Register
 * ---------------------------------
 */

#define SFIOR_ADDRESS			0x50	/* Special Function IO Register */

/*
 * ---------------------------------
 * TIMERx Control Registers
 * ---------------------------------
 */

#define TIMSK_ADDRESS			0x59	/* Timer/Counter Interrupt Mask Register */
#define TIFR_ADDRESS			0x58	/* Timer/Counter Interrupt Flag Register */

/*
 * ---------------------------------
 * TIMER0 Registers
 * ---------------------------------
 */

#define OCR0_ADDRESS			0x5C	/* Output Compare Register */
#define TCCR0_ADDRESS			0x53	/* Timer/Counter Control Register */
#define TCNT0_ADDRESS			0x52	/* Timer/Counter Register */

/*
 * ---------------------------------
 * TIMER1 Registers
 * ---------------------------------
 */

#define TCCR1A_ADDRESS			0x4F	/* Timer/Counter1 Control Register A */
#define TCCR1B_ADDRESS			0x4E	/* Timer/Counter1 Control Register B */
#define TCNT1H_ADDRESS			0x4D	/* Timer/Counter Register */
#define TCNT1L_ADDRESS			0x4C	/* Timer/Counter Register */
#define OCR1AH_ADDRESS			0x4B	/* Output Compare Register 1 A */
#define OCR1AL_ADDRESS			0x4A	/* Output Compare Register 1 A */
#define OCR1BH_ADDRESS			0x49	/* Output Compare Register 1 B */
#define OCR1BL_ADDRESS			0x48	/* Output Compare Register 1 B */
#define ICR1H_ADDRESS			0x47	/* Input Capture Register 1 */
#define ICR1L_ADDRESS			0x46	/* Input Capture Register 1 */

/*
 * ---------------------------------
 * TIMER2 Registers
 * ---------------------------------
 */

#define TCCR2_ADDRESS			0x45
#define TCNT2_ADDRESS			0x44
#define OCR2_ADDRESS			0x43

/*
 * ---------------------------------
 * WATCHDOG Register
 * ---------------------------------
 */

#define WDTCR_ADDRESS			0x41

/*
 * ---------------------------------
 * GPIO Port A Registers
 * ---------------------------------
 */

#define PORTA_ADDRESS			0x3B	/* Port A Data Register */
#define DDRA_ADDRESS			0x3A	/* Port A Data Direction Register */
#define PINA_ADDRESS			0x39	/* Port A Input Pins Address */

/*
 * ---------------------------------
 * GPIO Port B Registers
 * ---------------------------------
 */

#define PORTB_ADDRESS			0x38	/* Port B Data Register */
#define DDRB_ADDRESS			0x37	/* Port B Data Direction Register */
#define PINB_ADDRESS			0x36	/* Port B Input Pins Address */

/*
 * ---------------------------------
 * GPIO Port C Registers
 * ---------------------------------
 */

#define PORTC_ADDRESS			0x35	/* Port C Data Register */
#define DDRC_ADDRESS			0x34	/* Port C Data Direction Register */
#define PINC_ADDRESS			0x33	/* Port C Input Pins Address */

/*
 * ---------------------------------
 * GPIO Port D Registers
 * ---------------------------------
 */

#define PORTD_ADDRESS			0x32	/* Port D Data Register */
#define DDRD_ADDRESS			0x31	/* Port D Data Direction Register */
#define PIND_ADDRESS			0x30	/* Port D Input Pins Address */

/*
 * ---------------------------------
 * SPI Registers
 * ---------------------------------
 */

#define SPDR_ADDRESS			0x2F	/* SPI Data Register */
#define SPSR_ADDRESS			0x2E	/* SPI Status Register */
#define SPCR_ADDRESS			0x2D	/* SPI Control Register */

/*
 * ---------------------------------
 * USART Registers
 * ---------------------------------
 */

#define UDR_ADDRESS				0x2C	/* USART I/O Data Register */
#define UCSRA_ADDRESS			0x2B	/* USART Control and Status Register A */
#define UCSRB_ADDRESS			0x2A	/* USART Control and Status Register B */
#define UCSRC_ADDRESS			0x40	/* USART Control and Status Register C */
#define UBRRL_ADDRESS			0x29	/* USART Baud Rate Register Low Byte */
#define UBRRH_ADDRESS			0x40	/* USART Baud Rate Register High Byte */

/*
 * ---------------------------------
 * ADC Registers
 * ---------------------------------
 */

#define ADMUX_ADDRESS			0x27	/* ADC Multiplexer Selection Register */
#define ADCSRA_ADDRESS			0x26	/* ADC Control and Status Register A */
#define ADCH_ADDRESS			0x25	/* The ADC Data Register */
#define ADCL_ADDRESS			0x24	/* The ADC Data Register */

/*
 * ---------------------------------
 * I2C Registers
 * ---------------------------------
 */

#define TWCR_ADDRESS			0x56	/* TWI Control Register */
#define TWDR_ADDRESS			0x23	/* TWI Data Register */
#define TWAR_ADDRESS			0x22	/* TWI (Slave) Address Register */
#define TWSR_ADDRESS			0x21	/* TWI Status Register */
#define TWBR_ADDRESS			0x20	/* TWI Bit Rate Register */


/* ----------------------------------------------------------------------------------- */
/* --------Registers type structure declarations-------- */

/*
 * ---------------------------------
 * SREG : Status Register
 * ---------------------------------
 */

/* Status Register */
typedef union
{
	uint8 Byte;

	struct
	{
		uint8 _C:1;				/* Bit 0 – C: Carry Flag */
		uint8 _Z:1;				/* Bit 1 – Z: Zero Flag */
		uint8 _N:1;				/* Bit 2 – N: Negative Flag */
		uint8 _V:1;				/* Bit 3 – V: Two’s Complement Overflow Flag */
		uint8 _S:1;				/* Bit 4 – S: Sign Bit */
		uint8 _H:1;				/* Bit 5 – H: Half Carry Flag */
		uint8 _T:1;				/* Bit 6 – T: Bit Copy Storage */
		uint8 _I:1;				/* Bit 7 – I: Global Interrupt Enable */
	};
}SREG_CFG_t;

/*
 * ---------------------------------
 * SFIOR : Special Function IO Register
 * ---------------------------------
 */

/* Special Function IO Register */
typedef union
{
	uint8 Byte;

	struct
	{
		uint8 _PSR10:1;			/* Bit 0 – PSR10: Prescaler Reset Timer/Counter1 and Timer/Counter0 */
		uint8 _PSR2 :1;			/* Bit 1 – PSR2: Prescaler Reset Timer/Counter2 */
		uint8 _PUD  :1;			/* Bit 2 – PUD: Pull-up disable */
		uint8 _ACME :1;			/* Bit 3 – ACME: Analog Comparator Multiplexer Enable */
		uint8	   	:1;			/* Reserved */
		uint8 _ADTS0:1;			/* Bit 5 - ADST0: ADC Auto Trigger Source */
		uint8 _ADTS1:1;			/* Bit 6 - ADST1: ADC Auto Trigger Source */
		uint8 _ADTS2:1;			/* Bit 7 - ADST2: ADC Auto Trigger Source */
	};

	struct
	{
		uint8 _PSR10:1;			/* Bit 0 – PSR10: Prescaler Reset Timer/Counter1 and Timer/Counter0 */
		uint8 _PSR2 :1;			/* Bit 1 – PSR2: Prescaler Reset Timer/Counter2 */
		uint8 _PUD  :1;			/* Bit 2 – PUD: Pull-up disable */
		uint8 _ACME :1;			/* Bit 3 – ACME: Analog Comparator Multiplexer Enable */
		uint8	   	:1;			/* Reserved */
		uint8 _ADTSx:3;			/* Bit 7:5 – ADTS2:0: ADC Auto Trigger Source */
	};
}SFIOR_CFG_t;

/*
 * ---------------------------------
 * TIMERx Control Registers
 * ---------------------------------
 */

/* Timer/Counter Interrupt Mask Register */
typedef union
{
	uint8 Byte;

	struct
	{
		uint8 _TOIE0 :1;		/* Bit 0 – TOIE0: Timer/Counter0 Overflow Interrupt Enable */
		uint8 _OCIE0 :1;		/* Bit 1 – OCIE0: Timer/Counter0 Output Compare Match Interrupt Enable */
		uint8 _TOIE1 :1;		/* Bit 2 – TOIE1: Timer/Counter1, Overflow Interrupt Enable */
		uint8 _OCIE1B:1;		/* Bit 3 – OCIE1B: Timer/Counter1, Output Compare B Match Interrupt Enable */
		uint8 _OCIE1A:1;		/* Bit 4 – OCIE1A: Timer/Counter1, Output Compare A Match Interrupt Enable */
		uint8 _TICIE1:1;		/* Bit 5 – TICIE1: Timer/Counter1, Input Capture Interrupt Enable */
		uint8 _TOIE2 :1;		/* Bit 6 – TOIE2: Timer/Counter2 Overflow Interrupt Enable */
		uint8 _OCIE2 :1;		/* Bit 7 – OCIE2: Timer/Counter2 Output Compare Match Interrupt Enable */
	};
}TIMSK_CFG_t;

/* Timer/Counter Interrupt Flag Register */
typedef union
{
	uint8 Byte;

	struct
	{
		uint8 _TOV0 :1;			/* Bit 0 – TOV0: Timer/Counter0 Overflow Flag */
		uint8 _OCF0 :1;			/* Bit 1 – OCF0: Output Compare Flag 0 */
		uint8 _TOV1 :1;			/* Bit 2 – TOV1: Timer/Counter1, Overflow Flag */
		uint8 _OCF1B:1;			/* Bit 3 – OCF1B: Timer/Counter1, Output Compare B Match Flag */
		uint8 _OCF1A:1;			/* Bit 4 – OCF1A: Timer/Counter1, Output Compare A Match Flag */
		uint8 _ICF1 :1;			/* Bit 5 – ICF1: Timer/Counter1, Input Capture Flag */
		uint8 _TOV2 :1;			/* Bit 6 – TOV2: Timer/Counter2 Overflow Flag */
		uint8 _OCF2 :1;			/* Bit 7 – OCF2: Output Compare Flag 2 */
	};
}TIFR_CFG_t;

/*
 * ---------------------------------
 * TIMER0 Registers
 * ---------------------------------
 */

/* Output Compare Register */
typedef union
{
	uint8 Byte;

	struct
	{
		uint8 OCR0_bit0:1;
		uint8 OCR0_bit1:1;
		uint8 OCR0_bit2:1;
		uint8 OCR0_bit3:1;
		uint8 OCR0_bit4:1;
		uint8 OCR0_bit5:1;
		uint8 OCR0_bit6:1;
		uint8 OCR0_bit7:1;
	};
}OCR0_CFG_t;

/* Timer/Counter Control Register */
typedef union
{
	uint8 Byte;

	struct
	{
		uint8 _CS00 :1;			/* Bit 0 – CS00: Clock Select */
		uint8 _CS01 :1;			/* Bit 1 – CS01: Clock Select */
		uint8 _CS02 :1;			/* Bit 2 – CS02: Clock Select */
		uint8 _WGM01:1;			/* Bit 3 – WGM01: Waveform Generation Mode */
		uint8 _COM00:1;			/* Bit 4 – COM00: Compare Match Output Mode */
		uint8 _COM01:1;			/* Bit 5 – COM01: Compare Match Output Mode */
		uint8 __WGM00:1;		/* Bit 6 – WGM00: Waveform Generation Mode */
		uint8 _FOC0 :1;			/* Bit 7 – FOC0: Force Output Compare */
	};

	struct
	{
		uint8 _CS0x :3;			/* Bit 2:0 – CS02:0: Clock Select */
		uint8 _WGM01:1;			/* Bit 3 – WGM01: Waveform Generation Mode */
		uint8 _COM0x:2;			/* Bit 5:4 – COM01:0: Compare Match Output Mode */
		uint8 _WGM00:1;			/* Bit 6 – WGM00: Waveform Generation Mode */
		uint8 _FOC0 :1;			/* Bit 7 – FOC0: Force Output Compare */
		};
}TCCR0_CFG_t;

/* Timer/Counter Register */
typedef union
{
	uint8 Byte;

	struct
	{
		uint8 TCNT0_bit0:1;
		uint8 TCNT0_bit1:1;
		uint8 TCNT0_bit2:1;
		uint8 TCNT0_bit3:1;
		uint8 TCNT0_bit4:1;
		uint8 TCNT0_bit5:1;
		uint8 TCNT0_bit6:1;
		uint8 TCNT0_bit7:1;
	};
}TCNT0_CFG_t;

/*
 * ---------------------------------
 * TIMER1 Registers
 * ---------------------------------
 */

/* Timer/Counter1 Control Register A */
typedef union
{
	uint8 Byte;

	struct
	{
		uint8 _WGM10 :1;		/* Bit 0 – WGM10: Waveform Generation Mode */
		uint8 _WGM11 :1;		/* Bit 1 – WGM11: Waveform Generation Mode */
		uint8 _FOC1B :1;		/* Bit 2 – FOC1B: Force Output Compare for Compare unit B */
		uint8 _FOC1A :1;		/* Bit 3 – FOC1A: Force Output Compare for Compare unit A */
		uint8 _COM1B0:1;		/* Bit 4 – COM1B0: Compare Output Mode for Compare unit B */
		uint8 _COM1B1:1;		/* Bit 5 – COM1B1: Compare Output Mode for Compare unit B */
		uint8 _COM1A0:1;		/* Bit 6 – COM1A0: Compare Output Mode for Compare unit A */
		uint8 _COM1A1:1;		/* Bit 7 – COM1A1: Compare Output Mode for Compare unit A */
	};

	struct
	{
		uint8 _WGM1x :2;		/* Bit 1:0 – WGM11:0: Waveform Generation Mode */
		uint8 _FOC1B :1;		/* Bit 2 – FOC1B: Force Output Compare for Compare unit B */
		uint8 _FOC1A :1;		/* Bit 3 – FOC1A: Force Output Compare for Compare unit A */
		uint8 _COM1Bx:2;		/* Bit 5:4 – COM1B1:0: Compare Output Mode for Compare unit B */
		uint8 _COM1Ax:2;		/* Bit 7:6 – COM1A1:0: Compare Output Mode for Compare unit A */
	};
}TCCR1A_CFG_t;

/* Timer/Counter1 Control Register B */
typedef union
{
	uint8 Byte;

	struct
	{
		uint8 _CS10 :1;			/* Bit 0 – CS10: Clock Select */
		uint8 _CS11 :1;			/* Bit 1 – CS11: Clock Select */
		uint8 _CS12 :1;			/* Bit 2 – CS12: Clock Select */
		uint8 _WGM12:1;			/* Bit 3 – WGM12: Waveform Generation Mode */
		uint8 _WGM13:1;			/* Bit 4 – WGM13: Waveform Generation Mode */
		uint8 	   	:1;			/* Reserved */
		uint8 _ICES1:1;			/* Bit 6 – ICES1: Input Capture Edge Select */
		uint8 _ICNC1:1;			/* Bit 7 – ICNC1: Input Capture Noise Canceler */
	};

	struct
	{
		uint8 _CS1x :3;			/* Bit 2:0 – CS12:0: Clock Select */
		uint8 _WGM1x:2;			/* Bit 4:3 – WGM13:2: Waveform Generation Mode */
		uint8 	   	:1;			/* Reserved */
		uint8 _ICES1:1;			/* Bit 6 – ICES1: Input Capture Edge Select */
		uint8 _ICNC1:1;			/* Bit 7 – ICNC1: Input Capture Noise Canceler */
	};
}TCCR1B_CFG_t;

/* Timer/Counter Register */
typedef union
{
	uint16 TwoBytes;

	struct
	{
		uint16 _TCNT1L:8;
		uint16 _TCNT1H:8;
	};
}TCNT1_CFG_t;

/* Output Compare Register 1 A */
typedef union
{
	uint16 TwoBytes;

	struct
	{
		uint16 _OCR1AL:8;
		uint16 _OCR1AH:8;
	};
}OCR1A_CFG_t;

/* Output Compare Register 1 B */
typedef union
{
	uint16 TwoBytes;

	struct
	{
		uint16 _OCR1BL:8;
		uint16 _OCR1BH:8;
	};
}OCR1B_CFG_t;

/* Input Capture Register 1 */
typedef union
{
	uint16 TwoBytes;

	struct
	{
		uint16 _ICR1L:8;
		uint16 _ICR1H:8;
	};
}ICR1_CFG_t;

/*
 * ---------------------------------
 * TIMER2 Registers
 * ---------------------------------
 */

typedef union
{
	uint8 Byte;

	struct
	{
		uint8 _CS20 :1;
		uint8 _CS21 :1;
		uint8 _CS22 :1;
		uint8 _WGM21:1;
		uint8 _COM20:1;
		uint8 _COM21:1;
		uint8 _WGM20:1;
		uint8 _FOC2 :1;
	};
}TCCR2_CFG_t;

typedef union
{
	uint8 Byte;

	struct
	{
		uint8 TCNT2_bit0:1;
		uint8 TCNT2_bit1:1;
		uint8 TCNT2_bit2:1;
		uint8 TCNT2_bit3:1;
		uint8 TCNT2_bit4:1;
		uint8 TCNT2_bit5:1;
		uint8 TCNT2_bit6:1;
		uint8 TCNT2_bit7:1;
	};
}TCNT2_CFG_t;

typedef union
{
	uint8 Byte;

	struct
	{
		uint8 OCR2_bit0:1;
		uint8 OCR2_bit1:1;
		uint8 OCR2_bit2:1;
		uint8 OCR2_bit3:1;
		uint8 OCR2_bit4:1;
		uint8 OCR2_bit5:1;
		uint8 OCR2_bit6:1;
		uint8 OCR2_bit7:1;
	};
}OCR2_CFG_t;

/*
 * ---------------------------------
 * WATCHDOG Register
 * ---------------------------------
 */

typedef union
{
	uint8 Byte;

	struct
	{
		uint8 _WDP0 :1;
		uint8 _WDP1 :1;
		uint8 _WDP2 :1;
		uint8 _WDE  :1;
		uint8 _WDTOE:1;
		uint8      	:3;
	};
}WDTCR_CFG_t;

/*
 * ---------------------------------
 * GPIO Port A Registers
 * ---------------------------------
 */

/* Port A Data Register */
typedef union
{
	uint8 Byte;

	struct
	{
		uint8 _PORTA0:1;
		uint8 _PORTA1:1;
		uint8 _PORTA2:1;
		uint8 _PORTA3:1;
		uint8 _PORTA4:1;
		uint8 _PORTA5:1;
		uint8 _PORTA6:1;
		uint8 _PORTA7:1;
	};
}PORTA_CFG_t;

/* Port A Data Direction Register */
typedef union
{
	uint8 Byte;

	struct
	{
		uint8 _DDA0:1;
		uint8 _DDA1:1;
		uint8 _DDA2:1;
		uint8 _DDA3:1;
		uint8 _DDA4:1;
		uint8 _DDA5:1;
		uint8 _DDA6:1;
		uint8 _DDA7:1;
	};
}DDRA_CFG_t;

/* Port A Input Pins Address */
typedef union
{
	uint8 Byte;

	struct
	{
		uint8 _PINA0:1;
		uint8 _PINA1:1;
		uint8 _PINA2:1;
		uint8 _PINA3:1;
		uint8 _PINA4:1;
		uint8 _PINA5:1;
		uint8 _PINA6:1;
		uint8 _PINA7:1;
	};
}PINA_CFG_t;

/*
 * ---------------------------------
 * GPIO Port B Registers
 * ---------------------------------
 */

/* Port B Data Register */
typedef union
{
	uint8 Byte;

	struct
	{
		uint8 _PORTB0:1;
		uint8 _PORTB1:1;
		uint8 _PORTB2:1;
		uint8 _PORTB3:1;
		uint8 _PORTB4:1;
		uint8 _PORTB5:1;
		uint8 _PORTB6:1;
		uint8 _PORTB7:1;
	};
}PORTB_CFG_t;

/* Port B Data Direction Register */
typedef union
{
	uint8 Byte;

	struct
	{
		uint8 _DDB0:1;
		uint8 _DDB1:1;
		uint8 _DDB2:1;
		uint8 _DDB3:1;
		uint8 _DDB4:1;
		uint8 _DDB5:1;
		uint8 _DDB6:1;
		uint8 _DDB7:1;
	};
}DDRB_CFG_t;

/* Port B Input Pins Address */
typedef union
{
	uint8 Byte;

	struct
	{
		uint8 _PINB0:1;
		uint8 _PINB1:1;
		uint8 _PINB2:1;
		uint8 _PINB3:1;
		uint8 _PINB4:1;
		uint8 _PINB5:1;
		uint8 _PINB6:1;
		uint8 _PINB7:1;
	};
}PINB_CFG_t;

/*
 * ---------------------------------
 * GPIO Port C Registers
 * ---------------------------------
 */

/* Port C Data Register */
typedef union
{
	uint8 Byte;

	struct
	{
		uint8 _PORTC0:1;
		uint8 _PORTC1:1;
		uint8 _PORTC2:1;
		uint8 _PORTC3:1;
		uint8 _PORTC4:1;
		uint8 _PORTC5:1;
		uint8 _PORTC6:1;
		uint8 _PORTC7:1;
	};
}PORTC_CFG_t;

/* Port C Data Direction Register */
typedef union
{
	uint8 Byte;

	struct
	{
		uint8 _DDC0:1;
		uint8 _DDC1:1;
		uint8 _DDC2:1;
		uint8 _DDC3:1;
		uint8 _DDC4:1;
		uint8 _DDC5:1;
		uint8 _DDC6:1;
		uint8 _DDC7:1;
	};
}DDRC_CFG_t;

/* Port C Input Pins Address */
typedef union
{
	uint8 Byte;

	struct
	{
		uint8 _PINC0:1;
		uint8 _PINC1:1;
		uint8 _PINC2:1;
		uint8 _PINC3:1;
		uint8 _PINC4:1;
		uint8 _PINC5:1;
		uint8 _PINC6:1;
		uint8 _PINC7:1;
	};
}PINC_CFG_t;

/*
 * ---------------------------------
 * GPIO Port D Registers
 * ---------------------------------
 */

/* Port D Data Register */
typedef union
{
	uint8 Byte;

	struct
	{
		uint8 _PORTD0:1;
		uint8 _PORTD1:1;
		uint8 _PORTD2:1;
		uint8 _PORTD3:1;
		uint8 _PORTD4:1;
		uint8 _PORTD5:1;
		uint8 _PORTD6:1;
		uint8 _PORTD7:1;
	};
}PORTD_CFG_t;

/* Port D Data Direction Register */
typedef union
{
	uint8 Byte;

	struct
	{
		uint8 _DDD0:1;
		uint8 _DDD1:1;
		uint8 _DDD2:1;
		uint8 _DDD3:1;
		uint8 _DDD4:1;
		uint8 _DDD5:1;
		uint8 _DDD6:1;
		uint8 _DDD7:1;
	};
}DDRD_CFG_t;

/* Port D Input Pins Address */
typedef union
{
	uint8 Byte;

	struct
	{
		uint8 _PIND0:1;
		uint8 _PIND1:1;
		uint8 _PIND2:1;
		uint8 _PIND3:1;
		uint8 _PIND4:1;
		uint8 _PIND5:1;
		uint8 _PIND6:1;
		uint8 _PIND7:1;
	};
}PIND_CFG_t;

/*
 * ---------------------------------
 * SPI Registers
 * ---------------------------------
 */

/* SPI Data Register */
typedef union
{
	uint8 Byte;

	struct
	{
		uint8 SPDR_bit0:1;
		uint8 SPDR_bit1:1;
		uint8 SPDR_bit2:1;
		uint8 SPDR_bit3:1;
		uint8 SPDR_bit4:1;
		uint8 SPDR_bit5:1;
		uint8 SPDR_bit6:1;
		uint8 SPDR_bit7:1;
	};
}SPDR_CFG_t;

/* SPI Status Register */
typedef union
{
	uint8 Byte;

	struct
	{
		uint8 _SPI2X:1;			/* Bit 0 – SPI2X: Double SPI Speed Bit */
		uint8		:5;			/* Reserved */
		uint8 _WCOL :1;			/* Bit 6 – WCOL: Write COLlision Flag */
		uint8 _SPIF :1;			/* Bit 7 – SPIF: SPI Interrupt Flag */
	};
}SPSR_CFG_t;

/* SPI Control Register */
typedef union
{
	uint8 Byte;

	struct
	{
		uint8 _SPR0:1;			/* Bit 0 – SPR0: SPI Clock Rate Select 0 */
		uint8 _SPR1:1;			/* Bit 1 – SPR1: SPI Clock Rate Select 1 */
		uint8 _CPHA:1;			/* Bit 2 – CPHA: Clock Phase */
		uint8 _CPOL:1;			/* Bit 3 – CPOL: Clock Polarity */
		uint8 _MSTR:1;			/* Bit 4 – MSTR: Master/Slave Select */
		uint8 _DORD:1;			/* Bit 5 – DORD: Data Order */
		uint8 _SPE :1;			/* Bit 6 – SPE: SPI Enable */
		uint8 _SPIE:1;			/* Bit 7 – SPIE: SPI Interrupt Enable */
	};

	struct
	{
		uint8 _SPRx:2;			/* Bits 1, 0 – SPR1, SPR0: SPI Clock Rate Select 1 and 0 */
		uint8 _CPHA:1;			/* Bit 2 – CPHA: Clock Phase */
		uint8 _CPOL:1;			/* Bit 3 – CPOL: Clock Polarity */
		uint8 _MSTR:1;			/* Bit 4 – MSTR: Master/Slave Select */
		uint8 _DORD:1;			/* Bit 5 – DORD: Data Order */
		uint8 _SPE :1;			/* Bit 6 – SPE: SPI Enable */
		uint8 _SPIE:1;			/* Bit 7 – SPIE: SPI Interrupt Enable */
	};
}SPCR_CFG_t;

/*
 * ---------------------------------
 * USART Registers
 * ---------------------------------
 */

/* USART I/O Data Register */
typedef union
{
	uint8 Byte;

	struct
	{
		uint8 UDR_bit0:1;
		uint8 UDR_bit1:1;
		uint8 UDR_bit2:1;
		uint8 UDR_bit3:1;
		uint8 UDR_bit4:1;
		uint8 UDR_bit5:1;
		uint8 UDR_bit6:1;
		uint8 UDR_bit7:1;
	};
}UDR_CFG_t;

/* USART Control and Status Register A */
typedef union
{
	uint8 Byte;

	struct
	{
		uint8 _MPCM:1;			/* Bit 0 – MPCM: Multi-processor Communication Mode */
		uint8 _U2X :1;			/* Bit 1 – U2X: Double the USART Transmission Speed */
		uint8 _PE  :1;			/* Bit 2 – PE: Parity Error */
		uint8 _DOR :1;			/* Bit 3 – DOR: Data OverRun */
		uint8 _FE  :1;			/* Bit 4 – FE: Frame Error */
		uint8 _UDRE:1;			/* Bit 5 – UDRE: USART Data Register Empty */
		uint8 _TXC :1;			/* Bit 6 – TXC: USART Transmit Complete */
		uint8 _RXC :1;			/* Bit 7 – RXC: USART Receive Complete */
	};
}UCSRA_CFG_t;

/* USART Control and Status Register B */
typedef union
{
	uint8 Byte;

	struct
	{
		uint8 _TXB8 :1;			/* Bit 0 – TXB8: Transmit Data Bit 8 */
		uint8 _RXB8 :1;			/* Bit 1 – RXB8: Receive Data Bit 8 */
		uint8 _UCSZ2:1;			/* Bit 2 – UCSZ2: Character Size */
		uint8 _TXEN :1;			/* Bit 3 – TXEN: Transmitter Enable */
		uint8 _RXEN :1;			/* Bit 4 – RXEN: Receiver Enable */
		uint8 _UDRIE:1;			/* Bit 5 – UDRIE: USART Data Register Empty Interrupt Enable */
		uint8 _TXCIE:1;			/* Bit 6 – TXCIE: TX Complete Interrupt Enable */
		uint8 _RXCIE:1;			/* Bit 7 – RXCIE: RX Complete Interrupt Enable */
	};
}UCSRB_CFG_t;

/* USART Control and Status Register C */
typedef union
{
	uint8 Byte;

	struct
	{
		uint8 _UCPOL:1;			/* Bit 0 – UCPOL: Clock Polarity */
		uint8 _UCSZ0:1;			/* Bit 1 – UCSZ0: Character Size */
		uint8 _UCSZ1:1;			/* Bit 2 – UCSZ1: Character Size */
		uint8 _USBS :1;			/* Bit 3 – USBS: Stop Bit Select */
		uint8 _UPM0 :1;			/* Bit 4 – UPM0: Parity Mode */
		uint8 _UPM1 :1;			/* Bit 5 – UPM1: Parity Mode */
		uint8 _UMSEL:1;			/* Bit 6 – UMSEL: USART Mode Select */
		uint8 _URSEL:1;			/* Bit 7 – URSEL: Register Select */
	};

	struct
	{
		uint8 _UCPOL:1;			/* Bit 0 – UCPOL: Clock Polarity */
		uint8 _UCSZx:2;			/* Bit 2:1 – UCSZ1:0: Character Size */
		uint8 _USBS :1;			/* Bit 3 – USBS: Stop Bit Select */
		uint8 _UPMx :2;			/* Bit 5:4 – UPM1:0: Parity Mode */
		uint8 _UMSEL:1;			/* Bit 6 – UMSEL: USART Mode Select */
		uint8 _URSEL:1;			/* Bit 7 – URSEL: Register Select */
	};
}UCSRC_CFG_t;

/* USART Baud Rate Register Low Byte */
typedef union
{
	uint8 Byte;

	struct
	{
		uint8 UBRRL_bit0:1;
		uint8 UBRRL_bit1:1;
		uint8 UBRRL_bit2:1;
		uint8 UBRRL_bit3:1;
		uint8 UBRRL_bit4:1;
		uint8 UBRRL_bit5:1;
		uint8 UBRRL_bit6:1;
		uint8 UBRRL_bit7:1;
	};
}UBRRL_CFG_t;

/* USART Baud Rate Register High Byte */
typedef union
{
	uint8 Byte;

	struct
	{
		uint8 UBRRL_bit0:1;
		uint8 UBRRL_bit1:1;
		uint8 UBRRL_bit2:1;
		uint8 UBRRL_bit3:1;
		uint8 			:1;		/* Reserved */
		uint8 			:1;		/* Reserved */
		uint8 			:1;		/* Reserved */
		uint8 _URSEL	:1;		/* Bit 15 – URSEL: Register Select */
	};

	struct
	{
		uint8 UBRRHx:4;			/* The UBRRH contains the four most significant bits of USART baud rate */
		uint8		:3;			/* Reserved */
		uint8 _URSEL:1;			/* Bit 15 – URSEL: Register Select */
	};
}UBRRH_CFG_t;

/*
 * ---------------------------------
 * ADC Registers
 * ---------------------------------
 */

/* ADC Multiplexer Selection Register */
typedef union
{
	uint8 Byte;

	struct
	{
		uint8 _MUX0 :1;			/* Bit 0 – MUX0: Analog Channel and Gain Selection Bits */
		uint8 _MUX1 :1;			/* Bit 1 – MUX1: Analog Channel and Gain Selection Bits */
		uint8 _MUX2 :1;			/* Bit 2 – MUX2: Analog Channel and Gain Selection Bits */
		uint8 _MUX3 :1;			/* Bit 3 – MUX3: Analog Channel and Gain Selection Bits */
		uint8 _MUX4 :1;			/* Bit 4 – MUX4: Analog Channel and Gain Selection Bits */
		uint8 _ADLAR:1;			/* Bit 5 – ADLAR: ADC Left Adjust Result */
		uint8 _REFS0:1;			/* Bit 6 – REFS0: Reference Selection Bits */
		uint8 _REFS1:1;			/* Bit 7 – REFS1: Reference Selection Bits */
	};

	struct
	{
		uint8 _MUXx :5;			/* Bits 4:0 – MUX4:0: Analog Channel and Gain Selection Bits */
		uint8 _ADLAR:1;			/* Bit 5 – ADLAR: ADC Left Adjust Result */
		uint8 _REFSx:2;			/* Bit 7:6 – REFS1:0: Reference Selection Bits */
	};
}ADMUX_CFG_t;

/* ADC Control and Status Register A */
typedef union
{
	uint8 Byte;

	struct
	{
		uint8 _ADPS0:1;			/* Bit 0 – ADPS0: ADC Prescaler Select Bits */
		uint8 _ADPS1:1;			/* Bit 1 – ADPS1: ADC Prescaler Select Bits */
		uint8 _ADPS2:1;			/* Bit 2 – ADPS2: ADC Prescaler Select Bits */
		uint8 _ADIE :1;			/* Bit 3 – ADIE: ADC Interrupt Enable */
		uint8 _ADIF :1;			/* Bit 4 – ADIF: ADC Interrupt Flag */
		uint8 _ADATE:1;			/* Bit 5 – ADATE: ADC Auto Trigger Enable */
		uint8 _ADSC :1;			/* Bit 6 – ADSC: ADC Start Conversion */
		uint8 _ADEN :1;			/* Bit 7 – ADEN: ADC Enable */
	};

	struct
	{
		uint8 _ADPSx:3;			/* Bits 2:0 – ADPS2:0: ADC Prescaler Select Bits */
		uint8 _ADIE :1;			/* Bit 3 – ADIE: ADC Interrupt Enable */
		uint8 _ADIF :1;			/* Bit 4 – ADIF: ADC Interrupt Flag */
		uint8 _ADATE:1;			/* Bit 5 – ADATE: ADC Auto Trigger Enable */
		uint8 _ADSC :1;			/* Bit 6 – ADSC: ADC Start Conversion */
		uint8 _ADEN :1;			/* Bit 7 – ADEN: ADC Enable */
	};
}ADCSRA_CFG_t;

/* The ADC Data Register */
typedef union
{
	uint16 TwoBytes;			/* ADC9:0: ADC Conversion Result */

	struct
	{
		uint16 _ADCL:8;			/* ADC7:0: ADC Conversion Result */
		uint16 _ADCH:8;			/* ADC9:8: ADC Conversion Result */
	};
}ADC_CFG_t;

/*
 * ---------------------------------
 * I2C Registers
 * ---------------------------------
 */

/* TWI Control Register */
typedef union
{
	uint8 Byte;

	struct
	{
		uint8 _TWIE :1;			/* Bit 0 – TWIE: TWI Interrupt Enable */
		uint8 		:1;			/* Reserved */
		uint8 _TWEN :1;			/* Bit 2 – TWEN: TWI Enable Bit */
		uint8 _TWWC :1;			/* Bit 3 – TWWC: TWI Write Collision Flag */
		uint8 _TWSTO:1;			/* Bit 4 – TWSTO: TWI STOP Condition Bit */
		uint8 _TWSTA:1;			/* Bit 5 – TWSTA: TWI START Condition Bit */
		uint8 _TWEA :1;			/* Bit 6 – TWEA: TWI Enable Acknowledge Bit */
		uint8 _TWINT:1;			/* Bit 7 – TWINT: TWI Interrupt Flag */
	};
}TWCR_CFG_t;

/* TWI Data Register */
typedef union
{
	uint8 Byte;					/* Bits 7..0 – TWD: TWI Data Register */

	struct
	{
		uint8 _TWD0:1;			/* Bits 7..0 – TWD: TWI Data Register */
		uint8 _TWD1:1;			/* Bits 7..0 – TWD: TWI Data Register */
		uint8 _TWD2:1;			/* Bits 7..0 – TWD: TWI Data Register */
		uint8 _TWD3:1;			/* Bits 7..0 – TWD: TWI Data Register */
		uint8 _TWD4:1;			/* Bits 7..0 – TWD: TWI Data Register */
		uint8 _TWD5:1;			/* Bits 7..0 – TWD: TWI Data Register */
		uint8 _TWD6:1;			/* Bits 7..0 – TWD: TWI Data Register */
		uint8 _TWD7:1;			/* Bits 7..0 – TWD: TWI Data Register */
	};
}TWDR_CFG_t;

/* TWI (Slave) Address Register */
typedef union
{
	uint8 Byte;

	struct
	{
		uint8 _TWGCE:1;			/* Bit 0 – TWGCE: TWI General Call Recognition Enable Bit */
		uint8 _TWA0 :1;			/* Bits 7..1 – TWA: TWI (Slave) Address Register */
		uint8 _TWA1 :1;			/* Bits 7..1 – TWA: TWI (Slave) Address Register */
		uint8 _TWA2 :1;			/* Bits 7..1 – TWA: TWI (Slave) Address Register */
		uint8 _TWA3 :1;			/* Bits 7..1 – TWA: TWI (Slave) Address Register */
		uint8 _TWA4 :1;			/* Bits 7..1 – TWA: TWI (Slave) Address Register */
		uint8 _TWA5 :1;			/* Bits 7..1 – TWA: TWI (Slave) Address Register */
		uint8 _TWA6 :1;			/* Bits 7..1 – TWA: TWI (Slave) Address Register */
	};

	struct
	{
		uint8 _TWGCE:1;			/* Bit 0 – TWGCE: TWI General Call Recognition Enable Bit */
		uint8 _TWAx :7;			/* Bits 7..1 – TWA: TWI (Slave) Address Register */
	};
}TWAR_CFG_t;

/* TWI Status Register */
typedef union
{
	uint8 Byte;

	struct
	{
		uint8 _TWPS0:1;			/* Bit 0 – TWPS0: TWI Prescaler Bits */
		uint8 _TWPS1:1;			/* Bit 1 – TWPS1: TWI Prescaler Bits */
		uint8 		:1;			/* Reserved */
		uint8 _TWS3 :1;			/* Bit 3 – TWS3: TWI Status */
		uint8 _TWS4 :1;			/* Bit 4 – TWS4: TWI Status */
		uint8 _TWS5 :1;			/* Bit 5 – TWS5: TWI Status */
		uint8 _TWS6 :1;			/* Bit 6 – TWS6: TWI Status */
		uint8 _TWS7 :1;			/* Bit 7 – TWS7: TWI Status */
	};

	struct
	{
		uint8 _TWPSx:2;			/* Bits 1..0 – TWPS: TWI Prescaler Bits */
		uint8 		:1;			/* Reserved */
		uint8 _TWSx :5;			/* Bits 7..3 – TWS: TWI Status */
	};
}TWSR_CFG_t;

/* TWI Bit Rate Register */
typedef union
{
	uint8 Byte;					/* Bits 7..0 – TWI Bit Rate Register */

	struct
	{
		uint8 _TWBR0:1;			/* Bits 7..0 – TWI Bit Rate Register */
		uint8 _TWBR1:1;			/* Bits 7..0 – TWI Bit Rate Register */
		uint8 _TWBR2:1;			/* Bits 7..0 – TWI Bit Rate Register */
		uint8 _TWBR3:1;			/* Bits 7..0 – TWI Bit Rate Register */
		uint8 _TWBR4:1;			/* Bits 7..0 – TWI Bit Rate Register */
		uint8 _TWBR5:1;			/* Bits 7..0 – TWI Bit Rate Register */
		uint8 _TWBR6:1;			/* Bits 7..0 – TWI Bit Rate Register */
		uint8 _TWBR7:1;			/* Bits 7..0 – TWI Bit Rate Register */
	};
}TWBR_CFG_t;


/* ----------------------------------------------------------------------------------- */
/* ----------------Registers Definition----------------- */

/*
 * ---------------------------------
 * SREG : Status Register
 * ---------------------------------
 */

/* Status Register */
#define _SREG					( *(volatile SREG_CFG_t * const)(SREG_ADDRESS) )

/* -- Global Interrupt Enable/Disable -- */
#define GLOBAL_INTERRUPT_ENABLE()			(_SREG._I = SET)
#define GLOBAL_INTERRUPT_DISABLE()			(_SREG._I = RESET)

/*
 * ---------------------------------
 * SFIOR : Special Function IO Register
 * ---------------------------------
 */

/* Special Function IO Register */
#define _SFIOR					( *(volatile SFIOR_CFG_t * const)(SFIOR_ADDRESS) )

/*
 * ---------------------------------
 * TIMERx Control Registers
 * ---------------------------------
 */

/* Timer/Counter Interrupt Mask Register */
#define _TIMSK					( *(volatile TIMSK_CFG_t * const)(TIMSK_ADDRESS) )

/* Timer/Counter Interrupt Flag Register */
#define _TIFR					( *(volatile TIFR_CFG_t * const)(TIFR_ADDRESS) )

/*
 * ---------------------------------
 * TIMER0 Registers
 * ---------------------------------
 */

/* Output Compare Register */
#define _OCR0					( *(volatile OCR0_CFG_t * const)(OCR0_ADDRESS) )

/* Timer/Counter Control Register */
#define _TCCR0					( *(volatile TCCR0_CFG_t * const)(TCCR0_ADDRESS) )

/* Timer/Counter Register */
#define _TCNT0					( *(volatile TCNT0_CFG_t * const)(TCNT0_ADDRESS) )

/*
 * ---------------------------------
 * TIMER1 Registers
 * ---------------------------------
 */

/* Timer/Counter1 Control Register A */
#define _TCCR1A					( *(volatile TCCR1A_CFG_t * const)(TCCR1A_ADDRESS) )

/* Timer/Counter1 Control Register B */
#define _TCCR1B					( *(volatile TCCR1B_CFG_t * const)(TCCR1B_ADDRESS) )

/* Timer/Counter Register */
#define _TCNT1					( *(volatile TCNT1_CFG_t * const)(TCNT1L_ADDRESS) )

/* Output Compare Register 1 A */
#define _OCR1A					( *(volatile OCR1A_CFG_t * const)(OCR1AL_ADDRESS) )

/* Output Compare Register 1 B */
#define _OCR1B					( *(volatile OCR1B_CFG_t * const)(OCR1BL_ADDRESS) )

/* Input Capture Register 1 */
#define _ICR1					( *(volatile ICR1_CFG_t * const)(ICR1L_ADDRESS) )

/*
 * ---------------------------------
 * TIMER2 Registers
 * ---------------------------------
 */

#define _TCCR2					( *(volatile TCCR2_CFG_t * const)(TCCR2_ADDRESS) )
#define _TCNT2					( *(volatile TCNT2_CFG_t * const)(TCNT2_ADDRESS) )
#define _OCR2					( *(volatile OCR2_CFG_t * const)(OCR2_ADDRESS) )

/*
 * ---------------------------------
 * WATCHDOG Register
 * ---------------------------------
 */

#define _WDTCR					( *(volatile WDTCR_CFG_t * const)(WDTCR_ADDRESS) )

/*
 * ---------------------------------
 * GPIO Port A Registers
 * ---------------------------------
 */

/* Port A Data Register */
#define _PORTA					( *(volatile PORTA_CFG_t * const)(PORTA_ADDRESS) )

/* Port A Data Direction Register */
#define _DDRA					( *(volatile DDRA_CFG_t * const)(DDRA_ADDRESS) )

/* Port A Input Pins Address */
#define _PINA					( *(volatile const PINA_CFG_t * const)(PINA_ADDRESS) )

/*
 * ---------------------------------
 * GPIO Port B Registers
 * ---------------------------------
 */

/* Port B Data Register */
#define _PORTB					( *(volatile PORTB_CFG_t * const)(PORTB_ADDRESS) )

/* Port B Data Direction Register */
#define _DDRB					( *(volatile DDRB_CFG_t * const)(DDRB_ADDRESS) )

/* Port B Input Pins Address */
#define _PINB					( *(volatile const PINB_CFG_t * const)(PINB_ADDRESS) )

/*
 * ---------------------------------
 * GPIO Port C Registers
 * ---------------------------------
 */

/* Port C Data Register */
#define _PORTC					( *(volatile PORTC_CFG_t * const)(PORTC_ADDRESS) )

/* Port C Data Direction Register */
#define _DDRC					( *(volatile DDRC_CFG_t * const)(DDRC_ADDRESS) )

/* Port C Input Pins Address */
#define _PINC					( *(volatile const PINC_CFG_t * const)(PINC_ADDRESS) )


/*
 * ---------------------------------
 * GPIO Port D Registers
 * ---------------------------------
 */

/* Port D Data Register */
#define _PORTD					( *(volatile PORTD_CFG_t * const)(PORTD_ADDRESS) )

/* Port D Data Direction Register */
#define _DDRD					( *(volatile DDRD_CFG_t * const)(DDRD_ADDRESS) )

/* Port D Input Pins Address */
#define _PIND					( *(volatile const PIND_CFG_t * const)(PIND_ADDRESS) )

/*
 * ---------------------------------
 * SPI Registers
 * ---------------------------------
 */

/* SPI Data Register */
#define _SPDR					( *(volatile SPDR_CFG_t * const)(SPDR_ADDRESS) )

/* SPI Status Register */
#define _SPSR					( *(volatile SPSR_CFG_t * const)(SPSR_ADDRESS) )

/* SPI Control Register */
#define _SPCR					( *(volatile SPCR_CFG_t * const)(SPCR_ADDRESS) )

/*
 * ---------------------------------
 * USART Registers
 * ---------------------------------
 */

/* USART I/O Data Register */
#define _UDR					( *(volatile UDR_CFG_t * const)(UDR_ADDRESS) )

/* USART Control and Status Register A */
#define _UCSRA					( *(volatile UCSRA_CFG_t * const)(UCSRA_ADDRESS) )

/* USART Control and Status Register B */
#define _UCSRB					( *(volatile UCSRB_CFG_t * const)(UCSRB_ADDRESS) )

/* USART Control and Status Register C */
#define _UCSRC					( *(volatile UCSRC_CFG_t * const)(UCSRC_ADDRESS) )

/* USART Baud Rate Register Low Byte */
#define _UBRRL					( *(volatile UBRRL_CFG_t * const)(UBRRL_ADDRESS) )

/* USART Baud Rate Register High Byte */
#define _UBRRH					( *(volatile UBRRH_CFG_t * const)(UBRRH_ADDRESS) )

/*
 * ---------------------------------
 * ADC Registers
 * ---------------------------------
 */

/* ADC Multiplexer Selection Register */
#define _ADMUX					( *(volatile ADMUX_CFG_t * const)(ADMUX_ADDRESS) )

/* ADC Control and Status Register A */
#define _ADCSRA					( *(volatile ADCSRA_CFG_t * const)(ADCSRA_ADDRESS) )

/* The ADC Data Register */
#define _ADC					( *(volatile ADC_CFG_t * const)(ADCL_ADDRESS) )

/*
 * ---------------------------------
 * I2C Registers
 * ---------------------------------
 */

/* TWI Control Register */
#define _TWCR					( *(volatile TWCR_CFG_t * const)(TWCR_ADDRESS) )

/* TWI Data Register */
#define _TWDR					( *(volatile TWDR_CFG_t * const)(TWDR_ADDRESS) )

/* TWI (Slave) Address Register */
#define _TWAR					( *(volatile TWAR_CFG_t * const)(TWAR_ADDRESS) )

/* TWI Status Register */
#define _TWSR					( *(volatile TWSR_CFG_t * const)(TWSR_ADDRESS) )

/* TWI Bit Rate Register */
#define _TWBR					( *(volatile TWBR_CFG_t * const)(TWBR_ADDRESS) )


/* ----------------------------------------------------------------------------------- */
/* --------------GPIO Ports Index section--------------- */

/*
 * ---------------------------------
 * GPIO Port A pin Index
 * ---------------------------------
 */

#define PA0						0
#define PA1						1
#define PA2						2
#define PA3						3
#define PA4						4
#define PA5						5
#define PA6						6
#define PA7						7

/*
 * ---------------------------------
 * GPIO Port B pin Index
 * ---------------------------------
 */

#define PB0						0
#define PB1						1
#define PB2						2
#define PB3						3
#define PB4						4
#define PB5						5
#define PB6						6
#define PB7						7

/*
 * ---------------------------------
 * GPIO Port C pin Index
 * ---------------------------------
 */

#define PC0						0
#define PC1						1
#define PC2						2
#define PC3						3
#define PC4						4
#define PC5						5
#define PC6						6
#define PC7						7

/*
 * ---------------------------------
 * GPIO Port D pin Index
 * ---------------------------------
 */

#define PD0						0
#define PD1						1
#define PD2						2
#define PD3						3
#define PD4						4
#define PD5						5
#define PD6						6
#define PD7						7


/* ----------------------------------------------------------------------------------- */
#endif /* _ATMEGA32_H_ */
