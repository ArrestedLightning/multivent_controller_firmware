/*
 * can.c
 *
 *  Created on: Apr 6, 2024
 *      Author: jjlange
 */

#include "globals.h"

static void send_can_heartbeat(CanTxMsg *tx_msg);
static void process_can_message(CanRxMsg *rx_msg);

static uint16_t pending_message_id = 0;

static void can_gpio_init(void) {
    //Enable CAN transceiver by bringing up STB and EN pins
    GPIO_WriteBit(GPIOA, GPIO_Pin_6, SET);
    GPIO_WriteBit(GPIOB, GPIO_Pin_4, SET);
}

void can_init(void) {
    CAN_InitTypeDef can_init_struct;
    CAN_FilterInitTypeDef can_filter_init_struct;
    can_gpio_init();
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);
    CAN_StructInit(&can_init_struct);
    can_init_struct.CAN_Prescaler = 48000000/((3 + 4 + 1 + 1) * 50000);
    CAN_Init(CAN1, &can_init_struct);
    can_filter_init_struct.CAN_FilterActivation = ENABLE;
    can_filter_init_struct.CAN_FilterFIFOAssignment = CAN_Filter_FIFO0;
    can_filter_init_struct.CAN_FilterIdLow = 0;
    can_filter_init_struct.CAN_FilterMaskIdLow = 0;
    can_filter_init_struct.CAN_FilterIdHigh = 0x1FF;
    can_filter_init_struct.CAN_FilterMaskIdHigh = 0x1F0;
    can_filter_init_struct.CAN_FilterMode = CAN_FilterMode_IdMask;
    can_filter_init_struct.CAN_FilterNumber = 0;
    can_filter_init_struct.CAN_FilterFIFOAssignment = 0;
    CAN_FilterInit(&can_filter_init_struct);
}

PT_THREAD(can_tx_task(struct pt *pt)) {
    static timer can_timer;
    static CanTxMsg tx_msg;
    PT_BEGIN(pt);
    timer_init(&can_timer);
    while (1) {
        timer_start(&can_timer, 1000);
        PT_WAIT_UNTIL(pt, timer_is_done(&can_timer) || pending_message_id != 0);
        if (pending_message_id == 0) {
            send_can_heartbeat(&tx_msg);
        } else {
            tx_msg.DLC = 0;
            tx_msg.StdId = pending_message_id;
            CAN_Transmit(CAN1, &tx_msg);
            pending_message_id = 0;
        }
    }
    PT_END(pt);
}

static void send_can_heartbeat(CanTxMsg *tx_msg) {
    tx_msg->DLC = 1;
    tx_msg->ExtId = false;
    tx_msg->RTR = false;
    tx_msg->StdId = 0x1F0;
    tx_msg->Data[0] = 0x5A;
    tx_msg->Data[1] += 1;
    CAN_Transmit(CAN1, tx_msg);
    printf("Msg Sent!\n");
}

PT_THREAD(can_rx_task(struct pt *pt)) {
    static timer can_timer;
    static CanRxMsg rx_msg;
    PT_BEGIN(pt);
    timer_init(&can_timer);
    while (1) {
        PT_WAIT_UNTIL(pt, CAN_MessagePending(CAN1, CAN_FIFO0));
        CAN_Receive(CAN1, CAN_FIFO0, &rx_msg);
        process_can_message(&rx_msg);
    }
    PT_END(pt);
}

static void process_can_message(CanRxMsg *rx_msg) {
    printf("Msg Received: %d, %d\n", rx_msg->StdId, rx_msg->DLC);
    switch(rx_msg->StdId) {
    case 0x1F0:
        break;
    case 0x1F1:
        led_request_bar(LED_TOP_RED, LED_BAR_FULL, true);
        valve_close_set();
        fan_off_set();
        break;
    case 0x1F2:
        led_request_bar(LED_TOP_GREEN, LED_BAR_FULL, true);
        fan_on_set();
        valve_open_set();
        break;
    case 0x1F3:
        led_request_bar(LED_TOP_WHITE, LED_BAR_FULL, true);
        fan_on_set();
        break;
    }
}

void can_send_msg(uint16_t id) {
    pending_message_id = id;
}
