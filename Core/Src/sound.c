/*
 * sound.c
 *
 *  Created on: Oct 15, 2024
 *      Author: ilya
 */

#include "sound.h"

#include "gpio.h"

void make_sound(size_t ms_duration) {
	  HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_13);
	  HAL_Delay(500);
}
