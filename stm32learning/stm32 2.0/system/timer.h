#ifndef __TIMER_H
#define __TIMER_H

#include "stm32f10x.h"

typedef enum {
    MODE_SPEED_CONTROL = 0,
    MODE_FOLLOW_CONTROL
} SystemMode_t;

extern SystemMode_t system_mode;
extern uint32_t system_tick;

void Timer_Init(void);

#endif
