#include "infrared.h"
#include "Motor.h"
#include "stm32f10x.h"

// 外部变量声明
extern int32_t target_speed;
extern PID_TypeDef pid_motor1;
extern PID_TypeDef pid_motor2;

// 轨迹丢失检测变量
static uint32_t track_lost_count = 0;
static uint32_t search_timer = 0;
static uint8_t is_track_lost = 0;

void Infrared_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    
    track_lost_count = 0;
    search_timer = 0;
    is_track_lost = 0;
}

uint8_t Infrared_IsTrackLost(void)
{
    uint8_t left = GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_0);
    uint8_t center = GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_1);
    uint8_t right = GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_2);
    
    if(left == 1 && center == 1 && right == 1)
    {
        return 1;
    }
    return 0;
}

void Infrared_StopCar(void)
{
    Motor_SetSpeed(0, 0);
    is_track_lost = 1;
}

void Infrared_Tracking(void)
{
    uint8_t left = GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_0);
    uint8_t center = GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_1);
    uint8_t right = GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_2);
    
    int16_t base_speed = target_speed;
    int16_t speed1 = base_speed;
    int16_t speed2 = base_speed;
    
    if(Infrared_IsTrackLost())
    {
        track_lost_count++;
        search_timer++;
        
        if(track_lost_count >= 50)
        {
            Infrared_StopCar();
            return;
        }
        
        if(search_timer % 20 < 10)
        {
            speed1 = base_speed / 3 - 5;
            speed2 = base_speed / 3 + 5;
        }
        else
        {
            speed1 = base_speed / 3 + 5;
            speed2 = base_speed / 3 - 5;
        }
        
        if(search_timer >= 100)
        {
            Infrared_StopCar();
            return;
        }
    }
    else
    {
        track_lost_count = 0;
        search_timer = 0;
        is_track_lost = 0;
        
        if(center == 0)
        {
            speed1 = base_speed;
            speed2 = base_speed;
        }
        else if(left == 0 && center == 1 && right == 1)
        {
            speed1 = base_speed - 15;
            speed2 = base_speed + 15;
        }
        else if(left == 1 && center == 1 && right == 0)
        {
            speed1 = base_speed + 15;
            speed2 = base_speed - 15;
        }
        else if(left == 0 && center == 0 && right == 1)
        {
            speed1 = base_speed - 25;
            speed2 = base_speed + 25;
        }
        else if(left == 1 && center == 0 && right == 0)
        {
            speed1 = base_speed + 25;
            speed2 = base_speed - 25;
        }
        else if(left == 0 && center == 0 && right == 0)
        {
            speed1 = base_speed;
            speed2 = base_speed;
        }
    }
    
    if(is_track_lost)
    {
        speed1 = 0;
        speed2 = 0;
    }
    
    if(speed1 > 100) speed1 = 100;
    if(speed2 > 100) speed2 = 100;
    if(speed1 < -100) speed1 = -100;
    if(speed2 < -100) speed2 = -100;
    
    pid_motor1.target = speed1;
    pid_motor2.target = speed2;
}
