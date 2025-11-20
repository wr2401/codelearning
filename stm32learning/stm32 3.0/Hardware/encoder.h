#ifndef __ENCODER_H
#define __ENCODER_H

#include "stm32f10x.h"

void Encoder_Init(void);
int16_t Encoder_GetCount1(void);
int16_t Encoder_GetCount2(void);
void Encoder_ClearCount1(void);
void Encoder_ClearCount2(void);

#endif
