/*
 * time.c
 *
 *  Created on: Oct 14, 2024
 *      Author: ilya
 */

#include "time.h"
#include <stdio.h>

#define HOUR 3600
#define MINUTE 60


time_s seconds_to_time(size_t time) {
	time_s result; // Declare a variable of type time_s
	result.hours = (time / HOUR) % 24;
	result.minutes = (time % HOUR) / MINUTE;
	result.seconds = time % MINUTE;
	return result; // Return the result
}

void time_to_string(const time_s *time, char *dest) {
	snprintf(dest, REPRESENTATION_SIZE, "%02d:%02d:%02d", time->hours, time->minutes, time->seconds);
}
