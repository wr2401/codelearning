#include "stm32f10x.h"
#include "motor.h"
#include "encoder.h"
#include "timer.h"
#include "pwm.h"
#include "pid.h"
#include "key.h"
#include "menu.h"
#include "uart.h"
#include "OLED.h"
#include "Delay.h"

// 全局变量
SystemMode_t system_mode = MODE_SPEED_CONTROL;
uint32_t system_tick = 0;

// 中断服务函数声明
void TIM1_UP_IRQHandler(void);
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
    UART_Init(9600);//需在VOFA+设置相同的波特率
    // PID参数初始化
	
    PID_Init(&motor1, 0.303f, 0.05f, 0.10f); //KP增大，因为PWM范围变小
    PID_Init(&motor2, 0.303f, 0.05f, 0.10f);
    OLED_Init();
    // 初始显示
    Menu_UpdateDisplay();
    while(1)
    {
        // 处理串口数据
        UART_ProcessData();
		//PWM_SetMotor1(50); // 电机1正转，50%占空比
        //PWM_SetMotor2(-50); // 电机2反转，50%占空比
        //Delay_ms(3000);

        //PWM_SetMotor1(-50); // 电机1反转
        //PWM_SetMotor2(50);  // 电机2正转
        //Delay_ms(3000);
		
        // 菜单更新（非实时任务）
        Menu_Update();
        
    }
}
