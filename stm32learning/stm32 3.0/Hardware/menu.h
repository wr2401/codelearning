#ifndef __MENU_H
#define __MENU_H

#include "stm32f10x.h"
#include "key.h"  // 包含key.h来获取Key_Type定义

typedef struct
{
    uint8_t on_off;
    float kp;
    float ki;
    float kd;
} Menu_Param;

typedef enum
{
    MENU_ON_OFF = 0,
    MENU_KP,
    MENU_KI,
    MENU_KD
} Menu_Item;

void MENU_Init(void);
void MENU_Update(void);
void MENU_KeyHandler(Key_Type key);
Menu_Param* MENU_GetParam(void);

#endif
