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
static uint32_t last_update = 0;

void Menu_Update(void)
{
    if(system_mode != last_mode) {
        Menu_UpdateDisplay();
        last_mode = system_mode;
        last_update = system_tick;
    }
    
    if(system_tick - last_update > 200) {
        last_update = system_tick;
        Menu_UpdateDisplay();
    }
}
