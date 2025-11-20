#include "timer.h"
#include "motor.h"

// 外部变量声明
extern uint8_t car_start;

void Timer_Init(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
    
    TIM_TimeBaseStructure.TIM_Period = 1000 - 1;
    TIM_TimeBaseStructure.TIM_Prescaler = 72 - 1;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
    
    TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);
    
    NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
    TIM_Cmd(TIM1, ENABLE);
}

void TIM1_UP_IRQHandler(void)
{
    if(TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET)
    {
        TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
        
        static uint32_t speed_update_count = 0;
        static uint32_t pid_control_count = 0;
        
        speed_update_count++;
        pid_control_count++;
        
        // 每50ms更新一次速度
        if(speed_update_count >= 50)
        {
            speed_update_count = 0;
            Motor_UpdateSpeed();
        }
        
        // 每20ms进行一次PID控制
        if(pid_control_count >= 20)
        {
            pid_control_count = 0;
            if(car_start)
            {
                Motor_PIDControl();
            }
        }
    }
}
