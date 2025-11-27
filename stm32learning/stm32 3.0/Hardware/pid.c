#include "pid.h"

void PID_Init(PID_Type* pid, float kp, float ki, float kd)
{
    pid->kp = kp;
    pid->ki = ki;
    pid->kd = kd;
    pid->integral = 0;
    pid->prev_error = 0;
    pid->output = 0;
}

float PID_Calculate(PID_Type* pid, float target, float current)
{
    float error = target - current;
    
    pid->integral += error;
    if(pid->integral > 100) pid->integral = 100;
    if(pid->integral < -100) pid->integral = -100;
    
    float derivative = error - pid->prev_error;
    
    pid->output = pid->kp * error + pid->ki * pid->integral + pid->kd * derivative;
    
    pid->prev_error = error;
    
    return pid->output;
}
