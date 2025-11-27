#ifndef __PID_H
#define __PID_H

typedef struct
{
    float kp;
    float ki;
    float kd;
    float integral;
    float prev_error;
    float output;
} PID_Type;

void PID_Init(PID_Type* pid, float kp, float ki, float kd);
float PID_Calculate(PID_Type* pid, float target, float current);

#endif
