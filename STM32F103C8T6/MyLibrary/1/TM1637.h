/*** 
 * @Author: QuanSi 2810334190@qq.com
 * @Date: 2024-07-11 21:33:22
 * @LastEditors: QuanSi 2810334190@qq.com
 * @LastEditTime: 2024-07-13 16:16:53
 * @Description: 
 * @Copyright (c) 2024 by QuanSi 2810334190@qq.com, All Rights Reserved. 
 */
#ifndef TM1637_H
#define TM1637_H

#include "delay.h"

// 位带操作
#define BITBAND_REG(REG, BIT) (*((u32 volatile *)(0x42000000u + (((u32) & (REG) - (u32)0x40000000u) << 5) + (((u32)(BIT)) << 2))))

/*---------------------------------------------------------I2C---------------------------------------------------------*/
//可根据实际连接的引脚进行修改
#define TM1637_GPIO_PORT GPIOB
#define TM1637_GPIO_CLK RCC_APB2Periph_GPIOB
#define TM1637_SCL GPIO_Pin_6
#define TM1637_SDA GPIO_Pin_7
#define TM1637_SDA_BITBAND BITBAND_REG(TM1637_GPIO_PORT->ODR, 7)

#define TM1637_SCL_H GPIO_SetBits(TM1637_GPIO_PORT, TM1637_SCL)
#define TM1637_SCL_L GPIO_ResetBits(TM1637_GPIO_PORT, TM1637_SCL)
#define TM1637_SDA_H GPIO_SetBits(TM1637_GPIO_PORT, TM1637_SDA)
#define TM1637_SDA_L GPIO_ResetBits(TM1637_GPIO_PORT, TM1637_SDA)

/*---------------------------------------------------------按键---------------------------------------------------------*/
#define NO_KEY_PRESSED 0xFF

#define K1_1 0xF7
#define K1_2 0xF6
#define K1_3 0xF5
#define K1_4 0xF4
#define K1_5 0xF3
#define K1_6 0xF2
#define K1_7 0xF1
#define K1_8 0xF0

#define K2_1 0xEF
#define K2_2 0xEE
#define K2_3 0xED
#define K2_4 0xEC
#define K2_5 0xEB
#define K2_6 0xEA
#define K2_7 0xE9
#define K2_8 0xE8

/*---------------------------------------------------------命令---------------------------------------------------------*/
#define TM1637_ReadKey 0x42           // 读键扫数据
#define TM1637_FixedAddress_Mode 0x44 // 固定地址模式

/*---------------------------------------------------------函数---------------------------------------------------------*/
void TM1637_Init();
void TM1637_Start();
void TM1637_Stop();
u8 TM1637_Ack();
void TM1637_Write_Byte(u8 dat);
void TM1637_Setluminance(u8 luminance);
u8 Key_TM1637();
void Display_TM1637(u8 grid, u8 sg);

#endif // TM1637_H