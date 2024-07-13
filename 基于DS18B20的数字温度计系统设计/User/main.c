#include "TM1637.h"
#include "USART.h"

/*----------------------------------------------------数码管段码----------------------------------------------------*/
/*共阴段码		0	  1	    2	  3	    4	  5	    6	  7	    8	  9	    C	  负号  灭  全亮  */
u8 sg[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x39, 0x40, 0, 0xFF};
u8 sg1[] = {0xBF, 0x86, 0xDB, 0xCF, 0xE6, 0xED, 0xFD, 0x87, 0xFF, 0xEF}; // 带点共阴段码0. 1. 2. 3. 4. 5. 6. 7. 8. 9.

int main(void)
{
    u8 Key = 0;
    USART_init(USART1, 9600, USART_WordLength_8b, USART_StopBits_1, USART_Parity_No, USART_Mode_Rx | USART_Mode_Tx, USART_HardwareFlowControl_None, ENABLE);
    USART_IT_Init(USART1, 0, 0, ENABLE);
    TM1637_Init();
    TM1637_Setluminance(1);
    Display_TM1637(1, sg1[8]);
    Display_TM1637(2, sg1[8]);
    Display_TM1637(3, sg1[8]);
    Display_TM1637(4, sg1[8]);
    while (1)
    {
        Key = Key_TM1637();
        switch (Key)
        {
        case NO_KEY_PRESSED:
            Display_TM1637(1, sg[0]);
            Display_TM1637(2, sg[0]);
            Display_TM1637(3, sg[0]);
            Display_TM1637(4, sg[0]);
            break;
        case K1_1:
            Display_TM1637(1, sg[1]);
            Display_TM1637(2, sg[1]);
            Display_TM1637(3, sg[1]);
            Display_TM1637(4, sg[1]);
            USART_SendData(USART1, Key);
            break;
        case K1_2:
            Display_TM1637(1, sg[2]);
            Display_TM1637(2, sg[2]);
            Display_TM1637(3, sg[2]);
            Display_TM1637(4, sg[2]);
            USART_SendData(USART1, Key);
            break;
        case K1_3:
            Display_TM1637(1, sg[3]);
            Display_TM1637(2, sg[3]);
            Display_TM1637(3, sg[3]);
            Display_TM1637(4, sg[3]);
            USART_SendData(USART1, Key);
            break;
        case K1_4:
            Display_TM1637(1, sg[4]);
            Display_TM1637(2, sg[4]);
            Display_TM1637(3, sg[4]);
            Display_TM1637(4, sg[4]);
            USART_SendData(USART1, Key);
            break;
        case K1_5:
            for (u8 i = 0; i < 10; i++)
            {
                Display_TM1637(1, sg1[i]);
                Display_TM1637(2, sg1[i]);
                Display_TM1637(3, sg1[i]);
                Display_TM1637(4, sg1[i]);
                delay_ms(500);
            }
            USART_SendData(USART1, Key);
            break;
        default:
            break;
        }
    }
}
