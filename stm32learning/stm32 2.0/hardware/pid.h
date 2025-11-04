#ifndef __PID_H
#define __PID_H

#include "stm32f10x.h"
#include "motor.h"

void PID_Init(Motor_t* motor, float kp, float ki, float kd);
void PID_Calculate(Motor_t* motor);
void Follow_Control(void);
void PID_Reset(Motor_t* motor);

#endif
