#ifndef __DS18B20_H__
#define __DS18B20_H__

#include "delay.h"

// 位带操作
#define BITBAND_REG(REG, BIT) (*((u32 volatile *)(0x42000000u + (((u32) & (REG) - (u32)0x40000000u) << 5) + (((u32)(BIT)) << 2))))

#define DS18B20_GPIO_PORT GPIOB
#define DS18B20_GPIO_CLK RCC_APB2Periph_GPIOB
#define DS18B20_DQ GPIO_Pin_0
#define DS18B20_DQ_BITBAND BITBAND_REG(DS18B20_GPIO_PORT->ODR, 0)

#define DS18B20_DQ_H GPIO_SetBits(DS18B20_GPIO_PORT, DS18B20_DQ)
#define DS18B20_DQ_L GPIO_ResetBits(DS18B20_GPIO_PORT, DS18B20_DQ)

/*-------------------------------------------ROM命令-------------------------------------------*/
#define Search_Rom 0xF0   // 搜索ROM
#define Read_Rom 0x33     // 读取ROM
#define Match_Rom 0x55    // 匹配ROM
#define Skip_Rom 0xCC     // 跳过ROM
#define Alarm_Search 0xEC // 警报搜索

/*-------------------------------------------功能命令-------------------------------------------*/
#define Convert_T 0x44         // 转换温度
#define Read_Scratchpad 0xBE   // 读取暂存寄存器:读取暂存寄存器中全部的数据，包括 CRC 字节。
#define Write_Scratchpad 0x4E  // 写入暂存寄存器:向暂存寄存器中的TH，TL 及配置寄存器（精度）写入数据
#define Copy_Scratchpad 0x48   // 拷贝暂存寄存器:将暂存寄存器中的TH，TL及配置寄存器中的数据拷贝到EEPROM中
#define Recall_EEPROM 0xB8     // 召回EEPROM:将EEPROM中的TH，TL及配置寄存器数据召回到暂存寄存器中
#define Read_Power_Supply 0xB4 // 读取供电模式:读取 DS18B20 的供电模式到控制器中

/*-------------------------------------------转换精度-------------------------------------------*/
#define precision_12 0x7F // 转换位数12，最大转换时间t=750ms
#define precision_11 0x5F // 转换位数11，最大转换时间t/2=375ms
#define precision_10 0x3F // 转换位数10，最大转换时间t/4=187.5ms
#define precision_9 0x1F  // 转换位数9，最大转换时间t/8=93.75ms

void DS18B20_Init();
u8 DS18B20_Reset();
void DS18B20_Write_Byte(u8 dat);
u8 readbyteDS18B20();
u8 calcrc_1byte(u8 dat);
u8 calcrc_bytes(u8 *addr, u8 len);
u16 DS18B20_Read_Temp();
u16 DS18B20_Read_EEPROM();
void DS18B20_Write_Scratchpad_EEPROM(s8 TH, s8 TL, u8 precision);

#endif // __DS18B20_H__