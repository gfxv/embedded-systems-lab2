/*
 * screen.h
 *
 *  Created on: Oct 14, 2024
 *      Author: ilya
 */

#ifndef INC_SCREEN_H_
#define INC_SCREEN_H_

#include "time.h"

#define DISPLAY_WIDTH 128
#define DISPLAY_HEIGHT 64
#define DISPLAY_BUFFER_SIZE (DISPLAY_WIDTH * DISPLAY_HEIGHT) / 8


void screen_init();
void screen_clear(void);
void screen_fill();
void screen_update_screen(void);
void screen_write_time(const time_s *time);
void screen_update();

#endif /* INC_SCREEN_H_ */
