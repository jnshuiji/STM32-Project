/*** 
 * @Author: QuanSi 2810334190@qq.com
 * @Date: 2024-07-13 10:31:47
 * @LastEditors: QuanSi 2810334190@qq.com
 * @LastEditTime: 2024-07-13 13:20:03
 * @Description: 
 * @Copyright (c) 2024 by QuanSi 2810334190@qq.com, All Rights Reserved. 
 */
#ifndef USART_H
#define USART_H

#include <stm32f10x.h>

void USART_init(USART_TypeDef *USARTx, u32 BaudRate, u16 WordLength, u16 StopBits, u16 Parity, u16 Mode, u16 HardwareFlowControl, FunctionalState NewState);
void USART_IT_Init(USART_TypeDef *USARTx, u8 PreemptionPriority, u8 SubPriority, FunctionalState NewState);

#endif // !USART_H
