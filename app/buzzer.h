/*
 * buzzer.h
 *
 *  Created on: Apr 6, 2024
 *      Author: jjlange
 */

#ifndef APP_BUZZER_H_
#define APP_BUZZER_H_

#include "pt.h"

PT_THREAD(buzzer_task(struct pt *pt));


#endif /* APP_BUZZER_H_ */
