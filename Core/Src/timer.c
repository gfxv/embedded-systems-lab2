/*
 * timer.c
 *
 *  Created on: Oct 15, 2024
 *      Author: ilya
 */

#include "timer.h"
#include "sound.h"
#include "keyboard.h"
#include "buzzer.h"

static size_t time_left = 0;

static size_t time = 0;

static size_t time_hour = 0;
static size_t time_min = 0;
static size_t time_sec = 0;
static size_t mojno = 0;

static void handle_time_end() {
	Buzzer_Set_Volume(BUZZER_VOLUME_MAX);
	HAL_Delay(3000);
	Buzzer_Set_Volume(BUZZER_VOLUME_MUTE);
	HAL_Delay(1000);

}

void timer_tick() {
//	if (time_left) {
//		time_left--;
//		handle_time_end();
	if (time_sec == 0) {
		if (time_min == 0) {
			if (time_hour > 0) {
				time_hour--;
				time_sec = 59;
				time_min = 59;
			} else {
				if (mojno == 1) {
					mojno=0;
					handle_time_end();
				}
			}
		} else {
			time_min--;
			time_sec = 59;
		}
	} else {
		time_sec--;
	}
//	}
}

size_t get_time() {
	return time;
}

static key_e last_key = NONE_KEY;

void handle_key_click(const key_e *key) {
	if (*key == last_key) return;
	last_key = *key;

	switch (*key) {
	case START_KEY:
//		timer_start();
		break;
	case DELETE_KEY:
		time_hour = 0;
		time_min = 0;
		time_sec = 0;
		break;
	case ONE_KEY:
		time_hour++;
		if (time_hour > 23) {
			time_hour = 0;
		}
		break;
	case TWO_KEY:
		time_min++;
		if (time_min > 59) {
			time_min = 0;
		}
		break;
	case THREE_KEY:
		time_sec++;
		if (time_sec > 59) {
			time_sec = 0;
		}
		break;
	case FOUR_KEY:
		if (time_hour == 0) {
			time_hour = 23;
		} else {
			time_hour--;
		}
		break;
	case FIVE_KEY:
		if (time_min == 0) {
			time_min = 59;
		} else {
			time_min--;
		}
		break;
	case SIX_KEY:
		if (time_sec == 0) {
			time_sec = 59;
		} else {
			time_sec--;
		}
		break;
	default:
		break;
//		int8_t i = key_to_int(key);
//		input_time *= 10;
//		input_time += i;
	}
	mojno=1;
}

void update_timer_time(void) {
	time = time_hour * 3600 + time_min * 60 + time_sec;
}
