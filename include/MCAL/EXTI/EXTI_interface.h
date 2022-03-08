/*
 * EXTI_interface.h
 *
 *  Created on: Feb 28, 2022
 *      Author: Mohy
 */

#ifndef EXTI_INTERFACE_H_
#define EXTI_INTERFACE_H_


void MEXTI_voidInit(u8 copy_LineNumber, u8 copy_SenseMode);
void MEXTI_voidEnableEXTI(u8 copy_LineNumber);
void MEXTI_voidDisableEXTI(u8 copy_LineNumber);
void MEXTI_voidSetCallBack(void (*CallBackFunction) (void) );


#define EXTI_LINE_0					0
#define EXTI_LINE_1 				1
#define EXTI_LINE_2 				2
#define EXTI_LINE_3 				3
#define EXTI_LINE_4 				4
#define EXTI_LINE_5 				5
#define EXTI_LINE_6 				6
#define EXTI_LINE_7 				7
#define EXTI_LINE_8 				8
#define EXTI_LINE_9 				9
#define EXTI_LINE_10				10
#define EXTI_LINE_11				11
#define EXTI_LINE_12				12
#define EXTI_LINE_13				13
#define EXTI_LINE_14				14
#define EXTI_LINE_15				15

#define EXTI_SENSE_RISING_EDGE			0
#define EXTI_SENSE_FALLING_EDGE			1
#define EXTI_SENSE_ANY_EDGE				2


#endif /* EXTI_INTERFACE_H_ */
