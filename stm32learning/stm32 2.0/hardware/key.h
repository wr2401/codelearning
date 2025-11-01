#ifndef __KEY_H
#define __KEY_H

#include "stm32f10x.h"

typedef struct {
    uint8_t currentState;
    uint8_t lastState;
    uint32_t pressTime;
    uint8_t isLongPress;
    uint32_t lastLongPressTime;
} Key_TypeDef;


extern Key_TypeDef upKey, downKey, okKey, backKey;

extern volatile uint32_t now;
  
uint32_t Get_SystemTick(void);
void KEY_Init(void);
void Key_Scan(void);
void Key_Up_Handler(void);
void Key_Up_LongPress_Handler(void);
void Key_OK_Handler(void);
void Key_Back_Handler(void);
void Key_Down_Handler(void);
void Key_Down_LongPress_Handler(void);
void Process_Key(Key_TypeDef *key, void (*shortHandler)(void), void (*longHandler)(void));
void TIM3_IRQHandler(void);


#endif
