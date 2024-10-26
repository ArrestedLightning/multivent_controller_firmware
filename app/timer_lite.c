/*
 * timer_lite.c
 *
 *  Created on: May 18, 2016
 *      Author: Josh Lange
 */

#include "timer_lite.h"
#include "systick.h"
#include <stdint.h>
#include <stdbool.h>

extern volatile uint32_t current_time_ms;

void timer_init(timer *t) {
    t->start_time = 0;
    t->length = 0;
}

void timer_start(timer *t, uint32_t len) {
    t->start_time = current_time_ms;
    t->length = len;
}

bool timer_is_done(timer *t) {
    return ((current_time_ms - t->start_time) > t->length);
}

void delay_ms(uint32_t ms) {
    volatile uint32_t end = current_time_ms + ms;
    while (current_time_ms != end) {};
}
