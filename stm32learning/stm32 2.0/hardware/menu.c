#include "menu.h"
#include "oled.h"
#include "motor.h"
#include "stdio.h"
#include "timer.h"

extern SystemMode_t system_mode;

void Menu_Init(void)
{
    OLED_Init();
    Menu_UpdateDisplay();
}

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
    
    // 显示电机2速度
    sprintf(buffer, "M2:%d/%d", motor2.actual_speed, motor2.target_speed);
    OLED_ShowString(3, 1, buffer);
    
    // 显示编码器位置
    sprintf(buffer, "Pos:%ld", (long)motor1.encoder_count);
    OLED_ShowString(4, 1, buffer);
}

static uint8_t last_mode = 0xFF;

void Menu_Update(void)
{
	int16_t last_target_speed1=0;
	int16_t last_target_speed2=0;
	int16_t last_actual_speed1=0;
	int16_t last_actual_speed2=0;
	
    if(system_mode != last_mode) {
        Menu_UpdateDisplay();
        last_mode = system_mode;
    }
    
    if(last_target_speed1 != motor1.target_speed || last_target_speed2 != motor2.target_speed || last_actual_speed1 != motor1.actual_speed || last_actual_speed2 != motor2.actual_speed) {
        last_target_speed1=motor1.target_speed;
	    last_target_speed2=motor2.target_speed;
	    last_actual_speed1=motor1.actual_speed;
	    last_actual_speed2=motor2.actual_speed;
        Menu_UpdateDisplay();
    }
}
