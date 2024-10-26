/*
 * leds.c
 *
 *  Created on: Apr 4, 2024
 *      Author: jjlange
 */


#include "globals.h"

#define TOP_WHITE_LEDS_MASK 0xAA
#define BOT_WHITE_LEDS_MASK 0x55

//PT_THREAD(test_leds(struct pt *pt));

static bool led_update_pending = false;
static LED_BAR_t pending_bar;
static uint8_t pending_bits;
static bool pending_clear;

static uint8_t led_buffer[6] = {0};

void led_dma_init(void) {
    DMA_InitTypeDef dma_init;
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

    DMA_StructInit(&dma_init);
    DMA1_Channel2->PADDR = (uint32_t)&USART3->DATAR;
    DMA1_Channel2->MADDR = (uint32_t)led_buffer;
    DMA1_Channel2->CFGR = DMA_DIR_PeripheralDST | DMA_MemoryInc_Enable | DMA_Priority_Medium;

}

static void led_dma_sendit(uint16_t len) {
    DMA1_Channel2->CFGR &= ~DMA_CFGR1_EN;
    DMA1_Channel2->CNTR = len;
    DMA1_Channel2->CFGR |= DMA_CFGR1_EN;
}

void leds_init(void) {
//    timer led_timer;
//    timer_init(&led_timer);
//    //Turn of LEDs to start with
//    led_buffer[0] = 0x18;//send command
//    led_buffer[1] = 0x0C;//display off and 8x5 mode
//    led_dma_sendit(2);
//    while(!DMA_GetFlagStatus(DMA1_FLAG_TC2));
//    timer_start(&led_timer, 5);
//    while (!timer_is_done(&led_timer));
//    //clear LEDs before turning on
//    led_buffer[0] = 0x08;//send address
//    memset(&led_buffer[1], 0, sizeof(led_buffer) - 1);
//    //rest of the buffer should already be zeroed out on startup
//    led_dma_sendit(6);
//    while(!DMA_GetFlagStatus(DMA1_FLAG_TC2));
//    timer_start(&led_timer, 5);
//    while (!timer_is_done(&led_timer));
    led_buffer[0] = 0x18;//send command
    led_buffer[1] = 0x1C;//configure brightness and 8x5 mode
    led_dma_sendit(2);
}

void led_request_bar(LED_BAR_t bar, uint8_t bits, bool clear) {
    pending_bar = bar;
    pending_bits = bits;
    pending_clear = clear;
    led_update_pending = true;
}

static void led_set_bar(LED_BAR_t bar, uint8_t bits, bool clear) {
    if (clear) {
        memset(&led_buffer[1], 0, sizeof(led_buffer) - 1);
    }
    led_buffer[0] = 0x08;
    switch (bar) {
        case LED_TOP_RED:
            led_buffer[1] = bits;
            break;
        case LED_TOP_GREEN:
            led_buffer[2] = bits;
            break;
        case LED_BOT_GREEN:
            led_buffer[3] = bits;
            break;
        case LED_BOT_RED:
            led_buffer[4] = bits;
            break;
        case LED_TOP_WHITE:
            led_buffer[5] |= bits & TOP_WHITE_LEDS_MASK;
            break;
        case LED_BOT_WHITE:
            led_buffer[5] |= bits & BOT_WHITE_LEDS_MASK;
            break;
        default:
            return;
    }
}

PT_THREAD(status_led_task(struct pt *pt)) {
    static timer led_timer;
    PT_BEGIN(pt);
    timer_init(&led_timer);
    timer_start(&led_timer, 10);
    PT_WAIT_UNTIL(pt, timer_is_done(&led_timer));
    led_set_bar(LED_TOP_RED, 0xFF, true);
    led_set_bar(LED_BOT_RED, 0xFF, false);
    led_dma_sendit(6);
    timer_start(&led_timer, 1000);
    PT_WAIT_UNTIL(pt, timer_is_done(&led_timer));
    led_set_bar(LED_TOP_WHITE, 0xFF, true);
    led_set_bar(LED_BOT_WHITE, 0xFF, false);
    led_dma_sendit(6);
    timer_start(&led_timer, 1000);
    PT_WAIT_UNTIL(pt, timer_is_done(&led_timer));
    led_set_bar(LED_TOP_GREEN, 0xFF, true);
    led_set_bar(LED_BOT_GREEN, 0xFF, false);
    led_dma_sendit(6);
    timer_start(&led_timer, 1000);
    PT_WAIT_UNTIL(pt, timer_is_done(&led_timer));
    led_set_bar(LED_NONE, 0xFF, true);
    led_dma_sendit(6);
    timer_start(&led_timer, 200);
    PT_WAIT_UNTIL(pt, timer_is_done(&led_timer));
    while (1) {
        PT_WAIT_UNTIL(pt, led_update_pending);
        led_set_bar(pending_bar, pending_bits, pending_clear);
        led_dma_sendit(6);
        //allow DMA transfer to finish
        PT_WAIT_UNTIL(pt, DMA_GetFlagStatus(DMA1_FLAG_TC2));
        //enforce silence to allow driver chip to update
        timer_start(&led_timer, 5);
        PT_WAIT_UNTIL(pt, timer_is_done(&led_timer));
    }
    PT_END(pt)
}

#if 0
PT_THREAD(test_leds(struct pt *pt)) {
    static uint16_t i;
    static timer led_test_timer;
    PT_BEGIN(pt);
    timer_init(&led_test_timer);
    for (i = 0; i < (5 * 8); i+= 1) {
        timer_start(&led_test_timer, 50);
        PT_WAIT_UNTIL(pt, timer_is_done(&led_test_timer));
        PT_WAIT_UNTIL(pt, DMA_GetFlagStatus(DMA1_FLAG_TC2));
        DMA_ClearFlag(DMA1_FLAG_TC2);
        led_buffer[0] = 0x08;//send address
        for (int j = 1; j < 6; j+=1 ) {
            if ((j - 1) == (i / 8)) {
                led_buffer[j] = 1 << (i % 8);//data
            } else {
                led_buffer[j] = 0;
            }
        }
        led_dma_sendit(6);
    }
    memset(&led_buffer[1], 0, sizeof(led_buffer) - 1);
    led_dma_sendit(6);
    PT_END(pt);
}
#endif
