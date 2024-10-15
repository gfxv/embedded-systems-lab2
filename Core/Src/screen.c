/*
 * screen.c
 *
 *  Created on: Oct 14, 2024
 *      Author: ilya
 */

#include "screen.h"
#include "font.h"
#include "i2c.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_i2c.h"

#include <stdbool.h>

size_t CURSOR_X = 0;
size_t CURSOR_Y = 0;

uint8_t DISPLAY [DISPLAY_BUFFER_SIZE] = {0};
uint8_t DISPLAY_TRANSMIT [DISPLAY_BUFFER_SIZE] = {0};

void screen_clear(void) {
	for (size_t i = 0; i < DISPLAY_BUFFER_SIZE; i++) {
		DISPLAY[i] = 0x00;
	}
}

void screen_fill() {
	for (size_t i = 0 ; i < DISPLAY_BUFFER_SIZE ; i++) {
	  DISPLAY[i] = 0xFE;
	}
}

static inline size_t x_to_page_coord(size_t x) { return x; }

static inline size_t y_to_page_coord(size_t y) { return y / 8; }

static inline size_t y_to_page_bit_number(size_t y) { return y % 8; }

static void command_to_oled(uint8_t command) {
	HAL_I2C_Mem_Write(&hi2c1,0x78,0x00,1,&command,1,10);
}

static void move_cursor_x(const size_t x_pos) {
	CURSOR_X = x_pos % DISPLAY_WIDTH;
}

static void move_cursor_start() {
	CURSOR_X = 0;
	CURSOR_Y = 0;
}

void screen_init() {
	HAL_Delay(100);

	command_to_oled(0xAE);
	command_to_oled(0x20);
	command_to_oled(0x10);
	command_to_oled(0xB0);
	command_to_oled(0xC8);
	command_to_oled(0x00);
	command_to_oled(0x10);
	command_to_oled(0x40);
	command_to_oled(0x81);
	command_to_oled(0xFF);
	command_to_oled(0xA1);
	command_to_oled(0xA6);
	command_to_oled(0xA8);
	command_to_oled(0x3F);
	command_to_oled(0xA4);
	command_to_oled(0xD3);
	command_to_oled(0x00);
	command_to_oled(0xD5);
	command_to_oled(0xF0);
	command_to_oled(0xD9);
	command_to_oled(0x22);
	command_to_oled(0xDA);
	command_to_oled(0x12);
	command_to_oled(0xDB);
	command_to_oled(0x20);
	command_to_oled(0x8D);
	command_to_oled(0x14);
	command_to_oled(0xAF);

	screen_clear();

	screen_update();

	move_cursor_start();
}

static void screen_draw_pixel(size_t x, size_t y, bool pixel_state) {
	if (x >= DISPLAY_WIDTH || y >= DISPLAY_HEIGHT) {
		return;
	}

	x = x_to_page_coord(x);
	y = x_to_page_coord(y);

	DISPLAY[x + y * DISPLAY_WIDTH] = (pixel_state ? 1 : 0) << y_to_page_bit_number(y);
}

static void screen_write_char(char c) {
	for (size_t i = 0; i < FONT_HEIDHT; i++) {
		for (size_t j = 0; j < FONT_WIDTH; i++) {
			size_t write_bit_x = CURSOR_X + j;
			size_t write_bit_y = CURSOR_Y + i;
			screen_draw_pixel(write_bit_x, write_bit_y, (1 << j) & FONT[(c - FONT_PADDING) * FONT_HEIDHT + i * sizeof(uint16_t)]);
		}
	}
}


static void screen_write_string(const char* str) {
	move_cursor_start();
	while (*str) {
		screen_write_char(*str);
		move_cursor_x(FONT_WIDTH);
		str++;
	}
}

void screen_write_time(const time_s *time) {
	char time_repr [REPRESENTATION_SIZE];
	time_to_string(time, time_repr);

	screen_write_string(time_repr);
}

void screen_update(void) {
	command_to_oled(0xB0);
	command_to_oled(0x00);
	command_to_oled(0x10);

//	screen_draw_pixel(100, 50, true);
	screen_write_char('H');

	HAL_I2C_Mem_Write(&hi2c1,0x78,0x40,I2C_MEMADD_SIZE_8BIT,&DISPLAY,DISPLAY_BUFFER_SIZE,100);
}
