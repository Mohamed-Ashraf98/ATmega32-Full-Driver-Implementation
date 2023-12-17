/*
 =========================================================================================
 Name        : std_types.h
 Author      : Mohamed Ashraf El-Sayed
 Version     : 1.0.0
 Copyright   : Your copyright notice
 date        : Sun, Sep 24 2023
 time        :
 Description : Standard Types Header file , Ansi-style
 =========================================================================================
*/

#ifndef _STD_TYPES_H_
#define _STD_TYPES_H_
/* ----------------------------------------------------------------------------------- */
/* ------------Macro declaration section----------- */

#ifndef ZERO_INIT
#define ZERO_INIT 		0
#endif
/* --------------------------------- */
#ifndef RESET
#define RESET			0
#endif

#ifndef SET
#define SET				1
#endif
/* --------------------------------- */
#ifndef DISABLE
#define DISABLE			0
#endif

#ifndef ENABLE
#define ENABLE			1
#endif
/* --------------------------------- */
#ifndef FALSE
#define FALSE 			0
#endif

#ifndef TRUE
#define TRUE  			1
#endif
/* --------------------------------- */
#ifndef BIT_MASK
#define BIT_MASK		1
#endif
/* --------------------------------- */
/* operation failed */
#ifndef E_NOK
#define E_NOK			(Std_ReturnType)1
#endif

/* operation success */
#ifndef E_OK
#define E_OK			(Std_ReturnType)0
#endif
/* --------------------------------- */
#define NULL_PTR		( (void *)0 )
/* --------------------------------- */
#define CPU_TYPE_8  	8
#define CPU_TYPE_16 	16
#define CPU_TYPE_32 	32
#define CPU_TYPE_64 	64


#define CPU_TYPE 		CPU_TYPE_8

/* ----------------------------------------------------------------------------------- */
/* status of the performed operation */
typedef unsigned char Std_ReturnType;
/* ----------------------------------------------------------------------------------- */


#if CPU_TYPE == CPU_TYPE_64
/* -----------CPU_TYPE_64----------- */

typedef unsigned char			boolean;
typedef unsigned char      		uint8  ;
typedef unsigned short     		uint16 ;
typedef unsigned int       		uint32 ;
typedef unsigned long long 		uint64 ;

typedef signed char        		sint8  ;
typedef signed short       		sint16 ;
typedef signed int         		sint32 ;
typedef signed long long   		sint64 ;

typedef float              		float32;
typedef double             		float64;

typedef volatile uint8     		vuint8_t ;
typedef volatile uint16    		vuint16_t;
typedef volatile uint32    		vuint32_t;
typedef volatile uint64    		vuint64_t;

typedef volatile sint8     		vint8_t  ;
typedef volatile sint16    		vint16_t ;
typedef volatile sint32    		vint32_t ;
typedef volatile sint64    		vint64_t ;


#elif CPU_TYPE == CPU_TYPE_32
/* -----------CPU_TYPE_32----------- */

typedef unsigned char			boolean;
typedef unsigned char      		uint8  ;
typedef unsigned short     		uint16 ;
typedef unsigned long      		uint32 ;
typedef unsigned long long 		uint64 ;

typedef signed char        		sint8  ;
typedef signed short       		sint16 ;
typedef signed long        		sint32 ;
typedef signed long long   		sint64 ;

typedef float              		float32;
typedef double             		float64;

typedef volatile uint8     		vuint8_t ;
typedef volatile uint16    		vuint16_t;
typedef volatile uint32    		vuint32_t;
typedef volatile uint64    		vuint64_t;

typedef volatile sint8     		vint8_t  ;
typedef volatile sint16    		vint16_t ;
typedef volatile sint32    		vint32_t ;
typedef volatile sint64    		vint64_t ;


#elif CPU_TYPE == CPU_TYPE_16
/* -----------CPU_TYPE_16----------- */

typedef unsigned char			boolean;
typedef unsigned char      		uint8  ;
typedef unsigned short     		uint16 ;
typedef unsigned long      		uint32 ;
typedef unsigned long long 		uint64 ;

typedef signed char        		sint8  ;
typedef signed short       		sint16 ;
typedef signed long        		sint32 ;
typedef signed long long   		sint64 ;

typedef float              		float32;
typedef double             		float64;

typedef volatile uint8     		vuint8_t ;
typedef volatile uint16    		vuint16_t;
typedef volatile uint32    		vuint32_t;
typedef volatile uint64    		vuint64_t;

typedef volatile sint8     		vint8_t  ;
typedef volatile sint16    		vint16_t ;
typedef volatile sint32    		vint32_t ;
typedef volatile sint64    		vint64_t ;


#elif CPU_TYPE == CPU_TYPE_8
/* ------------CPU_TYPE_8----------- */

typedef unsigned char			boolean;
typedef unsigned char      		uint8  ;
typedef unsigned short     		uint16 ;
typedef unsigned long      		uint32 ;

typedef signed char        		sint8  ;
typedef signed short       		sint16 ;
typedef signed long        		sint32 ;

typedef float              		float32;

typedef volatile uint8     		vuint8_t ;
typedef volatile uint16    		vuint16_t;
typedef volatile uint32    		vuint32_t;

typedef volatile sint8     		vint8_t  ;
typedef volatile sint16    		vint16_t ;
typedef volatile sint32    		vint32_t ;


#endif

/* ----------------------------------------------------------------------------------- */
#endif /* _STD_TYPES_H_ */
