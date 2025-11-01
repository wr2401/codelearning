#ifndef __LED_H
#define __LED_H

#include "stm32f10x.h"

void LED_Init(void);
void LED_Flow_Control(void);
void TIM2_IRQHandler(void);

extern uint8_t ledDirection;

#endif
