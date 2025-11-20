#ifndef __PID_H
#define __PID_H

#include "stm32f10x.h"

typedef struct {
    float Kp;
    float Ki;
    float Kd;
    float target;
    float actual;
    float error;
    float error_last;
    float error_sum;
    float error_max;
    float output;
    float output_max;
    float output_min;
} PID_TypeDef;

void PID_Init(PID_TypeDef* pid, float kp, float ki, float kd, float max_output, float max_integral);
float PID_Calculate(PID_TypeDef* pid, float target, float actual);
void PID_Clear(PID_TypeDef* pid);

#endif
