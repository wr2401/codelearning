#include "stm32f10x.h"
#include "key.h"
#include "oled.h"
#include "pwm.h"
#include "encoder.h"
#include "timer.h"
#include "serial.h"
#include "motor.h"
#include "infrared.h"
#include "pid.h"
#include <math.h>

// 全局变量定义（只能在一个文件中定义）
int32_t target_speed = 0;
int32_t motor1_speed = 0;  // 如果需要保留这些变量
int32_t motor2_speed = 0;
uint8_t car_start = 0;

// PID控制器定义
PID_TypeDef pid_motor1;
PID_TypeDef pid_motor2;

int main(void)
{
    SystemInit();
    
    Key_Init();
    OLED_Init();
    PWM_Init();
    Encoder_Init();
    Timer_Init();
    Serial_Init();
    Motor_Init();
    Infrared_Init();
    
    PID_Init(&pid_motor1, 1.0f, 0.1f, 0.05f, 100.0f, 1000.0f);
    PID_Init(&pid_motor2, 1.0f, 0.1f, 0.05f, 100.0f, 1000.0f);
    
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    
    while(1)
    {
        Key_Check();
        
        if(car_start)
        {
            Infrared_Tracking();
            Serial_SendData();
        }
    }
}

void Car_Reset(void)
{
    target_speed = 0;
    motor1_speed = 0;
    motor2_speed = 0;
    Motor_SetSpeed(0, 0);
    PID_Clear(&pid_motor1);
    PID_Clear(&pid_motor2);
}
