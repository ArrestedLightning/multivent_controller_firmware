/*
 * leds.h
 *
 *  Created on: Apr 4, 2024
 *      Author: jjlange
 */

#ifndef APP_LEDS_H_
#define APP_LEDS_H_
#include "pt.h"
#include <stdint.h>
#include <stdbool.h>

#define LED_BAUD_RATE 19200

#define LED_BAR_FULL    0xFF

typedef enum LED_BAR_t {
    LED_TOP_RED,
    LED_TOP_GREEN,
    LED_BOT_GREEN,
    LED_BOT_RED,
    LED_TOP_WHITE,
    LED_BOT_WHITE,
    LED_NONE,
} LED_BAR_t;

void led_dma_init(void);
void leds_init(void);
PT_THREAD(status_led_task(struct pt *pt));
void led_request_bar(LED_BAR_t bar, uint8_t bits, bool clear);

#endif /* APP_LEDS_H_ */
