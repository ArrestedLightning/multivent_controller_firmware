/*
 * defines.h
 *
 *  Created on: Apr 7, 2024
 *      Author: jjlange
 */

#ifndef APP_DEFINES_H_
#define APP_DEFINES_H_

#define PWM_FULL_SCALE	0xFF

#define delta_t(t) 	(current_time_ms - t)

#define array_size(a)   (sizeof(a) / sizeof(a[0]))

#endif /* APP_DEFINES_H_ */
