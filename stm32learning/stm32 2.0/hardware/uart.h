#ifndef __UART_H
#define __UART_H

#include "stm32f10x.h"

extern uint8_t uart_rx_buffer[32];
extern uint8_t uart_rx_index;
extern uint8_t uart_data_ready;

void UART_Init(uint32_t baudrate);
void UART_SendData(void);
void UART_ProcessData(void);

#endif
