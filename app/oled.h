/*
 * oled.h
 *
 *  Created on: Sep 14, 2024
 *      Author: jjlange
 */

#ifndef APP_OLED_H_
#define APP_OLED_H_

void oled_test(void);
uint8_t u8g2_gpio_and_delay_ch32(U8X8_UNUSED u8x8_t *u8x8, U8X8_UNUSED uint8_t msg, U8X8_UNUSED uint8_t arg_int, U8X8_UNUSED void *arg_ptr);
uint8_t u8x8_byte_wch32_hw_i2c(u8x8_t *u8g2, uint8_t msg, uint8_t arg_int, void *arg_ptr);

#endif /* APP_OLED_H_ */
