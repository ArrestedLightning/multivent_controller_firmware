/*
 * oled.c
 *
 *  Created on: Sep 14, 2024
 *      Author: jjlange
 *
 *      Portions based on https://github.com/M-Minhaj/u8g2-with-RISC-V-MCU---CH32V305-7-MCU
 */
#include "globals.h"

//#define OLED_DBG    1

#ifdef OLED_DBG
#define oled_dbg(...)   printf(__VA_ARGS__)
#else
#define oled_dbg(...)
#endif

#define OLED_ADDRESS     (0x3C << 1)

u8g2_t u8g2;

void oled_test(void) {
    u8g2_Setup_ssd1306_i2c_128x64_noname_f(&u8g2, U8G2_R0,u8x8_byte_wch32_hw_i2c,u8g2_gpio_and_delay_ch32);

    u8g2_InitDisplay(&u8g2); // send init sequence to the display, display is in sleep mode after this,
    u8g2_SetPowerSave(&u8g2, 0);
    u8g2_SetFlipMode(&u8g2, 1);
    u8g2_ClearBuffer(&u8g2);
    u8g2_SetFont(&u8g2, u8g2_font_fub14_tf);

    u8g2_DrawStr(&u8g2, 2, 30, "Hello World");

    u8g2_SendBuffer(&u8g2);
    oled_dbg("Buffer Sent\n");
}

uint8_t u8g2_gpio_and_delay_ch32(U8X8_UNUSED u8x8_t *u8x8, U8X8_UNUSED uint8_t msg, U8X8_UNUSED uint8_t arg_int, U8X8_UNUSED void *arg_ptr)
{

  switch(msg)
  {
    case U8X8_MSG_GPIO_AND_DELAY_INIT:  // called once during init phase of u8g2/u8x8
        oled_dbg("Delay Init\n");
        Delay_Init();
        break;                          // can be used to setup pins
    case U8X8_MSG_DELAY_100NANO:        // delay arg_int * 100 nano seconds
        oled_dbg("100n\n");
      for (uint16_t n = 0; arg_int*n < 20; n++)
          {
           __NOP();
          }
      break;
    case U8X8_MSG_DELAY_10MICRO:        // delay arg_int * 10 micro seconds
        oled_dbg("10u\n");
      for (uint16_t n = 0; n < arg_int*2000; n++)
          {
           __NOP();
          }


      break;
    case U8X8_MSG_DELAY_MILLI:          // delay arg_int * 1 milli second
//        printf("delayms(%u)\n", arg_int);
        delay_ms(arg_int);

      break;

    default:
      return 0;
    }

  return 1;
}

uint8_t u8x8_byte_wch32_hw_i2c(u8x8_t *u8g2, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
    // u8g2/u8x8 will never send more than 32 bytes between START_TRANSFER and END_TRANSFER
    //   add extra byte for the i2c address
//    static uint8_t buffer[34];
//    static uint8_t buf_idx;
    uint8_t *data;
    static i2c_queued_msg qmsg = {.slave_addr = OLED_ADDRESS};
    static uint32_t seq_num = 0;
    oled_dbg("i2c:%d\n", msg);
    switch(msg)  {
        case U8X8_MSG_BYTE_SEND:
            memcpy(&qmsg.buffer[qmsg.byte_count], arg_ptr, arg_int);
            qmsg.byte_count += arg_int;
            break;
        case U8X8_MSG_BYTE_INIT:
            // add your custom code to init i2c subsystem
            break;
        case U8X8_MSG_BYTE_SET_DC:
        //   ignored for i2c
            break;
        case U8X8_MSG_BYTE_START_TRANSFER:
            qmsg.byte_count = 0;
            break;
        case U8X8_MSG_BYTE_END_TRANSFER:
//            I2C_Master_Transmit(I2C1, OLED_ADDRESS, (uint8_t *)buffer, buf_idx);
            queue_i2c_msg(&qmsg);
            seq_num += 1;
            oled_dbg("seq_num: %d\n", seq_num);
            break;
        default:
            return 0;
    }

    return 1;
}
