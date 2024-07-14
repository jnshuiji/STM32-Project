#include "TM1637.h"

void TM1637_Init()
{
    /*------------------------------------------------------时钟配置------------------------------------------------------*/
    RCC_APB2PeriphClockCmd(TM1637_GPIO_CLK, ENABLE);

    /*------------------------------------------------------引脚配置------------------------------------------------------*/
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = TM1637_SCL | TM1637_SDA;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(TM1637_GPIO_PORT, &GPIO_InitStructure);
}

void TM1637_Start()
{
    TM1637_SDA_H;
    TM1637_SCL_H;
    delay_us(1);
    TM1637_SDA_L;
    TM1637_SCL_L;
}

void TM1637_Stop()
{
    TM1637_SDA_L;
    TM1637_SCL_H;
    delay_us(1);
    TM1637_SDA_H;
}

u8 TM1637_Ack()
{
    u8 ack;
    TM1637_SDA_H;
    TM1637_SCL_H;
    delay_us(1);
    ack = GPIO_ReadInputDataBit(TM1637_GPIO_PORT, TM1637_SDA);
    TM1637_SCL_L;
    return ack;
}

void TM1637_Write_Byte(u8 dat)
{
    u8 i;
    for (i = 0; i < 8; i++)
    {
        TM1637_SDA_BITBAND = dat & 1;
        TM1637_SCL_H;
        delay_us(1);
        TM1637_SCL_L;
        dat >>= 1;
    }
}

void TM1637_Setluminance(u8 luminance)
{
    u8 temp = 0;
    if (luminance)
        temp = (luminance - 1) | 0x88;
    else
        temp = 0x80;

    TM1637_Start();
    TM1637_Write_Byte(temp);
    TM1637_Ack();
    TM1637_Stop();
}

u8 Key_TM1637()
{
    u8 Key = 0, i = 0;
    TM1637_Start();
    TM1637_Write_Byte(TM1637_ReadKey);
    TM1637_Ack();
    TM1637_SDA_H;
    for (; i < 8; i++)
    {
        delay_us(1);
        Key >>= 1;
        TM1637_SCL_H;
        if (GPIO_ReadInputDataBit(TM1637_GPIO_PORT, TM1637_SDA) == 1)
            Key |= 0x80;
        delay_us(1);
        TM1637_SCL_L;
    }
    TM1637_Ack();
    TM1637_Stop();
    return Key;
}

void TM1637_Display(u8 grid, u8 sg)
{
    TM1637_Start();
    TM1637_Write_Byte(TM1637_FixedAddress_Mode);
    TM1637_Ack();
    TM1637_Stop();

    TM1637_Start();
    TM1637_Write_Byte((grid - 1) | 0xC0);
    TM1637_Ack();
    TM1637_Write_Byte(sg);
    TM1637_Ack();
    TM1637_Stop();
}