/*
 * sdk_uart.h
 *
 *  Created on: Oct 15, 2024
 *      Author: ilya
 */

#ifndef INC_SDK_UART_H_
#define INC_SDK_UART_H_

#include "main.h"

void UART_Transmit(uint8_t data[]);
HAL_StatusTypeDef UART_Receive(uint8_t *data, uint8_t len);

#endif /* INC_SDK_UART_H_ */
