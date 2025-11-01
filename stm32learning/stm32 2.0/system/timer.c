#include "timer.h"
#include "stm32f10x.h"
#include "stdio.h"

uint16_t currentLEDPeriod = 5000;
//volatile uint32_t systemTick = 0;

void TIM2_Init(void)  // 流水灯定时器 500ms
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    
    // 开启TIM2时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    
    // 定时器配置
    TIM_TimeBaseStructure.TIM_Period = currentLEDPeriod - 1;         // 自动重装载值ARR
    TIM_TimeBaseStructure.TIM_Prescaler = 7200 - 1;      // 预分频PSC
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
    
    // 使能TIM2更新中断
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
    
    // 配置NVIC
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
    // 启动定时器
    TIM_Cmd(TIM2, ENABLE);
}

void LED_SetFlowSpeed(uint8_t speed)
{
    uint16_t newPeriod;
    
    // 根据速度等级设置定时器周期
    switch(speed) {
        case 0: newPeriod = 5000; break;   // 500ms
        case 1: newPeriod = 10000; break;  // 1000ms
        case 2: newPeriod = 20000; break;  // 2000ms
        default: newPeriod = 5000; break;
    }
    
    // 如果速度有变化，更新定时器
    if(newPeriod != currentLEDPeriod) {
        // 关闭定时器
        TIM_Cmd(TIM2, DISABLE);
        
        // 更新自动重装载值
        TIM2->ARR = newPeriod - 1;
        
        // 重置计数器
        TIM2->CNT = 0;
        
        // 重新启动定时器
        TIM_Cmd(TIM2, ENABLE);
        
        currentLEDPeriod = newPeriod;
    }
}

void TIM3_Init(void)  // 按键扫描定时器 10ms
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    
    // 开启TIM3时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    
    // 定时器配置
    TIM_TimeBaseStructure.TIM_Period = 1000 - 1;         // 自动重装载值
    TIM_TimeBaseStructure.TIM_Prescaler = 72 - 1;       // 预分频
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
    
    // 使能TIM3更新中断
    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
    
    // 配置NVIC
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
    // 启动定时器
    TIM_Cmd(TIM3, ENABLE);
}
