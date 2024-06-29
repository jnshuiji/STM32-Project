/*
 * @Author: QuanSi 2810334190@qq.com
 * @Date: 2024-06-27 21:18:51
 * @LastEditors: QuanSi 2810334190@qq.com
 * @LastEditTime: 2024-06-29 12:04:23
 * @FilePath: \1.LED\User\main.c
 * @Description:
 *
 * Copyright (c) 2024 by 犬饲, All Rights Reserved.
 */
#include <stm32f10x.h>

/**
 * @description: LED初始化配置
 * @return {*}
 */
void LED_config(void)
{
    GPIO_InitTypeDef led_gpio;
    // 使能端口A的时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    // LED I/O口配置
    led_gpio.GPIO_Pin = GPIO_Pin_8;
    led_gpio.GPIO_Mode = GPIO_Mode_Out_PP; // 推挽输出
    led_gpio.GPIO_Speed = GPIO_Speed_2MHz; // 2M时钟速度
    GPIO_Init(GPIOA, &led_gpio);
    // 默认关闭LED
    GPIO_SetBits(GPIOA, GPIO_Pin_8);
}

/*控制LED1~LED4(直接操作寄存器)*/
// PA0输出相反状态
#define LED1_TOGGLE GPIOA->ODR ^= GPIO_Pin_0
// PA0输出高电平 LED1 灭
#define LED1_OFF GPIOA->BSRR = GPIO_Pin_0
// PA0输出低电平 LED1 亮
#define LED1_ON GPIOA->BRR = GPIO_Pin_0
#define LED2_TOGGLE GPIOA->ODR ^= GPIO_Pin_1
#define LED2_OFF GPIOA->BSRR = GPIO_Pin_1
#define LED2_ON GPIOA->BRR = GPIO_Pin_1
#define LED3_TOGGLE GPIOA->ODR ^= GPIO_Pin_2
#define LED3_OFF GPIOA->BSRR = GPIO_Pin_2
#define LED3_ON GPIOA->BRR = GPIO_Pin_2
#define LED4_TOGGLE GPIOA->ODR ^= GPIO_Pin_3
#define LED4_OFF GPIOA->BSRR = GPIO_Pin_3
#define LED4_ON GPIOA->BRR = GPIO_Pin_3
/*控制LED5~LED8(调用库函数)*/
// PA5输出高电平 LED5 灭
#define LED5_OFF GPIO_ResetBits(GPIOA, GPIO_Pin_4)
// PA5输出低电平 LED5 亮
#define LED5_ON GPIO_SetBits(GPIOA, GPIO_Pin_4)
#define LED6_OFF GPIO_ResetBits(GPIOA, GPIO_Pin_5)
#define LED6_ON GPIO_SetBits(GPIOA, GPIO_Pin_5)
#define LED7_OFF GPIO_ResetBits(GPIOA, GPIO_Pin_6)
#define LED7_ON GPIO_SetBits(GPIOA, GPIO_Pin_6)
#define LED8_OFF GPIO_ResetBits(GPIOA, GPIO_Pin_7)
#define LED8_ON GPIO_SetBits(GPIOA, GPIO_Pin_7)

int main(void)
{
    LED_config();
    while (1)
    {
        // 全部低电平
        LED1_ON;
        LED2_ON;
        LED3_ON;
        LED4_ON;
        LED5_ON;
        LED6_ON;
        LED7_ON;
        LED8_ON;
    }
}
