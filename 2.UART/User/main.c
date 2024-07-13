#include <stm32f10x.h>
#include "USART.h"

void USART3_IRQHandler(void)
{
    u8 dat;
    if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
    {
        dat = USART_ReceiveData(USART3);
        USART_SendData(USART3, dat);
        switch (dat)
        {
        case 1:
            GPIO_ResetBits(GPIOB, GPIO_Pin_0);
            USART_SendData(USART3, 0x01);
            break;
        case 2:
            GPIO_SetBits(GPIOB, GPIO_Pin_0);
            USART_SendData(USART3, 0x02);
            break;
        default:
            break;
        }
    }
}

int main(void)
{
    USART_init(USART3, 115200, USART_WordLength_8b, USART_StopBits_1, USART_Parity_No, USART_Mode_Rx | USART_Mode_Tx, USART_HardwareFlowControl_None, ENABLE);
    USART_IT_Init(USART3, 0, 0, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; // 50MHz
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; // 推挽输出
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    while (1)
    {
    }
}
