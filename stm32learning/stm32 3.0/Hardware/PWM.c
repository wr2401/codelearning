#include "stm32f10x.h"                  // Device header
#include "pwm.h"

void PWM_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;
    
    // 使能GPIOA时钟 (PWM引脚)
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    // 使能TIM2时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    
    // 配置PWM引脚
    // PA2 - TIM2_CH3 (电机2)
    // PA1 - TIM2_CH2 (电机1) 
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    // 定时器基础设置
    TIM_TimeBaseStructure.TIM_Period = 100 - 1;        // PWM周期为100
    TIM_TimeBaseStructure.TIM_Prescaler = 720 - 1;     // 72MHz/720 = 100kHz
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
    
    // PWM模式设置
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OCInitStructure.TIM_Pulse = 0;
    
    // 初始化通道2 (电机1 - PA1)
    TIM_OC2Init(TIM2, &TIM_OCInitStructure);
    TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);
    
    // 初始化通道3 (电机2 - PA2)
    TIM_OC3Init(TIM2, &TIM_OCInitStructure);
    TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);
    
    TIM_Cmd(TIM2, ENABLE);
    TIM_CtrlPWMOutputs(TIM2, ENABLE);
}

void PWM_SetCompare2(uint16_t compare)
{
    if(compare > 100) compare = 100;
    TIM_SetCompare2(TIM2, compare);  // 电机1
}

void PWM_SetCompare3(uint16_t compare)
{
    if(compare > 100) compare = 100;
    TIM_SetCompare3(TIM2, compare);  // 电机2
}
