#include "pid.h"
#include "motor.h"
#include "pwm.h"

void PID_Init(Motor_t* motor, float kp, float ki, float kd)
{
    motor->kp = kp;
    motor->ki = ki;
    motor->kd = kd;
    motor->error = 0;
    motor->last_error = 0;
    motor->integral = 0;
}

void PID_Calculate(Motor_t* motor)
{
    // 增量式PID计算
    motor->error = motor->target_speed - motor->actual_speed;
    
    float p_term = motor->kp * motor->error;
    
    // 积分项，带积分限幅
    motor->integral += motor->error;
    if(motor->integral > 1000) motor->integral = 1000;
    if(motor->integral < -1000) motor->integral = -1000;
    float i_term = motor->ki * motor->integral;
    
    // 微分项
    float d_term = motor->kd * (motor->error - motor->last_error);
    
    motor->last_error = motor->error;
    
    // 计算输出
    float output = p_term + i_term + d_term;
    
    // 输出限幅
    if(output > 800) output = 800;
    if(output < -800) output = -800;
    
    motor->pwm_output = (int16_t)output;
}

void Follow_Control(void)
{
    static float last_error = 0, integral = 0;
    
    // 位置误差
    int32_t position_error = motor1.encoder_count - motor2.encoder_count;
    
    // 位置式PID参数
    float kp = 0.5f, ki = 0.01f, kd = 0.1f;
    
    // P项
    float p_term = kp * position_error;
    
    // I项，带积分限幅
    integral += position_error;
    if(integral > 1000) integral = 1000;
    if(integral < -1000) integral = -1000;
    float i_term = ki * integral;
    
    // D项
    float d_term = kd * (position_error - last_error);
    last_error = position_error;
    
    // 计算输出
    float output = p_term + i_term + d_term;
    
    // 输出限幅
    if(output > 1000) output = 1000;
    if(output < -1000) output = -1000;
    
    // 设置电机输出
    PWM_SetMotor1(0);  // 电机1自由转动
    PWM_SetMotor2((int16_t)output);  // 电机2跟随
}

void PID_Reset(Motor_t* motor)
{
    motor->error = 0;
    motor->last_error = 0;
    motor->integral = 0;
    motor->pwm_output = 0;
}
