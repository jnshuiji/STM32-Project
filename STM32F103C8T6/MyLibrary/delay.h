/*** 
 * @Author: QuanSi 2810334190@qq.com
 * @Date: 2024-07-10 17:41:35
 * @LastEditors: QuanSi 2810334190@qq.com
 * @LastEditTime: 2024-07-13 13:22:13
 * @Description: 
 * @Copyright (c) 2024 by QuanSi 2810334190@qq.com, All Rights Reserved. 
 */
#ifndef delay_h
#define delay_h

#include <stm32f10x.h>

void delay_start(void);
void delay_us(u32 us);
void delay_ms(u16 ms);

#endif