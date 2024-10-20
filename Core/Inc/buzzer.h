/*
 * buzzer.h
 *
 *  Created on: Oct 15, 2024
 *      Author: ilya
 */

#ifndef INC_BUZZER_H_
#define INC_BUZZER_H_

#include <stdint.h>

#define BUZZER_VOLUME_MAX	10
#define BUZZER_VOLUME_MUTE	0

void Buzzer_Init (void);
void Buzzer_Set_Freq (uint16_t freq);
void Buzzer_Set_Volume (uint16_t volume);
void Buzzer_Play (uint32_t* melody, uint32_t* delays, uint16_t len);

#endif /* INC_BUZZER_H_ */
