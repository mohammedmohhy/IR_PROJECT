/*
 * EXTI_private.h
 *
 *  Created on: Feb 28, 2022
 *      Author: Mohy
 */

#ifndef EXTI_PRIVATE_H_
#define EXTI_PRIVATE_H_


typedef struct
{
	volatile u32 IMR;
	volatile u32 EMR;
	volatile u32 RTSR;
	volatile u32 FTSR;
	volatile u32 SWIER;
	volatile u32 PR;
}EXTI_regs;

#define EXTI_REG			((volatile EXTI_regs*)0x40010400)


#endif /* EXTI_PRIVATE_H_ */
