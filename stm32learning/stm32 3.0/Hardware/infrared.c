#include "stm32f10x.h"                  // Device header
#include "infrared.h"

void INFRARED_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    
    GPIO_InitStructure.GPIO_Pin = IR_LEFT_PIN | IR_MIDDLE_PIN | IR_RIGHT_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
}

uint8_t INFRARED_ReadLeft(void)
{
    return GPIO_ReadInputDataBit(IR_LEFT_PORT, IR_LEFT_PIN);
}

uint8_t INFRARED_ReadMiddle(void)
{
    return GPIO_ReadInputDataBit(IR_MIDDLE_PORT, IR_MIDDLE_PIN);
}

uint8_t INFRARED_ReadRight(void)
{
    return GPIO_ReadInputDataBit(IR_RIGHT_PORT, IR_RIGHT_PIN);
}

int8_t INFRARED_GetLineError(void)
{
    uint8_t left = INFRARED_ReadLeft();
    uint8_t middle = INFRARED_ReadMiddle();
    uint8_t right = INFRARED_ReadRight();
    
    // 十字路口检测（所有传感器都检测到黑线）
    if(left == 1 && middle == 1 && right == 1) 
        return 10;   // 十字路口
    
    // 原有逻辑保持不变
    if(middle == 1 && left == 0 && right == 0) 
        return 0;    // 直线
    if(left == 1 && middle == 0 && right == 0) 
        return -1;   // 偏左
    if(right == 1 && middle == 0 && left == 0) 
        return 1;    // 偏右
    if(left == 1 && middle == 1 && right == 0) 
        return -2;   // 大偏左
    if(right == 1 && middle == 1 && left == 0) 
        return 2;    // 大偏右
    if(left == 0 && middle == 0 && right == 0) 
        return -3;   // 丢失路线
    
    return 0;
}
