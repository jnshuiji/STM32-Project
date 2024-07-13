#include "TM1637.h"
#include "USART.h"
#include "DS18B20.h"

u8 tens_unit = 0;            // 十分位
u8 unit = 0;                 // 个位
u8 tens_digit = 0;           // 十位
u8 Key = 0;                  // 按键值
u8 precision = precision_12; // 精度

/*----------------------------------------------------数码管段码----------------------------------------------------*/
/*共阴段码		0	  1	    2	  3	    4	  5	    6	  7	    8	  9	    C	  负号  灭  全亮  */
u8 sg[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x39, 0x40, 0, 0xFF};
u8 sg1[] = {0xBF, 0x86, 0xDB, 0xCF, 0xE6, 0xED, 0xFD, 0x87, 0xFF, 0xEF}; // 带点共阴段码0. 1. 2. 3. 4. 5. 6. 7. 8. 9.

s8 displaytemp();
s8 set_limit(s8 TH_or_TL);

int main(void)
{
    s8 TH = 0, TL = 0, tempreatuer = 0;
    USART_init(USART1, 9600, USART_WordLength_8b, USART_StopBits_1, USART_Parity_No, USART_Mode_Rx | USART_Mode_Tx, USART_HardwareFlowControl_None, ENABLE);
    USART_IT_Init(USART1, 0, 0, ENABLE);
    TM1637_Init();
    DS18B20_Init();

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_SetBits(GPIOA, GPIO_Pin_7);
    while (DS18B20_Reset())
    {
        GPIO_ResetBits(GPIOA, GPIO_Pin_7);
        delay_ms(1000);
        GPIO_SetBits(GPIOA, GPIO_Pin_7);
        delay_ms(1000);
    }

    // 开局一读取，获取DS18B20的EEPROM中储存的温度上下限
    TH = DS18B20_Read_EEPROM() >> 8;
    TL = DS18B20_Read_EEPROM();

    TM1637_Setluminance(1);
    while (1)
    {
        Key = Key_TM1637();
        switch (Key)
        {
        case K1_1:
            /*--------------------------------------------------
                                设置温度上限
                        短按K1两秒以内进入温度上限设置模式
                          进入此模式后各按键功能如下：
                K1:转为正数，设置范围：0~99
                K2:转为负数，设置范围：-49~0
                K3:十位循环加一，范围（0不显示）：负数是0~4；正数是0~9
                K4:个位循环加一，范围：0~9
                K5:设置结束，将数据存入DS18B20的EEPROM中
            --------------------------------------------------*/
            TH = DS18B20_Read_EEPROM() >> 8;
            TH = set_limit(TH);
            DS18B20_Write_Scratchpad_EEPROM(TH, TL, precision);
            break;
            /*--------------------------------------------------
                                设置温度下限
                            按K2进入温度下限设置模式
                            进入此模式后各按键功能如下：
                K1:转为正数，设置范围：0~99
                K2:转为负数，设置范围：-49~0
                K3:十位循环加一，范围（0不显示）：负数是0~4；正数是0~9
                K4:个位循环加一，范围：0~9
                K5:设置结束，将数据存入DS18B20的EEPROM中
            --------------------------------------------------*/
        case K1_2:
            // TL = Read_EEPROM_DS18B20();
            // TL = set_limit(TL);
            // Write_Scratchpad_EEPROM_DS18B20(TH, TL, precision);
            // break;
        default:
            tempreatuer = displaytemp();
            break;
        }
        // 如果温度低于下限或者高于上限，警报一直响
        if (tempreatuer >= TH || tempreatuer <= TL)
            GPIO_ResetBits(GPIOA, GPIO_Pin_7);
        else
            GPIO_SetBits(GPIOA, GPIO_Pin_7);
    }
}

s8 displaytemp()
{
    s8 tempreatuer = 0;
    s16 original = DS18B20_Read_Temp();
    if (original < 0) // 负
    {
        unit = ((~original + 1) >> 4) % 10;            // 个位
        tens_digit = ((~original + 1) >> 4) / 10 % 10; // 十位
        tempreatuer = ~(tens_digit * 10 + unit) + 1;   // 负数原码取反加一得补码
        TM1637_Display(3, sg[unit]);
        if (tens_digit)
        {
            TM1637_Display(2, sg[tens_digit]);
            TM1637_Display(1, sg[11]);
        }
        else
        {
            TM1637_Display(2, sg[11]);
            TM1637_Display(1, sg[12]);
        }
    }
    else // 正
    {
        tens_unit = (0x000F & original) * 625 / 1000 % 10; // 十分位
        unit = original / 16 % 10;                         // 个位
        tens_digit = original / 160 % 10;                  // 十位
        tempreatuer = tens_digit * 10 + unit;              // 正数原码补码一致
        TM1637_Display(3, sg[tens_unit]);
        TM1637_Display(2, sg1[unit]);
        if (tens_digit)
            TM1637_Display(1, sg[tens_digit]);
        else
            TM1637_Display(1, sg[12]);
    }
    TM1637_Display(4, sg[10]); // 单位摄氏度C
    delay_ms(5);
    return tempreatuer;
}

s8 set_limit(s8 TH_or_TL)
{
    u8 mode1 = 0; // 符号位，1为负数，0为正数
    TM1637_Display(4, sg[10]);
    if (TH_or_TL < 0)
    {
        mode1 = 1;
        unit = (~TH_or_TL + 1) % 10;
        tens_digit = (~TH_or_TL + 1) / 10 % 10;
    }
    else
    {
        mode1 = 0;
        unit = TH_or_TL % 10;
        tens_digit = TH_or_TL / 10 % 10;
    }
    while (Key != K1_5)
    {
        if (mode1)
        {
            if (tens_digit > 4)
                tens_digit = 4;
            Key = Key_TM1637();
            while ((Key != K1_1) && (Key != K1_5))
            {
                if (tens_digit)
                {
                    TM1637_Display(1, sg[11]);
                    TM1637_Display(2, sg[tens_digit]);
                    TM1637_Display(3, sg[unit]);
                }
                else
                {
                    TM1637_Display(1, sg[12]);
                    TM1637_Display(2, sg[11]);
                    TM1637_Display(3, sg[unit]);
                }
                delay_ms(2);
                Key = Key_TM1637();
                while (Key_TM1637() != NO_KEY_PRESSED)
                    delay_ms(2);
                if (Key == K1_3)
                {
                    if (tens_digit == 4)
                        tens_digit = 0;
                    else
                        tens_digit++;
                }
                if (Key == K1_4)
                {
                    if (unit == 9)
                        unit = 0;
                    else
                        unit++;
                }
            }
            if (Key == K1_1)
                mode1 = 0;
        }
        if (!mode1)
        {
            while ((Key != K1_2) && (Key != K1_5))
            {
                if (tens_digit)
                {
                    TM1637_Display(2, sg[tens_digit]);
                    TM1637_Display(3, sg[unit]);
                }
                else
                {
                    TM1637_Display(2, sg[12]);
                    TM1637_Display(3, sg[unit]);
                }
                TM1637_Display(1, sg[12]);
                delay_ms(2);
                Key = Key_TM1637();
                while (Key_TM1637() != NO_KEY_PRESSED)
                    delay_ms(2);
                if (Key == K1_3)
                {
                    if (tens_digit == 9)
                        tens_digit = 0;
                    else
                        tens_digit++;
                }
                if (Key == K1_4)
                {
                    if (unit == 9)
                        unit = 0;
                    else
                        unit++;
                }
            }
            if (Key == K1_2)
                mode1 = 1;
        }
    }
    TH_or_TL = tens_digit * 10 + unit;
    if (mode1)
        TH_or_TL = ~TH_or_TL + 1; // 转补码
    return TH_or_TL;
}
