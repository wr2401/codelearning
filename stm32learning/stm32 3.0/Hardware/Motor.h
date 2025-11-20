#ifndef __MOTOR_H
#define __MOTOR_H

#include "stm32f10x.h"
#include "pid.h"

typedef struct {
    int32_t target_speed;
    int32_t actual_speed;
    int32_t pwm_output;
} Motor_t;

extern Motor_t motor1, motor2;

void Motor_Init(void);
void Motor_SetSpeed(int16_t speed1, int16_t speed2);
void Motor_UpdateSpeed(void);
void Motor_PIDControl(void);

#endif
