/*
 * pwm_out.c
 *
 *  Created on: Oct 26, 2024
 *      Author: jjlange
 */

#include "globals.h"

void valve_open_set(void) {
    TIM1->CH1CVR = 0;
    TIM1->CH2CVR = PWM_FULL_SCALE;
    TIM1->CH3CVR = 0;
    printf("Opening Valve\n");
}

void valve_close_set(void) {
    TIM1->CH1CVR = PWM_FULL_SCALE;
    TIM1->CH2CVR = 0;
    TIM1->CH3CVR = PWM_FULL_SCALE;
    printf("Closing Valve\n");
}

void fan_on_set(void) {
    TIM2->CH2CVR = 0;
    TIM2->CH3CVR = PWM_FULL_SCALE;
    printf("Fan On\n");
}

void fan_off_set(void) {
    TIM2->CH2CVR = PWM_FULL_SCALE;
    TIM2->CH3CVR = 0;
    printf("Fan Off\n");
}
