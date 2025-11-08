#include "menu.h"
#include "oled.h"
#include "motor.h"
#include "stdio.h"
#include "timer.h"

extern SystemMode_t system_mode;

void Menu_UpdateDisplay(void)
{
    char buffer[32];
    
    OLED_Clear();
    
    // 显示当前模式
    if(system_mode == MODE_SPEED_CONTROL) {
        OLED_ShowString(1, 1, "Mode: Speed PID");
    } else {
        OLED_ShowString(1, 1, "Mode: Follow");
    }
    
    // 显示电机1速度
    sprintf(buffer, "M1:%d/%d", motor1.actual_speed, motor1.target_speed);
    OLED_ShowString(2, 1, buffer);
	//output计算值
    sprintf(buffer, "out:%d", motor1.pwm_output);
    OLED_ShowString(2, 9, buffer);
    // 显示电机2速度
    sprintf(buffer, "M2:%d/%d", motor2.actual_speed, motor2.target_speed);
    OLED_ShowString(3, 1, buffer);
	
	sprintf(buffer, "out:%d", motor2.pwm_output);
    OLED_ShowString(3, 9, buffer);
    
    // 显示编码器位置
    sprintf(buffer, "Pos:%ld", (long)motor2.encoder_count);
    OLED_ShowString(4, 1, buffer);
}

void Menu_Update(void)
{
    static uint32_t last_update = 0;
    static uint32_t tick_count = 0;
    
    tick_count++;
    char buffer[32];
    // 每500ms更新一次显示
    if(tick_count - last_update >=200) {  // 50 * 10ms = 500ms
        // 显示电机1速度
		sprintf(buffer, "M1:%d/%d", motor1.actual_speed, motor1.target_speed);
		OLED_ShowString(2, 1, buffer);
		//output计算值
		sprintf(buffer, "out:%d", motor1.pwm_output);
		OLED_ShowString(2, 9, buffer);
		// 显示电机2速度
		sprintf(buffer, "M2:%d/%d", motor2.actual_speed, motor2.target_speed);
		OLED_ShowString(3, 1, buffer);
	
		sprintf(buffer, "out:%d", motor2.pwm_output);
		OLED_ShowString(3, 9, buffer);
    
		// 显示编码器位置
		sprintf(buffer, "Pos:%ld", (long)motor2.encoder_count);
		OLED_ShowString(4, 1, buffer);
		last_update = tick_count;
    }
}
