#include "OLED.h"
#include "codetab.h"

void OLED_Write_CMD(u8 cmd)
{
    I2C_Write_2_Bytes(I2C1, OLED_ADDRESS, OLED_CMD, cmd);
}

void OLED_Write_DAT(u8 dat)
{
    I2C_Write_2_Bytes(I2C1, OLED_ADDRESS, OLED_DAT, dat);
}

void OLED_Init()
{
    OLED_Write_CMD(0xAE); // 关闭显示
    OLED_Write_CMD(0x20); // 设置内存地址模式
    OLED_Write_CMD(0x10); // 垂直寻址模式
    OLED_Write_CMD(0xb0); // 设置页地址
    OLED_Write_CMD(0xc8); // 设置COM扫描方向
    OLED_Write_CMD(0x00); // 设置低列地址
    OLED_Write_CMD(0x10); // 设置高列地址
    OLED_Write_CMD(0x40); // 设置起始行
    OLED_Write_CMD(0x81); // 设置对比度
    OLED_Write_CMD(0xff); // 对比度值
    OLED_Write_CMD(0xa1); // 设置段重映射
    OLED_Write_CMD(0xa6); // 设置正显
    OLED_Write_CMD(0xa8); // 设置复用比
    OLED_Write_CMD(0x3f); // 复用比
    OLED_Write_CMD(0xa4); // 全局显示开启
    OLED_Write_CMD(0xd3); // 设置显示偏移
    OLED_Write_CMD(0x00); // 偏移量
    OLED_Write_CMD(0xd5); // 设置显示时钟分频
    OLED_Write_CMD(0xf0); // 分频
    OLED_Write_CMD(0xd9); // 设置预充电周期
    OLED_Write_CMD(0x22); // 1/2
    OLED_Write_CMD(0xda); // 设置COM硬件引脚配置
    OLED_Write_CMD(0x12); // 设置硬件引脚配置
    OLED_Write_CMD(0xdb); // 设置VCOMH
    OLED_Write_CMD(0x20); // 设置VCOMH
    OLED_Write_CMD(0x8d); // 设置电荷泵
    OLED_Write_CMD(0x14); // 电荷泵
    OLED_Write_CMD(0xaf); // 开启显示
}

void OLED_Fill(u8 fill_Data) // 全屏填充
{
    u8 m, n;
    for (m = 0; m < 8; m++)
    {
        OLED_Write_CMD(0xb0 + m); // page0-page1
        OLED_Write_CMD(0x00);     // low column start address
        OLED_Write_CMD(0x10);     // high column start address
        for (n = 0; n < 128; n++)
        {
            OLED_Write_DAT(fill_Data);
        }
    }
}

void OLED_SetPos(u8 x, u8 y) // 设置起始点坐标
{
    OLED_Write_CMD(0xb0 + y);
    OLED_Write_CMD(((x & 0xf0) >> 4) | 0x10);
    OLED_Write_CMD((x & 0x0f) | 0x01);
}

void OLED_Show_String(u8 x, u8 y, u8 ch[], u8 TextSize)
{
    u8 c = 0, i = 0, j = 0;
    switch (TextSize)
    {
    case 1:
    {
        while (ch[j] != '\0')
        {
            c = ch[j] - 32;
            if (x > 126)
            {
                x = 0;
                y++;
            }
            OLED_SetPos(x, y);
            for (i = 0; i < 6; i++)
                OLED_Write_DAT(F6x8[c][i]);
            x += 6;
            j++;
        }
    }
    break;
    case 2:
    {
        while (ch[j] != '\0')
        {
            c = ch[j] - 32;
            if (x > 120)
            {
                x = 0;
                y++;
            }
            OLED_SetPos(x, y);
            for (i = 0; i < 8; i++)
                OLED_Write_DAT(F8X16[c * 16 + i]);
            OLED_SetPos(x, y + 1);
            for (i = 0; i < 8; i++)
                OLED_Write_DAT(F8X16[c * 16 + i + 8]);
            x += 8;
            j++;
        }
    }
    break;
    }
}
void OLED_ShowCN(u8 x, u8 y, u8 N)
{
    u8 wm = 0;
    unsigned int adder = 32 * N;
    OLED_SetPos(x, y);
    for (wm = 0; wm < 16; wm++)
    {
        OLED_Write_DAT(F16x16[adder]);
        adder += 1;
    }
    OLED_SetPos(x, y + 1);
    for (wm = 0; wm < 16; wm++)
    {
        OLED_Write_DAT(F16x16[adder]);
        adder += 1;
    }
}

void OLED_DrawBMP(u8 x0, u8 y0, u8 x1, u8 y1, u8 BMP[])
{
    unsigned int j = 0;
    u8 x, y;

    if (y1 % 8 == 0)
        y = y1 / 8;
    else
        y = y1 / 8 + 1;
    for (y = y0; y < y1; y++)
    {
        OLED_SetPos(x0, y);
        for (x = x0; x < x1; x++)
        {
            OLED_Write_DAT(BMP[j++]);
        }
    }
}

void OLED_ON()
{
    OLED_Write_CMD(0X8D); // 设置电荷泵
    OLED_Write_CMD(0X14); // 开启电荷泵
    OLED_Write_CMD(0XAF); // OLED唤醒
}

void OLED_OFF()
{
    OLED_Write_CMD(0X8D); // 设置电荷泵
    OLED_Write_CMD(0X10); // 关闭电荷泵
    OLED_Write_CMD(0XAE); // OLED休眠
}
