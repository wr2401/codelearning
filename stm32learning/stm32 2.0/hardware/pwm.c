#include "pwm.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "motor.h"

void PWM_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;
    
    // 使能时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    
    // PWM引脚配置 - PA2(TIM2_CH3), PA3(TIM2_CH4)
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//复用推挽输出，控制权交给片上外设
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    // 定时器1基础配置（把高级定时器当作通用定时器输出PWM）
    TIM_TimeBaseStructure.TIM_Period = 100 - 1;      // ARR
    TIM_TimeBaseStructure.TIM_Prescaler = 36 - 1;     // 72MHz/72 = 1MHz, 1kHz PWM
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
    
    // PWM输出配置
	TIM_OCStructInit(&TIM_OCInitStructure);//全部初始化，防止局部变量的不确定性
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;//设置输出比较模式
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;//设置输出使能
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;//设置输出比较极性
    TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
    TIM_OCInitStructure.TIM_Pulse = 0;//CCR
    
    // 通道3 - 电机1 PWM
    TIM_OC3Init(TIM2, &TIM_OCInitStructure);//输出比较参数，通道初始化
    TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);//CCR寄存器预装功能：写入的值在更新时生效
    
    // 通道4 - 电机2 PWM
    TIM_OC4Init(TIM2, &TIM_OCInitStructure);
    TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);
    
    // 使能定时器
    TIM_CtrlPWMOutputs(TIM2, ENABLE);
    TIM_Cmd(TIM2, ENABLE);
}

void PWM_SetMotor1(int16_t pwm)
{
    // 设置方向
    if(pwm > 0) {
        Motor_SetDirection(1, MOTOR_FORWARD);
    } else if(pwm < 0) {
        Motor_SetDirection(1, MOTOR_BACKWARD);
        pwm = -pwm;
    } else {
        Motor_SetDirection(1, MOTOR_STOP);
    }
    
    // 限制PWM值
    if(pwm > 100) pwm = 100;
    
    // 设置PWM占空比
    TIM_SetCompare3(TIM2, pwm);
}

void PWM_SetMotor2(int16_t pwm)
{
    // 设置方向
    if(pwm > 0) {
        Motor_SetDirection(2, MOTOR_FORWARD);
    } else if(pwm < 0) {
        Motor_SetDirection(2, MOTOR_BACKWARD);
        pwm = -pwm;
    } else {
        Motor_SetDirection(2, MOTOR_STOP);
    }
    
    // 限制PWM值
    if(pwm > 100) pwm = 100;
    
    // 设置PWM占空比
    TIM_SetCompare4(TIM2, pwm);
}
