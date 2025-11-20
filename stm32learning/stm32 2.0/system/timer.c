#include "timer.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_rcc.h"
#include "pid.h"
#include "Motor.h"
#include "encoder.h"
#include "serial.h"
#include "pwm.h"

extern SystemMode_t system_mode;
extern uint32_t system_tick;

void Timer_Init(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    
    // 使能定时器1时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
    
    // 定时器1配置 - 10ms中断
    TIM_TimeBaseStructure.TIM_Period = 1000 - 1;      // 1kHz
    TIM_TimeBaseStructure.TIM_Prescaler = 720 - 1;    // 72MHz/720 = 100kHz
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;// 重复计数器必须设为0
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
    
    // 使能定时器1中断
    TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);
    TIM_Cmd(TIM1, ENABLE);
    
    // NVIC配置
    NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;// 重复计数器必须设为UP中断
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

// 定时器1中断服务函数
void TIM1_UP_IRQHandler(void)
{
    static int32_t last_count1 = 0, last_count2 = 0;
    
    if(TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET)
    {
        TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
        
        system_tick++;
        
        // 1. 读取编码器速度
        int16_t current_count1 = Encoder_GetCount(1);
        int16_t current_count2 = Encoder_GetCount(2);
        
        int16_t speed1 = current_count1 - last_count1;
        int16_t speed2 = current_count2 - last_count2;
        
        // 处理计数器溢出
        if(speed1 > 32767) speed1 -= 65536;
        else if(speed1 < -32768) speed1 += 65536;
        if(speed2 > 32767) speed2 -= 65536;
        else if(speed2 < -32768) speed2 += 65536;
        
        motor1.actual_speed = speed1;
        motor2.actual_speed = speed2;
        motor1.encoder_count += speed1;
        motor2.encoder_count += speed2;
        
        last_count1 = current_count1;
        last_count2 = current_count2;
        
        // 2. 根据模式进行控制
        if(system_mode == MODE_SPEED_CONTROL)
        {
            // PID速度控制
            PID_Calculate(&motor1);
            PID_Calculate(&motor2);
            
            // 设置电机PWM
            //PWM_SetMotor1(motor1.pwm_output);
            //PWM_SetMotor2(motor2.pwm_output);
			
			Motor1_SetSpeed(motor1.pwm_output);
			Motor2_SetSpeed(motor2.pwm_output);
			
        }
        else
        {
            // 位置跟随控制
            Follow_Control();
        }
        
        // 3. 每100ms发送数据到上位机
        if(system_tick % 10 == 0) {
            UART_SendData();
        }
    }
}
