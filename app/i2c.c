/*
 * i2c.c
 *
 *  Created on: Mar 20, 2024
 *      Author: jjlange
 */

#include "globals.h"

//#define I2C_DBG    1

#ifdef I2C_DBG
#define i2c_dbg(...)   printf(__VA_ARGS__)
#else
#define i2c_dbg(...)
#endif

static void release_i2c_msg_slot(void *msg);

#define NUM_SLOTS 100

static i2c_queued_msg i2c_message_backing[NUM_SLOTS] = {0};
DLL_ALLOC(i2c_message_queue, NUM_SLOTS, release_i2c_msg_slot);

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

    u8 i = 0;

    while (I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY) != RESET);
    I2C_GenerateSTART(I2C1, ENABLE);
    while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT));
    I2C_Send7bitAddress(I2Cx, slave_add, I2C_Direction_Transmitter);
    while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

    while (idx > 0) {
        if (I2C_GetFlagStatus(I2Cx, I2C_FLAG_TXE) != RESET) {
            I2C_SendData(I2Cx, buff[i]);
            idx--;
            i++;
        }
    }

    while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
    I2C_GenerateSTOP(I2Cx, ENABLE);
}

PT_THREAD(i2c_transmit(struct pt *pt, I2C_TypeDef *I2Cx, i2c_queued_msg *msg)) {
    PT_BEGIN(pt);
    PT_WAIT_WHILE(pt, I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY) != RESET);
    i2c_dbg("GenerateStart\n");
    I2C_GenerateSTART(I2Cx, ENABLE);
    PT_WAIT_UNTIL(pt, I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT));
    i2c_dbg("SendAddr\n");
    I2C_Send7bitAddress(I2Cx, msg->slave_addr, I2C_Direction_Transmitter);
    PT_WAIT_UNTIL(pt, I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
    msg->bytes_sent = 0;
    i2c_dbg("Sending %d bytes\n", msg->byte_count);
    while (msg->bytes_sent < msg->byte_count) {
        PT_WAIT_UNTIL(pt, I2C_GetFlagStatus(I2Cx, I2C_FLAG_TXE) != RESET);
        I2C_SendData(I2Cx, msg->buffer[msg->bytes_sent]);
        msg->bytes_sent += 1;
    }
    PT_WAIT_UNTIL(pt, I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
    i2c_dbg("GenerateStop\n");
    I2C_GenerateSTOP(I2Cx, ENABLE);
    PT_END(pt);
}

PT_THREAD(i2c_task(struct pt *pt)) {
    static i2c_queued_msg *current_message;
    static struct pt pt_subthread;
    PT_BEGIN(pt);
    while (true) {
        current_message = dll_get_first(&i2c_message_queue);
        if (current_message) {
            i2c_dbg("New i2c message going out!\n");
            PT_SPAWN(pt, &pt_subthread, i2c_transmit(&pt_subthread, I2C1, current_message));
            dll_remove_start(&i2c_message_queue);
        } else {
            PT_YIELD(pt);
        }
    }
    PT_END(pt);
}

/* Finds a message slot from the backing array, marks it active, and
 * returns it.
 */
static i2c_queued_msg *find_free_i2c_msg_slot(void) {
    for (uint32_t i = 0; i < array_size(i2c_message_backing); i += 1) {
        if (!i2c_message_backing[i].active) {
            i2c_message_backing[i].active = true;
            i2c_dbg("i2c slot %d allocated\n", i);
            return &i2c_message_backing[i];
        }
    }
    return NULL;
}

static void release_i2c_msg_slot(void *msg) {
    if (msg) {
        i2c_dbg("I2C slot released: %08x\n", msg);
        i2c_queued_msg *i2c_msg = (i2c_queued_msg*) msg;
        i2c_msg->active = false;
    }
}

/* Enqueues a copy of the provided I2C message structure for transmission */
bool queue_i2c_msg(i2c_queued_msg *msg) {
    i2c_queued_msg *msg_slot;
    if (msg) {
        msg_slot = find_free_i2c_msg_slot();
        if (msg_slot) {
            //manual copy to avoid overwriting active status FIXME should use wrapper struct
            msg_slot->slave_addr = msg->slave_addr;
            msg_slot->byte_count = msg->byte_count;
            memcpy(msg_slot->buffer, msg->buffer, msg->byte_count);
//            *msg_slot = *msg;//copy data from source pointer to allocated message
            bool result = dll_insert_last(&i2c_message_queue, msg_slot);
            i2c_dbg("msg inserted: %d\n", result);
            return true;
        } else {
            printf("Out of I2C message slots!\n");
        }
    }
    return false;
}
