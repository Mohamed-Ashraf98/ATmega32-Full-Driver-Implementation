/*
 =========================================================================================
 Name        : common_macros.h
 Author      : Mohamed Ashraf El-Sayed
 Version     : 1.0.0
 Copyright   : Your copyright notice
 date        : Sun, Sep 24 2023
 time        :
 Description : Common Macros Header file , Ansi-style
 =========================================================================================
*/

#ifndef _COMMON_MACROS_H_
#define _COMMON_MACROS_H_
/* ----------------------------------------------------------------------------------- */
/* --------------Macro declaration section-------------- */

/* set any specific bit in any register */
#define SET_BIT(REG,BIT_NUM)					( (REG) = ( (REG) | (1 << BIT_NUM) ) )

/* clear any specific bit in any register */
#define CLEAR_BIT(REG,BIT_NUM)					( (REG) = ( (REG) & ~(1 << BIT_NUM) ) )

/* toggle any specific bit in any register */
#define TOGGLE_BIT(REG,BIT_NUM)					( (REG) = ( (REG) ^ (1 << BIT_NUM) ) )

/* rotate the value of any register to the right with any value */
#define ROTATE_RIGHT(REG,NUM)					( (REG) = ( (REG >> NUM) | (REG << (sizeof(REG) - NUM)) ) )

/* rotate the value of any register to the left with any value */
#define ROTATE_LEFT(REG,NUM)					( (REG) = ( (REG << NUM) | (REG >> (sizeof(REG) - NUM)) ) )

/* check if a specific bit in a register is set */
#define BIT_IS_SET(REG,BIT_NUM)					( (REG) & (1 << BIT_NUM) )

/* check if a specific bit in a register is cleared */
#define BIT_IS_CLEARED(REG,BIT_NUM)				( !( (REG) & (1 << BIT_NUM) ) )

/* get a specific bit in any register */
#define GET_BIT(REG,BIT_NUM)					( ( (REG) & (1 << BIT_NUM) ) >> BIT_NUM )


/* ----------------------------------------------------------------------------------- */
#endif /* _COMMON_MACROS_H_ */
