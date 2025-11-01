#include "key.h"
#include "timer.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "menu.h"
#include "stdio.h"

extern volatile uint8_t menuNeedUpate;
volatile uint32_t now = 0;
volatile uint32_t systemTick = 0;
// 按键结构体
Key_TypeDef upKey, downKey, okKey, backKey;

void KEY_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    // 开启GPIOA和GPIOC时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC, ENABLE);
    
    // 配置上键(PA4)、下键(PA2)、确认键(PA0)
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_2 | GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;  // 上拉输入
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    // 配置返回键(PC14)
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    
    // 初始化按键状态
    upKey.lastState = 1;
    downKey.lastState = 1;
    okKey.lastState = 1;
    backKey.lastState = 1;
}


//确认键处理函数

void Key_OK_Handler(void)
{
	
    if(!isParamSelected) {
        // 进入子菜单或选中参数
        switch(currentMenu) {
            case MENU_MAIN:
                 switch(cursorPos) {
                    case 0: currentMenu = MENU_LED_CONTROL; break;
                    case 1: currentMenu = MENU_PID; break;
                    case 2: currentMenu = MENU_IMAGE; break;
                    case 3: currentMenu = MENU_ANGLE; break;
                    default: currentMenu = MENU_MAIN; break;
				 }
				 cursorPos = 0;  // 进入子菜单重置光标
                break;
            case MENU_LED_CONTROL:
                if(cursorPos < 2) {
                    isParamSelected = 1;
                }
                break;
            case MENU_PID:
                if(cursorPos < 3) {
                    isParamSelected = 1;
                }
                break;
            case MENU_IMAGE:
				if(cursorPos==0) {
                    isParamSelected = 1;
                }
				break;
            case MENU_ANGLE:
				if(cursorPos ==0) {
                    isParamSelected = 1;
                }
                break;
        }
    } else {
        // 取消参数选中
        isParamSelected = 0;
    }
    
}

//返回键处理函数
void Key_Back_Handler(void)
{
	
    if(isParamSelected) {
        isParamSelected = 0;
    } else {
        currentMenu = MENU_MAIN;
        cursorPos = 0;
    }
}

//上键处理函数

void Key_Up_Handler(void)
{
	
    if(!isParamSelected) {
        // 菜单光标上移
        switch(currentMenu) {
            case MENU_MAIN:
                cursorPos = (cursorPos == 0) ? 3 : (cursorPos - 1);
                break;
            case MENU_LED_CONTROL:
				cursorPos = (cursorPos == 0) ? 1 : (cursorPos - 1);
                break;
            case MENU_PID:
				cursorPos = (cursorPos == 0) ? 2 : (cursorPos - 1);
                break;
            case MENU_IMAGE:
				cursorPos=0;
				break;
            case MENU_ANGLE:
				cursorPos=0;
                break;
        }
        menuNeedUpate = 1;
    } else {
        // 参数编辑模式下：参数增加
        switch(currentMenu) {
			case MENU_MAIN:
				break;
            case MENU_LED_CONTROL:
                if(cursorPos == 0) {
                    ledSpeed=(ledSpeed+1)%3;
					LED_SetFlowSpeed(ledSpeed);
                } else if(cursorPos == 1) {
                    ledDirection = 1-ledDirection;
                }
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
        menuNeedUpate = 1;
    }
}

//下键处理函数

void Key_Down_Handler(void)
{
	
    if(!isParamSelected) {
        // 菜单光标下移
        switch(currentMenu) {
            case MENU_MAIN:
                cursorPos = (cursorPos == 3) ? 0 : (cursorPos + 1);
                break;
            case MENU_LED_CONTROL:
				cursorPos = (cursorPos == 1) ? 0 : (cursorPos + 1);
                break;
            case MENU_PID:
				cursorPos = (cursorPos == 2) ? 0 : (cursorPos + 1);
                break;
            case MENU_IMAGE:
				break;
            case MENU_ANGLE:
                break;
        }
        menuNeedUpate = 1;
    } else {
        // 参数编辑模式下：参数减少
        switch(currentMenu) {
			case MENU_MAIN:
				break;
            case MENU_LED_CONTROL:
                if(cursorPos == 0) {
                    ledSpeed=(ledSpeed-1)%3;
                    LED_SetFlowSpeed(ledSpeed);
                } else if(cursorPos == 1) {
                    ledDirection = 1-ledDirection;
                }
                break;
            case MENU_PID:
				if(cursorPos == 0) pid_kp -= 0.1f;
                else if(cursorPos == 1) pid_ki -= 0.1f;
                else if(cursorPos == 2) pid_kd -= 0.1f;
                break;
			case MENU_IMAGE:
				break;
            case MENU_ANGLE:
				break;
        }
        menuNeedUpate = 1;
    }
    menuNeedUpate = 1;
}

//上键长按处理函数

void Key_Up_LongPress_Handler(void)
{
    if(isParamSelected) {
        // 长按连续增加参数
        Key_Up_Handler();
        menuNeedUpate = 1;
    }
    
}

//下键长按处理函数

void Key_Down_LongPress_Handler(void)
{
    if(isParamSelected) {
        // 长按连续减少参数
        Key_Down_Handler();
        menuNeedUpate = 1;
    }

}

//处理按键状态机

void Process_Key(Key_TypeDef *key, void (*shortHandler)(void), void (*longHandler)(void))
{
    now = Get_SystemTick();

    // 按下
    if(key->lastState && !key->currentState) {
        key->pressTime = now;
        key->isLongPress = 0;
        key->lastLongPressTime = now;
    }

    // 持续按下
    if(!key->currentState) {
        // 第一次触发长按
        if(!key->isLongPress && (now - key->pressTime >= 1000)) {
            key->isLongPress = 1;
            key->lastLongPressTime = now;
            longHandler();
             menuNeedUpate = 1;
        }

        // 连续长按，每100ms触发一次
        if(key->isLongPress && (now - key->lastLongPressTime >= 100)) {
            key->lastLongPressTime = now;
            longHandler();
            menuNeedUpate = 1;
        }
    }

    // 松开
    if(!key->lastState && key->currentState) {
        if(!key->isLongPress && (now - key->pressTime < 1000)) {
            shortHandler(); // 短按
        }
        key->isLongPress = 0;
    }

    key->lastState = key->currentState;
    
}


//按键扫描函数

void Key_Scan(void)
{
    // 读取当前按键状态
    upKey.currentState = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_4);
    downKey.currentState = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_2);
    okKey.currentState = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0);
    backKey.currentState = GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_14);
    
    // 上键处理
    Process_Key(&upKey, Key_Up_Handler, Key_Up_LongPress_Handler);

    
    // 下键处理  
    Process_Key(&downKey, Key_Down_Handler, Key_Down_LongPress_Handler);

    
    // 确认键处理
    if(okKey.lastState && !okKey.currentState) { // 按下
        Key_OK_Handler();
        menuNeedUpate = 1;
    }
    okKey.lastState = okKey.currentState;
    
    // 返回键处理
    if(backKey.lastState && !backKey.currentState) { // 按下
        Key_Back_Handler();
        menuNeedUpate = 1;
    }
    backKey.lastState = backKey.currentState;
}

//TIM3中断函数 - 按键扫描

void TIM3_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) {
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
        systemTick++;
        Key_Scan();
     
    }
}

uint32_t Get_SystemTick(void)
{
  return systemTick;  
}
