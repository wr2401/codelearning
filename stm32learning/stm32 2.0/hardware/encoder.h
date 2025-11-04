#ifndef __ENCODER_H
#define __ENCODER_H

#include "stm32f10x.h"

void Encoder_Init(void);
int32_t Encoder_GetCount(uint8_t encoder_num);
void Encoder_ClearCount(uint8_t encoder_num);

#endif
