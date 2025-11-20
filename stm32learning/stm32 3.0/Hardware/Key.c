#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "key.h"

// 外部变量声明
extern uint8_t car_start;
extern int32_t target_speed;
void Car_Reset(void);

void Key_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void Key_Check(void)
{
    // 检测按键按下
    if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == 0)
    {
        // 简单延时消抖
        Delay_ms(100);
        
        // 确认按键按下
        if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == 0)
        {
            // 切换小车状态
            car_start = !car_start;
            if(car_start)
            {
                target_speed = 30;
            }
            else
            {
                Car_Reset();
            }
            
            // 等待按键释放
            while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == 0);
        }
    }
}
