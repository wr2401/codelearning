#ifndef __ENCODER_H
#define __ENCODER_H

#include "stm32f10x.h"

void ENCODER_Init(void);
int16_t ENCODER_GetSpeed1(void);
int16_t ENCODER_GetSpeed2(void);
void ENCODER_Clear(void);

#endif
