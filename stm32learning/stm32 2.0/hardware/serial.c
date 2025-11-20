#include "stm32f10x.h"
#include "motor.h"
#include "stdio.h"
#include "string.h"
//未用数据包，不过我感觉这个应该没事
uint8_t uart_rx_buffer[32];//数据缓冲区
uint8_t uart_rx_index = 0;//缓冲区写入位置
uint8_t uart_data_ready = 0;//数据接收完成标志

void UART_Init(uint32_t baudrate)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    
    // 使能时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1, ENABLE);
    
    // TX - PA9, RX - PA10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    // 串口配置
    USART_InitStructure.USART_BaudRate = baudrate;//波特率
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长
    USART_InitStructure.USART_StopBits = USART_StopBits_1;//停止位
    USART_InitStructure.USART_Parity = USART_Parity_No;//校验位
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//硬件流
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART1, &USART_InitStructure);
    
    // 使能接收中断
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
    USART_Cmd(USART1, ENABLE);
    
    // NVIC配置
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

void UART_SendData(void)
{
    char buffer[64];
    
    // 发送到VOFA+的格式: 电机1实际速度,电机1目标速度,电机2实际速度
    sprintf(buffer, "%.2f,%.2f,%.2f\r\n", 
            (float)motor1.actual_speed, 
            (float)motor1.target_speed,
            (float)motor2.actual_speed);
    
    for(int i = 0; i < strlen(buffer); i++) {
        while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);//等待标志位置1
        USART_SendData(USART1, buffer[i]);
    }
}

void UART_ProcessData(void)
{
    if(uart_data_ready) {
        uart_data_ready = 0;
        
        // 解析速度设置命令: "@speed%200"
        if(strstr((char*)uart_rx_buffer, "@speed%") != NULL) {
            int speed;
			//scanf从键盘输入读取，无法进入，%%%中前两个%指配置一个%，对应%200中的%
            if(sscanf((char*)uart_rx_buffer, "@speed%%%d", &speed) == 1) {
                motor1.target_speed = speed;
                motor2.target_speed = speed;
            }
        }
        
        uart_rx_index = 0;
		for(int i = 0; i < sizeof(uart_rx_buffer); i++) {
			uart_rx_buffer[i] = 0;
		}
    }
}

// 串口中断服务函数 
void USART1_IRQHandler(void)
{
    if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) {
        uint8_t data = USART_ReceiveData(USART1);
        
        if(data == '@') {
			//开始新命令，重置索引
			uart_rx_index = 0;
            uart_rx_buffer[uart_rx_index++] = data;
        } else if(data == '\r' || data == '\n') {
            //命令结束，设置就绪标志
			uart_rx_buffer[uart_rx_index] = '\0';
            uart_data_ready = 1;
        } else if(uart_rx_index < sizeof(uart_rx_buffer)-1) {
            //存储数据
			uart_rx_buffer[uart_rx_index++] = data;
        }
        
        USART_ClearITPendingBit(USART1, USART_IT_RXNE);
    }
}
