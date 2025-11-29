#ifndef __INFRARED_H
#define __INFRARED_H

#include "stm32f10x.h"

// 5路传感器引脚定义
#define IR_1_PIN      GPIO_Pin_4    // A4 - 最左边
#define IR_1_PORT     GPIOA
#define IR_2_PIN      GPIO_Pin_5    // A5 
#define IR_2_PORT     GPIOA
#define IR_3_PIN      GPIO_Pin_8    // A8 - 中间
#define IR_3_PORT     GPIOA
#define IR_4_PIN      GPIO_Pin_1    // B1
#define IR_4_PORT     GPIOB
#define IR_5_PIN      GPIO_Pin_4    // B3 - 最右边
#define IR_5_PORT     GPIOB

void INFRARED_Init(void);

// 5路传感器读取函数
uint8_t INFRARED_Read1(void);  // 最左边
uint8_t INFRARED_Read2(void);
uint8_t INFRARED_Read3(void);  // 中间
uint8_t INFRARED_Read4(void);
uint8_t INFRARED_Read5(void);  // 最右边

int8_t INFRARED_GetLineError(void);

#endif
