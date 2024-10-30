/*
 * pwm_out.h
 *
 *  Created on: Oct 26, 2024
 *      Author: jjlange
 */

#ifndef APP_PWM_OUT_H_
#define APP_PWM_OUT_H_

#define VALVE_FAN_PWM_FREQ  20000

void valve_open_set(void);
void valve_close_set(void);
void fan_on_set(void);
void fan_off_set(void);

#endif /* APP_PWM_OUT_H_ */
