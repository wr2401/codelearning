#include "serial.h"
#include "motor.h"
#include "pid.h"

// 外部变量声明（只声明，不定义）
extern int32_t target_speed;
extern Motor_t motor1, motor2;
extern PID_TypeDef pid_motor1, pid_motor2;

void Serial_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    USART_InitStructure.USART_BaudRate = 9600;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
    
    USART_Init(USART1, &USART_InitStructure);
    USART_Cmd(USART1, ENABLE);
}

void Serial_SendData(void)
{
    static uint32_t send_count = 0;
    send_count++;
    
    if(send_count >= 100)
    {
        send_count = 0;
        
        // 发送数据包
        USART_SendData(USART1, 0xAA);
        while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
        
        USART_SendData(USART1, (uint8_t)target_speed);           // 基础速度
        while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
        
        USART_SendData(USART1, (uint8_t)pid_motor1.target);      // 左轮目标速度
        while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
        
        USART_SendData(USART1, (uint8_t)pid_motor2.target);      // 右轮目标速度
        while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
        
        USART_SendData(USART1, (uint8_t)motor1.actual_speed);    // 左轮实际速度
        while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
        
        USART_SendData(USART1, (uint8_t)motor2.actual_speed);    // 右轮实际速度
        while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
        
        USART_SendData(USART1, 0x55);
        while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
    }
}
