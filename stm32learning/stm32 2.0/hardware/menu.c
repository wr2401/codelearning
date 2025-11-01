#include "menu.h"
#include "OLED.h"
#include "stm32f10x.h"
#include "stdio.h"
#include "key.h"
// 全局菜单变量
MenuState currentMenu = MENU_MAIN;
uint8_t cursorPos = 0;
uint8_t isParamSelected = 0;
uint8_t ledSpeed = 0;
volatile uint8_t menuNeedUpate = 0;
float pid_kp = 0.8f, pid_ki = 0.1f, pid_kd = 0.5f;

// 菜单文本数组
char *mainMenu[] = {
    "LED Control",
    "PID",
    "Image", 
    "Angle"
};

char *ledMenu[] = {
    " LED_speed    0",
    " LED_dir      1",
};

char *pidMenu[] = {
    " kp         0.8",
    " ki         0.1",
    " kd         0.5",
};

char *imageMenu[] = {
    " Image", 
};

char *angleMenu[] = {
    " Angle",
};

void Menu_Init(void)
{
    currentMenu = MENU_MAIN;
    cursorPos = 0;
    isParamSelected = 0;
    ledSpeed = 0;
}

void OLED_Display_Menu(void)
{
    OLED_Clear();
    
    switch(currentMenu) {
        case MENU_MAIN:
            Display_Main_Menu();
            break;
        case MENU_LED_CONTROL:
            Display_LED_Menu();
            break;
        case MENU_PID:
            Display_PID_Menu();
            break;
        case MENU_IMAGE:
            Display_Image_Menu();
            break;
        case MENU_ANGLE:
            Display_Angle_Menu();
            break;
    }
}

void Display_Main_Menu(void)
{
    if(cursorPos == 0) {
        OLED_ShowString(1, 1, ">LED Control ");
    } else {
        OLED_ShowString(1, 1, " LED Control ");
    }
   
    if(cursorPos == 1) {
        OLED_ShowString(2, 1, ">PID   ");
    } else {
        OLED_ShowString(2, 1, " PID   ");
    }
	
    if(cursorPos == 2) {
        OLED_ShowString(3, 1, ">Image ");
    } else {
        OLED_ShowString(3, 1, " Image ");
    }
    
    if(cursorPos == 3) {
        OLED_ShowString(4, 1, ">Angle   ");
    } else {
        OLED_ShowString(4, 1, " Angle   ");
    }
    
}

void Display_LED_Menu(void)
{
    OLED_ShowString(1, 1, "LED Control");
    if(isParamSelected) {
        OLED_ShowString(1, 16, "E");
    }

    if(cursorPos == 0) {
        OLED_ShowString(2, 1, ">LED_speed ");
    } else {
        OLED_ShowString(2, 1, " LED_speed ");
    }
    OLED_ShowNum(2, 15, ledSpeed, 1);

    if(cursorPos == 1) {
        OLED_ShowString(3, 1, ">LED_dir   ");
    } else {
        OLED_ShowString(3, 1, " LED_dir   ");
    }
    OLED_ShowNum(3, 15, ledDirection, 1);
    
}

void Display_PID_Menu(void)
{
    OLED_ShowString(1, 1, "PID Control");
    if(isParamSelected) {
        OLED_ShowString(1, 16, "E");
    }
    char temp[10];
	
    if(cursorPos == 0) {
        OLED_ShowString(2, 1, ">kp   ");
    } else {
        OLED_ShowString(2, 1, " kp   ");
    }
    if(pid_kp>=0){
		sprintf(temp, "%.1f", pid_kp);      // 浮点数转字符串
		OLED_ShowString(2, 13, temp);
    }else{
		sprintf(temp, "%.1f", pid_kp);
		OLED_ShowString(2, 12, temp);
	}

    if(cursorPos == 1) {
        OLED_ShowString(3, 1, ">ki   ");
    } else {
        OLED_ShowString(3, 1, " ki   ");
    }
    if(pid_ki>=0){
		sprintf(temp, "%.1f", pid_ki); 
		OLED_ShowString(3, 13, temp);
    }else{
		sprintf(temp, "%.1f", pid_ki);
		OLED_ShowString(3, 12, temp);
	}

    if(cursorPos == 2) {
        OLED_ShowString(4, 1, ">kd   ");
    } else {
        OLED_ShowString(4, 1, " kd   ");
    }
    if(pid_kd>=0){
		sprintf(temp, "%.1f", pid_kd); 
		OLED_ShowString(4, 13, temp);
    }else{
		sprintf(temp, "%.1f", pid_kd);
		OLED_ShowString(4, 12, temp);
	}
    
}

void Display_Image_Menu(void)
{
    OLED_ShowString(1, 1, "Image Menu");
    if(isParamSelected) {
        OLED_ShowString(1, 16, "E");
    }
    if(cursorPos == 0) {
		OLED_ShowString(2, 1, ">Image");
    }else{
		OLED_ShowString(2, 1, " Image");
	}
}

void Display_Angle_Menu(void)
{
    OLED_ShowString(1, 1, "Angle Menu");
    if(isParamSelected) {
        OLED_ShowString(1, 16, "E");
    }
    if(cursorPos == 0) {
		OLED_ShowString(2, 1, ">Angle");
    }else{
		OLED_ShowString(2, 1, " Angle");
	}
}
