/*
 * timer.c
 *
 *  Created on: Oct 15, 2024
 *      Author: ilya
 */

#include "timer.h"
#include "sound.h"

static size_t time_left = 0;

static void handle_time_end() {
	make_sound(1000);
}

void timer_tick() {
	if (!time_left--) {
		handle_time_end();
	}
}

size_t get_time_left() {
	return time_left;
}
