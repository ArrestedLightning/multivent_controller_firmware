/*
 * vent_control.h
 *
 *  Created on: Apr 6, 2024
 *      Author: jjlange
 */

#ifndef APP_VENT_CONTROL_H_
#define APP_VENT_CONTROL_H_

/*
 * Possible states
 *
 *  IDLE - System is not doing anything
 *  START -
 */
typedef enum {
    VENT_STATE_UNKNOWN = 0,
    VENT_STATE_OPENING,
    VENT_STATE_OPEN,
    VENT_STATE_CLOSING,
    VENT_STATE_CLOSED,
} _vent_state;

typedef enum {
    FAN_STATE_OFF,
    FAN_STATE_ON,
} _fan_state;

typedef enum {
    STATION_
};

typedef enum {
    CMD_NONE,
    CMD_LOCAL_START,
    CMD_LOCAL_OVERRIDE,
    CMD_LOCAL_STOP,
    CMD_REMOTE_START,
    CMD_REMOTE_STOP,
} _system_cmd;

void start_button_pressed(void);
void override_button_pressed(void);
void stop_button_pressed(void);
void system_state_cmd(_system_cmd cmd);
PT_THREAD(vent_control_task(struct pt *pt));

#endif /* APP_VENT_CONTROL_H_ */
