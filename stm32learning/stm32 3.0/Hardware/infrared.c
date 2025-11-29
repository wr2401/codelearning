#include "stm32f10x.h"                  // Device header
#include "infrared.h"


void INFRARED_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    // 使能GPIOA和GPIOB时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);
    
    // 配置5路传感器为上拉输入模式
    GPIO_InitStructure.GPIO_Pin = IR_1_PIN | IR_2_PIN | IR_3_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = IR_4_PIN | IR_5_PIN;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}

// 5路传感器读取函数
uint8_t INFRARED_Read1(void) { return GPIO_ReadInputDataBit(IR_1_PORT, IR_1_PIN); }
uint8_t INFRARED_Read2(void) { return GPIO_ReadInputDataBit(IR_2_PORT, IR_2_PIN); }
uint8_t INFRARED_Read3(void) { return GPIO_ReadInputDataBit(IR_3_PORT, IR_3_PIN); }
uint8_t INFRARED_Read4(void) { return GPIO_ReadInputDataBit(IR_4_PORT, IR_4_PIN); }
uint8_t INFRARED_Read5(void) { return GPIO_ReadInputDataBit(IR_5_PORT, IR_5_PIN); }

int8_t INFRARED_GetLineError(void)
{
    uint8_t s1 = INFRARED_Read1();  // 最左边
    uint8_t s2 = INFRARED_Read2();
    uint8_t s3 = INFRARED_Read3();  // 中间
    uint8_t s4 = INFRARED_Read4();
    uint8_t s5 = INFRARED_Read5();  // 最右边
    
    // 十字路口检测（所有传感器都检测到黑线）
    if(s1 == 1 && s2 == 1 && s3 == 1 && s4 == 1 && s5 == 1) 
        return 10;   // 十字路口
    else if(s2 == 1 && s3 == 1 && s4 == 1)
		return 10;
    // 5路传感器精确循迹算法
	//else if(s1==1 && s2==0 && s3==0 && s4==1 && s5 ==1) return 6;
	//else if(s1==0 && s2==0 &&s3==0 && s4==1 && s5 ==0) return 6;
	
    else if(s3 == 1) {
        // 中间传感器在黑线上
        if(s2 == 0 && s4 == 0) return 0;       // 00100 - 完美直线
        if(s2 == 1 && s4 == 0) return -1;      // 01100 - 轻微偏左
        if(s2 == 0 && s4 == 1) return 1;       // 00110 - 轻微偏右
        if(s2 == 1 && s4 == 1) return 0;		// 01110 - 直线（较宽黑线）
		if(s2 ==1 && s1 ==1 && s4==0 && s5==0) return -5;
		if(s2 ==0&&s1==0&&s4==1&&s5==1)return 5;
    }
    else {
        // 中间传感器不在黑线上
        if(s2 == 1 && s4 == 0) return -2;      // 01000 - 偏左
        if(s2 == 0 && s4 == 1) return 2;       // 00010 - 偏右
        if(s1 == 1 && s4 == 0) return -3;      // 10000 - 大偏左
        if(s1 == 0 && s5 == 1) return 3;       // 00001 - 大偏右
        if(s1 == 1 && s5 == 1) return 10;      // 10001 - 可能为十字路口
    }
    
    // 丢失路线检测
    if(s1 == 0 && s2 == 0 && s3 == 0 && s4 == 0 && s5 == 0) 
        return -4;   // 00000 - 完全丢失路线
    
    return 0;
}
