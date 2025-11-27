#include "motor.h"
#include "pwm.h"
void MOTOR_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    // 使能时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    
    // 配置电机控制引脚 (根据您提供的引脚配置)
    // AIN1 - PB12, AIN2 - PB13, BIN1 - PB14, BIN2 - PB15
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    // 初始化所有控制引脚为低电平
    GPIO_ResetBits(GPIOB, GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);
}

void MOTOR_SetSpeed(int16_t speed1, int16_t speed2)
{
    // 电机1 (左轮) - 对应TB6612的AO1/AO2
    if(speed1 > 0)
    {
        GPIO_SetBits(GPIOB, GPIO_Pin_12);   // AIN1 = 1
        GPIO_ResetBits(GPIOB, GPIO_Pin_13); // AIN2 = 0
        PWM_SetCompare3(speed1);               // PWMA
    }
    else if(speed1 < 0)
    {
        GPIO_ResetBits(GPIOB, GPIO_Pin_12); // AIN1 = 0
        GPIO_SetBits(GPIOB, GPIO_Pin_13);   // AIN2 = 1
        PWM_SetCompare3(-speed1);              // PWMA
    }
    else
    {
        GPIO_ResetBits(GPIOB, GPIO_Pin_12); // AIN1 = 0
        GPIO_ResetBits(GPIOB, GPIO_Pin_13); // AIN2 = 0
        PWM_SetCompare3(0);                    // PWMA
    }
    
    // 电机2 (右轮) - 对应TB6612的BO1/BO2
    if(speed2 > 0)
    {
        GPIO_SetBits(GPIOB, GPIO_Pin_14);   // BIN1 = 1
        GPIO_ResetBits(GPIOB, GPIO_Pin_15); // BIN2 = 0
        PWM_SetCompare2(speed2);               // PWMB
    }
    else if(speed2 < 0)
    {
        GPIO_ResetBits(GPIOB, GPIO_Pin_14); // BIN1 = 0
        GPIO_SetBits(GPIOB, GPIO_Pin_15);   // BIN2 = 1
        PWM_SetCompare2(-speed2);              // PWMB
    }
    else
    {
        GPIO_ResetBits(GPIOB, GPIO_Pin_14); // BIN1 = 0
        GPIO_ResetBits(GPIOB, GPIO_Pin_15); // BIN2 = 0
        PWM_SetCompare2(0);                    // PWMB
    }
}
