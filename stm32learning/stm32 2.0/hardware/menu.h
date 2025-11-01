#ifndef __MENU_H
#define __MENU_H

#include "stm32f10x.h"

// 菜单状态枚举
typedef enum {
    MENU_MAIN = 0,
    MENU_LED_CONTROL,
    MENU_PID,
    MENU_IMAGE,
    MENU_ANGLE
} MenuState;

// 全局变量声明
extern MenuState currentMenu;
extern uint8_t cursorPos;
extern uint8_t isParamSelected;
extern uint8_t ledSpeed;
extern uint8_t ledDirection;
extern float pid_kp, pid_ki, pid_kd;
extern volatile uint8_t menuNeedUpate;
// 函数声明
void Menu_Init(void);
void OLED_Display_Menu(void);
void Display_Main_Menu(void);
void Display_LED_Menu(void);
void Display_PID_Menu(void);
void Display_Image_Menu(void);
void Display_Angle_Menu(void);

#endif
