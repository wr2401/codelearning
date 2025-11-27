#include "menu.h"
#include "oled.h"
#include "key.h"
#include <stdio.h>
#include <string.h>

static Menu_Param menu_param = {0, 1.0, 0.1, 0.01};
static Menu_Item current_item = MENU_ON_OFF;
static uint8_t edit_mode = 0;

void MENU_Init(void)
{
    menu_param.on_off = 0;
    menu_param.kp = 1.0;
    menu_param.ki = 0.1;
    menu_param.kd = 0.01;
    current_item = MENU_ON_OFF;
    edit_mode = 0;
}

void MENU_Update(void)
{
    char buf[20];
    
    OLED_Clear();
    
    // 第一行: ON/OFF
    if(current_item == MENU_ON_OFF)
        sprintf(buf, "Run:%s<", menu_param.on_off ? "ON " : "OFF");
    else
        sprintf(buf, "Run:%s ", menu_param.on_off ? "ON " : "OFF");
    OLED_ShowString(0, 0, buf);  // 移除(uint8_t*)强制转换
    
    // 第二行: KP
    if(current_item == MENU_KP)
        sprintf(buf, "KP:%.3f%s", menu_param.kp, edit_mode ? "*" : "<");
    else
        sprintf(buf, "KP:%.3f ", menu_param.kp);
    OLED_ShowString(0, 2, buf);  // 移除(uint8_t*)强制转换
    
    // 第三行: KI
    if(current_item == MENU_KI)
        sprintf(buf, "KI:%.3f%s", menu_param.ki, edit_mode ? "*" : "<");
    else
        sprintf(buf, "KI:%.3f ", menu_param.ki);
    OLED_ShowString(0, 4, buf);  // 移除(uint8_t*)强制转换
    
    // 第四行: KD
    if(current_item == MENU_KD)
        sprintf(buf, "KD:%.3f%s", menu_param.kd, edit_mode ? "*" : "<");
    else
        sprintf(buf, "KD:%.3f ", menu_param.kd);
    OLED_ShowString(0, 6, buf);  // 移除(uint8_t*)强制转换
}

void MENU_KeyHandler(Key_Type key)
{
    if(key == KEY_NONE) return;
    
    if(edit_mode)
    {
        switch(current_item)
        {
            case MENU_KP:
                if(key == KEY_UP) menu_param.kp += 0.1;
                if(key == KEY_DOWN) menu_param.kp -= 0.1;
                if(menu_param.kp < 0) menu_param.kp = 0;
                break;
                
            case MENU_KI:
                if(key == KEY_UP) menu_param.ki += 0.01;
                if(key == KEY_DOWN) menu_param.ki -= 0.01;
                if(menu_param.ki < 0) menu_param.ki = 0;
                break;
                
            case MENU_KD:
                if(key == KEY_UP) menu_param.kd += 0.001;
                if(key == KEY_DOWN) menu_param.kd -= 0.001;
                if(menu_param.kd < 0) menu_param.kd = 0;
                break;
                
            default:
                break;
        }
        
        if(key == KEY_ENTER)
        {
            edit_mode = 0;
        }
    }
    else
    {
        if(key == KEY_UP)
        {
            if(current_item > 0) current_item--;
        }
        else if(key == KEY_DOWN)
        {
            if(current_item < MENU_KD) current_item++;
        }
        else if(key == KEY_ENTER)
        {
            if(current_item == MENU_ON_OFF)
            {
                menu_param.on_off = !menu_param.on_off;
            }
            else
            {
                edit_mode = 1;
            }
        }
    }
    
    MENU_Update();
}

Menu_Param* MENU_GetParam(void)
{
    return &menu_param;
}
