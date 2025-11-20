#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Motor.h"
#include "Key.h"
#include "menu.h"
#include "timer.h"
#include "PWM.h"
#include "encoder.h"
#include "pid.h"
#include "serial.h"

SystemMode_t system_mode = MODE_SPEED_CONTROL;
uint32_t system_tick = 0;
//int8_t Speed=40;

int main(void)
{
	SystemInit();
	OLED_Init();
	Motor_Init();
	Key_Init();
	Encoder_Init();
	Timer_Init();
	UART_Init(9600);
	
	PID_Init(&motor1, 1.0f, 0.50f, 0.10f); //KP增大，因为PWM范围变小
    PID_Init(&motor2, 1.0f, 0.50f, 0.10f);
	Menu_UpdateDisplay();
	
	//OLED_ShowString(1, 1, "Speed:");
	
	while (1)
	{
		UART_ProcessData();
		
		//Motor1_SetSpeed(Speed);
		//Motor2_SetSpeed(Speed);
		//OLED_ShowSignedNum(1, 7, Speed, 3);
		Menu_Update();
	}
}
