/*
 * systick.c
 *
 *  Created on: Mar 31, 2024
 *      Author: jjlange
 */

#include "systick.h"

#define SYSTICK_SWIE 	(1<<31)
#define SYSTICK_INIT 	(1<<5)
#define SYSTICK_MODE 	(1<<4)
#define SYSTICK_STRE 	(1<<3)
#define SYSTICK_STCLK 	(1<<2)
#define SYSTICK_STIE    (1<<1)
#define SYSTICK_STE		(1<<0)

volatile uint32_t current_time_ms = 0;

__attribute__((interrupt())) void SysTick_Handler(void) {
	current_time_ms += 1;
	SysTick->SR = 0;
}

void SysTick_Init(uint32_t interval) {
	uint32_t cmp = SystemCoreClock / interval;
	SysTick->CMP = cmp;
	SysTick->CTLR = SYSTICK_INIT | SYSTICK_STRE | SYSTICK_STCLK | SYSTICK_STIE | SYSTICK_STE;
	NVIC_SetPriority(SysTick_IRQn, 0);
	NVIC_EnableIRQ(SysTick_IRQn);
}
