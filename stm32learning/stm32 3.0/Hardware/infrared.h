#ifndef __INFRARED_H
#define __INFRARED_H

#include "stm32f10x.h"

void Infrared_Init(void);
void Infrared_Tracking(void);
uint8_t Infrared_IsTrackLost(void);
void Infrared_StopCar(void);

#endif
