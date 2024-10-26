/*
 * buzzer.c
 *
 *  Created on: Apr 6, 2024
 *      Author: jjlange
 */

#include "globals.h"

static void set_buzzer_freq(uint16_t freq) {
    uint16_t prescaler;
    prescaler = (SystemCoreClock / PWM_FULL_SCALE) / freq;
    TIM3->PSC = prescaler;
    //printf("PSC: %u\n", prescaler);
}
static void set_buzzer_volume (uint8_t volume) {
    volume /= 2;
    volume = PWM_FULL_SCALE - volume + 1;
    //printf("Vol: %d\n", volume);
    TIM3->CH2CVR = volume;
}
//static void buzzer_on(void) {
//
//}

PT_THREAD(buzzer_task(struct pt *pt)) {
    static timer buzz_timer;
    static int i;
    PT_BEGIN(pt);
    timer_init(&buzz_timer);
    set_buzzer_freq(1000);
//    set_buzzer_volume(64);
    for (i = 100; i > 0; i -= 1) {
        set_buzzer_volume(i);
        timer_start(&buzz_timer, 7);
        PT_WAIT_UNTIL(pt, timer_is_done(&buzz_timer));
    }
    set_buzzer_volume(0);
    while (1) {
        PT_YIELD(pt);
    }
    PT_END(pt);
}
