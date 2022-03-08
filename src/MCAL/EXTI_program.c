/*
 * EXTI_program.c
 *
 *  Created on: Feb 28, 2022
 *      Author: Mohy
 */

#include "../../include/LIB/std_types.h"
#include "../../include/LIB/bit_math.h"

#include "../../include/MCAL/EXTI/EXTI_private.h"
#include "../../include/MCAL/EXTI/EXTI_configuration.h"
#include "../../include/MCAL/EXTI/EXTI_interface.h"



/*******************************************************************************************************************************
 * Function: MEXTI_voidInit
 * I/P parms: copy_LineNumber , copy_SenseMode
 * return: NONE
 * DESC: this function is used to Initialize the passed EXTI channel sense mode.
 * *****************************************************************************************************************************/
void MEXTI_voidInit(u8 copy_LineNumber, u8 copy_SenseMode)
{
	EXTI_REG->RTSR=0x00000000;
	EXTI_REG->FTSR=0x00000000;
	/*disable interrupt*/
	EXTI_REG->IMR &= ~(1<<copy_LineNumber) ;

	/*sense mode for the line*/
	switch(copy_SenseMode)
	{
	case EXTI_SENSE_RISING_EDGE:
		/*enable rising*/
		EXTI_REG->RTSR |=(1<<copy_LineNumber);
		/*disable falling*/
		EXTI_REG->FTSR &=~(1<<copy_LineNumber);
		break;

	case EXTI_SENSE_FALLING_EDGE:
		/*enable falling*/
		EXTI_REG->FTSR |=(1<<copy_LineNumber);
		/*disable rising*/
		EXTI_REG->RTSR &=~(1<<copy_LineNumber);
		break;

	case EXTI_SENSE_ANY_EDGE:
		/*enable rising*/
		EXTI_REG->RTSR |=(1<<copy_LineNumber);
		/*enable falling*/
		EXTI_REG->FTSR |=(1<<copy_LineNumber);
		break;
	}

	EXTI_REG ->PR |= (1<<0);
	EXTI_REG ->PR |= (1<<1);

}
/*******************************************************************************************************************************
 * Function: MEXTI_voidEnableEXTI
 * I/P parms: copy_LineNumber
 * return: NONE
 * DESC: this function is used to Enable the EXTI passed channel..
 * *****************************************************************************************************************************/
void MEXTI_voidEnableEXTI(u8 copy_LineNumber)
{
	/*interrupt request is not masked*/
	EXTI_REG->IMR |= (1<<copy_LineNumber);
}
/*******************************************************************************************************************************
 * Function: MEXTI_voidDisableEXTI
 * I/P parms: copy_LineNumber
 * return: NONE
 * DESC: this function is used to Disable the EXTI passed channel..
 * *****************************************************************************************************************************/
void MEXTI_voidDisableEXTI(u8 copy_LineNumber)
{
	/*interrupt request is masked*/
	EXTI_REG->IMR &=~ (1<<copy_LineNumber);
}
/*global pointer used by MEXTI_voidSetCallBack function and called in ISR */
void (*ptr3)(void);
/*******************************************************************************************************************************
 * Function: MEXTI_voidSetCallBack
 * I/P parms: void (*CallBackFunction) (void)
 * return: NONE
 * DESC: this function is used to Set call back function to the Interrupt.
 * *****************************************************************************************************************************/
void MEXTI_voidSetCallBack(void (*CallBackFunction) (void) )
{
	ptr3 = CallBackFunction;
}

/*EXTI1_ISR*/
void EXTI1_IRQHandler(void)
{
	EXTI_REG ->PR |= (1<<1);
	ptr3();
}
/*EXTI0_ISR*/
void EXTI0_IRQHandler(void)
{
	EXTI_REG ->PR |= (1<<0);
	ptr3();
}




