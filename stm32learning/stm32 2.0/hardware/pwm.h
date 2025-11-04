#ifndef __PWM_H
#define __PWM_H

#include "stm32f10x.h"

void PWM_Init(void);
void PWM_SetMotor1(int16_t pwm);
void PWM_SetMotor2(int16_t pwm);

#endif
