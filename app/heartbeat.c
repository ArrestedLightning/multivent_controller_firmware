/*
 * heartbeat.c
 *
 *  Created on: Mar 31, 2024
 *      Author: jjlange
 */

#include "globals.h"
const uint8_t gamma[] = {
    0,  1,  3,  4,  6,  7,  9, 10, 12, 13, 15, 16, 18, 19, 21, 22,
   24, 25, 27, 28, 29, 31, 32, 34, 35, 37, 38, 39, 41, 42, 44, 45,
   46, 48, 49, 51, 52, 53, 55, 56, 58, 59, 60, 62, 63, 64, 66, 67,
   68, 70, 71, 73, 74, 75, 77, 78, 79, 81, 82, 83, 84, 86, 87, 88,
   90, 91, 92, 94, 95, 96, 97, 99,100,101,103,104,105,106,108,109,
  110,111,113,114,115,116,117,119,120,121,122,123,125,126,127,128,
  129,131,132,133,134,135,136,138,139,140,141,142,143,145,146,147,
  148,149,150,151,152,153,155,156,157,158,159,160,161,162,163,164,
  165,166,167,169,170,171,172,173,174,175,176,177,178,179,180,181,
  182,183,184,185,186,187,188,189,190,190,191,192,193,194,195,196,
  197,198,199,200,201,202,202,203,204,205,206,207,208,209,209,210,
  211,212,213,214,214,215,216,217,218,218,219,220,221,221,222,223,
  224,224,225,226,227,227,228,229,229,230,231,232,232,233,234,234,
  235,235,236,237,237,238,239,239,240,240,241,241,242,243,243,244,
  244,245,245,246,246,247,247,248,248,249,249,249,250,250,251,251,
  251,252,252,252,253,253,253,253,254,254,254,254,254,254,255,255 };

PT_THREAD(heartbeat_task(struct pt *pt)) {
    static timer hb_timer;
    PT_BEGIN(pt);
    timer_init(&hb_timer);
    while (1) {
        timer_start(&hb_timer, 100);
        PT_WAIT_UNTIL(pt, timer_is_done(&hb_timer));
        GPIO_WriteBit(GPIOB, GPIO_Pin_0, Bit_SET);
        timer_start(&hb_timer, 1000);
        PT_WAIT_UNTIL(pt, timer_is_done(&hb_timer));
        GPIO_WriteBit(GPIOB, GPIO_Pin_0, Bit_RESET);
    }
    PT_END(pt);
}

#define PWM_MIN     0
#define PWM_MAX     PWM_FULL_SCALE
#define PWM_STEP     1

PT_THREAD(breathe_task(struct pt *pt)) {
    static uint16_t pwm_val = PWM_MAX;
    static bool direction = false;
    static timer hb_timer;
    PT_BEGIN(pt);
    timer_init(&hb_timer);
    while (1) {
        timer_start(&hb_timer, 15);
        PT_WAIT_UNTIL(pt, timer_is_done(&hb_timer));
        if (direction) {
            pwm_val += PWM_STEP;
            if (pwm_val > (PWM_MAX - PWM_STEP)) {
                direction = false;
            }
        } else {
            pwm_val -= PWM_STEP;
            if (pwm_val < (PWM_MIN + PWM_STEP)) {
                direction = true;
            }
        }
//        printf("%d\n", gamma8[pwm_val] + 1);
        TIM_SetCompare3(TIM3, gamma[pwm_val] + 1);
    }
    PT_END(pt);
}
