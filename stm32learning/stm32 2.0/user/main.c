#include "stm32f10x.h"
#include "motor.h"
#include "encoder.h"
#include "timer.h"
#include "pwm.h"
#include "pid.h"
#include "key.h"
#include "menu.h"
#include "uart.h"

// 全局变量
SystemMode_t system_mode = MODE_SPEED_CONTROL;
uint32_t system_tick = 0;

// 中断服务函数声明（Keil MDK格式）
void TIM2_IRQHandler(void);
void USART1_IRQHandler(void);
void EXTI0_IRQHandler(void);

int main(void)
{
    // 系统初始化
    SystemInit();  // 标准库提供的系统时钟初始化
    
    // 各模块初始化
    Motor_Init();
    Encoder_Init();
    PWM_Init();
    Timer_Init();
    Key_Init();
    UART_Init(115200);
    Menu_Init();
    
    // PID参数初始化
    PID_Init(&motor1, 0.8f, 0.2f, 0.1f);
    PID_Init(&motor2, 0.8f, 0.2f, 0.1f);
    
    // 初始显示
    Menu_UpdateDisplay();
    
    while(1)
    {
        // 处理串口数据
        UART_ProcessData();
        
        // 菜单更新（非实时任务）
        Menu_Update();
    }
}
