#include "encoder.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"

void Encoder_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_ICInitTypeDef TIM_ICInitStructure;
    
    // 使能时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3 | RCC_APB1Periph_TIM4, ENABLE);
    
    // 编码器引脚初始化 - 浮空输入
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    
    // 电机1编码器 A6(TIM3_CH1), A7(TIM3_CH2)
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    // 电机2编码器 B6(TIM4_CH1), B7(TIM4_CH2)
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    // 定时器3时钟源配置 - 电机1编码器
    TIM_TimeBaseStructure.TIM_Period = 0xFFFF;//即65536-1满量程计数，范围最大，方便换算负数
    TIM_TimeBaseStructure.TIM_Prescaler = 0;//不分频
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
    //输入捕获单元配置，只用滤波器和极性选择，ICPrescaler和ICSelection不用
    TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI12, 
                              TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);//都计数
    
    TIM_ICStructInit(&TIM_ICInitStructure);//防止结构体不完整造成不确定值
    TIM_ICInitStructure.TIM_ICFilter = 10;//若计数丢失，把值减小，若误计数，把值增大
    TIM_ICInit(TIM3, &TIM_ICInitStructure);
    
    TIM_SetCounter(TIM3, 0);
    TIM_Cmd(TIM3, ENABLE);
    
    // 定时器4配置 - 电机2编码器 (类似配置)
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
    TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12, 
	                           TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);//后面两个是极性，不对就改falling
    TIM_ICInit(TIM4, &TIM_ICInitStructure);
    TIM_SetCounter(TIM4, 0);
    TIM_Cmd(TIM4, ENABLE);
}

int32_t Encoder_GetCount(uint8_t encoder_num)
{
    if(encoder_num == 1) {
        return (int32_t)(int16_t)TIM_GetCounter(TIM3);
    } else {
        return (int32_t)(int16_t)TIM_GetCounter(TIM4);
    }
}

void Encoder_ClearCount(uint8_t encoder_num)
{
    if(encoder_num == 1) {
        TIM_SetCounter(TIM3, 0);
    } else {
        TIM_SetCounter(TIM4, 0);
    }
}
