/*** 
 * @Author: QuanSi 2810334190@qq.com
 * @Date: 2024-07-14 14:36:08
 * @LastEditors: QuanSi 2810334190@qq.com
 * @LastEditTime: 2024-07-14 20:34:49
 * @Description: 
 * @Copyright (c) 2024 by QuanSi 2810334190@qq.com, All Rights Reserved. 
 */
#ifndef I2C_H
#define I2C_H

#include <stm32f10x.h>

#define OwnAddress1 0x30

#define I2C_TIMEROUT 65535

#define I2C_BUSY 1
#define I2C_START_ERROR 2
#define I2C_Slave_Address_Transmission_ERROR 3
#define I2C_TRANSMITTER_ERROR 4
#define I2C_RECEIVER_ERROR 5

void I2C_My_Init(I2C_TypeDef *I2Cx);
void I2C_My_IT_Init(I2C_TypeDef *I2Cx);
u8 I2C_START(I2C_TypeDef *I2Cx, u8 address, u8 direction);
u8 I2C_Write_Byte(I2C_TypeDef *I2Cx, u8 address, u8 dat);
u8 I2C_Write_2_Bytes(I2C_TypeDef *I2Cx, u8 address, u8 dat1, u8 dat2);
u8 I2C_Write_Bytes(I2C_TypeDef *I2Cx, u8 address, u8 *dat, u32 len);
u8 I2C_Read_Byte(I2C_TypeDef *I2Cx, u8 address);
u8 I2C_Read_Bytes(I2C_TypeDef *I2Cx, u8 address, u8 *dat, u8 len);

#endif // I2C_H