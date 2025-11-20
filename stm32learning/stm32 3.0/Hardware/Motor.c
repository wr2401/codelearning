#include "motor.h"
#include "pwm.h"
#include "encoder.h"

// 电机结构体定义
Motor_t motor1, motor2;

// 外部变量声明
extern PID_TypeDef pid_motor1;
extern PID_TypeDef pid_motor2;

// 速度计算相关变量
static int16_t last_count1 = 0, last_count2 = 0;

void Motor_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    
    // 电机1方向控制引脚: PB12, PB13
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    // 电机2方向控制引脚: PB14, PB15  
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14 | GPIO_Pin_15;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    // 初始化电机参数
    motor1.target_speed = 0;
    motor1.actual_speed = 0;
    motor1.pwm_output = 0;
    
    motor2.target_speed = 0;
    motor2.actual_speed = 0;
    motor2.pwm_output = 0;
    
    PWM_Init();
}

void Motor1_SetSpeed(int16_t Speed)
{	
    if (Speed >= 0)
    {
        // 正转
        GPIO_SetBits(GPIOB, GPIO_Pin_12);
        GPIO_ResetBits(GPIOB, GPIO_Pin_13);
        PWM_SetCompare2(Speed);
    }
    else
    {
        // 反转
        GPIO_ResetBits(GPIOB, GPIO_Pin_12);
        GPIO_SetBits(GPIOB, GPIO_Pin_13);
        PWM_SetCompare2(-Speed);
    }
    motor1.pwm_output = Speed;
}

void Motor2_SetSpeed(int16_t Speed)
{	
    if (Speed >= 0)
    {
        // 正转
        GPIO_SetBits(GPIOB, GPIO_Pin_14);
        GPIO_ResetBits(GPIOB, GPIO_Pin_15);
        PWM_SetCompare3(Speed);
    }
    else
    {
        // 反转
        GPIO_ResetBits(GPIOB, GPIO_Pin_14);
        GPIO_SetBits(GPIOB, GPIO_Pin_15);
        PWM_SetCompare3(-Speed);
    }
    motor2.pwm_output = Speed;
}

void Motor_SetSpeed(int16_t speed1, int16_t speed2)
{
    Motor1_SetSpeed(speed1);
    Motor2_SetSpeed(speed2);
}

void Motor_UpdateSpeed(void)
{
    int16_t current_count1, current_count2;
    
    current_count1 = Encoder_GetCount1();
    current_count2 = Encoder_GetCount2();
    
    motor1.actual_speed = current_count1 - last_count1;
    motor2.actual_speed = current_count2 - last_count2;
    
    last_count1 = current_count1;
    last_count2 = current_count2;
    
    if(abs(current_count1) > 30000)
        Encoder_ClearCount1();
    if(abs(current_count2) > 30000)
        Encoder_ClearCount2();
}

void Motor_PIDControl(void)
{
    float output1, output2;
    
    output1 = PID_Calculate(&pid_motor1, pid_motor1.target, motor1.actual_speed);
    output2 = PID_Calculate(&pid_motor2, pid_motor2.target, motor2.actual_speed);
    
    Motor_SetSpeed((int16_t)output1, (int16_t)output2);
}
