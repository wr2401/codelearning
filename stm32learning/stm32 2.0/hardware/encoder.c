#include "encoder.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_exti.h"
#include "menu.h"

extern volatile uint8_t menuNeedUpate;
int16_t Encoder_Count;

void Encoder_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    
    // 开启GPIOB时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
    
    // 配置PB0, PB1为上拉输入
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    // 配置PB0、1为外部中断
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource0);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource1);
    EXTI_InitStructure.EXTI_Line = EXTI_Line0 | EXTI_Line1;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  // 下降沿触发
    EXTI_Init(&EXTI_InitStructure);
    
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
    // 配置NVIC
    NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;			
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;	
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;	
	NVIC_Init(&NVIC_InitStructure);				

	NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;		
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;	
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;			
	NVIC_Init(&NVIC_InitStructure);		
    
}

int16_t Encoder_Get(void)
{
	int16_t Temp;
	Temp = Encoder_Count;
	Encoder_Count = 0;
	return Temp;
}

// 编码器中断服务函数
void EXTI0_IRQHandler(void)//逆时针
{
	if (EXTI_GetITStatus(EXTI_Line0) == SET)
	{
		/*如果出现数据乱跳的现象，可再次判断引脚电平，以避免抖动*/
		if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0) == 0)
		{
			if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1) == 0)
			{
				Encoder_Count --;
			}
		}
		EXTI_ClearITPendingBit(EXTI_Line0);
	}
}

void EXTI1_IRQHandler(void)//顺时针
{
	if (EXTI_GetITStatus(EXTI_Line1) == SET)
	{
		/*如果出现数据乱跳的现象，可再次判断引脚电平，以避免抖动*/
		if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1) == 0)
		{
			if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0) == 0)
			{
				Encoder_Count ++;
			}
		}
		EXTI_ClearITPendingBit(EXTI_Line1);
	}
}

void Encoder_UP_Handler(void)
{
    if(isParamSelected) {
        // 参数增加0.1
        switch(currentMenu) {
			case MENU_MAIN:
				break;
            case MENU_LED_CONTROL:
                break;
            case MENU_PID:
                if(cursorPos == 0) pid_kp += 0.1f;
                else if(cursorPos == 1) pid_ki += 0.1f;
                else if(cursorPos == 2) pid_kd += 0.1f;
                break;
			case MENU_IMAGE:
				break;
			case MENU_ANGLE:
				break;
        }
    }
	menuNeedUpate=1;
}

void Encoder_DOWN_Handler(void)
{
    if(isParamSelected) {
        // 参数减少0.1
        switch(currentMenu) {
			case MENU_MAIN:
				break;
            case MENU_LED_CONTROL:
                break;
            case MENU_PID:
                if(cursorPos == 0 ) pid_kp -= 0.1f;
                else if(cursorPos == 1 ) pid_ki -= 0.1f;
                else if(cursorPos == 2 ) pid_kd -= 0.1f;
                break;
			case MENU_IMAGE:
				break;
			case MENU_ANGLE:
				break;
        }
    }
	menuNeedUpate = 1;
}
