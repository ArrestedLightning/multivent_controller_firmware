/*
 * buttons.c
 *
 *  Created on: Apr 6, 2024
 *      Author: jjlange
 */

#include "globals.h"

#define BUTTON_MASK 0xE000
#define BUTTON_SHIFT 13
#define START_BUTTON_MASK 0b100
#define OVERRIDE_BUTTON_MASK 0b010
#define STOP_BUTTON_MASK 0b001


#define BUTTON_DEBOUNCE_TIME 20//ms
#define BUTTON_HOLD_TIME 2000//ms

typedef struct btn {
    btn_fn press_action;
    btn_fn long_press_action;
    uint32_t last_button_pressed_time;
    bool last_button_state;
    bool active_low;
} _button;

_button start_btn = {
        .active_low = true,
        .press_action = start_button_pressed,
};
_button override_btn = {
        .active_low = true,
        .press_action = override_button_pressed,
};;
_button stop_btn = {
        .active_low = true,
        .press_action = stop_button_pressed,
};;
_button ext_start_pin;
_button ext_stop_pin;

static uint16_t read_main_buttons(void) {
    return (GPIOC->INDR & BUTTON_MASK) >> BUTTON_SHIFT;
}

/* Handle states:
 * 1. Button was not pressed, is now pressed: Capture time pressed
 * 2. Button was pressed, is now not pressed: Check if debounce time has elapsed and handle
 * 3. Button was pressed, is still pressed: Check if hold time has elapsed and handle.
 */
static bool check_button(_button *btn, uint16_t current_button_state) {
    //force bit 0 and handle active low inversion.
    current_button_state = current_button_state ? 1 : 0;
    current_button_state ^= btn->active_low;
    //logic after this point is all active high
    if (btn->last_button_state) {//button was pressed
        if (current_button_state) {//button is still pressed
            if (delta_t(btn->last_button_pressed_time) > BUTTON_HOLD_TIME) {
                if (btn->long_press_action) {
                    btn->long_press_action();
                }
            }
        } else { //button is no longer pressed
            if (delta_t(btn->last_button_pressed_time) > BUTTON_DEBOUNCE_TIME) {
                if (btn->press_action) {
                    btn->press_action();
                }
            }
        }
    } else { //button was not pressed
        if (current_button_state) { //button is now pressed
            btn->last_button_pressed_time = current_time_ms;
        }
    }
    btn->last_button_state = current_button_state;
}

PT_THREAD(button_task(struct pt *pt)) {
    PT_BEGIN(pt);
    while (1) {
        uint16_t button_state = read_main_buttons();
        check_button(&start_btn, button_state & START_BUTTON_MASK);
        check_button(&override_btn, button_state & OVERRIDE_BUTTON_MASK);
        check_button(&stop_btn, button_state & STOP_BUTTON_MASK);
        PT_YIELD(pt);
    }
    PT_END(pt);
}
