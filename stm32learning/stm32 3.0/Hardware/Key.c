#include "stm32f10x.h"                  // Device header
#include "key.h"
#include "delay.h"

void KEY_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    
    GPIO_InitStructure.GPIO_Pin = KEY_UP_PIN | KEY_DOWN_PIN | KEY_ENTER_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}

Key_Type KEY_Scan(void)
{
    static uint8_t key_up = 1;
    
    if(key_up && (GPIO_ReadInputDataBit(KEY_UP_PORT, KEY_UP_PIN) == 0 ||
                  GPIO_ReadInputDataBit(KEY_DOWN_PORT, KEY_DOWN_PIN) == 0 ||
                  GPIO_ReadInputDataBit(KEY_ENTER_PORT, KEY_ENTER_PIN) == 0))
    {
        Delay_ms(10);
        key_up = 0;
        if(GPIO_ReadInputDataBit(KEY_UP_PORT, KEY_UP_PIN) == 0)
            return KEY_UP;
        else if(GPIO_ReadInputDataBit(KEY_DOWN_PORT, KEY_DOWN_PIN) == 0)
            return KEY_DOWN;
        else if(GPIO_ReadInputDataBit(KEY_ENTER_PORT, KEY_ENTER_PIN) == 0)
            return KEY_ENTER;
    }
    else if(GPIO_ReadInputDataBit(KEY_UP_PORT, KEY_UP_PIN) == 1 &&
            GPIO_ReadInputDataBit(KEY_DOWN_PORT, KEY_DOWN_PIN) == 1 &&
            GPIO_ReadInputDataBit(KEY_ENTER_PORT, KEY_ENTER_PIN) == 1)
    {
        key_up = 1;
    }
    
    return KEY_NONE;
}
