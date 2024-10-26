/*
 * i2c.h
 *
 *  Created on: Mar 20, 2024
 *      Author: jjlange
 */

#ifndef APP_I2C_H_
#define APP_I2C_H_

#include <stdint.h>

#define MAX_I2C_MSG_SIZE    34

typedef struct {
    uint8_t slave_addr;
    uint16_t byte_count;
    uint16_t bytes_remaining;
    uint8_t buffer[MAX_I2C_MSG_SIZE];
} i2c_queued_msg;

void IIC_Init(uint32_t baud, uint16_t address);
void I2C_Master_Transmit(I2C_TypeDef *I2Cx, uint16_t slave_add, uint8_t *buff,
                         uint8_t idx);

#endif /* APP_I2C_H_ */
