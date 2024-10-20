/*
 * timer.h
 *
 *  Created on: Oct 15, 2024
 *      Author: ilya
 */

#ifndef INC_TIMER_H_
#define INC_TIMER_H_

#include <stddef.h>
#include "keyboard.h"

void timer_tick();
size_t get_time();

void handle_key_click(const key_e *key);
void update_timer_time(void);

#endif /* INC_TIMER_H_ */
