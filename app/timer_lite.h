/*
 * timer_lite.h
 *
 *  Created on: May 18, 2016
 *      Author: Josh Lange
 */

#ifndef _INC_TIMER_LITE_H_
#define _INC_TIMER_LITE_H_

#include <stdint.h>
#include <stdbool.h>
#include "systick.h"

typedef struct {
    uint32_t length;
    uint32_t start_time;
} timer;

void timer_start(timer *t, uint32_t len);
bool timer_is_done(timer *t);
void timer_init(timer *t);
void delay_ms(uint32_t ms);

#endif /* _INC_TIMER_LITE_H_ */
