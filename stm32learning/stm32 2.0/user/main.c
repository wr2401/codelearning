#include "stm32f10x.h"
#include "motor.h"
#include "encoder.h"
#include "timer.h"
#include "pwm.h"
#include "pid.h"
#include "key.h"
#include "menu.h"
#include "uart.h"
#include "stdio.h"
#include "OLED.h"
// 全局变量
SystemMode_t system_mode = MODE_SPEED_CONTROL;
uint32_t system_tick = 0;

int main(void)
{
    // 只调用SystemInit() - 让标准库处理时钟
    SystemInit();
    
    // 初始化串口用于调试
    UART_Init(115200);
    
    // 各模块初始化
    Motor_Init();
    Encoder_Init();
    PWM_Init();
    Timer_Init();
    Key_Init();
	OLED_Init();
    
    // PID初始化
    PID_Init(&motor1, 0.8f, 0.2f, 0.1f);
    PID_Init(&motor2, 0.8f, 0.2f, 0.1f);
	
	TIM_SetCompare2(TIM1, 20);
	TIM_SetCompare3(TIM1, 20);
    
    while(1)
    {
        UART_ProcessData();
        Menu_Update();
		
        static uint32_t last_send = 0;
        if(system_tick - last_send > 100) {  // 100*10ms=1s
            last_send = system_tick;
            UART_SendData();  // 发送到VOFA+
        }
    }
}
