/*
 * heartbeat.h
 *
 *  Created on: Mar 31, 2024
 *      Author: jjlange
 */

#ifndef APP_HEARTBEAT_H_
#define APP_HEARTBEAT_H_

#include "pt.h"

#define HB_PWM_FREQ	2000UL

PT_THREAD(heartbeat_task(struct pt *pt));
PT_THREAD(breathe_task(struct pt *pt));

#endif /* APP_HEARTBEAT_H_ */
