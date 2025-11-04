#ifndef __MOTOR_H
#define __MOTOR_H

#include "stm32f10x.h"

typedef enum {
    MOTOR_FORWARD = 0,
    MOTOR_BACKWARD,
    MOTOR_STOP
} MotorDirection_t;

typedef struct {
    int32_t encoder_count;
    int32_t last_encoder_count;
    int16_t actual_speed;
    int16_t target_speed;
    float kp, ki, kd;
    float error, last_error, integral;
    int16_t pwm_output;
} Motor_t;

extern Motor_t motor1, motor2;

void Motor_Init(void);
void Motor_SetDirection(uint8_t motor_num, MotorDirection_t dir);

#endif
