/*
 * time.h
 *
 *  Created on: Oct 14, 2024
 *      Author: ilya
 */

#ifndef INC_TIME_H_
#define INC_TIME_H_

#include <stdint.h>
#include <stddef.h>

#define REPRESENTATION_SIZE 9  // XX:XX:XX\0

typedef struct {
	uint8_t hours;
	uint8_t minutes;
	uint8_t seconds;
} time_s;


time_s seconds_to_time(size_t time);
void time_to_string(const time_s *time, char *dest);

#endif /* INC_TIME_H_ */
