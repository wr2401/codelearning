#include "stm32f10x.h"
#include "timer.h"
#include "key.h"
#include "menu.h"
#include "pwm.h"
#include "motor.h"
#include "encoder.h"
#include "pid.h"
#include "infrared.h"
#include "delay.h"
#include "oled.h"
#include <stdlib.h>
#include <math.h>

PID_Type pid_left, pid_right;
Menu_Param* menu_param;

static uint8_t in_crossroad = 0;
static uint32_t crossroad_timer = 0;

int main(void)
{
    KEY_Init();
    MENU_Init();
    OLED_Init();
    PWM_Init();
    MOTOR_Init();
    ENCODER_Init();
    INFRARED_Init();
    TIM1_Init();
    
    PID_Init(&pid_left, 1.0, 0.1, 0.01);
    PID_Init(&pid_right, 1.0, 0.1, 0.01);
    
    menu_param = MENU_GetParam();
    
    while(1)
    {
        Key_Type key = KEY_Scan();
        if(key != KEY_NONE)
        {
            MENU_KeyHandler(key);
        }
        
        if(menu_param->on_off)
        {
            // 读取编码器速度
            int16_t speed_left = ENCODER_GetSpeed1();
            int16_t speed_right = ENCODER_GetSpeed2();
            
            // 读取红外循迹误差
            int8_t line_error = INFRARED_GetLineError();
            
            // 基础速度和转向调整
            int16_t base_speed = 40;  // 根据您的PWM周期100调整
            int16_t target_left, target_right;
            
            // 十字路口处理逻辑
            if(line_error == 10 && !in_crossroad) {
                // 第一次检测到十字路口，开始直行通过
                in_crossroad = 1;
                crossroad_timer = 0;
                target_left = base_speed;
                target_right = base_speed;
            }
            else if(in_crossroad) {
                // 正在通过十字路口，继续直行
                target_left = base_speed;
                target_right = base_speed;
                crossroad_timer += 10;  // 每循环10ms，计时增加
                
                // 通过十字路口800ms后恢复循迹
                if(crossroad_timer >= 800) {
                    in_crossroad = 0;
                }
            }
            else {
                // 正常循迹模式
                int16_t adjust = line_error * 10;
                target_left = base_speed + adjust;
                target_right = base_speed - adjust;
            }
            
            // PID计算
            float out_left = PID_Calculate(&pid_left, target_left, speed_left);
            float out_right = PID_Calculate(&pid_right, target_right, speed_right);
            
            // 限制输出范围
            if(out_left > 100) out_left = 100;
            if(out_left < -100) out_left = -100;
            if(out_right > 100) out_right = 100;
            if(out_right < -100) out_right = -100;
            
            // 设置电机速度
            MOTOR_SetSpeed((int16_t)out_left, (int16_t)out_right);
        }
        else
        {
            MOTOR_SetSpeed(0, 0);
            // 停止时重置十字路口状态
            in_crossroad = 0;
            crossroad_timer = 0;
        }
        
        Delay_ms(10);  // 使用您的Delay_ms
    }
}

void TIM1_UP_IRQHandler(void)
{
    if(TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET)
    {
        // 定时更新PID参数
        pid_left.kp = menu_param->kp;
        pid_left.ki = menu_param->ki;
        pid_left.kd = menu_param->kd;
        
        pid_right.kp = menu_param->kp;
        pid_right.ki = menu_param->ki;
        pid_right.kd = menu_param->kd;
        
        TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
    }
}
