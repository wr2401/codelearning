#include "stm32f10x.h"
#include "led.h"
#include "key.h"
#include "timer.h"
#include "menu.h"
#include "encoder.h"
#include "OLED.h"
#include "Delay.h"
#include "stdio.h"

extern volatile uint8_t menuNeedUpate;
extern volatile uint32_t now;

int main(void)
{
    // 系统初始化
    SystemInit();
    
    // 外设初始化
    Menu_Init();
    LED_Init();
    KEY_Init();
    Encoder_Init();
    TIM2_Init();  // 流水灯定时器
    TIM3_Init();  // 按键扫描定时器
  
    OLED_Init();
    OLED_Display_Menu();
        
    while (1) {
    int16_t encVal = Encoder_Get();
    if(currentMenu == MENU_PID && isParamSelected == 1)
    {
      if(encVal > 0) Encoder_UP_Handler();
      else if(encVal < 0) Encoder_DOWN_Handler(); 
    }

    if(menuNeedUpate)
    {
      OLED_Display_Menu();
      menuNeedUpate = 0;
    }
    Delay_ms(100);
  }
}
