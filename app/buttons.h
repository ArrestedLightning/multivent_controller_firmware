/*
 * buttons.h
 *
 *  Created on: Apr 6, 2024
 *      Author: jjlange
 */

#ifndef APP_BUTTONS_H_
#define APP_BUTTONS_H_

#include "pt.h"

typedef void (*btn_fn)(void);

PT_THREAD(button_task(struct pt *pt));

#endif /* APP_BUTTONS_H_ */
