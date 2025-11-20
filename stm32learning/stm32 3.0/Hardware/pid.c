#include "pid.h"

void PID_Init(PID_TypeDef* pid, float kp, float ki, float kd, float max_output, float max_integral)
{
    pid->Kp = kp;
    pid->Ki = ki;
    pid->Kd = kd;
    pid->target = 0.0f;
    pid->actual = 0.0f;
    pid->error = 0.0f;
    pid->error_last = 0.0f;
    pid->error_sum = 0.0f;
    pid->error_max = max_integral;
    pid->output = 0.0f;
    pid->output_max = max_output;
    pid->output_min = -max_output;
}

float PID_Calculate(PID_TypeDef* pid, float target, float actual)
{
    float increment;
    
    pid->target = target;
    pid->actual = actual;
    pid->error = pid->target - pid->actual;
    
    pid->error_sum += pid->error;
    
    if(pid->error_sum > pid->error_max)
        pid->error_sum = pid->error_max;
    else if(pid->error_sum < -pid->error_max)
        pid->error_sum = -pid->error_max;
    
    increment = pid->Kp * pid->error +
                pid->Ki * pid->error_sum +
                pid->Kd * (pid->error - pid->error_last);
    
    pid->output += increment;
    
    if(pid->output > pid->output_max)
        pid->output = pid->output_max;
    else if(pid->output < pid->output_min)
        pid->output = pid->output_min;
    
    pid->error_last = pid->error;
    
    return pid->output;
}

void PID_Clear(PID_TypeDef* pid)
{
    pid->error = 0.0f;
    pid->error_last = 0.0f;
    pid->error_sum = 0.0f;
    pid->output = 0.0f;
}
