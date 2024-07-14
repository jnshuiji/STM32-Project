#include "USART.h"

/**
 * @description:
 * @param {USART_TypeDef} *USARTx:串口号
 *      @arg USART1
 *      @arg USART2
 *      @arg USART3
 * @param {u32} BaudRate:波特率
 *      @arg 9600
 *      @arg 38400
 *      @arg 115200
 * @param {u16} WordLength:数据位
 *      @arg USART_WordLength_8b
 *      @arg USART_WordLength_9b
 * @param {u16} StopBits:停止位
 *      @arg USART_StopBits_1
 *      @arg USART_StopBits_0_5
 *      @arg USART_StopBits_2
 *      @arg USART_StopBits_1_5
 * @param {u16} Parity:校验位
 *      @arg USART_Parity_No
 *      @arg USART_Parity_Even
 *      @arg USART_Parity_Odd
 * @param {u16} Mode:模式
 *      @arg USART_Mode_Rx
 *      @arg USART_Mode_Tx
 *      @arg USART_Mode_Rx | USART_Mode_Tx
 * @param {u16} HardwareFlowControl:硬件流控
 *      @arg USART_HardwareFlowControl_None
 *      @arg USART_HardwareFlowControl_RTS
 *      @arg USART_HardwareFlowControl_CTS
 *      @arg USART_HardwareFlowControl_RTS_CTS
 * @param {FunctionalState} NewState:使能状态
 *      @arg ENABLE
 *      @arg DISABLE
 * @return {*}
 */
void USART_init(USART_TypeDef *USARTx, u32 BaudRate, u16 WordLength, u16 StopBits, u16 Parity, u16 Mode, u16 HardwareFlowControl, FunctionalState NewState)
{
    /*--------------------------------------------------------------------------引脚-------------------------------------------------------------------------*/
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; // 50MHz

    if (USARTx == USART1)
    {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1, ENABLE);
        USART_DeInit(USART1);

        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; // 复用推挽输出
        GPIO_Init(GPIOA, &GPIO_InitStructure);

        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; // 浮空输入
        GPIO_Init(GPIOA, &GPIO_InitStructure);
    }
    else if (USARTx == USART2)
    {
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
        USART_DeInit(USART2);

        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; // 复用推挽输出
        GPIO_Init(GPIOA, &GPIO_InitStructure);

        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; // 浮空输入
        GPIO_Init(GPIOA, &GPIO_InitStructure);
    }
    else if (USARTx == USART3)
    {
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
        USART_DeInit(USART3);

        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; // 复用推挽输出
        GPIO_Init(GPIOB, &GPIO_InitStructure);

        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; // 浮空输入
        GPIO_Init(GPIOB, &GPIO_InitStructure);
    }

    /*--------------------------------------------------------------------------串口-------------------------------------------------------------------------*/
    USART_InitTypeDef USART_InitStructure;
    USART_InitStructure.USART_BaudRate = BaudRate;
    USART_InitStructure.USART_WordLength = WordLength;
    USART_InitStructure.USART_StopBits = StopBits;
    USART_InitStructure.USART_Parity = Parity;
    USART_InitStructure.USART_Mode = Mode;
    USART_InitStructure.USART_HardwareFlowControl = HardwareFlowControl;
    USART_Init(USARTx, &USART_InitStructure);
    USART_Cmd(USARTx, NewState);
}

/**
 * @description:
 * @param {USART_TypeDef} *USARTx   串口号
 *      @arg USART1
 *      @arg USART2
 *      @arg USART3
 * @param {u8} PreemptionPriority   抢占优先级
 * @param {u8} SubPriority        响应优先级
 * @param {FunctionalState} NewState    使能状态
 *      @arg ENABLE
 *      @arg DISABLE
 * @return {*}
 */
void USART_IT_Init(USART_TypeDef *USARTx, u8 PreemptionPriority, u8 SubPriority, FunctionalState NewState)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    if (USARTx == USART1)
        NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    else if (USARTx == USART2)
        NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    else if (USARTx == USART3)
        NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = PreemptionPriority;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = SubPriority;
    NVIC_InitStructure.NVIC_IRQChannelCmd = NewState; // 使能中断
    NVIC_Init(&NVIC_InitStructure);                   // 初始化NVIC
    USART_ITConfig(USARTx, USART_IT_RXNE, NewState);  // 使能接收中断
}