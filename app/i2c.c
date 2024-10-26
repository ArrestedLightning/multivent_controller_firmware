/*
 * i2c.c
 *
 *  Created on: Mar 20, 2024
 *      Author: jjlange
 */

#include "globals.h"

void IIC_Init(u32 baud, u16 address) {
    I2C_InitTypeDef I2C_InitTSturcture = { 0 };

    I2C_InitTSturcture.I2C_ClockSpeed = baud;
    I2C_InitTSturcture.I2C_Mode = I2C_Mode_I2C;
    I2C_InitTSturcture.I2C_DutyCycle = I2C_DutyCycle_16_9;
    I2C_InitTSturcture.I2C_OwnAddress1 = address;
    I2C_InitTSturcture.I2C_Ack = I2C_Ack_Enable;
    I2C_InitTSturcture.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    I2C_Init(I2C1, &I2C_InitTSturcture);

    I2C_Cmd(I2C1, ENABLE);

    I2C_AcknowledgeConfig(I2C1, ENABLE);
}

void I2C_Master_Transmit(I2C_TypeDef *I2Cx, uint16_t slave_add, uint8_t *buff,
                         uint8_t idx) {

//    IIC_Init(400000, slave_add);

    u8 i = 0;

//    while (I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY) != RESET);
//    I2C_GenerateSTART(I2C1, ENABLE);
//    while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT));
//    I2C_Send7bitAddress(I2Cx, slave_add, I2C_Direction_Transmitter);
//    while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

    while (idx > 0) {
        if (I2C_GetFlagStatus(I2Cx, I2C_FLAG_TXE) != RESET) {
            I2C_SendData(I2Cx, buff[i]);
            idx--;
            i++;
        }
    }

    while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
        ;
    I2C_GenerateSTOP(I2Cx, ENABLE);
}

PT_THREAD(i2c_transmit_task(struct pt *pt, I2C_TypeDef *I2Cx, uint16_t slave_add, uint8_t *buff,
        uint8_t len)) {
    PT_BEGIN(pt);
    PT_WAIT_WHILE(pt, I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY) != RESET);
    I2C_GenerateSTART(I2Cx, ENABLE);
    PT_WAIT_UNTIL(pt, I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT));
    I2C_Send7bitAddress(I2Cx, slave_add, I2C_Direction_Transmitter);
    PT_WAIT_UNTIL(pt, I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
    PT_END(pt);
}
