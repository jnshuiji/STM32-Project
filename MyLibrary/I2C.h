#ifndef I2C_H
#define I2C_H

#include <stm32f10x.h>

void I2C_init(void);
void I2C_Write_Byte(I2C_TypeDef *I2Cx, u8 address, u8 dat);
void I2C_Write_Bytes(I2C_TypeDef *I2Cx, u8 address, u8 *dat, u32 len);
u8 I2C_Read_Byte(I2C_TypeDef *I2Cx, u8 address);
void I2C_Read_Bytes(I2C_TypeDef *I2Cx, u8 address, u8 *dat, u8 len);

#endif // I2C_H