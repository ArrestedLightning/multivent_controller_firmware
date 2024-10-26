/*
 * systick.h
 *
 *  Created on: Mar 31, 2024
 *      Author: jjlange
 */

#ifndef VENDOR_USER_SYSTICK_H_
#define VENDOR_USER_SYSTICK_H_

#include <stdint.h>
#include "ch32v20x.h"
#include "core_riscv.h"
#include "debug.h"

extern volatile uint32_t current_time_ms;
void SysTick_Init(uint32_t interval);

#endif /* VENDOR_USER_SYSTICK_H_ */
