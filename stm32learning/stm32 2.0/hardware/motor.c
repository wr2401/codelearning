#include "motor.h"
#include "stm32f10x.h"

Motor_t motor1, motor2;

void Motor_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    // 使能时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    
    // 电机方向控制引脚初始化
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13; // AIN1, AIN2
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14 | GPIO_Pin_15; // BIN1, BIN2
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    // 初始化电机状态
    motor1.target_speed = 0;
    motor1.actual_speed = 0;
    motor1.pwm_output = 0;
    
    motor2.target_speed = 0;
    motor2.actual_speed = 0;
    motor2.pwm_output = 0;
}

void Motor_SetDirection(uint8_t motor_num, MotorDirection_t dir)
{
    if(motor_num == 1) {
        switch(dir) {
            case MOTOR_FORWARD:
                GPIO_SetBits(GPIOB, GPIO_Pin_12);
                GPIO_ResetBits(GPIOB, GPIO_Pin_13);
                break;
            case MOTOR_BACKWARD:
                GPIO_ResetBits(GPIOB, GPIO_Pin_12);
                GPIO_SetBits(GPIOB, GPIO_Pin_13);
                break;
            case MOTOR_STOP:
                GPIO_ResetBits(GPIOB, GPIO_Pin_12);
                GPIO_ResetBits(GPIOB, GPIO_Pin_13);
                break;
        }
    } else {
        switch(dir) {
            case MOTOR_FORWARD:
                GPIO_SetBits(GPIOB, GPIO_Pin_14);
                GPIO_ResetBits(GPIOB, GPIO_Pin_15);
                break;
            case MOTOR_BACKWARD:
                GPIO_ResetBits(GPIOB, GPIO_Pin_14);
                GPIO_SetBits(GPIOB, GPIO_Pin_15);
                break;
            case MOTOR_STOP:
                GPIO_ResetBits(GPIOB, GPIO_Pin_14);
                GPIO_ResetBits(GPIOB, GPIO_Pin_15);
                break;
        }
    }
}
