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
    
    if(middle && !left && !right) return 0;   // 直线
    if(left && !middle && !right) return -1;  // 偏左
    if(right && !middle && !left) return 1;   // 偏右
    if(left && middle && !right) return -2;   // 大偏左
    if(right && middle && !left) return 2;    // 大偏右
    if(!left && !middle && !right) return -3; // 丢失路线
    
    return 0;
}
