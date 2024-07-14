#include "OLED.h"
#include "delay.h"

int main(void)
{
    u8 i = 0;
    I2C_My_Init(I2C1);
    OLED_Init();
    while (1)
    {
        OLED_Fill(0xFF); // 全屏点亮
        delay_ms(2000);
        OLED_Fill(0x00); // 全屏灭
        delay_ms(2000);
        for (i = 0; i < 5; i++)
        {
            OLED_ShowCN(22 + i * 16, 0, i); // 测试显示中文
        }
        delay_ms(2000);
        OLED_Show_String(0, 3, "HelTec Automation", 1); // 测试6*8字符
        OLED_Show_String(0, 4, "Hello Tech", 2);        // 测试8*16字符
        delay_ms(2000);
        OLED_Fill(0x00); // 全屏灭
    }
}
