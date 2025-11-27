#ifndef __MOTOR_H
#define __MOTOR_H

#include "stm32f10x.h"

void MOTOR_Init(void);
void MOTOR_SetSpeed(int16_t speed1, int16_t speed2);

#endif
