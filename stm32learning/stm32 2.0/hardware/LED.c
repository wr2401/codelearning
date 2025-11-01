#include "led.h"
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"

uint8_t ledDirection = 1;  // 0:左, 1:右
uint8_t currentLED = 0;

void LED_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    // 开启GPIOB时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    
    // 配置LED引脚为推挽输出
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    // 初始关闭所有LED
    GPIO_SetBits(GPIOB, GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);
}

uint16_t ledPins[] = {GPIO_Pin_12, GPIO_Pin_13, GPIO_Pin_14, GPIO_Pin_15};

void LED_Flow_Control(void)
{
    static int current = 0;
    
    // 关闭所有LED
    GPIO_SetBits(GPIOB, GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);
    
    // 更新索引
    if (ledDirection) {
        current = (current + 1) % 4;  // 向右：0→1→2→3→0
    } else {
        current = (current + 3) % 4;  // 向左：0→3→2→1→0
    }
    
    // 点亮当前LED
    GPIO_ResetBits(GPIOB, ledPins[current]);
}

void TIM2_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) {
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
        
        // 流水灯控制
        LED_Flow_Control();
    }
}
