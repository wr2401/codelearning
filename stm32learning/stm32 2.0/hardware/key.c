#include "key.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_exti.h"
#include "menu.h"
#include "timer.h"
#include "pid.h"
#include "Motor.h"

extern SystemMode_t system_mode;

void Key_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    
    // 使能时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
    
    // 按键引脚配置 - PA0
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    // 外部中断配置
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);
    
    EXTI_InitStructure.EXTI_Line = EXTI_Line0;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  // 下降沿触发
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
    
    // NVIC配置
    NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

uint8_t Key_GetState(void)
{
    return GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0);
}

// 外部中断服务函数
void EXTI0_IRQHandler(void)
{
    if(EXTI_GetITStatus(EXTI_Line0) != RESET)
    {
        EXTI_ClearITPendingBit(EXTI_Line0);
        
        // 延时消抖（阻塞式，待优化，先放一下）
        for(volatile int i = 0; i < 10000; i++);
        
        // 确认按键仍然按下
        if(Key_GetState() == 0) {
            // 切换系统模式
            if(system_mode == MODE_SPEED_CONTROL) {
                system_mode = MODE_FOLLOW_CONTROL;
            } else {
                system_mode = MODE_SPEED_CONTROL;
            }
            
            // 重置PID参数
            PID_Reset(&motor1);
            PID_Reset(&motor2);
            
            // 更新菜单显示
            Menu_UpdateDisplay();
        }
    }
}
