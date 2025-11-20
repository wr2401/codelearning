#ifndef __MOTOR_H
#define __MOTOR_H

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
void Motor1_SetSpeed(int8_t Speed);
void Motor2_SetSpeed(int8_t Speed);
#endif
