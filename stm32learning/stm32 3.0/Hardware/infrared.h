#ifndef __INFRARED_H
#define __INFRARED_H

#include "stm32f10x.h"

#define IR_LEFT_PIN      GPIO_Pin_3
#define IR_LEFT_PORT     GPIOC
#define IR_MIDDLE_PIN    GPIO_Pin_4
#define IR_MIDDLE_PORT   GPIOC
#define IR_RIGHT_PIN     GPIO_Pin_5
#define IR_RIGHT_PORT    GPIOC

void INFRARED_Init(void);
uint8_t INFRARED_ReadLeft(void);
uint8_t INFRARED_ReadMiddle(void);
uint8_t INFRARED_ReadRight(void);
int8_t INFRARED_GetLineError(void);

#endif
