#ifndef __KEY_H
#define __KEY_H

#include "stm32f10x.h"

#define KEY_UP_PIN       GPIO_Pin_0
#define KEY_UP_PORT      GPIOA
#define KEY_DOWN_PIN     GPIO_Pin_1
#define KEY_DOWN_PORT    GPIOA
#define KEY_ENTER_PIN    GPIO_Pin_2
#define KEY_ENTER_PORT   GPIOA

typedef enum
{
    KEY_UP = 0,
    KEY_DOWN,
    KEY_ENTER,
    KEY_NONE
} Key_Type;

void KEY_Init(void);
Key_Type KEY_Scan(void);

#endif
