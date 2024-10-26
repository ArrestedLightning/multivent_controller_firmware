/*
 * vent_control.c
 *
 *  Created on: Apr 6, 2024
 *      Author: jjlange
 */

#include "globals.h"

#define VENT_OPEN_TIME  15000
#define VENT_CLOSE_TIME 15000

bool is_master = false;

_vent_state current_vent_state = VENT_STATE_UNKNOWN;
_vent_state next_vent_state = VENT_STATE_UNKNOWN;

_system_cmd next_system_cmd = CMD_NONE;
bool vent_state_change = false;

/* When the start button is pressed:
 *  This station becomes the master node.
 *  "Close all stations" message is sent to all nodes 5x
 *  Local valve starts opening
 *
 *  If in menu mode, this button functions as UP
 */

void start_button_pressed(void) {
    system_state_cmd(CMD_LOCAL_START);
}

/* When the override button is pressed:
 * If system is currently active (receiving fan keep alive messages):
 *  Local valve starts opening
 *
 * If system is not currently active, holding the button will enter menu mode.
 */
void override_button_pressed(void) {
    system_state_cmd(CMD_LOCAL_OVERRIDE);
}

/* When the stop button is pressed:
 * If the system is currently active, send CAN_PRE_FAN_OFF message.
 * If this is the master node, stop sending the FAN keep alive message.
 */
void stop_button_pressed(void) {
    system_state_cmd(CMD_LOCAL_STOP);
}

void system_state_cmd(_system_cmd cmd) {
    next_system_cmd = cmd;
}

PT_THREAD(vent_control_task(struct pt *pt)) {
    PT_BEGIN(pt);
    while (1) {
        if (current_vent_state == VENT_STATE_UNKNOWN) {

        } else if (current_vent_state == VENT_STATE_OPENING) {

        } else if (current_vent_state == VENT_STATE_OPEN) {

        } else if (current_vent_state == VENT_STATE_CLOSING) {

        } else if (current_vent_state == VENT_STATE_CLOSED) {

        }
        PT_WAIT_UNTIL(pt, vent_state_change);
    }
    PT_END(pt);
}
