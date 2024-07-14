#include "DS18B20.h"

void DS18B20_Init()
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(DS18B20_GPIO_CLK, ENABLE);
    GPIO_InitStructure.GPIO_Pin = DS18B20_DQ;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(DS18B20_GPIO_PORT, &GPIO_InitStructure);
}

u8 DS18B20_Reset()
{
    u8 ack = 0;
    DS18B20_DQ_L;
    delay_us(480);
    DS18B20_DQ_H;
    delay_us(60);
    ack = GPIO_ReadInputDataBit(DS18B20_GPIO_PORT, DS18B20_DQ);
    delay_us(420);
    return ack;
}

void DS18B20_Write_Byte(u8 dat)
{
    u8 i;
    for (i = 0; i < 8; i++)
    {
        DS18B20_DQ_L;
        delay_us(2);
        DS18B20_DQ_BITBAND = dat & 1;
        delay_us(58);
        DS18B20_DQ_H;
        delay_us(1);
        dat >>= 1;
    }
}

u8 DS18B20_Read_Byte()
{
    u8 i = 0, dat = 0;
    for (; i < 8; i++)
    {
        DS18B20_DQ_L;
        delay_us(2);
        dat >>= 1;
        DS18B20_DQ_H;
        delay_us(2);
        if (GPIO_ReadInputDataBit(DS18B20_GPIO_PORT, DS18B20_DQ))
        {
            dat |= 0x80;
        }
        delay_us(56);
    }
    return dat;
}

u8 calcrc_1byte(u8 abyte)
{
    u8 i, crc_1byte;
    crc_1byte = 0; // 设定crc_1byte初值为0
    for (i = 0; i < 8; i++)
    {
        if ((crc_1byte ^ abyte) & 0x01)
        {
            crc_1byte ^= 0x18;
            crc_1byte >>= 1;
            crc_1byte |= 0x80;
        }
        else
            crc_1byte >>= 1;
        abyte >>= 1;
    }
    return crc_1byte;
}

u8 calcrc_bytes(u8 *p, u8 len)
{
    u8 crc = 0;
    while (len--) // len为总共要校验的字节数
    {
        crc = calcrc_1byte(crc ^ *p++);
    }
    return crc;
}

u16 DS18B20_Read_Temp()
{
    u8 i = 0, byte9[9];
    u16 temp = 0;
    DS18B20_Reset();
    DS18B20_Write_Byte(Skip_Rom);
    DS18B20_Write_Byte(Convert_T);
    DS18B20_Reset();
    DS18B20_Write_Byte(Skip_Rom);
    DS18B20_Write_Byte(Read_Scratchpad);
    for (i = 0; i < 9; i++)
    {
        byte9[i] = DS18B20_Read_Byte();
    }
    while (calcrc_bytes(byte9, 9)) // 数据正确CRC应为0
    {
        GPIO_ResetBits(GPIOA, GPIO_Pin_7);
        delay_ms(100);
        GPIO_SetBits(GPIOA, GPIO_Pin_7);
        delay_ms(100);
    }
    temp = byte9[0] | ((byte9[1] | temp) << 8);
    return temp;
}

u16 DS18B20_Read_EEPROM()
{
    u8 i = 0, byte9[9];
    u16 temp = 0;
    DS18B20_Reset();
    DS18B20_Write_Byte(Skip_Rom);
    DS18B20_Write_Byte(Recall_EEPROM);
    DS18B20_Reset();
    DS18B20_Write_Byte(Skip_Rom);
    DS18B20_Write_Byte(Read_Scratchpad);
    for (i = 0; i < 9; i++)
    {
        byte9[i] = DS18B20_Read_Byte();
    }
    while (calcrc_bytes(byte9, 9)) // 数据正确CRC应为0
    {
        GPIO_ResetBits(GPIOA, GPIO_Pin_7);
        delay_ms(100);
        GPIO_SetBits(GPIOA, GPIO_Pin_7);
        delay_ms(100);
    }
    temp = byte9[3] | ((byte9[2] | temp) << 8);
    return temp;
}

void DS18B20_Write_Scratchpad_EEPROM(s8 TH, s8 TL, u8 precision)
{
    DS18B20_Reset();
    DS18B20_Write_Byte(Skip_Rom);
    DS18B20_Write_Byte(Write_Scratchpad);
    DS18B20_Write_Byte(TH);
    DS18B20_Write_Byte(TL);
    DS18B20_Write_Byte(precision);
    DS18B20_Reset();
    DS18B20_Write_Byte(Skip_Rom);
    DS18B20_Write_Byte(Copy_Scratchpad);
}