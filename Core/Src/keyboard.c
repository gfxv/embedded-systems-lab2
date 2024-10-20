/*
 * keyboard.c
 *
 *  Created on: Oct 15, 2024
 *      Author: ilya
 */

#include "main.h"
#include "i2c.h"
#include "sdk_uart.h"
#include "keyboard.h"
#include "usart.h"

#define KBRD_ADDR 0xE2

HAL_StatusTypeDef Set_Keyboard( void ) {
	HAL_StatusTypeDef ret = HAL_OK;
	uint8_t buf;

	buf = 0;
	ret = PCA9538_Write_Register(KBRD_ADDR, POLARITY_INVERSION, &buf);
	if( ret != HAL_OK ) {
		UART_Transmit((uint8_t*)"Error write polarity\n");
		goto exit;
	}

	buf = 0;
	ret = PCA9538_Write_Register(KBRD_ADDR, OUTPUT_PORT, &buf);
	if( ret != HAL_OK ) {
		UART_Transmit((uint8_t*)"Error write output\n");
	}

exit:
	return ret;
}

pressed_keys get_pressed_keys(void) {
	pressed_keys result = {0};
	HAL_StatusTypeDef ret = HAL_OK;
	uint8_t buf;
	uint8_t kbd_in;
	uint8_t row_code[4] = {ROW1, ROW2, ROW3, ROW4};

	for (uint8_t row_ind = 0; row_ind < 4; ++row_ind) {
		ret = Set_Keyboard();
		if( ret != HAL_OK ) {
			UART_Transmit((uint8_t*)"Error write init\n");
		}
		buf = row_code[row_ind];
		ret = PCA9538_Write_Register(KBRD_ADDR, CONFIG, &buf);
		if( ret != HAL_OK ) {
			UART_Transmit((uint8_t*)"Error write config\n");
		}

		ret = PCA9538_Read_Inputs(KBRD_ADDR, &buf);
		if( ret != HAL_OK ) {
			UART_Transmit((uint8_t*)"Read error\n");
		}
		kbd_in = buf & 0x70;
		if( kbd_in != 0x70) {
			if( !(kbd_in & 0x10) ) {
				switch (row_code[row_ind]) {
					case ROW1:
						result.keys[0] = 1;
						break;
					case ROW2:
						result.keys[3] = 1;
						break;
					case ROW3:
						result.keys[6] = 1;
						break;
					case ROW4:
						result.keys[9] = 1;
						break;
				}
			}
			if( !(kbd_in & 0x20) ) {
				switch (row_code[row_ind]) {
					case ROW1:
						result.keys[1] = 1;
						break;
					case ROW2:
						result.keys[4] = 1;
						break;
					case ROW3:
						result.keys[7] = 1;
						break;
					case ROW4:
						result.keys[10] = 1;
						break;
				}
			}
			if( !(kbd_in & 0x40) ) {
				switch (row_code[row_ind]) {
					case ROW1:
						result.keys[2] = 1;
						break;
					case ROW2:
						result.keys[5] = 1;
						break;
					case ROW3:
						result.keys[8] = 1;
						break;
					case ROW4:
						result.keys[11] = 1;
						break;
				}
			}
		}
	}

	return result;

}

uint8_t Check_Row( uint8_t  Nrow ) {
	uint8_t Nkey = 0x00;
	HAL_StatusTypeDef ret = HAL_OK;
	uint8_t buf;
	uint8_t kbd_in;

	ret = Set_Keyboard();
	if( ret != HAL_OK ) {
		UART_Transmit((uint8_t*)"Error write init\n");
	}

	buf = Nrow;
	ret = PCA9538_Write_Register(KBRD_ADDR, CONFIG, &buf);
	if( ret != HAL_OK ) {
		UART_Transmit((uint8_t*)"Error write config\n");
	}

	ret = PCA9538_Read_Inputs(KBRD_ADDR, &buf);
	if( ret != HAL_OK ) {
		UART_Transmit((uint8_t*)"Read error\n");
	}

	kbd_in = buf & 0x70;
	Nkey = kbd_in;
	if( kbd_in != 0x70) {
		if( !(kbd_in & 0x10) ) {
			switch (Nrow) {
				case ROW1:
					Nkey = 0x04;
					break;
				case ROW2:
					Nkey = 0x04;
					break;
				case ROW3:
					Nkey = 0x04;
					break;
				case ROW4:
					Nkey = 0x04;
					break;
			}
		}
		if( !(kbd_in & 0x20) ) {
			switch (Nrow) {
				case ROW1:
					Nkey = 0x02;
					break;
				case ROW2:
					Nkey = 0x02;
					break;
				case ROW3:
					Nkey = 0x02;
					break;
				case ROW4:
					Nkey = 0x02;
					break;
			}
		}
		if( !(kbd_in & 0x40) ) {
			switch (Nrow) {
				case ROW1:
					Nkey = 0x01;
					break;
				case ROW2:
					Nkey = 0x01;
					break;
				case ROW3:
					Nkey = 0x01;
					break;
				case ROW4:
					Nkey = 0x01;
					break;
			}
		}
	}
	else Nkey = 0x00;

	return Nkey;
}


HAL_StatusTypeDef PCA9538_Read_Register(uint16_t addr, pca9538_regs_t reg, uint8_t* buf) {
	return HAL_I2C_Mem_Read(&hi2c1, addr | 1, reg, 1, buf, 1, 100);
}


HAL_StatusTypeDef PCA9538_Write_Register(uint16_t addr, pca9538_regs_t reg, uint8_t* buf) {
	return HAL_I2C_Mem_Write(&hi2c1, addr & 0xFFFE, reg, 1, buf, 1, 100);
}

HAL_StatusTypeDef PCA9538_Read_Config(uint16_t addr, uint8_t* buf) {
	HAL_StatusTypeDef ret = HAL_OK;
	uint8_t *buf_ptr = buf;
	uint8_t i;

	for( i=0; i<4; i++ ) {
		ret = PCA9538_Read_Register(addr, i, buf_ptr++);

		if(ret!=HAL_OK)
			return ret;
	}

	return ret;
}

HAL_StatusTypeDef PCA9538_Check_DefaultConfig(uint16_t addr) {
	HAL_StatusTypeDef ret = HAL_OK;
	uint8_t buf[4];

	ret = PCA9538_Read_Config(addr, buf);
	if( ret != HAL_OK ){
		UART_Transmit((uint8_t*)"Can't read default config\n");
	}
	else if ( buf[1] != 0xFF && buf[2] != 0xFF && buf[3] != 0xFF ) {
		UART_Transmit((uint8_t*)"Non-reset config\n");
		UART_Transmit(buf);
		ret = HAL_ERROR;
	}

	return ret;
}

HAL_StatusTypeDef PCA9538_Read_Inputs(uint16_t addr, uint8_t* buf) {
	return PCA9538_Read_Register(addr, INPUT_PORT, buf);
}

key_e get_pressed_key() {
	pressed_keys pressed = get_pressed_keys();
	for (size_t i = 0; i < 12; i++) {
		if (pressed.keys[i]) { return (key_e)i; }
	}
	return NONE_KEY;
}

static const int8_t KEY_INT[13] = {
	[ZERO_KEY] = 0,
	[ONE_KEY] = 1,
	[TWO_KEY] = 2,
	[THREE_KEY] = 3,
	[FOUR_KEY] = 4,
	[FIVE_KEY] = 5,
	[SIX_KEY] = 6,
	[SEVEN_KEY] = 7,
	[EUGHT_KEY] = 8,
	[NINE_KEY] = 9,
	[START_KEY] = -1,
	[DELETE_KEY] = -1,
	[NONE_KEY] = -1
};

int8_t key_to_int(const key_e *key) {
	return KEY_INT[*key];
}


