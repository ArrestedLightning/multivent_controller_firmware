/*
 * can.h
 *
 *  Created on: Apr 6, 2024
 *      Author: jjlange
 */

#ifndef APP_CAN_H_
#define APP_CAN_H_

#define CAN_KEEPALIVE   0x1F0 /* Sent by the master unit to indicate that the system is in the FAN ON state */
#define CAN_FAN_ON      0x1F1
#define CAN_PRE_FAN_OFF 0x1F2
#define CAN_FAN_OFF     0x1F3
#define CAN_SYNC        0x1F4

void can_init(void);
PT_THREAD(can_rx_task(struct pt *pt));
PT_THREAD(can_tx_task(struct pt *pt));
void can_send_msg(uint16_t id);

#endif /* APP_CAN_H_ */
