/*
 * keyboard.h
 *
 *  Created on: Oct 15, 2024
 *      Author: ilya
 */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "main.h"

#ifndef INC_KEYBOARD_H_
#define INC_KEYBOARD_H_


#define DIP_RD_ADDR 0xE1
#define DIP_WR_ADDR 0xE0

#define ROW1 0xFE
#define ROW2 0xFD
#define ROW3 0xFB
#define ROW4 0xF7

typedef struct {
	bool keys[12];
} pressed_keys;


typedef enum {
	INPUT_PORT = 0x00, //Read byte XXXX XXXX
	OUTPUT_PORT = 0x01, //Read/write byte 1111 1111
	POLARITY_INVERSION = 0x02, //Read/write byte 0000 0000
	CONFIG = 0x03 //Read/write byte 1111 1111
} pca9538_regs_t;

typedef enum {
	ZERO_KEY = 10,
	ONE_KEY = 0,
	TWO_KEY = 1,
	THREE_KEY = 2,
	FOUR_KEY = 3,
	FIVE_KEY = 4,
	SIX_KEY = 5,
	SEVEN_KEY = 6,
	EUGHT_KEY = 7,
	NINE_KEY = 8,
	START_KEY = 9,
	DELETE_KEY = 11,
	NONE_KEY
} key_e;

typedef enum{
	LOW = 0,
	HIGH = 1
}what_bytes_t;

HAL_StatusTypeDef PCA9538_Read_Register(uint16_t addr, pca9538_regs_t reg, uint8_t* buf);
HAL_StatusTypeDef PCA9538_Write_Register(uint16_t addr, pca9538_regs_t reg, uint8_t* buf);
HAL_StatusTypeDef PCA9538_Read_Config(uint16_t addr, uint8_t* buf);
HAL_StatusTypeDef PCA9538_Check_DefaultConfig(uint16_t addr);
HAL_StatusTypeDef PCA9538_Read_Inputs(uint16_t addr, uint8_t* buf);

uint8_t Check_Row( uint8_t  Nrow );
HAL_StatusTypeDef Set_Keyboard( void );
pressed_keys get_pressed_keys(void);

key_e get_pressed_key();

int8_t key_to_int(const key_e *key);


#endif /* INC_KEYBOARD_H_ */
